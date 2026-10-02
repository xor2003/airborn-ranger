/* integration tests against the real game objects (all objs minus main.o):
 *   - image load + fixups
 *   - func_at address->function mapping
 *   - BIOS int8 stub -> int1c -> countdown timers
 *   - LZW resource decoder decompress_res vs an independent reference implementation
 *     over every *CHR/*.SCR/misc .DTX file in the game dir
 */
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
static size_t port_lzw(const char *path){
    FILE *f = fopen(path, "rb");
    if (!f) return (size_t)-1;
    size_t n = fread(&mem[((dd)SRCSEG << 4) + SRCOFF], 1, 0x8000, f);
    fclose(f);
    ds = SRCSEG; si = SRCOFF; es = DSTSEG; di = 0; cx = (dw)n; dx = 0;
    ss = STKSEG; sp = 0xFFFE;
    decompress_res();
    return di;                                  /* dest offset = bytes out */
}

int main(void){
    /* ---- image load ---- */
    mem_load_image();
    mem_apply_fixups();
    CHECK(mem[0x1a20] != 0 || mem[0x1a21] != 0);  /* image landed */
    long nz = 0; for (long i = 0x1a20; i < 0x1a20 + 163888; i++) nz += mem[i] != 0;
    CHECK(nz > 60000);                             /* real code+data present */

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

    /* ---- LZW: every .DTX decodes identically under both implementations ---- */
    const char *gdir = "/home/xor/games/airborn";
    DIR *d = opendir(gdir);
    CHECK(d != NULL);
    struct dirent *e; int ndtx = 0;
    static unsigned char ref_out[0x20000];
    while ((e = readdir(d))){
        const char *nm = e->d_name;
        size_t l = strlen(nm);
        if (l < 4 || strcasecmp(nm + l - 4, ".DTX")) continue;
        char path[512]; snprintf(path, sizeof path, "%s/%s", gdir, nm);
        FILE *f = fopen(path, "rb"); if (!f) continue;
        static unsigned char src[0x8100];
        size_t n = fread(src, 1, sizeof src, f); fclose(f);
        if (n < 3 || n > 0x8000) continue;         /* src must fit past the dict */
        size_t rn = ref_lzw(src, n, ref_out);
        size_t pn = port_lzw(path);
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
    closedir(d);
    CHECK(ndtx > 20);                              /* really exercised the corpus */
    fprintf(stderr, "  (%d DTX files decoded identically)\n", ndtx);

    /* ---- status-panel digit patcher: WOUNDS field ds:0xB93F <- byte_29712
     * template lives in the image with literal "XX" placeholders; sub_1BBB9
     * converts al to two ASCII digits and stores at ds:[si]/ds:[si+1]. ---- */
    ds = *(dw*)&mem[0x1a20];                     /* seg_10000: relocated data seg */
    int wslot = (ds << 4) + 0xB93F;
    CHECK(mem[wslot] == 'X' && mem[wslot+1] == 'X');   /* unpatched template */
    al = 7; si = 0xB93F; sub_1bbb9();
    CHECK(mem[wslot] == '0' && mem[wslot+1] == '7');
    al = 0; si = 0xB964; sub_1bbb9();            /* FIRST AID slot */
    CHECK(mem[(ds<<4)+0xB964] == '0' && mem[(ds<<4)+0xB965] == '0');
    al = 42; si = 0xB930; sub_1bbb9();           /* CARBINE MAGS slot */
    CHECK(mem[(ds<<4)+0xB930] == '4' && mem[(ds<<4)+0xB931] == '2');

    fprintf(stderr, "test_game: %d checks, %d failures\n", checks, fails);
    return fails ? 1 : 0;
}
