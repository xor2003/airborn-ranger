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

/* ---------- palette upload + mode dispatch (loc_10397 family) ----------
   load_palette / load_palette_b set ds=seg_data and the 16-entry color
   table (0x1962 normal, 0x1972 alt), then tail into pal_upload which
   dispatches through pal_jt[adapter_id].  Non-MCGA cases land in the
   int-10h AL=0 per-register loop (load_palette_e103a5); the MCGA case
   writes DAC registers directly (pal_upload_mcga), mirroring each of
   the 16 colors into DAC regs i and i+0x18 via the RGB component
   tables at ds:0xB8/0xC8/0xD8.  Only the MCGA case resolves to a real
   proc in this build; other adapters hit the unresolved-jmp fallback,
   which the readable form reproduces verbatim. */

static void jt_tail(dd flat){           /* indirect jmp through a cs table */
    vfn f_ = func_at(flat);
    if (f_) f_();
    else fprintf(stderr, "unresolved ind jmp %x\n", flat);
}

void pal_upload_c(void){                /* loc_10397 */
    di = adapter_id;
    CF = (((dd)di << 1) >> 16) & 1; di <<= 1;
    ZF = (di == 0); SF = (di >> 15);
    jt_tail((dd)RES_CSBASE + *(dw*)(((db*)&jpt_1039d) + di));
}

void load_palette_c(void){              /* sub_1037E */
    ax = seg_data;
    ds = ax;
    palette_ptr = 0x1962;
    pal_upload_c();
}

void load_palette_b_c(void){            /* sub_1036F — alt palette table */
    ax = seg_data;
    ds = ax;
    palette_ptr = 0x1972;
    pal_upload_c();
}

void load_palette_e103a5_c(void){       /* loc_103A5: EGA/Tandy reg loop */
    do {
        push(si);
        bx = si;
        { dd t_ = (dd)si + palette_ptr; CF = t_ > 0xFFFF; si = t_;
          ZF = (si == 0); SF = (si >> 15); }
        bh = *(db*)raddr(ds, si) & 0x0F; CF = 0; OF = 0;
        ZF = (bh == 0); SF = (bh >> 7);
        ax = 0x1000;                    /* SET PALETTE REGISTER bl=si bh=color */
        bios_palette();
        si = pop();
        (si)++; ZF = (si == 0); SF = (si >> 15);
        CF = si < 0x10; ZF = ((dw)(si - 0x10) == 0); SF = (((dw)(si - 0x10)) >> 15);
    } while (si < 0x10);
}

static void pal_dac_pair(void){         /* write DAC regs bx=i and i+0x18 */
    { dd t_ = (dd)si + palette_ptr; CF = t_ > 0xFFFF; si = t_;
      ZF = (si == 0); SF = (si >> 15); }
    al = *(db*)raddr(ds, si);
    ax &= 0x0F; CF = 0; OF = 0;
    ZF = (ax == 0); SF = (ax >> 15);
    si = ax;                            /* color index -> component tables */
    ch = *(db*)raddr(ds, si + 0x0D8);   /* G */
    cl = *(db*)raddr(ds, si + 0x0C8);   /* B */
    dh = *(db*)raddr(ds, si + 0x0B8);   /* R */
    ax = 0x1010;                        /* SET INDIVIDUAL DAC REGISTER */
    bios_palette();
}

void pal_upload_mcga_e10421_c(void){    /* loc_10421: MCGA DAC loop body */
    do {
        push(si);
        bx = si;
        pal_dac_pair();                 /* DAC[bx] = rgb(pal[bx]) */
        si = pop();
        push(si);
        bx = si;
        { dd t_ = (dd)bx + 0x18; CF = t_ > 0xFFFF; bx = t_;
          ZF = (bx == 0); SF = (bx >> 15); }
        pal_dac_pair();                 /* DAC[i+0x18] = same color */
        si = pop();
        (si)++; ZF = (si == 0); SF = (si >> 15);
        CF = si < 0x10; ZF = ((dw)(si - 0x10) == 0); SF = (((dw)(si - 0x10)) >> 15);
    } while (si < 0x10);
}

void pal_upload_mcga_c(void){           /* loc_1041E — jumptable case 3 */
    si = 0;
    pal_upload_mcga_e10421_c();
}

void rec_walk_e10491_c(void){           /* loc_10491: 6-byte rec walker  */
    do {
        si = *(dw*)raddr(ds, di);
        CF = si < 0xFFFF; ZF = ((dw)(si - 0xFFFF) == 0); SF = (((dw)(si - 0xFFFF)) >> 15);
        if (si == 0xFFFF) return;
        bx = *(dw*)raddr(ds, di + 2);
        dx = *(dw*)raddr(ds, di + 4);
        es = seg_data;
        ax = 0;
        { dd t_ = (dd)di + 6; CF = t_ > 0xFFFF; di = t_;
          ZF = (di == 0); SF = (di >> 15); }
        push(di);
        load_palette();                 /* per-record apply (jt case 0) */
        di = pop();
    } while (1);
}

/* ---------- mode-call dispatch (loc_10797 family) ---------- */

void glyph_conv_dispatch_c(void){       /* sub_10797 — mode-1 post-load  */
    es = res_seg;
    ax = 0;
    si = res_ptr;
    dx = *(dw*)raddr(es, si);           /* glyph count at buf+0          */
    bx = si;
    { dd t_ = (dd)bx + 2; CF = t_ > 0xFFFF; bx = t_;
      ZF = (bx == 0); SF = (bx >> 15); }/* bx = glyph data               */
    { dd t_ = (dd)si + 0x42; CF = t_ > 0xFFFF; si = t_;
      ZF = (si == 0); SF = (si >> 15); }
    bp = si;                            /* bp = second table             */
    di = adapter_id;
    CF = (((dd)di << 1) >> 16) & 1; di <<= 1;
    ZF = (di == 0); SF = (di >> 15);
    jt_tail((dd)RES_CSBASE + *(dw*)(((db*)&glyphconv_jt) + di));
}

void mode_call_c(void){                 /* sub_10834 — ds:0194 indexed   */
    bx = *(dw*)raddr(ds, 0x194);
    CF = (((dd)bx << 1) >> 16) & 1; bx <<= 1;
    ZF = (bx == 0); SF = (bx >> 15);
    { vfn f_ = func_at((dd)RES_CSBASE + *(dw*)(((db*)&funcs_1083a) + bx));
      dw sp_ = sp;
      if (f_) f_();
      else fprintf(stderr, "unresolved ind call %x\n",
                   (dd)(RES_CSBASE + *(dw*)(((db*)&funcs_1083a) + bx)));
      if ((short)(sp - sp_) > 0) { sp = sp_; return; } }
    CF = 0;
}

/* ---------- render pacing (sub_14ACD family) ----------
 * render_tick is the combat frame pump: a down-counter paces renders
 * (reloaded from stage_parm_b), the body composes + flips one frame,
 * and the tail drains the int-1Ch tick budget before reloading
 * timer_cnt from stage_parm_a — floored at 1 tick (lift.py pacing
 * floor; a 0 reload would free-run the battle ~1500x too fast).    */

static void render_body(void){          /* loc_14ADD: compose + flip    */
    compose_gate = 1;
    compose_frame_cond();
    video_bufs_setup();
    hud_weapon_update();
    fx_overlay_fill();
    flip_frame();
    mcga_dirty_update();
    compose_gate = 0;
    farcall_ptr_a9e();
}

static void render_wait(void){          /* loc_14AFE: drain + reload    */
    do {
        CF = 0; ZF = ((dw)(timer_cnt) == 0); SF = (((dw)(timer_cnt)) >> 15);
    } while (timer_cnt != 0);
    ax = stage_parm_a;
    timer_cnt = ax ? ax : 1;            /* pacing floor — never 0 */
}

void render_tick_c(void){               /* sub_14ACD */
    (ot0a_tick_65ac)++;
    ZF = (ot0a_tick_65ac == 0); SF = (ot0a_tick_65ac >> 15);
    (render_cnt)--;
    ZF = (render_cnt == 0); SF = (render_cnt >> 15);
    if ((short)render_cnt >= 0) return; /* still pacing — skip frame    */
    ax = stage_parm_b;
    render_cnt = ax;
    render_body();
    render_wait();
}

void redraw_frame_c(void){              /* sub_14ADD: unconditional      */
    render_body();
    render_wait();
}

void redraw_frame_e14afe_c(void){       /* loc_14AFE: wait tail only     */
    render_wait();
}

/* ---------- hit-flash timer (sub_16D73) ----------
 * hit_flash counts down per tick; flash_period = 0x1E while flashing,
 * 0xFF (idle sentinel) when it hits 0.                             */

void hitflash_dec_e16d84_c(void){       /* loc_16D84: store period       */
    flash_period = al;
}

void hitflash_dec_e16d7e_c(void){       /* loc_16D7E: dec + active period */
    (hit_flash)--; ZF = (hit_flash == 0); SF = (hit_flash >> 7);
    al = 0x1E;
    hitflash_dec_e16d84_c();
}

void hitflash_dec_c(void){              /* sub_16D73 */
    al = hit_flash;
    al |= al; CF = 0; OF = 0; ZF = (al == 0); SF = (al >> 7);
    if (al == 0){ al = 0xFF; hitflash_dec_e16d84_c(); return; }
    hitflash_dec_e16d7e_c();
}

/* ---------- object slot tables (sub_160E6 family) ----------
 * The object system is columnar: slot i's fields live at
 * ds:(i - TABLE_OFF) & 0xFFFF across ~50 byte tables — type byte at
 * ds:0xBE8D+i, weights at ds:0xC8A7+i, alive flag at ds:0xC487+i.   */

void find_free_slot_b_e160e9_c(void){   /* loc_160E9: scan loop head     */
    for (;;){
        al = *(db*)raddr(ds, si - 0x4173);
        al |= al; CF = 0; OF = 0; ZF = (al == 0); SF = (al >> 7);
        if (al == 0){ CF = 0; return; }              /* free: si = index  */
        (si)++; ZF = (si == 0); SF = (si >> 15);
        CF = (dd)si < 0x22; ZF = ((dw)(si - 0x22) == 0); SF = (((dw)(si - 0x22)) >> 15);
        if (si >= 0x22){ CF = 1; return; }           /* all 34 slots used */
    }
}

void find_free_slot_b_c(void){          /* sub_160E6 */
    si = 0;
    find_free_slot_b_e160e9_c();
}

void find_free_slot_b_e160f9_c(void){   /* loc_160F9: found tail         */
    CF = 0;
}

void obj_rec_clear_c(void){             /* sub_160FB: clear slot record  */
    static const dw zoffs[47] = {       /* fields zeroed                 */
        0x4173,0x4151,0x412F,0x4085,0x4063,0x4041,0x401F,0x3FFD,
        0x3FDB,0x3FB9,0x3F97,0x3F75,0x3F53,0x3F31,0x3F0F,0x3EED,
        0x3ECB,0x3EA9,0x3E87,0x3E65,0x3E43,0x3E21,0x3DFF,0x3DDD,
        0x3DBB,0x3D99,0x3D77,0x3D55,0x3CEF,0x3CCD,0x3CAB,0x3C89,
        0x410D,0x40EB,0x40C9,0x40A7,0x3C45,0x3C23,0x3B79,0x3B57,
        0x3B35,0x3B13,0x3AF1,0x3ACF,0x3AAD,0x3A8B,0x3A69 };
    static const dw foffs[3] = { 0x3D33, 0x3D11, 0x3C67 };  /* 0xFF marks */
    unsigned i;
    al = 0;
    for (i = 0; i < 47; i++) *(db*)raddr(ds, si - zoffs[i]) = al;
    al = 0xFF;
    for (i = 0; i < 3; i++)  *(db*)raddr(ds, si - foffs[i]) = al;
}

void obj_tick_all_c(void){              /* sub_1891A: per-type dispatch  */
    bx = 0;
    do {
        otick_a7a8 = bx;
        al = *(db*)raddr(ds, bx - 0x4173);           /* slot type byte   */
        al |= al; CF = 0; OF = 0; ZF = (al == 0); SF = (al >> 7);
        if (al != 0){
            ah = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
            CF = (((dd)ax << 1) >> 16) & 1; ax <<= 1;
            ZF = (ax == 0); SF = (ax >> 15);
            si = ax;
            { vfn f_ = func_at((dd)RES_CSBASE + *(dw*)(((db*)&objtype_tbl) + si));
              dw sp_ = sp;
              if (f_) f_();
              else fprintf(stderr, "unresolved ind call %x\n",
                           (dd)(RES_CSBASE + *(dw*)(((db*)&objtype_tbl) + si)));
              if ((short)(sp - sp_) > 0){ sp = sp_; return; } }
            CF = (dd)bx < (dd)otick_a7a8;
            ZF = ((dw)(bx - otick_a7a8) == 0); SF = (((dw)(bx - otick_a7a8)) >> 15);
            if (bx != otick_a7a8){                 /* callee moved the slot */
                al = *(db*)raddr(ds, bx - 0x4173);
                ah = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
                otick_d8ea = ax;
                si = otick_a7a8;
                al = *(db*)raddr(ds, si - 0x4173);
                otick_d8ec = ax;
                pushf(); push(ax);
                keytest_d0 = ax;
                ax = 0x4F;
                nullsub_4();                        /* debug hook (dead)   */
                ax = pop(); popf();
            }
            bx = otick_a7a8;
        }
        (bx)++; ZF = (bx == 0); SF = (bx >> 15);
        CF = (dd)bx < 0x22; ZF = ((dw)(bx - 0x22) == 0); SF = (((dw)(bx - 0x22)) >> 15);
    } while (bx < 0x22);
}

void obj_alive_mark_e16b8b_c(void){     /* loc_16B8B: set alive flag     */
    al = *(db*)raddr(ds, bx - 0x3B79);
    al |= al; CF = 0; OF = 0; ZF = (al == 0); SF = (al >> 7);
    if (al != 0) return;
    screen_state = 0x1E;
    al = 1;
    *(db*)raddr(ds, bx - 0x3B79) = al;
}

void obj_alive_mark_c(void){            /* sub_16B72: map/deadzone gates */
    al = map_kind;
    CF = (dd)al < 0x0B; ZF = ((db)(al - 0x0B) == 0); SF = (((db)(al - 0x0B)) >> 7);
    if (al != 0x0B) return;
    al = *(db*)raddr(ds, bx - 0x3FFD);
    CF = (dd)al < 0xEE; ZF = ((db)(al - 0xEE) == 0); SF = (((db)(al - 0xEE)) >> 7);
    if (al >= 0xEE) return;
    CF = (dd)bx < 0; ZF = ((dw)(bx - 0) == 0); SF = (((dw)(bx - 0)) >> 15);
    if (bx == 0) wounds = 5;
    obj_alive_mark_e16b8b_c();
}

/* ---------- AI weight/difficulty (sub_16D4E family) ----------
 * slot_weight_sum: ax = sum over si=5..0 of count[si]*weight[si]
 * (count table ds:[si-0x376B], weight table ds:[si-0x3759]); the
 * 16-bit accumulate runs as al + carry into ah.  Epilogue stores
 * weight_sum and returns ax = sum>>3.                              */

static void wsum_accum(void){           /* loc_16D5D: cx-folded add      */
    do {
        { dd t_ = (dd)al + *(db*)raddr(ds, si - 0x3759);
          CF = t_ > 0xFF; al = t_; ZF = (al == 0); SF = (al >> 7); }
        { dd t_ = (dd)ah + CF;
          CF = t_ > 0xFF; ah = t_; ZF = (ah == 0); SF = (ah >> 7); }
    } while (--cx != 0);
}

static void wsum_epi(void){             /* loc_16D69: store + /8         */
    weight_sum = ax;
    CF = ax & 1; ax >>= 1; ZF = (ax == 0); SF = (ax >> 15);
    CF = ax & 1; ax >>= 1; ZF = (ax == 0); SF = (ax >> 15);
    CF = ax & 1; ax >>= 1; ZF = (ax == 0); SF = (ax >> 15);
}

static void wsum_outer(void){           /* loc_16D53: per-slot iter      */
    for (;;){
        cl = *(db*)raddr(ds, si - 0x376B);
        cx &= 0x0FF; CF = 0; OF = 0; ZF = (cx == 0); SF = (cx >> 15);
        if (cx != 0) wsum_accum();
        (si)--; ZF = (si == 0); SF = (si >> 15);
        if ((short)si < 0){ wsum_epi(); return; }
    }
}

void slot_weight_sum_c(void){           /* sub_16D4E */
    si = 5;
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    wsum_outer();
}
void slot_weight_sum_e16d53_c(void){ wsum_outer(); }    /* mid: own si/ax */
void slot_weight_sum_e16d5d_c(void){                    /* mid: inner      */
    wsum_accum();
    (si)--; ZF = (si == 0); SF = (si >> 15);
    if ((short)si < 0){ wsum_epi(); return; }
    wsum_outer();
}
void slot_weight_sum_e16d66_c(void){                    /* mid: dec point  */
    (si)--; ZF = (si == 0); SF = (si >> 15);
    if ((short)si < 0){ wsum_epi(); return; }
    wsum_outer();
}

void ai_param_fetch_c(void){            /* sub_16D13: difficulty params  */
    slot_weight_sum();
    *(db*)(&scan_id) = al;              /* live-population estimate     */
    al = wounds;
    CF = (((dd)al << 1) >> 8) & 1; al <<= 1; ZF = (al == 0); SF = (al >> 7);
    { dd t_ = (dd)al + *(db*)&scan_id; CF = t_ > 0xFF; al = t_;
      ZF = (al == 0); SF = (al >> 7); }
    CF = (dd)al < 5; ZF = ((db)(al - 5) == 0); SF = (((db)(al - 5)) >> 7);
    if (al >= 5) al = 5;                /* clamp 0..5                   */
    CF = (((dd)al << 1) >> 8) & 1; al <<= 1; ZF = (al == 0); SF = (al >> 7);
    dl = diff_parm;
    dh = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    si = dx;
    dl |= dl; CF = 0; OF = 0; ZF = (dl == 0); SF = (dl >> 7);
    CF = (dd)si < 2; ZF = ((dw)(si - 2) == 0); SF = (((dw)(si - 2)) >> 15);
    if (si < 2){                        /* easy levels get +0xC bias    */
        dd t_ = (dd)al + 0x0C; CF = t_ > 0xFF; al = t_;
        ZF = (al == 0); SF = (al >> 7);
    }
    ah = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    si = ax;
    ax = *(dw*)raddr(ds, si - 0x331B);  /* param tables: ds:0xCCE5+i    */
    ai_parm_b = ax;
    ax = *(dw*)raddr(ds, si - 0x3303);  /*                 ds:0xCCFD+i  */
    ai_parm_a = ax;
}

void target_pri_decay_c(void){          /* sub_16B56: target heat decay  */
    al = pri_best;
    al |= al; CF = 0; OF = 0; ZF = (al == 0); SF = (al >> 7);
    if ((signed char)al < 0){           /* wrapped negative: clamp      */
        al = 0;
        pri_best = al;
        al |= al; CF = 0; OF = 0; ZF = 1; SF = 0;
    }
    if (ZF) return;                     /* no live target               */
    al = cool_gate;
    al |= al; CF = 0; OF = 0; ZF = (al == 0); SF = (al >> 7);
    if (al != 0) return;                /* still cooling                */
    (pri_best)--; ZF = (pri_best == 0); SF = (pri_best >> 7);
}

/* ---- frame dispatch (seg000:0116..1C5F) — compose/flip/vblank ---------
 * Adapter-indexed video tails: present_jt / flip_jt / compose_jt /
 * compose2_jt / clrback_jt all key off word_1D934 (adapter_id). Only the
 * MCGA bodies (case 3) survive in this build — other cases land in the
 * deleted-driver gaps and resolve as unresolved far jumps, which the
 * readable dispatch preserves verbatim via func_at.
 */
void farcall_ptr_a9e_c(void){             /* sub_10116: deferred overlay call */
    bx = *(dw*)raddr(ds, 0x0A9E);         /* slot id, 0xFFFF = none      */
    CF = (dd)bx < (dd)0x0FFFF; ZF = (bx == 0x0FFFF);
    SF = (((dw)(bx - 0x0FFFF)) >> 15);
    if (bx == 0x0FFFF) return;
    CF = ((dd)bx << 1) >> 16 & 1; bx <<= 1; ZF = (bx == 0); SF = (bx >> 15);
    push(ds);
    patched_trampoline();                 /* call far ptr sub_1E829      */
    ds = pop();
    *(dw*)raddr(ds, 0x0A9E) = 0x0FFFF;
}

void compose_frame_cond_e11962_c(void){   /* adapter==3: clear compose state */
    dirtyrect_ptr = 0;
    cframe_e0a3 = 0;
    cframe_e12f = 0;
}
void compose_frame_cond_c(void){          /* sub_11952                     */
    ax = seg_data; ds = ax;
    CF = (dd)adapter_id < 3; ZF = (adapter_id == 3);
    SF = (((dw)(adapter_id - 3)) >> 15);
    if (adapter_id != 3){ compose_frame_e11983(); return; }
    compose_frame_cond_e11962_c();
}

static void compose_present(void){        /* loc_11983: present_jt dispatch */
    ax = seg_flip; es = ax;
    di = adapter_id;
    al = adpt_draw;
    bx = seg_draw; ds = bx;
    CF = ((dd)di << 1) >> 16 & 1; di <<= 1; ZF = (di == 0); SF = (di >> 15);
    { vfn f_ = func_at((dd)0x1a20 + (*(dw*)(((db*)&present_jt)+di)));
      if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n",
          (dd)((dd)0x1a20 + (*(dw*)(((db*)&present_jt)+di)))); return; }
}
void compose_frame_e11983_c(void){ compose_present(); }
void compose_frame_c(void){               /* sub_1197D                     */
    ax = seg_data; ds = ax;
    compose_present();
}
void present_mcga_c(void){                /* jumptable 11999 case 3        */
    di = 0; si = 0; cx = 0x7D00;
    while (cx--) { *(dw*)raddr_(es,di) = *(dw*)raddr_(ds,si);
                   si += DF?-2:2; di += DF?-2:2; }
    ax = seg_data; ds = ax;
}

static void flip_tail(void){              /* loc_11B7A                     */
    ax = seg_data; ds = ax;
}
void flip_frame_e11a5c_c(void){           /* tandy page-B path             */
    blit_sel = 1;
    adpt_draw = 0x0E6; adpt_flip = 0x0F6;
    bl = 4; bh = 6;
    ax = 0x0583;
    bios_video(ax);
    flip_tail();
}
void flip_mcga_c(void){                   /* jumptable 11A25 case 3        */
    *(dw*)raddr(ds, 0x0AB7) = 0;
    es = seg_screen;
    ax = seg_flip; ds = ax;
    si = 0; di = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    cx = 0x7D00;
    while (cx--) { *(dw*)raddr_(es,di) = *(dw*)raddr_(ds,si);
                   si += DF?-2:2; di += DF?-2:2; }
    flip_tail();
}
void flip_frame_e11b7a_c(void){ flip_tail(); }
void flip_frame_c(void){                  /* sub_11A19                     */
    ax = seg_data; ds = ax;
    di = adapter_id;
    CF = ((dd)di << 1) >> 16 & 1; di <<= 1; ZF = (di == 0); SF = (di >> 15);
    { vfn f_ = func_at((dd)0x1a20 + (*(dw*)(((db*)&flip_jt)+di)));
      if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n",
          (dd)((dd)0x1a20 + (*(dw*)(((db*)&flip_jt)+di)))); return; }
}

void vblank_wait_e11b84_c(void){          /* loc_11B84: poll status bit 3  */
    do { al = in(dx); al &= 8; CF = 0; OF = 0; ZF = (al == 0); SF = (al >> 7);
    } while (al == 0);
}
void vblank_wait_c(void){                 /* sub_11B81                     */
    dx = 0x3DA;
    vblank_wait_e11b84_c();
}

void compose_mcga_c(void){                /* border color via DAC pair     */
    ax = 0x1001; bl = 0;
    bios_palette();
}
void compose_mcga_11c0d_c(void){          /* jumptable 11BC9 case 3        */
    bx = ax;
    bx &= 0x0F; CF = 0; OF = 0; ZF = (bx == 0); SF = (bx >> 15);
    ch = *(db*)raddr(ds, bx + 0x0D8);
    cl = *(db*)raddr(ds, bx + 0x0C8);
    dh = *(db*)raddr(ds, bx + 0x0B8);
    bx = 0;
    ax = 0x1010;
    bios_palette();
}
void locret_11c27_c(void){}

void spr_state_copy_b_c(void){            /* sub_11B8A                     */
    clear_backbuf();
    ax = seg000_1_0d4a;
    bh = seg000_1_0d4c;
    sprst_dst = ax;
    sprst_dstb = bh;
    set_border_color();
}
void set_border_color_c(void){            /* sub_11BBC: compose_jt         */
    cx = seg_data; ds = cx;
    di = adapter_id;
    CF = ((dd)di << 1) >> 16 & 1; di <<= 1; ZF = (di == 0); SF = (di >> 15);
    { vfn f_ = func_at((dd)0x1a20 + (*(dw*)(((db*)&compose_jt)+di)));
      if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n",
          (dd)((dd)0x1a20 + (*(dw*)(((db*)&compose_jt)+di)))); return; }
}
void clear_backbuf_c(void){               /* sub_10B43: clrback_jt         */
    push(ds); push(es);
    ds = seg_data;
    ax = seg_screen; es = ax;
    di = adapter_id;
    CF = ((dd)di << 1) >> 16 & 1; di <<= 1; ZF = (di == 0); SF = (di >> 15);
    { vfn f_ = func_at((dd)0x1a20 + (*(dw*)(((db*)&jpt_10b57)+di)));
      if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n",
          (dd)((dd)0x1a20 + (*(dw*)(((db*)&jpt_10b57)+di)))); return; }
}

static void compose2_dispatch(void){      /* loc_11C4C: compose2_jt        */
    CF = ((dd)di << 1) >> 16 & 1; di <<= 1; ZF = (di == 0); SF = (di >> 15);
    { vfn f_ = func_at((dd)0x1a20 + (*(dw*)(((db*)&compose2_jt)+di)));
      if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n",
          (dd)((dd)0x1a20 + (*(dw*)(((db*)&compose2_jt)+di)))); return; }
}
void adapter_compose_flip_e11c4c_c(void){ compose2_dispatch(); }
void adapter_compose_flip_e11c59_c(void){ /* cases 0,3,4 tail              */
    compose_frame(); flip_frame();
}
void adapter_compose_flip_e11c53_c(void){ /* cases 1,2: double compose     */
    compose_frame(); flip_frame();
    adapter_compose_flip_e11c59_c();
}
void adapter_compose_flip_c(void){        /* sub_11C28                     */
    ax = seg_data; ds = ax;
    di = adapter_id;
    CF = (dd)di < 3; ZF = (di == 3); SF = (((dw)(di - 3)) >> 15);
    if (di == 3) return;
    compose2_dispatch();
}
void compose_flip_c(void){                /* sub_11C42                     */
    ax = seg_data; ds = ax;
    di = adapter_id;
    compose2_dispatch();
}

/* ---- clip/dirty scan (seg000:26B9..3996) — MCGA draw-path setup ------
 * video_bufs_setup walks clip_jt[adapter]; only the MCGA clip scan
 * (clip_go_mcga, case 3) survives in this build. It sweeps 32 rect-reg
 * slots (rectreg_base 0x1F..0) probing column tables at ds:0xF1F/0xF23
 * against bit masks at ds:0xDD5; hit slots feed clip_rest_mcga/_b which
 * paint the strip. mcga_dirty_update walks dirtyrect_ptr through the
 * per-rect column tables and rep-movsw's each run draw->flip.
 */
void video_bufs_init_c(void){             /* sub_11643: bufsel_jt        */
    ax = seg_data; ds = ax;
    di = adapter_id;
    CF = ((dd)di << 1) >> 16 & 1; di <<= 1; ZF = (di == 0); SF = (di >> 15);
    { vfn f_ = func_at((dd)0x1a20 + (*(dw*)(((db*)&bufsel_jt)+di)));
      if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n",
          (dd)((dd)0x1a20 + (*(dw*)(((db*)&bufsel_jt)+di)))); return; }
}

static void clip_dispatch(void){          /* loc_126C9: clip_jt          */
    ax = seg_data; ds = ax;
    di = adapter_id;
    CF = ((dd)di << 1) >> 16 & 1; di <<= 1; ZF = (di == 0); SF = (di >> 15);
    { vfn f_ = func_at((dd)0x1a20 + (*(dw*)(((db*)&clip_jt)+di)));
      if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n",
          (dd)((dd)0x1a20 + (*(dw*)(((db*)&clip_jt)+di)))); return; }
}
void video_bufs_setup_e126c9_c(void){ clip_dispatch(); }
void video_bufs_setup_c(void){            /* sub_126B9                   */
    blitdst_a = seg_flip;
    ax = seg_draw;
    blitdst_b = ax;
    clip_dispatch();
}

static void clip_mcga_iter(void){         /* loc_13047: probe one slot   */
    si = rectreg_base;
    bx = si; cl = 3;
    if (cl){ CF = (bx >> (cl-1)) & 1; bx >>= cl; ZF = (bx == 0); SF = (bx >> 15); }
    si &= 7; CF = 0; OF = 0; ZF = (si == 0); SF = (si >> 15);
    al = *(db*)raddr(ds, bx + 0x0F1F);
    al &= *(db*)raddr(ds, si + 0x0DD5);
    CF = 0; OF = 0; ZF = (al == 0); SF = (al >> 7);
    if (al == 0) return;
    dl = *(db*)raddr(ds, bx + 0x0F23);
    dl &= *(db*)raddr(ds, si + 0x0DD5);
    CF = 0; OF = 0; ZF = (dl == 0); SF = (dl >> 7);
    si = rectreg_base;
    bl = *(db*)raddr(ds, si + 0x0EFF);
    bl |= bl; CF = 0; OF = 0; ZF = (bl == 0); SF = (bl >> 7);
    if ((signed char)bl < 0) return;
    if (bl == 0) return;
    clip_f = bl;
    bl = *(db*)raddr(ds, si + 0x0EDF);
    bh = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    CF = ((dd)bx << 1) >> 16 & 1; bx <<= 1; ZF = (bx == 0); SF = (bx >> 15);
    ax = *(dw*)raddr(ds, bx + 0x1356);
    clip_base = ax;
    al = *(db*)raddr(ds, si + 0x0E7F);
    ah = *(db*)raddr(ds, si + 0x0E9F);
    { CF = (ax >> 0) & 1; ax = (short)ax >> 1; ZF = (ax == 0); SF = (ax >> 15); }
    bl = *(db*)raddr(ds, si + 0x0EBF);
    bh = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    cl = *(db*)raddr(ds, si + 0x0E3F);
    ch = *(db*)raddr(ds, si + 0x0E5F);
    CF = (dd)cx < (dd)0x200; ZF = ((dw)(cx - 0x200) == 0);
    SF = (((dw)(cx - 0x200)) >> 15);
    if (cx >= 0x200) return;
    bp = cx;
    dl |= dl; CF = 0; OF = 0; ZF = (dl == 0); SF = (dl >> 7);
    if (dl == 0){ clip_rest_mcga(); return; }
    clip_rest_mcga_b();
}
static void clip_mcga_scan(void){         /* loc_130B3: dec + loop       */
    for (;;){
        (rectreg_base)--; ZF = (rectreg_base == 0); SF = (rectreg_base >> 15);
        if ((short)rectreg_base < 0) return;
        clip_mcga_iter();
    }
}
void clip_go_mcga_c(void){                /* jumptable 126D5 case 3      */
    rectreg_base = 0x1F;
    rectreg_off = 0;
    CF = (dd)blit_sel < (dd)0; ZF = (blit_sel == 0); SF = (blit_sel >> 15);
    if (blit_sel != 0) rectreg_off = 0x20;
    for (;;){
        clip_mcga_iter();
        (rectreg_base)--; ZF = (rectreg_base == 0); SF = (rectreg_base >> 15);
        if ((short)rectreg_base < 0) return;
    }
}
void clip_go_mcga_e13047_c(void){         /* mid: iter + scan            */
    for (;;){
        clip_mcga_iter();
        (rectreg_base)--; ZF = (rectreg_base == 0); SF = (rectreg_base >> 15);
        if ((short)rectreg_base < 0) return;
    }
}
void clip_go_mcga_e130b0_c(void){         /* mid: rest_b then scan       */
    clip_rest_mcga_b();
    clip_mcga_scan();
}
void clip_go_mcga_e130b3_c(void){         /* mid: bare dec+loop          */
    clip_mcga_scan();
}

static void mcga_dirty_row(void){         /* loc_13943: one rect         */
    di = dirtyrect_ptr;
    al = *(db*)raddr(ds, di + 0x1223);
    ah = *(db*)raddr(ds, di + 0x12AF);
    si = ax;
    dl = *(db*)raddr(ds, di + 0x0F67);
    dh = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    al = *(db*)raddr(ds, di + 0x0FF3);
    blitgo_c = al;
    al = *(db*)raddr(ds, di + 0x107F);
    ah = *(db*)raddr(ds, di + 0x110B);
    bl = *(db*)raddr(ds, di + 0x1197);
    bh = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    CF = ((dd)bx << 1) >> 16 & 1; bx <<= 1; ZF = (bx == 0); SF = (bx >> 15);
    cx = seg_flip; es = cx;
    do {                                  /* loc_13975: blit runs        */
        di = *(dw*)raddr(ds, bx + 0x0BD5);
        ds = seg_draw;
        { dd t_ = (dd)di + (dd)ax; CF = t_ > 0xFFFF; di = t_;
          ZF = (di == 0); SF = (di >> 15); }
        cx = dx;
        while (cx--) { *(dw*)raddr_(es,di) = *(dw*)raddr_(ds,si);
                       si += DF?-2:2; di += DF?-2:2; }
        { dd t_ = (dd)bx + (dd)2; CF = t_ > 0xFFFF; bx = t_;
          ZF = (bx == 0); SF = (bx >> 15); }
        bx &= 0x1FF; CF = 0; OF = 0; ZF = (bx == 0); SF = (bx >> 15);
        ds = seg_data;
        (blitgo_c)--; ZF = (blitgo_c == 0); SF = (blitgo_c >> 7);
    } while (blitgo_c != 0);
}
static void mcga_dirty_scan(void){        /* loc_1393C: dec + loop       */
    for (;;){
        (dirtyrect_ptr)--; ZF = (dirtyrect_ptr == 0); SF = (dirtyrect_ptr >> 15);
        if ((short)dirtyrect_ptr < 0) return;
        mcga_dirty_row();
    }
}
void mcga_dirty_update_c(void){           /* sub_13930                   */
    ax = seg_data;
    CF = (dd)adapter_id < 3; ZF = (adapter_id == 3);
    SF = (((dw)(adapter_id - 3)) >> 15);
    if (adapter_id != 3) return;
    mcga_dirty_scan();
}
void mcga_dirty_update_e1393c_c(void){ mcga_dirty_scan(); }
void mcga_dirty_update_e13943_c(void){    /* mid: row then scan          */
    for (;;){
        mcga_dirty_row();
        (dirtyrect_ptr)--; ZF = (dirtyrect_ptr == 0); SF = (dirtyrect_ptr >> 15);
        if ((short)dirtyrect_ptr < 0) return;
    }
}
void mcga_dirty_update_e13975_c(void){    /* mid: inner run then scan    */
    do {
        di = *(dw*)raddr(ds, bx + 0x0BD5);
        ds = seg_draw;
        { dd t_ = (dd)di + (dd)ax; CF = t_ > 0xFFFF; di = t_;
          ZF = (di == 0); SF = (di >> 15); }
        cx = dx;
        while (cx--) { *(dw*)raddr_(es,di) = *(dw*)raddr_(ds,si);
                       si += DF?-2:2; di += DF?-2:2; }
        { dd t_ = (dd)bx + (dd)2; CF = t_ > 0xFFFF; bx = t_;
          ZF = (bx == 0); SF = (bx >> 15); }
        bx &= 0x1FF; CF = 0; OF = 0; ZF = (bx == 0); SF = (bx >> 15);
        ds = seg_data;
        (blitgo_c)--; ZF = (blitgo_c == 0); SF = (blitgo_c >> 7);
    } while (blitgo_c != 0);
    mcga_dirty_scan();
}

/* ---- HUD weapon panel + fx overlay (seg000:7B10..7D70) ---------------
 * hud_weapon_update derives the ammo/tens digits and the three evac
 * meter words, then fills 11 clip-column records and re-runs
 * video_bufs_setup. fx_overlay_fill clears the overlay columns and
 * copies the meter_b-selected mask pair into clip_mask_a/hud_da1.
 */
static void hud_wpn_loop(void){           /* loc_17B4A: count 10s        */
    do {
        (si)++; ZF = (si == 0); SF = (si >> 15);
        { dd t_ = (dd)al - (dd)0x0A; CF = (dd)al < (dd)0x0A; al = t_;
          ZF = (al == 0); SF = (al >> 7); }
    } while (!CF);
    (si)--; ZF = (si == 0); SF = (si >> 15);
    { dd t_ = (dd)al + (dd)0x0A; CF = t_ > 0xFF; al = t_;
      ZF = (al == 0); SF = (al >> 7); }
    CF = ((dd)si << 1) >> 16 & 1; si <<= 1; ZF = (si == 0); SF = (si >> 15);
    cx = *(dw*)raddr(ds, si - 0x300A);
    wpn_9e38 = cx;
    ah = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    si = ax;
    CF = ((dd)si << 1) >> 16 & 1; si <<= 1; ZF = (si == 0); SF = (si >> 15);
    ax = *(dw*)raddr(ds, si - 0x300A);
    wpn_9e3a = ax;
    bh = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    bl = evac_h;
    CF = (((dd)bl << 1) >> 8) & 1; bl <<= 1; ZF = (bl == 0); SF = (bl >> 7);
    ax = *(dw*)raddr(ds, bx - 0x300A);
    wpn_9e3c = ax;
    bl = *(db*)(&evac_tu);
    CF = (((dd)bl << 1) >> 8) & 1; bl <<= 1; ZF = (bl == 0); SF = (bl >> 7);
    ax = *(dw*)raddr(ds, bx - 0x300A);
    wpn_9e3e = ax;
    bl = *(db*)(((db*)&evac_tu) + 1);
    CF = (((dd)bl << 1) >> 8) & 1; bl <<= 1; ZF = (bl == 0); SF = (bl >> 7);
    wpn_9e40 = *(dw*)raddr(ds, bx - 0x300A);
    cx = 0x13A; ax = 0x13B;
    si = 0x0C;
    CF = (dd)wounds < (dd)0; ZF = ((db)(wounds - 0) == 0);
    SF = (((db)(wounds - 0)) >> 7);
    fld304a_store();
    si = 0x0E;
    CF = (dd)wounds < (dd)1; ZF = ((db)(wounds - 1) == 0);
    SF = (((db)(wounds - 1)) >> 7);
    fld304a_store();
    si = 0x10;
    CF = (dd)wounds < (dd)2; ZF = ((db)(wounds - 2) == 0);
    SF = (((db)(wounds - 2)) >> 7);
    fld304a_store();
    tile_variant_sel();
    bx = 0; si = 0; cx = 0x0B;
}
static void hud_col_fill(void){           /* loc_17BC5: 11 clip records  */
    do {
        ax = *(dw*)raddr(ds, bx - 0x304A);
        *(db*)raddr(ds, si + 0x0E3F) = al;
        *(db*)raddr(ds, si + 0x0E5F) = ah;
        ax = *(dw*)raddr(ds, bx - 0x3060);
        *(db*)raddr(ds, si + 0x0E7F) = al;
        *(db*)raddr(ds, si + 0x0E9F) = ah;
        ax = *(dw*)raddr(ds, bx - 0x3034);
        *(db*)raddr(ds, si + 0x0EBF) = al;
        *(db*)raddr(ds, si + 0x0EDF) = 5;
        *(db*)raddr(ds, si + 0x0EFF) = 0x7F;
        { dd t_ = (dd)bx + (dd)2; CF = t_ > 0xFFFF; bx = t_;
          ZF = (bx == 0); SF = (bx >> 15); }
        (si)++; ZF = (si == 0); SF = (si >> 15);
    } while (--cx != 0);
    clip_mask_a = 0x7FF;
    hud_da1 = 0;
    clip_mask_b = 0;
    hud_da5 = 0;
    video_bufs_setup();
}
void hud_weapon_update_e17bc5_c(void){ hud_col_fill(); }
void hud_weapon_update_e17b4a_c(void){    /* mid: digit loop onward      */
    hud_wpn_loop();
    hud_col_fill();
}
void hud_weapon_update_e17b38_c(void){    /* mid: ammo byte fetch        */
    al = *(db*)raddr(ds, bx - 0x376B);
    CF = (dd)bx < (dd)0; ZF = (bx == 0); SF = (bx >> 15);
    if (bx == 0){
        CF = (dd)flash_period < (dd)0; ZF = (flash_period == 0);
        SF = ((db)(flash_period - 0) >> 7);
        if ((signed char)flash_period >= (signed char)0){
            (al)++; ZF = (al == 0); SF = (al >> 7);
        }
    }
    hud_wpn_loop();
    hud_col_fill();
}
void hud_weapon_update_e17b22_c(void){    /* mid: table load             */
    CF = ((dd)si << 1) >> 16 & 1; si <<= 1; ZF = (si == 0); SF = (si >> 15);
    ax = *(dw*)raddr(ds, si - 0x301E);
    wpn_9e36 = ax;
    si = 0;
    CF = (dd)bx < (dd)3; ZF = (bx == 3); SF = ((dw)(bx - 3) >> 15);
    if (bx == 3){ al = bkey_970e; goto body; }
    al = *(db*)raddr(ds, bx - 0x376B);
    CF = (dd)bx < (dd)0; ZF = (bx == 0); SF = (bx >> 15);
    if (bx == 0){
        CF = (dd)flash_period < (dd)0; ZF = (flash_period == 0);
        SF = ((db)(flash_period - 0) >> 7);
        if ((signed char)flash_period >= (signed char)0){
            (al)++; ZF = (al == 0); SF = (al >> 7);
        }
    }
body:
    hud_wpn_loop();
    hud_col_fill();
}
void hud_weapon_update_c(void){           /* sub_17B10                   */
    bl = weapon_sel;
    bh = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    si = bx;
    CF = (dd)mst_a7af < (dd)0; ZF = (mst_a7af == 0); SF = (mst_a7af >> 7);
    if (mst_a7af != 0){
        dd t_ = (dd)si + (dd)5; CF = t_ > 0xFFFF; si = t_;
        ZF = (si == 0); SF = (si >> 15);
    }
    hud_weapon_update_e17b22_c();
}

static void fx_col_fill(void){            /* loc_17D32: 32 col records   */
    do {
        ax = 0x113;
        *(db*)raddr(ds, si + 0x0E3F) = al;
        *(db*)raddr(ds, si + 0x0E5F) = ah;
        ax = fx_overl_9f60;
        *(db*)raddr(ds, si + 0x0E7F) = al;
        *(db*)raddr(ds, si + 0x0E9F) = ah;
        al = *(db*)raddr(ds, si - 0x2F42);
        { dd t_ = (dd)al - (dd)fx_overl_9f5e;
          CF = (dd)al < (dd)fx_overl_9f5e; al = t_;
          ZF = (al == 0); SF = (al >> 7); }
        *(db*)raddr(ds, si + 0x0EBF) = al;
        *(db*)raddr(ds, si + 0x0EDF) = 5;
        *(db*)raddr(ds, si + 0x0EFF) = 0x7F;
        (si)++; ZF = (si == 0); SF = (si >> 15);
    } while (--cx != 0);
    clip_mask_b = 0;
    hud_da5 = 0;
    video_bufs_setup();
}
void fx_overlay_fill_e17d32_c(void){ fx_col_fill(); }
void fx_overlay_fill_e17d0e_c(void){      /* mid: mask pair load         */
    CF = (bx >> 0) & 1; bx >>= 1; ZF = (bx == 0); SF = (bx >> 15);
    al = *(db*)raddr(ds, bx - 0x2FC2);
    *(db*)(&clip_mask_a) = al;
    al = *(db*)raddr(ds, bx - 0x2FA2);
    *(db*)(((db*)&clip_mask_a) + 1) = al;
    al = *(db*)raddr(ds, bx - 0x2F82);
    *(db*)(&hud_da1) = al;
    al = *(db*)raddr(ds, bx - 0x2F62);
    *(db*)(((db*)&hud_da1) + 1) = al;
    si = 0; cx = 0x20;
    fx_col_fill();
}
void fx_overlay_fill_e17cfd_c(void){      /* mid: parity + masks         */
    (bx)--; ZF = (bx == 0); SF = (bx >> 15);
    fx_overl_9f5e = 0;
    CF = 0; OF = 0; ZF = ((bx & 1) == 0); SF = (((bx & 1)) >> 15);
    if ((bx & 1) == 0) fx_overl_9f5e = 4;
    fx_overlay_fill_e17d0e_c();
}
void fx_overlay_fill_c(void){             /* sub_17CF4                   */
    bx = meter_b;
    bx |= bx; CF = 0; OF = 0; ZF = (bx == 0); SF = (bx >> 15);
    if (bx == 0) return;
    fx_overlay_fill_e17cfd_c();
}

/* ---- MCGA tile/glyph blitters (seg000:0C14..1C4F) ---------------------
 * tile_blit_flipbuf pushes ds/es, aims es at seg_flip (passed via di),
 * then ind-jmps through tileblit_jt[adapter]; the MCGA case
 * tile_blit_mcga runs tile_row_mcga and falls into the shared
 * col++/row+=8 advance tail that pops the saved segs. tile_row_mcga is
 * the 8bpp translate loop: pattern byte bl indexes color tables at
 * ds:0x3C70/0x3D70 -> word store to es:di (pitch 0x140). glyph_blit_
 * mcga is the same shape with the wparm_a/b glyph-bit masks folded in.
 * mapcols_draw walks a 32-col row either raw (map_base>=0x40) or from
 * the map cell table at ds:bx-0x68CA with the 0x80 bit set.
 */
static void tileblit_advance(void){         /* ret_1a2_16f6_e1175b tail  */
    (draw_col)++; ZF = (draw_col == 0); SF = (draw_col >> 15);
    CF = (dd)draw_col < (dd)0x28; ZF = (draw_col == 0x28);
    SF = ((dw)(draw_col - 0x28) >> 15);
    if (draw_col >= 0x28){
        draw_col = 0;
        { dd t_ = (dd)draw_row + (dd)8; CF = t_ > 0xFFFF; draw_row = t_;
          ZF = (draw_row == 0); SF = (draw_row >> 15); }
        CF = (dd)draw_row < (dd)0x0C8; ZF = (draw_row == 0x0C8);
        SF = ((dw)(draw_row - 0x0C8) >> 15);
        if (draw_row >= 0x0C8) draw_row = 0x0C7;
    }
    es = pop();
    ds = pop();
}
void tile_blit_flipbuf_c(void){
    di = seg_flip;
    bx = 0;
    push(ds);
    push(es);
    es = di;
    ds = seg_data;
    di = adapter_id;
    CF = ((dd)di << 1) >> 16 & 1; di <<= 1; ZF = (di == 0); SF = (di >> 15);
    { vfn f_ = func_at((dd)0x1a20 + (*(dw*)(((db*)&tileblit_jt) + di)));
      if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n",
          (dd)((dd)0x1a20 + (*(dw*)(((db*)&tileblit_jt) + di))));
      return; }
}
static void tile_row_mcga_loop(void){       /* loc_118C6: 8-row xlate    */
    do {
        bl = *(db*)raddr(ds, si);
        al = *(db*)raddr(ds, bx + 0x3C70);
        ah = *(db*)raddr(ds, bx + 0x3D70);
        *(dw*)(raddr(es, di)) = ax;
        bl = *(db*)raddr(ds, si + 1);
        al = *(db*)raddr(ds, bx + 0x3C70);
        ah = *(db*)raddr(ds, bx + 0x3D70);
        *(dw*)(raddr(es, di + 2)) = ax;
        bl = *(db*)raddr(ds, si + 2);
        al = *(db*)raddr(ds, bx + 0x3C70);
        ah = *(db*)raddr(ds, bx + 0x3D70);
        *(dw*)(raddr(es, di + 4)) = ax;
        bl = *(db*)raddr(ds, si + 3);
        al = *(db*)raddr(ds, bx + 0x3C70);
        ah = *(db*)raddr(ds, bx + 0x3D70);
        *(dw*)(raddr(es, di + 6)) = ax;
        { dd t_ = (dd)si + (dd)4; CF = t_ > 0xFFFF; si = t_;
          ZF = (si == 0); SF = (si >> 15); }
        { dd t_ = (dd)di + (dd)0x140; CF = t_ > 0xFFFF; di = t_;
          ZF = (di == 0); SF = (di >> 15); }
    } while (--cx != 0);
}
void tile_row_mcga_e118c6_c(void){ tile_row_mcga_loop(); }
void tile_row_mcga_c(void){
    di = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    bx = *(dw*)raddr(ds, 0x0A82);
    CF = ((dd)bx << 1) >> 16 & 1; bx <<= 1; ZF = (bx == 0); SF = (bx >> 15);
    { dd t_ = (dd)di + (dd)*(dw*)raddr(ds, bx + 0x0BD5); CF = t_ > 0xFFFF;
      di = t_; ZF = (di == 0); SF = (di >> 15); }
    bx = *(dw*)raddr(ds, 0x0A80);
    CF = ((dd)bx << 1) >> 16 & 1; bx <<= 1; ZF = (bx == 0); SF = (bx >> 15);
    CF = ((dd)bx << 1) >> 16 & 1; bx <<= 1; ZF = (bx == 0); SF = (bx >> 15);
    CF = ((dd)bx << 1) >> 16 & 1; bx <<= 1; ZF = (bx == 0); SF = (bx >> 15);
    { dd t_ = (dd)di + (dd)bx; CF = t_ > 0xFFFF; di = t_;
      ZF = (di == 0); SF = (di >> 15); }
    cl = 5;
    if (cl){ CF = ((dd)ax << cl) >> 16 & 1; ax <<= cl;
             ZF = (ax == 0); SF = (ax >> 15); }
    { dd t_ = (dd)si + (dd)ax; CF = t_ > 0xFFFF; si = t_;
      ZF = (si == 0); SF = (si >> 15); }
    cx = 8;
    bh = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    tile_row_mcga_loop();
}
void tile_blit_mcga_c(void){
    tile_row_mcga();
    tileblit_advance();
}
static void glyph_blit_mcga_loop(void){     /* loc_10D9E: masked xlate   */
    do {
        bl = *(db*)raddr(ds, si);
        cl = *(db*)raddr(ds, bx + 0x3B70);
        bl &= eblit_d6dc; CF = 0; OF = 0; ZF = (bl == 0); SF = (bl >> 7);
        cl &= eblit_d6dd; CF = 0; OF = 0; ZF = (cl == 0); SF = (cl >> 7);
        bl |= cl; CF = 0; OF = 0; ZF = (bl == 0); SF = (bl >> 7);
        al = *(db*)raddr(ds, bx + 0x3C70);
        ah = *(db*)raddr(ds, bx + 0x3D70);
        *(dw*)(raddr(es, di)) = ax;
        bl = *(db*)raddr(ds, si + 1);
        cl = *(db*)raddr(ds, bx + 0x3B70);
        bl &= eblit_d6dc; CF = 0; OF = 0; ZF = (bl == 0); SF = (bl >> 7);
        cl &= eblit_d6dd; CF = 0; OF = 0; ZF = (cl == 0); SF = (cl >> 7);
        bl |= cl; CF = 0; OF = 0; ZF = (bl == 0); SF = (bl >> 7);
        al = *(db*)raddr(ds, bx + 0x3C70);
        ah = *(db*)raddr(ds, bx + 0x3D70);
        *(dw*)(raddr(es, di + 2)) = ax;
        bl = *(db*)raddr(ds, si + 2);
        cl = *(db*)raddr(ds, bx + 0x3B70);
        bl &= eblit_d6dc; CF = 0; OF = 0; ZF = (bl == 0); SF = (bl >> 7);
        cl &= eblit_d6dd; CF = 0; OF = 0; ZF = (cl == 0); SF = (cl >> 7);
        bl |= cl; CF = 0; OF = 0; ZF = (bl == 0); SF = (bl >> 7);
        al = *(db*)raddr(ds, bx + 0x3C70);
        ah = *(db*)raddr(ds, bx + 0x3D70);
        *(dw*)(raddr(es, di + 4)) = ax;
        bl = *(db*)raddr(ds, si + 3);
        cl = *(db*)raddr(ds, bx + 0x3B70);
        bl &= eblit_d6dc; CF = 0; OF = 0; ZF = (bl == 0); SF = (bl >> 7);
        cl &= eblit_d6dd; CF = 0; OF = 0; ZF = (cl == 0); SF = (cl >> 7);
        bl |= cl; CF = 0; OF = 0; ZF = (bl == 0); SF = (bl >> 7);
        al = *(db*)raddr(ds, bx + 0x3C70);
        ah = *(db*)raddr(ds, bx + 0x3D70);
        *(dw*)(raddr(es, di + 6)) = ax;
        { dd t_ = (dd)si + (dd)4; CF = t_ > 0xFFFF; si = t_;
          ZF = (si == 0); SF = (si >> 15); }
        { dd t_ = (dd)di + (dd)0x140; CF = t_ > 0xFFFF; di = t_;
          ZF = (di == 0); SF = (di >> 15); }
        (ch)--; ZF = (ch == 0); SF = (ch >> 7);
    } while (ch != 0);
}
void glyph_blit_mcga_e10d9e_c(void){ glyph_blit_mcga_loop(); }
void glyph_blit_mcga_c(void){
    bx = draw_row;
    CF = ((dd)bx << 1) >> 16 & 1; bx <<= 1; ZF = (bx == 0); SF = (bx >> 15);
    di = *(dw*)raddr(ds, bx + 0x0BD5);
    bx = draw_col;
    CF = ((dd)bx << 1) >> 16 & 1; bx <<= 1; ZF = (bx == 0); SF = (bx >> 15);
    CF = ((dd)bx << 1) >> 16 & 1; bx <<= 1; ZF = (bx == 0); SF = (bx >> 15);
    CF = ((dd)bx << 1) >> 16 & 1; bx <<= 1; ZF = (bx == 0); SF = (bx >> 15);
    { dd t_ = (dd)di + (dd)bx; CF = t_ > 0xFFFF; di = t_;
      ZF = (di == 0); SF = (di >> 15); }
    cl = 5;
    if (cl){ CF = ((dd)ax << cl) >> 16 & 1; ax <<= cl;
             ZF = (ax == 0); SF = (ax >> 15); }
    { dd t_ = (dd)ax + (dd)glyph_rows; CF = t_ > 0xFFFF; ax = t_;
      ZF = (ax == 0); SF = (ax >> 15); }
    si = ax;
    ax = wparm_a;
    eblit_d6dc = al;
    ax = wparm_b;
    eblit_d6dd = al;
    ch = 8;
    bh = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    glyph_blit_mcga_loop();
}
void glyph_put_mcga_10c1c_c(void){
    glyph_blit_mcga();
    text_cursor_next();
    es = pop();
}
void glyph_put2_mcga_c(void){
    glyph_blit_mcga();
    text_cursor_next();
    es = pop();
}

static void mapcols_loop_a(void){           /* loc_1BCDF: raw 32 cols    */
    do {
        push(cx);
        ax = maprow_base;
        si = 0x1B52;
        tile_blit_flipbuf();
        cx = pop();
    } while (--cx != 0);
}
void mapcols_draw_e1bcdf_c(void){ mapcols_loop_a(); }
void mapcols_draw_e1bcdc_c(void){
    cx = 0x20;
    mapcols_loop_a();
}
static void mapcols_loop_b(void){           /* loc_1BCF4: cell-mapped    */
    do {
        push(cx);
        push(bx);
        al = *(db*)raddr(ds, bx - 0x68CA);
        ah = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
        al |= 0x80; CF = 0; OF = 0; ZF = (al == 0); SF = (al >> 7);
        si = 0x1B52;
        tile_blit_flipbuf();
        bx = pop();
        (bx)++; ZF = (bx == 0); SF = (bx >> 15);
        cx = pop();
    } while (--cx != 0);
}
void mapcols_draw_e1bcf4_c(void){ mapcols_loop_b(); }
void mapcols_draw_e1bced_c(void){
    cl = 5;
    if (cl){ CF = ((dd)bx << cl) >> 16 & 1; bx <<= cl;
             ZF = (bx == 0); SF = (bx >> 15); }
    cx = 0x20;
    mapcols_loop_b();
}
void mapcols_draw_c(void){
    bx = map_base;
    bx |= bx; CF = 0; OF = 0; ZF = (bx == 0); SF = (bx >> 15);
    if ((short)bx >= 0){
        CF = (dd)bx < (dd)0x40; ZF = (bx == 0x40);
        SF = ((dw)(bx - 0x40) >> 15);
        if (bx < 0x40){ mapcols_draw_e1bced_c(); return; }
    }
    mapcols_draw_e1bcdc_c();
}

/* ---- blitflag + scroll/pan (seg000:382E..3D60) -------------------------
 * blitflag_go_mcga sweeps the 32-slot rect-flag table: each nonzero flag
 * [si+0xF67] holds a word width, [si+0xFF3] a run count, [si+0x1197]
 * the row index, [si+0x107F]/[si+0x110B] the column pair; the inner loop
 * copies that many words seg_draw -> seg_flip per run and clears the
 * flag. scroll_go_a..h are the pan handlers: es=ds=seg_draw, an indirect
 * call through scroll_tbl_x (far-thunk sp restore), then scroll_edge_x.
 */
static void blitflag_runs(void){            /* loc_13884: run copy       */
    do {
        bx &= 0x1FF; CF = 0; OF = 0; ZF = (bx == 0); SF = (bx >> 15);
        si = *(dw*)raddr(ds, bx + 0x0BD5);
        { dd t_ = (dd)si + (dd)dx; CF = t_ > 0xFFFF; si = t_;
          ZF = (si == 0); SF = (si >> 15); }
        di = si;
        cl = *(db*)(&blitgo_a);
        ch = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
        ds = bp;
        while (cx--) { *(dw*)raddr_(es, di) = *(dw*)raddr_(ds, si);
                       si += DF ? -2 : 2; di += DF ? -2 : 2; }
        ds = ax;
        { dd t_ = (dd)bx + (dd)2; CF = t_ > 0xFFFF; bx = t_;
          ZF = (bx == 0); SF = (bx >> 15); }
        (blitgo_c)--; ZF = (blitgo_c == 0); SF = (blitgo_c >> 7);
    } while (blitgo_c != 0);
}
static void blitflag_load(void){            /* loc_1385C: slot load      */
    *(db*)raddr(ds, si + 0x0F67) = 0;
    *(db*)(&blitgo_a) = al;
    al = *(db*)raddr(ds, si + 0x0FF3);
    blitgo_c = al;
    bl = *(db*)raddr(ds, si + 0x1197);
    bh = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    dl = *(db*)raddr(ds, si + 0x107F);
    dh = *(db*)raddr(ds, si + 0x110B);
    CF = ((dd)bx << 1) >> 16 & 1; bx <<= 1;
    ZF = (bx == 0); SF = (bx >> 15);
    ax = seg_data;
    bp = seg_draw;
}
static void blitflag_fill(void){
    blitflag_load();
    blitflag_runs();
}
static int blitflag_probe(void){            /* loc_1384D: flag test      */
    si = dirtyrect_ptr;
    al = *(db*)raddr(ds, si + 0x0F67);
    al |= al; CF = 0; OF = 0; ZF = (al == 0); SF = (al >> 7);
    return al != 0;
}
static int blitflag_next(void){             /* loc_138A5: dec, 1 = done  */
    (dirtyrect_ptr)--; ZF = (dirtyrect_ptr == 0);
    SF = (dirtyrect_ptr >> 15);
    return (short)dirtyrect_ptr < 0;
}
static void blitflag_scan(void){            /* loc_1384D: rect sweep     */
    for (;;){
        if (blitflag_probe()) blitflag_fill();
        if (blitflag_next()) return;
    }
}
void blitflag_go_mcga_e138a5_c(void){       /* mid: dec-first sweep      */
    for (;;){
        if (blitflag_next()) return;
        if (blitflag_probe()) blitflag_fill();
    }
}
void blitflag_go_mcga_e13884_c(void){       /* mid: runs-first sweep     */
    for (;;){
        blitflag_runs();
        for (;;){
            if (blitflag_next()) return;
            if (blitflag_probe()) break;
        }
        blitflag_load();
    }
}
void blitflag_go_mcga_e1385c_c(void){       /* mid: fill-first sweep     */
    for (;;){
        blitflag_fill();
        for (;;){
            if (blitflag_next()) return;
            if (blitflag_probe()) break;
        }
    }
}
void blitflag_go_mcga_e1384d_c(void){ blitflag_scan(); }
void blitflag_go_mcga_c(void){
    ax = seg_flip;
    es = ax;
    dirtyrect_ptr = 0x1F;
    rectreg_off = 0;
    CF = (dd)blit_sel < (dd)0; ZF = (blit_sel == 0);
    SF = ((dw)(blit_sel - 0) >> 15);
    if (blit_sel != 0) rectreg_off = 0x20;
    blitflag_scan();
}

static void scroll_go_common(db *tbl, vfn edge, vfn edge2){
    /* ind call through the per-direction table, sp-restoring thunk */
    { vfn f_ = func_at((dd)0x1a20 + (*(dw*)(tbl + di)));
      dw sp_ = sp;
      if (f_) f_();
      else fprintf(stderr, "unresolved ind call %x\n",
          (dd)((dd)0x1a20 + (*(dw*)(tbl + di))));
      if ((short)(sp - sp_) > 0){ sp = sp_; return; } }
    ax = seg_data;
    ds = ax;
    edge();
    if (edge2) edge2();
}
void scroll_go_a_c(void){
    ax = seg_draw;
    es = ax;
    di = adapter_id;
    CF = ((dd)di << 1) >> 16 & 1; di <<= 1; ZF = (di == 0); SF = (di >> 15);
    ds = ax;
    scroll_go_common((db*)&scroll_tbl_a, scroll_edge_a, 0);
}
static void scroll_go_idx(void){            /* shared b..h head          */
    ax = seg_draw;
    es = ax;
    di = *(dw*)raddr(ds, 0x0AB4);
    CF = ((dd)di << 1) >> 16 & 1; di <<= 1; ZF = (di == 0); SF = (di >> 15);
    ds = ax;
}
void scroll_go_b_c(void){
    scroll_go_idx();
    scroll_go_common((db*)&scroll_tbl_b, scroll_edge_b, 0);
}
void scroll_go_c_c(void){
    scroll_go_idx();
    scroll_go_common((db*)&scroll_tbl_c, scroll_edge_c, 0);
}
void scroll_go_d_c(void){
    scroll_go_idx();
    scroll_go_common((db*)&scroll_tbl_d, scroll_edge_d, 0);
}
void scroll_go_e_c(void){                   /* diagonal: edge b+d        */
    scroll_go_idx();
    scroll_go_common((db*)&scroll_tbl_e, scroll_edge_b, scroll_edge_d);
}
void scroll_go_f_c(void){                   /* diagonal: edge c+d        */
    scroll_go_idx();
    scroll_go_common((db*)&scroll_tbl_f, scroll_edge_c, scroll_edge_d);
}
void scroll_go_g_c(void){                   /* diagonal: edge b+a        */
    scroll_go_idx();
    scroll_go_common((db*)&scroll_tbl_g, scroll_edge_b, scroll_edge_a);
}
void scroll_go_h_c(void){                   /* diagonal: edge c+a        */
    scroll_go_idx();
    scroll_go_common((db*)&scroll_tbl_h, scroll_edge_c, scroll_edge_a);
}

/* ---- scroll_edge_a..d (seg000:42xx..44xx) ------------------------------
 * Per-edge tile compose loops driven off the mode flag [ds:0xC7FD]&4
 * (double-step variant) and the scroll offsets at [0xC801]/[0xC803];
 * each iteration stages [0xA80]/[0xA82]/[0xDBE5]/[0xDBE7] then calls
 * cell_tile_compose. a = bottom row sweep, d = top row, b = right col,
 * c = left col; b/c carry the e143e1-style re-entry loop for the
 * wraparound column.
 */
void scroll_edge_a_e142ac_c(void){          /* 40-col sweep loop         */
    do {
        ax = *(dw*)raddr(ds, 0x96F2);
        { dd t_ = (dd)ax + (dd)*(dw*)raddr(ds, 0x0C801); CF = t_ > 0xFFFF;
          ax = t_; ZF = (ax == 0); SF = (ax >> 15); }
        *(dw*)(raddr(ds, 0x0DBE7)) = ax;
        *(dw*)(raddr(ds, 0x0A82)) = 0x0C4;
        ax = *(dw*)raddr(ds, 0x96F0);
        *(dw*)(raddr(ds, 0x0A80)) = ax;
        { dd t_ = (dd)ax + (dd)*(dw*)raddr(ds, 0x0C803); CF = t_ > 0xFFFF;
          ax = t_; ZF = (ax == 0); SF = (ax >> 15); }
        *(dw*)(raddr(ds, 0x0DBE5)) = ax;
        cell_tile_compose();
        (*(dw*)raddr(ds, 0x96F0))++; ZF = (*(dw*)raddr(ds,0x96F0) == 0);
        SF = (*(dw*)raddr(ds,0x96F0) >> 15);
        CF = (dd)*(dw*)raddr(ds,0x96F0) < (dd)0x28;
        ZF = (*(dw*)raddr(ds,0x96F0) == 0x28);
        SF = ((dw)(*(dw*)raddr(ds,0x96F0) - 0x28) >> 15);
    } while (*(dw*)raddr(ds,0x96F0) < 0x28);
    si = pop();
    bx = pop();
}
void scroll_edge_a_e1429f_c(void){
    *(dw*)(raddr(ds,0x0C992)) = ax;
    *(dw*)(raddr(ds,0x96F0)) = 0;
    *(dw*)(raddr(ds,0x96F2)) = cx;
    scroll_edge_a_e142ac_c();
}
void scroll_edge_a_c(void){
    push(bx);
    push(si);
    ax = 2;
    cx = 0x18;
    CF = 0; OF = 0; ZF = ((*(db*)raddr(ds,0x0C7FD) & 4) == 0);
    SF = (((db)(*(db*)raddr(ds,0x0C7FD) & 4)) >> 7);
    if ((*(db*)raddr(ds,0x0C7FD) & 4) != 0){ ax = 1; cx = 0x19; }
    scroll_edge_a_e1429f_c();
}
void scroll_edge_d_e142f6_c(void){          /* top-row sweep loop        */
    do {
        ax = *(dw*)raddr(ds,0x0C801);
        *(dw*)(raddr(ds,0x0DBE7)) = ax;
        *(dw*)(raddr(ds,0x0A82)) = 0;
        ax = *(dw*)raddr(ds,0x96F0);
        *(dw*)(raddr(ds,0x0A80)) = ax;
        { dd t_ = (dd)ax + (dd)*(dw*)raddr(ds,0x0C803); CF = t_ > 0xFFFF;
          ax = t_; ZF = (ax == 0); SF = (ax >> 15); }
        *(dw*)(raddr(ds,0x0DBE5)) = ax;
        cell_tile_compose();
        (*(dw*)raddr(ds,0x96F0))++; ZF = (*(dw*)raddr(ds,0x96F0) == 0);
        SF = (*(dw*)raddr(ds,0x96F0) >> 15);
        CF = (dd)*(dw*)raddr(ds,0x96F0) < (dd)0x28;
        ZF = (*(dw*)raddr(ds,0x96F0) == 0x28);
        SF = ((dw)(*(dw*)raddr(ds,0x96F0) - 0x28) >> 15);
    } while (*(dw*)raddr(ds,0x96F0) < 0x28);
    si = pop();
    bx = pop();
}
void scroll_edge_d_e142ed_c(void){
    *(dw*)(raddr(ds,0x0C992)) = ax;
    *(dw*)(raddr(ds,0x96F0)) = 0;
    scroll_edge_d_e142f6_c();
}
void scroll_edge_d_c(void){
    push(bx);
    push(si);
    ax = 1;
    dx = *(dw*)raddr(ds,0x0C7FD);
    CF = 0; OF = 0; ZF = ((*(db*)raddr(ds,0x0C7FD) & 4) == 0);
    SF = (((db)(*(db*)raddr(ds,0x0C7FD) & 4)) >> 7);
    if ((*(db*)raddr(ds,0x0C7FD) & 4) != 0) ax = 2;
    scroll_edge_d_e142ed_c();
}
static void scroll_edge_bc_body(int left){  /* shared b/c column loop    */
    for (;;){                                 /* goto e143xx re-enters   */
        do {
            push(*(dw*)raddr(ds,0x0A82));
            if (left){
                ax = 0x27;
                *(dw*)(raddr(ds,0x0A80)) = ax;
                { dd t_ = (dd)ax + (dd)*(dw*)raddr(ds,0x0C803);
                  CF = t_ > 0xFFFF; ax = t_; ZF = (ax == 0);
                  SF = (ax >> 15); }
                *(dw*)(raddr(ds,0x0DBE5)) = ax;
                ax = *(dw*)raddr(ds,0x96F0);
                { dd t_ = (dd)ax + (dd)*(dw*)raddr(ds,0x0C801);
                  CF = t_ > 0xFFFF; ax = t_; ZF = (ax == 0);
                  SF = (ax >> 15); }
                *(dw*)(raddr(ds,0x0DBE7)) = ax;
            } else {
                *(dw*)(raddr(ds,0x0A80)) = 0;
                ax = *(dw*)raddr(ds,0x0C803);
                *(dw*)(raddr(ds,0x0DBE5)) = ax;
                ax = *(dw*)raddr(ds,0x96F0);
                { dd t_ = (dd)ax + (dd)*(dw*)raddr(ds,0x0C801);
                  CF = t_ > 0xFFFF; ax = t_; ZF = (ax == 0);
                  SF = (ax >> 15); }
                *(dw*)(raddr(ds,0x0DBE7)) = ax;
            }
            cell_tile_compose();
            ax = pop();
            { dd t_ = (dd)ax + (dd)*(dw*)raddr(ds,0x96F8);
              CF = t_ > 0xFFFF; ax = t_; ZF = (ax == 0); SF = (ax >> 15); }
            *(dw*)(raddr(ds,0x0A82)) = ax;
            *(dw*)(raddr(ds,0x96F8)) = 8;
            *(dw*)(raddr(ds,0x0C992)) = 0;
            (*(dw*)raddr(ds,0x96F0))++; ZF = (*(dw*)raddr(ds,0x96F0) == 0);
            SF = (*(dw*)raddr(ds,0x96F0) >> 15);
            ax = *(dw*)raddr(ds,0x96F0);
            CF = (dd)ax < (dd)*(dw*)raddr(ds,0x96F2);
            ZF = (ax == *(dw*)raddr(ds,0x96F2));
            SF = ((dw)(ax - *(dw*)raddr(ds,0x96F2)) >> 15);
        } while (ax < *(dw*)raddr(ds,0x96F2));
        if (!(ax <= *(dw*)raddr(ds,0x96F2))) break;
        CF = (dd)*(dw*)raddr(ds,0x96FA) < (dd)0;
        ZF = (*(dw*)raddr(ds,0x96FA) == 0);
        SF = ((dw)(*(dw*)raddr(ds,0x96FA) - 0) >> 15);
        if (*(dw*)raddr(ds,0x96FA) != 0)
            *(dw*)(raddr(ds,0x0C992)) = 1;
    }
    si = pop();
    bx = pop();
}
void scroll_edge_b_e143e1_c(void){ scroll_edge_bc_body(0); }
void scroll_edge_b_e143cd_c(void){
    *(dw*)(raddr(ds,0x0C992)) = ax;
    *(dw*)(raddr(ds,0x96FA)) = ax;
    *(dw*)(raddr(ds,0x96F8)) = cx;
    *(dw*)(raddr(ds,0x96F2)) = bx;
    *(dw*)(raddr(ds,0x96F0)) = 0;
    scroll_edge_bc_body(0);
}
void scroll_edge_b_e14430_c(void){ si = pop(); bx = pop(); }
void scroll_edge_b_c(void){
    push(bx);
    push(si);
    *(dw*)(raddr(ds,0x0A82)) = 0;
    ax = 0;
    cx = 8;
    bx = 0x18;
    CF = 0; OF = 0; ZF = ((*(db*)raddr(ds,0x0C7FD) & 4) == 0);
    SF = (((db)(*(db*)raddr(ds,0x0C7FD) & 4)) >> 7);
    if ((*(db*)raddr(ds,0x0C7FD) & 4) != 0){ ax = 2; cx = 4; bx = 0x19; }
    scroll_edge_b_e143cd_c();
}
void scroll_edge_c_e14359_c(void){ scroll_edge_bc_body(1); }
void scroll_edge_c_e14345_c(void){
    *(dw*)(raddr(ds,0x0C992)) = ax;
    *(dw*)(raddr(ds,0x96FA)) = ax;
    *(dw*)(raddr(ds,0x96F8)) = cx;
    *(dw*)(raddr(ds,0x96F2)) = bx;
    *(dw*)(raddr(ds,0x96F0)) = 0;
    scroll_edge_bc_body(1);
}
void scroll_edge_c_e143a9_c(void){ si = pop(); bx = pop(); }
void scroll_edge_c_c(void){
    push(bx);
    push(si);
    *(dw*)(raddr(ds,0x0A82)) = 0;
    ax = 0;
    cx = 8;
    bx = 0x18;
    dx = *(dw*)raddr(ds,0x0C7FD);
    CF = 0; OF = 0; ZF = ((*(db*)raddr(ds,0x0C7FD) & 4) == 0);
    SF = (((db)(*(db*)raddr(ds,0x0C7FD) & 4)) >> 7);
    if ((*(db*)raddr(ds,0x0C7FD) & 4) != 0){ ax = 2; cx = 4; bx = 0x19; }
    scroll_edge_c_e14345_c();
}

/* ---- cell->tile compose chain + status bar (seg000:B26x..1D81, 7Cxx) ---
 * cell_tile_compose: px_to_cell (probe_px/py >>3 -> cell_x/y),
 * map_probe_xy (bounds + map byte at cell_base+cell_x, CF=1 on OOB),
 * map_rowhdr_get (two-level row-header tables at ds:0x3F22/0x6322
 * indexed by (probe_y&7)*16 + (probe_x&7)*2 -> row_meta), then
 * draw_tile_compose (draw_row/col -> es:di into seg_draw, sprrow_tbl
 * dispatch on (tilemap_page + adapter_id), shared col++/row+=8 tail).
 */
void px_to_cell_c(void){
    ax = probe_px;
    cl = 3;
    if (cl){ CF = (ax >> (cl - 1)) & 1; ax = ax >> cl;
             ZF = (ax == 0); SF = (ax >> 15); }
    cell_x = al;
    ax = probe_py;
    cl = 3;
    if (cl){ CF = (ax >> (cl - 1)) & 1; ax = ax >> cl;
             ZF = (ax == 0); SF = (ax >> 15); }
    cell_y = al;
}
static void map_probe_fin(void){            /* loc_1B29D: stage + ret    */
    pushf();
    spawn_a6d = al;
    probe_a6e = cell_x;
    probe_a6f = cell_y;
    al = spawn_a6d;
    popf();
}
void map_probe_xy_e1b29d_c(void){ map_probe_fin(); }
void map_probe_xy_e1b2b2_c(void){           /* mid: OOB -> CF=1          */
    CF = 1;
    al = 0;
    map_probe_fin();
}
void map_probe_xy_c(void){
    al = cell_x;
    CF = (dd)al < (dd)0x20; ZF = ((db)(al - 0x20) == 0);
    SF = (((db)(al - 0x20)) >> 7);
    if (al >= 0x20){ CF = 1; al = 0; map_probe_fin(); return; }
    ah = cell_y;
    CF = (dd)ah < (dd)0x40; ZF = ((db)(ah - 0x40) == 0);
    SF = (((db)(ah - 0x40)) >> 7);
    if (ah >= 0x40){ CF = 1; al = 0; map_probe_fin(); return; }
    si = 0;
    al = 0;
    CF = (ax >> 0) & 1; ax = ax >> 1; ZF = (ax == 0); SF = (ax >> 15);
    CF = (ax >> 0) & 1; ax = ax >> 1; ZF = (ax == 0); SF = (ax >> 15);
    CF = (ax >> 0) & 1; ax = ax >> 1; ZF = (ax == 0); SF = (ax >> 15);
    { dd t_ = (dd)ax + (dd)0x9736; CF = t_ > 0xFFFF; ax = t_;
      ZF = (ax == 0); SF = (ax >> 15); }
    cell_base = ax;
    dl = cell_x;
    dh = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    si = dx;
    dl |= dl; CF = 0; OF = 0; ZF = (dl == 0); SF = (dl >> 7);
    bp = cell_base;
    al = *(db*)raddr(ds, bp + si);
    al |= al; CF = 0; OF = 0; ZF = (al == 0); SF = (al >> 7);
    CF = 0;
    map_probe_fin();
}
void map_rowhdr_get_c(void){
    ah = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    CF = ((dd)ax << 1) >> 16 & 1; ax <<= 1; ZF = (ax == 0); SF = (ax >> 15);
    di = ax;
    maprow_idx = di;
    di = *(dw*)raddr(ds, di + 0x3F22);
    { dd t_ = (dd)di + (dd)0x3F22; CF = t_ > 0xFFFF; di = t_;
      ZF = (di == 0); SF = (di >> 15); }
    al = *(db*)(&probe_py);
    ax &= 7; CF = 0; OF = 0; ZF = (ax == 0); SF = (ax >> 15);
    CF = ((dd)ax << 1) >> 16 & 1; ax <<= 1; ZF = (ax == 0); SF = (ax >> 15);
    CF = ((dd)ax << 1) >> 16 & 1; ax <<= 1; ZF = (ax == 0); SF = (ax >> 15);
    CF = ((dd)ax << 1) >> 16 & 1; ax <<= 1; ZF = (ax == 0); SF = (ax >> 15);
    CF = ((dd)ax << 1) >> 16 & 1; ax <<= 1; ZF = (ax == 0); SF = (ax >> 15);
    map_rowh_aa44 = ax;
    { dd t_ = (dd)di + (dd)ax; CF = t_ > 0xFFFF; di = t_;
      ZF = (di == 0); SF = (di >> 15); }
    al = *(db*)(&probe_px);
    ax &= 7; CF = 0; OF = 0; ZF = (ax == 0); SF = (ax >> 15);
    CF = ((dd)ax << 1) >> 16 & 1; ax <<= 1; ZF = (ax == 0); SF = (ax >> 15);
    { dd t_ = (dd)di + (dd)ax; CF = t_ > 0xFFFF; di = t_;
      ZF = (di == 0); SF = (di >> 15); }
    map_rowh_aa46 = ax;
    ax = *(dw*)raddr(ds, di);
    di = maprow_idx;
    di = *(dw*)raddr(ds, di + 0x6322);
    { dd t_ = (dd)di + (dd)0x6322; CF = t_ > 0xFFFF; di = t_;
      ZF = (di == 0); SF = (di >> 15); }
    { dd t_ = (dd)di + (dd)map_rowh_aa44; CF = t_ > 0xFFFF; di = t_;
      ZF = (di == 0); SF = (di >> 15); }
    { dd t_ = (dd)di + (dd)map_rowh_aa46; CF = t_ > 0xFFFF; di = t_;
      ZF = (di == 0); SF = (di >> 15); }
    di = *(dw*)raddr(ds, di);
    row_meta = di;
}
void draw_tile_compose_c(void){
    ds = seg_data;
    di = seg_draw;
    es = di;
    bx = draw_row;
    CF = ((dd)bx << 1) >> 16 & 1; bx <<= 1; ZF = (bx == 0); SF = (bx >> 15);
    di = *(dw*)raddr(ds, bx + 0x0BD5);
    bx = draw_col;
    CF = ((dd)bx << 1) >> 16 & 1; bx <<= 1; ZF = (bx == 0); SF = (bx >> 15);
    { dd t_ = (dd)di + (dd)*(dw*)raddr(ds, bx + 0x0AEF); CF = t_ > 0xFFFF;
      di = t_; ZF = (di == 0); SF = (di >> 15); }
    bx = tmap_9812;
    CF = ((dd)bx << 1) >> 16 & 1; bx <<= 1; ZF = (bx == 0); SF = (bx >> 15);
    bx = *(dw*)raddr(ds, bx + 0x948);
    { dd t_ = (dd)bx + (dd)adapter_id; CF = t_ > 0xFFFF; bx = t_;
      ZF = (bx == 0); SF = (bx >> 15); }
    CF = ((dd)bx << 1) >> 16 & 1; bx <<= 1; ZF = (bx == 0); SF = (bx >> 15);
    dx = row_meta;
    cx = seg_resbuf;
    ds = cx;
    si = 0x42;
    { vfn f_ = func_at((dd)0x1a20 + (*(dw*)(((db*)&sprrow_tbl) + bx)));
      dw sp_ = sp;
      if (f_) f_();
      else fprintf(stderr, "unresolved ind call %x\n",
          (dd)((dd)0x1a20 + (*(dw*)(((db*)&sprrow_tbl) + bx))));
      if ((short)(sp - sp_) > 0){ sp = sp_; return; } }
    ax = seg_data;
    ds = ax;
    (draw_col)++; ZF = (draw_col == 0); SF = (draw_col >> 15);
    CF = (dd)draw_col < (dd)0x28; ZF = (draw_col == 0x28);
    SF = ((dw)(draw_col - 0x28) >> 15);
    if (draw_col < 0x28) return;
    draw_col = 0;
    { dd t_ = (dd)draw_row + (dd)8; CF = t_ > 0xFFFF; draw_row = t_;
      ZF = (draw_row == 0); SF = (draw_row >> 15); }
    CF = (dd)draw_row < (dd)0x0C8; ZF = (draw_row == 0x0C8);
    SF = ((dw)(draw_row - 0x0C8) >> 15);
    if (draw_row < 0x0C8) return;
    draw_row = 0x0C7;
}
void cell_tile_compose_c(void){
    px_to_cell();
    map_probe_xy();
    map_rowhdr_get();
    draw_tile_compose();
}

void bar_tandy_c(void){                     /* sub_17C69-ish bar draw    */
    al = *(db*)raddr(ds, si - 0x2FEE);
    ah = al;
    di = bp;
    dx |= dx; CF = 0; OF = 0; ZF = (dx == 0); SF = (dx >> 15);
    if (dx != 0){
        if ((short)dx >= 0){
            do {                              /* loc_17C77 fill words    */
                *(dw*)(raddr(ss, bp + 0)) = 0x0DD88;
                *(dw*)(raddr(ss, bp + 2)) = 0x0DDDD;
                { dd t_ = (dd)bp + (dd)4; CF = t_ > 0xFFFF; bp = t_;
                  ZF = (bp == 0); SF = (bp >> 15); }
                (dx)--; ZF = (dx == 0); SF = (dx >> 15);
            } while (dx != 0);
        }
    }
    CF = (dd)bx < (dd)0x16; ZF = (bx == 0x16);
    SF = ((dw)(bx - 0x16) >> 15);
    if (bx > 0x16) bx = 0x16;
    do {                                      /* loc_17C8F color pairs   */
        *(db*)raddr(ss, bp + 0) = 0x88;
        *(db*)raddr(ss, bp + 1) = al;
        *(dw*)(raddr(ss, bp + 2)) = ax;
        { dd t_ = (dd)bp + (dd)4; CF = t_ > 0xFFFF; bp = t_;
          ZF = (bp == 0); SF = (bp >> 15); }
        (bx)--; ZF = (bx == 0); SF = (bx >> 15);
    } while (bx != 0);
    bp = di;
    al = *(db*)raddr(ss, bp + 0x19);
    al &= 0x0F; CF = 0; OF = 0; ZF = (al == 0); SF = (al >> 7);
    al |= 0x80; CF = 0; OF = 0; ZF = (al == 0); SF = (al >> 7);
    *(db*)raddr(ss, bp + 0x19) = al;
    al = *(db*)raddr(ss, bp + 0x1B);
    al &= 0x0F0; CF = 0; OF = 0; ZF = (al == 0); SF = (al >> 7);
    al |= 8; CF = 0; OF = 0; ZF = (al == 0); SF = (al >> 7);
    *(db*)raddr(ss, bp + 0x1B) = al;
}

/* ---- MCGA tile pixel writer + text cursor (seg000:1EB3, 0FD0) ----------
 * blit_tile_mcga: es=seg_flip, si += ax*32 (glyph/tile index into the
 * seg_resbuf pattern data), then 8 rows of 4 pixel pairs: pattern byte
 * bl indexes the resbuf translate tables at bx-0x1000/bx-0x0F00 -> word
 * store at es:di, pitch 0x140. This is the actual background-tile
 * pixel path reached via sprrow_tbl from draw_tile_compose.
 * text_cursor_next: shared draw_col++/draw_row+=8 advance (the
 * 0xC0/0x27 bottom-of-screen clamp).
 */
static void blit_tile_mcga_loop(void){      /* loc_11EC5: 8-row xlate    */
    do {
        bl = *(db*)raddr(ds, si);
        al = *(db*)raddr(ds, bx - 0x1000);
        ah = *(db*)raddr(ds, bx - 0x0F00);
        *(dw*)(raddr(es, di)) = ax;
        bl = *(db*)raddr(ds, si + 1);
        al = *(db*)raddr(ds, bx - 0x1000);
        ah = *(db*)raddr(ds, bx - 0x0F00);
        *(dw*)(raddr(es, di + 2)) = ax;
        bl = *(db*)raddr(ds, si + 2);
        al = *(db*)raddr(ds, bx - 0x1000);
        ah = *(db*)raddr(ds, bx - 0x0F00);
        *(dw*)(raddr(es, di + 4)) = ax;
        bl = *(db*)raddr(ds, si + 3);
        al = *(db*)raddr(ds, bx - 0x1000);
        ah = *(db*)raddr(ds, bx - 0x0F00);
        *(dw*)(raddr(es, di + 6)) = ax;
        { dd t_ = (dd)si + (dd)4; CF = t_ > 0xFFFF; si = t_;
          ZF = (si == 0); SF = (si >> 15); }
        { dd t_ = (dd)di + (dd)0x140; CF = t_ > 0xFFFF; di = t_;
          ZF = (di == 0); SF = (di >> 15); }
    } while (--cx != 0);
}
void blit_tile_mcga_e11ec5_c(void){ blit_tile_mcga_loop(); }
void blit_tile_mcga_c(void){
    cx = seg_flip;
    es = cx;
    cl = 5;
    if (cl){ CF = ((dd)ax << cl) >> 16 & 1; ax <<= cl;
             ZF = (ax == 0); SF = (ax >> 15); }
    { dd t_ = (dd)si + (dd)ax; CF = t_ > 0xFFFF; si = t_;
      ZF = (si == 0); SF = (si >> 15); }
    cx = 8;
    bh = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    blit_tile_mcga_loop();
}
void text_cursor_next_c(void){              /* sub_10FD0: col wrap       */
    (draw_col)++; ZF = (draw_col == 0); SF = (draw_col >> 15);
    CF = (dd)draw_col < (dd)0x28; ZF = (draw_col == 0x28);
    SF = ((dw)(draw_col - 0x28) >> 15);
    if (draw_col < 0x28) return;
    draw_col = 0;
    { dd t_ = (dd)draw_row + (dd)8; CF = t_ > 0xFFFF; draw_row = t_;
      ZF = (draw_row == 0); SF = (draw_row >> 15); }
    CF = (dd)draw_row < (dd)0x0C8; ZF = (draw_row == 0x0C8);
    SF = ((dw)(draw_row - 0x0C8) >> 15);
    if (draw_row < 0x0C8) return;
    draw_row = 0x0C0;
    draw_col = 0x27;
}

/* ---- map-cell attribute lookup + tilemap leaf helpers ---- */
void tile_lookup_c(void) {               /* sub_1B35E: cell attrs       */
    ax &= 0x7F; CF = 0; OF = 0; ZF = (ax == 0); SF = (ax >> 15);
    si = ax;
    bp = tile_loo_b24f;
    al = *(db*)raddr(ds, bp + si);
    al |= al; CF = 0; OF = 0; ZF = (al == 0); SF = (al >> 7);
    ot0d_aa61 = al;
    CF = (si >> 12) & 1; si <<= 4;      /* four shl si,1 */
    ZF = (si == 0); SF = (si >> 15);
    { dd t_ = (dd)si + (dd)tile_loo_b251; CF = t_ > 0xFFFF; si = t_;
      ZF = (si == 0); SF = (si >> 15); }
    al = *(db*)(&probe_py);
    al &= 6; CF = 0; OF = 0; ZF = (al == 0); SF = (al >> 7);
    CF = (al >> 6) & 1; al = (db)(al << 1);
    ZF = (al == 0); SF = (al >> 7);
    ah = *(db*)(&probe_px);
    CF = ah & 1; ah >>= 1;
    ZF = (ah == 0); SF = (ah >> 7);
    ah &= 3; CF = 0; OF = 0; ZF = (ah == 0); SF = (ah >> 7);
    { dd t_ = (dd)al + (dd)ah; CF = t_ > 0xFF; al = t_;
      ZF = (al == 0); SF = (al >> 7); }
    ah = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    { dd t_ = (dd)si + (dd)ax; CF = t_ > 0xFFFF; si = t_;
      ZF = (si == 0); SF = (si >> 15); }
    al = *(db*)raddr(ds, si);
    map_kind = al;
    ah = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    si = ax;
    al = *(db*)raddr(ds, si - 0x2410); odelta_a5c   = al;
    al = *(db*)raddr(ds, si - 0x2403); rplan_aa5d   = al;
    al = *(db*)raddr(ds, si - 0x23F6); omove_a5e    = al;
    al = *(db*)raddr(ds, si - 0x23E9); omove_a5f    = al;
    al = *(db*)raddr(ds, si - 0x23DC); spawn_a64    = al;
    al = *(db*)raddr(ds, si - 0x23CF); motion_t_aa60 = al;
    al = *(db*)raddr(ds, si - 0x23C2); omot_a62     = al;
}
void obj_cell_tile_c(void) {             /* sub_1B344: probe+lookup     */
    map_prob_aa56 = bx;
    map_prob_aa58 = si;
    xy_to_cell();
    obj_cell_tile_e1b34f_c();
}
void obj_cell_tile_e1b34f_c(void) {
    map_probe_xy();
    tile_lookup();
    bx = map_prob_aa56;
    si = map_prob_aa58;
}
void tilemap_init_c(void) {              /* sub_11CD9: field init       */
    glyph_rows = 0x3F22;
    tile_src = 0x8722;
    seg000_1_d969 = 0;
    seg000_1_d965 = 0x28;
    tilemap_compose_e11c82();
}
void build_mcga_lut_c(void) {            /* sub_14B0C: LUT copy gate    */
    ax = seg_data;
    ds = ax;
    CF = (dd)adapter_id < 3; ZF = (adapter_id == 3);
    SF = ((dw)(adapter_id - 3) >> 15);
    if (adapter_id != 3) return;
    build_mcga_lut_e14b1a_c();
}
void build_mcga_lut_e14b1a_c(void) {
    ax = seg_resbuf;
    es = ax;
    si = 0x3C70;
    di = 0x0F000;
    cx = 0x100;
    while (cx--) {
        *(dw*)raddr_(es, di) = *(dw*)raddr_(ds, si);
        si += DF ? -2 : 2; di += DF ? -2 : 2;
    }
}
void blit_dst_patch_c(void) {            /* sub_126A0: dst seg patch    */
    ax = seg_draw;
    blitdst_a = ax;
    blitdst_b = ax;
    video_bufs_setup_e126c9();
}
void tile_draw_3f22_c(void) {            /* sub_154CE: tile @3F22       */
    push(si);
    si = 0x3F22;
    tile_blit();
    si = pop();
}
void tile_blit_c(void) {                 /* sub_116E0: blit entry       */
    di = seg_draw;
    bx = 0;
    farcall_seg0e_e1170b();
}
void rdr_copy_mcga_c(void) {             /* sub_1AA39: 0x7A80w copy     */
    si = 0x500;
    di = 0;
    cx = 0x7A80;
    while (cx--) {
        *(dw*)raddr_(es, di) = *(dw*)raddr_(ds, si);
        si += DF ? -2 : 2; di += DF ? -2 : 2;
    }
}

/* ---- half-tile MCGA row writers (sprrow_tbl cases) ---- */
static void sprrow_half_rows(void) {     /* 4 rows x 4 px via LUTs      */
    do {
        bl = *(db*)raddr(ds, si);
        al = *(db*)raddr(ds, bx - 0x1000);
        ah = *(db*)raddr(ds, bx - 0x0F00);
        *(dw*)(raddr(es, di)) = ax;
        bl = *(db*)raddr(ds, si + 1);
        al = *(db*)raddr(ds, bx - 0x1000);
        ah = *(db*)raddr(ds, bx - 0x0F00);
        *(dw*)(raddr(es, di + 2)) = ax;
        bl = *(db*)raddr(ds, si + 2);
        al = *(db*)raddr(ds, bx - 0x1000);
        ah = *(db*)raddr(ds, bx - 0x0F00);
        *(dw*)(raddr(es, di + 4)) = ax;
        bl = *(db*)raddr(ds, si + 3);
        al = *(db*)raddr(ds, bx - 0x1000);
        ah = *(db*)raddr(ds, bx - 0x0F00);
        *(dw*)(raddr(es, di + 6)) = ax;
        { dd t_ = (dd)si + (dd)4; CF = t_ > 0xFFFF; si = t_;
          ZF = (si == 0); SF = (si >> 15); }
        { dd t_ = (dd)di + (dd)0x140; CF = t_ > 0xFFFF; di = t_;
          ZF = (di == 0); SF = (di >> 15); }
    } while (--cx != 0);
}
void sprrow_mcga_b_c(void) {             /* sub_1203B: left half tile   */
    cx = seg_flip;
    es = cx;
    cl = 5;
    if (cl) {
        CF = (((dd)ax << cl) >> 16) & 1; ax <<= cl;
        ZF = (ax == 0); SF = (ax >> 15);
    }
    { dd t_ = (dd)si + (dd)ax; CF = t_ > 0xFFFF; si = t_;
      ZF = (si == 0); SF = (si >> 15); }
    cx = 4;
    bh = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    sprrow_half_rows();
}
void sprrow_mcga_b_e1204d_c(void) {
    sprrow_half_rows();
}
void sprrow_mcga_c_c(void) {             /* sub_1219F: right half tile  */
    cx = seg_flip;
    es = cx;
    cl = 5;
    if (cl) {
        CF = (((dd)ax << cl) >> 16) & 1; ax <<= cl;
        ZF = (ax == 0); SF = (ax >> 15);
    }
    { dd t_ = (dd)si + (dd)ax; CF = t_ > 0xFFFF; si = t_;
      ZF = (si == 0); SF = (si >> 15); }
    { dd t_ = (dd)si + (dd)0x10; CF = t_ > 0xFFFF; si = t_;
      ZF = (si == 0); SF = (si >> 15); }
    cx = 4;
    bh = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    sprrow_half_rows();
}
void sprrow_mcga_c_e121b4_c(void) {
    sprrow_half_rows();
}

/* ---- sel-mask bit scan → record compares ---- */
static void bitrow_fill(void) {          /* rec_ptr_a[5..0] = '0'       */
    do {
        bp = rec_ptr_a;
        *(db*)raddr(ds, bp + si) = al;
        (si)--; ZF = (si == 0); SF = (si >> 15);
    } while ((short)(si) >= 0);
}
static void bitrow_scan(void) {          /* sel_mask bit loop           */
    do {
        CF = sel_mask & 1; sel_mask >>= 1;
        ZF = (sel_mask == 0); SF = (sel_mask >> 15);
        if (CF) {
            ax = bx;
            CF = (((dd)ax << 1) >> 16) & 1; ax <<= 1;
            ZF = (ax == 0); SF = (ax >> 15);
            si = ax;
            ax = *(dw*)raddr(ds, si - 0x295D);
            rec_ptr_b = ax;
            rec5_cmp();
        }
        (bx)--; ZF = (bx == 0); SF = (bx >> 15);
    } while ((short)(bx) >= 0);
}
void render_bit_row_c(void) {            /* sub_182DA: '0' row + scan   */
    si = 5;
    al = 0x30;
    bitrow_fill();
    bx = 0x0F;
    bitrow_scan();
}
void render_bit_row_e182df_c(void) {
    bitrow_fill();
    bx = 0x0F;
    bitrow_scan();
}
void render_bit_row_e182ec_c(void) {
    bitrow_scan();
}
void render_bit_row_e18302_c(void) {
    for (;;) {
        do {
            (bx)--; ZF = (bx == 0); SF = (bx >> 15);
            if ((short)(bx) < 0) return;
            CF = sel_mask & 1; sel_mask >>= 1;
            ZF = (sel_mask == 0); SF = (sel_mask >> 15);
        } while (!CF);
        ax = bx;
        CF = (((dd)ax << 1) >> 16) & 1; ax <<= 1;
        ZF = (ax == 0); SF = (ax >> 15);
        si = ax;
        ax = *(dw*)raddr(ds, si - 0x295D);
        rec_ptr_b = ax;
        rec5_cmp();
    }
}

/* ---- alert-level tile variant + status bar (bar_jt dispatch) ---- */
void bar_draw_c(void);                   /* fwd: shared tail            */
void tile_variant_sel_c(void) {          /* sub_17C27: alert→si select  */
    si = 0;
    CF = (dd)alert_aux < 0; ZF = (alert_aux == 0);
    SF = ((db)(alert_aux - 0) >> 7);
    if (alert_aux == 0) {
        si = 2;
        CF = (dd)alert_lvl < (dd)0x0A0; ZF = (alert_lvl == 0x0A0);
        SF = ((db)(alert_lvl - 0x0A0) >> 7);
        if (alert_lvl >= 0x0A0) si = 1;
    }
    bar_draw_c();
}
static void bar_draw_fill(void) {        /* e17c77: DD88/DDDD cells     */
    do {
        *(dw*)(raddr(ss, bp + 0)) = 0x0DD88;
        *(dw*)(raddr(ss, bp + 2)) = 0x0DDDD;
        { dd t_ = (dd)bp + (dd)4; CF = t_ > 0xFFFF; bp = t_;
          ZF = (bp == 0); SF = (bp >> 15); }
        (dx)--; ZF = (dx == 0); SF = (dx >> 15);
    } while (dx != 0);
}
static void bar_draw_mark(void) {        /* e17c8f: 88/al/ax cells+tail */
    do {
        *(db*)raddr(ss, bp + 0) = 0x88;
        *(db*)raddr(ss, bp + 1) = al;
        *(dw*)raddr(ss, bp + 2) = ax;
        { dd t_ = (dd)bp + (dd)4; CF = t_ > 0xFFFF; bp = t_;
          ZF = (bp == 0); SF = (bp >> 15); }
        (bx)--; ZF = (bx == 0); SF = (bx >> 15);
    } while (bx != 0);
    bp = di;
    al = *(db*)raddr(ss, bp + 0x19);
    al &= 0x0F; CF = 0; OF = 0; ZF = (al == 0); SF = (al >> 7);
    al |= 0x80; CF = 0; OF = 0; ZF = (al == 0); SF = (al >> 7);
    *(db*)raddr(ss, bp + 0x19) = al;
    al = *(db*)raddr(ss, bp + 0x1B);
    al &= 0x0F0; CF = 0; OF = 0; ZF = (al == 0); SF = (al >> 7);
    al |= 8; CF = 0; OF = 0; ZF = (al == 0); SF = (al >> 7);
    *(db*)raddr(ss, bp + 0x1B) = al;
}
void bar_draw_c(void) {                  /* sub_17C3E: bar compute+disp */
    bl = alert_lvl;
    bh = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    cl = 4;
    if (cl) {
        CF = (bx >> (cl - 1)) & 1; bx >>= cl;
        ZF = (bx == 0); SF = (bx >> 15);
    }
    (bx)++; ZF = (bx == 0); SF = (bx >> 15);
    { dd t_ = (dd)bx + (dd)6; CF = t_ > 0xFFFF; bx = t_;
      ZF = (bx == 0); SF = (bx >> 15); }
    ax = 0x16;
    { dd t_ = (dd)ax - (dd)bx; CF = (dd)ax < (dd)bx; ax = t_;
      ZF = (ax == 0); SF = (ax >> 15); }
    dx = ax;
    bp = 0x139;
    CF = (((dd)bp << 1) >> 16) & 1; bp <<= 1;
    ZF = (bp == 0); SF = (bp >> 15);
    bp = *(dw*)raddr(ss, bp + 0);
    { dd t_ = (dd)bp + (dd)4; CF = t_ > 0xFFFF; bp = t_;
      ZF = (bp == 0); SF = (bp >> 15); }
    di = adapter_id;
    CF = (((dd)di << 1) >> 16) & 1; di <<= 1;
    ZF = (di == 0); SF = (di >> 15);
    { vfn f_ = func_at((dd)0x1a20 + (*(dw*)(((db*)&jpt_17c64) + di)));
      if (f_) f_();
      else fprintf(stderr, "unresolved ind jmp %x\n",
                   (dd)((dd)0x1a20 + (*(dw*)(((db*)&jpt_17c64) + di))));
      return; }
}
void bar_draw_e17c77_c(void) {
    bar_draw_fill();
    CF = (dd)bx < (dd)0x16; ZF = (bx == 0x16);
    SF = ((dw)(bx - 0x16) >> 15);
    if (bx > 0x16) bx = 0x16;
    bar_draw_mark();
}
void bar_draw_e17c87_c(void) {
    CF = (dd)bx < (dd)0x16; ZF = (bx == 0x16);
    SF = ((dw)(bx - 0x16) >> 15);
    if (bx > 0x16) bx = 0x16;
    bar_draw_mark();
}
void bar_draw_e17c8f_c(void) {
    bar_draw_mark();
}

/* ---- cell-strip renderer: 8 cells via cellgfx_jt (tandy planar /
 * mcga LUT), then a 32-cell strip (map bytes or flat fill) and the
 * 4-cell tail — the scrolling edge column painter. ------------------ */
static void strip_tail4(void) {          /* e1b06d: 4x glyph fetch     */
    ax = maprow_base; cell_glyph_fetch();
    ax = maprow_base; cell_glyph_fetch();
    ax = maprow_base; cell_glyph_fetch();
    ax = maprow_base; cell_glyph_fetch();
}
static void strip_fill32(void) {         /* e1b047: same cell x32      */
    do {
        push(cx);
        ax = maprow_base;
        cell_glyph_fetch();
        cx = pop();
    } while (--cx != 0);
    strip_tail4();
}
static void strip_map32(void) {          /* e1b05b: map bytes |0x80    */
    do {
        push(cx);
        push(bx);
        al = *(db*)raddr(ds, bx - 0x68CA);
        ah = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
        al |= 0x80; CF = 0; OF = 0; ZF = (al == 0); SF = (al >> 7);
        cell_glyph_fetch();
        bx = pop();
        (bx)++; ZF = (bx == 0); SF = (bx >> 15);
        cx = pop();
    } while (--cx != 0);
    strip_tail4();
}
void draw_cell_strip_c(void) {           /* sub_1B002: edge strip      */
    ax = seg_draw;
    es = ax;
    bp = draw_cel_dbdd;
    bx = scroll_cnt;
    bx &= 7; CF = 0; OF = 0; ZF = (bx == 0); SF = (bx >> 15);
    CF = (((dd)bx << 1) >> 16) & 1; bx <<= 1;
    ZF = (bx == 0); SF = (bx >> 15);
    ax = 0x7F;
    { dd t_ = (dd)ax + (dd)*(dw*)raddr(ds, bx - 0x24F3); CF = t_ > 0xFFFF;
      ax = t_; ZF = (ax == 0); SF = (ax >> 15); }
    maprow_base = ax;
    ax = maprow_base; cell_glyph_fetch();
    ax = maprow_base; cell_glyph_fetch();
    ax = maprow_base; cell_glyph_fetch();
    ax = maprow_base; cell_glyph_fetch();
    bx = scroll_cnt;
    CF = bx & 1; bx = (short)bx >> 1;
    ZF = (bx == 0); SF = (bx >> 15);
    if ((short)(bx) >= 0) {
        CF = (dd)bx < (dd)0x40; ZF = (bx == 0x40);
        SF = ((dw)(bx - 0x40) >> 15);
        if (bx < 0x40) { draw_cell_strip_e1b054_c(); return; }
    }
    draw_cell_strip_e1b044_c();
}
void draw_cell_strip_e1b044_c(void) {
    cx = 0x20;
    strip_fill32();
}
void draw_cell_strip_e1b047_c(void) {
    strip_fill32();
}
void draw_cell_strip_e1b054_c(void) {
    cl = 5;
    if (cl) {
        CF = (((dd)bx << cl) >> 16) & 1; bx <<= cl;
        ZF = (bx == 0); SF = (bx >> 15);
    }
    cx = 0x20;
    strip_map32();
}
void draw_cell_strip_e1b05b_c(void) {
    strip_map32();
}
void draw_cell_strip_e1b06d_c(void) {
    strip_tail4();
}
void cell_glyph_fetch_c(void) {          /* sub_1B090: cellgfx_jt disp */
    di = bp;
    si = adapter_id;
    CF = (((dd)si << 1) >> 16) & 1; si <<= 1;
    ZF = (si == 0); SF = (si >> 15);
    { vfn f_ = func_at((dd)0x1a20 + (*(dw*)(((db*)&cellgfx_jt) + si)));
      if (f_) f_();
      else fprintf(stderr, "unresolved ind jmp %x\n",
                   (dd)((dd)0x1a20 + (*(dw*)(((db*)&cellgfx_jt) + si))));
      return; }
}
void cell_glyph_fetch_e1b0b1_c(void) {   /* tandy 4-plane glyph fetch  */
    ax = *(dw*)raddr(ds, si);
    ax = *(dw*)raddr_(ds, si); si += DF ? -2 : 2;
    *(dw*)(raddr(es, di)) = ax;
    ax = *(dw*)raddr_(ds, si); si += DF ? -2 : 2;
    *(dw*)(raddr(es, di + 2)) = ax;
    ax = *(dw*)raddr_(ds, si); si += DF ? -2 : 2;
    *(dw*)(raddr(es, di + 0x2000)) = ax;
    ax = *(dw*)raddr_(ds, si); si += DF ? -2 : 2;
    *(dw*)(raddr(es, di + 0x2002)) = ax;
    ax = *(dw*)raddr_(ds, si); si += DF ? -2 : 2;
    *(dw*)(raddr(es, di + 0x4000)) = ax;
    ax = *(dw*)raddr_(ds, si); si += DF ? -2 : 2;
    *(dw*)(raddr(es, di + 0x4002)) = ax;
    ax = *(dw*)raddr_(ds, si); si += DF ? -2 : 2;
    *(dw*)(raddr(es, di + 0x6000)) = ax;
    ax = *(dw*)raddr_(ds, si); si += DF ? -2 : 2;
    *(dw*)(raddr(es, di + 0x6002)) = ax;
    { dd t_ = (dd)bp + (dd)4; CF = t_ > 0xFFFF; bp = t_;
      ZF = (bp == 0); SF = (bp >> 15); }
}
static void cellgfx_rows(void) {         /* e1b177: 4-row LUT blit     */
    do {
        bl = *(db*)raddr(ds, si);
        al = *(db*)raddr(ds, bx + 0x3C70);
        ah = *(db*)raddr(ds, bx + 0x3D70);
        *(dw*)(raddr(es, di)) = ax;
        bl = *(db*)raddr(ds, si + 1);
        al = *(db*)raddr(ds, bx + 0x3C70);
        ah = *(db*)raddr(ds, bx + 0x3D70);
        *(dw*)(raddr(es, di + 2)) = ax;
        bl = *(db*)raddr(ds, si + 2);
        al = *(db*)raddr(ds, bx + 0x3C70);
        ah = *(db*)raddr(ds, bx + 0x3D70);
        *(dw*)(raddr(es, di + 4)) = ax;
        bl = *(db*)raddr(ds, si + 3);
        al = *(db*)raddr(ds, bx + 0x3C70);
        ah = *(db*)raddr(ds, bx + 0x3D70);
        *(dw*)(raddr(es, di + 6)) = ax;
        { dd t_ = (dd)si + (dd)4; CF = t_ > 0xFFFF; si = t_;
          ZF = (si == 0); SF = (si >> 15); }
        { dd t_ = (dd)di + (dd)0x140; CF = t_ > 0xFFFF; di = t_;
          ZF = (di == 0); SF = (di >> 15); }
    } while (--cx != 0);
    { dd t_ = (dd)bp + (dd)8; CF = t_ > 0xFFFF; bp = t_;
      ZF = (bp == 0); SF = (bp >> 15); }
}
void cellgfx_mcga_c(void) {              /* jpt_1B098 case 3           */
    cl = 5;
    if (cl) {
        CF = (((dd)ax << cl) >> 16) & 1; ax <<= cl;
        ZF = (ax == 0); SF = (ax >> 15);
    }
    { dd t_ = (dd)ax + (dd)0x1B52; CF = t_ > 0xFFFF; ax = t_;
      ZF = (ax == 0); SF = (ax >> 15); }
    si = ax;
    CF = 0; OF = 0; ZF = ((dw)(scroll_cnt & 1) == 0);
    SF = ((dw)(scroll_cnt & 1) >> 15);
    if (scroll_cnt & 1) {
        dd t_ = (dd)si + (dd)0x10; CF = t_ > 0xFFFF; si = t_;
        ZF = (si == 0); SF = (si >> 15);
    }
    cellgfx_mcga_e1b172_c();
}
void cellgfx_mcga_e1b172_c(void) {
    cx = 4;
    bh = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    cellgfx_rows();
}
void cellgfx_mcga_e1b177_c(void) {
    cellgfx_rows();
}

/* ---- camera pan: target snap, pan-flag detect, view drift/input --- */
void scroll_nop_c(void) {                /* loc_16A15: store pan flags*/
    ax = si;
    pan_flags = al;
    al |= al; CF = 0; OF = 0; ZF = (al == 0); SF = (al >> 7);
}
static void pan_y_cmp(void) {            /* e169e5: y-axis compare     */
    ax = cam_tgt_y;
    CF = ax & 1; ax = (short)ax >> 1;
    ZF = (ax == 0); SF = (ax >> 15);
    CF = ax & 1; ax = (short)ax >> 1;
    ZF = (ax == 0); SF = (ax >> 15);
    dw oldy = cam_px_y;
    CF = (dd)ax < (dd)oldy; ZF = (ax == oldy);
    SF = ((dw)(ax - oldy) >> 15);
    cam_px_y = ax;
    if (ax != oldy) {
        if ((short)ax >= (short)oldy) {
            si |= 2; CF = 0; OF = 0; ZF = (si == 0); SF = (si >> 15);
            CF = 0; OF = 0;
            ZF = ((*(db*)(&cam_tgt_y) & 4) == 0);
            SF = ((*(db*)(&cam_tgt_y) & 4) >> 7);
            if ((*(db*)(&cam_tgt_y) & 4) != 0) { scroll_nop_c(); return; }
            (cam_9681)++; ZF = (cam_9681 == 0); SF = (cam_9681 >> 15);
            scroll_nop_c(); return;
        }
        cam_pan_detect_e16a07_c();
        return;
    }
    scroll_nop_c();
}
void cam_pan_detect_c(void) {            /* sub_169B6: pan flag build  */
    cam_tgt_x &= 0x0FFFC; CF = 0; OF = 0;
    ZF = (cam_tgt_x == 0); SF = (cam_tgt_x >> 15);
    cam_tgt_y &= 0x0FFFC; CF = 0; OF = 0;
    ZF = (cam_tgt_y == 0); SF = (cam_tgt_y >> 15);
    si = 0;
    ax = cam_tgt_x;
    CF = ax & 1; ax = (short)ax >> 1;
    ZF = (ax == 0); SF = (ax >> 15);
    CF = ax & 1; ax = (short)ax >> 1;
    ZF = (ax == 0); SF = (ax >> 15);
    dw oldx = cam_px_x;
    CF = (dd)ax < (dd)oldx; ZF = (ax == oldx);
    SF = ((dw)(ax - oldx) >> 15);
    cam_px_x = ax;
    if (ax != oldx) {
        if ((short)ax >= (short)oldx) {
            si = 8;
            (cam_org_x)++; ZF = (cam_org_x == 0); SF = (cam_org_x >> 15);
        } else {
            cam_pan_detect_e169de_c();
            return;
        }
    }
    pan_y_cmp();
}
void cam_pan_detect_e169de_c(void) {     /* x pan left: org--, si=4    */
    (cam_org_x)--; ZF = (cam_org_x == 0); SF = (cam_org_x >> 15);
    si = 4;
    pan_y_cmp();
}
void cam_pan_detect_e169e5_c(void) {
    pan_y_cmp();
}
void cam_pan_detect_e16a07_c(void) {     /* y pan up: si|=1, subtile   */
    si |= 1; CF = 0; OF = 0; ZF = (si == 0); SF = (si >> 15);
    CF = 0; OF = 0;
    ZF = ((*(db*)(&cam_tgt_y) & 4) == 0);
    SF = ((*(db*)(&cam_tgt_y) & 4) >> 7);
    if ((*(db*)(&cam_tgt_y) & 4) != 0) {
        (cam_9681)--; ZF = (cam_9681 == 0); SF = (cam_9681 >> 15);
    }
    scroll_nop_c();
}
static void clamp_y_tail(void) {         /* e189aa: y window clamp     */
    al = *(db*)raddr(ds, bx - 0x4063);
    ah = *(db*)raddr(ds, bx - 0x401F);
    { dd t_ = (dd)ax - (dd)cam_tgt_y; CF = (dd)ax < (dd)cam_tgt_y;
      ax = t_; ZF = (ax == 0); SF = (ax >> 15); }
    rec_ptr_a = ax;
    CF = (dd)ax < (dd)0x58; ZF = (ax == 0x58);
    SF = ((dw)(ax - 0x58) >> 15);
    if ((short)ax <= (short)0x58) {
        al = *(db*)raddr(ds, bx - 0x4063);
        ah = *(db*)raddr(ds, bx - 0x401F);
        { dd t_ = (dd)ax - (dd)0x58; CF = (dd)ax < (dd)0x58; ax = t_;
          ZF = (ax == 0); SF = (ax >> 15); }
        cam_tgt_y = ax;
        return;
    }
    CF = (dd)ax < (dd)0x78; ZF = (ax == 0x78);
    SF = ((dw)(ax - 0x78) >> 15);
    if ((short)ax < (short)0x78) return;
    al = *(db*)raddr(ds, bx - 0x4063);
    ah = *(db*)raddr(ds, bx - 0x401F);
    { dd t_ = (dd)ax - (dd)0x78; CF = (dd)ax < (dd)0x78; ax = t_;
      ZF = (ax == 0); SF = (ax >> 15); }
    cam_tgt_y = ax;
}
static void clamp_x_fix(void) {          /* e18996: x adjust + y tail  */
    al = *(db*)raddr(ds, bx - 0x4085);
    { dd t_ = (dd)al - (dd)*(db*)(&rec_ptr_a);
      CF = (dd)al < (dd)*(db*)(&rec_ptr_a); al = t_;
      ZF = (al == 0); SF = (al >> 7); }
    *(db*)(&cam_tgt_x) = al;
    al = *(db*)raddr(ds, bx - 0x4041);
    { dd t_ = (dd)al - (dd)0 - CF; CF = (dd)al < (dd)0 + CF; al = t_;
      ZF = (al == 0); SF = (al >> 7); }
    *(db*)(((db*)&cam_tgt_x) + 1) = al;
    clamp_y_tail();
}
void clamp_obj_pos_c(void) {             /* sub_18968: cam window      */
    al = *(db*)raddr(ds, bx - 0x4085);
    { dd t_ = (dd)al - (dd)*(db*)(&cam_tgt_x);
      CF = (dd)al < (dd)*(db*)(&cam_tgt_x); al = t_;
      ZF = (al == 0); SF = (al >> 7); }
    *(db*)(&rec_ptr_a) = al;
    al = *(db*)raddr(ds, bx - 0x4041);
    { dd t_ = (dd)al - (dd)*(db*)(((db*)&cam_tgt_x) + 1) - CF;
      CF = (dd)al < (dd)*(db*)(((db*)&cam_tgt_x) + 1) + CF; al = t_;
      ZF = (al == 0); SF = (al >> 7); }
    if ((signed char)(al) < 0) {
        al = 0x40; *(db*)(&rec_ptr_a) = al;
    } else if (al != 0) {
        al = 0x60; *(db*)(&rec_ptr_a) = al;
    } else {
        al = *(db*)(&rec_ptr_a);
        CF = (dd)al < (dd)0x40; ZF = (al == 0x40);
        SF = ((db)(al - 0x40) >> 7);
        if (al < 0x40) {
            al = 0x40; *(db*)(&rec_ptr_a) = al;
        } else {
            CF = (dd)al < (dd)0x60; ZF = (al == 0x60);
            SF = ((db)(al - 0x60) >> 7);
            if (al < 0x60) { clamp_y_tail(); return; }
            al = 0x60; *(db*)(&rec_ptr_a) = al;
        }
    }
    clamp_x_fix();
}
void clamp_obj_pos_e1898a_c(void) {
    al = 0x60; *(db*)(&rec_ptr_a) = al;
    clamp_x_fix();
}
void clamp_obj_pos_e18991_c(void) {
    al = 0x40; *(db*)(&rec_ptr_a) = al;
    clamp_x_fix();
}
void clamp_obj_pos_e18996_c(void) {
    clamp_x_fix();
}
void clamp_obj_pos_e189aa_c(void) {
    clamp_y_tail();
}
void clamp_obj_pos_e189ce_c(void) {      /* y over-window fix          */
    CF = (dd)ax < (dd)0x78; ZF = (ax == 0x78);
    SF = ((dw)(ax - 0x78) >> 15);
    if ((short)ax < (short)0x78) return;
    al = *(db*)raddr(ds, bx - 0x4063);
    ah = *(db*)raddr(ds, bx - 0x401F);
    { dd t_ = (dd)ax - (dd)0x78; CF = (dd)ax < (dd)0x78; ax = t_;
      ZF = (ax == 0); SF = (ax >> 15); }
    cam_tgt_y = ax;
}
void snap_to_cell_c(void) {              /* sub_16A26: grid snap       */
    { dd t_ = (dd)bx - (dd)bx; CF = (dd)bx < (dd)bx; bx = t_;
      ZF = (bx == 0); SF = (bx >> 15); }
    clamp_obj_pos();
    ax = cam_tgt_x;
    ax &= 0x0FFFC; CF = 0; OF = 0; ZF = (ax == 0); SF = (ax >> 15);
    cam_tgt_x = ax;
    cam_9676 = ax;
    cl = 2;
    if (cl) {
        CF = (ax >> (cl - 1)) & 1; ax = (short)ax >> cl;
        ZF = (ax == 0); SF = (ax >> 15);
    }
    cam_org_x = ax;
    cam_px_x = ax;
    ax = cam_tgt_y;
    ax &= 0x0FFFC; CF = 0; OF = 0; ZF = (ax == 0); SF = (ax >> 15);
    cam_tgt_y = ax;
    cam_9678 = ax;
    CF = ax & 1; ax = (short)ax >> 1; ZF = (ax == 0); SF = (ax >> 15);
    CF = ax & 1; ax = (short)ax >> 1; ZF = (ax == 0); SF = (ax >> 15);
    cam_px_y = ax;
    CF = ax & 1; ax = (short)ax >> 1; ZF = (ax == 0); SF = (ax >> 15);
    cam_9681 = ax;
}
void cam_pan_apply_c(void) {             /* sub_16B1E: snap+set+clear  */
    push(bx);
    push(si);
    snap_to_cell();
    sprite_param_set();
    pan_flags = 0;
    si = pop();
    bx = pop();
}
void obj_lookup_word_c(void) {           /* sub_1AF63: word tbl store  */
    ah = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    CF = (((dd)ax << 1) >> 16) & 1; ax <<= 1;
    ZF = (ax == 0); SF = (ax >> 15);
    si = ax;
    ax = *(dw*)raddr(ds, si - 0x2468);
    *(db*)raddr(ds, bx + 0x0E3F) = al;
    *(db*)raddr(ds, bx + 0x0E5F) = ah;
}

/* ---- view pan step: parity delta shift + input/drift pan ---------- */
static void pan_shift_odd(void) {        /* e1aca5: -246E->-2471 x3    */
    do {
        al = *(db*)raddr(ds, si - 0x246E);
        *(db*)raddr(ds, si - 0x2471) = al;
        (si)--; ZF = (si == 0); SF = (si >> 15);
    } while ((short)(si) >= 0);
}
static void pan_shift_even(void) {       /* e1acb2: -246B->-2471 x3    */
    do {
        al = *(db*)raddr(ds, si - 0x246B);
        *(db*)raddr(ds, si - 0x2471) = al;
        (si)--; ZF = (si == 0); SF = (si >> 15);
    } while ((short)(si) >= 0);
}
static void pan_finish(void) {           /* e1ad0d                     */
    bx = 0;
    al = 0;
    obj_lookup_word();
}
static void pan_clamp_store(void) {      /* e1acdb..ecec               */
    CF = (dd)bx < (dd)8; ZF = (bx == 8);
    SF = ((dw)(bx - 8) >> 15);
    if (bx < 8) bx = 8;
    CF = (dd)bx < (dd)0x118; ZF = (bx == 0x118);
    SF = ((dw)(bx - 0x118) >> 15);
    if (bx >= 0x118) bx = 0x118;
    *(db*)(&dpar_2) = bl;
    *(db*)(&dpar_3) = bh;
    *(db*)(&dpar_4) = 0x46;
    pan_finish();
}
static void pan_drift_step(void) {       /* e1acfb: drift advance      */
    bl = *(db*)(&dpar_4);
    { dd t_ = (dd)bl + (dd)4; CF = t_ > 0xFF; bl = t_;
      ZF = (bl == 0); SF = (bl >> 7); }
    CF = (dd)bl < (dd)0x0C8; ZF = (bl == 0x0C8);
    SF = ((db)(bl - 0x0C8) >> 7);
    if (bl >= 0x0C8) bl = 0x0C8;
    *(db*)(&dpar_4) = bl;
    pan_finish();
}
static void pan_input(void) {            /* e1acbd: input-mask pan     */
    al = drift_flag;
    al |= al; CF = 0; OF = 0; ZF = (al == 0); SF = (al >> 7);
    if (al != 0) { pan_drift_step(); return; }
    al = *(db*)(&input_mask);
    bl = *(db*)(&dpar_2);
    bh = *(db*)(&dpar_3);
    CF = 0; OF = 0; ZF = ((al & 8) == 0); SF = ((al & 8) >> 7);
    if ((al & 8) == 0) {
        (bx)++; ZF = (bx == 0); SF = (bx >> 15);
        pan_clamp_store(); return;
    }
    CF = 0; OF = 0; ZF = ((al & 4) == 0); SF = ((al & 4) >> 7);
    if ((al & 4) == 0) {
        (bx)--; ZF = (bx == 0); SF = (bx >> 15);
    }
    pan_clamp_store();
}
void view_pan_step_c(void) {             /* sub_1AC97: parity+pan      */
    (view_pan_a9d8)++; ZF = (view_pan_a9d8 == 0);
    SF = (view_pan_a9d8 >> 7);
    si = 2;
    al = view_pan_a9d8;
    al &= 1; CF = 0; OF = 0; ZF = (al == 0); SF = (al >> 7);
    if (al == 0) pan_shift_even();
    else pan_shift_odd();
    pan_input();
}
void view_pan_step_e1aca5_c(void) {
    pan_shift_odd();
    pan_input();
}
void view_pan_step_e1acb2_c(void) {
    pan_shift_even();
    pan_input();
}
void view_pan_step_e1acbd_c(void) {
    pan_input();
}
void view_pan_step_e1acd6_c(void) {
    CF = 0; OF = 0; ZF = ((al & 4) == 0); SF = ((al & 4) >> 7);
    if ((al & 4) == 0) {
        (bx)--; ZF = (bx == 0); SF = (bx >> 15);
    }
    pan_clamp_store();
}
void view_pan_step_e1acdb_c(void) {
    pan_clamp_store();
}
void view_pan_step_e1ace3_c(void) {
    CF = (dd)bx < (dd)0x118; ZF = (bx == 0x118);
    SF = ((dw)(bx - 0x118) >> 15);
    if (bx >= 0x118) bx = 0x118;
    *(db*)(&dpar_2) = bl;
    *(db*)(&dpar_3) = bh;
    *(db*)(&dpar_4) = 0x46;
    pan_finish();
}
void view_pan_step_e1acec_c(void) {
    *(db*)(&dpar_2) = bl;
    *(db*)(&dpar_3) = bh;
    *(db*)(&dpar_4) = 0x46;
    pan_finish();
}
void view_pan_step_e1acfb_c(void) {
    pan_drift_step();
}
void view_pan_step_e1ad09_c(void) {
    *(db*)(&dpar_4) = bl;
    pan_finish();
}
void view_pan_step_e1ad0d_c(void) {
    pan_finish();
}
