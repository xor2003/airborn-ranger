/* VGA video via SDL2.
 * Mode 13h: pixels at A000:0000 (mem[0xa0000]), palette via 0x3c8/0x3c9,
 *   vertical retrace polled on 0x3da bit3 — we present there.
 * Text modes: 80x25 char+attr cells at B800:0000 (color) / B000:0000 (mono).
 */
#include <SDL2/SDL.h>
#include <math.h>
#include <pthread.h>
#include <signal.h>
#include <sched.h>
#include "rt.h"
#include "font8x8.h"

static SDL_Window *win;
static SDL_Renderer *ren;
static SDL_Texture *tex;
static Uint32 pal32[256];
static db pal[768];
static int pidx, pwrote;               /* 0x3c8 write index state */
static int pidx_r;                     /* 0x3c7 read index */
static db attrpal[17];                 /* int10 AX=100x palette regs 0-15 + border */
static Uint32 last_frame;
static int pit_latch;                /* 0=expect ch0 reload lo, 1=expect hi */
static unsigned pit_reload = 0;      /* ch0 reload value; 0 == 65536 */
static double pit_period_ms = 1000.0*65536/1193180.0;   /* current IRQ0 period */
static double irq_accum;             /* ms elapsed toward next IRQ0 dispatch */

/* Guest int8 handlers (TANDYSND ISR) are lifted C on the shared register file —
 * they may only run while the CPU (worker) thread is parked, like a real IRQ
 * preempting the CPU. SIGUSR1 parks the worker in a handler that spins on
 * atomics (no guest state touched) until the ISR completes. */
static pthread_t cpu_tid;
void rt_set_cpu_thread(void){ cpu_tid = pthread_self(); }
static volatile sig_atomic_t park_req, park_ack;
volatile int rt_isr_ctx;             /* inside a timer-thread guest ISR */

static void isr_hold(int sig){
    (void)sig;
    sig_atomic_t id = park_req;
    while (park_ack < id) sched_yield();
}

static void guest_irq0(void){
    if (!cpu_tid) return;                  /* worker not registered yet */
    sig_atomic_t id = ++park_req;
    pthread_kill(cpu_tid, SIGUSR1);
    /* a real IRET restores the flag image pushed on interrupt; the lifted
     * ISR returns without one, so snapshot/restore the flag globals —
     * otherwise an IRQ landing between a guest CMP and its Jcc flips the
     * branch (corrupt tiles, misrouted sequencer state) */
    int cf=CF,zf=ZF,sf=SF,of=OF,pf=PF,af=AF,df=DF,iff=IF,tf=TF;
    /* M2C_IRQSAVE=full: also snapshot GPRs/segment regs — diagnostic to tell
     * apart a register leak (original semantics wouldn't leak) from a
     * deterministic bug in scene decoding */
    int full = getenv("M2C_IRQSAVE") != NULL;
    dd r[8]; dw s[7];
    if (full){
        r[0]=eax;r[1]=ebx;r[2]=ecx;r[3]=edx;r[4]=esi;r[5]=edi;r[6]=esp;r[7]=ebp;
        s[0]=cs;s[1]=ds;s[2]=es;s[3]=fs;s[4]=gs;s[5]=ss;s[6]=ip;
    }
    /* The lifted ISR runs on the interrupted stack like real hardware — but
     * guest code (the LZW decoder) parks scratch data below sp inside
     * push/pop chains, which ISR pushes would overwrite. Real hardware has
     * the same hazard; the window is just far narrower. Give the ISR its own
     * stack so decoder scratch below sp is never clobbered. */
    dw sss = ss, ssp = sp;
    ss = 0xF000; sp = 0xFFFE;             /* BIOS ROM area: never touched by game */
    rt_isr_ctx = 1;
    rt_call_vector(8);
    rt_isr_ctx = 0;
    ss = sss; sp = ssp;
    CF=cf; ZF=zf; SF=sf; OF=of; PF=pf; AF=af; DF=df; IF=iff; TF=tf;
    if (full){
        eax=r[0];ebx=r[1];ecx=r[2];edx=r[3];esi=r[4];edi=r[5];esp=r[6];ebp=r[7];
        cs=s[0];ds=s[1];es=s[2];fs=s[3];gs=s[4];ss=s[5];ip=s[6];
    }
    park_ack = id;
}

#define VGA_BASE 0xa0000
#define TEXT_BASE 0xb8000
#define MONO_BASE 0xb0000
#define VW 320
#define VH 200
#define TW 640
#define TH 400                       /* 80x25 at 8x16 */

/* CGA/EGA 16-color palette -> RGB */
static const Uint32 cgapal[16] = {
    0xff000000,0xff0000aa,0xff00aa00,0xff00aaaa,0xffaa0000,0xffaa00aa,0xffaa5500,0xffaaaaaa,
    0xff555555,0xff5555ff,0xff55ff55,0xff55ffff,0xffff5555,0xffff55ff,0xffffff55,0xffffffff
};

/* program one DAC color register; r/g/b are 6-bit VGA values */
static void set_dac(int i, int r, int g, int b){
    pal[i*3]   = r & 0x3f;
    pal[i*3+1] = g & 0x3f;
    pal[i*3+2] = b & 0x3f;
    pal32[i] = 0xff000000 | (Uint32)(r&0x3f)<<18 | (Uint32)(g&0x3f)<<10 | (Uint32)(b&0x3f)<<2;
}

static int cur_mode = 3;             /* DOS boots in mode 3 (80x25 color text) */
static db cursor_pos[2] = {0,0};     /* dh,dl from set_cursor */

void rt_present(int pump);

/* IRQ0 dispatch at the guest-programmed PIT rate. The BIOS default is 18.2 Hz
 * (reload 0 == 65536), but the TANDYSND overlay reprograms ch0 to 0x4DAE
 * (~60 Hz) while music plays — its sequencer counts these ticks for note
 * durations, so honoring the rate is required for correct tempo.
 * Runs on an SDL timer thread so pure delay loops that never touch I/O still
 * see time pass. Also stages frames on wall-clock: on real hardware the VGA
 * scans A000 continuously, so code that draws without polling 0x3da (intro
 * animation) is still visible — the main thread presents staged frames. */
static Uint32 tick_cb(Uint32 interval, void *param){
    (void)param;
    irq_accum += interval;
    while (irq_accum >= pit_period_ms){
        irq_accum -= pit_period_ms;
        /* IRQ0: BIOS stub runs inline (mem-only C, thread-safe); a guest hook
         * (TANDYSND ISR) runs with the main CPU thread parked via signal. */
        if (*(dd*)&mem[0x20] == 0xf000fea5) rt_bios_int8();
        else if (!getenv("M2C_NOIRQ")) guest_irq0();
    }
    Uint32 now = SDL_GetTicks();
    static Uint32 last_feed;
    if (now - last_feed >= 40){ rt_script_feed(); last_feed = now; }
    if (now - last_frame >= 33){ rt_present(0); last_frame = now; }
    return interval;
}

void video_init(void){
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
    win = SDL_CreateWindow("Airborne Ranger", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                           VW*3, VH*3, SDL_WINDOW_SHOWN);
    /* software renderer: SDL calls happen only on the main thread
     * (video_run_loop) — see rt_present note */
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
    tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, TW, TH);
    /* worker-park hook must exist before the timer starts */
    signal(SIGUSR1, isr_hold);
    for (int i=0;i<256;i++) pal32[i] = 0xff000000;
    for (int i=0;i<16;i++){           /* power-on palette: identity attr regs, CGA DAC */
        attrpal[i] = i;
        set_dac(i, (cgapal[i]>>18)&0x3f, (cgapal[i]>>10)&0x3f, (cgapal[i]>>2)&0x3f);
    }
    attrpal[16] = 0;
    /* fast base tick: IRQ0s are accumulated in tick_cb at the guest's PIT rate
     * (18.2Hz default, ~60Hz under TANDYSND) — 4ms gives clean subdivisions */
    SDL_AddTimer(4, tick_cb, NULL);
}

/* render one text cell: ch at font row r, attribute attr -> 8 pixels */
static void text_px(Uint32 *dst, int pitch, int x, int y, db chr, db attr){
    /* attr nibbles index the attr-controller palette regs, which select DAC colors */
    Uint32 fg = pal32[attrpal[attr & 0x0f]], bg = pal32[attrpal[(attr >> 4) & 7]];
    const uint8_t *g = (chr >= 0x20 && chr < 0x80) ? g_font1_bitmaps[chr - 0x20] : NULL;
    for (int r = 0; r < 16; r++){            /* stretch 8 rows -> 16 */
        int bits = g ? g[r >> 1] : (chr ? 0xff : 0);
        Uint32 *row = dst + (y + r) * pitch + x;
        for (int c = 0; c < 8; c++) row[c] = (bits & (0x80 >> c)) ? fg : bg;
    }
}

void rt_frame(void){ rt_present(1); }

/* Staging framebuffer — rendered by rt_present (any thread: pure CPU, no SDL).
 * SDL video calls may only run on the main thread: on X11 the software
 * renderer's SDL_RenderPresent does not display when invoked from a worker or
 * the SDL timer thread (observed: frozen/black window during tick-paced scenes
 * like the intro drop, where the guest never polls 0x3da). */
static Uint32 staging[TW*TH];
static volatile int frame_dirty;

void rt_present(int pump){
    (void)pump;                              /* pump now lives in video_run_loop */
    Uint32 *pix = staging;
    if (cur_mode == 0x13){
        const db *v = &mem[VGA_BASE];
        for (int y = 0; y < VH; y++)
            for (int x = 0; x < VW; x++)
                for (int s = 0; s < 2; s++)          /* pixel-double to 640x400 */
                    pix[(y*2)*TW + x*2 + s] = pix[(y*2+1)*TW + x*2 + s] = pal32[v[y*VW+x]];
    } else {
        dd tb = (cur_mode == 7) ? MONO_BASE : TEXT_BASE;
        for (int r = 0; r < 25; r++)
            for (int c = 0; c < 80; c++){
                db chr = mem[tb + (r*80+c)*2], at = mem[tb + (r*80+c)*2 + 1];
                text_px(pix, TW, c*8, r*16, chr, at);
            }
    }
    frame_dirty = 1;

    static int fno;
    const char *dump = getenv("M2C_DUMP");
    int every = getenv("M2C_DUMP_EVERY") ? atoi(getenv("M2C_DUMP_EVERY")) : 30;
    if (dump && ++fno % every == 0){
        char p[256]; snprintf(p, sizeof p, "%s.%04d.ppm", dump, fno/every);
        FILE *f = fopen(p, "wb");
        if (f){ fprintf(f, "P6\n%d %d\n255\n", TW, TH);
            for (int i=0;i<TW*TH;i++){ Uint32 c=pix[i]; fputc(c>>16,f);fputc(c>>8,f);fputc(c,f);} fclose(f);}
        char q[256]; snprintf(q, sizeof q, "%s.%04d.txt", dump, fno/every);
        if ((f = fopen(q, "wb"))){ dd tb = (cur_mode==7)?MONO_BASE:TEXT_BASE;
            for (int r=0;r<25;r++){ for(int c=0;c<80;c++){ db cc=mem[tb+(r*80+c)*2];
                fputc(cc>=0x20&&cc<0x7f?cc:'.',f);} fputc('\n',f);} fclose(f);}
    }
}

/* Main-thread loop: SDL event pump + present staged frames. The guest CPU runs
 * on a worker thread; all SDL video calls happen here. Never returns.
 * Pump is throttled to ~66 Hz — rt_script_feed counts pump invocations as its
 * key-feed cadence, so rate must stay near the historical present rate. */
void video_run_loop(void){
    Uint32 last_pump = 0;
    for (;;){
        Uint32 now = SDL_GetTicks();
        if (now - last_pump >= 15){ rt_pump_events(); last_pump = now; }
        if (frame_dirty){
            frame_dirty = 0;
            SDL_UpdateTexture(tex, NULL, staging, TW*4);
            SDL_RenderClear(ren);
            SDL_RenderCopy(ren, tex, NULL, NULL);
            SDL_RenderPresent(ren);
        }
        SDL_Delay(1);
    }
}

dw rt_in(dw p){
    switch (p & 0xffff){
    case 0x3da: {  /* input status 1 — real 70 Hz beam timing. Bit3 (vblank) is
                    * set only during the ~1.3 ms vblank tail of each 14.3 ms
                    * frame; bit0 (display enable) during the active scan.
                    * sub_11b81 polls bit3 to pace VRAM flips/animation — with
                    * instant toggling the whole intro ran in a flash. */
        static int vblank_was;
        double pos = fmod((double)SDL_GetPerformanceCounter() /
                          (double)SDL_GetPerformanceFrequency(), 1.0/70.0);
        int vblank = pos >= (1.0/70.0 - 0.0013);
        if (vblank && !vblank_was){ rt_frame(); last_frame = SDL_GetTicks(); }
        vblank_was = vblank;
        return vblank ? 9 : 0;                 /* bit3 + bit0 */
    }
    case 0x3c7: return 0;                        /* DAC state: ready */
    case 0x3c9: { db v = pal[pidx_r]; pidx_r = (pidx_r + 1) % 768; return v; }
    case 0x40:  return (dw)(SDL_GetTicks()*1193180ULL/1000) & 0xff;   /* PIT ch0 low byte */
    case 0x60:  return rt_kbd_port60();
    case 0x61:  return 0x30;                     /* speaker gate: on, data ok */
    default:    return 0;
    }
}

void rt_out(dw p, dw v){
    switch (p & 0xffff){
    case 0x3c8: pidx = (v & 0xff) * 3; pwrote = 0; break;    /* DAC entry index */
    case 0x3c7: pidx_r = (v & 0xff) * 3; pwrote = 0; break;
    case 0x3c9:
        pal[pidx] = v & 0x3f;
        pidx++;
        if (pidx % 3 == 0) {
            int c = pidx - 1;
            set_dac(c/3, pal[c-2], pal[c-1], pal[c]);
        }
        if (pidx >= 768) pidx = 0;
        break;
    case 0x3c4: case 0x3c5: case 0x3d4: case 0x3d5: break; /* seq/CRT — mode13 fixed */
    case 0x20: case 0xa0: break;                          /* PIC EOI */
    case 0x43:                                            /* PIT control */
        if ((v & 0x30) == 0x30 && (v & 0xc0) == 0) pit_latch = 0; /* ch0, lo/hi */
        break;
    case 0x40:                                            /* PIT ch0 reload */
        if (pit_latch == 0){ pit_reload = v & 0xff; pit_latch = 1; }
        else {
            pit_reload |= (v & 0xff) << 8; pit_latch = 0;
            if (!pit_reload) pit_reload = 0x10000;
            pit_period_ms = pit_reload * 1000.0 / 1193180.0;
            if (getenv("M2C_SND_TRACE"))
                fprintf(stderr, "pit %ums reload=%x ivt8=%lx\n",
                    SDL_GetTicks(), pit_reload, (unsigned long)*(dd*)&mem[0x20]);
        }
        break;
    case 0xc0: case 0xc1: rt_tnd_write(p, v); break;      /* Tandy SN76496 */
    case 0x61: rt_speaker(v); break;
    default: break;
    }
}

/* teletype-style character output at cursor, with CR/LF/wrap/scroll */
void text_putc(db chr){
    int r = cursor_pos[0], c = cursor_pos[1];
    dd tb = (cur_mode == 7) ? MONO_BASE : TEXT_BASE;
    if (chr == '\r'){ c = 0; goto done; }
    if (chr == '\n'){ r++; goto scroll; }
    if (ch == 7){ goto done; }                       /* bell: ignore */
    mem[tb + (r*80+c)*2]     = chr;
    mem[tb + (r*80+c)*2 + 1] = 7;
    if (++c >= 80){ c = 0; r++; }
scroll:
    if (r >= 25){
        memmove(&mem[tb], &mem[tb + 160], 24*160);
        memset(&mem[tb + 24*160], 0, 160);
        r = 24;
    }
done:
    cursor_pos[0] = r; cursor_pos[1] = c;
}

/* BIOS int 10h */
void bios_set_mode(void){ cur_mode = al & 0x7f; }
void bios_set_cursor(void){ if (ah == 2){ cursor_pos[0] = dh; cursor_pos[1] = dl; } }
void bios_scroll(void){}
void bios_putc(void){          /* AH=0Eh teletype */
    if (cur_mode == 0x13) return;
    text_putc(al);
}
/* int 10h AH=10h — palette/DAC functions, dispatched on AL */
void bios_palette(void){
    switch (al){
    case 0x00:                                   /* set palette register: BL=reg, BH=color */
        if (bl < 16) attrpal[bl] = bh;
        break;
    case 0x01:                                   /* set border color */
        attrpal[16] = bh;
        break;
    case 0x02: {                                 /* set all palette regs: ES:DX -> 17 bytes */
        memcpy(attrpal, raddr_(es, dx), 17);
        break;
    }
    case 0x07:                                   /* read palette register -> BH */
        bh = bl < 17 ? attrpal[bl] : 0;
        break;
    case 0x09:                                   /* read all palette regs -> ES:DX */
        memcpy(raddr_(es, dx), attrpal, 17);
        break;
    case 0x10:                                   /* set DAC reg BX = DH:r, CH:g, CL:b */
        set_dac(bx, dh, ch, cl);
        break;
    case 0x12: {                                 /* set DAC block BX..BX+CX-1 from ES:DX rgb triplets */
        const db *t = raddr_(es, dx);
        for (int i = 0; i < cx; i++) set_dac(bx + i, t[i*3], t[i*3+1], t[i*3+2]);
        break;
    }
    case 0x15:                                   /* read DAC reg BX -> DH/CH/CL */
        dh = pal[bx*3]; ch = pal[bx*3+1]; cl = pal[bx*3+2];
        break;
    case 0x17: {                                 /* read DAC block -> ES:DX */
        db *t = raddr_(es, dx);
        for (int i = 0; i < cx; i++){ t[i*3] = pal[(bx+i)*3]; t[i*3+1] = pal[(bx+i)*3+1]; t[i*3+2] = pal[(bx+i)*3+2]; }
        break;
    }
    }
}
static int video_page;                                    /* 0..7 -> 160-byte pages */
void bios_video(dw a){
    switch (a >> 8){
    case 0x0f: ax = (0x28 << 8) | cur_mode; bh = video_page; break;  /* get mode */
    case 0x00: cur_mode = a & 0x7f; break;                           /* set mode */
    case 0x05: video_page = a & 7; break;                            /* select page */
    case 0x09: {                       /* write attr/char at cursor, cx reps */
        int r = cursor_pos[0], c = cursor_pos[1];
        dd tb = (cur_mode == 7) ? MONO_BASE : TEXT_BASE;
        for (int i = 0; i < cx; i++){
            if (c + i < 80){
                mem[tb + video_page*0x800 + (r*80 + c + i)*2]     = al;
                mem[tb + video_page*0x800 + (r*80 + c + i)*2 + 1] = bl;
            }
        }
        break;
    }
    case 0x0b: break;                  /* set border/palette — n/a for text */
    }
}
