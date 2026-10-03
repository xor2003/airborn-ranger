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
    case SDL_SCANCODE_SELECT: return 0x1c;   /* Android TV DPAD_CENTER -> Enter */
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
 * same int9/int16 path a physical keypress uses. The pointer's ABSOLUTE
 * position drives everything — no deltas:
 *  - POD/crosshair screens: the guest cursor is written to the pointer's
 *    guest coordinates directly.
 *  - held-mask screens (gameplay): the pointer is a joystick knob — edge
 *    areas hold direction bits, center releases.
 *  - menu screens: pointer areas step the highlight toward that edge —
 *    bottom third scrolls down, top third scrolls up (repeats while held).
 * Left button = Enter (select), right button = Escape (back).
 * Set M2C_MOUSE=0 to disable. */
static int mouse_on = -1;                    /* lazy: -1 unknown, else 0/1 */
static int mgx = -1, mgy = -1;               /* pointer pos in guest px */
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
    if (getenv("M2C_MOUSEDBG"))
        fprintf(stderr, "mouse abs win=%d,%d -> cur=%d,%d\n", wx, wy, gx, gy);
}

/* Direction areas for the pointer's absolute position: the screen's edge
 * thirds mean "step/hold toward that edge", the middle is a dead zone. */
static void mouse_zone(int *hx, int *hy){
    *hx = *hy = 0;
    if (mgx < 0) return;
    if (mgx < 128)      *hx = 0x4b;      /* left  */
    else if (mgx >= 192) *hx = 0x4d;     /* right */
    if (mgy < 80)       *hy = 0x48;      /* up    */
    else if (mgy >= 120) *hy = 0x50;     /* down  */
}

/* Apply the pointer's absolute position once per pump. POD screens take the
 * real cursor position at motion time already; held-mask screens get the
 * direction bits held while the pointer parks in an edge area; menu screens
 * get a discrete step when the pointer enters an edge area, repeating while
 * it stays there. */
static void mouse_zone_step(void){
    static int mhx, mhy;                     /* arrow scans held by the mouse */
    static Uint32 rep; static int lhx, lhy;
    if (!mouse_enabled() || mgx < 0){ mhx = mhy = lhx = lhy = 0; return; }
    if (*(dw*)&mem[0xf340] == 5) return;     /* POD: absolute cursor, no zones */
    int hx, hy; mouse_zone(&hx, &hy);
    if (*(dw*)&mem[DS_BASE + 0xad7] == 1){
        if (hx != mhx){ if (mhx) mrelease(mhx); if (hx) int9_update(hx, 1); mhx = hx; }
        if (hy != mhy){ if (mhy) mrelease(mhy); if (hy) int9_update(hy, 1); mhy = hy; }
    } else {
        if (mhx){ mrelease(mhx); mhx = 0; }  /* divert dropped mid-drag: release */
        if (mhy){ mrelease(mhy); mhy = 0; }
        Uint32 now = SDL_GetTicks();
        if (hx != lhx || hy != lhy){
            if (hx){ synth_key(hx, 0, 1); synth_key(hx, 0, 0); }
            if (hy){ synth_key(hy, 0, 1); synth_key(hy, 0, 0); }
            lhx = hx; lhy = hy; rep = now + 350;
        } else if ((hx || hy) && now >= rep){
            if (hx){ synth_key(hx, 0, 1); synth_key(hx, 0, 0); }
            if (hy){ synth_key(hy, 0, 1); synth_key(hy, 0, 0); }
            rep = now + 250;
        }
    }
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

/* key-mapping menu — full definitions live after rt_pump_events */
static int kmap_event(SDL_Event *e);
static void kmap_tick(void);
static void kmap_toggle(int on);
static int kmap_wanted(void);
static int kmap_match(SDL_Event *e);
static int kmap_on;
static Uint32 back_dn;
static int back_hold_opened;

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
    kmap_tick();
    SDL_Event e;
    while (SDL_PollEvent(&e)){
        if (e.type == SDL_QUIT) rt_exit(0);
        if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED &&
            e.window.data1 > 0 && e.window.data2 > 0)
            { win_w = e.window.data1; win_h = e.window.data2; }
        /* key-mapping menu is modal while open */
        if (kmap_on && kmap_event(&e)) continue;
        /* open triggers when closed: MENU / F8 / gamepad GUIDE / hold-Back.
         * F10 is not a trigger — the game uses it (run/walk toggle); F8 is
         * free. */
        if (kmap_wanted() && e.type == SDL_KEYDOWN &&
            (e.key.keysym.scancode == SDL_SCANCODE_MENU ||
             e.key.keysym.scancode == SDL_SCANCODE_F8))
            { kmap_toggle(1); continue; }
        if (kmap_wanted() && e.type == SDL_CONTROLLERBUTTONDOWN &&
            e.cbutton.button == SDL_CONTROLLER_BUTTON_GUIDE)
            { kmap_toggle(1); continue; }
        /* AC_BACK (TV remote Back) defers its Esc to keyup: a short press is
         * Esc, holding it 700ms opens the mapping menu instead (kmap_tick). */
        if (kmap_wanted() &&
            (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) &&
            e.key.keysym.scancode == SDL_SCANCODE_AC_BACK){
            if (e.type == SDL_KEYDOWN && !e.key.repeat){
                back_dn = SDL_GetTicks(); back_hold_opened = 0;
            } else if (e.type == SDL_KEYUP){
                if (back_dn && !back_hold_opened && !kmap_on)
                    { synth_key(0x01, 0x1b, 1); synth_key(0x01, 0x1b, 0); }
                back_dn = 0;
            }
            continue;
        }
        /* a bound input fires its DOS key instead of the default mapping */
        if (kmap_match(&e)) continue;
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
            /* The pointer's absolute position is the input everywhere —
             * edge areas steer, the center is neutral. POD screens
             * (word_1D920==5) additionally get the real cursor position. */
            int gx = e.motion.x * 320 / (win_w > 0 ? win_w : 960);
            int gy = e.motion.y * 200 / (win_h > 0 ? win_h : 600);
            if (gx < 0) gx = 0; if (gx > 319) gx = 319;
            if (gy < 0) gy = 0; if (gy > 199) gy = 199;
            mgx = gx; mgy = gy;
            if (*(dw*)&mem[0xf340] == 5)            /* word_1D920 == POD */
                pod_abs_cursor(e.motion.x, e.motion.y);
            else if (getenv("M2C_MOUSEDBG"))
                fprintf(stderr, "mouse pos %d,%d held=%04x\n", gx, gy,
                        *(dw*)&mem[DS_BASE + 0xad9]);
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
    mouse_zone_step();
}

dw rt_kbd_port60(void){ dw v = port60; port60 = 0; return v; }

/* ---- key mapping menu (F15-style controls setup) ----
 * A list of the DOS keys the game reads, each bindable to a remote/keyboard
 * scancode or a gamepad button. D-pad/stick navigates, OK arms capture,
 * the next pressed input binds. Bindings persist to keymap.cfg in the
 * working dir (app filesDir on Android). While open the menu is modal.
 * Open: MENU / F8 / gamepad GUIDE / hold Back 700ms. M2C_NOKMAP=1 disables.
 * (F10 is the game's run/walk key — deliberately not a trigger.)
 */
typedef struct { const char *name; db scan; db a; } doskey;
static const doskey doskeys[] = {
    {"MOVE UP (UP)",0x48,0},{"MOVE DOWN (DOWN)",0x50,0},
    {"MOVE LEFT (LEFT)",0x4b,0},{"MOVE RIGHT (RIGHT)",0x4d,0},
    {"SELECT (ENTER)",0x1c,0x0d},{"BACK (ESC)",0x01,0x1b},
    {"FIRE (SPACE)",0x39,0x20},{"TAB",0x0f,0x09},
    {"WEAPON/MENU 1",0x02,'1'},{"WEAPON/MENU 2",0x03,'2'},
    {"WEAPON/MENU 3",0x04,'3'},{"WEAPON/MENU 4",0x05,'4'},
    {"WEAPON/MENU 5",0x06,'5'},{"WEAPON/MENU 6",0x07,'6'},
    {"WEAPON/MENU 7",0x08,'7'},{"WEAPON/MENU 8",0x09,'8'},
    {"WEAPON/MENU 9",0x0a,'9'},{"SPEED RESET (0)",0x0b,'0'},
    {"GAME SPEED (-)",0x0c,'-'},{"GAME SPEED (=)",0x0d,'='},
    {"CENTER (KP5)",0x4c,'5'},
    {"F1",0x3b,0},{"F2",0x3c,0},{"F3",0x3d,0},{"F4",0x3e,0},
    {"F5",0x3f,0},{"F6",0x40,0},{"F7",0x41,0},{"F8 (free)",0x42,0},
    {"MAP (F9)",0x43,0},{"RUN/WALK (F10)",0x44,0},
    {"RESET DEFAULTS",0,0},{"START GAME",0,0},
};
#define N_KMAP (int)(sizeof doskeys / sizeof doskeys[0])
#define N_BINDABLE (N_KMAP - 2)            /* last two rows are actions */

/* binding: kind 0=none 1=SDL scancode 2=controller button */
typedef struct { db kind; dw code; } kbind;
static kbind binds[N_KMAP];

static int kmap_on, kmap_sel;
static int kmap_enabled = -1;
static int kmap_cap;                     /* capture armed: next input binds */
static int kmap_loaded;
static Uint32 back_dn;                   /* AC_BACK press time (long-press timer) */
static int back_hold_opened;

static int kmap_wanted(void){
    if (kmap_enabled < 0)
        kmap_enabled = getenv("M2C_NOKMAP") ? 0 : 1;
    return kmap_enabled;
}

/* default map: keyboard keys bound to themselves; remote/gamepad extras
 * layered on the most useful keys. */
static void kmap_defaults(void){
    memset(binds, 0, sizeof binds);
    for (int i = 0; i < N_KMAP; i++){
        binds[i].kind = 1;
        switch (doskeys[i].scan){
        case 0x48: binds[i].code = SDL_SCANCODE_UP; break;
        case 0x50: binds[i].code = SDL_SCANCODE_DOWN; break;
        case 0x4b: binds[i].code = SDL_SCANCODE_LEFT; break;
        case 0x4d: binds[i].code = SDL_SCANCODE_RIGHT; break;
        case 0x1c: binds[i].code = SDL_SCANCODE_RETURN; break;
        case 0x01: binds[i].code = SDL_SCANCODE_ESCAPE; break;
        case 0x39: binds[i].code = SDL_SCANCODE_SPACE; break;
        case 0x0f: binds[i].code = SDL_SCANCODE_TAB; break;
        default:   binds[i].kind = 0; break;
        }
    }
}

#define KMAP_FILE "keymap.cfg"
static void kmap_save(void){
    FILE *f = fopen(KMAP_FILE, "w");
    if (!f) return;
    for (int i = 0; i < N_KMAP; i++)
        fprintf(f, "%d=%u:%u\n", i, binds[i].kind, binds[i].code);
    fclose(f);
}
static void kmap_load(void){
    kmap_loaded = 1;
    kmap_defaults();
    FILE *f = fopen(KMAP_FILE, "r");
    if (!f) return;
    int i, k; unsigned c;
    while (fscanf(f, "%d=%d:%u\n", &i, &k, &c) == 3)
        if (i >= 0 && i < N_KMAP && k >= 0 && k <= 2)
            { binds[i].kind = (db)k; binds[i].code = (dw)c; }
    fclose(f);
}
static void kmap_ensure(void){ if (!kmap_loaded) kmap_load(); }

int rt_kmap_open(void){ return kmap_on; }
int rt_kmap_rows(void){ return N_KMAP; }
int rt_kmap_cur(void){ return kmap_sel; }
int rt_kmap_capture(void){ return kmap_cap; }
const char *rt_kmap_name(int r){
    return (r >= 0 && r < N_KMAP) ? doskeys[r].name : "?";
}
const char *rt_kmap_bind(int r){
    static char buf[32];
    if (r < 0 || r >= N_KMAP) return "?";
    if (r >= N_BINDABLE) return "";
    if (binds[r].kind == 0) return "-";
    if (binds[r].kind == 1){
        snprintf(buf, sizeof buf, "%s",
                 SDL_GetScancodeName((SDL_Scancode)binds[r].code));
        return buf;
    }
    snprintf(buf, sizeof buf, "PAD %s",
             SDL_GameControllerGetStringForButton((SDL_GameControllerButton)binds[r].code));
    return buf;
}

static void kmap_toggle(int on){
    kmap_ensure();
    kmap_on = on;
    kmap_cap = 0;
    if (on) kmap_sel = 0;
}

/* consume one event in capture mode: bind it to the selected DOS key;
 * a key already bound elsewhere is moved (cleared from the old action) */
static int kmap_capture(SDL_Event *e){
    if (e->type == SDL_KEYDOWN && !e->key.repeat){
        SDL_Scancode s = e->key.keysym.scancode;
        if (s == SDL_SCANCODE_ESCAPE || s == SDL_SCANCODE_AC_BACK){
            kmap_cap = 0;                    /* cancel, don't bind */
            return 1;
        }
        for (int i = 0; i < N_KMAP; i++)
            if (i != kmap_sel && binds[i].kind == 1 && binds[i].code == s)
                binds[i].kind = 0;
        binds[kmap_sel].kind = 1; binds[kmap_sel].code = s;
        kmap_cap = 0; kmap_save();
        return 1;
    }
    if (e->type == SDL_CONTROLLERBUTTONDOWN){
        for (int i = 0; i < N_KMAP; i++)
            if (i != kmap_sel && binds[i].kind == 2 &&
                binds[i].code == e->cbutton.button)
                binds[i].kind = 0;
        binds[kmap_sel].kind = 2; binds[kmap_sel].code = e->cbutton.button;
        kmap_cap = 0; kmap_save();
        return 1;
    }
    /* swallow everything else while capturing */
    return e->type == SDL_KEYUP || e->type == SDL_CONTROLLERBUTTONUP ||
           e->type == SDL_CONTROLLERAXISMOTION;
}

/* closed-state dispatch: if this event matches a binding, inject the bound
 * DOS key and consume the event (default key/controller maps never see it). */
static int kmap_match(SDL_Event *e){
    kmap_ensure();
    if (e->type == SDL_KEYDOWN || e->type == SDL_KEYUP){
        int down = e->type == SDL_KEYDOWN;
        if (down && e->key.repeat) return 0;
        for (int i = 0; i < N_BINDABLE; i++)
            if (binds[i].kind == 1 && binds[i].code == e->key.keysym.scancode){
                synth_key(doskeys[i].scan, doskeys[i].a, down);
                return 1;
            }
        return 0;
    }
    if (e->type == SDL_CONTROLLERBUTTONDOWN || e->type == SDL_CONTROLLERBUTTONUP){
        int down = e->type == SDL_CONTROLLERBUTTONDOWN;
        for (int i = 0; i < N_BINDABLE; i++)
            if (binds[i].kind == 2 && binds[i].code == e->cbutton.button){
                synth_key(doskeys[i].scan, doskeys[i].a, down);
                return 1;
            }
        return 0;
    }
    return 0;
}

/* Returns 1 when the event was consumed by the menu. */
static int kmap_event(SDL_Event *e){
    if (kmap_cap && kmap_capture(e)) return 1;
    if (kmap_cap) return e->type == SDL_QUIT ||
        (e->type == SDL_WINDOWEVENT && e->window.event == SDL_WINDOWEVENT_RESIZED) ? 0 : 1;
    if (e->type == SDL_KEYDOWN){
        switch (e->key.keysym.scancode){
        case SDL_SCANCODE_UP:   kmap_sel = (kmap_sel + N_KMAP - 1) % N_KMAP; return 1;
        case SDL_SCANCODE_DOWN: kmap_sel = (kmap_sel + 1) % N_KMAP; return 1;
        case SDL_SCANCODE_LEFT:
        case SDL_SCANCODE_RIGHT: return 1;      /* ignored: column-less list */
        case SDL_SCANCODE_RETURN:
        case SDL_SCANCODE_KP_ENTER:
        case SDL_SCANCODE_SELECT:              /* Android TV DPAD_CENTER */
        case SDL_SCANCODE_SPACE:
            if (kmap_sel == N_KMAP - 2){ kmap_defaults(); kmap_save(); }
            else if (kmap_sel == N_KMAP - 1) kmap_on = 0;   /* START GAME */
            else kmap_cap = 1;
            return 1;
        case SDL_SCANCODE_MENU:
        case SDL_SCANCODE_AC_BACK:
            back_dn = 0;
            /* fall through */
        case SDL_SCANCODE_ESCAPE:
        case SDL_SCANCODE_F8:   kmap_toggle(0); return 1;
        default: return 1;                     /* modal: swallow all other keys */
        }
    }
    if (e->type == SDL_KEYUP) return 1;
    if (e->type == SDL_CONTROLLERBUTTONDOWN){
        switch (e->cbutton.button){
        case SDL_CONTROLLER_BUTTON_DPAD_UP:   kmap_sel = (kmap_sel + N_KMAP - 1) % N_KMAP; return 1;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN: kmap_sel = (kmap_sel + 1) % N_KMAP; return 1;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: return 1;
        case SDL_CONTROLLER_BUTTON_A:
        case SDL_CONTROLLER_BUTTON_START:
            if (kmap_sel == N_KMAP - 2){ kmap_defaults(); kmap_save(); }
            else if (kmap_sel == N_KMAP - 1) kmap_on = 0;   /* START GAME */
            else kmap_cap = 1;
            return 1;
        case SDL_CONTROLLER_BUTTON_GUIDE:
        case SDL_CONTROLLER_BUTTON_B:
        case SDL_CONTROLLER_BUTTON_BACK:      kmap_toggle(0); return 1;
        default: return 1;
        }
    }
    if (e->type == SDL_CONTROLLERBUTTONUP) return 1;
    if (e->type == SDL_CONTROLLERAXISMOTION){
        static int stk_v; static Uint32 stk_rep;
        int dir = 0;
        if (e->caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
            dir = e->caxis.value > 16000 ? 1 : e->caxis.value < -16000 ? -1 : 0;
        else return 1;
        Uint32 now = SDL_GetTicks();
        if (dir && (dir != stk_v || now >= stk_rep)){
            stk_v = dir; stk_rep = now + 220;
            kmap_sel = (kmap_sel + N_KMAP + dir) % N_KMAP;
        } else if (!dir) stk_v = 0;
        return 1;
    }
    if (e->type == SDL_QUIT ||
        (e->type == SDL_WINDOWEVENT && e->window.event == SDL_WINDOWEVENT_RESIZED))
        return 0;
    return 1;
}

/* Long-press Back on a TV remote opens the mapping menu. A short press
 * still sends Esc on release (keydown is deferred so holding never
 * double-fires). */
static void kmap_tick(void){
    if (!kmap_wanted()) return;
    if (back_dn && !back_hold_opened &&
        SDL_GetTicks() - back_dn > 700){
        back_hold_opened = 1;
        kmap_toggle(1);
    }
}

/* Exclusive controls-setup screen: shown on its own before the guest starts
 * (Android TV default; M2C_KMAPSTART=1 forces it on other builds). Runs its
 * own event/draw loop on the main thread — the game isn't running yet, so
 * nothing is drawn behind it. Returns when the user picks START GAME /
 * closes the menu; Esc/Back also leaves so a plain remote can't wedge. */
void rt_kmap_exclusive(void){
    if (!kmap_wanted()) return;
#ifndef __ANDROID__
    if (!getenv("M2C_KMAPSTART")) return;      /* desktop: opt-in only */
#endif
    kmap_ensure();
    kmap_on = 1; kmap_cap = 0;
    kmap_sel = N_KMAP - 1;                 /* land on START GAME */
    while (kmap_on){
        SDL_Event e;
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT) rt_exit(0);
            if (e.type == SDL_WINDOWEVENT &&
                e.window.event == SDL_WINDOWEVENT_RESIZED &&
                e.window.data1 > 0 && e.window.data2 > 0)
                { win_w = e.window.data1; win_h = e.window.data2; }
            kmap_event(&e);
        }
        rt_kmap_frame();
        SDL_Delay(16);
    }
    back_dn = 0; back_hold_opened = 0;     /* stray Back shouldn't reopen */
}


/* Scan the text buffer for a menu's title string — the graphics and control
 * menus are the only BIOS-blocking picks in the game. */
static int text_menu_has(const char *tag){
    int len = 0; while (tag[len]) len++;
    for (int row = 0; row < 25; row++){
        const unsigned char *p = &mem[0xb8000 + row*160];
        for (int col = 0; col + len <= 80; col++){
            int i; for (i = 0; i < len; i++)
                if (p[(col+i)*2] != (unsigned char)tag[i]) break;   /* chars at even ofs */
            if (i == len) return 1;
        }
    }
    return 0;
}

/* SELECT CONTROL DEVICE: 1=joystick 2=keyboard-directional 3=rotational.
 * The menu renders in graphics mode (no text buffer to scan), but the guest
 * marks it with menu_mode=8 (mem[0xf340], set only by input_device_menu).
 * The pick is injected as a real BIOS-buffer entry while the menu is up and
 * the queue is empty — kbhit reports it and read_key consumes it like a
 * typed key. TV remotes only have directional arrows, so Android defaults
 * to 2; M2C_CTRL overrides; other platforms wait for the user. */
static int ctrl_done;
static void ctrl_menu_autopick(void){
    if (ctrl_done || *(volatile dw*)&mem[0xf340] != 8) return;   /* menu_mode */
    const char *g = getenv("M2C_CTRL");
    char pick = g ? *g : 0;
#ifdef __ANDROID__
    if (!pick) pick = '2';
#endif
    if (pick < '1' || pick > '3') return;
    ctrl_done = 1;
    if (getenv("M2C_KTRACE")) fprintf(stderr, "ctrldev autopick '%c'\n", pick);
    kpush((dw)(((pick - '0' + 1) << 8) | pick));   /* '1'->0x0231 etc */
}

/* BIOS int 16h */
void bios_getch(void){      /* AH=0: block -> AX */
    int v;
    static int gfx_done;    /* auto-select MCGA once, on the mode menu */
    while ((v = kpop()) < 0){
        kbd_polls++; rt_frame(); SDL_Delay(1);
        ctrl_menu_autopick();
        if (!gfx_done && text_menu_has("DESIRED MODE")){
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
    if (khead == ktail) ctrl_menu_autopick();
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
