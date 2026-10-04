/* unit tests for dos.c: file ops, find patterns, memory alloc, int vectors.
 * includes dos.c directly to reach static helpers (asciz, match_pat). */
#include "../rt.h"
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

static char tbuf[256]; static int tn;    /* text_putc capture */
void text_putc(db c){ if (tn < (int)sizeof tbuf - 1) tbuf[tn++] = (char)c; }
dw rt_in(dw p){ (void)p; return 0; }
void rt_out(dw p, dw v){ (void)p; (void)v; }
vfn func_at(dd a){ (void)a; return 0; }

#include "../dos.c"

static int fails, checks;
#define CHECK(cond) do{ checks++; if(!(cond)){ fails++; \
    fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); } }while(0)

static void put_asciz(dw seg, dw off, const char *s){
    strcpy((char*)raddr_(seg,off), s);
}

int main(void){
    /* --- asciz --- */
    char buf[64];
    put_asciz(0x100, 0x20, "FOO.DTX");
    CHECK(!strcmp(asciz(0x100,0x20,buf,sizeof buf), "FOO.DTX"));

    /* empty path */
    put_asciz(0x100, 0x20, "");
    CHECK(strcmp(asciz(0x100,0x20,buf,sizeof buf), "") == 0);

    /* path with special chars */
    put_asciz(0x100, 0x20, "TEST.FIL.E");
    CHECK(!strcmp(asciz(0x100,0x20,buf,sizeof buf), "TEST.FIL.E"));

    /* --- open/read/seek/close on a temp fixture --- */
    const char *tp = "/tmp/ar_t_dos.bin";
    FILE *fx = fopen(tp, "wb");
    for (int i=0;i<64;i++) fputc(i, fx);
    fclose(fx);

    ds = 0x100; dx = 0x20;
    put_asciz(ds, dx, tp);
    dos_open();
    CHECK(CF == 0); CHECK(ax > 0 && ax < MAXH);
    dw h = ax;

    ds = 0x200; dx = 0x100; cx = 16; bx = h;
    dos_read();
    CHECK(CF == 0); CHECK(ax == 16);
    CHECK(mem_b(0x2100) == 0 && mem_b(0x210f) == 15);   /* ds:dx = 0x200:0x100 */

    al = 0; cx = 0; dx = 32; bx = h;      /* SEEK_SET 32 */
    dos_seek();
    CHECK(CF == 0); CHECK(ax == 32 && dx == 0);
    ds = 0x200; dx = 0x100; cx = 4;
    dos_read();
    CHECK(ax == 4); CHECK(mem_b(0x2100) == 32);

    al = 2; cx = 0xffff; dx = 0xfff0;     /* SEEK_END -16 */
    dos_seek();
    CHECK(CF == 0); CHECK(ax == 48);
    /* SEEK_CUR: 48 + 4 -> 52 */
    al = 1; cx = 0; dx = 4; bx = h;
    dos_seek();
    CHECK(CF == 0); CHECK(ax == 52 && dx == 0);

    /* --- write/create_new/delete/rename against a scratch file --- */
    bx = h; dos_close();
    CHECK(CF == 0);
    const char *wp = "/tmp/ar_t_dos_w.bin";
    remove(wp); remove("/tmp/ar_t_dos_w2.bin");
    ds = 0x100; dx = 0x20;
    put_asciz(ds, dx, wp);
    dos_create_new();
    CHECK(CF == 0); CHECK(ax > 0 && ax < MAXH);
    h = ax;
    mem_ww(0x2300, 0xbeef); mem_ww(0x2302, 0xdead);
    ds = 0x200; dx = 0x300; cx = 4; bx = h;
    dos_write();
    CHECK(CF == 0); CHECK(ax == 4);
    bx = h; dos_file_datetime();            /* packed DOS date/time */
    CHECK(CF == 0);
    {   time_t now = time(NULL);
        int yr = localtime(&now)->tm_year + 1900;
        CHECK((dx >> 9) == (dw)(yr - 1980));      /* year field matches now */
    }
    bx = h; dos_close();
    CHECK(CF == 0);
    FILE *vf = fopen(wp, "rb");
    CHECK(vf != NULL);
    CHECK(fgetc(vf) == 0xef && fgetc(vf) == 0xbe);
    CHECK(fgetc(vf) == 0xad && fgetc(vf) == 0xde);
    fclose(vf);
    ds = 0x100; dx = 0x20;                  /* get_attr on it */
    put_asciz(ds, dx, wp);
    dos_get_attr();
    CHECK(CF == 0);
    es = 0x100; di = 0x40;                  /* rename to w2 */
    put_asciz(es, di, "/tmp/ar_t_dos_w2.bin");
    dos_rename();
    CHECK(CF == 0);
    CHECK(access(wp, F_OK) != 0);
    ds = 0x100; dx = 0x40;                  /* delete the renamed file */
    put_asciz(ds, dx, "/tmp/ar_t_dos_w2.bin");
    dos_delete();
    CHECK(CF == 0);
    CHECK(access("/tmp/ar_t_dos_w2.bin", F_OK) != 0);
    dos_delete();                           /* already gone -> CF+ax=2 */
    CHECK(CF == 1); CHECK(ax == 2);

    /* bad-handle errors */
    bx = 0x7f; dos_read();   CHECK(CF == 1); CHECK(ax == 6);
    bx = 0x7f; dos_seek();   CHECK(CF == 1); CHECK(ax == 6);
    bx = 0x7f; dos_close();  CHECK(CF == 1); CHECK(ax == 6);
    bx = 0x7f; dos_file_datetime(); CHECK(CF == 1); CHECK(ax == 6);

    /* open failure: CF + ax=2 */
    ds = 0x100; put_asciz(ds, dx, "/tmp/definitely_not_here.xyz");
    dos_open();
    CHECK(CF == 1); CHECK(ax == 2);

    /* --- match_pat (DOS wildcards) --- */
    CHECK(match_pat("FOO.DTX", "FOO.DTX"));
    CHECK(match_pat("FOO.DTX", "*.DTX"));
    CHECK(match_pat("FOO.DTX", "FOO.*"));
    CHECK(match_pat("FOO.DTX", "F?O.DTX"));
    CHECK(!match_pat("FOO.DTX", "*.DAT"));
    CHECK(match_pat("FOO.DTX", "*.*"));
    CHECK(!match_pat("FOODTX", "FOO.DTX"));
    CHECK(match_pat("a.dtx", "*.DTX"));            /* case-insensitive */
    CHECK(!match_pat("FOO.DTXX", "*.DTX"));
    CHECK(match_pat("", ""));                        /* empty pattern matches empty name */

    /* --- find_first/find_next against a fixture dir --- */
    mkdir("/tmp/ar_ff", 0755);
    fclose(fopen("/tmp/ar_ff/ONE.DTX","w"));
    fclose(fopen("/tmp/ar_ff/TWO.DTX","w"));
    fclose(fopen("/tmp/ar_ff/THREE.DAT","w"));
    ds = 0x100; dx = 0x20;
    put_asciz(ds, dx, "/tmp/ar_ff/*.DTX");
    dx = 0x800; dos_set_dta();                     /* DTA at 0x100:0x800 */
    dx = 0x20;
    dos_find_first();
    CHECK(CF == 0);
    const char *n1 = (char*)&mem[dta + 30];
    CHECK(match_pat(n1, "*.DTX"));
    int found = 1;
    while (1){
        dos_find_next();
        if (CF) break;
        found++;
        CHECK(match_pat((char*)&mem[dta + 30], "*.DTX"));
    }
    CHECK(found == 2);

    /* find nothing */
    ds = 0x100; dx = 0x20;
    put_asciz(ds, dx, "/tmp/ar_ff/NONEXIST.DTX");
    dos_find_first();
    CHECK(CF == 1); CHECK(ax == 18);

    /* pattern without a directory prefix -> searches "." (chdir into fixture) */
    {   char oldcwd[512];
        CHECK(getcwd(oldcwd, sizeof oldcwd) != NULL);
        CHECK(chdir("/tmp/ar_ff") == 0);
        ds = 0x100; dx = 0x20;
        put_asciz(ds, dx, "*.DAT");
        dos_find_first();
        CHECK(CF == 0);
        CHECK(!strcmp((char*)&mem[dta + 30], "THREE.DAT"));
        CHECK(chdir(oldcwd) == 0);
    }

    /* --- int vectors --- */
    al = 0x1c; dx = 0x14d6; ds = 0x1a2;
    dos_set_int_vector();
    CHECK(*(dw*)&mem[0x1c*4] == 0x14d6 && *(dw*)&mem[0x1c*4+2] == 0x1a2);
    dos_get_int_vector();
    CHECK(es == 0x1a2 && bx == 0x14d6);

    /* --- memory allocation --- */
    nablk = nfblk = 0; alloc_next = 0x80000;
    bx = 0x100; dos_alloc();                       /* 0x100 paras */
    CHECK(CF == 0); CHECK(ax == 0x8000);
    dw seg1 = ax;
    bx = 0x100; dos_alloc();
    CHECK(ax == 0x8100);
    bx = 0x2000; dos_alloc();                      /* too big: 0x1e00 avail */
    CHECK(CF == 1); CHECK(bx == 0x1e00);
    es = seg1; dos_free();                          /* free first -> free list */
    bx = 0x80; dos_alloc();                         /* reuses freed block */
    CHECK(CF == 0); CHECK(ax == 0x8000);
    bx = 0x80; dos_alloc();                         /* leftover half */
    CHECK(ax == 0x8080);

    /* resize shrink returns tail to free list / frontier */
    bx = 0x40; es = 0x8080; dos_resize();
    CHECK(CF == 0);
    bx = 0xc0; dos_alloc();                         /* 0x40c0 fits freed space */
    CHECK(CF == 0);

    /* --- dos_exec overlay load: minimal MZ fixture with one relocation --- */
    {   unsigned char mz[64] = {0};
        mz[0] = 'M'; mz[1] = 'Z';
        *(dw*)(mz + 8)    = 2;                /* hdr size = 2 paras = 32B */
        *(dw*)(mz + 6)    = 1;                /* 1 relocation entry */
        *(dw*)(mz + 0x18) = 0x1c;             /* reloc table offset */
        *(dw*)(mz + 0x1c) = 0x0004;           /* entry: seg 0, off 4 */
        *(dw*)(mz + 0x1e) = 0x0000;
        *(dw*)(mz + 0x20 + 0x18) = 0x0300;    /* body: code-seg word at +0x18 */
        *(dw*)(mz + 0x20 + 4)    = 0x0007;    /* body: reloc target word */
        const char *mp = "/tmp/ar_t_exec.exe";
        FILE *ef = fopen(mp, "wb"); fwrite(mz, 1, sizeof mz, ef); fclose(ef);
        memset(&mem[0x70000], 0, 0x100);
        ds = 0x100; dx = 0x20; put_asciz(ds, dx, mp);
        es = 0x200; bx = 0x40;                /* param block: loadseg, relf */
        mem_ww(0x2040, 0x7000);               /* es:bx   -> load seg */
        mem_ww(0x2042, 0x1000);               /* es:bx+2 -> reloc factor */
        al = 3; dos_exec();
        CHECK(CF == 0);
        CHECK(tnd_base == 0x70000);
        CHECK(*(dw*)&mem[0x70004] == 0x0007 + 0x1000);    /* relocation applied */
        CHECK(tnd_cseg == *(dw*)&mem[0x70000 + 0x18]);
        al = 0; dos_exec();                   /* AL != 3 -> CF + ax=1 */
        CHECK(CF == 1); CHECK(ax == 1);
        al = 3; ds = 0x100; dx = 0x20;
        put_asciz(ds, dx, "/tmp/definitely_not_here.exe");
        dos_exec();
        CHECK(CF == 1); CHECK(ax == 2);
        const char *bad = "/tmp/ar_t_notmz.exe";
        ef = fopen(bad, "wb"); fwrite("NOPE!!", 1, 6, ef); fclose(ef);
        put_asciz(ds, dx, bad);
        al = 3; dos_exec();
        CHECK(CF == 1); CHECK(ax == 8);       /* not an MZ */
    }

    /* --- console output via recording text_putc --- */
    tn = 0; memset(tbuf, 0, sizeof tbuf);
    ds = 0x100; dx = 0x60;
    put_asciz(ds, dx, "HELLO");              /* put_asciz writes ASCIZ... */
    db *p = raddr_(ds, dx); p[5] = '$'; p[6] = 'X';  /* ...make it $-terminated */
    dos_print_string();
    CHECK(tn == 5 && !memcmp(tbuf, "HELLO", 5));
    tn = 0; dl = 'Z'; dos_write_char();
    CHECK(tn == 1 && tbuf[0] == 'Z');

    /* --- int1c tick (installed at seg000:14D6, C replica in dos.c) --- */
    *(dw*)&mem[0xf36d] = 3; rt_timer_tick();
    CHECK(*(dw*)&mem[0xf36d] == 2);
    *(dw*)&mem[0xf102] = 0xffff; *(dw*)&mem[0xf104] = 7;
    rt_timer_tick();                            /* low word wraps -> hi bumps */
    CHECK(*(dw*)&mem[0xf102] == 0 && *(dw*)&mem[0xf104] == 8);
    /* null vector: call is a no-op; bios int8 still bumps the BDA tick */
    *(dd*)&mem[0x1c*4] = 0;
    *(dd*)&mem[0x99*4] = 0;
    *(dd*)&mem[0x46c] = 0;
    rt_call_vector(0x99);                       /* unhooked: nothing happens */
    rt_bios_int8();
    CHECK(*(dd*)&mem[0x46c] == 1);

    /* --- misc services --- */
    dos_get_version(); CHECK(ax == 0x0500);
    dos_get_drive();   CHECK(al == 2);
    dos_get_psp();     CHECK(bx == 0x1a2);
    dos_get_verify();  CHECK(al == 0);
    dos_ioctl();       CHECK(CF == 1);
    dos_create_temp(); CHECK(CF == 1);
    dos_direct_io();   CHECK(al == 0 && ZF == 1);
    dos_check_stdin(); CHECK(al == 0xff);
    {   time_t now = time(NULL);
        int yr = localtime(&now)->tm_year + 1900;
        dos_get_date();    CHECK(cx == (dw)yr);
    }
    dos_get_time();    CHECK(ch <= 23 && cl <= 59);

    /* --- unmodeled/no-op services must not crash --- */
    dos_read_string();                     /* AH=0Ah buffered input: no-op */
    dos_int21(0x4c);                       /* dispatch stub */

    /* --- console input: stdin -> /dev/null returns EOF --- */
    {   FILE *nf = freopen("/dev/null", "r", stdin);
        CHECK(nf != NULL);
        dos_read_char_noecho(); CHECK(al == 0xff);
        dos_read_char_echo();   CHECK(al == 0xff);
    }

    /* --- dos_exit propagates the rt_exit code to the process exit status --- */
    {   pid_t p = fork();
        int st;
        CHECK(p >= 0);
        if (p == 0){ al = 0; dos_exit(); _exit(127); }
        waitpid(p, &st, 0);
        CHECK(WIFEXITED(st) && WEXITSTATUS(st) == 0);
        p = fork();
        CHECK(p >= 0);
        if (p == 0){ al = 7; dos_exit_code(); _exit(127); }
        waitpid(p, &st, 0);
        CHECK(WIFEXITED(st) && WEXITSTATUS(st) == 7);
    }

    fprintf(stderr, "test_dos: %d checks, %d failures\n", checks, fails);
    return fails ? 1 : 0;
}
