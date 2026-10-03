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
/* on-screen directional keyboard overlay (input.c); video.c draws it */
int rt_vkb_open(void);
void rt_vkb_geom(int *rows, int *cols, int *cur_r, int *cur_c);
const char *rt_vkb_label(int r, int c);
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


/* env-gated trace (M2C_TRACE=1) */
extern int rt_trace;

void rt_tracef(const char *tag);

#define swap(a,b) do{__typeof__(a)_t=(a);(a)=(b);(b)=_t;}while(0)
