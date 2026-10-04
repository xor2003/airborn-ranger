/* unit tests for input.c: scancode table, script feeder, int9 held-mask,
 * BIOS key queue, key-mapping menu. includes input.c to reach statics. */
#include <SDL2/SDL.h>
#include "../rt.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

void rt_frame(void){}                              /* video.c stubs */
void rt_kmap_frame(void){}
dd tnd_base;
dw rt_in(dw p){ (void)p; return 0; }
void rt_out(dw p, dw v){ (void)p; (void)v; }

#include "../input.c"

static int fails, checks;
#define CHECK(cond) do{ checks++; if(!(cond)){ fails++; \
    fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); } }while(0)

int main(void){
    /* M2C_HOLDWALK's env is captured on the first pump call (a file-local
     * static); setting it here is harmless — it only writes the held mask
     * while mem[0x1c3ea] (walk phase) is nonzero, which the tests control. */
    setenv("M2C_HOLDWALK", "3f", 1);

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
    CHECK(ascii_of(SDLK_RETURN) == '\r');
    CHECK(ascii_of(SDLK_ESCAPE) == 27);

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
    p = script_key("\\H", &sc, &asc);              /* hold-next-key flag */
    CHECK(sc == -3);
    p = script_key("\\F", &sc, &asc);              /* F9 -> map overlay */
    CHECK(sc == 0x43);

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
    memset(&mem[DS_BASE], 0, 0x1000);               /* clear stale divert masks */
    *(dw*)&mem[DS_BASE + 0xad7] = 0;                /* nothing diverted */
    memset(&mem[0xa0000], 0, 64000);                /* stable screen hash */
    memset(&mem[0xb8000], 0, 4000);
    setenv("M2C_KEYS", "5\\u.\\H\\d \r\e", 1);     /* tap, arrows, hold, punctuation */
    setenv("M2C_MASKDUMP", "1", 1);                /* exercise the debug dumps */
    setenv("M2C_KTRACE", "1", 1);
    /* first call: registers pending wait for a poll */
    for (int i = 0; i < 4; i++) rt_script_feed();
    CHECK(khead == ktail);                          /* waits for reader */
    bios_kbhit();                                   /* reader polls */
    for (int i = 0; i < 4; i++) rt_script_feed();
    bios_kbhit();
    CHECK(ZF == 0); CHECK((ax >> 8) == 0x06);       /* '5' scancode */
    kpop();

    /* the same script continues: diverted arrows, pause, \H hold flag —
     * must be consumed here because `script` is a process-lifetime static */
    *(dw*)&mem[DS_BASE + 0xad7] = 1;                /* divert on */
    *(dw*)&mem[DS_BASE + 0x950 + 0x48*2] = 1;       /* UP diverted */
    *(dw*)&mem[DS_BASE + 0x950 + 0x50*2] = 2;       /* DOWN diverted */
    for (int i = 0; i < 80 && !(*(dw*)&mem[DS_BASE + 0xad9] & 1); i++)
        rt_script_feed();
    CHECK(*(dw*)&mem[DS_BASE + 0xad9] & 1);          /* script held UP */
    for (int i = 0; i < 80 && (*(dw*)&mem[DS_BASE + 0xad9] & 1); i++)
        rt_script_feed();
    CHECK(!(*(dw*)&mem[DS_BASE + 0xad9] & 1));       /* script released UP */
    for (int i = 0; i < 80 && !(*(dw*)&mem[DS_BASE + 0xad9] & 2); i++)
        rt_script_feed();
    CHECK(*(dw*)&mem[DS_BASE + 0xad9] & 2);          /* \H: DOWN stays held */
    for (int i = 0; i < 10; i++) rt_script_feed();
    CHECK(*(dw*)&mem[DS_BASE + 0xad9] & 2);          /* still held (no release) */
    int9_update(0x50, 0);                            /* drop the leftover hold */

    /* tail of the script: ' ', CR, ESC — plain keys gated on reader polls */
    static const dw want[3] = {0x3920, 0x1c0d, 0x011b};
    for (int k = 0; k < 3; k++){
        khead = ktail = 0;
        for (int i = 0; i < 40 && khead == ktail; i++){
            if (i % 4 == 3) bios_kbhit();            /* reader polls periodically */
            rt_script_feed();
        }
        bios_kbhit();
        CHECK(ZF == 0 && ax == want[k]);
        kpop();
    }
    unsetenv("M2C_KEYS"); unsetenv("M2C_KEYS_DELAY");
    unsetenv("M2C_MASKDUMP"); unsetenv("M2C_KTRACE");
    memset(&mem[DS_BASE], 0, 0x1000);

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

    /* absolute pointer position -> direction areas on an int16 menu:
     * entering an edge zone fires one discrete step; center is neutral */
    *(dw*)&mem[DS_BASE + 0xad7] = 0;
    *(dw*)&mem[0xf340] = 0;                         /* not POD */
    khead = ktail = 0;
    mgx = 160; mgy = 150;                           /* bottom third -> DOWN */
    mouse_zone_step();
    int n = 0; while (kpop() >= 0) n++;
    CHECK(n == 1);                                  /* one step on zone entry */
    mgx = 160; mgy = 100;                           /* center -> nothing */
    mouse_zone_step();
    n = 0; while (kpop() >= 0) n++;
    CHECK(n == 0);
    mgx = 60;                                       /* left third -> LEFT */
    mouse_zone_step();
    n = 0; while (kpop() >= 0) n++;
    CHECK(n == 1);
    CHECK(kpop() == -1);

    /* absolute position -> held arrow bit on a held-mask (gameplay) screen */
    memset(&mem[DS_BASE], 0, 0x1000);
    *(dw*)&mem[DS_BASE + 0xad7] = 1;
    *(dw*)&mem[0xf340] = 0;
    *(dw*)&mem[DS_BASE + 0x950 + 0x4d*2] = 0x0040;  /* RIGHT -> bit6 */
    *(dw*)&mem[DS_BASE + 0x950 + 0x4b*2] = 0x0080;  /* LEFT  -> bit7 */
    mgx = 300; mgy = 100;                           /* right edge */
    mouse_zone_step();
    CHECK(*(dw*)&mem[DS_BASE + 0xad9] == 0x0040);   /* RIGHT held */
    mouse_zone_step();
    CHECK(*(dw*)&mem[DS_BASE + 0xad9] == 0x0040);   /* still held while parked */
    mgx = 160;                                      /* recenter -> release */
    mouse_zone_step();
    CHECK(*(dw*)&mem[DS_BASE + 0xad9] == 0);

    /* opposite edge flips the held bit, not just adds */
    mgx = 20;
    mouse_zone_step();
    CHECK(*(dw*)&mem[DS_BASE + 0xad9] == 0x0080);   /* LEFT held */

    /* --- SDL event loop integration: real MOUSEMOTION/BUTTON events --- */
    SDL_Init(SDL_INIT_EVENTS);
    *(dw*)&mem[DS_BASE + 0xad7] = 0;                /* int16 menu */
    *(dw*)&mem[0xf340] = 0;
    memset(&mem[DS_BASE + 0x950], 0, 0x200);
    khead = ktail = 0; mgx = mgy = -1; rel_n = 0;
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
    SDL_Event me; memset(&me, 0, sizeof me);
    me.type = SDL_MOUSEMOTION;                      /* right edge of 960x600 */
    me.motion.x = 900; me.motion.y = 300;
    SDL_PushEvent(&me);
    rt_pump_events();                               /* area -> dir step */
    n = 0; while (kpop() >= 0) n++;
    CHECK(n == 1);                                  /* one right-arrow press */

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

    /* --- rt_kbd_port60: set by key path, read clears --- */
    khead = ktail = 0;
    *(dw*)&mem[DS_BASE + 0xad7] = 0;
    synth_key(0x48, 0, 1);
    CHECK(rt_kbd_port60() == 0x48);
    CHECK(rt_kbd_port60() == 0);
    synth_key(0x48, 0, 0);                          /* break code */
    CHECK(rt_kbd_port60() == (0x48 | 0x80));
    while (kpop() >= 0);                            /* drain queued presses */

    /* --- bios_getch: returns pending key --- */
    khead = ktail = 0; kpush(0x2e03);
    bios_getch();
    CHECK(ax == 0x2e03 && khead == ktail);

    /* --- bios_time: int1Ah ticks --- */
    bios_time();
    CHECK(al == 0);

    /* --- POD screens: pointer sets cursor coords, zones suppressed --- */
    memset(&mem[DS_BASE], 0, 0x1000);               /* NB: 0xf340 is inside this range */
    *(dw*)&mem[0xf340] = 5;                          /* POD mode */
    win_w = 960; win_h = 600;
    pod_abs_cursor(960, 600);                        /* bottom-right corner */
    CHECK(mem[0xf71f] == 0x38 && mem[0xf73f] == 0x01);  /* X clamped 0x138 */
    CHECK(mem[0xf75f] == 0xc4);                         /* Y clamped 0xc4  */
    *(dw*)&mem[DS_BASE + 0xad7] = 1;
    *(dw*)&mem[DS_BASE + 0x950 + 0x4d*2] = 0x0040;
    mgx = 300; mgy = 100;
    mouse_zone_step();
    CHECK(*(dw*)&mem[DS_BASE + 0xad9] == 0);         /* zones off in POD */
    *(dw*)&mem[0xf340] = 0;

    /* --- deferred release: diverted keyup defers, rel_expire completes --- */
    memset(&mem[DS_BASE], 0, 0x1000);
    *(dw*)&mem[DS_BASE + 0xad7] = 1;
    *(dw*)&mem[DS_BASE + 0x950 + 0x4d*2] = 0x0040;
    *(dw*)&mem[DS_BASE + 0xad9] = 0;
    rel_n = 0;
    down_at[0x4d] = SDL_GetTicks();                  /* pressed just now */
    int9_update(0x4d, 1);
    rel_defer(0x4d);
    CHECK(rel_n == 1);                               /* deferred, still held */
    CHECK(*(dw*)&mem[DS_BASE + 0xad9] == 0x0040);
    rel_pend[0].at = 0;                              /* force expiry */
    rel_expire();
    CHECK(rel_n == 0);
    CHECK(*(dw*)&mem[DS_BASE + 0xad9] == 0);

    /* --- mrelease: clears held bit even with divert off --- */
    *(dw*)&mem[DS_BASE + 0xad7] = 0;
    *(dw*)&mem[DS_BASE + 0x950 + 0x4b*2] = 0x0080;
    *(dw*)&mem[DS_BASE + 0xad9] = 0x0080;
    mrelease(0x4b);
    CHECK(*(dw*)&mem[DS_BASE + 0xad9] == 0);

    /* --- ctrl_menu_autopick: menu_mode=8 + M2C_CTRL -> typed key --- */
    ctrl_done = 0;
    *(dw*)&mem[0xf340] = 8;
    khead = ktail = 0;
    setenv("M2C_CTRL", "2", 1);
    bios_kbhit();
    CHECK(ZF == 0); CHECK(ax == 0x0332);             /* '2' -> 0x03<<8|0x32 */
    kpop();
    bios_kbhit();                                    /* picked once only */
    CHECK(ZF == 1);
    ctrl_done = 0;
    setenv("M2C_CTRL", "9", 1);                      /* out of range: no pick */
    bios_kbhit();
    CHECK(ZF == 1);
    setenv("M2C_CTRL", "1", 1);
    *(dw*)&mem[0xf340] = 0;                          /* not the device menu */
    bios_kbhit();
    CHECK(ZF == 1);
    unsetenv("M2C_CTRL");

    /* --- key-mapping menu --- */
    {   char oldcwd[512];
        CHECK(getcwd(oldcwd, sizeof oldcwd) != NULL);
        mkdir("/tmp/ar_kmap", 0755);
        CHECK(chdir("/tmp/ar_kmap") == 0);
        unlink(KMAP_FILE);

        /* defaults: movement/select keys self-bound, F10 NOT bound */
        kmap_loaded = 0; kmap_load();
        CHECK(binds[0].kind == 1 && binds[0].code == SDL_SCANCODE_UP);
        CHECK(binds[4].kind == 1 && binds[4].code == SDL_SCANCODE_RETURN);
        CHECK(binds[30].kind == 0);                  /* RUN/WALK (F10) row */
        CHECK(N_KMAP == 33 && N_BINDABLE == 31);

        /* save/load roundtrip */
        binds[0].kind = 1; binds[0].code = SDL_SCANCODE_G;
        kmap_save();
        kmap_defaults();
        CHECK(binds[0].code == SDL_SCANCODE_UP);     /* back to default */
        kmap_load();
        CHECK(binds[0].kind == 1 && binds[0].code == SDL_SCANCODE_G);

        /* malformed file: keeps defaults, no crash */
        FILE *bf = fopen(KMAP_FILE, "w");
        fputs("garbage\n99=9:99999\n-1=1:5\n", bf); fclose(bf);
        kmap_defaults();
        kmap_load();
        CHECK(binds[0].kind == 1 && binds[0].code == SDL_SCANCODE_UP);
        unlink(KMAP_FILE);

        /* capture binds; an input bound elsewhere is stolen */
        kmap_sel = 0; kmap_cap = 1;
        SDL_Event ev; memset(&ev, 0, sizeof ev);
        ev.type = SDL_KEYDOWN; ev.key.keysym.scancode = SDL_SCANCODE_J;
        CHECK(kmap_capture(&ev) == 1);
        CHECK(binds[0].kind == 1 && binds[0].code == SDL_SCANCODE_J);
        CHECK(kmap_cap == 0);
        kmap_sel = 1; kmap_cap = 1;
        kmap_capture(&ev);                           /* same key -> stolen */
        CHECK(binds[0].kind == 0);
        CHECK(binds[1].kind == 1 && binds[1].code == SDL_SCANCODE_J);

        /* Esc cancels capture without rebinding */
        kmap_sel = 2; kmap_cap = 1;
        int oldk = binds[2].kind; unsigned oldc = binds[2].code;
        ev.key.keysym.scancode = SDL_SCANCODE_ESCAPE;
        kmap_capture(&ev);
        CHECK(binds[2].kind == oldk && binds[2].code == oldc);
        CHECK(kmap_cap == 0);

        /* controller button capture -> kind 2 */
        kmap_sel = 2; kmap_cap = 1;
        memset(&ev, 0, sizeof ev);
        ev.type = SDL_CONTROLLERBUTTONDOWN;
        ev.cbutton.button = SDL_CONTROLLER_BUTTON_X;
        kmap_capture(&ev);
        CHECK(binds[2].kind == 2 && binds[2].code == SDL_CONTROLLER_BUTTON_X);

        /* accessors */
        CHECK(rt_kmap_rows() == N_KMAP);
        CHECK(rt_kmap_capture() == 0);
        CHECK(!strcmp(rt_kmap_name(0), "MOVE UP (UP)"));
        CHECK(!strcmp(rt_kmap_bind(1), "J"));        /* scancode name */
        CHECK(!strcmp(rt_kmap_bind(0), "-"));        /* unbound */

        /* kmap_match: bound input injects its DOS key, repeats ignored */
        memset(&mem[DS_BASE], 0, 0x1000);
        *(dw*)&mem[DS_BASE + 0xad7] = 0;
        khead = ktail = 0;
        memset(&ev, 0, sizeof ev);
        ev.type = SDL_KEYDOWN; ev.key.keysym.scancode = SDL_SCANCODE_J;
        CHECK(kmap_match(&ev) == 1);
        bios_kbhit();
        CHECK(ZF == 0); CHECK(ax == (0x50 << 8));    /* row1 = MOVE DOWN */
        kpop();
        ev.key.repeat = 1;
        CHECK(kmap_match(&ev) == 0);                 /* autorepeat ignored */
        ev.key.repeat = 0;
        ev.type = SDL_KEYUP;
        CHECK(kmap_match(&ev) == 1);                 /* release also bound */
        memset(&ev, 0, sizeof ev);
        ev.type = SDL_KEYDOWN; ev.key.keysym.scancode = SDL_SCANCODE_K;
        CHECK(kmap_match(&ev) == 0);                 /* unbound key falls through */

        /* modal navigation: arrows move selection, Enter arms capture,
         * START GAME and Esc close the menu */
        kmap_on = 1; kmap_sel = 0; kmap_cap = 0;
        memset(&ev, 0, sizeof ev);
        ev.type = SDL_KEYDOWN; ev.key.keysym.scancode = SDL_SCANCODE_DOWN;
        CHECK(kmap_event(&ev) == 1); CHECK(kmap_sel == 1);
        ev.key.keysym.scancode = SDL_SCANCODE_UP;
        kmap_event(&ev); CHECK(kmap_sel == 0);
        ev.key.keysym.scancode = SDL_SCANCODE_RETURN;
        kmap_event(&ev); CHECK(kmap_cap == 1);       /* Enter on row -> capture */
        kmap_cap = 0;
        kmap_sel = N_KMAP - 1;                       /* START GAME */
        kmap_event(&ev); CHECK(kmap_on == 0);
        kmap_on = 1; kmap_sel = 0;
        ev.key.keysym.scancode = SDL_SCANCODE_ESCAPE;
        kmap_event(&ev); CHECK(kmap_on == 0);
        kmap_on = 0;

        /* RESET DEFAULTS row restores the map */
        binds[0].kind = 0;
        kmap_on = 1; kmap_sel = N_KMAP - 2;
        ev.key.keysym.scancode = SDL_SCANCODE_RETURN;
        kmap_event(&ev);
        CHECK(binds[0].kind == 1 && binds[0].code == SDL_SCANCODE_UP);
        kmap_on = 0;

        /* kmap_sel accessor */
        kmap_sel = 7; CHECK(rt_kmap_cur() == 7); kmap_sel = 0;

        /* controller nav: dpad moves, A/START select, B/BACK/GUIDE close */
        kmap_on = 1; kmap_sel = 0; kmap_cap = 0;
        memset(&ev, 0, sizeof ev);
        ev.type = SDL_CONTROLLERBUTTONDOWN;
        ev.cbutton.button = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
        kmap_event(&ev); CHECK(kmap_sel == 1);
        ev.cbutton.button = SDL_CONTROLLER_BUTTON_DPAD_UP;
        kmap_event(&ev); CHECK(kmap_sel == 0);
        ev.cbutton.button = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
        CHECK(kmap_event(&ev) == 1);                /* ignored */
        ev.cbutton.button = SDL_CONTROLLER_BUTTON_A;
        kmap_event(&ev); CHECK(kmap_cap == 1);      /* arms capture */
        kmap_cap = 0;
        kmap_sel = N_KMAP - 1;
        ev.cbutton.button = SDL_CONTROLLER_BUTTON_START;
        kmap_event(&ev); CHECK(kmap_on == 0);       /* START GAME */
        kmap_on = 1;
        ev.cbutton.button = SDL_CONTROLLER_BUTTON_B;
        kmap_event(&ev); CHECK(kmap_on == 0);       /* B = back */
        kmap_on = 1;
        ev.type = SDL_CONTROLLERBUTTONUP;
        CHECK(kmap_event(&ev) == 1);                /* swallowed */
        ev.type = SDL_CONTROLLERAXISMOTION;
        ev.caxis.axis = SDL_CONTROLLER_AXIS_LEFTY;
        ev.caxis.value = 30000;
        kmap_sel = 0;
        kmap_event(&ev); CHECK(kmap_sel == 1);      /* stick nav down */
        ev.caxis.value = -30000;
        kmap_event(&ev); CHECK(kmap_sel == 0);      /* opposite dir: immediate */
        ev.caxis.axis = SDL_CONTROLLER_AXIS_LEFTX;
        CHECK(kmap_event(&ev) == 1);                /* other axis ignored */
        ev.type = SDL_KEYDOWN; ev.key.keysym.scancode = SDL_SCANCODE_LEFT;
        CHECK(kmap_event(&ev) == 1);
        ev.key.keysym.scancode = SDL_SCANCODE_MENU;
        kmap_event(&ev); CHECK(kmap_on == 0);       /* MENU closes */
        kmap_on = 1;
        ev.type = SDL_QUIT;
        CHECK(kmap_event(&ev) == 0);                /* QUIT falls through */
        kmap_on = 0;

        /* capture mode swallows releases/motion, ignores other events */
        kmap_cap = 1;
        memset(&ev, 0, sizeof ev);
        ev.type = SDL_CONTROLLERBUTTONUP; CHECK(kmap_capture(&ev) == 1);
        ev.type = SDL_CONTROLLERAXISMOTION; CHECK(kmap_capture(&ev) == 1);
        ev.type = SDL_KEYUP;              CHECK(kmap_capture(&ev) == 1);
        ev.type = SDL_MOUSEMOTION;        CHECK(kmap_capture(&ev) == 0);
        kmap_cap = 0;
        CHECK(rt_kmap_capture() == 0);

        /* desktop without M2C_KMAPSTART: exclusive screen returns at once */
        unsetenv("M2C_KMAPSTART");
        rt_kmap_exclusive();

        /* kmap_wanted: M2C_NOKMAP disables */
        kmap_enabled = -1;
        setenv("M2C_NOKMAP", "1", 1);
        CHECK(kmap_wanted() == 0);
        kmap_enabled = -1;
        unsetenv("M2C_NOKMAP");
        CHECK(kmap_wanted() == 1);
        kmap_enabled = -1;

        CHECK(chdir(oldcwd) == 0);
        kmap_defaults();                             /* leave clean state */
    }

    /* --- xt_scan: the whole SDL->XT table --- */
    {   static const struct { int s; int x; } map[] = {
            {SDL_SCANCODE_ESCAPE,0x01},{SDL_SCANCODE_AC_BACK,0x01},
            {SDL_SCANCODE_1,0x02},{SDL_SCANCODE_2,0x03},{SDL_SCANCODE_3,0x04},
            {SDL_SCANCODE_4,0x05},{SDL_SCANCODE_5,0x06},{SDL_SCANCODE_6,0x07},
            {SDL_SCANCODE_7,0x08},{SDL_SCANCODE_8,0x09},{SDL_SCANCODE_9,0x0a},
            {SDL_SCANCODE_0,0x0b},{SDL_SCANCODE_MINUS,0x0c},{SDL_SCANCODE_EQUALS,0x0d},
            {SDL_SCANCODE_BACKSPACE,0x0e},{SDL_SCANCODE_TAB,0x0f},
            {SDL_SCANCODE_Q,0x10},{SDL_SCANCODE_W,0x11},{SDL_SCANCODE_E,0x12},
            {SDL_SCANCODE_R,0x13},{SDL_SCANCODE_T,0x14},{SDL_SCANCODE_Y,0x15},
            {SDL_SCANCODE_U,0x16},{SDL_SCANCODE_I,0x17},{SDL_SCANCODE_O,0x18},
            {SDL_SCANCODE_P,0x19},{SDL_SCANCODE_LEFTBRACKET,0x1a},
            {SDL_SCANCODE_RIGHTBRACKET,0x1b},
            {SDL_SCANCODE_RETURN,0x1c},{SDL_SCANCODE_LCTRL,0x1d},
            {SDL_SCANCODE_SELECT,0x1c},
            {SDL_SCANCODE_A,0x1e},{SDL_SCANCODE_S,0x1f},{SDL_SCANCODE_D,0x20},
            {SDL_SCANCODE_F,0x21},{SDL_SCANCODE_G,0x22},{SDL_SCANCODE_H,0x23},
            {SDL_SCANCODE_J,0x24},{SDL_SCANCODE_K,0x25},{SDL_SCANCODE_L,0x26},
            {SDL_SCANCODE_SEMICOLON,0x27},{SDL_SCANCODE_APOSTROPHE,0x28},
            {SDL_SCANCODE_GRAVE,0x29},{SDL_SCANCODE_LSHIFT,0x2a},
            {SDL_SCANCODE_BACKSLASH,0x2b},{SDL_SCANCODE_Z,0x2c},
            {SDL_SCANCODE_X,0x2d},{SDL_SCANCODE_C,0x2e},{SDL_SCANCODE_V,0x2f},
            {SDL_SCANCODE_B,0x30},{SDL_SCANCODE_N,0x31},{SDL_SCANCODE_M,0x32},
            {SDL_SCANCODE_COMMA,0x33},{SDL_SCANCODE_PERIOD,0x34},
            {SDL_SCANCODE_SLASH,0x35},{SDL_SCANCODE_RSHIFT,0x36},
            {SDL_SCANCODE_KP_MULTIPLY,0x37},{SDL_SCANCODE_LALT,0x38},
            {SDL_SCANCODE_SPACE,0x39},{SDL_SCANCODE_CAPSLOCK,0x3a},
            {SDL_SCANCODE_F1,0x3b},{SDL_SCANCODE_F2,0x3c},{SDL_SCANCODE_F3,0x3d},
            {SDL_SCANCODE_F4,0x3e},{SDL_SCANCODE_F5,0x3f},{SDL_SCANCODE_F6,0x40},
            {SDL_SCANCODE_F7,0x41},{SDL_SCANCODE_F8,0x42},{SDL_SCANCODE_F9,0x43},
            {SDL_SCANCODE_F10,0x44},{SDL_SCANCODE_NUMLOCKCLEAR,0x45},
            {SDL_SCANCODE_SCROLLLOCK,0x46},{SDL_SCANCODE_KP_7,0x47},
            {SDL_SCANCODE_KP_8,0x48},{SDL_SCANCODE_KP_9,0x49},
            {SDL_SCANCODE_KP_MINUS,0x4a},{SDL_SCANCODE_KP_4,0x4b},
            {SDL_SCANCODE_KP_5,0x4c},{SDL_SCANCODE_KP_6,0x4d},
            {SDL_SCANCODE_KP_PLUS,0x4e},{SDL_SCANCODE_KP_1,0x4f},
            {SDL_SCANCODE_KP_2,0x50},{SDL_SCANCODE_KP_3,0x51},
            {SDL_SCANCODE_KP_0,0x52},{SDL_SCANCODE_KP_PERIOD,0x53},
            {SDL_SCANCODE_UP,0x48},{SDL_SCANCODE_DOWN,0x50},
            {SDL_SCANCODE_LEFT,0x4b},{SDL_SCANCODE_RIGHT,0x4d},
            {SDL_SCANCODE_HOME,0x47},{SDL_SCANCODE_END,0x4f},
            {SDL_SCANCODE_PAGEUP,0x49},{SDL_SCANCODE_PAGEDOWN,0x51},
            {SDL_SCANCODE_INSERT,0x52},{SDL_SCANCODE_DELETE,0x53},
        };
        for (size_t i = 0; i < sizeof map / sizeof *map; i++)
            CHECK(xt_scan(map[i].s) == map[i].x);
        CHECK(xt_scan(SDL_SCANCODE_AUDIOPLAY) == 0);      /* unmapped -> 0 */
    }

    /* --- through the event pump: F8 opens the menu, F10 stays a game key --- */
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
    kmap_on = 0; kmap_enabled = -1;
    memset(&mem[DS_BASE], 0, 0x1000);
    *(dw*)&mem[DS_BASE + 0xad7] = 0;
    khead = ktail = 0;
    memset(&me, 0, sizeof me);
    me.type = SDL_KEYDOWN; me.key.keysym.scancode = SDL_SCANCODE_F8;
    SDL_PushEvent(&me);
    rt_pump_events();
    CHECK(rt_kmap_open() == 1);                      /* menu opened */
    SDL_PushEvent(&me);                              /* F8 again: closes */
    rt_pump_events();
    CHECK(rt_kmap_open() == 0);

    me.key.keysym.scancode = SDL_SCANCODE_F10;
    SDL_PushEvent(&me);
    rt_pump_events();
    CHECK(rt_kmap_open() == 0);                      /* NOT a menu trigger */
    bios_kbhit();
    CHECK(ZF == 0); CHECK(ax == (0x44 << 8));        /* F10 reached the game */
    kpop();

    /* --- controller button/axis through the pump --- */
    memset(&me, 0, sizeof me);
    me.type = SDL_CONTROLLERBUTTONDOWN;
    me.cbutton.button = SDL_CONTROLLER_BUTTON_A;
    SDL_PushEvent(&me);
    rt_pump_events();
    bios_kbhit();
    CHECK(ZF == 0); CHECK(ax == 0x1c0d);             /* A -> Enter */
    kpop();

    memset(&mem[DS_BASE], 0, 0x1000);
    *(dw*)&mem[DS_BASE + 0xad7] = 1;
    *(dw*)&mem[DS_BASE + 0x950 + 0x4d*2] = 0x0040;   /* RIGHT -> bit6 */
    memset(&me, 0, sizeof me);
    me.type = SDL_CONTROLLERAXISMOTION;
    me.caxis.axis = SDL_CONTROLLER_AXIS_LEFTX;
    me.caxis.value = 30000;
    SDL_PushEvent(&me);
    rt_pump_events();
    CHECK(*(dw*)&mem[DS_BASE + 0xad9] == 0x0040);    /* stick -> held RIGHT */
    me.caxis.value = 0;                              /* recenter */
    SDL_PushEvent(&me);
    rt_pump_events();                                /* release is deferred */
    for (int i = 0; i < rel_n; i++) rel_pend[i].at = 0;
    rel_expire();
    CHECK(*(dw*)&mem[DS_BASE + 0xad9] == 0);

    /* --- AC_BACK: short press = Esc, hold = mapping menu --- */
    memset(&mem[DS_BASE], 0, 0x1000);
    *(dw*)&mem[DS_BASE + 0xad7] = 0;
    khead = ktail = 0;
    memset(&me, 0, sizeof me);
    me.type = SDL_KEYDOWN; me.key.keysym.scancode = SDL_SCANCODE_AC_BACK;
    SDL_PushEvent(&me);
    rt_pump_events();
    CHECK(khead == ktail);                           /* Esc deferred to keyup */
    back_dn = SDL_GetTicks() - 800;                  /* simulate >700ms hold */
    rt_pump_events();
    CHECK(rt_kmap_open() == 1);                      /* hold opened the menu */
    me.type = SDL_KEYUP;
    SDL_PushEvent(&me);
    rt_pump_events();
    CHECK(khead == ktail);                           /* no Esc after hold-open */
    kmap_toggle(0);
    CHECK(rt_kmap_open() == 0);

    me.type = SDL_KEYDOWN; SDL_PushEvent(&me);       /* short press -> Esc */
    me.type = SDL_KEYUP; SDL_PushEvent(&me);
    rt_pump_events();
    bios_kbhit();
    CHECK(ZF == 0); CHECK(ax == 0x011b);
    kpop();

    /* --- controller default map: every button -> its XT key --- */
    {   static const struct { int b; dw key; } cmap[] = {
            {SDL_CONTROLLER_BUTTON_A,           0x1c0d},
            {SDL_CONTROLLER_BUTTON_START,       0x1c0d},
            {SDL_CONTROLLER_BUTTON_B,           0x011b},
            {SDL_CONTROLLER_BUTTON_BACK,        0x011b},
            {SDL_CONTROLLER_BUTTON_X,           0x3920},
            {SDL_CONTROLLER_BUTTON_Y,           0x4c00},
            {SDL_CONTROLLER_BUTTON_DPAD_UP,     0x4800},
            {SDL_CONTROLLER_BUTTON_DPAD_DOWN,   0x5000},
            {SDL_CONTROLLER_BUTTON_DPAD_LEFT,   0x4b00},
            {SDL_CONTROLLER_BUTTON_DPAD_RIGHT,  0x4d00},
            {SDL_CONTROLLER_BUTTON_LEFTSHOULDER,  0x0c2d},
            {SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, 0x0d3d},
        };
        memset(&mem[DS_BASE], 0, 0x1000);
        *(dw*)&mem[DS_BASE + 0xad7] = 0;
        khead = ktail = 0;
        for (size_t i = 0; i < sizeof cmap / sizeof *cmap; i++){
            memset(&me, 0, sizeof me);
            me.type = SDL_CONTROLLERBUTTONDOWN;
            me.cbutton.button = cmap[i].b;
            SDL_PushEvent(&me);
            rt_pump_events();
            bios_kbhit();
            CHECK(ZF == 0); CHECK(ax == cmap[i].key);
            kpop();
        }
        me.type = SDL_CONTROLLERBUTTONUP;              /* release path */
        me.cbutton.button = SDL_CONTROLLER_BUTTON_A;
        SDL_PushEvent(&me);
        rt_pump_events();
        bios_kbhit(); CHECK(ZF == 1);                  /* releases don't queue */
    }

    /* --- controller axes: both directions both sticks, then recenter --- */
    memset(&mem[DS_BASE], 0, 0x1000);
    *(dw*)&mem[DS_BASE + 0xad7] = 1;
    for (int s = 0x48; s <= 0x50; s++)
        *(dw*)&mem[DS_BASE + 0x950 + s*2] = 1 << (s - 0x48); /* distinct bits */
    mgx = -1; mgy = -1;                                  /* mouse zones off */
    memset(&me, 0, sizeof me);
    me.type = SDL_CONTROLLERAXISMOTION;
    me.caxis.axis = SDL_CONTROLLER_AXIS_LEFTX;
    me.caxis.value = -30000;                             /* LEFT */
    SDL_PushEvent(&me); rt_pump_events();
    CHECK((*(dw*)&mem[DS_BASE + 0xad9] & 8) != 0);         /* 0x4b -> bit3 */
    me.caxis.value = 30000;                              /* LEFT -> RIGHT */
    SDL_PushEvent(&me); rt_pump_events();
    for (int i = 0; i < rel_n; i++) rel_pend[i].at = 0;
    rel_expire();
    CHECK((*(dw*)&mem[DS_BASE + 0xad9] & 8) == 0);         /* LEFT released */
    CHECK((*(dw*)&mem[DS_BASE + 0xad9] & 0x20) != 0);      /* 0x4d -> bit5 */
    me.caxis.axis = SDL_CONTROLLER_AXIS_LEFTY;
    me.caxis.value = -30000;                             /* UP */
    SDL_PushEvent(&me); rt_pump_events();
    CHECK((*(dw*)&mem[DS_BASE + 0xad9] & 1) != 0);         /* 0x48 -> bit0 */
    me.caxis.axis = SDL_CONTROLLER_AXIS_RIGHTX;            /* untracked axis */
    SDL_PushEvent(&me); rt_pump_events();

    /* --- controller hotplug event: opens the pad (NULL which -> no crash) --- */
    memset(&me, 0, sizeof me);
    me.type = SDL_CONTROLLERDEVICEADDED;
    me.cdevice.which = 99;
    SDL_PushEvent(&me); rt_pump_events();

    /* --- mouse motion + buttons through the pump --- */
    memset(&mem[DS_BASE], 0, 0x1000);
    *(dw*)&mem[0xf340] = 0;                             /* not POD */
    win_w = 960; win_h = 600; mouse_on = -1;
    memset(&me, 0, sizeof me);
    me.type = SDL_MOUSEMOTION;
    me.motion.x = 480; me.motion.y = 300;
    SDL_PushEvent(&me); rt_pump_events();
    CHECK(mgx == 160 && mgy == 100);                    /* scaled to guest res */
    me.motion.x = 99999; me.motion.y = -5;
    SDL_PushEvent(&me); rt_pump_events();
    CHECK(mgx == 319 && mgy == 0);                      /* clamped */
    *(dw*)&mem[0xf340] = 5;                             /* POD: motion moves cursor */
    me.motion.x = 480; me.motion.y = 150;
    SDL_PushEvent(&me); rt_pump_events();
    CHECK(*(dw*)&mem[0xf71f] != 0 || mem[0xf71f+1] != 0 ||
          *(dw*)&mem[0xf75f] != 0);
    *(dw*)&mem[0xf340] = 0;
    mgx = -1; mgy = -1;                                 /* zone step off */
    me.type = SDL_MOUSEBUTTONDOWN;                      /* left click -> Enter */
    me.button.button = SDL_BUTTON_LEFT;
    me.button.x = 480; me.button.y = 300;
    khead = ktail = 0;
    SDL_PushEvent(&me); rt_pump_events();
    bios_kbhit();
    CHECK(ZF == 0); CHECK(ax == 0x1c0d); kpop();
    me.button.button = SDL_BUTTON_RIGHT;                /* right click -> Esc */
    SDL_PushEvent(&me); rt_pump_events();
    bios_kbhit();
    CHECK(ZF == 0); CHECK(ax == 0x011b); kpop();

    /* --- bios_kbd dispatch: AH=0 getch, AH=1 kbhit, AH=2 shift flags --- */
    khead = ktail = 0; kpush(0x1e61);
    bios_kbd(0x0000); CHECK(ax == 0x1e61);              /* getch pops it */
    khead = ktail = 0;
    bios_kbd(0x0100); CHECK(ZF == 1);                   /* empty -> ZF */
    al = 0xee; bios_kbd(0x0200); CHECK(al == 0);        /* shift flags -> 0 */

    /* --- rt_kmap_exclusive: M2C_KMAPSTART loop exits on START GAME --- */
    {   char oldcwd[512];
        CHECK(getcwd(oldcwd, sizeof oldcwd) != NULL);
        CHECK(chdir("/tmp/ar_kmap") == 0);
        setenv("M2C_KMAPSTART", "1", 1);
        kmap_enabled = -1; kmap_on = 0;
        SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
        memset(&me, 0, sizeof me);
        me.type = SDL_KEYDOWN; me.key.keysym.scancode = SDL_SCANCODE_RETURN;
        SDL_PushEvent(&me);                             /* lands on START GAME */
        SDL_Event wv;
        memset(&wv, 0, sizeof wv);
        wv.type = SDL_WINDOWEVENT;                      /* covers resize branch */
        wv.window.event = SDL_WINDOWEVENT_RESIZED;
        wv.window.data1 = 800; wv.window.data2 = 600;
        SDL_PushEvent(&wv);
        rt_kmap_exclusive();
        CHECK(kmap_on == 0);
        CHECK(win_w == 800 && win_h == 600);
        unsetenv("M2C_KMAPSTART");
        CHECK(chdir(oldcwd) == 0);
    }

    /* --- rel_defer edges: past-deadline release + full table --- */
    memset(&mem[DS_BASE], 0, 0x1000);
    *(dw*)&mem[DS_BASE + 0xad7] = 1;
    *(dw*)&mem[DS_BASE + 0x950 + 0x4d*2] = 0x40;
    int9_update(0x4d, 1);                             /* sets held + down_at */
    down_at[0x4d] = SDL_GetTicks() - MIN_HOLD - 1;    /* deadline already past */
    rel_n = 0;
    rel_defer(0x4d);                                  /* deadline passed -> now */
    CHECK(rel_n == 0);
    CHECK(!(*(dw*)&mem[DS_BASE + 0xad9] & 0x40));
    int9_update(0x4d, 1);                             /* fresh press */
    down_at[0x4d] = SDL_GetTicks();                   /* deadline in the future */
    for (int i = 0; i < 16; i++){                     /* table full, none due */
        rel_pend[i].scan = 0x48;
        rel_pend[i].at = SDL_GetTicks() + 60000;
    }
    rel_n = 16;
    rel_defer(0x4d);                                  /* releases immediately */
    CHECK(rel_n == 16);
    CHECK(!(*(dw*)&mem[DS_BASE + 0xad9] & 0x40));
    rel_n = 1; rel_pend[0].scan = 0x4d;
    rel_pend[0].at = SDL_GetTicks() + 60000;          /* not due yet */
    int9_update(0x4d, 1);
    rel_expire();                                     /* walks, removes nothing */
    CHECK(rel_n == 1 && (*(dw*)&mem[DS_BASE + 0xad9] & 0x40));
    rel_n = 0; int9_update(0x4d, 0);

    /* --- zone step: divert dropped mid-drag releases the held scan --- */
    memset(&mem[DS_BASE], 0, 0x1000);
    *(dw*)&mem[DS_BASE + 0xad7] = 1;
    *(dw*)&mem[DS_BASE + 0x950 + 0x4d*2] = 0x40;
    *(dw*)&mem[0xf340] = 0;
    mgx = 300; mgy = 100; mouse_on = -1;
    rt_pump_events();                                 /* holds RIGHT */
    CHECK(*(dw*)&mem[DS_BASE + 0xad9] & 0x40);
    *(dw*)&mem[DS_BASE + 0xad7] = 0;                  /* divert drops */
    khead = ktail = 0;
    rt_pump_events();                                 /* releases + discrete push */
    CHECK(!(*(dw*)&mem[DS_BASE + 0xad9] & 0x40));
    CHECK(khead != ktail);                            /* press+release queued */
    khead = ktail = 0;
    rt_pump_events();                                 /* within repeat window: idle */
    CHECK(khead == ktail);
    SDL_Delay(360);                                   /* past the 350ms repeat */
    rt_pump_events();                                 /* repeat fires again */
    CHECK(khead != ktail);
    khead = ktail = 0; mgx = -1; mgy = -1;

    /* --- pod_abs_cursor debug env + bios_getch blocking loop --- */
    setenv("M2C_MOUSEDBG", "1", 1);
    *(dw*)&mem[0xf340] = 5;
    pod_abs_cursor(480, 150);
    *(dw*)&mem[0xf340] = 0;
    unsetenv("M2C_MOUSEDBG");
    khead = ktail = 0; ctrl_done = 0;
    *(dw*)&mem[0xf340] = 8;                           /* control-device menu */
    setenv("M2C_CTRL", "2", 1);
    bios_getch();                                     /* loops once, autopick feeds */
    CHECK(ax == 0x0332);                              /* '2' as a BIOS keycode */
    *(dw*)&mem[0xf340] = 0;
    unsetenv("M2C_CTRL");

    /* --- debug/env-gated pump branches --- */
    memset(&mem[DS_BASE], 0, 0x1000);
    *(dw*)&mem[DS_BASE + 0xad7] = 1;
    *(dw*)&mem[DS_BASE + 0x950 + 0x48*2] = 1;         /* UP diverted */
    tnd_base = 0x100;
    *(dw*)&mem[tnd_base + 0x380] = 7;                 /* table live */
    rt_pump_events();
    *(dw*)&mem[tnd_base + 0x380] = 0;                 /* table zeroed -> TBL warn */
    setenv("M2C_HOLDWALK", "3f", 1); mem[0x1c3ea] = 1;
    setenv("M2C_SEEDPOS", "1", 1); *(dw*)&mem[0xf33e] = 1;
    setenv("M2C_KEYDBG", "1", 1); setenv("M2C_MOUSEDBG", "1", 1);
    memset(&me, 0, sizeof me);
    me.type = SDL_KEYDOWN;                            /* diverted key down+up */
    me.key.keysym.scancode = SDL_SCANCODE_UP;
    SDL_PushEvent(&me);
    me.type = SDL_KEYUP; SDL_PushEvent(&me);
    me.type = SDL_MOUSEBUTTONDOWN;                    /* mouse btn (debug out) */
    me.button.button = SDL_BUTTON_LEFT;
    me.button.x = me.button.y = 10;
    SDL_PushEvent(&me);
    me.type = SDL_MOUSEBUTTONUP; SDL_PushEvent(&me);
    khead = ktail = 0;
    rt_pump_events();                                 /* runs all debug blocks */
    CHECK(mem[0x1c3ea] == 1 && mem[0x1c3f3] == 1);    /* SEEDPOS applied */
    CHECK(*(dw*)&mem[DS_BASE + 0xad9] == 0x3f);       /* HOLDWALK mask applied */
    tnd_base = 0; mem[0x1c3ea] = 0; *(dw*)&mem[0xf33e] = 0;
    unsetenv("M2C_HOLDWALK"); unsetenv("M2C_SEEDPOS");
    unsetenv("M2C_KEYDBG"); unsetenv("M2C_MOUSEDBG");
    rel_n = 0;

    /* --- GUIDE button opens the mapping menu --- */
    memset(&mem[DS_BASE], 0, 0x1000);
    kmap_enabled = 1; kmap_on = 0;
    memset(&me, 0, sizeof me);
    me.type = SDL_CONTROLLERBUTTONDOWN;
    me.cbutton.button = SDL_CONTROLLER_BUTTON_GUIDE;
    SDL_PushEvent(&me); rt_pump_events();
    CHECK(rt_kmap_open() == 1);
    me.cbutton.button = SDL_CONTROLLER_BUTTON_B;      /* B closes */
    SDL_PushEvent(&me); rt_pump_events();
    CHECK(rt_kmap_open() == 0);

    /* --- kmap: PAD bind name, controller-button capture + playback --- */
    {   char oldcwd[512];
        CHECK(getcwd(oldcwd, sizeof oldcwd) != NULL);
        CHECK(chdir("/tmp/ar_kmap") == 0);
        binds[2].kind = 2; binds[2].code = SDL_CONTROLLER_BUTTON_Y;
        const char *bn = rt_kmap_bind(2);
        CHECK(bn && bn[0] == 'P' && bn[1] == 'A' && bn[2] == 'D');
        CHECK(rt_kmap_bind(-1)[0] == '?');
        CHECK(rt_kmap_bind(N_KMAP - 1)[0] == 0);       /* action row: no bind */
        kmap_on = 1; kmap_sel = 3; kmap_cap = 1;
        memset(&me, 0, sizeof me);
        me.type = SDL_MOUSEMOTION;                    /* non-key event in cap */
        CHECK(kmap_event(&me) == 1);                  /* swallowed */
        me.type = SDL_CONTROLLERBUTTONDOWN;
        me.cbutton.button = SDL_CONTROLLER_BUTTON_X;
        CHECK(kmap_event(&me) == 1);                  /* captured as kind=2 */
        CHECK(binds[3].kind == 2 &&
              binds[3].code == SDL_CONTROLLER_BUTTON_X);
        kmap_on = 0; kmap_cap = 0;
        /* playback: that pad button now injects doskeys[3] (RIGHT) */
        memset(&mem[DS_BASE], 0, 0x1000);
        khead = ktail = 0;
        SDL_PushEvent(&me); rt_pump_events();         /* same event, menu closed */
        bios_kbhit();
        CHECK(ZF == 0 && ax == 0x4d00);
        kpop();
        /* capture-mode axis repeat + stick direction change */
        kmap_on = 1; kmap_sel = 5;
        memset(&me, 0, sizeof me);
        me.type = SDL_CONTROLLERAXISMOTION;
        me.caxis.axis = SDL_CONTROLLER_AXIS_LEFTY;
        me.caxis.value = 30000;
        CHECK(kmap_event(&me) == 1);
        CHECK(kmap_sel == 6);
        CHECK(kmap_event(&me) == 1);                  /* held: repeat pending */
        me.caxis.value = 0;                           /* stick re-centers */
        CHECK(kmap_event(&me) == 1);
        kmap_on = 0;
        CHECK(chdir(oldcwd) == 0);
        kmap_defaults();
    }

    fprintf(stderr, "test_input: %d checks, %d failures\n", checks, fails);
    return fails ? 1 : 0;
}
