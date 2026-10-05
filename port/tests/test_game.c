/* integration tests against the real game objects (all objs minus main.o):
 *   - image load + fixups
 *   - func_at address->function mapping
 *   - BIOS int8 stub -> int1c -> countdown timers
 *   - LZW resource decoder decompress_res vs an independent reference implementation
 *     over every *CHR/*.SCR/misc .DTX file in the game dir
 */
/* pull setup_psp out of main.c for testing: the app's main() is renamed away
 * (never called); its other externs resolve against GAME_OBJS. main.c must
 * come first — it includes SDL before rt.h, whose reg macros break SDL's
 * prototypes if rt.h is already in force. */
#define main m2c_app_main
#include "../main.c"
#undef main
#include "../rt.h"
#include "../data_syms.h"
#include "../procs.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

static int fails, checks;
#define CHECK(cond) do{ checks++; if(!(cond)){ fails++; \
    fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); } }while(0)

/* ---------- independent LZW decoder (clean-room from the disassembly) ------
 * stream: byte0 = max code width; then LSB-... packed codes starting at
 * bitbuf = first word with 8 bits remaining (byte0 already consumed).
 * code width grows 9..maxw; dict[i] = {link,val}, literals 0..255, KwKwK
 * when code == next slot; dictionary resets when width would exceed maxw. */
typedef struct { unsigned short link; unsigned char val; } DE;
static size_t ref_lzw(const unsigned char *src, size_t n, unsigned char *dst){
    static DE dict[0x800];
    if (n < 2) return 0;
    int maxw = src[0];
    unsigned bitbuf = src[0] | (src[1] << 8);
    int avail = 8;
    int width = 9, mask = 0x1ff, dpos = 0x100, prev = -1;
    unsigned char first = 0;
    for (int i = 0; i < 0x800; i++) dict[i].link = 0xffff;
    for (int i = 0; i < 0x100; i++) dict[i].val = (unsigned char)i;
    const unsigned char *s = src + 2, *end = src + n;
    size_t out = 0;
    while (s < end){
        unsigned acc = bitbuf >> (16 - avail);
        int bits = avail;
        while (bits < width){
            unsigned w = 0;
            if (s + 1 < end) w = s[0] | (s[1] << 8);
            else if (s < end) w = s[0];
            s += 2; bitbuf = w; acc |= w << bits; bits += 16;
        }
        avail = bits - width;
        int code = acc & mask;
        unsigned char sstack[0x900]; int sdepth = 0;
        int a;
        if (code >= dpos){                      /* KwKwK: emit prev+first */
            sstack[sdepth++] = first;
            a = prev;
        } else a = code;
        while (a >= 0 && dict[a].link != 0xffff){ sstack[sdepth++] = dict[a].val; a = dict[a].link; }
        if (a < 0 || sdepth > 0x800) break;       /* corrupt: bail */
        first = dict[a].val;
        dst[out++] = first;
        while (sdepth) dst[out++] = sstack[--sdepth];
        dict[dpos].link = (unsigned short)prev;
        dict[dpos].val  = first;
        if (++dpos > mask){ width++; mask = (mask << 1) | 1; }
        if (width > maxw){                     /* dict reset */
            width = 9; mask = 0x1ff; dpos = 0x100;
            for (int i = 0; i < 0x800; i++) dict[i].link = 0xffff;
            for (int i = 0; i < 0x100; i++) dict[i].val = (unsigned char)i;
        }
        prev = code >= dpos - 1 ? dpos - 1 : code; /* cx = code or dx (KwKwK) */
    }
    return out;
}

/* ---------- port decoder plumbing ---------------------------------------- */
#define SRCSEG 0x5000
#define SRCOFF 0x8000   /* dict lives at ds:0x67F0..0x7FEF; source must sit past it */
#define DSTSEG 0x2000
#define STKSEG 0x9000   /* decoder uses ss:sp as a scratch push-chain; give it
                         * a full segment of headroom like the real game has */
static size_t port_lzw_src(const unsigned char *src, size_t n, vfn dec){
    memset(&mem[((dd)SRCSEG << 4) + SRCOFF], 0xAA, 0x8100); /* fixed overshoot */
    memcpy(&mem[((dd)SRCSEG << 4) + SRCOFF], src, n);
    ds = SRCSEG; si = SRCOFF; es = DSTSEG; di = 0; cx = (dw)n; dx = 0;
    ss = STKSEG; sp = 0xFFFE;
    *(dw*)raddr_(ds, 0x7ff9) = 0;   /* dcomp_4e79: BSS-cold prev=0 — garbage
        streams may open with a KwKwK code; a stale prev can chain-walk
        arbitrary memory forever (faithful, but the test must terminate) */
    dec();
    return di;                                  /* dest offset = bytes out */
}
/* lifted-vs-rewrite differential: identical pre-state -> identical post-state
 * (full 1MB image covers dict, ds-relative scratch, output and the ss:sp
 * push-chain; regs cover every register the decoder leaves). */
static db ab_pre[1<<20], ab_a[1<<20];
static dd ab_pre_r[8], ab_a_r[8], ab_b_r[8];
static dw ab_pre_g[8], ab_a_g[8], ab_b_g[8];
static void ab_grab(dd *r, dw *g){
    r[0]=eax;r[1]=ebx;r[2]=ecx;r[3]=edx;r[4]=esi;r[5]=edi;r[6]=esp;r[7]=ebp;
    g[0]=cs;g[1]=ds;g[2]=es;g[3]=ss;
}
static void ab_put(const dd *r, const dw *g){
    eax=r[0];ebx=r[1];ecx=r[2];edx=r[3];esi=r[4];edi=r[5];esp=r[6];ebp=r[7];
    cs=g[0];ds=g[1];es=g[2];ss=g[3];
}
/* returns bytes-out from the C rewrite, or -1 on any divergence */
static long ab_lzw(const unsigned char *src, size_t n){
    memcpy(ab_pre, mem, sizeof mem); ab_grab(ab_pre_r, ab_pre_g);
    size_t ln = port_lzw_src(src, n, decompress_res_lifted);
    memcpy(ab_a, mem, sizeof mem); ab_grab(ab_a_r, ab_a_g);
    memcpy(mem, ab_pre, sizeof mem); ab_put(ab_pre_r, ab_pre_g);
    size_t cn = port_lzw_src(src, n, decompress_res);  /* -> rewrite */
    ab_grab(ab_b_r, ab_b_g);
    CHECK(ln == cn);
    CHECK(!memcmp(ab_a, mem, sizeof mem));
    CHECK(!memcmp(ab_a_r, ab_b_r, sizeof ab_a_r));
    CHECK(!memcmp(ab_a_g, ab_b_g, sizeof ab_a_g));
    return cn;
}
/* resource-chain A/B: like ab_lzw but flags are part of the contract
 * (every caller gates on CF). feed low nibble = Enters to queue before
 * each run for res_file_error's key wait (a path can wait more than
 * once — mode_rec_load reports the write failure in write_res_file AND
 * again in its own fail tail). feed bit4 closes res_handle between runs
 * (open_res_file leaves the host handle occupied -> next open would get
 * a different handle number). */
static void ab_res(const char *nm, void(*lifted)(void), void(*now)(void), int feed){
    for (int i = feed & 0xf; i > 0; i--) rt_test_kpush(0x1c0d);
    memcpy(ab_pre, mem, sizeof mem); ab_grab(ab_pre_r, ab_pre_g);
    int cf0 = CF, zf0 = ZF, sf0 = SF, of0 = OF;
    lifted();
    memcpy(ab_a, mem, sizeof mem); ab_grab(ab_a_r, ab_a_g);
    int cfa = CF, zfa = ZF, sfa = SF, ofa = OF;
    if (feed & 0x10){ bx = res_handle; ah = 0x3e; dos_close(); }
    memcpy(mem, ab_pre, sizeof mem); ab_put(ab_pre_r, ab_pre_g);
    CF = cf0; ZF = zf0; SF = sf0; OF = of0;
    for (int i = feed & 0xf; i > 0; i--) rt_test_kpush(0x1c0d);
    now();
    ab_grab(ab_b_r, ab_b_g);
    int ok = !memcmp(ab_a, mem, sizeof mem) &&
             !memcmp(ab_a_r, ab_b_r, sizeof ab_a_r) &&
             !memcmp(ab_a_g, ab_b_g, sizeof ab_a_g) &&
             cfa == CF && zfa == ZF && sfa == SF && ofa == OF;
    if (!ok) {
        static const char *rn[8] = {"eax","ebx","ecx","edx","esi","edi","esp","ebp"};
        static const char *sn[4] = {"cs","ds","es","ss"};
        long d = -1, nd = 0;
        for (long i = 0; i < (long)sizeof mem; i++)
            if (ab_a[i] != mem[i]) { if (d < 0) d = i; nd++; }
        fprintf(stderr, "  res-ab %s: cf %d/%d zf %d/%d sf %d/%d of %d/%d mem@0x%lx n=%ld",
                nm, cfa, CF, zfa, ZF, sfa, SF, ofa, OF, d, nd);
        if (d >= 0) {
            fprintf(stderr, " [");
            for (long i = d; i < (long)sizeof mem && i < d + 8; i++)
                if (ab_a[i] != mem[i]) fprintf(stderr, " %lx:%02x/%02x", i, ab_a[i], mem[i]);
            fprintf(stderr, " ]");
        }
        for (int i = 0; i < 8; i++)
            if (ab_a_r[i] != ab_b_r[i])
                fprintf(stderr, " %s=%x/%x", rn[i], ab_a_r[i], ab_b_r[i]);
        for (int i = 0; i < 4; i++)
            if (ab_a_g[i] != ab_b_g[i])
                fprintf(stderr, " %s=%x/%x", sn[i], ab_a_g[i], ab_b_g[i]);
        fprintf(stderr, "\n");
    }
    CHECK(ok);
    if (feed & 0x10){ bx = res_handle; ah = 0x3e; dos_close(); }
}

/* ---- overlay-loader A/B ----------------------------------------------
 * Every load_overlay error tail lands on e13baa -> dos_exit_code ->
 * rt_exit -> exit(), so the impls can't run in-process here. Each impl
 * instead runs in a forked child: guest state is rebuilt in the child,
 * an atexit handler dumps mem + regs + flags + host-side DOS state
 * (allocator frontier, block lists, overlay map) to $M2C_ABDUMP, and
 * the parent compares the two dumps + exit codes byte-for-byte.
 * Children that return normally exit(0x5a) — the atexit dump still
 * runs, and the exit code records which path the impl took.          */
static void ab_dump(void){
    const char *p = getenv("M2C_ABDUMP"); if (!p) return;
    FILE *f = fopen(p, "wb"); if (!f) return;
    struct { dd r[8]; dw g[4], fl; struct dos_snap s; } d;
    memset(&d, 0, sizeof d);            /* pad bytes must be deterministic */
    ab_grab(d.r, d.g);
    d.fl = CF | (ZF << 1) | (SF << 2) | (OF << 3);
    dos_snap_get(&d.s);
    fwrite(mem, 1, sizeof mem, f);
    fwrite(&d, 1, sizeof d, f);
    fclose(f);
}

/* minimal MZ fixture: 32-byte header + body carrying the game's own
 * overlay header (code seg @+0x18, count @+0x1C, sizes @+0x1E/+0x20,
 * table count @+0x22, table offs @+0x24). */
static void mk_ovl_mz(const char *path, dw f18, dw f1c, dw f1e,
                      dw f20, dw f22, const dw *offs, int n){
    db mz[0x60]; memset(mz, 0, sizeof mz);
    mz[0] = 'M'; mz[1] = 'Z';
    *(dw*)(mz + 8)    = 2;              /* hdr size = 2 paras = 32B  */
    *(dw*)(mz + 6)    = 1;              /* 1 relocation entry        */
    *(dw*)(mz + 0x18) = 0x1c;           /* reloc table offset        */
    *(dw*)(mz + 0x1c) = 0x0004;         /* entry: seg 0, off 4       */
    db *b = mz + 0x20;
    *(dw*)(b + 0x18) = f18; *(dw*)(b + 0x1c) = f1c;
    *(dw*)(b + 0x1e) = f1e; *(dw*)(b + 0x20) = f20; *(dw*)(b + 0x22) = f22;
    for (int i = 0; i < n; i++) *(dw*)(b + 0x24 + i*2) = offs[i];
    *(dw*)(b + 4) = 7;                  /* reloc target word         */
    FILE *f = fopen(path, "wb"); fwrite(mz, 1, sizeof mz, f); fclose(f);
}

enum { OVL_OK, OVL_NOFILE, OVL_BADMZ, OVL_TOOBIG, OVL_MID_AB6,
       OVL_MID_AB6F, OVL_MID_ACA2, OVL_MID_ACA8, OVL_MID_AF9,
       OVL_MID_B08, OVL_MID_B12, OVL_MID_B46, OVL_MID_B85,
       OVL_MID_B9E, OVL_MID_BAA };
/* rebuild deterministic guest state inside the forked child — the image
 * must be real: the error $-strings live in it (dos_print_string walks
 * to '$' — a zeroed image sends it off the end of mem[]). */
static void ovl_prep(int v){
    mem_load_image();
    mem_apply_fixups();
    eax = ebx = ecx = edx = esi = edi = esp = ebp = 0;
    cs = ds = es = ss = seg_data;
    CF = ZF = SF = OF = 0; sp = 0x0ffe;
    struct dos_snap s; memset(&s, 0, sizeof s);
    s.alloc_next = 0x80000; dos_snap_set(&s);
    strcpy((char*)raddr_(seg_data, 0x40), "/tmp/ar_ovl_ok.exe");
    mem_ww((dd)seg_data * 16 + 0x19E2, 0x40);   /* name table [0] */
    bx = 0;
    switch (v){
    case OVL_NOFILE:
        strcpy((char*)raddr_(seg_data, 0x40), "/tmp/ar_ovl_gone.exe");
        break;
    case OVL_BADMZ:
        strcpy((char*)raddr_(seg_data, 0x40), "/tmp/ar_ovl_badmz.exe");
        break;
    case OVL_TOOBIG:                     /* shrink free pool so the size
                                          * check (16-bit sizes) can fire */
        s.alloc_next = 0x98000; dos_snap_set(&s);
        strcpy((char*)raddr_(seg_data, 0x40), "/tmp/ar_ovl_big.exe");
        break;
    case OVL_MID_AB6:                    /* e13ab6: alloc succeeds    */
        bx = 0x1000; push(0x40); break;
    case OVL_MID_AB6F:                   /* e13ab6: alloc too big     */
        bx = 0x9000; push(0x40); break;
    case OVL_MID_ACA2:                   /* e13aca: EXEC missing file */
        ax = 0x8000; push(0x40);
        strcpy((char*)raddr_(seg_data, 0x40), "/tmp/ar_ovl_gone.exe");
        break;
    case OVL_MID_ACA8:                   /* e13aca: EXEC bad MZ       */
        ax = 0x8000; push(0x40);
        strcpy((char*)raddr_(seg_data, 0x40), "/tmp/ar_ovl_badmz.exe");
        break;
    case OVL_MID_AF9:                    /* e13af9: code 8            */
        ax = 8; break;
    case OVL_MID_B08:                    /* e13b08: generic code      */
        ax = 1; break;
    case OVL_MID_B12: {                  /* post-EXEC restore + table */
        db *o = &mem[0x90000];
        *(dw*)(o + 0x18) = 0x0300;       /* overlay code seg          */
        *(dw*)(o + 0x1c) = 0x0000;       /* existing table entries    */
        *(dw*)(o + 0x1e) = 0x0060;       /* sizes -> 6+4 = 10 paras   */
        *(dw*)(o + 0x20) = 0x0040;
        *(dw*)(o + 0x22) = 0x0002;       /* 2 new table entries       */
        *(dw*)(o + 0x24) = 0x1111; *(dw*)(o + 0x26) = 0x2222;
        ovl_3a90 = 0x0ffe; ovl_3a92 = ss;
        ovl_e840 = 0x9000; ovl_e842 = 0x2000;
        break; }
    case OVL_MID_B46: {                  /* table-loop entry          */
        db *o = &mem[0x90000];
        *(dw*)(o + 0x1e) = 0x0060; *(dw*)(o + 0x20) = 0x0040;
        *(dw*)(o + 0x24) = 0x1111; *(dw*)(o + 0x26) = 0x2222;
        cx = 2; si = 0x24; bx = 0x1990; di = 0x0300;
        es = 0x9000; ovl_e840 = 0x9000; ovl_e842 = 0x2000;
        break; }
    case OVL_MID_B85:                    /* setblock tail             */
        bx = 0x10; ovl_e840 = 0x9000; break;
    case OVL_MID_B9E:                    /* trivial success tail      */
        ovl_e840 = 0x9000; break;
    default: break;                      /* OVL_OK / OVL_MID_BAA      */
    }
}

static void ab_ovl(const char *nm, vfn lif, vfn now, int variant){
    char pa[80], pb[80]; int ec[2] = { -1, -1 };
    snprintf(pa, sizeof pa, "/tmp/ab_ovl_a.bin");
    snprintf(pb, sizeof pb, "/tmp/ab_ovl_b.bin");
    for (int i = 0; i < 2; i++){
        setenv("M2C_ABDUMP", i ? pb : pa, 1);
        pid_t p = fork();
        if (p == 0){ ovl_prep(variant); (i ? now : lif)(); exit(0x5a); }
        int st = 0; waitpid(p, &st, 0);
        if (WIFEXITED(st)) ec[i] = WEXITSTATUS(st);
        else if (WIFSIGNALED(st)) ec[i] = -WTERMSIG(st);
    }
    FILE *fa = fopen(pa, "rb"), *fb = fopen(pb, "rb");
    int same = fa && fb; long firstdiff = -1;
    if (same){
        db ba[4096], bb[4096]; size_t ra, rb; long off = 0;
        for (;;){
            ra = fread(ba, 1, sizeof ba, fa); rb = fread(bb, 1, sizeof bb, fb);
            if (ra != rb || memcmp(ba, bb, ra)){
                same = 0;
                if (ra == rb) for (size_t k = 0; k < ra; k++)
                    if (ba[k] != bb[k]){ firstdiff = off + (long)k; break; }
                break;
            }
            off += (long)ra;
            if (!ra) break;
        }
    }
    if (fa) fclose(fa); if (fb) fclose(fb);
    if (ec[0] != ec[1] || !same)
        fprintf(stderr, "  ovl-ab %s: exit %d/%d, dumps %s (first @%lx)\n",
                nm, ec[0], ec[1], same ? "equal" : "DIFFER", firstdiff);
    CHECK(ec[0] == ec[1]); CHECK(same);
}

int main(void){
    /* ---- image load ---- */
    mem_load_image();
    mem_apply_fixups();
    CHECK(mem[0x1a20] != 0 || mem[0x1a21] != 0);  /* image landed */
    long nz = 0; for (long i = 0x1a20; i < 0x1a20 + 163888; i++) nz += mem[i] != 0;
    CHECK(nz > 60000);                             /* real code+data present */

    /* ---- setup_psp (from main.c): minimal PSP contract at 0x192:0 ---- */
    setup_psp();
    {   const db *psp = &mem[0x192 << 4];
        CHECK(psp[0] == 0xcd && psp[1] == 0x20);      /* int 20h */
        CHECK(*(const dw*)(psp + 2) == 0xa000);       /* top of memory */
        CHECK(psp[0x2c] == 0 && psp[0x2d] == 0);      /* env seg 0 */
        CHECK(psp[0x80] == 1 && psp[0x81] == ' ' && psp[0x82] == 0x0d);
    }

    /* ---- func_at ---- */
    CHECK(func_at(0xffea5) == rt_bios_int8);
    { vfn z = func_at(0); CHECK(z != 0); z(); }
    CHECK(func_at(0x1a20 + 0x3998) == decompress_res);  /* LZW decoder maps */
    CHECK(func_at(0x1a20 + 0x1c6a) == tilemap_compose);  /* tilemap compositor */

    /* ---- int1c special-case + BIOS int8 chain ---- */
    *(dw*)&mem[0x1c*4]   = 0x14d6;                 /* seg000:14D6 */
    *(dw*)&mem[0x1c*4+2] = 0x1a2;
    *(dw*)&mem[0x8*4]    = 0xfea5;                 /* planted BIOS stub */
    *(dw*)&mem[0x8*4+2]  = 0xf000;
    *(dd*)&mem[0x46c] = 0;
    *(dw*)&mem[0xf36d] = 10;                       /* word_1D94D countdown */
    rt_bios_int8();
    CHECK(*(dd*)&mem[0x46c] == 1);                 /* BDA tick bumped */
    CHECK(*(dw*)&mem[0xf36d] == 9);                /* int1c chain fired */
    rt_call_vector(8);                             /* through the IVT too */
    CHECK(*(dd*)&mem[0x46c] == 2);
    CHECK(*(dw*)&mem[0xf36d] == 8);
    rt_call_vector(0x1c);
    CHECK(*(dw*)&mem[0xf36d] == 7);
    /* countdown saturates at 0 */
    *(dw*)&mem[0xf36d] = 1; rt_call_vector(0x1c); rt_call_vector(0x1c);
    CHECK(*(dw*)&mem[0xf36d] == 0);

    /* ---- LZW: every .DTX decodes identically under both implementations ----
     * Corpus needs the original game files (not in the repo): env override,
     * the parent dir (tests run from port/), then the dev path as fallback.
     * No corpus -> skip that section, the rest of the test is asset-free. */
    const char *gdir = getenv("M2C_GAMEDIR");
    if (!gdir){
        static const char *cand[] = { "..", "/home/xor/games/airborn" };
        for (size_t i = 0; i < sizeof cand / sizeof *cand; i++){
            DIR *t = opendir(cand[i]);
            if (t){ closedir(t); gdir = cand[i]; break; }
        }
    }
    DIR *d = gdir ? opendir(gdir) : NULL;
    struct dirent *e; int ndtx = 0;
    static unsigned char ref_out[0x20000];
    while (d && (e = readdir(d))){
        const char *nm = e->d_name;
        size_t l = strlen(nm);
        if (l < 4 || strcasecmp(nm + l - 4, ".DTX")) continue;
        char path[512]; snprintf(path, sizeof path, "%s/%s", gdir, nm);
        FILE *f = fopen(path, "rb"); if (!f) continue;
        static unsigned char src[0x8100];
        size_t n = fread(src, 1, sizeof src, f); fclose(f);
        if (n < 3 || n > 0x8000) continue;         /* src must fit past the dict */
        size_t rn = ref_lzw(src, n, ref_out);
        size_t pn = ab_lzw(src, n);         /* lifted vs C: full state A/B */
        ndtx++;
        if (rn != pn || (rn && memcmp(ref_out, &mem[(dd)DSTSEG << 4], rn))){
            fails++;
            fprintf(stderr, "FAIL %-14s ref=%zx port=%zx", nm, rn, pn);
            if (rn && pn){
                size_t m = rn < pn ? rn : pn, i;
                for (i = 0; i < m && ref_out[i] == mem[((dd)DSTSEG<<4)+i]; i++);
                fprintf(stderr, " first-diff@%zx ref=%02x port=%02x",
                        i, ref_out[i], mem[((dd)DSTSEG<<4)+i]);
            }
            fputc('\n', stderr);
        } else checks += 2;
        /* 40x25 word-index tilemaps decode to exactly 0x7d0 bytes; every
         * index must be < 0x800 (dict-size bound). Larger SCR resources are
         * other structures - byte-compare above already covers them. */
        if (strstr(nm, "SCR") && pn == 0x7d0){
            int bad = 0;
            for (size_t i = 0; i + 1 < pn; i += 2){
                unsigned idx = mem[((dd)DSTSEG<<4)+i] | (mem[((dd)DSTSEG<<4)+i+1] << 8);
                if (idx >= 0x800) bad++;
            }
            CHECK(bad == 0);
        }
    }
    if (d) closedir(d);
    if (ndtx){
        CHECK(ndtx > 20);                          /* really exercised the corpus */
        fprintf(stderr, "  (%d DTX files decoded identically)\n", ndtx);
    } else {
        fprintf(stderr, "  SKIP: no .DTX corpus (gdir=%s)\n",
                gdir ? gdir : "(none found)");
    }

    /* ---- rewrite differential on synthetic streams (asset-free) ----
     * random streams across max-widths 9..16 exercise width growth, dict
     * resets, KwKwK and corrupt-code paths; all-zero / all-0xFF hit the
     * literal-only and max-code extremes. A/B compares full mem + regs. */
    {
        static unsigned char syn[0x4000];
        static const struct { int w; unsigned char fill; unsigned seed; } sc[] = {
            { 9, 0, 0x1234 }, { 10, 0, 0x77 }, { 12, 0, 0xdead }, { 16, 0, 1 },
            { 9, 0x00, 0 }, { 12, 0xFF, 0 },
        };
        for (size_t ci = 0; ci < sizeof sc / sizeof *sc; ci++){
            syn[0] = (unsigned char)sc[ci].w;
            unsigned rng = sc[ci].seed;
            for (size_t i = 1; i < sizeof syn; i++)
                syn[i] = sc[ci].seed ? (unsigned char)(rng = rng*1103515245 + 12345, rng >> 24)
                                     : sc[ci].fill;
            ab_lzw(syn, sizeof syn);
            /* tiny/truncated streams: the do-while still runs one decode */
            for (size_t n = 0; n < 6 && ci == 0; n++) ab_lzw(syn, n);
        }
    }

    /* ---- resource-file chain: lifted vs C rewrite ----------------------
     * Same A/B shape as the decoder — identical pre-state -> identical
     * post-state — but CF/ZF/SF/OF are part of the contract here (every
     * caller gates on CF), so flags are compared too. File side effects
     * are confined to test_res/test_big/test_out*.tmp the test creates
     * itself, so the whole section is asset-free. */
    {
        /* synthetic resource files: [0]=LZW maxw 9 + zeros decodes
         * deterministically; raw content is enough for the read paths */
        {   FILE *f = fopen("test_res.tmp", "wb"); CHECK(f);
            for (int i = 0; i < 20; i++) fputc(i ? 0 : 9, f);
            fclose(f); }
        {   FILE *f = fopen("test_big.tmp", "wb"); CHECK(f);
            fseek(f, 70000, SEEK_SET); fputc(0, f); fclose(f); }  /* >64K */

        ds = *(dw*)&mem[0x1a20];                      /* seg_data */
        ss = 0x8000; sp = 0xFFFE;                     /* guest stack in mem */
        *(dw*)raddr_(ds, 0xad7) = 0;                  /* no int9 divert */

        /* rec table: ds:756[res_cache_d] -> rec ptr; rec = 13B name +
         * mode byte + load seg:off + parm + staging seg:off */
        res_cache_d = 0;
        *(dw*)raddr_(ds, 0x756) = 0x400;
        memset(raddr_(ds, 0x400), 0, 0x18);
        strcpy((char*)raddr_(ds, 0x400), "test_res.tmp");  /* 12 ch + NUL */
        *(db*)raddr_(ds, 0x400 + 0x0D) = 0;             /* mode -> tbl[0] */
        *(dw*)raddr_(ds, 0x400 + 0x0E) = 0x6000;        /* load seg      */
        *(dw*)raddr_(ds, 0x400 + 0x10) = 0;             /* load off      */
        *(dw*)raddr_(ds, 0x400 + 0x12) = 0;             /* parm: raw     */
        *(dw*)raddr_(ds, 0x400 + 0x14) = 0x7000;        /* staging seg   */
        *(dw*)raddr_(ds, 0x400 + 0x16) = 0;             /* staging off   */
        funcs_1083a = 0x091f;                           /* -> locret_1091f */

        /* second rec slot for the write path */
        *(dw*)raddr_(ds, 0x758) = 0x440;
        memset(raddr_(ds, 0x440), 0, 0x18);
        strcpy((char*)raddr_(ds, 0x440), "test_out.tmp");
        *(dw*)raddr_(ds, 0x440 + 0x0E) = 0x6000;
        *(dw*)raddr_(ds, 0x440 + 0x10) = 0;

        /* --- leaf services --- */
        res_name_ptr = 0x400; strcpy((char*)raddr_(ds, 0x400), "test_res.tmp");
        ab_res("open_res_file", open_res_file_lifted, open_res_file, 0x10);

        res_handle = 0x77;                               /* bogus handle */
        ab_res("close_res_file", close_res_file_lifted, close_res_file, 0);

        /* --- seek: success / >64K / missing --- */
        ab_res("seek ok", seek_res_entry_lifted, seek_res_entry, 0);
        CHECK(res_f1a == 20);
        res_name_ptr = 0x400; strcpy((char*)raddr_(ds, 0x400), "test_big.tmp");
        ab_res("seek >64K", seek_res_entry_lifted, seek_res_entry, 0);
        res_name_ptr = 0x400; strcpy((char*)raddr_(ds, 0x400), "no_such.tmp");
        ab_res("seek missing", seek_res_entry_lifted, seek_res_entry, 0);
        res_handle = 0x77;
        ab_res("seek tail", seek_res_entry_e108c1_lifted, seek_res_entry_e108c1, 0);

        /* --- read: success / missing (fail path waits Enter) --- */
        res_name_ptr = 0x400; strcpy((char*)raddr_(ds, 0x400), "test_res.tmp");
        res_seg = 0x6000; res_ptr = 0;
        ab_res("read ok", res_file_read_lifted, res_file_read, 0);
        CHECK(!memcmp(&mem[0x60000], "\x09", 1));
        res_name_ptr = 0x400; strcpy((char*)raddr_(ds, 0x400), "no_such.tmp");
        ab_res("read missing", res_file_read_lifted, res_file_read, 1);
        res_name_ptr = 0x400; strcpy((char*)raddr_(ds, 0x400), "test_res.tmp");
        res_seg = 0x6000; res_ptr = 0; res_f1a = 20;
        ab_res("read mid", res_file_read_e10849_lifted, res_file_read_e10849, 0);
        res_handle = 0x77;
        ab_res("read fail", res_file_read_e10846_lifted, res_file_read_e10846, 1);

        /* --- error handler + mid-entries (all wait for Enter) --- */
        res_name_ptr = 0x400; strcpy((char*)raddr_(ds, 0x400), "test_res.tmp");
        res_handle = 0x77;
        ab_res("load_fail", res_load_fail_lifted, res_load_fail, 1);
        ab_res("res_error", res_file_error_lifted, res_file_error, 1);
        si = res_name_ptr; di = 0x13E;
        ab_res("err e109ab", res_file_error_e109ab_lifted, res_file_error_e109ab, 1);
        si = res_name_ptr; di = 0x13E;
        ab_res("err e109b7", res_file_error_e109b7_lifted, res_file_error_e109b7, 1);
        ab_res("err e109d9", res_file_error_e109d9_lifted, res_file_error_e109d9, 1);

        /* --- load_resource: raw + compressed recs --- */
        res_name_ptr = 0x400; strcpy((char*)raddr_(ds, 0x400), "test_res.tmp");
        res_cache_d = 0;
        *(dw*)raddr_(ds, 0x400 + 0x12) = 0;             /* parm: raw     */
        ab_res("load raw", load_resource_lifted, load_resource, 0);
        *(dw*)raddr_(ds, 0x400 + 0x12) = 1;             /* parm: dcomp   */
        ab_res("load cmp", load_resource_lifted, load_resource, 0);
        *(dw*)raddr_(ds, 0x400 + 0x12) = 0;

        /* --- write path: mode_rec_load + write_res_file + mid-entries ---
         * dos_int21 is an unmodeled stub, so CREAT never produces a handle —
         * the write path falls into res_load_fail and waits for Enter
         * (feed=1). This mirrors the real port's behavior faithfully. */
        res_cache_d = 1; res_f1a = 20;
        memset(&mem[0x60000], 0x41, 20);                /* source payload */
        ab_res("mode_rec", mode_rec_load_lifted, mode_rec_load, 2);
        remove("test_out.tmp");

        res_name_ptr = 0x400; strcpy((char*)raddr_(ds, 0x400), "test_out2.tmp");
        res_f1a = 20; res_seg = 0x6000; res_ptr = 0;
        ab_res("write_res", write_res_file_lifted, write_res_file, 1);
        remove("test_out2.tmp");

        res_handle = 0x77;
        ab_res("wrf e1092d", write_res_file_e1092d_lifted, write_res_file_e1092d, 1);

        /* e1092f with a live handle: dos_create_new() is callable directly
         * (only the int21 stub is unmodeled) — each run needs a fresh handle;
         * after close the slot recycles so the handle number matches */
        {
            res_name_ptr = 0x400; strcpy((char*)raddr_(ds, 0x400), "test_out3.tmp");
            res_f1a = 20; res_seg = 0x6000; res_ptr = 0;
            memcpy(ab_pre, mem, sizeof mem); ab_grab(ab_pre_r, ab_pre_g);
            int cf0 = CF, zf0 = ZF, sf0 = SF, of0 = OF;
            ah = 0x3C; dx = res_name_ptr; cx = 0; dos_create_new();
            dw h0 = ax;
            write_res_file_e1092f_lifted();
            memcpy(ab_a, mem, sizeof mem); ab_grab(ab_a_r, ab_a_g);
            int cfa = CF, zfa = ZF, sfa = SF, ofa = OF;
            ah = 0x3C; dx = res_name_ptr; cx = 0; dos_create_new();
            CHECK(ax == h0);
            memcpy(mem, ab_pre, sizeof mem); ab_put(ab_pre_r, ab_pre_g);
            CF = cf0; ZF = zf0; SF = sf0; OF = of0;
            ax = h0;
            write_res_file_e1092f();
            ab_grab(ab_b_r, ab_b_g);
            CHECK(!memcmp(ab_a, mem, sizeof mem));
            CHECK(!memcmp(ab_a_r, ab_b_r, sizeof ab_a_r));
            CHECK(!memcmp(ab_a_g, ab_b_g, sizeof ab_a_g));
            CHECK(cfa == CF); CHECK(zfa == ZF); CHECK(sfa == SF); CHECK(ofa == OF);
            remove("test_out3.tmp");
        }

        /* drain any leftover queued key, then clean up temp files */
        for (;;){ bios_kbhit(); if (ZF) break; bios_getch(); }
        remove("test_res.tmp"); remove("test_big.tmp");
        fprintf(stderr, "  (res chain: lifted vs C, %d checks)\n", checks);
    }

    /* ---- palette upload + mode dispatch: lifted vs C ----------------
     * pal_upload dispatches on adapter_id via pal_jt; only the MCGA
     * case (3) resolves to a live proc — other adapters hit the
     * unresolved-jmp fallback (stderr noise is expected). The MCGA
     * loop mirrors palette[i] into DAC regs i and i+0x18 via the RGB
     * component tables at ds:0xB8/0xC8/0xD8. bios_palette() is a host
     * svc — side effects identical for both runs.                    */
    {
        ds = seg_data;
        for (int i = 0; i < 16; i++){                    /* palette tbl */
            *(db*)raddr_(ds, 0x1962 + i) = i;
            *(db*)raddr_(ds, 0x1972 + i) = 15 - i;
            *(db*)raddr_(ds, 0x0B8 + i) = i * 4;         /* R */
            *(db*)raddr_(ds, 0x0C8 + i) = i * 4 + 1;     /* B */
            *(db*)raddr_(ds, 0x0D8 + i) = i * 4 + 2;     /* G */
        }
        for (dw a = 0; a < 8; a++){                      /* all cases   */
            adapter_id = a;
            ab_res("pal_upload", pal_upload_lifted, pal_upload, 0);
        }
        adapter_id = 3;                                  /* MCGA        */
        ab_res("load_palette", load_palette_lifted, load_palette, 0);
        ab_res("load_palette_b", load_palette_b_lifted, load_palette_b, 0);
        ds = seg_data; palette_ptr = 0x1962;
        si = 0;
        ab_res("ega loop", load_palette_e103a5_lifted, load_palette_e103a5, 0);
        si = 5;                                          /* mid-entry   */
        ab_res("ega mid5", load_palette_e103a5_lifted, load_palette_e103a5, 0);
        ab_res("mcga", pal_upload_mcga_lifted, pal_upload_mcga, 0);
        si = 5;
        ab_res("mcga mid5", pal_upload_mcga_e10421_lifted, pal_upload_mcga_e10421, 0);

        /* rec_walk: 6-byte records {si,bx,dx} until si==0xFFFF, each
         * iteration calls load_palette through the trampoline       */
        di = 0x7000;
        for (int i = 0; i < 3; i++){
            *(dw*)raddr_(ds, 0x7000 + i*6) = 0x100 + i;
            *(dw*)raddr_(ds, 0x7002 + i*6) = 0x200 + i;
            *(dw*)raddr_(ds, 0x7004 + i*6) = 0x300 + i;
        }
        *(dw*)raddr_(ds, 0x7000 + 3*6) = 0xFFFF;
        ab_res("rec_walk", rec_walk_e10491_lifted, rec_walk_e10491, 0);

        /* glyph_conv_dispatch: setup + glyphconv_jt[adapter] — case 3
         * is nullsub_8; unresolvable cases print + return           */
        res_seg = seg_data; res_ptr = 0x7100;
        *(dw*)raddr_(seg_data, 0x7100) = 7;               /* glyph cnt  */
        for (dw a = 0; a < 8; a++){
            adapter_id = a;
            ab_res("glyphconv", glyph_conv_dispatch_lifted, glyph_conv_dispatch, 0);
        }

        /* mode_call: ds:0x194-indexed ind call through funcs_1083a.
         * The res fixture redirected entries 0-1 (dd write) — restore
         * the image values: 0 -> nullsub_1 (sp-balanced),
         * 1 -> glyph_conv_dispatch, 4 -> mid-proc offset (unresolved) */
        ((dw*)&funcs_1083a)[0] = 0x078A;
        ((dw*)&funcs_1083a)[1] = 0x0797;
        for (dw i = 0; i < 5; i++){
            *(dw*)raddr_(ds, 0x194) = i;
            ab_res("mode_call", mode_call_lifted, mode_call, 0);
        }
        fprintf(stderr, "  (palette/dispatch: lifted vs C, %d checks)\n", checks);
    }

    /* ---- render pump + object slots + AI params: lifted vs C --------
     * Columnar object tables: type byte ds:0xBE8D+i, count ds:0xC895+i,
     * weight ds:0xC8A7+i, alive flag ds:0xC487+i, gate ds:0xC005+i.
     * render_tick callees are all real procs — A/B compares their
     * combined effect; timer_cnt=0 makes the drain loop exit.        */
    {
        ds = seg_data;
        /* render pump: render_cnt>0 skips the frame, <=0 renders     */
        timer_cnt = 0;
        render_cnt = 3; ot0a_tick_65ac = 0;
        ab_res("render skip", render_tick_lifted, render_tick, 0);
        render_cnt = 0; ot0a_tick_65ac = 0;
        ab_res("render fire", render_tick_lifted, render_tick, 0);
        stage_parm_a = 7; timer_cnt = 0;
        ab_res("redraw", redraw_frame_lifted, redraw_frame, 0);
        timer_cnt = 0;                  /* reload leaves stage_parm_a  */
        ab_res("redraw tail", redraw_frame_e14afe_lifted, redraw_frame_e14afe, 0);

        /* hit flash: 0 -> period 0xFF, n -> dec + period 0x1E        */
        hit_flash = 3;
        ab_res("hitflash", hitflash_dec_lifted, hitflash_dec, 0);
        hit_flash = 0;
        ab_res("hitflash 0", hitflash_dec_lifted, hitflash_dec, 0);
        hit_flash = 9;
        ab_res("hitf mid", hitflash_dec_e16d7e_lifted, hitflash_dec_e16d7e, 0);
        al = 0x42;
        ab_res("hitf tail", hitflash_dec_e16d84_lifted, hitflash_dec_e16d84, 0);

        /* slot scan: all-free / hole@3 / full                        */
        memset(raddr_(ds, 0xBE8D), 0, 0x22);
        ab_res("slot free", find_free_slot_b_lifted, find_free_slot_b, 0);
        memset(raddr_(ds, 0xBE8D), 7, 0x22);
        *(db*)raddr_(ds, 0xBE8D + 3) = 0;
        ab_res("slot hole", find_free_slot_b_lifted, find_free_slot_b, 0);
        memset(raddr_(ds, 0xBE8D), 9, 0x22);
        ab_res("slot full", find_free_slot_b_lifted, find_free_slot_b, 0);
        memset(raddr_(ds, 0xBE8D), 0, 0x22);
        memset(raddr_(ds, 0xBE8D), 5, 5);
        si = 0x10;
        ab_res("slot mid", find_free_slot_b_e160e9_lifted, find_free_slot_b_e160e9, 0);
        ab_res("slot clc", find_free_slot_b_e160f9_lifted, find_free_slot_b_e160f9, 0);

        /* obj_rec_clear: fill the record columns with 0xAA first     */
        memset(raddr_(ds, 0xBEDD), 0xAA, 0x80A);
        si = 0x50;
        ab_res("obj clear", obj_rec_clear_lifted, obj_rec_clear, 0);

        /* obj_tick_all: empty slots vs type-1/2 dispatch             */
        memset(raddr_(ds, 0xBE8D), 0, 0x22);
        ab_res("tick empty", obj_tick_all_lifted, obj_tick_all, 0);
        *(db*)raddr_(ds, 0xBE8D) = 1;
        *(db*)raddr_(ds, 0xBE8F) = 2;
        ab_res("tick live", obj_tick_all_lifted, obj_tick_all, 0);
        memset(raddr_(ds, 0xBE8D), 0, 0x22);

        /* obj_alive_mark: map/deadzone/bx gates then the flag set    */
        map_kind = 0x0B; bx = 4;
        *(db*)raddr_(ds, (dw)(4 - 0x3FFD)) = 0x10;   /* <0xEE passes    */
        *(db*)raddr_(ds, (dw)(4 - 0x3B79)) = 0;      /* not marked      */
        ab_res("alive mark", obj_alive_mark_lifted, obj_alive_mark, 0);
        bx = 0;
        ab_res("alive bx0", obj_alive_mark_lifted, obj_alive_mark, 0);
        map_kind = 5;
        ab_res("alive map!", obj_alive_mark_lifted, obj_alive_mark, 0);
        bx = 4; *(db*)raddr_(ds, (dw)(4 - 0x3B79)) = 1;
        ab_res("alive mid", obj_alive_mark_e16b8b_lifted, obj_alive_mark_e16b8b, 0);

        /* slot_weight_sum: counts ds:0xC895+i, weights ds:0xC8A7+i   */
        { static const db cnt[6] = {2,0,3,1,0,4}, wgt[6] = {10,20,30,40,50,60};
          memcpy(raddr_(ds, 0xC895), cnt, 6);
          memcpy(raddr_(ds, 0xC8A7), wgt, 6); }
        ab_res("wsum", slot_weight_sum_lifted, slot_weight_sum, 0);
        si = 2; ax = 0x100;
        ab_res("wsum mid", slot_weight_sum_e16d53_lifted, slot_weight_sum_e16d53, 0);
        si = 1; cx = 3; ax = 0;
        ab_res("wsum acc", slot_weight_sum_e16d5d_lifted, slot_weight_sum_e16d5d, 0);
        si = 3; ax = 7;
        ab_res("wsum dec", slot_weight_sum_e16d66_lifted, slot_weight_sum_e16d66, 0);

        /* ai_param_fetch: wounds/difficulty sweeps; param word tables
         * at ds:0xCCE5+si and ds:0xCCFD+si (si = 2*clamped_level)    */
        for (int i = 0; i < 14; i++){
            *(dw*)raddr_(ds, 0xCCE5 + i*2) = 0x100 + i;
            *(dw*)raddr_(ds, 0xCCFD + i*2) = 0x200 + i;
        }
        wounds = 3; diff_parm = 1;                       /* easy: +0xC  */
        ab_res("ai easy", ai_param_fetch_lifted, ai_param_fetch, 0);
        wounds = 0; diff_parm = 4;
        ab_res("ai hard", ai_param_fetch_lifted, ai_param_fetch, 0);
        wounds = 40; diff_parm = 2;                      /* clamp path  */
        ab_res("ai clamp", ai_param_fetch_lifted, ai_param_fetch, 0);

        /* target_pri_decay: positive/negative/zero × cooldown gate   */
        pri_best = 5; cool_gate = 0;
        ab_res("pri dec", target_pri_decay_lifted, target_pri_decay, 0);
        pri_best = 5; cool_gate = 1;
        ab_res("pri cool", target_pri_decay_lifted, target_pri_decay, 0);
        pri_best = 0x80; cool_gate = 0;
        ab_res("pri neg", target_pri_decay_lifted, target_pri_decay, 0);
        pri_best = 0; cool_gate = 0;
        ab_res("pri zero", target_pri_decay_lifted, target_pri_decay, 0);
        fprintf(stderr, "  (render/objsys/ai: lifted vs C, %d checks)\n", checks);
    }

    /* ---- frame dispatch: compose/flip/vblank adapter tables --------
     * All 5 adapter indices exercised through every table; only the MCGA
     * bodies resolve in this build — other cases hit the identical
     * unresolved-jump path in both impls. farcall_ptr_a9e needs ds:0xA9E. */
    {
        /* the 64K frame copies run ds:0 -> es:0 through seg_flip/draw/
         * screen — pin them to scratch high mem so the fixture doesn't
         * stomp the image the digit-patcher test reads afterwards      */
        dw s0 = seg_flip, s1 = seg_draw, s2 = seg_screen;
        seg_flip = 0x9000; seg_draw = 0x9800; seg_screen = 0xA000;
        adapter_id = 3;
        *(dw*)raddr_(ds, 0x0A9E) = 0xFFFF;
        ab_res("farcall none", farcall_ptr_a9e_lifted, farcall_ptr_a9e, 0);
        *(dw*)raddr_(ds, 0x0A9E) = 7;
        ab_res("farcall live", farcall_ptr_a9e_lifted, farcall_ptr_a9e, 0);
        *(dw*)raddr_(ds, 0x0A9E) = 0xFFFF;

        adapter_id = 3;
        ab_res("compose cond3", compose_frame_cond_lifted, compose_frame_cond, 0);
        adapter_id = 1;
        ab_res("compose cond1", compose_frame_cond_lifted, compose_frame_cond, 0);
        ab_res("compose clr", compose_frame_cond_e11962_lifted,
               compose_frame_cond_e11962, 0);
        for (int i = 0; i < 5; i++){
            adapter_id = i;
            ab_res("compose disp", compose_frame_e11983_lifted,
                   compose_frame_e11983, 0);
            ab_res("compose", compose_frame_lifted, compose_frame, 0);
            ab_res("flip", flip_frame_lifted, flip_frame, 0);
            ab_res("border", set_border_color_lifted, set_border_color, 0);
            ab_res("clrbk", clear_backbuf_lifted, clear_backbuf, 0);
            ab_res("adpflip", adapter_compose_flip_lifted,
                   adapter_compose_flip, 0);
            ab_res("compflip", compose_flip_lifted, compose_flip, 0);
        }
        adapter_id = 3;
        es = seg_flip; ds = seg_draw;
        ab_res("present mcga", present_mcga_lifted, present_mcga, 0);
        es = seg_screen; ds = seg_flip;
        ab_res("flip mcga", flip_mcga_lifted, flip_mcga, 0);
        ds = 0x0E8A;
        blit_sel = 1;
        ab_res("flip e11a5c", flip_frame_e11a5c_lifted, flip_frame_e11a5c, 0);
        ab_res("flip tail", flip_frame_e11b7a_lifted, flip_frame_e11b7a, 0);
        dx = 0x3DA;                       /* head sets the status port   */
        ab_res("vblank mid", vblank_wait_e11b84_lifted, vblank_wait_e11b84, 0);
        ab_res("vblank", vblank_wait_lifted, vblank_wait, 0);
        ab_res("compose mcga", compose_mcga_lifted, compose_mcga, 0);
        ax = 5;
        ab_res("mcga dac", compose_mcga_11c0d_lifted, compose_mcga_11c0d, 0);
        ab_res("locret c27", locret_11c27_lifted, locret_11c27, 0);
        seg000_1_0d4a = 0x1234; seg000_1_0d4c = 0x56;
        ab_res("sprst copy", spr_state_copy_b_lifted, spr_state_copy_b, 0);
        di = 2;
        ab_res("adpflip mid", adapter_compose_flip_e11c4c_lifted,
               adapter_compose_flip_e11c4c, 0);
        ab_res("adpflip 53", adapter_compose_flip_e11c53_lifted,
               adapter_compose_flip_e11c53, 0);
        ab_res("adpflip 59", adapter_compose_flip_e11c59_lifted,
               adapter_compose_flip_e11c59, 0);
        seg_flip = s0; seg_draw = s1; seg_screen = s2;   /* unpin segs */
        fprintf(stderr, "  (frame dispatch: lifted vs C, %d checks)\n", checks);
    }

    /* ---- clip/dirty scan: video_bufs_setup -> clip_jt, clip_go_mcga
     * rectreg sweep, mcga_dirty_update rect walk. Column tables live at
     * ds:0xDD5 (bit masks), 0xF1F/0xF23 (column bytes), 0xE../0x13..
     * record fields; empty tables exercise the early-out scan, a rigged
     * slot exercises the hit path (cx>=0x200 skips the rest_* fill).  */
    {
        dw s0 = seg_flip, s1 = seg_draw, s2 = seg_screen;
        seg_flip = 0x9000; seg_draw = 0x9800; seg_screen = 0xA000;
        for (int i = 0; i < 5; i++){
            adapter_id = i;
            ab_res("bufinit", video_bufs_init_lifted, video_bufs_init, 0);
            ab_res("bufsetup", video_bufs_setup_lifted, video_bufs_setup, 0);
            ab_res("buf mid", video_bufs_setup_e126c9_lifted,
                   video_bufs_setup_e126c9, 0);
        }
        /* bufsel_mcga set seg_flip=seg_draw+0x1000 etc — repin scratch */
        seg_flip = 0x9000; seg_draw = 0x9800; seg_screen = 0xA000;

        /* clip_go_mcga: empty mask bytes -> pure 32-slot dec scan     */
        memset(raddr_(ds, 0x0F1F), 0, 0x20);
        memset(raddr_(ds, 0x0F23), 0, 0x20);
        *(db*)raddr_(ds, 0x0DD5 + 0) = 0xFF;      /* bit-mask table      */
        adapter_id = 3; blit_sel = 0;
        ab_res("clip mcga", clip_go_mcga_lifted, clip_go_mcga, 0);
        blit_sel = 1;
        ab_res("clip sel1", clip_go_mcga_lifted, clip_go_mcga, 0);
        blit_sel = 0; rectreg_base = 5;
        ab_res("clip mid47", clip_go_mcga_e13047_lifted,
               clip_go_mcga_e13047, 0);
        rectreg_base = 3;
        ab_res("clip midb0", clip_go_mcga_e130b0_lifted,
               clip_go_mcga_e130b0, 0);
        rectreg_base = 2;
        ab_res("clip midb3", clip_go_mcga_e130b3_lifted,
               clip_go_mcga_e130b3, 0);
        /* hit path: mask&column nonzero, gate byte positive, cx>=0x200
         * so the rest_* scanline fills stay out of the test           */
        rectreg_base = 0; blit_sel = 0;
        *(db*)raddr_(ds, 0x0F1F) = 0xFF;
        *(db*)raddr_(ds, 0x0F23) = 0x00;
        *(db*)raddr_(ds, 0x0DD5) = 0x01;
        *(db*)raddr_(ds, 0x0EFF) = 1;
        *(db*)raddr_(ds, 0x0E5F) = 2;             /* ch=2 -> cx=0x200    */
        ab_res("clip hit", clip_go_mcga_lifted, clip_go_mcga, 0);
        *(db*)raddr_(ds, 0x0F1F) = 0;

        /* mcga_dirty_update: adapter gate, then 1-rect 2-run blit     */
        adapter_id = 0; dirtyrect_ptr = 1;
        ab_res("dirty gate", mcga_dirty_update_lifted, mcga_dirty_update, 0);
        adapter_id = 3; dirtyrect_ptr = 0;
        ab_res("dirty empty", mcga_dirty_update_lifted, mcga_dirty_update, 0);
        dirtyrect_ptr = 1;
        *(db*)raddr_(ds, 0x1223) = 0x00; *(db*)raddr_(ds, 0x12AF) = 0x10;
        *(db*)raddr_(ds, 0x0F67) = 2;                      /* width      */
        *(db*)raddr_(ds, 0x0FF3) = 2;                      /* run count  */
        *(db*)raddr_(ds, 0x107F) = 0; *(db*)raddr_(ds, 0x110B) = 0;
        *(db*)raddr_(ds, 0x1197) = 0;                      /* row idx    */
        ab_res("dirty 1", mcga_dirty_update_lifted, mcga_dirty_update, 0);
        ab_res("dirty mid3c", mcga_dirty_update_e1393c_lifted,
               mcga_dirty_update_e1393c, 0);
        dirtyrect_ptr = 1;
        ab_res("dirty mid43", mcga_dirty_update_e13943_lifted,
               mcga_dirty_update_e13943, 0);
        /* e13975 enters mid-run: bx/ax/si/dx/es/blitgo_c preset       */
        bx = 0; ax = 0; si = 0x1000; dx = 2; blitgo_c = 1;
        es = seg_flip; dirtyrect_ptr = 0;
        ab_res("dirty mid75", mcga_dirty_update_e13975_lifted,
               mcga_dirty_update_e13975, 0);
        seg_flip = s0; seg_draw = s1; seg_screen = s2;
        fprintf(stderr, "  (clip/dirty: lifted vs C, %d checks)\n", checks);
    }

    /* ---- HUD weapon panel + fx overlay: digit-count loop, evac meter
     * word loads, 11/32-record clip-column fills; both tails re-run
     * video_bufs_setup so the pinned segs/empty clip tables apply. --- */
    {
        dw s0 = seg_flip, s1 = seg_draw, s2 = seg_screen;
        seg_flip = 0x9000; seg_draw = 0x9800; seg_screen = 0xA000;
        adapter_id = 3;
        memset(raddr_(ds, 0x0F1F), 0, 0x20);
        memset(raddr_(ds, 0x0F23), 0, 0x20);
        weapon_sel = 2; mst_a7af = 0;
        ab_res("hud sel2", hud_weapon_update_lifted, hud_weapon_update, 0);
        weapon_sel = 0; flash_period = 1;      /* bx==0 & >=0 -> al++   */
        ab_res("hud sel0", hud_weapon_update_lifted, hud_weapon_update, 0);
        weapon_sel = 3;                        /* bx==3 -> bkey fetch   */
        ab_res("hud sel3", hud_weapon_update_lifted, hud_weapon_update, 0);
        mst_a7af = 1; weapon_sel = 1;          /* si+=5 mission offset  */
        ab_res("hud mst", hud_weapon_update_lifted, hud_weapon_update, 0);
        mst_a7af = 0;
        bx = 1; si = 4;                        /* e17b22: shl+table mid */
        ab_res("hud mid22", hud_weapon_update_e17b22_lifted,
               hud_weapon_update_e17b22, 0);
        bx = 0; al = 25;                       /* e17b38: ammo fetch    */
        ab_res("hud mid38", hud_weapon_update_e17b38_lifted,
               hud_weapon_update_e17b38, 0);
        al = 42; si = 0;                       /* e17b4a: count loop    */
        ab_res("hud mid4a", hud_weapon_update_e17b4a_lifted,
               hud_weapon_update_e17b4a, 0);
        ab_res("hud midc5", hud_weapon_update_e17bc5_lifted,
               hud_weapon_update_e17bc5, 0);

        meter_b = 0;                           /* bx|bx==0 -> ret       */
        ab_res("fx zero", fx_overlay_fill_lifted, fx_overlay_fill, 0);
        meter_b = 3;                           /* odd -> offset stays 0 */
        ab_res("fx odd", fx_overlay_fill_lifted, fx_overlay_fill, 0);
        meter_b = 6;                           /* even -> fx_off = 4    */
        ab_res("fx even", fx_overlay_fill_lifted, fx_overlay_fill, 0);
        bx = 8;
        ab_res("fx midfd", fx_overlay_fill_e17cfd_lifted,
               fx_overlay_fill_e17cfd, 0);
        bx = 4;
        ab_res("fx mid0e", fx_overlay_fill_e17d0e_lifted,
               fx_overlay_fill_e17d0e, 0);
        si = 0; cx = 0x20;
        ab_res("fx mid32", fx_overlay_fill_e17d32_lifted,
               fx_overlay_fill_e17d32, 0);
        seg_flip = s0; seg_draw = s1; seg_screen = s2;
        fprintf(stderr, "  (hud/fx: lifted vs C, %d checks)\n", checks);
    }

    /* ---- tile/glyph blitters: flipbuf dispatches tileblit_jt (case 3 =
     * tile_blit_mcga -> 8-row color-translate store into es=seg_flip);
     * glyph_blit adds the wparm masks; mapcols_draw drives 32 columns.
     * Pattern/xlate tables at ds:0x3B70..0x3D70+0x100, row-offset table
     * at ds:0xBD5. es pinned to seg_flip scratch for the stores. ------ */
    {
        dw s0 = seg_flip, s1 = seg_draw, s2 = seg_screen;
        seg_flip = 0x9000; seg_draw = 0x9800; seg_screen = 0xA000;
        ss = 0x8000; sp = 0xFFFE;             /* guest stack in scratch   */
        draw_col = 4; draw_row = 0x20;
        ax = 2;                               /* glyph/tile idx          */
        si = 0x1B52;
        for (int i = 0; i < 5; i++){
            adapter_id = i;
            ab_res("tileblit", tile_blit_flipbuf_lifted, tile_blit_flipbuf, 0);
        }
        adapter_id = 3;
        es = seg_flip;
        ab_res("tile mcga", tile_blit_mcga_lifted, tile_blit_mcga, 0);
        ds = seg_data;
        *(dw*)raddr_(ds, 0x0A82) = 1;         /* draw_row table idx      */
        *(dw*)raddr_(ds, 0x0A80) = 2;         /* draw_col                */
        ax = 3;
        ab_res("tilerow", tile_row_mcga_lifted, tile_row_mcga, 0);
        si = 0x2000; di = 0; cx = 8; bx = 0;  /* e118c6: loop mid-entry  */
        ab_res("tilerow mid", tile_row_mcga_e118c6_lifted,
               tile_row_mcga_e118c6, 0);
        draw_col = 39; draw_row = 0xC0;       /* col wrap + row clamp    */
        ab_res("tile wrap", tile_blit_mcga_lifted, tile_blit_mcga, 0);
        draw_col = 4; draw_row = 0x20;
        ax = 5;
        ab_res("glyphblit", glyph_blit_mcga_lifted, glyph_blit_mcga, 0);
        si = 0x2100; di = 0x80; cx = 0x408; bx = 0;  /* ch=8 loop mid    */
        ab_res("glyph mid", glyph_blit_mcga_e10d9e_lifted,
               glyph_blit_mcga_e10d9e, 0);
        ax = 1;
        ab_res("glyphput", glyph_put_mcga_10c1c_lifted, glyph_put_mcga_10c1c, 0);
        ax = 2;
        ab_res("glyphput2", glyph_put2_mcga_lifted, glyph_put2_mcga, 0);

        map_base = 0x40;                      /* raw 32-col path         */
        ab_res("mapcols raw", mapcols_draw_lifted, mapcols_draw, 0);
        map_base = 3;                         /* cell-table path         */
        ab_res("mapcols map", mapcols_draw_lifted, mapcols_draw, 0);
        map_base = 0x8000;                    /* sign bit -> raw path    */
        ab_res("mapcols sgn", mapcols_draw_lifted, mapcols_draw, 0);
        ab_res("mapcols mid-dc", mapcols_draw_e1bcdc_lifted,
               mapcols_draw_e1bcdc, 0);
        cx = 3;
        ab_res("mapcols mid-df", mapcols_draw_e1bcdf_lifted,
               mapcols_draw_e1bcdf, 0);
        bx = 2;
        ab_res("mapcols mid-ed", mapcols_draw_e1bced_lifted,
               mapcols_draw_e1bced, 0);
        bx = 1; cx = 2;
        ab_res("mapcols mid-f4", mapcols_draw_e1bcf4_lifted,
               mapcols_draw_e1bcf4, 0);
        seg_flip = s0; seg_draw = s1; seg_screen = s2;
        fprintf(stderr, "  (tile/glyph: lifted vs C, %d checks)\n", checks);
    }

    /* ---- blitflag dirty-flag blit + scroll/pan handlers: blitflag_
     * go_mcga sweeps 32 rect slots (flag at [si+0xF67], runs [si+0xFF3],
     * row [si+0x1197], col pair [si+0x107F]/[si+0x110B]) copying
     * seg_draw->seg_flip; scroll_go_* ind-call through scroll_tbl_x with
     * the far-thunk sp restore, then run scroll_edge_x. -------------- */
    {
        dw s0 = seg_flip, s1 = seg_draw, s2 = seg_screen;
        seg_flip = 0x9000; seg_draw = 0x9800; seg_screen = 0xA000;
        ss = 0x8000; sp = 0xFFFE;
        adapter_id = 3;
        ds = seg_data; es = seg_flip;
        memset(raddr_(ds, 0x0F67), 0, 0x20);  /* all flags clear        */
        blit_sel = 0;
        ab_res("blitflag", blitflag_go_mcga_lifted, blitflag_go_mcga, 0);
        blit_sel = 1;
        ab_res("blitflag sel1", blitflag_go_mcga_lifted, blitflag_go_mcga, 0);
        blit_sel = 0;
        /* rig slot 1: 2 runs of 4 words from row 0, col 2              */
        *(db*)raddr_(ds, 1 + 0x0F67) = 4;     /* width                  */
        *(db*)raddr_(ds, 1 + 0x0FF3) = 2;     /* run count              */
        *(db*)raddr_(ds, 1 + 0x1197) = 0;     /* row idx                */
        *(db*)raddr_(ds, 1 + 0x107F) = 2;     /* col lo                 */
        *(db*)raddr_(ds, 1 + 0x110B) = 0;     /* col hi                 */
        ab_res("blitflag hit", blitflag_go_mcga_lifted, blitflag_go_mcga, 0);
        dirtyrect_ptr = 0x1F; rectreg_off = 0;
        ab_res("blitflag mid4d", blitflag_go_mcga_e1384d_lifted,
               blitflag_go_mcga_e1384d, 0);
        *(db*)raddr_(ds, 2 + 0x0F67) = 3;
        *(db*)raddr_(ds, 2 + 0x0FF3) = 1;
        dirtyrect_ptr = 2; si = 2; al = 3;    /* live si = slot idx     */
        ab_res("blitflag mid5c", blitflag_go_mcga_e1385c_lifted,
               blitflag_go_mcga_e1385c, 0);
        bx = 0; dx = 2; ax = seg_data; bp = seg_draw; blitgo_a = 4;
        blitgo_c = 2;
        ab_res("blitflag mid84", blitflag_go_mcga_e13884_lifted,
               blitflag_go_mcga_e13884, 0);
        dirtyrect_ptr = 0;
        ab_res("blitflag mida5", blitflag_go_mcga_e138a5_lifted,
               blitflag_go_mcga_e138a5, 0);

        /* scroll_go_*: edge procs are still lifted (called identically
         * from both impls); index byte at ds:0xAB4 picks the table slot */
        adapter_id = 3;
        *(dw*)raddr_(ds, 0x0AB4) = 3;
        ds = seg_data; es = seg_draw;
        ab_res("scroll a", scroll_go_a_lifted, scroll_go_a, 0);
        ab_res("scroll b", scroll_go_b_lifted, scroll_go_b, 0);
        ab_res("scroll c", scroll_go_c_lifted, scroll_go_c, 0);
        ab_res("scroll d", scroll_go_d_lifted, scroll_go_d, 0);
        ab_res("scroll e", scroll_go_e_lifted, scroll_go_e, 0);
        ab_res("scroll f", scroll_go_f_lifted, scroll_go_f, 0);
        ab_res("scroll g", scroll_go_g_lifted, scroll_go_g, 0);
        ab_res("scroll h", scroll_go_h_lifted, scroll_go_h, 0);
        *(dw*)raddr_(ds, 0x0AB4) = 0;
        adapter_id = 0;
        ab_res("scroll a0", scroll_go_a_lifted, scroll_go_a, 0);
        seg_flip = s0; seg_draw = s1; seg_screen = s2;
        fprintf(stderr, "  (blitflag/scroll: lifted vs C, %d checks)\n",
                checks);
    }

    /* ---- scroll_edge_a..d: per-edge cell_tile_compose sweep loops.
     * [ds:0xC7FD]&4 selects the double-step variant; the staged fields
     * at 0xA80/0xA82/0xDBE5/0xDBE7 + bounds at 0x96F0..0x96FA drive the
     * compose calls. ds pinned to seg_data; video segs to scratch. --- */
    {
        dw s0 = seg_flip, s1 = seg_draw, s2 = seg_screen;
        seg_flip = 0x9000; seg_draw = 0x9800; seg_screen = 0xA000;
        ss = 0x8000; sp = 0xFFFE;
        ds = seg_data; adapter_id = 3;
        *(dw*)raddr_(ds, 0x0C801) = 3;        /* scroll off a           */
        *(dw*)raddr_(ds, 0x0C803) = 5;        /* scroll off b           */
        *(db*)raddr_(ds, 0x0C7FD) = 0;        /* normal step            */
        ab_res("edge a", scroll_edge_a_lifted, scroll_edge_a, 0);
        ab_res("edge d", scroll_edge_d_lifted, scroll_edge_d, 0);
        ab_res("edge b", scroll_edge_b_lifted, scroll_edge_b, 0);
        ab_res("edge c", scroll_edge_c_lifted, scroll_edge_c, 0);
        *(db*)raddr_(ds, 0x0C7FD) = 4;        /* double-step variant    */
        ab_res("edge a x2", scroll_edge_a_lifted, scroll_edge_a, 0);
        ab_res("edge d x2", scroll_edge_d_lifted, scroll_edge_d, 0);
        ab_res("edge b x2", scroll_edge_b_lifted, scroll_edge_b, 0);
        ab_res("edge c x2", scroll_edge_c_lifted, scroll_edge_c, 0);
        /* mid-entries: ax/cx live for the store mids; fields preset    */
        ax = 2; cx = 0x18;
        ab_res("edge a mid9f", scroll_edge_a_e1429f_lifted,
               scroll_edge_a_e1429f, 0);
        *(dw*)raddr_(ds, 0x96F2) = 0x18; *(dw*)raddr_(ds, 0x96F0) = 0;
        ab_res("edge a midac", scroll_edge_a_e142ac_lifted,
               scroll_edge_a_e142ac, 0);
        ax = 1;
        ab_res("edge d mided", scroll_edge_d_e142ed_lifted,
               scroll_edge_d_e142ed, 0);
        *(dw*)raddr_(ds, 0x96F0) = 0;
        ab_res("edge d midf6", scroll_edge_d_e142f6_lifted,
               scroll_edge_d_e142f6, 0);
        ax = 0; cx = 8; bx = 0x18;
        ab_res("edge b midcd", scroll_edge_b_e143cd_lifted,
               scroll_edge_b_e143cd, 0);
        *(dw*)raddr_(ds, 0x96F2) = 0x18; *(dw*)raddr_(ds, 0x96F0) = 0;
        *(dw*)raddr_(ds, 0x96F8) = 8; *(dw*)raddr_(ds, 0x96FA) = 0;
        ab_res("edge b mide1", scroll_edge_b_e143e1_lifted,
               scroll_edge_b_e143e1, 0);
        ab_res("edge b mid30", scroll_edge_b_e14430_lifted,
               scroll_edge_b_e14430, 0);
        ax = 0; cx = 8; bx = 0x18;
        ab_res("edge c mid45", scroll_edge_c_e14345_lifted,
               scroll_edge_c_e14345, 0);
        *(dw*)raddr_(ds, 0x96F2) = 0x18; *(dw*)raddr_(ds, 0x96F0) = 0;
        *(dw*)raddr_(ds, 0x96F8) = 8; *(dw*)raddr_(ds, 0x96FA) = 0;
        ab_res("edge c mid59", scroll_edge_c_e14359_lifted,
               scroll_edge_c_e14359, 0);
        ab_res("edge c mida9", scroll_edge_c_e143a9_lifted,
               scroll_edge_c_e143a9, 0);
        seg_flip = s0; seg_draw = s1; seg_screen = s2;
        fprintf(stderr, "  (scroll_edge: lifted vs C, %d checks)\n",
                checks);
    }

    /* ---- cell->tile compose chain: px_to_cell shifts, map_probe_xy
     * bounds + map byte fetch, map_rowhdr_get two-level row tables,
     * draw_tile_compose sprrow_tbl dispatch; bar_tandy fills the
     * ss:bp status-bar frame. -----------------------------------------*/
    {
        dw s0 = seg_flip, s1 = seg_draw, s2 = seg_screen;
        seg_flip = 0x9000; seg_draw = 0x9800; seg_screen = 0xA000;
        ss = 0x8000; sp = 0xFFFE;
        ds = seg_data; adapter_id = 3;
        probe_px = 0x48; probe_py = 0x30;     /* -> cell 9,6            */
        ab_res("px2cell", px_to_cell_lifted, px_to_cell, 0);
        cell_x = 9; cell_y = 6;              /* in-bounds probe        */
        ab_res("probe ok", map_probe_xy_lifted, map_probe_xy, 0);
        cell_x = 0x20;                        /* x OOB -> CF=1          */
        ab_res("probe xoob", map_probe_xy_lifted, map_probe_xy, 0);
        cell_x = 9; cell_y = 0x40;           /* y OOB                  */
        ab_res("probe yoob", map_probe_xy_lifted, map_probe_xy, 0);
        al = 0x77;
        ab_res("probe mid9d", map_probe_xy_e1b29d_lifted,
               map_probe_xy_e1b29d, 0);
        ab_res("probe midb2", map_probe_xy_e1b2b2_lifted,
               map_probe_xy_e1b2b2, 0);
        cell_x = 9; cell_y = 6;
        ax = 2; probe_px = 0x25; probe_py = 0x31;
        ab_res("rowhdr", map_rowhdr_get_lifted, map_rowhdr_get, 0);
        draw_col = 4; draw_row = 0x20; tmap_9812 = 0;
        ab_res("drawtile", draw_tile_compose_lifted, draw_tile_compose, 0);
        ab_res("celltile", cell_tile_compose_lifted, cell_tile_compose, 0);
        /* bar_tandy: ss:bp frame fill; dx>0 words then bx<=0x16 pairs  */
        bp = 0x200; si = 0x400; dx = 3; bx = 5;
        ab_res("bar", bar_tandy_lifted, bar_tandy, 0);
        dx = 0; bx = 0x20;                    /* bx>0x16 clamps         */
        ab_res("bar clamp", bar_tandy_lifted, bar_tandy, 0);
        seg_flip = s0; seg_draw = s1; seg_screen = s2;
        fprintf(stderr, "  (cellcompose: lifted vs C, %d checks)\n",
                checks);
    }

    /* ---- tilemap leaf helpers: tile_lookup cell attrs, obj_cell_tile
     * chain, mcga LUT build/copy procs, half-tile sprrow writers,
     * render_bit_row mask scan, tile_variant_sel + bar_draw frame. --- */
    {
        dw s0 = seg_flip, s1 = seg_draw, s2 = seg_screen, s3 = seg_resbuf;
        seg_flip = 0x9000; seg_draw = 0x9800; seg_screen = 0xA000;
        seg_resbuf = 0xB000;
        ss = 0x8000; sp = 0xFFFE;
        ds = seg_data; adapter_id = 3;
        ax = 0x25; probe_px = 0x48; probe_py = 0x30;
        ab_res("tile_lookup", tile_lookup_lifted, tile_lookup, 0);
        ax = 0x7F; probe_px = 0xFF; probe_py = 0xFF;
        ab_res("tile_lookup hi", tile_lookup_lifted, tile_lookup, 0);
        obj_sx = 0x48; obj_sy = 0x30; bx = 0x11; si = 0x22;
        cell_x = 9; cell_y = 6;
        ab_res("obj_cell_tile", obj_cell_tile_lifted, obj_cell_tile, 0);
        cell_x = 9; cell_y = 6;
        ab_res("obj_cell mid", obj_cell_tile_e1b34f_lifted,
               obj_cell_tile_e1b34f, 0);
        adapter_id = 3; ds = seg_data;
        ab_res("mcga lut", build_mcga_lut_lifted, build_mcga_lut, 0);
        adapter_id = 0;
        ab_res("mcga lut a0", build_mcga_lut_lifted, build_mcga_lut, 0);
        adapter_id = 3;
        ab_res("mcga lut mid", build_mcga_lut_e14b1a_lifted,
               build_mcga_lut_e14b1a, 0);
        ab_res("blitdst", blit_dst_patch_lifted, blit_dst_patch, 0);
        ds = seg_data;
        ab_res("tilemap_init", tilemap_init_lifted, tilemap_init, 0);
        ds = seg_data; es = 0xA800;
        ab_res("rdr copy", rdr_copy_mcga_lifted, rdr_copy_mcga, 0);
        draw_col = 4; draw_row = 0x20; si = 0x3F22;
        ab_res("tile_blit", tile_blit_lifted, tile_blit, 0);
        si = 0x3F22;
        ab_res("tile 3f22", tile_draw_3f22_lifted, tile_draw_3f22, 0);
        /* half-tile writers: ds=resbuf so [bx-1000]/[bx-F00] hit the
         * LUTs at resbuf:F000/F100; src tile at resbuf:2000          */
        ds = seg_resbuf;
        for (int i = 0; i < 0x200; i++)
            mem[(seg_resbuf << 4) + 0xF000 + i] = i;
        for (int i = 0; i < 16; i++)
            mem[(seg_resbuf << 4) + 0x2000 + i] = 0x10 + i;
        si = 0x2000; ax = 2; di = 0x100;
        ab_res("sprrow b", sprrow_mcga_b_lifted, sprrow_mcga_b, 0);
        si = 0x2000; di = 0x100; es = 0x9000; cx = 4; bh = 0;
        ab_res("sprrow b mid", sprrow_mcga_b_e1204d_lifted,
               sprrow_mcga_b_e1204d, 0);
        si = 0x2000; ax = 2; di = 0x100;
        ab_res("sprrow c", sprrow_mcga_c_lifted, sprrow_mcga_c, 0);
        si = 0x2000; di = 0x100; es = 0x9000; cx = 4; bh = 0;
        ab_res("sprrow c mid", sprrow_mcga_c_e121b4_lifted,
               sprrow_mcga_c_e121b4, 0);
        /* bit scan: '0' fill + sel_mask bits -> rec5_cmp decimal adds */
        ds = seg_data;
        rec_ptr_a = 0x6000; sel_mask = 0xA005;
        ab_res("bitrow", render_bit_row_lifted, render_bit_row, 0);
        si = 5; al = 0x30;
        ab_res("bitrow fill", render_bit_row_e182df_lifted,
               render_bit_row_e182df, 0);
        bx = 0x0F; sel_mask = 0x00FF;
        ab_res("bitrow scan", render_bit_row_e182ec_lifted,
               render_bit_row_e182ec, 0);
        bx = 0x0F; sel_mask = 0x00FF;
        ab_res("bitrow next", render_bit_row_e18302_lifted,
               render_bit_row_e18302, 0);
        /* alert->tile variant + status-bar frame fill (bar_jt -> tandy) */
        alert_aux = 0; alert_lvl = 0xA0;
        *(dw*)raddr_(ss, 0x272) = 0x4000;      /* bar frame base */
        ab_res("tilevar", tile_variant_sel_lifted, tile_variant_sel, 0);
        alert_aux = 1; alert_lvl = 0x40;
        ab_res("tilevar aux", tile_variant_sel_lifted, tile_variant_sel, 0);
        alert_aux = 0; alert_lvl = 0x20;
        ab_res("tilevar lo", tile_variant_sel_lifted, tile_variant_sel, 0);
        *(dw*)raddr_(ss, 0x272) = 0x4000;
        ab_res("bar_draw", bar_draw_lifted, bar_draw, 0);
        bp = 0x4004; di = 6; bx = 3; dx = 2; ax = 0x2211;
        ab_res("bar fill", bar_draw_e17c77_lifted, bar_draw_e17c77, 0);
        bp = 0x4004; di = 6; bx = 0x30;
        ab_res("bar clamp", bar_draw_e17c87_lifted, bar_draw_e17c87, 0);
        bp = 0x4004; di = 6; bx = 4; ax = 0x5566;
        ab_res("bar mark", bar_draw_e17c8f_lifted, bar_draw_e17c8f, 0);
        seg_flip = s0; seg_draw = s1; seg_screen = s2; seg_resbuf = s3;
        fprintf(stderr, "  (tilemap-leaf: lifted vs C, %d checks)\n",
                checks);
    }

    /* ---- cell-strip renderer: draw_cell_strip edge column (cellgfx_jt
     * dispatch -> cellgfx_mcga LUT blit), glyph fetch + mids. -------- */
    {
        dw s0 = seg_flip, s1 = seg_draw, s2 = seg_screen;
        seg_flip = 0x9000; seg_draw = 0x9800; seg_screen = 0xA000;
        ss = 0x8000; sp = 0xFFFE;
        ds = seg_data; adapter_id = 3;
        draw_cel_dbdd = 0x1000; scroll_cnt = 0;
        ab_res("cellstrip", draw_cell_strip_lifted, draw_cell_strip, 0);
        draw_cel_dbdd = 0x1000; scroll_cnt = 5;
        ab_res("cellstrip sc5", draw_cell_strip_lifted,
               draw_cell_strip, 0);
        draw_cel_dbdd = 0x1000; scroll_cnt = 0x8000;
        ab_res("cellstrip neg", draw_cell_strip_lifted,
               draw_cell_strip, 0);
        draw_cel_dbdd = 0x1000;
        ab_res("cellstrip e044", draw_cell_strip_e1b044_lifted,
               draw_cell_strip_e1b044, 0);
        draw_cel_dbdd = 0x1000; cx = 0x10;
        ab_res("cellstrip e047", draw_cell_strip_e1b047_lifted,
               draw_cell_strip_e1b047, 0);
        draw_cel_dbdd = 0x1000; bx = 3;
        ab_res("cellstrip e054", draw_cell_strip_e1b054_lifted,
               draw_cell_strip_e1b054, 0);
        draw_cel_dbdd = 0x1000; bx = 0x100; cx = 6;
        ab_res("cellstrip e05b", draw_cell_strip_e1b05b_lifted,
               draw_cell_strip_e1b05b, 0);
        draw_cel_dbdd = 0x1000;
        ab_res("cellstrip e06d", draw_cell_strip_e1b06d_lifted,
               draw_cell_strip_e1b06d, 0);
        bp = 0x2000; adapter_id = 3; es = 0x9800;
        ab_res("glyphfetch", cell_glyph_fetch_lifted, cell_glyph_fetch, 0);
        bp = 0x2000; adapter_id = 1; es = 0x9800;
        ab_res("glyphfetch t", cell_glyph_fetch_lifted,
               cell_glyph_fetch, 0);
        adapter_id = 3;
        si = 0x1B52; di = 0x2000; bp = 0x2000; es = 0x9800;
        ab_res("glyph mid", cell_glyph_fetch_e1b0b1_lifted,
               cell_glyph_fetch_e1b0b1, 0);
        ax = 7; scroll_cnt = 0; es = 0x9800; di = 0x2000; bp = 0x2000;
        ab_res("cellgfx", cellgfx_mcga_lifted, cellgfx_mcga, 0);
        ax = 7; scroll_cnt = 1; es = 0x9800; di = 0x2000; bp = 0x2000;
        ab_res("cellgfx odd", cellgfx_mcga_lifted, cellgfx_mcga, 0);
        si = 0x1B52; di = 0x2000; bp = 0x2000; es = 0x9800;
        ab_res("cellgfx mid", cellgfx_mcga_e1b172_lifted,
               cellgfx_mcga_e1b172, 0);
        si = 0x1B52; di = 0x2000; bp = 0x2000; es = 0x9800;
        cx = 3; bh = 0;
        ab_res("cellgfx rows", cellgfx_mcga_e1b177_lifted,
               cellgfx_mcga_e1b177, 0);
        seg_flip = s0; seg_draw = s1; seg_screen = s2;
        fprintf(stderr, "  (cellstrip: lifted vs C, %d checks)\n",
                checks);
    }

    /* ---- camera pan: target snap, pan-flag detect, view drift/input,
     * clamp_obj_pos window clamp, obj_lookup_word field store. ------- */
    {
        ss = 0x8000; sp = 0xFFFE;
        ds = seg_data;
        /* cam_pan_detect: x/y quarter-px compare -> pan_flags bits    */
        cam_tgt_x = 0x0200; cam_tgt_y = 0x0180;
        cam_px_x = 0x80; cam_px_y = 0x60; cam_org_x = 0x80;
        cam_9681 = 0x30; si = 0;
        ab_res("pandetect", cam_pan_detect_lifted, cam_pan_detect, 0);
        cam_tgt_x = 0x0100; cam_tgt_y = 0x0200;
        cam_px_x = 0x80; cam_px_y = 0x60; cam_org_x = 0x80;
        cam_9681 = 0x30; si = 0;
        ab_res("pandetect l", cam_pan_detect_lifted, cam_pan_detect, 0);
        cam_tgt_x = 0x0244; cam_tgt_y = 0x0184;   /* subtile bit set   */
        cam_px_x = 0x80; cam_px_y = 0x60;
        ab_res("pandetect st", cam_pan_detect_lifted, cam_pan_detect, 0);
        cam_tgt_x = 0x0080; cam_px_x = 0x80;
        ab_res("pandetect =x", cam_pan_detect_lifted, cam_pan_detect, 0);
        ab_res("pandet xdec", cam_pan_detect_e169de_lifted,
               cam_pan_detect_e169de, 0);
        ab_res("pandet y", cam_pan_detect_e169e5_lifted,
               cam_pan_detect_e169e5, 0);
        ab_res("pandet ydec", cam_pan_detect_e16a07_lifted,
               cam_pan_detect_e16a07, 0);
        si = 0x0B;
        ab_res("scrollnop", scroll_nop_lifted, scroll_nop, 0);
        /* clamp_obj_pos: obj x/y at ds:[bx-...] vs cam_tgt window     */
        bx = 0x5000;
        *(db*)raddr_(ds, bx - 0x4085) = 0x50;
        *(db*)raddr_(ds, bx - 0x4041) = 0;
        *(db*)raddr_(ds, bx - 0x4063) = 0x60;
        *(db*)raddr_(ds, bx - 0x401F) = 0;
        cam_tgt_x = 0x0100; cam_tgt_y = 0x0080;
        ab_res("clamp", clamp_obj_pos_lifted, clamp_obj_pos, 0);
        *(db*)raddr_(ds, bx - 0x4085) = 0x20;  /* obj below window     */
        cam_tgt_x = 0x0100;
        ab_res("clamp lo", clamp_obj_pos_lifted, clamp_obj_pos, 0);
        *(db*)raddr_(ds, bx - 0x4085) = 0xF0;  /* obj above window     */
        ab_res("clamp hi", clamp_obj_pos_lifted, clamp_obj_pos, 0);
        *(db*)raddr_(ds, bx - 0x4063) = 0x10;
        ab_res("clamp ylo", clamp_obj_pos_lifted, clamp_obj_pos, 0);
        ab_res("clamp 98a", clamp_obj_pos_e1898a_lifted,
               clamp_obj_pos_e1898a, 0);
        ab_res("clamp 991", clamp_obj_pos_e18991_lifted,
               clamp_obj_pos_e18991, 0);
        *(db*)&rec_ptr_a = 0x50;
        ab_res("clamp 996", clamp_obj_pos_e18996_lifted,
               clamp_obj_pos_e18996, 0);
        ab_res("clamp 9aa", clamp_obj_pos_e189aa_lifted,
               clamp_obj_pos_e189aa, 0);
        ax = 0x90;
        ab_res("clamp 9ce", clamp_obj_pos_e189ce_lifted,
               clamp_obj_pos_e189ce, 0);
        /* snap_to_cell: clamp + 4px grid snap + derived org/px/row    */
        bx = 0x5000;
        *(db*)raddr_(ds, bx - 0x4085) = 0x50;
        *(db*)raddr_(ds, bx - 0x4041) = 0;
        *(db*)raddr_(ds, bx - 0x4063) = 0x60;
        *(db*)raddr_(ds, bx - 0x401F) = 0;
        cam_tgt_x = 0x0123; cam_tgt_y = 0x00AB;
        ab_res("snapcell", snap_to_cell_lifted, snap_to_cell, 0);
        ab_res("panapply", cam_pan_apply_lifted, cam_pan_apply, 0);
        /* view_pan_step: parity shift + input/drift pan               */
        view_pan_a9d8 = 0; drift_flag = 0; input_mask = 0x08;
        *(db*)&dpar_2 = 0x40; *(db*)&dpar_3 = 0; *(db*)&dpar_4 = 0x10;
        bx = 0x5000;
        ab_res("panstep", view_pan_step_lifted, view_pan_step, 0);
        view_pan_a9d8 = 1; input_mask = 0x04;
        ab_res("panstep odd", view_pan_step_lifted, view_pan_step, 0);
        drift_flag = 1; *(db*)&dpar_4 = 0xC0;
        ab_res("panstep drift", view_pan_step_lifted, view_pan_step, 0);
        si = 2;
        ab_res("panstep a5", view_pan_step_e1aca5_lifted,
               view_pan_step_e1aca5, 0);
        si = 2;
        ab_res("panstep b2", view_pan_step_e1acb2_lifted,
               view_pan_step_e1acb2, 0);
        drift_flag = 0;
        ab_res("panstep bd", view_pan_step_e1acbd_lifted,
               view_pan_step_e1acbd, 0);
        al = 0x04; bx = 0x20;
        ab_res("panstep d6", view_pan_step_e1acd6_lifted,
               view_pan_step_e1acd6, 0);
        bx = 4;
        ab_res("panstep db", view_pan_step_e1acdb_lifted,
               view_pan_step_e1acdb, 0);
        bx = 0x200;
        ab_res("panstep e3", view_pan_step_e1ace3_lifted,
               view_pan_step_e1ace3, 0);
        bx = 0x50;
        ab_res("panstep ec", view_pan_step_e1acec_lifted,
               view_pan_step_e1acec, 0);
        *(db*)&dpar_4 = 0x60;
        ab_res("panstep fb", view_pan_step_e1acfb_lifted,
               view_pan_step_e1acfb, 0);
        bl = 0x70;
        ab_res("panstep 09", view_pan_step_e1ad09_lifted,
               view_pan_step_e1ad09, 0);
        bx = 0x5000;
        ab_res("panstep 0d", view_pan_step_e1ad0d_lifted,
               view_pan_step_e1ad0d, 0);
        al = 4; bx = 0x5000;
        ab_res("objlookup", obj_lookup_word_lifted, obj_lookup_word, 0);
        fprintf(stderr, "  (campan: lifted vs C, %d checks)\n", checks);
    }

    /* ---- tilemap redraw + draw-list walkers: sprite_param_set 40x24
     * cell grid -> cell_tile_compose, sprite_blit_flagged dispatch,
     * draw_list_walk slot flags, copy_draw_params record loop,
     * scroll_d_direct 40-col draw_tile_compose sweep. ----------------- */
    {
        dw s0 = seg_flip, s1 = seg_draw, s2 = seg_screen;
        seg_flip = 0x9000; seg_draw = 0x9800; seg_screen = 0xA000;
        ss = 0x8000; sp = 0xFFFE;
        ds = seg_data; adapter_id = 3;
        *(db*)raddr_(ds,0xC7FD) = 0;          /* 24-row redraw mode   */
        ab_res("redraw", sprite_param_set_lifted, sprite_param_set, 0);
        *(db*)raddr_(ds,0xC7FD) = 4;          /* 25-row redraw mode   */
        ab_res("redraw +r", sprite_param_set_lifted, sprite_param_set, 0);
        ax = 0; cx = 8; bx = 0x18;
        ab_res("redraw 458", sprite_param_set_e14458_lifted,
               sprite_param_set_e14458, 0);
        *(dw*)raddr_(ds,0x96F2) = 2; *(dw*)raddr_(ds,0x96FA) = 0;
        *(dw*)raddr_(ds,0x96F8) = 8; *(dw*)raddr_(ds,0x96F0) = 0;
        *(dw*)raddr_(ds,0x96F6) = 0; *(dw*)raddr_(ds,0x0C992) = 0;
        ab_res("redraw 472", sprite_param_set_e14472_lifted,
               sprite_param_set_e14472, 0);
        *(dw*)raddr_(ds,0x96F2) = 2; *(dw*)raddr_(ds,0x96FA) = 1;
        *(dw*)raddr_(ds,0x96F8) = 8; *(dw*)raddr_(ds,0x96F0) = 1;
        *(dw*)raddr_(ds,0x96F6) = 8; *(dw*)raddr_(ds,0x0C992) = 0;
        *(dw*)raddr_(ds,0x96F4) = 0x20;
        ab_res("redraw 478", sprite_param_set_e14478_lifted,
               sprite_param_set_e14478, 0);
        for (int i = 0; i < 5; i++) {
            adapter_id = i;
            ab_res("blitflag dsp", sprite_blit_flagged_lifted,
                   sprite_blit_flagged, 0);
        }
        adapter_id = 3;
        si = 0;
        ab_res("dlw mid", draw_list_walk_e15bf0_lifted,
               draw_list_walk_e15bf0, 0);
        ab_res("dlw tail", draw_list_walk_e15c1a_lifted,
               draw_list_walk_e15c1a, 0);
        ab_res("dlw", draw_list_walk_lifted, draw_list_walk, 0);
        ab_res("cdp", copy_draw_params_lifted, copy_draw_params, 0);
        si = 0xBBD6 + 6; cx = 3;
        ab_res("cdp mid", copy_draw_params_e15d39_lifted,
               copy_draw_params_e15d39, 0);
        ab_res("scrolld", scroll_d_direct_lifted, scroll_d_direct, 0);
        cx = 2; ax = 0x11;
        ab_res("scrolld 501", scroll_d_direct_e14501_lifted,
               scroll_d_direct_e14501, 0);
        frame_cnt2 = 3; frame_cnt = 0;
        ab_res("scrolld 50e", scroll_d_direct_e1450e_lifted,
               scroll_d_direct_e1450e, 0);
        seg_flip = s0; seg_draw = s1; seg_screen = s2;
        fprintf(stderr, "  (redraw: lifted vs C, %d checks)\n", checks);
    }

    /* ---- scroll_mcga pixel shifters, glyph_put dispatchers, tilemap
     * grids (draw_tilemap_frame / tilemap_compose tail), geometry
     * helpers (xy_to_cell/cell_to_px/cell_to_col/pair2), rec5_cmp. ---- */
    {
        dw s0 = seg_flip, s1 = seg_draw, s2 = seg_screen, s3 = seg_10018;
        seg_flip = 0x9000; seg_draw = 0x9800; seg_screen = 0xA000;
        ss = 0x8000; sp = 0xFFFE;
        ds = seg_data; adapter_id = 3;
        ab_res("scroll a", scroll_a_mcga_lifted, scroll_a_mcga, 0);
        ab_res("scroll b", scroll_b_mcga_lifted, scroll_b_mcga, 0);
        ab_res("scroll c", scroll_c_mcga_lifted, scroll_c_mcga, 0);
        ab_res("scroll d", scroll_d_mcga_lifted, scroll_d_mcga, 0);
        ab_res("scroll e", scroll_e_mcga_lifted, scroll_e_mcga, 0);
        ab_res("scroll f", scroll_f_mcga_lifted, scroll_f_mcga, 0);
        ab_res("scroll g", scroll_g_mcga_lifted, scroll_g_mcga, 0);
        ab_res("scroll h", scroll_h_mcga_lifted, scroll_h_mcga, 0);
        al = 'A';
        ab_res("gput fb", glyph_put_flipbuf_lifted, glyph_put_flipbuf, 0);
        al = '^';
        ab_res("gput fb^", glyph_put_flipbuf_lifted, glyph_put_flipbuf, 0);
        al = 'A'; di = seg_flip;
        ab_res("gput fbm", glyph_put_flipbuf_e10c3c_lifted,
               glyph_put_flipbuf_e10c3c, 0);
        al = 'B';
        ab_res("gput bb", glyph_put_backbuf_lifted, glyph_put_backbuf, 0);
        al = 'C';
        ab_res("gput wrap", glyph_put_wrap_lifted, glyph_put_wrap, 0);
        for (int i = 0; i < 5; i++) {
            adapter_id = i;
            ab_res("rdr disp", render_dispatch_lifted, render_dispatch, 0);
            ab_res("clr flip", clear_flipbuf_lifted, clear_flipbuf, 0);
            ab_res("clr draw", clear_drawbuf_lifted, clear_drawbuf, 0);
        }
        adapter_id = 3;
        *(dw*)raddr_(ds,0x0A9C) = 3;
        ab_res("disp a9c", dispatch_a9c_lifted, dispatch_a9c, 0);
        *(dw*)raddr_(ds,0x0A9C) = 0xC;
        ab_res("disp a9c+", dispatch_a9c_lifted, dispatch_a9c, 0);
        di = 4;
        ab_res("disp 5e5", dispatch_a9c_e115e5_lifted,
               dispatch_a9c_e115e5, 0);
        frame_cnt2 = 0; tmframe_6566 = 0; tmframe_6570 = 0;
        ab_res("tmframe", draw_tilemap_frame_lifted, draw_tilemap_frame, 0);
        tmframe_6570 = 0;
        ab_res("tmf 54f", draw_tilemap_frame_e1454f_lifted,
               draw_tilemap_frame_e1454f, 0);
        frame_cnt = 0; tmframe_6570 = 0;
        ab_res("tmf 555", draw_tilemap_frame_e14555_lifted,
               draw_tilemap_frame_e14555, 0);
        seg000_1_d969 = 0x10; seg000_1_d965 = 4;
        tile_src = 0x4000; glyph_rows = 0x5000;
        ab_res("tmc 1c82", tilemap_compose_e11c82_lifted,
               tilemap_compose_e11c82, 0);
        map_base = 0; tile_src = 0x4000;
        si = tile_src;
        ab_res("tmc 1c92", tilemap_compose_e11c92_lifted,
               tilemap_compose_e11c92, 0);
        map_base = 0; routemap_d8f6 = 0; si = 0x4000;
        ab_res("tmc 1c98", tilemap_compose_e11c98_lifted,
               tilemap_compose_e11c98, 0);
        adapter_id = 3;
        ab_res("lut mcga", build_tile_tables_lifted, build_tile_tables, 0);
        adapter_id = 2;
        ab_res("lut ega", build_tile_tables_lifted, build_tile_tables, 0);
        adapter_id = 3; di = 0x0BD5; ax = 0; cx = 0x0C8;
        ds = seg_data; es = seg_data;
        ab_res("lut mid", build_tile_tables_e101f1_lifted,
               build_tile_tables_e101f1, 0);
        ab_res("bufsel", bufsel_mcga_lifted, bufsel_mcga, 0);
        ds = seg_data; adapter_id = 3;
        *(db*)(&obj_sx) = 0x48; *(db*)(((db*)&obj_sx) + 1) = 0;
        *(db*)(&obj_sy) = 0x60; *(db*)(((db*)&obj_sy) + 1) = 0;
        ab_res("xy2cell", xy_to_cell_lifted, xy_to_cell, 0);
        al = 5;
        ab_res("cell2px", cell_to_px_lifted, cell_to_px, 0);
        probe_px = 0x48; probe_py = 0x30;
        cam_org_x = 0x20; cam_9681 = 0x10;
        ab_res("cell2col", cell_to_col_lifted, cell_to_col, 0);
        probe_px = 0x08;                      /* left of window -> CF  */
        ab_res("cell2col oob", cell_to_col_lifted, cell_to_col, 0);
        ab_res("c2col e5", cell_to_col_e1b2e5_lifted, cell_to_col_e1b2e5, 0);
        bx = 0x1111; si = 0x2222;
        cell_x = 9; cell_y = 6; probe_px = 0x48; probe_py = 0x30;
        ab_res("pair2", map_probe_pair2_lifted, map_probe_pair2, 0);
        rec_ptr_a = 0x7000; rec_ptr_b = 0x7010;
        for (int i = 0; i < 6; i++) {
            *(db*)raddr_(ds, 0x7000 + i) = '1' + i;
            *(db*)raddr_(ds, 0x7010 + i) = '4' + i;
        }
        ab_res("rec5", rec5_cmp_lifted, rec5_cmp, 0);
        di = rec_ptr_b + 5; si = rec_ptr_a + 5; cx = 5;
        rec5_cmp_a506 = 0;
        ab_res("rec5 31b", rec5_cmp_e1831b_lifted, rec5_cmp_e1831b, 0);
        si = rec_ptr_a + 5; di = rec_ptr_b + 5; cx = 4;
        rec5_cmp_a506 = 1;
        ab_res("rec5 339", rec5_cmp_e18339_lifted, rec5_cmp_e18339, 0);
        seg_flip = s0; seg_draw = s1; seg_screen = s2; seg_10018 = s3;
        fprintf(stderr, "  (scrl+geo: lifted vs C, %d checks)\n", checks);
    }

    /* ---- map access layer + record rect/row emit ---------------------- */
    {   ds = *(dw*)&mem[0x1a20];
        ss = 0x8000; sp = 0xFFFE;
        dw dsc = ds, esc = es;
        /* map_cell_read: ax=col si=row -> ds:0x9736+row*32+col */
        ax = 3; si = 2;
        ab_res("cellrd", map_cell_read_lifted, map_cell_read, 0);
        ax = 0x20; si = 2;
        ab_res("cellrd oob", map_cell_read_lifted, map_cell_read, 0);
        ax = 0x10; si = 0x40;
        ab_res("cellrd oob2", map_cell_read_lifted, map_cell_read, 0);
        ab_res("cellrd b7", map_cell_read_e1b9b7_lifted, map_cell_read_e1b9b7, 0);
        /* map_cell_write: cell_x/cell_y in bounds + oob */
        cell_x = 4; cell_y = 6; al = 0x5A; bx = 0x11; si = 0x22;
        ab_res("cellwr", map_cell_write_lifted, map_cell_write, 0);
        cell_x = 0x40; cell_y = 0;
        ab_res("cellwr oob", map_cell_write_lifted, map_cell_write, 0);
        bx = 0x33; si = 0x44;
        ab_res("cellwr e16b0a", map_cell_write_e16b0a_lifted, map_cell_write_e16b0a, 0);
        /* map_mark_cell: exit-table entry at si&0x7F negative vs ok */
        cell_x = 5; cell_y = 3;
        for (int i = 0; i < 0x80; i++)
            *(db*)raddr_(ds, map_exit_b253 + i) = 0x01;
        ab_res("markcell", map_mark_cell_lifted, map_mark_cell, 0);
        /* si = (0x9736 + 3*32 + 5) & 0x7F = 0x1B -> negative exit entry */
        *(db*)raddr_(ds, map_exit_b253 + 0x1B) = 0x80;
        ab_res("markcell neg", map_mark_cell_lifted, map_mark_cell, 0);
        /* grid_cell_mark: mark_row*32+mark_col-0x68CA */
        mark_row = 4; mark_col = 2;
        ab_res("gridmark", grid_cell_mark_lifted, grid_cell_mark, 0);
        mark_row = 0x80; mark_col = 0;
        ab_res("gridmark hi", grid_cell_mark_lifted, grid_cell_mark, 0);
        /* map_probe_cell chain: in-bounds and oob */
        cell_x = 8; cell_y = 9;
        ab_res("probecell", map_probe_cell_lifted, map_probe_cell, 0);
        ab_res("probecell 209", map_probe_cell_e1b209_lifted, map_probe_cell_e1b209, 0);
        cell_x = 0x30;
        ab_res("probecell oob", map_probe_cell_lifted, map_probe_cell, 0);
        ab_res("probecell 21e", map_probe_cell_e1b21e_lifted, map_probe_cell_e1b21e, 0);
        /* map_init: neutralize the mission jt/pop_tbl dispatch to a locret */
        mission_idx = 0;
        mem[(ds << 4) + 0xE207] = 4;          /* mission_pop_tbl idx for idx 0 */
        *(dw*)(((db*)&mission_jt) + 0) = 0x091F;
        *(dw*)(((db*)&mission_pop_tbl) + 8) = 0x091F;
        ab_res("mapinit", map_init_lifted, map_init, 0);
        rec_ptr_a = 0x9736; bx = 2; si = 0x10;
        ab_res("mapinit 430", map_init_e1b430_lifted, map_init_e1b430, 0);
        rec_ptr_a = 0x9736; bx = 1; si = 5;
        ab_res("mapinit 43f", map_init_e1b43f_lifted, map_init_e1b43f, 0);
        rec_ptr_a = 0x9736; bx = 1; si = 0xF0;
        ab_res("mapinit 448", map_init_e1b448_lifted, map_init_e1b448, 0);
        /* map_rect_write: rec = [w][h][w*h tiles] at ds:0x9000; clear the
         * scanned cells so the scan passes and the fill pass runs */
        rec_ptr_a = 0x9000;
        *(db*)raddr_(ds, 0x9000) = 3;
        *(db*)raddr_(ds, 0x9001) = 2;
        for (int i = 0; i < 6; i++) *(db*)raddr_(ds, 0x9002 + i) = 0x40 + i;
        cell_bx = 2; cell_by = 1;
        for (int r = 0; r < 2; r++)
            for (int c = 0; c < 3; c++)
                *(db*)raddr_(ds, 0x9736 + (cell_by + r) * 32 + cell_bx + c) = 0;
        ab_res("mrect", map_rect_write_lifted, map_rect_write, 0);
        ab_res("mrect 906", map_rect_write_e1b906_lifted, map_rect_write_e1b906, 0);
        ab_res("mrect 914", map_rect_write_e1b914_lifted, map_rect_write_e1b914, 0);
        rec_ptr_a = 0x9002; mrect_b23c = 3; mrect_b23d = 2; cell_base = 0x9736 + 32 + 2;
        ab_res("mrect 95c", map_rect_write_e1b95c_lifted, map_rect_write_e1b95c, 0);
        si = 2;
        ab_res("mrect 967", map_rect_write_e1b967_lifted, map_rect_write_e1b967, 0);
        ab_res("mrect 991", map_rect_write_e1b991_lifted, map_rect_write_e1b991, 0);
        /* occupied cell -> scan bails stc */
        *(db*)raddr_(ds, 0x9736 + 32 + 4) = 0x77;
        rec_ptr_a = 0x9000;
        ab_res("mrect busy", map_rect_write_lifted, map_rect_write, 0);
        /* maprow_emit / maprow_4: glyph/tile emit, es pinned to scratch */
        es = 0x9000; di = 0;
        draw_row = 0x40; ax = 0x41;
        ab_res("maprow", maprow_emit_lifted, maprow_emit, 0);
        draw_col = 0x20;
        ab_res("maprow e4", maprow_emit_e1bae4_lifted, maprow_emit_e1bae4, 0);
        maprow_base = 0x55;
        ab_res("maprow4", maprow_4_lifted, maprow_4, 0);
        cx = 2;
        ab_res("maprow4 c1", maprow_4_e1bcc1_lifted, maprow_4_e1bcc1, 0);
        /* hdr_walk: link list {off@+0xFE, x@+0x100}, 0xFFFF-terminated */
        *(dw*)raddr_(ds, 0x0FE) = 0x1234;
        *(dw*)raddr_(ds, 0x100) = 0x5678;
        *(dw*)raddr_(ds, 0x102) = 0xFFFF;
        ab_res("hdrwalk", hdr_walk_lifted, hdr_walk, 0);
        bx = 0;
        ab_res("hdrwalk 262", hdr_walk_e10262_lifted, hdr_walk_e10262, 0);
        *(dw*)raddr_(ds, 0x0FE) = 0xFFFF;
        ab_res("hdrwalk term", hdr_walk_lifted, hdr_walk, 0);
        /* rec_row_fetch: bx -> row/col tables + 0x48-stride tile table */
        bx = 0;
        ab_res("recrow", rec_row_fetch_lifted, rec_row_fetch, 0);
        bx = 2;
        ab_res("recrow 2", rec_row_fetch_lifted, rec_row_fetch, 0);
        bx = 0; cx = 3; si = 0x8EF2; draw_row = 4; draw_col = 2;
        ab_res("recrow 480", rec_row_fetch_e15480_lifted, rec_row_fetch_e15480, 0);
        bx = 0; cx = 0x0C; si = 0x8EF2; draw_row = 4; draw_col = 2;
        push(2); push(0x40); push(8);         /* cx=2 rows, draw_row/col */
        ab_res("recrow 497", rec_row_fetch_e15497_lifted, rec_row_fetch_e15497, 0);
        sp += 6;
        cx = 4; bx = 0;
        ab_res("recrow 4bc", rec_row_fetch_e154bc_lifted, rec_row_fetch_e154bc, 0);
        ds = dsc; es = esc;
        fprintf(stderr, "  (mapacc: lifted vs C, %d checks)\n", checks);
    }

    /* ---- rng: LFSR + pickers ------------------------------------------ */
    {   ss = 0x8000; sp = 0xFFFE;
        dw dsc = ds;
        rand_s0 = 0x1234; rand_s1 = 0x5678;
        ab_res("rand", rand_next_lifted, rand_next, 0);
        rand_s0 = 0x8000; rand_s1 = 0x0001;   /* high-bit -> xor taps */
        ab_res("rand tap", rand_next_lifted, rand_next, 0);
        rand_s0 = 0x7FFF; rand_s1 = 0xFFFF;
        ab_res("rand 326", rand_next_e11326_lifted, rand_next_e11326, 0);
        ab_res("rand 33c", rand_next_e1133c_lifted, rand_next_e1133c, 0);
        al = 9;
        ab_res("randmul", rand_mul_lifted, rand_mul, 0);
        rand_s0 = 0x1111; rand_s1 = 0x2222;
        ab_res("rand022", rand_0_22_lifted, rand_0_22, 0);
        /* rand_map_pos retries on busy rect; clear a wide strip so the
         * first (cell_bx,1) placement is likely empty.  The record at
         * ds:0xE3B6 may be clobbered by earlier sections — plant a
         * minimal 2x2 record (w,h + 4 payload bytes). */
        for (int i = 0; i < 0x400; i++)
            *(db*)raddr_(ds, 0x9736 + 32 + i) = 0;
        db *rp_rec = (db*)raddr_(ds, 0xE3B6);
        rp_rec[0] = 2; rp_rec[1] = 2;
        rp_rec[2] = 0x2d; rp_rec[3] = 0xff;
        rp_rec[4] = 0xff; rp_rec[5] = 0xff;
        rand_s0 = 0x4242; rand_s1 = 0x1111;
        ab_res("randpos", rand_map_pos_lifted, rand_map_pos, 0);
        /* rand_tbl_pick: spawn_level/diff_parm tables + dst slot */
        spawn_level = 3; diff_parm = 2;
        *(db*)raddr_(ds, 3 - 0x3501) = 0x80;
        *(db*)raddr_(ds, 3 - 0x34FD) = 4;
        *(db*)raddr_(ds, 3 - 0x34F9) = 0x33;
        *(db*)raddr_(ds, 3 - 0x34F5) = 0x44;
        si = 1; al = 0;
        ab_res("tblpick", rand_tbl_pick_lifted, rand_tbl_pick, 0);
        si = 7; al = 5;
        ab_res("tblpick 4a", rand_tbl_pick_e16e4a_lifted, rand_tbl_pick_e16e4a, 0);
        si = 2;
        ab_res("tblpick 65", rand_tbl_pick_e16e65_lifted, rand_tbl_pick_e16e65, 0);
        ab_res("tblpick 69", rand_tbl_pick_e16e69_lifted, rand_tbl_pick_e16e69, 0);
        ab_res("tblpick 77", rand_tbl_pick_e16e77_lifted, rand_tbl_pick_e16e77, 0);
        si = 3; al = 0xFF;
        ab_res("tblpick 86", rand_tbl_pick_e16e86_lifted, rand_tbl_pick_e16e86, 0);
        si = 3;
        ab_res("tblpick 90", rand_tbl_pick_e16e90_lifted, rand_tbl_pick_e16e90, 0);
        /* mapgen_fill loops 0x20 x mapgen_retry -> map_rect_write:
         * deterministic under identical rand state */
        rand_s0 = 0x1357; rand_s1 = 0x2468;
        ab_res("mfill_a", mapgen_fill_a_lifted, mapgen_fill_a, 0);
        rand_s0 = 0x1357; rand_s1 = 0x2468;
        ab_res("mfill_b", mapgen_fill_b_lifted, mapgen_fill_b, 0);
        rand_s0 = 0x1357; rand_s1 = 0x2468; fill_b_b0c7 = 3;
        ab_res("mfill 48b", mapgen_fill_b_e1b48b_lifted, mapgen_fill_b_e1b48b, 0);
        ds = dsc;
        fprintf(stderr, "  (rand+fill: lifted vs C, %d checks)\n", checks);
    }

    /* ---- mission leaf layer: ds wraps, cursor res sel, mission
     * setup/populate call chains, cells scatter, obj spawn ---- */
    {
        dw dsc = ds;
        ss = 0x8000; sp = 0xFFFE;
        ab_res("dswrap_a", ds_wrap_a_lifted, ds_wrap_a, 0);
        push(ds);
        ab_res("dswrap_a mid", ds_wrap_a_e1bd56_lifted, ds_wrap_a_e1bd56, 0);
        ab_res("dswrap_b", ds_wrap_b_lifted, ds_wrap_b, 0);
        push(ds);
        ab_res("dswrap_b mid", ds_wrap_b_e1c671_lifted, ds_wrap_b_e1c671, 0);
        ab_res("dswrap_c", ds_wrap_c_lifted, ds_wrap_c, 0);
        push(ds);
        ab_res("dswrap_c mid", ds_wrap_c_e1c9ff_lifted, ds_wrap_c_e1c9ff, 0);
        ab_res("locret4c", locret_1bd4c_lifted, locret_1bd4c, 0);
        /* cursor_res_sel: mission_idx indexes 4 res tables (di-1DED..-1DC9) */
        mission_idx = 4;
        adapter_id = 3;                  /* skips the second pair */
        ab_res("curres adp3", cursor_res_sel_lifted, cursor_res_sel, 0);
        adapter_id = 0;                  /* adapter 0 -> second pair too */
        ab_res("curres adp0", cursor_res_sel_lifted, cursor_res_sel, 0);
        adapter_id = 4;
        ab_res("curres adp4", cursor_res_sel_lifted, cursor_res_sel, 0);
        adapter_id = 1;
        ab_res("curres mid", cursor_res_sel_e1bd32_lifted, cursor_res_sel_e1bd32, 0);
        /* mission setup: select_resource + load_resource chain — feed
         * Enters for any res-file error path (bad handle here) */
        ab_res("m6 setup", mission6_setup_lifted, mission6_setup, 0xF);
        ab_res("m7 setup", mission7_setup_lifted, mission7_setup, 0xF);
        ab_res("m8 setup", mission8_setup_lifted, mission8_setup, 0xF);
        /* populate: full mapgen pipeline — fixed rand seed so retries
         * terminate identically on both sides */
        for (int i = 0; i < 0x800; i++)
            *(db*)raddr_(ds, 0x9736 + i) = 0;
        rand_s0 = 0x1357; rand_s1 = 0x2468;
        ab_res("m6 populate", mission6_populate_lifted, mission6_populate, 0);
        rand_s0 = 0x1357; rand_s1 = 0x2468;
        ab_res("m7 populate", mission7_populate_lifted, mission7_populate, 0);
        rand_s0 = 0x1357; rand_s1 = 0x2468;
        ab_res("m8 populate", mission8_populate_lifted, mission8_populate, 0);
        /* cells scatter: bounded stride sweep, rec tbl ds:si-1BA2/19F9 */
        rand_s0 = 0x1357; rand_s1 = 0x2468;
        ab_res("cells_a", mapgen_cells_a_lifted, mapgen_cells_a, 0);
        rand_s0 = 0x1357; rand_s1 = 0x2468;
        ab_res("cells_a mid", mapgen_cells_a_e1c167_lifted, mapgen_cells_a_e1c167, 0);
        rand_s0 = 0x1357; rand_s1 = 0x2468;
        ab_res("cells_b", mapgen_cells_b_lifted, mapgen_cells_b, 0);
        rand_s0 = 0x1357; rand_s1 = 0x2468;
        ab_res("cells_b mid", mapgen_cells_b_e1ca06_lifted, mapgen_cells_b_e1ca06, 0);
        rand_s0 = 0x1357; rand_s1 = 0x2468;
        *(db*)raddr_(ds, 0x0E574) = 3;
        ab_res("mgen_obj", mapgen_obj_lifted, mapgen_obj, 0);
        /* mapgen_test/retry: rec_ptr_b slot table -> word rec ptrs at
         * +0x14+2k all plant the same 2x2 record; mgen bounds small */
        for (int k = 0; k < 12; k++)
            *(dw*)raddr_(ds, 0xE35D + 0x14 + 2 * k) = 0xE3B6;
        rec_ptr_b = 0xE35D;
        mgen_12a = 2; mgen_12b = 2; mgen_127 = 0x40; mgen_128 = 0x40;
        rand_s0 = 0x1357; rand_s1 = 0x2468;
        ab_res("mgen retry", mapgen_retry_lifted, mapgen_retry, 0);
        rand_s0 = 0x1357; rand_s1 = 0x2468; retry_b126 = 0x10; retry_b129 = 4;
        ab_res("mgen r70b", mapgen_retry_e1b70b_lifted, mapgen_retry_e1b70b, 0);
        rand_s0 = 0x1357; rand_s1 = 0x2468; retry_b126 = 3; retry_b129 = 1;
        ab_res("mgen r72d", mapgen_retry_e1b72d_lifted, mapgen_retry_e1b72d, 0);
        rand_s0 = 0x1357; rand_s1 = 0x2468;
        ab_res("mgen r738", mapgen_retry_e1b738_lifted, mapgen_retry_e1b738, 0);
        retry_b129 = 2;
        ab_res("mgen test", mapgen_test_lifted, mapgen_test, 0);
        /* obj_alloc: find_free_slot scans the slot table — clear flags */
        for (int i = 0; i < 0x300; i++)
            *(db*)raddr_(ds, 0xBE8D - 0x4085 + i) = 0;
        al = 0x16;
        ab_res("obj_alloc", obj_alloc_lifted, obj_alloc, 0);
        ab_res("obj_alloc f7", obj_alloc_e1b9f7_lifted, obj_alloc_e1b9f7, 0);
        /* mapgen_emit: si slot 0..3, emit cursor b0d3/b0d4 */
        mgen_b0cb = 1; mgen_b0ca = 0; mgen_b0d3 = 4; mgen_b0d4 = 2;
        rand_s0 = 0x1357; rand_s1 = 0x2468;
        ab_res("mgen emit", mapgen_emit_lifted, mapgen_emit, 0);
        si = 2; mgen_b0d3 = 4; mgen_b0d4 = 2;
        rand_s0 = 0x1357; rand_s1 = 0x2468;
        ab_res("mgen e581", mapgen_emit_e1b581_lifted, mapgen_emit_e1b581, 0);
        mgen_b0cb = 2; mgen_b0d3 = 4; mgen_b0d4 = 2;
        ab_res("mgen e5ca", mapgen_emit_e1b5ca_lifted, mapgen_emit_e1b5ca, 0);
        ds = dsc;
        fprintf(stderr, "  (mission leaf: lifted vs C, %d checks)\n", checks);
    }

    /* ---- status-panel digit patcher: WOUNDS field ds:0xB93F <- byte_29712
     * template lives in the image with literal "XX" placeholders; sub_1BBB9
     * converts al to two ASCII digits and stores at ds:[si]/ds:[si+1]. ---- */
    ds = *(dw*)&mem[0x1a20];                     /* seg_10000: relocated data seg */
    int wslot = (ds << 4) + 0xB93F;
    mem[wslot] = 'X'; mem[wslot+1] = 'X';        /* hud section rewrote it */
    CHECK(mem[wslot] == 'X' && mem[wslot+1] == 'X');   /* unpatched template */
    al = 7; si = 0xB93F; fmt_2digit();
    CHECK(mem[wslot] == '0' && mem[wslot+1] == '7');
    al = 0; si = 0xB964; fmt_2digit();            /* FIRST AID slot */
    CHECK(mem[(ds<<4)+0xB964] == '0' && mem[(ds<<4)+0xB965] == '0');
    al = 42; si = 0xB930; fmt_2digit();           /* CARBINE MAGS slot */
    CHECK(mem[(ds<<4)+0xB930] == '4' && mem[(ds<<4)+0xB931] == '2');

    /* ---- overlay loader: lifted vs C, forked A/B (error paths exit) ---- */
    atexit(ab_dump);
    {   dw offs[2] = { 0x1111, 0x2222 };
        mk_ovl_mz("/tmp/ar_ovl_ok.exe", 0x0300, 0, 0x0060, 0x0040, 2, offs, 2);
        FILE *f = fopen("/tmp/ar_ovl_badmz.exe", "wb");
        fwrite("NOPE!!", 1, 6, f); fclose(f);
        mk_ovl_mz("/tmp/ar_ovl_big.exe", 0x0300, 0, 0x8000, 0x8000, 2, offs, 2);
        ab_ovl("load_overlay", load_overlay_lifted, load_overlay, OVL_OK);
        ab_ovl("ovl missing", load_overlay_lifted, load_overlay, OVL_NOFILE);
        ab_ovl("ovl badmz", load_overlay_lifted, load_overlay, OVL_BADMZ);
        ab_ovl("ovl toobig", load_overlay_lifted, load_overlay, OVL_TOOBIG);
        ab_ovl("ovl ab6 ok", load_overlay_e13ab6_lifted, load_overlay_e13ab6, OVL_MID_AB6);
        ab_ovl("ovl ab6 fail", load_overlay_e13ab6_lifted, load_overlay_e13ab6, OVL_MID_AB6F);
        ab_ovl("ovl aca ax2", load_overlay_e13aca_lifted, load_overlay_e13aca, OVL_MID_ACA2);
        ab_ovl("ovl aca ax8", load_overlay_e13aca_lifted, load_overlay_e13aca, OVL_MID_ACA8);
        ab_ovl("ovl af9 ax8", load_overlay_e13af9_lifted, load_overlay_e13af9, OVL_MID_AF9);
        ab_ovl("ovl af9 ax1", load_overlay_e13af9_lifted, load_overlay_e13af9, OVL_MID_B08);
        ab_ovl("ovl b08", load_overlay_e13b08_lifted, load_overlay_e13b08, OVL_MID_B08);
        ab_ovl("ovl b12", load_overlay_e13b12_lifted, load_overlay_e13b12, OVL_MID_B12);
        ab_ovl("ovl b46", load_overlay_e13b46_lifted, load_overlay_e13b46, OVL_MID_B46);
        ab_ovl("ovl b85", load_overlay_e13b85_lifted, load_overlay_e13b85, OVL_MID_B85);
        ab_ovl("ovl b9e", load_overlay_e13b9e_lifted, load_overlay_e13b9e, OVL_MID_B9E);
        ab_ovl("ovl baa", load_overlay_e13baa_lifted, load_overlay_e13baa, OVL_MID_BAA);
        unsetenv("M2C_ABDUMP");
        remove("/tmp/ar_ovl_ok.exe"); remove("/tmp/ar_ovl_badmz.exe");
        remove("/tmp/ar_ovl_big.exe"); remove("/tmp/ab_ovl_a.bin"); remove("/tmp/ab_ovl_b.bin");
        fprintf(stderr, "  (overlay chain: lifted vs C, %d checks)\n", checks);
    }

    fprintf(stderr, "test_game: %d checks, %d failures\n", checks, fails);
    return fails ? 1 : 0;
}
