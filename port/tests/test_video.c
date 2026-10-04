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

    /* --- console output: text_putc echoes to stderr (no text screen) --- */
    text_putc('H'); text_putc('i'); text_putc('\n');

    /* --- bios_palette DAC cases (attr regs removed: MCGA only) --- */
    al = 0x10; bx = 7; dh = 1; ch = 2; cl = 3;     /* set DAC reg 7 */
    bios_palette();
    CHECK(pal[21] == 1 && pal[22] == 2 && pal[23] == 3);
    al = 0x15; bx = 7; bios_palette();             /* read DAC reg */
    CHECK(dh == 1 && ch == 2 && cl == 3);
    al = 0x00; bl = 3; bh = 9; bios_palette();     /* attr-reg write: no-op */
    CHECK(pal[9] == 0);                           /* DAC untouched */
    al = 0x10; bx = 8; dh = 0xff; ch = 0x41; cl = 0x3f;  /* 6-bit masking */
    bios_palette();
    CHECK(pal[24] == 0x3f && pal[25] == 0x01 && pal[26] == 0x3f);

    /* block write 0x12: CX triplets from ES:DX into DAC BX.. */
    es = 0x200; dx = 0x100;
    for (int i = 0; i < 9; i++) mem[0x2100 + i] = 10 + i;
    al = 0x12; bx = 20; cx = 3;
    bios_palette();
    CHECK(pal[60] == 10 && pal[64] == 14 && pal[68] == 18);
    /* block read 0x17: DAC BX.. into ES:DX triplets */
    memset(&mem[0x2200], 0, 16);
    es = 0x200; dx = 0x200;
    al = 0x17; bx = 20; cx = 3;
    bios_palette();
    CHECK(mem[0x2200] == 10 && mem[0x2205] == 15 && mem[0x2208] == 18);

    /* DAC write index wraps at 768 -> next write restarts at reg 0 */
    rt_out(0x3c8, 255);
    rt_out(0x3c9, 1); rt_out(0x3c9, 2); rt_out(0x3c9, 3);   /* reg 255 */
    rt_out(0x3c9, 9);                                        /* wraps -> reg 0 r */
    CHECK(pal[0] == 9);

    /* --- 0x3da input-status: vblank toggles within a frame period --- */
    {   int saw_off = 0, saw_on = 0, bad = 0;
        Uint32 t0 = SDL_GetTicks();
        while (SDL_GetTicks() - t0 < 500 && !(saw_off && saw_on)){
            dw v = rt_in(0x3da);
            bad |= v & ~9;                        /* only bits 0+3 used */
            if (v == 9) saw_on = 1; else if (v == 0) saw_off = 1;
        }
        CHECK(bad == 0);
        CHECK(saw_off && saw_on);                 /* real 70Hz beam timing */
    }
    CHECK(rt_in(0x3c7) == 0);                     /* DAC state: ready */
    CHECK(rt_in(0x9999) == 0);                    /* unhandled port -> 0 */
    rt_out(0x3c4, 0x01); rt_out(0x3d4, 0x11);     /* seq/CRT: harmless no-ops */
    rt_out(0x20, 0x20);                           /* PIC EOI: no-op */

    /* --- bios_video get/set mode --- */
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
