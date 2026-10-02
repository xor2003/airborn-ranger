/* unit tests for input.c: scancode table, script feeder, int9 held-mask,
 * BIOS key queue. includes input.c to reach statics. */
#include <SDL2/SDL.h>
#include "../rt.h"
#include <stdio.h>

void rt_frame(void){}                              /* video.c stubs */
dd tnd_base;
dw rt_in(dw p){ (void)p; return 0; }
void rt_out(dw p, dw v){ (void)p; (void)v; }

#include "../input.c"

static int fails, checks;
#define CHECK(cond) do{ checks++; if(!(cond)){ fails++; \
    fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); } }while(0)

int main(void){
    /* --- xt_scan table (spot checks across rows) --- */
    CHECK(xt_scan(SDL_SCANCODE_ESCAPE) == 0x01);
    CHECK(xt_scan(SDL_SCANCODE_1) == 0x02);
    CHECK(xt_scan(SDL_SCANCODE_9) == 0x0a);
    CHECK(xt_scan(SDL_SCANCODE_RETURN) == 0x1c);
    CHECK(xt_scan(SDL_SCANCODE_A) == 0x1e);
    CHECK(xt_scan(SDL_SCANCODE_Z) == 0x2c);
    CHECK(xt_scan(SDL_SCANCODE_SPACE) == 0x39);
    CHECK(xt_scan(SDL_SCANCODE_F1) == 0x3b);
    CHECK(xt_scan(SDL_SCANCODE_UP) == 0x48);
    CHECK(xt_scan(SDL_SCANCODE_DOWN) == 0x50);
    CHECK(xt_scan(SDL_SCANCODE_LEFT) == 0x4b);
    CHECK(xt_scan(SDL_SCANCODE_RIGHT) == 0x4d);
    CHECK(xt_scan(SDL_SCANCODE_KP_ENTER) == 0);    /* unmapped -> 0 */

    /* --- ascii_of --- */
    CHECK(ascii_of('a') == 'a');
    CHECK(ascii_of('5') == '5');
    CHECK(ascii_of('\r') == '\r');
    CHECK(ascii_of(0x1000) == 0);

    /* --- script_key parsing --- */
    int sc; db asc;
    const char *p = script_key("5", &sc, &asc);
    CHECK(sc == 0x06 && asc == '5' && *p == 0);
    p = script_key("\\r", &sc, &asc);
    CHECK(sc == 0x1c && asc == 0x0d && *p == 0);
    p = script_key("\\u", &sc, &asc);
    CHECK(sc == 0x48 && asc == 0);
    p = script_key("\\d", &sc, &asc);
    CHECK(sc == 0x50);
    p = script_key("\\l", &sc, &asc);
    CHECK(sc == 0x4b);
    p = script_key("\\R", &sc, &asc);
    CHECK(sc == 0x4d);
    p = script_key("\\e", &sc, &asc);
    CHECK(sc == 0x01 && asc == 0x1b);
    p = script_key(".", &sc, &asc);
    CHECK(sc == -1);                                /* pause token */
    p = script_key("q", &sc, &asc);
    CHECK(sc == 0x10 && asc == 'q');

    /* --- int9_update: held-key bitmask --- */
    memset(&mem[DS_BASE], 0, 0x1000);
    *(dw*)&mem[DS_BASE + 0xad7] = 0;                /* divert off -> no-op */
    *(dw*)&mem[DS_BASE + 0x950 + 0x1c*2] = 0x0010;  /* ENTER -> bit4 */
    CHECK(int9_update(0x1c, 1) == 0);
    *(dw*)&mem[DS_BASE + 0xad7] = 1;
    CHECK(int9_update(0x1c, 1) == 1);               /* diverted */
    CHECK(*(dw*)&mem[DS_BASE + 0xad9] == 0x0010);
    CHECK(int9_update(0x1c, 0) == 1);               /* release clears */
    CHECK(*(dw*)&mem[DS_BASE + 0xad9] == 0);
    CHECK(int9_update(0x02, 1) == 0);               /* no mask entry: not diverted */

    /* --- key queue + bios_kbhit --- */
    khead = ktail = 0;
    kpush(0x1c0d); kpush(0x0332);
    bios_kbhit();
    CHECK(ZF == 0); CHECK(ax == 0x1c0d);            /* peek: no consume */
    bios_kbhit();
    CHECK(ax == 0x1c0d);
    int v = kpop();
    CHECK(v == 0x1c0d);
    CHECK(kpop() == 0x0332);
    CHECK(kpop() == -1);
    bios_kbhit();
    CHECK(ZF == 1);

    /* queue full: drop-oldest-free behavior (no overflow past tail) */
    khead = ktail = 0;
    for (int i = 0; i < QN + 8; i++) kpush((dw)i);
    int cnt = 0; while (kpop() >= 0) cnt++;
    CHECK(cnt == QN - 1);                           /* ring holds QN-1 */

    /* --- bios_kbd dispatch --- */
    khead = ktail = 0; kpush(0x2e03);
    bios_kbd(0x0100);                               /* AH=1 kbhit */
    CHECK(ZF == 0 && ax == 0x2e03);
    bios_kbd(0x0200);                               /* AH=2 shift flags */
    CHECK(al == 0);

    /* --- feeder: plain key reaches queue after a reader polls --- */
    unsetenv("M2C_KEYS"); setenv("M2C_KEYS_DELAY", "1", 1);
    khead = ktail = 0; kbd_polls = 0;
    *(dw*)&mem[DS_BASE + 0xad7] = 0;                /* nothing diverted */
    setenv("M2C_KEYS", "5", 1);
    /* first call: registers pending wait for a poll */
    for (int i = 0; i < 4; i++) rt_script_feed();
    CHECK(khead == ktail);                          /* waits for reader */
    bios_kbhit();                                   /* reader polls */
    for (int i = 0; i < 4; i++) rt_script_feed();
    bios_kbhit();
    CHECK(ZF == 0); CHECK((ax >> 8) == 0x06);       /* '5' scancode */
    unsetenv("M2C_KEYS");

    /* --- mouse -> keyboard emulation --- */
    unsetenv("M2C_MOUSE"); mouse_on = -1;
    CHECK(mouse_enabled() == 1);                    /* default on */
    setenv("M2C_MOUSE", "0", 1); mouse_on = -1;
    CHECK(mouse_enabled() == 0);
    unsetenv("M2C_MOUSE"); mouse_on = -1;

    /* click = Enter keycode on an int16 (non-diverted) menu */
    memset(&mem[DS_BASE], 0, 0x1000);
    *(dw*)&mem[DS_BASE + 0xad7] = 0;
    khead = ktail = 0; rel_n = 0;
    synth_key(0x1c, 0x0d, 1);                       /* left button down */
    synth_key(0x1c, 0x0d, 0);                       /* up: queues nothing */
    bios_kbhit();
    CHECK(ZF == 0); CHECK(ax == 0x1c0d);
    kpop();
    CHECK(khead == ktail);                          /* exactly one key */

    /* click = held-mask Enter on a diverted (POD) screen */
    *(dw*)&mem[DS_BASE + 0xad7] = 1;
    *(dw*)&mem[DS_BASE + 0x950 + 0x1c*2] = 0x0010;  /* ENTER -> bit4 */
    *(dw*)&mem[DS_BASE + 0xad9] = 0;
    rel_n = 0; khead = ktail = 0;
    synth_key(0x1c, 0x0d, 1);
    CHECK(*(dw*)&mem[DS_BASE + 0xad9] == 0x0010);   /* bit held */
    CHECK(khead == ktail);                          /* nothing on int16 */
    synth_key(0x1c, 0x0d, 0);
    CHECK(rel_n == 1);                              /* release deferred */

    /* motion -> discrete arrow keycodes on an int16 menu */
    *(dw*)&mem[DS_BASE + 0xad7] = 0;
    khead = ktail = 0;
    macc_x = 0; macc_y = 50;                        /* 50px down = 2x20px steps */
    mouse_dir_step();
    int n = 0; while (kpop() >= 0) n++;
    CHECK(n == 2); CHECK(macc_y == 10);             /* 50-40 leftover */

    /* motion -> held arrow bit on a held-mask (POD) screen */
    memset(&mem[DS_BASE], 0, 0x1000);
    *(dw*)&mem[DS_BASE + 0xad7] = 1;
    *(dw*)&mem[DS_BASE + 0x950 + 0x4d*2] = 0x0040;  /* RIGHT -> bit6 */
    *(dw*)&mem[DS_BASE + 0x950 + 0x4b*2] = 0x0080;  /* LEFT  -> bit7 */
    macc_x = 30; macc_y = 0;
    mouse_dir_step();
    CHECK(*(dw*)&mem[DS_BASE + 0xad9] == 0x0040);   /* RIGHT held */
    CHECK(macc_x == 18);                            /* drained 12px */
    mouse_dir_step();
    CHECK(*(dw*)&mem[DS_BASE + 0xad9] == 0x0040);   /* still held mid-drain */
    macc_x = 0;
    for (int i = 0; i < 8; i++) mouse_dir_step();
    CHECK(*(dw*)&mem[DS_BASE + 0xad9] == 0);        /* released when spent */

    /* opposite direction flips the held bit, not just adds */
    macc_x = -15;
    mouse_dir_step();
    CHECK(*(dw*)&mem[DS_BASE + 0xad9] == 0x0080);   /* LEFT held */

    /* --- SDL event loop integration: real MOUSEMOTION/BUTTON events --- */
    SDL_Init(SDL_INIT_EVENTS);
    *(dw*)&mem[DS_BASE + 0xad7] = 0;                /* int16 menu */
    memset(&mem[DS_BASE + 0x950], 0, 0x200);
    khead = ktail = 0; macc_x = macc_y = 0; rel_n = 0;
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
    SDL_Event me; memset(&me, 0, sizeof me);
    me.type = SDL_MOUSEMOTION; me.motion.xrel = 40; me.motion.yrel = 0;
    SDL_PushEvent(&me);
    rt_pump_events();                               /* drain -> dir_step */
    n = 0; while (kpop() >= 0) n++;
    CHECK(n == 2);                                  /* 40px -> 2 right arrows */

    memset(&me, 0, sizeof me);
    me.type = SDL_MOUSEBUTTONDOWN; me.button.button = SDL_BUTTON_LEFT;
    SDL_PushEvent(&me);
    memset(&me, 0, sizeof me);
    me.type = SDL_MOUSEBUTTONUP; me.button.button = SDL_BUTTON_LEFT;
    SDL_PushEvent(&me);
    rt_pump_events();
    bios_kbhit();
    CHECK(ZF == 0); CHECK(ax == 0x1c0d);            /* click -> Enter */
    kpop();

    fprintf(stderr, "test_input: %d checks, %d failures\n", checks, fails);
    return fails ? 1 : 0;
}
