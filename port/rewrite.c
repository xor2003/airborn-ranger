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
#include "procs.h"

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

/* ---- resource file I/O chain (seg000:07C3..09E5) -----------------------
 * The resource directory is a word-indexed table of records: res_cache_d
 * selects the record pointer through the table at ds:0x756. The record
 * pointer doubles as the ASCIZ filename; name field is 13 bytes, then
 * rec[0x0D] = modecall index, and words: load seg:off, parm (compressed
 * flag), staging seg:off.
 *
 * DOS services report errors through CF; every failure tail funnels into
 * res_load_fail (close, three beeps, error text + Enter wait) -> CF=1.
 * res_pos is a progress breadcrumb the error path leaves in ds:186.
 *
 * The *_e<off> functions are the lifted secondary entries — reachable
 * through func_at — each covering the tail of its parent routine. */

/* res record field offsets (after the 13-byte name field) */
#define RES_MODE   0x0D   /* modecall_tbl index                       */
#define RES_LSEG   0x0E   /* load segment                             */
#define RES_LOFF   0x10   /* load offset                              */
#define RES_PARM   0x12   /* nonzero -> decompress into staging       */
#define RES_SSEG   0x14   /* staging segment                          */
#define RES_SOFF   0x16   /* staging offset                           */
#define RES_RECS   0x756  /* ds base of the record-pointer table      */
#define RES_ERRBUF 0x13E  /* error-line buffer for the filename       */
#define RES_CSBASE 0x1a20 /* flat base of the seg000 (cs) image       */

void open_res_file_c(void){
    ax = 0x3D00;                            /* OPEN existing, read-only */
    dx = res_name_ptr;
    dos_open();
    res_handle = ax;
}

void close_res_file_c(void){
    ah = 0x3E;
    bx = res_handle;
    dos_close();
}

/* shared fail tail: close + CF — seek_res_entry's e108c1 entry point */
static void res_fail_close(void){
    close_res_file();
    CF = 1;
}
void seek_res_entry_e108c1_c(void){ res_fail_close(); }

/* open + size the resource via LSEEK-end; res_f1a <- byte count.
 * CF=1 if the open/seek fails or the file exceeds 64K. */
void seek_res_entry_c(void){
    al = 0;
    open_res_file();
    res_pos = 1;
    if (!CF){
        ah = 0x42; al = 2; cx = 0; dx = 0;  /* LSEEK end+0 -> size      */
        bx = res_handle;
        dos_seek();
        res_pos = 2;
        if (!CF){
            res_f1a = ax;
            dx |= dx; CF = 0; OF = 0;       /* high word must be zero   */
            ZF = (dx == 0); SF = (dx >> 15);
            res_pos = 3;
            if (dx == 0){
                close_res_file();
                res_pos = 0;
                CF = 0;
                return;
            }
        }
    }
    res_fail_close();
}

void res_load_fail_c(void){
    close_res_file();
    speaker_beep(); speaker_beep(); speaker_beep();
    res_file_error();
    CF = 1;
}

void res_file_read_c(void){
    seek_res_entry();
    if (CF){ res_load_fail(); return; }
    res_file_read_e10849_c();
}
void res_file_read_e10846_c(void){ res_load_fail(); }

/* mid-entry: reopen + read res_f1a bytes into res_seg:res_ptr */
void res_file_read_e10849_c(void){
    al = 0;
    open_res_file();
    *(dw*)raddr(ds, 0x186) = 4;             /* res_pos: reading         */
    if (CF){ res_load_fail(); return; }
    bx = *(dw*)raddr(ds, 0x196);            /* res_handle               */
    cx = *(dw*)raddr(ds, 0x19A);            /* res_f1a                  */
    dx = *(dw*)raddr(ds, 0x190);            /* res_ptr                  */
    ax = *(dw*)raddr(ds, 0x192);            /* res_seg                  */
    ds = ax;
    ah = 0x3F;
    dos_read();
    ax = seg_data;
    ds = ax;
    res_pos = 5;
    if (CF){ res_load_fail(); return; }
    close_res_file();
    res_pos = 0;
    CF = 0;
}

/* res_file_error tails ---------------------------------------------- */

/* e109d9 head: wait for Enter (read_key filters V/v mode toggles) */
static void res_err_wait_enter(void){
    do {
        read_key();
        CF = al < 0x0D; ZF = ((db)(al - 0x0D) == 0); SF = (((db)(al - 0x0D)) >> 7);
    } while (al != 0x0D);
    menu_state = pop();
    CF = 1;
}
void res_file_error_e109d9_c(void){ res_err_wait_enter(); }

/* e109b7 head: terminate the copied name, print, save menu state */
static void res_err_report(void){
    *(db*)raddr(ds, di) = 0x24;             /* '$' for DOS print        */
    dx = 0x120; ah = 9; dos_print_string();
    dx = 0x13C; ah = 9; dos_print_string();
    dx = 0x152; ah = 9; dos_print_string();
    push(menu_state);
    menu_state = 0;
    res_err_wait_enter();
}
void res_file_error_e109b7_c(void){ res_err_report(); }

/* e109ab head: copy ASCIZ name ds:si -> ds:di */
static void res_err_copy_name(void){
    for (;;){
        al = *(db*)raddr(ds, si);
        al |= al; CF = 0; OF = 0; ZF = (al == 0); SF = (al >> 7);
        if (al == 0) break;
        *(db*)raddr(ds, di) = al;
        si++; ZF = (si == 0); SF = (si >> 15);
        di++; ZF = (di == 0); SF = (di >> 15);
    }
    res_err_report();
}
void res_file_error_e109ab_c(void){ res_err_copy_name(); }

void res_file_error_c(void){
    si = res_name_ptr;
    di = RES_ERRBUF;
    res_err_copy_name();
}

/* ------------------------------------------------------------------ */

/* mode dispatch: modecall_tbl[bx] is a near offset into the seg000
 * image; the callee may consume extra stack words (far-thunk shape) —
 * restore sp and propagate the early return like the lifted macro. */
static int res_mode_call(dw idx){
    dw off = *(dw*)((db*)&funcs_1083a + idx);
    dd fa = RES_CSBASE + off;
    vfn f_ = func_at(fa);
    dw sp_ = sp;
    if (f_) f_();
    else fprintf(stderr, "unresolved ind call %x\n", fa);
    if ((short)(sp - sp_) > 0){ sp = sp_; return 1; }
    return 0;
}

/* load_resource: fetch rec -> res_* state, read file, decompress into
 * staging when res_parm, then dispatch the per-mode post-load hook. */
void load_resource_c(void){
    ax = seg_data;
    ds = ax;
    bx = res_cache_d;
    CF = (((dd)bx << 1) >> 16) & 1; bx <<= 1;
    ZF = (bx == 0); SF = (bx >> 15);
    si = *(dw*)raddr(ds, bx + RES_RECS);    /* rec = dir[res_cache_d]   */
    load_res_d004 = si;
    res_name_ptr = si;
    al = *(db*)raddr(ds, si + RES_MODE);
    ah = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    res_tmp = ax;                           /* mode index               */
    ax = *(dw*)raddr(ds, si + RES_LSEG); res_seg = ax;
    ax = *(dw*)raddr(ds, si + RES_LOFF); res_ptr = ax;
    ax = *(dw*)raddr(ds, si + RES_PARM); res_parm = ax;
    ax = *(dw*)raddr(ds, si + RES_SSEG); res_stg_seg = ax;
    ax = *(dw*)raddr(ds, si + RES_SOFF); res_off = ax;
    res_file_read();
    if (CF) return;
    if (res_parm != 0){
        push(ds); push(es);
        cx = res_f1a;
        es = res_stg_seg;
        di = res_off;
        si = res_ptr;
        ds = res_seg;
        decompress_res();
        es = pop(); ds = pop();
        /* the live buffer is now the staging area */
        ax = *(dw*)raddr(ds, 0x174);          /* res_stg_seg            */
        *(dw*)raddr(ds, 0x192) = ax;          /* -> res_seg             */
        ax = *(dw*)raddr(ds, 0x176);          /* res_off                */
        *(dw*)raddr(ds, 0x190) = ax;          /* -> res_ptr             */
    }
    /* modecall: bx = res_tmp * 2 */
    bx = *(dw*)raddr(ds, 0x194);            /* res_tmp                  */
    CF = (((dd)bx << 1) >> 16) & 1; bx <<= 1;
    ZF = (bx == 0); SF = (bx >> 15);
    if (res_mode_call(bx)) return;
    CF = 0;
}

/* mode_rec_load: same rec lookup but a WRITE (mode-record save path) */
void mode_rec_load_c(void){
    ax = seg_data;
    ds = ax;
    bx = res_cache_d;
    CF = (((dd)bx << 1) >> 16) & 1; bx <<= 1;
    ZF = (bx == 0); SF = (bx >> 15);
    si = *(dw*)raddr(ds, bx + RES_RECS);
    load_res_d004 = si;
    res_name_ptr = si;
    ax = *(dw*)raddr(ds, si + RES_LSEG); res_seg = ax;
    ax = *(dw*)raddr(ds, si + RES_LOFF); res_ptr = ax;
    write_res_file();
    if (!CF) return;
    res_pos = 6;
    res_load_fail();
}

/* write_res_file: CREAT + write res_f1a bytes from res_seg:res_ptr.
 * e1092f enters with ax = the CREAT handle. */
void write_res_file_e1092d_c(void){ res_load_fail(); }
void write_res_file_e1092f_c(void){
    res_handle = ax;
    bx = res_handle;
    cx = res_f1a;
    dx = res_ptr;
    ax = res_seg;
    ds = ax;
    ah = 0x40;
    dos_write();
    ax = seg_data;
    ds = ax;
    if (CF){ res_load_fail(); return; }
    close_res_file();
    CF = 0;
}
void write_res_file_c(void){
    ah = 0x3C;                              /* CREAT, attr 0            */
    dx = res_name_ptr;
    cx = 0;
    dos_int21(ax);
    if (CF){ res_load_fail(); return; }
    write_res_file_e1092f_c();
}

/* ---- overlay EXEC loader (seg000:3A94..3BC0) -------------------------
 * load_overlay(bx = index into ds:0x19E2 name table):
 *   probe-max alloc -> alloc whole free span -> EXEC-load the overlay
 *   image into it -> build the 5-byte call table at ds:0x1990 from the
 *   overlay's own header -> SETBLOCK to the exact size -> ax = ovl seg.
 * Every error prints a $-string then exits via e13baa (exit+free loop).
 * ------------------------------------------------------------------ */

/* e13baa: desperate exit — try EXIT(al=0); if the port's exit somehow
 * returns, free es and retry after printing the free-fail message. */
void load_overlay_e13baa_c(void){
    for (;;){
        ax = 0x4C00;
        dos_exit_code();
        es = ax;
        ah = 0x49;
        dos_free();
        if (!CF) return;
        dx = 0x1ADD;
        ah = 9;
        dos_print_string();
    }
}

/* e13b9e: success tail — restore ds/es and hand back the overlay seg. */
void load_overlay_e13b9e_c(void){
    ax = seg_data;
    ds = ax;
    es = ax;
    ax = ovl_e840;
}

/* e13b85: SETBLOCK(es = ovl seg, bx + 0xA paras); fail -> print + exit. */
void load_overlay_e13b85_c(void){
    { dd t_ = (dd)bx + (dd)0x0A; CF = t_ > 0xFFFF; bx = t_;
      ZF = (bx == 0); SF = (bx >> 15); }
    ah = 0x4A;
    cx = ovl_e840;
    es = cx;
    dos_resize();
    if (CF){
        dx = 0x1AB2;
        ah = 9;
        dos_print_string();
        load_overlay_e13baa_c();
        return;
    }
    load_overlay_e13b9e_c();
}

/* e13b46: table-fill loop — cx entries {off = es:[si], seg = di} into
 * ds:bx+1/+3 (5-byte stride); then size check vs the free-para probe. */
void load_overlay_e13b46_c(void){
    do {
        ax = *(dw*)raddr(es, si);
        *(dw*)raddr(ds, bx + 1) = ax;
        *(dw*)raddr(ds, bx + 3) = di;
        { dd t_ = (dd)si + (dd)2; CF = t_ > 0xFFFF; si = t_;
          ZF = (si == 0); SF = (si >> 15); }
        { dd t_ = (dd)bx + (dd)5; CF = t_ > 0xFFFF; bx = t_;
          ZF = (bx == 0); SF = (bx >> 15); }
    } while (--cx != 0);
    /* needed paras = (es:[0x1E] + es:[0x20]) >> 4 vs probed free span */
    di = 0x1E;
    bx = *(dw*)raddr(es, di);
    CF = (bx >> 3) & 1; bx >>= 4; ZF = (bx == 0); SF = (bx >> 15);
    di = 0x20;
    cx = *(dw*)raddr(es, di);
    CF = (cx >> 3) & 1; cx >>= 4; ZF = (cx == 0); SF = (cx >> 15);
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_;
      ZF = (bx == 0); SF = (bx >> 15); }
    CF = (dd)bx < (dd)ovl_e842;
    ZF = ((dw)(bx - ovl_e842) == 0); SF = (((dw)(bx - ovl_e842)) >> 15);
    if ((short)bx > (short)ovl_e842){
        dx = 0x1A8D;
        ah = 9;
        dos_print_string();
        load_overlay_e13baa_c();
        return;
    }
    load_overlay_e13b85_c();
}

/* e13b12: post-EXEC restore — ss:sp were clobbered by EXEC — then seed
 * the table cursor (bx = 0x1990 + es:[0x1C]*5, cx = es:[0x22],
 * si = 0x24, di = es:[0x18] = overlay code seg). */
void load_overlay_e13b12_c(void){
    sp = ovl_3a90;
    ds = seg_data;
    ss = ovl_3a92;
    ax = ovl_e840;
    es = ax;
    bx = 0x1990;
    di = 0x1C;
    ax = *(dw*)raddr(es, di);
    dl = 5;
    ax = (dw)al * dl;                       /* mul dl — count*5 */
    { dd t_ = (dd)bx + (dd)ax; CF = t_ > 0xFFFF; bx = t_;
      ZF = (bx == 0); SF = (bx >> 15); }
    cx = *(dw*)raddr(es, 0x22);
    si = 0x24;
    di = 0x18;
    di = *(dw*)raddr(es, di);
    load_overlay_e13b46_c();
}

/* e13af9: EXEC error != 2 — code 8 gets the format msg, rest generic. */
void load_overlay_e13b08_c(void){
    dx = 0x1A69;
    ah = 9;
    dos_print_string();
    load_overlay_e13baa_c();
}
void load_overlay_e13af9_c(void){
    CF = (dd)ax < (dd)8;
    ZF = ((dw)(ax - 8) == 0); SF = (((dw)(ax - 8)) >> 15);
    if (ax != 8){ load_overlay_e13b08_c(); return; }
    dx = 0x1A4A;
    ah = 9;
    dos_print_string();
    load_overlay_e13baa_c();
}

/* e13aca: EXEC(AL=3) the name popped off the stack into the alloc'd
 * block — param block at ds:0x19C4 gets {loadseg, relf} = ovl seg;
 * EXEC clobbers ss:sp so save/restore them via ovl_3a92/ovl_3a90. */
void load_overlay_e13aca_c(void){
    ovl_e840 = ax;
    ovl_e844 = ax;
    ovl_e846 = ax;
    bx = 0x19C4;
    ax = 0x4B03;
    dx = pop();
    ovl_3a90 = sp;
    cx = ss;
    ovl_3a92 = cx;
    dos_exec();
    if (!CF){ load_overlay_e13b12_c(); return; }
    CF = (dd)ax < (dd)2;
    ZF = ((dw)(ax - 2) == 0); SF = (((dw)(ax - 2)) >> 15);
    if (ax != 2){ load_overlay_e13af9_c(); return; }
    dx = 0x1A37;
    ah = 9;
    dos_print_string();
    load_overlay_e13baa_c();
}

/* e13ab6: real ALLOC of the probed size (bx paras). */
void load_overlay_e13ab6_c(void){
    ah = 0x48;
    ovl_e842 = bx;
    dos_alloc();
    if (CF){
        dx = 0x1A01;
        ah = 9;
        dos_print_string();
        load_overlay_e13baa_c();
        return;
    }
    load_overlay_e13aca_c();
}

void load_overlay_c(void){
    ax = seg_data;
    ds = ax;
    es = ax;
    CF = (((dd)bx << 1) >> 16) & 1; bx <<= 1;
    ZF = (bx == 0); SF = (bx >> 15);
    dx = *(dw*)raddr(ds, bx + 0x19E2);      /* name ptr for index bx */
    push(dx);
    ah = 0x48;
    bx = 0xFFFF;                            /* probe: alloc all paras */
    dos_alloc();                            /* must fail -> bx = max  */
    if (!CF){                               /* impossible success   */
        dx = 0x19E6;
        ah = 9;
        dos_print_string();
        load_overlay_e13baa_c();
        return;
    }
    load_overlay_e13ab6_c();
}
