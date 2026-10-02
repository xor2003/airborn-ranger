/* DOS int 21h services on the host filesystem.
 * Register-level semantics matching the original calls used by the game:
 * filenames are ASCIZ in mem[] at ds:dx, handles are small ints, CF=error.
 */
#include "rt.h"
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>
#include <time.h>

#define MAXH 16
static FILE *htab[MAXH];
static dd dta;                 /* disk transfer area (linear) */
static dd vectors[256];        /* int vectors: para<<16|off */
static dd alloc_next = 0xa0000; /* nothing free until the game SETBLOCKs its own block */
dd tnd_base = 0, tnd_cbase = 0; dw tnd_cseg = 0;   /* TANDYSND overlay mapping */
static db verify;

static dd lin(dw s, dw o){ return ((dd)s<<4) + o; }

static FILE *hget(int h){ return (h>0 && h<MAXH) ? htab[h] : NULL; }
static int hnew(void){ for(int i=1;i<MAXH;i++) if(!htab[i]) return i; return -1; }

static char *asciz(dw s, dw o, char *buf, size_t n){
    db *p = raddr_(s,o); size_t i=0;
    while (i+1<n && p[i]) { buf[i]=p[i]; i++; }
    buf[i]=0; return buf;
}

void dos_open(void){           /* AH=3Dh AL=mode DS:DX=path -> AX=handle */
    char path[256]; asciz(ds,dx,path,sizeof path);
    if(rt_trace) fprintf(stderr,"  open '%s' ds:dx=%x:%x\n", path, ds, dx);
    FILE *f = fopen(path, "rb");
    if(!f){ CF=1; ax=2; return; }              /* file not found */
    int h = hnew(); if(h<0){ fclose(f); CF=1; ax=4; return; }
    htab[h]=f; CF=0; ax=h;
}
void dos_create_new(void){     /* AH=3Ch/5Bh DS:DX=path -> AX=handle */
    char path[256]; asciz(ds,dx,path,sizeof path);
    FILE *f = fopen(path,"wb+");
    if(!f){ CF=1; ax=5; return; }
    int h = hnew(); if(h<0){ fclose(f); CF=1; ax=4; return; }
    htab[h]=f; CF=0; ax=h;
}
void dos_close(void){          /* AH=3Eh BX=handle */
    FILE *f = hget(bx); if(!f){ CF=1; ax=6; return; }
    fclose(f); htab[bx]=NULL; CF=0;
}
void dos_read(void){           /* AH=3Fh BX=h CX=n DS:DX=buf -> AX=bytes */
    FILE *f = hget(bx); if(!f){ CF=1; ax=6; return; }
    size_t n = fread(raddr_(ds,dx), 1, cx, f);
    if(rt_trace) fprintf(stderr,"  read h=%x cx=%x -> %x:%x got %zx\n", bx, cx, ds, dx, n);
    CF = ferror(f) ? 1 : 0; ax = (dw)n;
}
void dos_write(void){          /* AH=40h */
    FILE *f = hget(bx); if(!f){ CF=1; ax=6; return; }
    size_t n = fwrite(raddr_(ds,dx), 1, cx, f);
    CF = ferror(f) ? 1 : 0; ax = (dw)n;
}
void dos_seek(void){           /* AH=42h AL=origin BX=h CX:DX=off -> DX:AX */
    FILE *f = hget(bx); if(!f){ CF=1; ax=6; return; }
    int w = al==0?SEEK_SET : al==1?SEEK_CUR : SEEK_END;
    long off = ((dd)cx<<16)|dx;
    if(fseek(f, off, w)){ CF=1; ax=1; return; }
    long pos = ftell(f); ax=pos&0xffff; dx=(pos>>16)&0xffff; CF=0;
}
void dos_delete(void){         /* AH=41h */
    char path[256]; asciz(ds,dx,path,sizeof path);
    CF = remove(path) ? 1 : 0; if(CF) ax=2;
}
void dos_rename(void){         /* AH=56h DS:DX=old ES:DI=new */
    char o[256],n[256]; asciz(ds,dx,o,sizeof o); asciz(es,di,n,sizeof n);
    CF = rename(o,n) ? 1 : 0; if(CF) ax=2;
}
void dos_get_attr(void){       /* AH=43h AL=0 -> CX=attr */
    char path[256]; asciz(ds,dx,path,sizeof path);
    struct stat st; if(stat(path,&st)){ CF=1; ax=2; return; }
    cx=0; CF=0;
}
void dos_file_datetime(void){  /* AH=57h AL=0 BX=h -> CX=time DX=date */
    FILE *f = hget(bx); if(!f){ CF=1; ax=6; return; }
    struct stat st; fstat(fileno(f),&st);
    struct tm *t = localtime(&st.st_mtime);
    dx = ((t->tm_year+1900-1980)<<9)|((t->tm_mon+1)<<5)|t->tm_mday;
    cx = (t->tm_hour<<11)|(t->tm_min<<5)|(t->tm_sec/2); CF=0;
}
void dos_set_dta(void){ dta = lin(ds,dx); }
/* find_first/next: 43-byte DOS DTA — we store a DIR handle in reserved area */
typedef struct { DIR *d; char pat[256]; } FindCtx;
static FindCtx fctx;
static void dta_put(const char *name){
    db *b = &mem[dta];
    memset(b, 0, 43);
    b[21] = 0x20;                     /* attr */
    b[30] = 0;                        /* name at 30..42 */
    strncpy((char*)b+30, name, 13);
}
static int match_pat(const char *name, const char *pat){
    /* DOS-style * and ? */
    while (*pat){
        if (*pat=='*'){ pat++; if(!*pat) return 1;
            for(const char *s=name;;s++){ if(match_pat(s,pat)) return 1; if(!*s) return 0; } }
        if (!*name) return 0;
        if (*pat!='?' && tolower(*pat)!=tolower(*name)) return 0;
        pat++; name++;
    }
    return !*name;
}
void dos_find_first(void){     /* AH=4Eh DS:DX=pat CX=attr */
    char pat[256], dirbuf[256]; asciz(ds,dx,pat,sizeof pat);
    if(fctx.d) closedir(fctx.d);
    strcpy(fctx.pat, pat);
    char *slash = strrchr(fctx.pat, '/');
    const char *dir = "."; const char *mask = fctx.pat;
    if (slash){ *slash = 0; dir = fctx.pat; mask = slash+1; }
    snprintf(dirbuf, sizeof dirbuf, "%s", dir);       /* dir aliases fctx.pat */
    memmove(fctx.pat, mask, strlen(mask)+1);          /* keep mask only */
    fctx.d = opendir(dirbuf);
    if(!fctx.d){ CF=1; ax=18; return; }
    struct dirent *e;
    while((e = readdir(fctx.d))){ if(match_pat(e->d_name, fctx.pat)){ dta_put(e->d_name); CF=0; return; } }
    CF=1; ax=18;
}
void dos_find_next(void){      /* AH=4Fh */
    if(!fctx.d){ CF=1; ax=18; return; }
    struct dirent *e;
    while((e = readdir(fctx.d))){ if(match_pat(e->d_name, fctx.pat)){ dta_put(e->d_name); CF=0; return; } }
    CF=1; ax=18;
}
/* minimal MCB substitute: a bump frontier + free-list of resized/freed tails */
static struct blk { dd base, size; } ablk[64], fblk[64];
static int nablk, nfblk;

void dos_alloc(void){          /* AH=48h BX=paras -> AX=seg; CF+BX=max on fail */
    dd avail = (0xa0000 - alloc_next) >> 4;   /* conventional memory ends at A000 */
    dd bf = 0; int bi = -1;
    for (int i = 0; i < nfblk; i++)
        if ((fblk[i].size >> 4) > bf) { bf = fblk[i].size >> 4; bi = i; }
    dd maxp = avail > bf ? avail : bf;
    if ((dd)bx > maxp){ CF=1; bx = (dw)maxp; return; }
    if (bi >= 0 && (fblk[bi].size >> 4) >= (dd)bx){
        dd a = fblk[bi].base;
        if ((fblk[bi].size >> 4) > (dd)bx){ fblk[bi].base += (dd)bx<<4; fblk[bi].size -= (dd)bx<<4; }
        else fblk[bi] = fblk[--nfblk];
        ablk[nablk++] = (struct blk){ a, (dd)bx << 4 };
        ax = a >> 4; CF=0; return;
    }
    dd a = alloc_next; alloc_next += (dd)bx<<4;
    ablk[nablk++] = (struct blk){ a, (dd)bx << 4 };
    if (getenv("M2C_MEMTRACE")) fprintf(stderr,"alloc %x paras -> %x:%x\n", bx, a>>4, a + ((dd)bx<<4));
    ax = a >> 4; CF=0;
}
void dos_free(void){           /* AH=49h ES=seg */
    dd b = (dd)es << 4;
    if (getenv("M2C_MEMTRACE")) fprintf(stderr,"free %x\n", b);
    for (int i = 0; i < nablk; i++)
        if (ablk[i].base == b){ fblk[nfblk++] = ablk[i]; ablk[i] = ablk[--nablk]; break; }
    CF=0;
}
void dos_resize(void){         /* AH=4Ah ES=seg BX=new paras */
    dd b = (dd)es << 4, ns = (dd)bx << 4;
    for (int i = 0; i < nablk; i++)
        if (ablk[i].base == b){
            if (ablk[i].size > ns){
                if (b + ablk[i].size == alloc_next) alloc_next = b + ns;  /* top block: pull frontier back */
                else fblk[nfblk++] = (struct blk){ b + ns, ablk[i].size - ns };
            }
            ablk[i].size = ns; CF=0; return;
        }
    if (b + ns < alloc_next) alloc_next = b + ns;  /* SETBLOCK on the loader's block opens free space */
    if (getenv("M2C_MEMTRACE")) fprintf(stderr,"setblock %x -> %x paras\n", b, bx);
    CF=0;
}
void dos_exec(void){           /* AH=4Bh AL=03h — overlay load (TANDYSND.EXE) */
    if (al != 3){ CF=1; ax=1; return; }
    dw loadseg = *(dw*)raddr_(es,bx);
    dw relf    = *(dw*)raddr_(es,bx+2);
    char path[256]; asciz(ds,dx,path,sizeof path);
    if (getenv("M2C_EXECTRACE"))
        fprintf(stderr,"EXEC try '%s' ds=%x es=%x bx=%x loadseg=%x relf=%x\n",
                path, ds, es, bx, loadseg, relf);
    FILE *f = fopen(path,"rb");
    if (!f){ CF=1; ax=2; return; }
    db hdr[0x20];
    if (fread(hdr,1,0x20,f)!=0x20 || hdr[0]!='M' || (hdr[1]!='Z'&&hdr[1]!='M')){ fclose(f); CF=1; ax=8; return; }
    dw hsize = *(dw*)(hdr+8)*16, nreloc = *(dw*)(hdr+6), reloff = *(dw*)(hdr+0x18);
    fseek(f,0,SEEK_END); long fsz = ftell(f);
    if (hsize >= fsz){ fclose(f); CF=1; ax=8; return; }
    fseek(f,hsize,SEEK_SET);
    db *base = &mem[(dd)loadseg<<4];
    if (fread(base,1,fsz-hsize,f)!=(size_t)(fsz-hsize)){ fclose(f); CF=1; ax=8; return; }
    for (unsigned i=0;i<nreloc;i++){
        db rent[4];
        fseek(f,reloff+i*4,SEEK_SET);
        if (fread(rent,1,4,f)!=4){ fclose(f); CF=1; ax=8; return; }
        dw roff=*(dw*)rent, rseg=*(dw*)(rent+2);
        *(dw*)&mem[((dd)(loadseg+rseg)<<4)+roff] += relf;
    }
    fclose(f);
    /* record overlay mapping: code seg para is the relocated word at +0x18
     * (raw 7 paras => image offset 0x70) used by lifted procs' push cs */
    tnd_base = (dd)loadseg << 4;
    tnd_cseg = *(dw*)&mem[tnd_base + 0x18];
    tnd_cbase = ((dd)tnd_cseg) << 4;
    fprintf(stderr,"EXEC overlay %s -> seg %x (%ld bytes, %d relocs) cseg %x tbl@380: %x %x %x %x\n", path, loadseg, fsz-hsize, nreloc, tnd_cseg,
            *(dw*)&mem[tnd_base+0x380], *(dw*)&mem[tnd_base+0x382], *(dw*)&mem[tnd_base+0x384], *(dw*)&mem[tnd_base+0x386]);
    CF=0;
}
void dos_set_int_vector(void){ *(dw*)&mem[al*4] = dx; *(dw*)&mem[al*4+2] = ds; }
void dos_get_int_vector(void){ es=*(dw*)&mem[al*4+2]; bx=*(dw*)&mem[al*4]; }
void dos_get_psp(void){ bx = 0x1a2; }             /* fake PSP para */
void dos_get_version(void){ ax=0x0500; bx=cx=0; } /* DOS 5.0 */
void dos_get_drive(void){ al=2; }                 /* C: */
void dos_get_verify(void){ al=verify; }
void dos_get_date(void){ time_t t=time(0); struct tm *l=localtime(&t);
    cx=1900+l->tm_year; dx=((l->tm_mon+1)<<8)|l->tm_mday; }
void dos_get_time(void){ time_t t=time(0); struct tm *l=localtime(&t);
    cx=(l->tm_hour<<8)|l->tm_min; dx=(l->tm_sec<<8); }
void dos_ioctl(void){ CF=1; }                     /* 44h — unsupported */
void dos_exit_code(void){ rt_exit(al); }
void dos_exit(void){ rt_exit(0); }
void dos_int21(dw a){ (void)a; }                  /* unmodeled AH */

/* console I/O — routed to the emulated text screen at the BIOS cursor */
void text_putc(db);
void dos_write_char(void){ text_putc(dl); }
void dos_print_string(void){ db *p=raddr_(ds,dx); while(*p!='$') text_putc(*p++); }
void dos_read_char_echo(void){ int c=getchar(); putchar(c); al=c; }
void dos_read_char_noecho(void){ al=getchar(); }
void dos_direct_io(void){ al=0; ZF=1; }
void dos_read_string(void){ /* AH=0Ah buffered input — unused by game */ }
void dos_check_stdin(void){ al=0xff; }            /* always ready */
void dos_create_temp(void){ CF=1; }

/* ISR dispatch for vectors the game installs (int9 kbd, int1c/8 timer).
 * The game patches the IVT in low memory directly (es:[n*4]), so read it
 * from mem[] rather than a side table. */
void rt_timer_tick(void);

void rt_call_vector(int n){
    dw off = *(dw*)&mem[n*4], seg = *(dw*)&mem[n*4+2];
    if (!off && !seg) return;
    dd lin = ((dd)seg<<4) + off;
    if (n == 0x1c && lin == 0x2ef6){ rt_timer_tick(); return; } /* seg000:14D6 */
    vfn f = func_at(lin);
    if (f) f();
}

/* BIOS int8 (IRQ0) replica: bump the BDA tick count and chain to int1c,
 * which runs the game's user-tick handler. TANDYSND's int8 chains here
 * (via its saved dword_103DE) every 3 ticks while music plays (~20 Hz). */
void rt_bios_int8(void){
    ++rt_i8_cnt;
    ++*(dd*)&mem[0x46c];                        /* BIOS_DATA tick count */
    rt_call_vector(0x1c);
}

/* int 1Ch timer-tick ISR the game installs at seg000:14D6 (unlabeled code —
 * no lifted function exists for it): six countdown timers + 32-bit tick count. */
void rt_timer_tick(void){
    ++rt_1c_cnt;
    if (*(dw*)&mem[0xf36d]) --*(dw*)&mem[0xf36d];   /* word_1D94D */
    if (*(dw*)&mem[0xf36f]) --*(dw*)&mem[0xf36f];   /* word_1D94F */
    if (*(dw*)&mem[0xf371]) --*(dw*)&mem[0xf371];   /* word_1D951 */
    if (*(dw*)&mem[0xf373]) --*(dw*)&mem[0xf373];   /* word_1D953 */
    if (*(dw*)&mem[0xf375]) --*(dw*)&mem[0xf375];   /* word_1D955 */
    if (*(dw*)&mem[0x1b00b]) --*(dw*)&mem[0x1b00b]; /* word_295EB */
    if (++*(dw*)&mem[0xf102] == 0) ++*(dw*)&mem[0xf104]; /* word_1D6E2:1D6E4 */
}
