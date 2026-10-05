/* Hand-written readable replacements for machine-shaped lifted procs.
 *
 * A proc listed in tools/gen_port.py REWRITES keeps its lifted body under
 * <name>_lifted() while every caller is routed to <name>() -> <name>_c().
 * The rewrite must reproduce the original's observable state exactly:
 * registers, dcomp_* scratch globals (ds-relative), the ds:0x67F0
 * dictionary, output via stosb, and the ss:sp push-chain. Equivalence is
 * proven in test_game.c: lifted vs C vs the independent reference over the
 * full .DTX corpus.
 *
 * The Amiga/ST decompiles use a different stream format for resources
 * (backward bitstream + checksum trailer), so the DOS lifted routine is
 * the oracle here; they remain the readability oracle for later rewrites.
 */
#include "rt.h"
#include "data_syms.h"

/* ---- decompress_res (seg000:3998) — LZW resource decoder --------------
 * In:  ds:si -> stream ([maxw][LSB-first packed codes]), cx = byte count
 * Out: stosb to es:di (di ends = bytes produced)
 * Dict: 3-byte {link:dw, suffix:db} records at ds:DICT_LINK/DICT_SUFFIX;
 *       link LINK_NULL marks the 0..255 literal roots.
 * Strings are emitted reversed through the x86 stack (sp/bp), which is
 * also the observable stack the original used.
 *
 * Scratch state (ds-relative aliases, fixed names — they are the ABI):
 *   dcomp_4e71  max code width        (stream byte0)
 *   dcomp_lim   current code width    (9 growing to max)
 *   dcomp_mask  code mask             (1<<lim)-1
 *   dcomp_4e74  bit accumulator       (raw last-loaded word, pending at top)
 *   dcomp_4e76  pending bit count     (bits still valid in accumulator)
 *   dcomp_4e77  stream end offset     (si limit)
 *   dcomp_4e79  previously emitted code (KwKwK seed + appended link)
 *   dcomp_4e7b  first byte of current string (KwKwK extra + appended suffix) */
#define LZW_INIT_WIDTH 9
#define LZW_LITERALS   0x100    /* 256 literal-root entries              */
#define LZW_DICT_WORDS 0x800    /* link-init pass covers dict[0..0x7FF]  */
#define LINK_NULL      0xFFFF   /* -1 link terminator = literal root     */
#define DICT_LINK      0x67F0   /* ds: +slot*3 -> dw link (parent code)  */
#define DICT_SUFFIX    0x67F2   /* ds: +slot*3 -> db appended byte       */

/* The x86 string/memory idioms, named after the ops they reproduce. */
static dw lodsw_(void){ dw v = *(dw*)raddr_(ds,si); si += DF?-2:2; return v; }
static void stosb_(void){ *(db*)raddr_(es,di) = al; di += DF?-1:1; }

/* sub_13A55 — shared dictionary reset (sprtab_init_a falls into it,
 * decompress_res calls it on width overflow). */
static void dict_reset(void){
    dcomp_lim = LZW_INIT_WIDTH;
    dcomp_mask = (1 << LZW_INIT_WIDTH) - 1;
    dx = LZW_LITERALS;                      /* next free dict slot      */
    ax = LINK_NULL;
    bx = 0; cx = LZW_DICT_WORDS;
    do { *(dw*)raddr(ds,bx+DICT_LINK) = ax; bx += 3; } while (--cx);
    al = 0; bx = 0; cx = LZW_LITERALS;
    do { *(db*)raddr(ds,bx+DICT_SUFFIX) = al; al++; bx += 3; } while (--cx);
}

void sprtab_init_b_c(void){ dict_reset(); }

void sprtab_init_a_c(void){
    ax = lodsw_();
    dcomp_4e74 = ax;                        /* raw first word           */
    dcomp_4e76 = 8;                         /* byte0 already consumed   */
    dcomp_4e71 = al;                        /* byte0 = max code width   */
    dict_reset();
}

void decompress_res_c(void){
    cx = (dw)(cx + si);                     /* add cx,si                */
    dcomp_4e77 = cx;                        /* stream end offset        */
    sprtab_init_a_c();
    do {                                    /* end check is at loop tail */
        /* --- pull lim bits: pending bits live at the TOP of the last
         * loaded word; new words shift in ABOVE them (16-bit shl drops
         * the far byte, which is what makes the packing LSB-first) --- */
        bx = dcomp_4e74;
        ch = dcomp_4e76;                    /* avail bits pending       */
        cl = (db)(16 - ch);
        bx >>= cl;                          /* pending -> low bits      */
        cl = ch;
        while ((signed char)cl < (signed char)dcomp_lim){
            ax = lodsw_();
            dcomp_4e74 = ax;                /* store raw word           */
            ax <<= cl;
            bx |= ax;
            cl = (db)(cl + 16);
        }
        cl = (db)(cl - dcomp_lim);
        dcomp_4e76 = cl;                    /* bits left for next pull  */
        ax = bx & dcomp_mask;               /* next dictionary code     */

        /* --- emit the decoded string, reversed via the stack --------- */
        cx = ax;                            /* code (prev-slot value)   */
        bp = sp;                            /* unwind mark              */
        if ((short)ax >= (short)dx){        /* KwKwK: code == next slot */
            cx = dx;
            ax = dcomp_4e79;                /* reuse previous code      */
            bl = dcomp_4e7b;                /* + its first byte         */
            push(bx);
        }
        for (;;){                           /* walk link chain to a root */
            bx = ax*3;                      /* record stride 3 (pushed) */
            ax = *(dw*)raddr(ds,bx+DICT_LINK);
            if (++ax == 0){                 /* link -1: literal leaf    */
                ax--;
                bl = *(db*)raddr(ds,bx+DICT_SUFFIX);
                dcomp_4e7b = bl;            /* first byte of the string */
                al = bl;
                stosb_();
                break;
            }
            ax--;
            bl = *(db*)raddr(ds,bx+DICT_SUFFIX);
            push(bx);
        }
        while (sp < bp){ ax = pop(); stosb_(); }

        /* --- append {prev,first} at dict[dx]; widen or reset --------- */
        al = bl;
        bx = dx*3;
        *(db*)raddr(ds,bx+DICT_SUFFIX) = al; /* suffix = first byte     */
        ax = dcomp_4e79;
        *(dw*)raddr(ds,bx+DICT_LINK) = ax;   /* link   = previous code  */
        dx++;
        if ((short)dx > (short)dcomp_mask){
            dcomp_lim++;
            dcomp_mask = (dcomp_mask << 1) | 1;
        }
        al = dcomp_lim;                     /* lifted loads al before cmp:
                                               keeps ax end-state exact  */
        if ((signed char)al > (signed char)dcomp_4e71)
            sprtab_init_b_c();              /* width overflow: reset    */
        dcomp_4e79 = cx;                    /* prev = code emitted      */
    } while (si < dcomp_4e77);
}
