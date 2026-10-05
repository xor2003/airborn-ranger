/* Airborne Ranger C port — runtime interface.
 * Real C reimplementation of the original routines; the x86 register file is
 * kept as globals because the ported routines genuinely shared registers.
 * Memory: mem[] flat 1MB; image loaded at paragraph 0x1a2 (mem[0x1a20]).
 * Data symbols alias mem[] via data_syms.h.
 */
#pragma once
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#if !defined(_WIN32) && !defined(__ANDROID__)
#include <execinfo.h>
#endif

typedef uint8_t db; typedef uint16_t dw; typedef uint32_t dd;

extern dd eax,ebx,ecx,edx,esi,edi,esp,ebp;
extern dw cs,ds,es,fs,gs,ss,ip;
extern int CF,ZF,SF,OF,PF,AF,DF,IF,TF;

/* 8/16-bit views into the 32-bit registers (x86 partial-register aliasing,
 * same trick as masm2c asm_regs_m2c.h REGDEF_hl) */
#define ax (*(dw*)&eax)
#define al (*(db*)&eax)
#define ah (*(((db*)&eax)+1))
#define bx (*(dw*)&ebx)
#define bl (*(db*)&ebx)
#define bh (*(((db*)&ebx)+1))
#define cx (*(dw*)&ecx)
#define cl (*(db*)&ecx)
#define ch (*(((db*)&ecx)+1))
#define dx (*(dw*)&edx)
#define dl (*(db*)&edx)
#define dh (*(((db*)&edx)+1))
#define si (*(dw*)&esi)
#define di (*(dw*)&edi)
#define sp (*(dw*)&esp)
#define bp (*(dw*)&ebp)

extern db mem[1<<20];

__attribute__((always_inline)) static inline db *raddr_(int seg,int off){return &mem[((seg&0xffff)<<4)+(off&0xffff)];}
__attribute__((always_inline)) static inline db *raddr(int seg,int off){return raddr_(seg,off);}

void push(dw); dw pop(void); void pushf(void); void popf(void);
dw rol16(dw,int); dw ror16(dw,int); dw rcl16(dw,int); dw rcr16(dw,int);
dw in(dw); void out(dw,dw); void swi(int);
void indirect_jump(void); void __dispatch_call_ext(void);

/* DOS int21h services */
void dos_int21(dw);
void dos_exit(void);void dos_read_char_echo(void);void dos_write_char(void);void dos_direct_io(void);
void dos_read_char_noecho(void);void dos_print_string(void);void dos_read_string(void);
void dos_check_stdin(void);void dos_get_drive(void);void dos_set_dta(void);void dos_set_int_vector(void);
void dos_get_date(void);void dos_get_time(void);void dos_get_version(void);void dos_get_int_vector(void);
void dos_open(void);void dos_close(void);void dos_read(void);void dos_write(void);void dos_delete(void);
void dos_seek(void);void dos_get_attr(void);void dos_ioctl(void);void dos_alloc(void);void dos_free(void);
void dos_resize(void);void dos_exec(void);void dos_exit_code(void);void dos_find_first(void);
void dos_find_next(void);void dos_get_verify(void);void dos_rename(void);void dos_file_datetime(void);
void dos_create_temp(void);void dos_create_new(void);void dos_get_psp(void);
/* BIOS */
void bios_video(dw);
void bios_set_mode(void);void bios_set_cursor(void);void bios_scroll(void);void bios_putc(void);
void bios_palette(void);void bios_getch(void);void bios_kbhit(void);void bios_kbd(dw);void bios_time(void);

/* runtime internals */
dw rt_in(dw p); void rt_out(dw p,dw v);
void rt_frame(void);            /* stage a VGA frame for the main loop */
void rt_exit(int code);
void video_init(void);
void video_run_loop(void);      /* main thread: SDL event pump + present */
void rt_set_cpu_thread(void);   /* worker thread: register as IRQ0 park target */
void rt_pump_events(void);
void rt_script_feed(void);          /* scripted-key step — mem-only, timer-safe */
dw rt_kbd_port60(void);
void rt_test_kpush(dw v);           /* test hook: queue one BIOS-buffer key */
/* test hook: snapshot/restore host-side DOS state (allocator + overlay map) */
struct dos_snap { dd alloc_next, tnd_base, tnd_cbase; dw tnd_cseg;
                  int nablk, nfblk; dd ablk[64][2], fblk[64][2]; };
void dos_snap_get(struct dos_snap *);
void dos_snap_set(const struct dos_snap *);
/* key-mapping menu overlay (input.c); video.c draws it */
void rt_kmap_frame(void);
void rt_kmap_exclusive(void);
int rt_kmap_open(void);
int rt_kmap_rows(void);
int rt_kmap_cur(void);
int rt_kmap_capture(void);
const char *rt_kmap_name(int r);
const char *rt_kmap_bind(int r);
void rt_tnd_write(dw p, dw v);
void rt_speaker(dw v);
void rt_call_vector(int n);
void rt_timer_tick(void);
void rt_bios_int8(void);
extern volatile int rt_isr_ctx;
/* TICKSTAT counters: BIOS int8 dispatches / int1c ticks (M2C_TICKSTAT=1) */
extern dd rt_i8_cnt, rt_1c_cnt;
/* TANDYSND.EXE overlay: image base mem addr / code-seg mem base / code seg para.
 * Set by dos_exec when the overlay loads (0x30000 / 0x30070 / 0x3007). */
extern dd tnd_base, tnd_cbase; extern dw tnd_cseg;
void mem_load_image(void); void mem_apply_fixups(void);
typedef void(*vfn)(void); vfn func_at(dd addr);
/* far seg:off pair -> flat mem addr */
#define rt_far(v) (((((dd)(v)) >> 16) << 4) + (((dd)(v)) & 0xffff))
dw mem_w(dd a); db mem_b(dd a); void mem_ww(dd a, dw v); void mem_wb(dd a, db v);

/* debug hook for intro diagnostics */

/* readable rewrites (port/rewrite.c) — gen code trampolines <name>() ->
 * <name>_c(); the original stays callable as <name>_lifted for A/B tests */
void decompress_res_c(void); void decompress_res_lifted(void);
void sprtab_init_a_c(void); void sprtab_init_a_lifted(void);
void sprtab_init_b_c(void); void sprtab_init_b_lifted(void);
void load_resource_c(void); void load_resource_lifted(void);
void res_file_read_c(void); void res_file_read_lifted(void);
void res_file_read_e10846_c(void); void res_file_read_e10846_lifted(void);
void res_file_read_e10849_c(void); void res_file_read_e10849_lifted(void);
void seek_res_entry_c(void); void seek_res_entry_lifted(void);
void seek_res_entry_e108c1_c(void); void seek_res_entry_e108c1_lifted(void);
void open_res_file_c(void); void open_res_file_lifted(void);
void close_res_file_c(void); void close_res_file_lifted(void);
void res_load_fail_c(void); void res_load_fail_lifted(void);
void res_file_error_c(void); void res_file_error_lifted(void);
void res_file_error_e109ab_c(void); void res_file_error_e109ab_lifted(void);
void res_file_error_e109b7_c(void); void res_file_error_e109b7_lifted(void);
void res_file_error_e109d9_c(void); void res_file_error_e109d9_lifted(void);
void mode_rec_load_c(void); void mode_rec_load_lifted(void);
void write_res_file_c(void); void write_res_file_lifted(void);
void write_res_file_e1092d_c(void); void write_res_file_e1092d_lifted(void);
void write_res_file_e1092f_c(void); void write_res_file_e1092f_lifted(void);
void load_overlay_c(void); void load_overlay_lifted(void);
void load_overlay_e13ab6_c(void); void load_overlay_e13ab6_lifted(void);
void load_overlay_e13aca_c(void); void load_overlay_e13aca_lifted(void);
void load_overlay_e13af9_c(void); void load_overlay_e13af9_lifted(void);
void load_overlay_e13b08_c(void); void load_overlay_e13b08_lifted(void);
void load_overlay_e13b12_c(void); void load_overlay_e13b12_lifted(void);
void load_overlay_e13b46_c(void); void load_overlay_e13b46_lifted(void);
void load_overlay_e13b85_c(void); void load_overlay_e13b85_lifted(void);
void load_overlay_e13b9e_c(void); void load_overlay_e13b9e_lifted(void);
void load_overlay_e13baa_c(void); void load_overlay_e13baa_lifted(void);
void load_palette_c(void); void load_palette_lifted(void);
void load_palette_b_c(void); void load_palette_b_lifted(void);
void pal_upload_c(void); void pal_upload_lifted(void);
void load_palette_e103a5_c(void); void load_palette_e103a5_lifted(void);
void pal_upload_mcga_c(void); void pal_upload_mcga_lifted(void);
void pal_upload_mcga_e10421_c(void); void pal_upload_mcga_e10421_lifted(void);
void rec_walk_e10491_c(void); void rec_walk_e10491_lifted(void);
void glyph_conv_dispatch_c(void); void glyph_conv_dispatch_lifted(void);
void mode_call_c(void); void mode_call_lifted(void);
void render_tick_c(void); void render_tick_lifted(void);
void redraw_frame_c(void); void redraw_frame_lifted(void);
void redraw_frame_e14afe_c(void); void redraw_frame_e14afe_lifted(void);
void hitflash_dec_c(void); void hitflash_dec_lifted(void);
void hitflash_dec_e16d7e_c(void); void hitflash_dec_e16d7e_lifted(void);
void hitflash_dec_e16d84_c(void); void hitflash_dec_e16d84_lifted(void);
void find_free_slot_b_c(void); void find_free_slot_b_lifted(void);
void find_free_slot_b_e160e9_c(void); void find_free_slot_b_e160e9_lifted(void);
void find_free_slot_b_e160f9_c(void); void find_free_slot_b_e160f9_lifted(void);
void obj_rec_clear_c(void); void obj_rec_clear_lifted(void);
void obj_tick_all_c(void); void obj_tick_all_lifted(void);
void obj_alive_mark_c(void); void obj_alive_mark_lifted(void);
void obj_alive_mark_e16b8b_c(void); void obj_alive_mark_e16b8b_lifted(void);
void slot_weight_sum_c(void); void slot_weight_sum_lifted(void);
void slot_weight_sum_e16d53_c(void); void slot_weight_sum_e16d53_lifted(void);
void slot_weight_sum_e16d5d_c(void); void slot_weight_sum_e16d5d_lifted(void);
void slot_weight_sum_e16d66_c(void); void slot_weight_sum_e16d66_lifted(void);
void ai_param_fetch_c(void); void ai_param_fetch_lifted(void);
void target_pri_decay_c(void); void target_pri_decay_lifted(void);
void farcall_ptr_a9e_c(void); void farcall_ptr_a9e_lifted(void);
void compose_frame_cond_c(void); void compose_frame_cond_lifted(void);
void compose_frame_cond_e11962_c(void); void compose_frame_cond_e11962_lifted(void);
void compose_frame_e11983_c(void); void compose_frame_e11983_lifted(void);
void compose_frame_c(void); void compose_frame_lifted(void);
void present_mcga_c(void); void present_mcga_lifted(void);
void flip_frame_e11a5c_c(void); void flip_frame_e11a5c_lifted(void);
void flip_mcga_c(void); void flip_mcga_lifted(void);
void flip_frame_e11b7a_c(void); void flip_frame_e11b7a_lifted(void);
void flip_frame_c(void); void flip_frame_lifted(void);
void vblank_wait_e11b84_c(void); void vblank_wait_e11b84_lifted(void);
void vblank_wait_c(void); void vblank_wait_lifted(void);
void compose_mcga_c(void); void compose_mcga_11c0d_c(void);
void compose_mcga_lifted(void); void compose_mcga_11c0d_lifted(void);
void locret_11c27_c(void); void locret_11c27_lifted(void);
void spr_state_copy_b_c(void); void spr_state_copy_b_lifted(void);
void set_border_color_c(void); void set_border_color_lifted(void);
void clear_backbuf_c(void); void clear_backbuf_lifted(void);
void adapter_compose_flip_e11c4c_c(void); void adapter_compose_flip_e11c4c_lifted(void);
void adapter_compose_flip_e11c53_c(void); void adapter_compose_flip_e11c53_lifted(void);
void adapter_compose_flip_e11c59_c(void); void adapter_compose_flip_e11c59_lifted(void);
void adapter_compose_flip_c(void); void adapter_compose_flip_lifted(void);
void compose_flip_c(void); void compose_flip_lifted(void);
void video_bufs_init_c(void); void video_bufs_init_lifted(void);
void video_bufs_setup_c(void); void video_bufs_setup_lifted(void);
void video_bufs_setup_e126c9_c(void); void video_bufs_setup_e126c9_lifted(void);
void clip_go_mcga_c(void); void clip_go_mcga_lifted(void);
void clip_go_mcga_e13047_c(void); void clip_go_mcga_e13047_lifted(void);
void clip_go_mcga_e130b0_c(void); void clip_go_mcga_e130b0_lifted(void);
void clip_go_mcga_e130b3_c(void); void clip_go_mcga_e130b3_lifted(void);
void mcga_dirty_update_c(void); void mcga_dirty_update_lifted(void);
void mcga_dirty_update_e1393c_c(void); void mcga_dirty_update_e1393c_lifted(void);
void mcga_dirty_update_e13943_c(void); void mcga_dirty_update_e13943_lifted(void);
void mcga_dirty_update_e13975_c(void); void mcga_dirty_update_e13975_lifted(void);
void hud_weapon_update_c(void); void hud_weapon_update_lifted(void);
void hud_weapon_update_e17b22_c(void); void hud_weapon_update_e17b22_lifted(void);
void hud_weapon_update_e17b38_c(void); void hud_weapon_update_e17b38_lifted(void);
void hud_weapon_update_e17b4a_c(void); void hud_weapon_update_e17b4a_lifted(void);
void hud_weapon_update_e17bc5_c(void); void hud_weapon_update_e17bc5_lifted(void);
void fx_overlay_fill_c(void); void fx_overlay_fill_lifted(void);
void fx_overlay_fill_e17cfd_c(void); void fx_overlay_fill_e17cfd_lifted(void);
void fx_overlay_fill_e17d0e_c(void); void fx_overlay_fill_e17d0e_lifted(void);
void fx_overlay_fill_e17d32_c(void); void fx_overlay_fill_e17d32_lifted(void);
void tile_blit_flipbuf_c(void); void tile_blit_flipbuf_lifted(void);
void tile_blit_mcga_c(void); void tile_blit_mcga_lifted(void);
void tile_row_mcga_c(void); void tile_row_mcga_lifted(void);
void tile_row_mcga_e118c6_c(void); void tile_row_mcga_e118c6_lifted(void);
void glyph_blit_mcga_c(void); void glyph_blit_mcga_lifted(void);
void glyph_blit_mcga_e10d9e_c(void); void glyph_blit_mcga_e10d9e_lifted(void);
void glyph_put_mcga_10c1c_c(void); void glyph_put_mcga_10c1c_lifted(void);
void glyph_put2_mcga_c(void); void glyph_put2_mcga_lifted(void);
void mapcols_draw_c(void); void mapcols_draw_lifted(void);
void mapcols_draw_e1bcdc_c(void); void mapcols_draw_e1bcdc_lifted(void);
void mapcols_draw_e1bcdf_c(void); void mapcols_draw_e1bcdf_lifted(void);
void mapcols_draw_e1bced_c(void); void mapcols_draw_e1bced_lifted(void);
void mapcols_draw_e1bcf4_c(void); void mapcols_draw_e1bcf4_lifted(void);
void blitflag_go_mcga_c(void); void blitflag_go_mcga_lifted(void);
void blitflag_go_mcga_e1384d_c(void); void blitflag_go_mcga_e1384d_lifted(void);
void blitflag_go_mcga_e1385c_c(void); void blitflag_go_mcga_e1385c_lifted(void);
void blitflag_go_mcga_e13884_c(void); void blitflag_go_mcga_e13884_lifted(void);
void blitflag_go_mcga_e138a5_c(void); void blitflag_go_mcga_e138a5_lifted(void);
void scroll_go_a_c(void); void scroll_go_a_lifted(void);
void scroll_go_b_c(void); void scroll_go_b_lifted(void);
void scroll_go_c_c(void); void scroll_go_c_lifted(void);
void scroll_go_d_c(void); void scroll_go_d_lifted(void);
void scroll_go_e_c(void); void scroll_go_e_lifted(void);
void scroll_go_f_c(void); void scroll_go_f_lifted(void);
void scroll_go_g_c(void); void scroll_go_g_lifted(void);
void scroll_go_h_c(void); void scroll_go_h_lifted(void);
void scroll_edge_a_c(void); void scroll_edge_a_lifted(void);
void scroll_edge_a_e1429f_c(void); void scroll_edge_a_e1429f_lifted(void);
void scroll_edge_a_e142ac_c(void); void scroll_edge_a_e142ac_lifted(void);
void scroll_edge_d_c(void); void scroll_edge_d_lifted(void);
void scroll_edge_d_e142ed_c(void); void scroll_edge_d_e142ed_lifted(void);
void scroll_edge_d_e142f6_c(void); void scroll_edge_d_e142f6_lifted(void);
void scroll_edge_b_c(void); void scroll_edge_b_lifted(void);
void scroll_edge_b_e143cd_c(void); void scroll_edge_b_e143cd_lifted(void);
void scroll_edge_b_e143e1_c(void); void scroll_edge_b_e143e1_lifted(void);
void scroll_edge_b_e14430_c(void); void scroll_edge_b_e14430_lifted(void);
void scroll_edge_c_c(void); void scroll_edge_c_lifted(void);
void scroll_edge_c_e14345_c(void); void scroll_edge_c_e14345_lifted(void);
void scroll_edge_c_e14359_c(void); void scroll_edge_c_e14359_lifted(void);
void scroll_edge_c_e143a9_c(void); void scroll_edge_c_e143a9_lifted(void);
void px_to_cell_c(void); void px_to_cell_lifted(void);
void map_probe_xy_c(void); void map_probe_xy_lifted(void);
void map_probe_xy_e1b29d_c(void); void map_probe_xy_e1b29d_lifted(void);
void map_probe_xy_e1b2b2_c(void); void map_probe_xy_e1b2b2_lifted(void);
void map_rowhdr_get_c(void); void map_rowhdr_get_lifted(void);
void draw_tile_compose_c(void); void draw_tile_compose_lifted(void);
void cell_tile_compose_c(void); void cell_tile_compose_lifted(void);
void bar_tandy_c(void); void bar_tandy_lifted(void);
void blit_tile_mcga_c(void); void blit_tile_mcga_lifted(void);
void blit_tile_mcga_e11ec5_c(void); void blit_tile_mcga_e11ec5_lifted(void);
void text_cursor_next_c(void); void text_cursor_next_lifted(void);

/* env-gated trace (M2C_TRACE=1) */
extern int rt_trace;

void rt_tracef(const char *tag);

#define swap(a,b) do{__typeof__(a)_t=(a);(a)=(b);(b)=_t;}while(0)
void tile_lookup_c(void); void tile_lookup_lifted(void);
void obj_cell_tile_c(void); void obj_cell_tile_lifted(void);
void obj_cell_tile_e1b34f_c(void); void obj_cell_tile_e1b34f_lifted(void);
void tilemap_init_c(void); void tilemap_init_lifted(void);
void build_mcga_lut_c(void); void build_mcga_lut_lifted(void);
void build_mcga_lut_e14b1a_c(void); void build_mcga_lut_e14b1a_lifted(void);
void blit_dst_patch_c(void); void blit_dst_patch_lifted(void);
void tile_draw_3f22_c(void); void tile_draw_3f22_lifted(void);
void tile_blit_c(void); void tile_blit_lifted(void);
void rdr_copy_mcga_c(void); void rdr_copy_mcga_lifted(void);
void sprrow_mcga_b_c(void); void sprrow_mcga_b_lifted(void);
void sprrow_mcga_b_e1204d_c(void); void sprrow_mcga_b_e1204d_lifted(void);
void sprrow_mcga_c_c(void); void sprrow_mcga_c_lifted(void);
void sprrow_mcga_c_e121b4_c(void); void sprrow_mcga_c_e121b4_lifted(void);
void render_bit_row_c(void); void render_bit_row_lifted(void);
void render_bit_row_e182df_c(void); void render_bit_row_e182df_lifted(void);
void render_bit_row_e182ec_c(void); void render_bit_row_e182ec_lifted(void);
void render_bit_row_e18302_c(void); void render_bit_row_e18302_lifted(void);
void tile_variant_sel_c(void); void tile_variant_sel_lifted(void);
void bar_draw_c(void); void bar_draw_lifted(void);
void bar_draw_e17c77_c(void); void bar_draw_e17c77_lifted(void);
void bar_draw_e17c87_c(void); void bar_draw_e17c87_lifted(void);
void bar_draw_e17c8f_c(void); void bar_draw_e17c8f_lifted(void);
