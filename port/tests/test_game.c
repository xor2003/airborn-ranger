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
    if (!ok)
        fprintf(stderr, "  res-ab %s: cf %d/%d zf %d/%d sf %d/%d of %d/%d\n",
                nm, cfa, CF, zfa, ZF, sfa, SF, ofa, OF);
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

    /* ---- status-panel digit patcher: WOUNDS field ds:0xB93F <- byte_29712
     * template lives in the image with literal "XX" placeholders; sub_1BBB9
     * converts al to two ASCII digits and stores at ds:[si]/ds:[si+1]. ---- */
    ds = *(dw*)&mem[0x1a20];                     /* seg_10000: relocated data seg */
    int wslot = (ds << 4) + 0xB93F;
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
