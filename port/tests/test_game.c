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
#include "../procs.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

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

    fprintf(stderr, "test_game: %d checks, %d failures\n", checks, fails);
    return fails ? 1 : 0;
}
