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
