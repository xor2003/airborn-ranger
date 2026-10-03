/* unit tests for video.c: PIT latch->period, DAC write sequencing, text
 * output, palette functions. includes video.c to reach statics; SDL funcs
 * that need init are avoided (video_init/rt_present not exercised here). */
#include <SDL2/SDL.h>
#include <pthread.h>
#include <signal.h>
#include "../rt.h"
#include <stdio.h>

/* stubs for externs video.c references */
void rt_pump_events(void){}
void rt_script_feed(void){}
void rt_bios_int8(void){}
void rt_call_vector(int n){ (void)n; }
void rt_tnd_write(dw p, dw v){ (void)p; (void)v; }
void rt_speaker(dw v){ (void)v; }
dw rt_kbd_port60(void){ return 0; }
vfn func_at(dd a){ (void)a; return 0; }
int rt_kmap_open(void){ return 0; }
int rt_kmap_rows(void){ return 0; }
int rt_kmap_cur(void){ return 0; }
int rt_kmap_capture(void){ return 0; }
const char *rt_kmap_name(int r){ (void)r; return ""; }
const char *rt_kmap_bind(int r){ (void)r; return ""; }
/* rt_exit comes from rt.o */

#include "../video.c"

static int fails, checks;
#define CHECK(cond) do{ checks++; if(!(cond)){ fails++; \
    fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); } }while(0)

int main(void){
    /* --- PIT: control byte latches lo/hi; reload -> period --- */
    pit_latch = 0; pit_reload = 0; pit_period_ms = 0;
    rt_out(0x43, 0x36);                            /* ch0, lo/hi, mode 3 */
    CHECK(pit_latch == 0);
    rt_out(0x40, 0xae);                            /* low byte */
    CHECK(pit_latch == 1); CHECK(pit_reload == 0xae);
    rt_out(0x40, 0x4d);                            /* high byte */
    CHECK(pit_reload == 0x4dae);
    CHECK(pit_period_ms > 16.5 && pit_period_ms < 16.8); /* 0x4DAE -> ~16.66ms (60Hz) */

    /* reload 0 -> 65536 -> ~55ms (18.2Hz) */
    rt_out(0x43, 0x36); rt_out(0x40, 0); rt_out(0x40, 0);
    CHECK(pit_reload == 0x10000);
    CHECK(pit_period_ms > 54.8 && pit_period_ms < 55.0);

    /* --- DAC write sequencing: 0x3c8 index + 3x 0x3c9 --- */
    pidx = 0; memset(pal, 0, sizeof pal);
    rt_out(0x3c8, 5);
    rt_out(0x3c9, 10); rt_out(0x3c9, 20); rt_out(0x3c9, 30);
    CHECK(pal[15] == 10 && pal[16] == 20 && pal[17] == 30);
    CHECK((pal32[5] & 0xffffff) == ((10u<<18)|(20u<<10)|(30u<<2)));

    /* DAC read index */
    pidx_r = 0; rt_out(0x3c7, 5);
    CHECK(rt_in(0x3c9) == 10 && rt_in(0x3c9) == 20 && rt_in(0x3c9) == 30);

    /* --- text output: char+attr write, cursor advance, wrap/scroll --- */
    memset(&mem[TEXT_BASE], 0, 4000);
    cur_mode = 3; cursor_pos[0] = 0; cursor_pos[1] = 0;
    text_putc('H'); text_putc('i');
    CHECK(mem[TEXT_BASE] == 'H' && mem[TEXT_BASE+1] == 7);
    CHECK(mem[TEXT_BASE+2] == 'i');
    CHECK(cursor_pos[1] == 2);
    text_putc('\r'); CHECK(cursor_pos[1] == 0);
    cursor_pos[1] = 79; text_putc('x');            /* wrap to next row */
    CHECK(cursor_pos[0] == 1 && cursor_pos[1] == 0);
    /* scroll at bottom */
    cursor_pos[0] = 24; cursor_pos[1] = 0;
    mem[TEXT_BASE + 160] = 'Q';                    /* row1 marker */
    for (int i = 0; i < 80; i++) text_putc('z');   /* fill row24 -> scroll */
    CHECK(cursor_pos[0] == 24);
    CHECK(mem[TEXT_BASE] == 'Q');                  /* row1 moved to row0 */

    /* --- bios_palette cases --- */
    al = 0x00; bl = 3; bh = 9; bios_palette();     /* set attr reg */
    CHECK(attrpal[3] == 9);
    al = 0x07; bl = 3; bios_palette();             /* read it back */
    CHECK(bh == 9);
    al = 0x10; bx = 7; dh = 1; ch = 2; cl = 3;     /* set DAC reg 7 */
    bios_palette();
    CHECK(pal[21] == 1 && pal[22] == 2 && pal[23] == 3);
    al = 0x15; bx = 7; bios_palette();             /* read DAC reg */
    CHECK(dh == 1 && ch == 2 && cl == 3);

    /* --- bios_video get/set mode & page --- */
    ax = 0x0013; bios_video(0x0013);
    CHECK(cur_mode == 0x13);
    ax = 0; bios_video(0x0f00);
    CHECK((ax & 0xff) == 0x13 && (ax >> 8) == 0x28);

    /* --- PIT channel read returns a byte --- */
    CHECK((rt_in(0x40) & ~0xff) == 0);
    /* --- port 0x61 speaker bits --- */
    CHECK(rt_in(0x61) == 0x30);

    fprintf(stderr, "test_video: %d checks, %d failures\n", checks, fails);
    return fails ? 1 : 0;
}
