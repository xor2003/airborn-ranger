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
    static SDL_mutex *mx;
    if (!mx) mx = SDL_CreateMutex();
    SDL_LockMutex(mx);
    if (!script){ script = getenv("M2C_KEYS"); sdel = atoi(getenv("M2C_KEYS_DELAY") ?: "20"); }
    int delay = atoi(getenv("M2C_KEYS_DELAY") ?: "20");
    if (getenv("M2C_KTRACE")){ static int lp; if (++lp % 400 == 0)
        fprintf(stderr, "fs: polls=%d pend=%d sdel=%d next=%c\n", kbd_polls, pend_scan, sdel, script && *script ? *script : '-'); }
    if (pend_scan >= 0){                        /* release the pending press */
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
        if (xs < 0){                            /* pause token: just wait a window */
            script = next; sdel = 0; goto out;
        }
        int diverted = *(dw*)&mem[DS_BASE + 0xad7] == 1 &&
                       *(dw*)&mem[DS_BASE + 0x950 + (xs & 0x7f) * 2] != 0;
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
            if (getenv("M2C_KTRACE")) fprintf(stderr, "feed %02x div=%d\n", xs, d);
            if (!d) kpush((dw)((xs << 8) | asc));
            pend_scan = xs;
        }
    }
out:
    SDL_UnlockMutex(mx);
}

void rt_pump_events(void){
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
    SDL_Event e;
    while (SDL_PollEvent(&e)){
        if (e.type == SDL_QUIT) rt_exit(0);
        if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP){
            int xs = xt_scan(e.key.keysym.scancode);
            if (!xs) continue;
            port60 = xs | (e.type == SDL_KEYUP ? 0x80 : 0);
            int diverted = int9_update(xs & 0x7f, e.type == SDL_KEYDOWN);
            if (e.type == SDL_KEYDOWN && !diverted){
                char a = ascii_of(e.key.keysym.sym);
                kpush((dw)((xs<<8)|a));
            }
        }
    }
}

dw rt_kbd_port60(void){ dw v = port60; port60 = 0; return v; }

/* BIOS int 16h */
void bios_getch(void){      /* AH=0: block -> AX */
    int v;
    while ((v = kpop()) < 0){ kbd_polls++; rt_frame(); SDL_Delay(1); }
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
