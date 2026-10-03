/* keyboard: SDL events -> BIOS int16h queue + int9 ISR dispatch.
 * DOS int16h returns AX = (scancode<<8) | ascii.
 */
#include <SDL2/SDL.h>
#include "rt.h"

#define QN 64
static dw kq[QN]; static int khead, ktail;
static db port60;

static void kpush(dw v){ int n=(khead+1)%QN; if(n!=ktail){ kq[khead]=v; khead=n; } }
static int kpop(void){ if(khead==ktail) return -1; int v=kq[ktail]; ktail=(ktail+1)%QN; return v; }

/* SDL scancode -> XT set-1 scancode (DOS-era keys used by AR) */
static int xt_scan(SDL_Scancode s){
    switch (s){
    case SDL_SCANCODE_ESCAPE: return 0x01;
    case SDL_SCANCODE_AC_BACK: return 0x01;   /* Android TV remote BACK -> Esc */
    case SDL_SCANCODE_1: return 0x02; case SDL_SCANCODE_2: return 0x03;
    case SDL_SCANCODE_3: return 0x04; case SDL_SCANCODE_4: return 0x05;
    case SDL_SCANCODE_5: return 0x06; case SDL_SCANCODE_6: return 0x07;
    case SDL_SCANCODE_7: return 0x08; case SDL_SCANCODE_8: return 0x09;
    case SDL_SCANCODE_9: return 0x0a; case SDL_SCANCODE_0: return 0x0b;
    case SDL_SCANCODE_MINUS: return 0x0c; case SDL_SCANCODE_EQUALS: return 0x0d;
    case SDL_SCANCODE_BACKSPACE: return 0x0e; case SDL_SCANCODE_TAB: return 0x0f;
    case SDL_SCANCODE_Q: return 0x10; case SDL_SCANCODE_W: return 0x11;
    case SDL_SCANCODE_E: return 0x12; case SDL_SCANCODE_R: return 0x13;
    case SDL_SCANCODE_T: return 0x14; case SDL_SCANCODE_Y: return 0x15;
    case SDL_SCANCODE_U: return 0x16; case SDL_SCANCODE_I: return 0x17;
    case SDL_SCANCODE_O: return 0x18; case SDL_SCANCODE_P: return 0x19;
    case SDL_SCANCODE_LEFTBRACKET: return 0x1a; case SDL_SCANCODE_RIGHTBRACKET: return 0x1b;
    case SDL_SCANCODE_RETURN: return 0x1c; case SDL_SCANCODE_LCTRL: return 0x1d;
    case SDL_SCANCODE_A: return 0x1e; case SDL_SCANCODE_S: return 0x1f;
    case SDL_SCANCODE_D: return 0x20; case SDL_SCANCODE_F: return 0x21;
    case SDL_SCANCODE_G: return 0x22; case SDL_SCANCODE_H: return 0x23;
    case SDL_SCANCODE_J: return 0x24; case SDL_SCANCODE_K: return 0x25;
    case SDL_SCANCODE_L: return 0x26; case SDL_SCANCODE_SEMICOLON: return 0x27;
    case SDL_SCANCODE_APOSTROPHE: return 0x28; case SDL_SCANCODE_GRAVE: return 0x29;
    case SDL_SCANCODE_LSHIFT: return 0x2a; case SDL_SCANCODE_BACKSLASH: return 0x2b;
    case SDL_SCANCODE_Z: return 0x2c; case SDL_SCANCODE_X: return 0x2d;
    case SDL_SCANCODE_C: return 0x2e; case SDL_SCANCODE_V: return 0x2f;
    case SDL_SCANCODE_B: return 0x30; case SDL_SCANCODE_N: return 0x31;
    case SDL_SCANCODE_M: return 0x32; case SDL_SCANCODE_COMMA: return 0x33;
    case SDL_SCANCODE_PERIOD: return 0x34; case SDL_SCANCODE_SLASH: return 0x35;
    case SDL_SCANCODE_RSHIFT: return 0x36; case SDL_SCANCODE_KP_MULTIPLY: return 0x37;
    case SDL_SCANCODE_LALT: return 0x38; case SDL_SCANCODE_SPACE: return 0x39;
    case SDL_SCANCODE_CAPSLOCK: return 0x3a;
    case SDL_SCANCODE_F1: return 0x3b; case SDL_SCANCODE_F2: return 0x3c;
    case SDL_SCANCODE_F3: return 0x3d; case SDL_SCANCODE_F4: return 0x3e;
    case SDL_SCANCODE_F5: return 0x3f; case SDL_SCANCODE_F6: return 0x40;
    case SDL_SCANCODE_F7: return 0x41; case SDL_SCANCODE_F8: return 0x42;
    case SDL_SCANCODE_F9: return 0x43; case SDL_SCANCODE_F10: return 0x44;
    case SDL_SCANCODE_NUMLOCKCLEAR: return 0x45; case SDL_SCANCODE_SCROLLLOCK: return 0x46;
    case SDL_SCANCODE_KP_7: return 0x47; case SDL_SCANCODE_KP_8: return 0x48;
    case SDL_SCANCODE_KP_9: return 0x49; case SDL_SCANCODE_KP_MINUS: return 0x4a;
    case SDL_SCANCODE_KP_4: return 0x4b; case SDL_SCANCODE_KP_5: return 0x4c;
    case SDL_SCANCODE_KP_6: return 0x4d; case SDL_SCANCODE_KP_PLUS: return 0x4e;
    case SDL_SCANCODE_KP_1: return 0x4f; case SDL_SCANCODE_KP_2: return 0x50;
    case SDL_SCANCODE_KP_3: return 0x51; case SDL_SCANCODE_KP_0: return 0x52;
    case SDL_SCANCODE_KP_PERIOD: return 0x53;
    case SDL_SCANCODE_UP: return 0x48; case SDL_SCANCODE_DOWN: return 0x50;
    case SDL_SCANCODE_LEFT: return 0x4b; case SDL_SCANCODE_RIGHT: return 0x4d;
    case SDL_SCANCODE_HOME: return 0x47; case SDL_SCANCODE_END: return 0x4f;
    case SDL_SCANCODE_PAGEUP: return 0x49; case SDL_SCANCODE_PAGEDOWN: return 0x51;
    case SDL_SCANCODE_INSERT: return 0x52; case SDL_SCANCODE_DELETE: return 0x53;
    default: return 0;
    }
}

/* ascii for ascii-keyed reads (text entry screens) */
static char ascii_of(SDL_Keycode k){
    if (k >= 'a' && k <= 'z') return k;
    if (k >= '0' && k <= '9') return k;
    if (k == ' ') return ' ';
    if (k == '\r') return '\r';
    if (k == 27) return 27;
    return 0;
}

/* Replica of the game's INT 9 ISR (seg000:0x25a1 — data bytes only in the image).
 * When word_1D957 (dataseg+0xad7)==1 the handler owns the keyboard: it looks up a
 * per-scancode held-key bitmask at dataseg+0x950 and ORs/ANDs it into word_1D959
 * (dataseg+0xad9). Data segment is seg002 = para 0xe8a (mem+0xe8a0).
 * Returns 1 if the key was diverted to the bitmask (not a BIOS-buffer key). */
#define DS_BASE 0xe8a0
static int int9_update(int scan, int pressed){
    if (*(dw*)&mem[DS_BASE + 0xad7] != 1) return 0;
    dw bit = *(dw*)&mem[DS_BASE + 0x950 + (scan & 0x7f) * 2];
    if (!bit) return 0;
    dw *held = (dw*)&mem[DS_BASE + 0xad9];
    if (pressed) *held = (dw)(*held | bit);
    else         *held = (dw)(*held & ~bit);
    return 1;
}

/* The game's diverted keys land in a held-bitmask which menu/screens sample
 * from inside a tick-gated polling loop (~55ms windows, int1c-paced). A real
 * key tap shorter than one event-pump interval would set+clear the bit within
 * a single SDL_PollEvent drain — invisible to the guest. Diverted releases are
 * therefore deferred so every press stays visible for at least MIN_HOLD ms. */
#define MIN_HOLD 60
static struct { int scan; Uint32 at; } rel_pend[16];
static int rel_n;
static Uint32 down_at[128];                      /* last KEYDOWN tick per scan */

static void rel_expire(void){
    Uint32 now = SDL_GetTicks();
    int i = 0;
    while (i < rel_n){
        if ((Sint32)(now - rel_pend[i].at) >= 0){
            int9_update(rel_pend[i].scan, 0);
            rel_pend[i] = rel_pend[--rel_n];
        } else i++;
    }
}
static void rel_defer(int scan){
    rel_expire();
    Uint32 deadline = down_at[scan & 0x7f] + MIN_HOLD;
    if ((Sint32)(SDL_GetTicks() - deadline) >= 0){ int9_update(scan, 0); return; }
    if (rel_n < 16){ rel_pend[rel_n].scan = scan; rel_pend[rel_n].at = deadline; rel_n++; }
    else int9_update(scan, 0);                    /* table full: release now */
}
static void rel_cancel(int scan){
    for (int i = 0; i < rel_n; i++)
        if (rel_pend[i].scan == scan){ rel_pend[i] = rel_pend[--rel_n]; return; }
}

/* ---- mouse -> keyboard emulation --------------------------------------
 * The DOS original is keyboard-only (no int 33h): menus navigate with the
 * arrow keys and confirm with Enter, so mouse input is folded into the
 * same int9/int16 path a physical keypress uses. Motion accumulates into a
 * pending pixel delta that rt_pump_events folds into arrow keys once per
 * pump — diverted (held-mask) screens get the arrow's bitmask bit held,
 * int16 screens get discrete keycodes. Left button = Enter (select),
 * right button = Escape (back). Set M2C_MOUSE=0 to disable. */
static int mouse_on = -1;                    /* lazy: -1 unknown, else 0/1 */
static int macc_x, macc_y;                   /* pending deltas (window px) */
static int win_w = 960, win_h = 600;         /* window px; window is VW*3 x VH*3 */

static int mouse_enabled(void){
    if (mouse_on < 0){
        const char *v = getenv("M2C_MOUSE");
        mouse_on = !(v && *v == '0');
    }
    return mouse_on;
}

/* Force-clear a held-mask bit (bypasses the int9 divert-flag check — used to
 * drop a mouse-held bit even if the screen has since left divert mode). */
static void mrelease(int scan){
    *(dw*)&mem[DS_BASE + 0xad9] &= ~*(dw*)&mem[DS_BASE + 0x950 + (scan & 0x7f) * 2];
}

/* One keypress through the same diverted/kpush path as a physical key:
 * diverted keys set their held-mask bit (release deferred so the guest's
 * tick-gated sampler can't miss it); plain keys queue an int16 keycode. */
static void synth_key(int xs, db a, int down){
    if (down){
        rel_cancel(xs & 0x7f);
        down_at[xs & 0x7f] = SDL_GetTicks();
        port60 = xs;
        if (!int9_update(xs & 0x7f, 1))
            kpush((dw)((xs << 8) | a));
    } else {
        port60 = xs | 0x80;
        if (*(dw*)&mem[DS_BASE + 0xad7] == 1 &&
            *(dw*)&mem[DS_BASE + 0x950 + (xs & 0x7f) * 2])
            rel_defer(xs & 0x7f);
    }
    /* non-diverted releases queue nothing (BIOS buffers only presses) */
}

/* Fold accumulated mouse deltas into arrow keys. Runs once per pump so the
 * guest samples each held bit inside its own polling window. */
static void mouse_dir_step(void){
    static int mhx, mhy;                     /* arrow scans held by the mouse */
    if (!mouse_enabled()){ macc_x = macc_y = 0; return; }
    if (*(dw*)&mem[DS_BASE + 0xad7] == 1){
        /* Held-mask screens (POD, gameplay): hold a direction bit while delta
         * is pending. The cursor moves ~4 game-px per guest poll = ~12 window
         * px on the 3x window, so drain 12/poll and it tracks the mouse ~1:1. */
        const int DRAIN = 12;
        int hx = 0, hy = 0;
        if (macc_x > 0){ hx = 0x4d; macc_x = macc_x > DRAIN ? macc_x - DRAIN : 0; }
        else if (macc_x < 0){ hx = 0x4b; macc_x = macc_x < -DRAIN ? macc_x + DRAIN : 0; }
        if (macc_y > 0){ hy = 0x50; macc_y = macc_y > DRAIN ? macc_y - DRAIN : 0; }
        else if (macc_y < 0){ hy = 0x48; macc_y = macc_y < -DRAIN ? macc_y + DRAIN : 0; }
        if (hx != mhx){ if (mhx) mrelease(mhx); if (hx) int9_update(hx, 1); mhx = hx; }
        if (hy != mhy){ if (mhy) mrelease(mhy); if (hy) int9_update(hy, 1); mhy = hy; }
    } else {
        if (mhx){ mrelease(mhx); mhx = 0; }  /* divert dropped mid-drag: release */
        if (mhy){ mrelease(mhy); mhy = 0; }
        const int STEP = 20;                 /* window px of motion per item step */
        while (macc_x >=  STEP){ synth_key(0x4d, 0, 1); synth_key(0x4d, 0, 0); macc_x -= STEP; }
        while (macc_x <= -STEP){ synth_key(0x4b, 0, 1); synth_key(0x4b, 0, 0); macc_x += STEP; }
        while (macc_y >=  STEP){ synth_key(0x50, 0, 1); synth_key(0x50, 0, 0); macc_y -= STEP; }
        while (macc_y <= -STEP){ synth_key(0x48, 0, 1); synth_key(0x48, 0, 0); macc_y += STEP; }
    }
}

/* POD cursor screens: drop the guest crosshair at the absolute pointer
 * position (window px -> 320x200 guest px, clamped to the cursor bounds). */
static void pod_abs_cursor(int wx, int wy){
    int gx = wx * 320 / (win_w > 0 ? win_w : 960);
    int gy = wy * 200 / (win_h > 0 ? win_h : 600);
    if (gx < 0) gx = 0; if (gx > 0x138) gx = 0x138;
    if (gy < 0) gy = 0; if (gy > 0xC4)  gy = 0xC4;
    *(db*)&mem[0xf71f] = (db)gx;         /* word_1DCFF (cursor X lo) */
    *(db*)&mem[0xf73f] = (db)(gx >> 8);  /* word_1DD1F (cursor X hi) */
    *(db*)&mem[0xf75f] = (db)gy;         /* word_1DD3F (cursor Y)    */
    macc_x = macc_y = 0;
    if (getenv("M2C_MOUSEDBG"))
        fprintf(stderr, "mouse abs win=%d,%d -> cur=%d,%d\n", wx, wy, gx, gy);
}

/* scripted key feeder for headless testing: M2C_KEYS="1245 " drips one
 * keypress per M2C_KEYS_DELAY pump calls. Escapes: \r enter \e esc
 * \u \d \l \r2 arrows (up/down/left/right). Each key press is followed by
 * a release so the int9 held-bitmask behaves like real typing. */
static int script_scan(char c){
    switch (c){
    case ' ': return 0x39;
    case '\r': case '\n': return 0x1c;
    case 27: return 0x01;
    }
    if (c >= '1' && c <= '9') return c - '1' + 2;
    if (c == '0') return 0x0b;
    if (c >= 'a' && c <= 'z'){
        const char *r = "qwertyuiop\0\0asdfghjkl;\0zxcvbnm,./";
        const char *p = strchr(r, c);
        if (p) return (int)(p - r) + 0x10;
    }
    return 0x39;
}
static db script_ascii(char c){
    if (c == ' ') return 0x20;
    if (c == '\r' || c == '\n') return 0x0d;
    if (c == 27) return 0x1b;
    if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) return c;
    return 0;
}
static const char *script_key(const char *s, int *scan, db *ascii){
    if (s[0] == '\\' && s[1]){
        switch (s[1]){
        case 'r': case 'n': *scan = 0x1c; *ascii = 0x0d; return s + 2;
        case 'e': *scan = 0x01; *ascii = 0x1b; return s + 2;
        case 'u': *scan = 0x48; *ascii = 0; return s + 2;
        case 'd': *scan = 0x50; *ascii = 0; return s + 2;
        case 'l': *scan = 0x4b; *ascii = 0; return s + 2;
        case 'R': *scan = 0x4d; *ascii = 0; return s + 2;
        case 'H': *scan = -3; *ascii = 0; return s + 2;      /* hold-next-key flag */
        case 'F': *scan = 0x43; *ascii = 0; return s + 2;    /* F9 -> map overlay */
        case '5': *scan = 0x4c; *ascii = 0; return s + 2;    /* KP5 -> fire/divert bit4 */
        }
    }
    if (*s == '.'){ *scan = -1; *ascii = 0; return s + 1; }   /* pause one delay window */
    *scan = script_scan(*s); *ascii = script_ascii(*s);
    return s + 1;
}
static int kbd_polls;                            /* bumped by bios_kbhit/bios_getch */
extern dd tnd_base;

/* Scripted-key feeder step — pure guest-memory ops (int9 mask / kq push), safe
 * to call from the SDL timer thread as well as the main-thread event pump.
 * Steps are cadence-based: callers invoke it once per pump/tick. */
void rt_script_feed(void){
    static const char *script; static int sdel;
    static int pend_scan = -1;
    static int wait_poll0 = -1;
    static int hold_next = 0;
    static SDL_mutex *mx;
    if (!mx) mx = SDL_CreateMutex();
    SDL_LockMutex(mx);
    if (!script){ script = getenv("M2C_KEYS"); sdel = atoi(getenv("M2C_KEYS_DELAY") ?: "20"); }
    int delay = atoi(getenv("M2C_KEYS_DELAY") ?: "20");
    if (getenv("M2C_KTRACE")){ static int lp; if (++lp % 400 == 0)
        fprintf(stderr, "fs: polls=%d pend=%d sdel=%d next=%c\n", kbd_polls, pend_scan, sdel, script && *script ? *script : '-'); }
    if (pend_scan >= 0){                        /* release the pending press */
        if (getenv("M2C_MASKDUMP"))
            fprintf(stderr, "hold x=%04x y=%04x held=%04x in=%04x ca2=%04x "
                    "gate=%d tick=%d\n",
                    *(dw*)&mem[0xf71f], *(dw*)&mem[0xf75f],
                    *(dw*)&mem[DS_BASE + 0xad9], *(dw*)&mem[0x18804],
                    *(dw*)&mem[0xf6c2],   /* word_1DCA2 */
                    *(dw*)&mem[0xf36d],   /* word_1D94D sampler gate */
                    *(dw*)&mem[0xf102]);  /* word_1D6E2 int1c tick count */
        if (++sdel >= delay){                   /* hold for a full delay window */
            sdel = 0;
            int9_update(pend_scan, 0);
            pend_scan = -1;
        }
    } else if (script && script[0]){
        /* Peek at the next key: diverted keys (divert flag on + bitmask entry)
         * land in the int9 held-mask which menus sample every frame — feed them
         * without requiring int16 polls. Plain keys need an int16 reader to
         * have polled at least once since the key became pending. */
        int xs; db asc;
        const char *next = script_key(script, &xs, &asc);
        if (xs == -3){                          /* \H: next key is held, not released */
            hold_next = 1; script = next; sdel = 0; goto out;
        }
        if (xs < 0){                            /* pause token: just wait a window */
            script = next; sdel = 0; goto out;
        }
        int divert_flag = *(dw*)&mem[DS_BASE + 0xad7] == 1;
        int diverted = divert_flag &&
                       *(dw*)&mem[DS_BASE + 0x950 + (xs & 0x7f) * 2] != 0;
        if (getenv("M2C_MASKDUMP") && divert_flag){
            /* Print which scancodes the current screen consumes via the int9
             * held-mask (the "divert" table), once per mask-content change. */
            static uint64_t last_mh; static char buf[1024];
            uint64_t mh = 1469598103934665603ULL;
            for (int i = 0; i < 0x80; i++)
                mh = (mh ^ *(dw*)&mem[DS_BASE + 0x950 + i*2]) * 1099511628211ULL;
            if (mh != last_mh){
                last_mh = mh; int n = 0;
                for (int i = 0; i < 0x80; i++){
                    dw v = *(dw*)&mem[DS_BASE + 0x950 + i*2];
                    if (v) n += snprintf(buf+n, sizeof(buf)-n, "%02x:%x ", i, v);
                }
                fprintf(stderr, "mask: %s\n", buf);
            }
            fprintf(stderr, "cur x=%04x y=%04x held=%04x in=%04x "
                    "s0x=%04x s0y=%04x s0i=%04x s0f=%04x s0e=%02x m=%08lx\n",
                    *(dw*)&mem[0xf71f], *(dw*)&mem[0xf75f],
                    *(dw*)&mem[DS_BASE + 0xad9], *(dw*)&mem[0x18804],
                    *(dw*)&mem[DS_BASE + 0xe7f], *(dw*)&mem[DS_BASE + 0xebf],
                    *(dw*)&mem[DS_BASE + 0xedf], *(dw*)&mem[DS_BASE + 0xeff],
                    *(db*)&mem[DS_BASE + 0xdd5],
                    (unsigned long)*(dd*)&mem[DS_BASE + 0xf1f]);
        }
        static int gate_open;
        if (diverted && !gate_open && delay < 1000000){
            /* Held-mask keys are consumed by whatever screen is currently up.
             * Feeding them blind mid-transition drops them on the wrong screen:
             * wait until the rendered screen (vga + text mem) is stable for a
             * settle window, i.e. a screen that finished drawing. */
            static uint64_t last_h = (uint64_t)-1; static int settled, tried;
            uint64_t h = 1469598103934665603ULL;
            for (int i = 0; i < 64000; i += 97) h = (h ^ mem[0xa0000 + i]) * 1099511628211ULL;
            for (int i = 0; i < 4000; i += 53)  h = (h ^ mem[0xb8000 + i]) * 1099511628211ULL;
            /* Animated screens never hash-stabilize: after a bounded number
             * of retries, stop waiting and feed anyway. */
            if (h != last_h){ last_h = h; settled = 0;
                if (++tried < delay * 4){ sdel = 0; goto out; }
            } else if (++settled < delay){ sdel = 0; goto out; }
            if (getenv("M2C_KTRACE")) fprintf(stderr, "settle ok tried=%d\n", tried);
            tried = 0; settled = 0; gate_open = 1;
        }
        if (!diverted && delay < 1000000){
            if (wait_poll0 < 0) wait_poll0 = kbd_polls;
            if (kbd_polls == wait_poll0){       /* no reader has polled yet */
                static int dbg;
                if (getenv("M2C_KTRACE") && ++dbg % 500 == 0)
                    fprintf(stderr, "stall: polls=%d next=%02x\n", kbd_polls, xs);
                sdel = 0;
                goto out;
            }
        }
        if (++sdel >= delay){
            sdel = 0;
            script = next;
            wait_poll0 = -1;
            gate_open = 0;
            int d = int9_update(xs, 1);
            if (getenv("M2C_KTRACE")) fprintf(stderr, "feed %02x div=%d scr=%x\n", xs, d,
                                              *(dw*)&mem[DS_BASE + 0xa94]);
            if (!d) kpush((dw)((xs << 8) | asc));
            pend_scan = hold_next ? -1 : xs;    /* \H: press stays held, no release */
            hold_next = 0;
        }
    }
out:
    SDL_UnlockMutex(mx);
}

void rt_pump_events(void){
    /* open every attached gamepad once (pads are enumerated at SDL init;
     * hot-plug arrives via SDL_CONTROLLERDEVICEADDED in the event loop) */
    {   static int pad_init;
        if (!pad_init){ pad_init = 1;
            for (int i = 0; i < SDL_NumJoysticks(); i++)
                if (SDL_IsGameController(i)) SDL_GameControllerOpen(i); } }
    static int tbl_ok = -1;
    if (tnd_base){
        unsigned v = *(dw*)&mem[tnd_base+0x380];
        if (tbl_ok < 0) tbl_ok = (v != 0);
        else if (tbl_ok && v == 0){
            tbl_ok = 0;
            fprintf(stderr,"TBL zeroed! ds=%x es=%x di=%x si=%x cs1a2=%x\n",
                    ds, es, di, si, cs);
        }
    }
    rt_script_feed();
    rel_expire();
    /* DBG: M2C_HOLDWALK=<hex> forces word_1D959 dir bits only in walk phase
     * (byte_2A9CA!=0) so menus/descent aren't disturbed. dir bit set=held.
     * (M2C_HOLD in video.c ORs bits globally for menu/POD testing instead.) */
    {   static int hset=-1; static dw hv;
        if (hset<0){ const char*s=getenv("M2C_HOLDWALK"); hv=s?strtol(s,0,16):0; hset=s?1:0; }
        if (hset && mem[0x1c3ea]!=0)
            *(dw*)&mem[DS_BASE+0xad9] = (dw)((*(dw*)&mem[DS_BASE+0xad9] & ~0x3f) | (hv & 0x3f)); }
    /* DBG: M2C_SEEDPOS=1 forces the landed walk phase once the mission loop is
     * running (descent counter word_2A968 past its 0xFFF0 init), snapping the
     * ranger to a known mid-map position so direction tests aren't confounded
     * by where the scripted descent happened to land. */
    if (getenv("M2C_SEEDPOS")){
        if (*(dw*)&mem[0xf33e] != 0){            /* mission state active */
            mem[0x1c3f3] = 1;                    /* byte_2A9D3 = landed  */
            mem[0x1c3ea] = 1;                    /* byte_2A9CA = walk    */
            if (*(dw*)&mem[0xf71f] == 0){        /* re-seed until it sticks */
                mem[0xf720]=0x60; mem[0xf740]=0x00;   /* X=0x0060 -> cellX 8  */
                mem[0xf760]=0x64;                    /* Y=0x64               */
                mem[0xf71f]=0x80; mem[0xf75f]=0x80;  /* fine = mid           */
                mem[0x1c402]=0xF0;                   /* byte_2A9E2 budget    */
            }
        }
    }
    SDL_Event e;
    while (SDL_PollEvent(&e)){
        if (e.type == SDL_QUIT) rt_exit(0);
        if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED &&
            e.window.data1 > 0 && e.window.data2 > 0)
            { win_w = e.window.data1; win_h = e.window.data2; }
        if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP){
            int xs = xt_scan(e.key.keysym.scancode);
            if (!xs) continue;
            int down = e.type == SDL_KEYDOWN;
            if (down){ rel_cancel(xs & 0x7f); down_at[xs & 0x7f] = SDL_GetTicks(); }
            port60 = xs | (down ? 0 : 0x80);
            /* A diverted release is deferred (see rel_pend) so the guest's
             * tick-gated held-mask sampler can't miss sub-frame taps. A press
             * that diverted nothing still releases immediately. */
            int diverted;
            if (down) diverted = int9_update(xs & 0x7f, 1);
            else {
                diverted = *(dw*)&mem[DS_BASE + 0xad7] == 1 &&
                           *(dw*)&mem[DS_BASE + 0x950 + (xs & 0x7f) * 2] != 0;
                if (diverted) rel_defer(xs & 0x7f);
            }
            if (getenv("M2C_KEYDBG"))
                fprintf(stderr, "key %s sdl=%d xt=%02x div=%d held=%04x in=%02x "
                        "gate=%d cur=%x,%x\n",
                        down ? "dn" : "up",
                        e.key.keysym.scancode, xs, diverted,
                        *(dw*)&mem[DS_BASE + 0xad9],
                        *(db*)&mem[0x18804],
                        *(dw*)&mem[0xf36d],
                        *(db*)&mem[0xf71f], *(db*)&mem[0xf75f]);
            if (down && !diverted){
                char a = ascii_of(e.key.keysym.sym);
                kpush((dw)((xs<<8)|a));
            }
        }
        if (e.type == SDL_MOUSEMOTION && mouse_enabled()){
            /* Screens with a free pixel cursor (POD selection: word_1D920==5)
             * take the absolute pointer position — the guest cursor globals are
             * written directly, matching the original "move crosshair to point"
             * feel. Other menus stay on relative deltas -> synthesized arrows. */
            if (*(dw*)&mem[0xf340] == 5)            /* word_1D920 == POD */
                pod_abs_cursor(e.motion.x, e.motion.y);
            else {
                macc_x += e.motion.xrel; macc_y += e.motion.yrel;
                /* clamp backlog: a fast fling shouldn't queue seconds of input */
                if (macc_x >  480) macc_x =  480; else if (macc_x < -480) macc_x = -480;
                if (macc_y >  480) macc_y =  480; else if (macc_y < -480) macc_y = -480;
                if (getenv("M2C_MOUSEDBG"))
                    fprintf(stderr, "mouse mv %d,%d acc=%d,%d held=%04x\n",
                            e.motion.xrel, e.motion.yrel, macc_x, macc_y,
                            *(dw*)&mem[DS_BASE + 0xad9]);
            }
        }
        /* Gamepad / Android-TV remote-as-controller: map buttons and the left
         * stick onto the same XT scancodes the keyboard path produces.
         * A=Enter(select/fire) B=Esc(back) X=Space Y=KP5(alt fire)
         * Start=Enter Back=Esc LB='-' RB='+'(=) dpad/stick=arrows */
        if (e.type == SDL_CONTROLLERDEVICEADDED){
            SDL_GameControllerOpen(e.cdevice.which);
        }
        if (e.type == SDL_CONTROLLERBUTTONDOWN || e.type == SDL_CONTROLLERBUTTONUP){
            int down = e.type == SDL_CONTROLLERBUTTONDOWN;
            int xs = 0; db a = 0;
            switch (e.cbutton.button){
            case SDL_CONTROLLER_BUTTON_A:
            case SDL_CONTROLLER_BUTTON_START:          xs = 0x1c; a = 0x0d; break;
            case SDL_CONTROLLER_BUTTON_B:
            case SDL_CONTROLLER_BUTTON_BACK:           xs = 0x01; a = 0x1b; break;
            case SDL_CONTROLLER_BUTTON_X:              xs = 0x39; a = 0x20; break;
            case SDL_CONTROLLER_BUTTON_Y:              xs = 0x4c; break;
            case SDL_CONTROLLER_BUTTON_DPAD_UP:        xs = 0x48; break;
            case SDL_CONTROLLER_BUTTON_DPAD_DOWN:      xs = 0x50; break;
            case SDL_CONTROLLER_BUTTON_DPAD_LEFT:      xs = 0x4b; break;
            case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:     xs = 0x4d; break;
            case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:   xs = 0x0c; a = '-'; break;
            case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:  xs = 0x0d; a = '='; break;
            }
            if (xs) synth_key(xs, a, down);
        }
        if (e.type == SDL_CONTROLLERAXISMOTION){
            /* left stick -> held arrow scans (menus read held bits; combat
             * samples them per tick — either way arrows are right) */
            static int axh, axv;
            int v = e.caxis.value, dir;
            const int TH = 16000;
            if (e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX){
                dir = v > TH ? 0x4d : v < -TH ? 0x4b : 0;
                if (dir != axh){ if (axh) synth_key(axh, 0, 0); if (dir) synth_key(dir, 0, 1); axh = dir; }
            } else if (e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY){
                dir = v > TH ? 0x50 : v < -TH ? 0x48 : 0;
                if (dir != axv){ if (axv) synth_key(axv, 0, 0); if (dir) synth_key(dir, 0, 1); axv = dir; }
            }
        }
        if ((e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) &&
            mouse_enabled()){
            int down = e.type == SDL_MOUSEBUTTONDOWN;
            /* A click with no preceding motion (window entry, touchpad tap)
             * would hit-test the stale cursor — sync it from the button's
             * own coordinates first. */
            if (*(dw*)&mem[0xf340] == 5)
                pod_abs_cursor(e.button.x, e.button.y);
            if (e.button.button == SDL_BUTTON_LEFT)
                synth_key(0x1c, 0x0d, down);      /* Enter: select */
            else if (e.button.button == SDL_BUTTON_RIGHT)
                synth_key(0x01, 0x1b, down);      /* Escape: back */
            if (getenv("M2C_MOUSEDBG"))
                fprintf(stderr, "mouse btn %d %s held=%04x\n",
                        e.button.button, down ? "dn" : "up",
                        *(dw*)&mem[DS_BASE + 0xad9]);
        }
    }
    mouse_dir_step();
}

dw rt_kbd_port60(void){ dw v = port60; port60 = 0; return v; }

/* Detect the graphics-mode menu by its "DESIRED MODE" string in the text
 * buffer — its int16 read is the only BIOS-blocking pick in the game. */
static int gfx_menu_visible(void){
    static const char tag[] = "DESIRED MODE";
    for (int row = 0; row < 25; row++){
        const unsigned char *p = &mem[0xb8000 + row*160];
        for (int col = 0; col + (int)sizeof(tag)-1 <= 80; col++){
            int i; for (i = 0; i < (int)sizeof(tag)-1; i++)
                if (p[(col+i)*2] != (unsigned char)tag[i]) break;   /* chars at even ofs */
            if (i == (int)sizeof(tag)-1) return 1;
        }
    }
    return 0;
}

/* BIOS int 16h */
void bios_getch(void){      /* AH=0: block -> AX */
    int v;
    static int gfx_done;    /* auto-select MCGA once, on the mode menu */
    while ((v = kpop()) < 0){
        kbd_polls++; rt_frame(); SDL_Delay(1);
        if (!gfx_done && gfx_menu_visible()){
            const char *g = getenv("M2C_GFXMODE");
            char pick = g ? *g : '4';       /* 4 = MCGA / mode 13h */
            if (pick >= '1' && pick <= '5'){
                gfx_done = 1;
                if (getenv("M2C_KTRACE")) fprintf(stderr, "getch <- gfxmode '%c'\n", pick);
                ax = pick; return;
            }
        }
    }
    if (getenv("M2C_KTRACE")) fprintf(stderr, "getch <- %04x caller=%p\n", v, __builtin_return_address(0));
    ax = v;
}
void bios_kbhit(void){      /* AH=1: ZF=0 & AX=key if pending */
    kbd_polls++;
    rt_frame();
    if (khead == ktail){ ZF=1; return; }
    ZF=0; ax = kq[ktail];
}
void bios_kbd(dw a){        /* dispatch on AH */
    switch (a>>8){
    case 0: bios_getch(); break;
    case 1: bios_kbhit(); break;
    case 2: al=0; break;              /* shift flags */
    }
}
void bios_time(void){       /* int 1Ah AH=0 -> CX:DX ticks since midnite (18.2Hz) */
    dd t = (dd)(SDL_GetTicks() * 18.206 / 1000);
    cx = t>>16; dx = t & 0xffff; al = 0;
}
