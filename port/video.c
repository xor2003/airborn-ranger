/* VGA video via SDL2.
 * Mode 13h: pixels at A000:0000 (mem[0xa0000]), palette via 0x3c8/0x3c9,
 *   vertical retrace polled on 0x3da bit3 — we present there.
 * Text modes: 80x25 char+attr cells at B800:0000 (color) / B000:0000 (mono).
 */
#include <SDL2/SDL.h>
#include <math.h>
#include <pthread.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <signal.h>
#include <sched.h>
#endif
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
int m2c_glyph_calls;
dd m2c_dmc_rects, m2c_dmc_calls, m2c_scroll_calls, m2c_dtc_calls;
static int pit_latch;                /* 0=expect ch0 reload lo, 1=expect hi */
static unsigned pit_reload = 0;      /* ch0 reload value; 0 == 65536 */
static double pit_period_ms = 1000.0*65536/1193180.0;   /* current IRQ0 period */
static double irq_accum;             /* ms elapsed toward next IRQ0 dispatch */

/* Guest int8 handlers (TANDYSND ISR) are lifted C on the shared register file —
 * they may only run while the CPU (worker) thread is parked, like a real IRQ
 * preempting the CPU. POSIX: SIGUSR1 parks the worker in a handler that spins
 * on atomics (no guest state touched) until the ISR completes. Windows has no
 * per-thread signals: SuspendThread freezes the worker instead — the lifted
 * ISR then runs on this timer thread exactly as on POSIX. */
static pthread_t cpu_tid;
#ifdef _WIN32
static HANDLE cpu_hnd;
#else
static volatile sig_atomic_t park_req, park_ack, park_in;
static void isr_hold(int sig){
    (void)sig;
    sig_atomic_t id = park_req;
    park_in = id;                       /* handshake: worker is now frozen */
    while (park_ack < id) sched_yield();
}
#endif
volatile int rt_isr_ctx;             /* inside a timer-thread guest ISR */

void rt_set_cpu_thread(void){
    cpu_tid = pthread_self();
#ifndef _WIN32
    /* SDL spawns threads with SIGUSR1 masked (observed on Android: the park
     * signal sat pending forever, the handshake skipped every IRQ0, and the
     * guest froze at boot on a flat fill). The worker must take the signal. */
    sigset_t usr1;
    sigemptyset(&usr1);
    sigaddset(&usr1, SIGUSR1);
    pthread_sigmask(SIG_UNBLOCK, &usr1, NULL);
#else
    DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(),
                    &cpu_hnd, 0, FALSE, DUPLICATE_SAME_ACCESS);
#endif
}

static unsigned g8_cnt;                  /* IRQ0 dispatches, for TICKSTAT */
static void guest_irq0(void){
    ++g8_cnt;
#ifdef _WIN32
    if (!cpu_hnd) return;                  /* worker not registered yet */
    SuspendThread(cpu_hnd);
#else
    if (!cpu_tid) return;                  /* worker not registered yet */
    sig_atomic_t id = ++park_req;
    pthread_kill(cpu_tid, SIGUSR1);
    /* Wait for the worker to actually reach isr_hold before touching shared
     * guest state — the kill only lands on the next kernel-entry, and running
     * the ISR against live registers mid-instruction corrupts the guest
     * (glitched tiles, wedged wait-loops — the Android black-screen bug).
     * Bounded: if the signal is blocked/foreign, skip this IRQ rather than
     * race the worker. */
    {   int spins = 0;
        while (park_in < id && spins++ < 300000) sched_yield();
        if (park_in < id){ park_ack = id; return; }
    }
#endif
    /* A real interrupt pushes flags+CS:IP and the handler preserves every
     * register it touches. The lifted ISR is plain C over the shared reg
     * file — it can clobber anything — so snapshot/restore ALL registers.
     * Flags-only left the guest resuming on ISR-mutated regs: corrupted
     * sprite blits (garbled player/enemy tiles) and flipped CMP/Jcc pairs. */
    int cf=CF,zf=ZF,sf=SF,of=OF,pf=PF,af=AF,df=DF,iff=IF,tf=TF;
    dd r[8]; dw s[7];
    r[0]=eax;r[1]=ebx;r[2]=ecx;r[3]=edx;r[4]=esi;r[5]=edi;r[6]=esp;r[7]=ebp;
    s[0]=cs;s[1]=ds;s[2]=es;s[3]=fs;s[4]=gs;s[5]=ss;s[6]=ip;
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
    eax=r[0];ebx=r[1];ecx=r[2];edx=r[3];esi=r[4];edi=r[5];esp=r[6];ebp=r[7];
    cs=s[0];ds=s[1];es=s[2];fs=s[3];gs=s[4];ss=s[5];ip=s[6];
#ifdef _WIN32
    ResumeThread(cpu_hnd);
#else
    park_ack = id;
#endif
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
    /* M2C_GOD=1: pin the wounds counter (byte_29712) — hits can't accumulate.
     * Hold the mission clock: word_295EB is the 18-tick divider that drives
     * the 3-digit BCD countdown (byte_28CD4 / word_28CD5) — pinning it stops
     * the seconds. BUT only pin while >15s remain: the evac boarding window
     * counts down through ~10s and reaching 1s triggers screen_state=0x14 —
     * freezing the divider there would block evacuation forever.
     * Also hold ammo (byte_29715-29718) at its high-water mark so firing can't
     * deplete it while resupply/pickups still raise it. */
    static int godmode = -1;
    static db ammo_hwm[4];
    if (godmode < 0) godmode = getenv("M2C_GOD") != NULL;
    if (godmode){
        mem[0x1b132] = 0;
        int bcd_sec = mem[0x1a6f4]*100 + mem[0x1a6f5]*10 + mem[0x1a6f6];
        if (bcd_sec > 15)
            *(dw*)&mem[0x1b00b] = 0x12;
        for (int i = 0; i < 4; i++){
            db v = mem[0x1b135 + i];
            if (v > ammo_hwm[i]) ammo_hwm[i] = v;
            mem[0x1b135 + i] = ammo_hwm[i];
        }
    }
    /* accumulate real elapsed time — SDL timer granularity coarsens `interval`
     * (4ms requested, ~8-10ms delivered), which would run ticks at half rate */
    static Uint32 tick_last;
    Uint32 tnow = SDL_GetTicks();
    if (!tick_last) tick_last = tnow;
    irq_accum += tnow - tick_last;
    tick_last = tnow;
    /* IRQ latch: ticks that pile up while the CPU is busy are lost, never
     * queued — cap catch-up so a long guest op can't dump a burst of ticks */
    if (irq_accum > pit_period_ms * 4) irq_accum = pit_period_ms * 4;
    while (irq_accum >= pit_period_ms){
        irq_accum -= pit_period_ms;
        /* IRQ0: BIOS stub runs inline (mem-only C, thread-safe); a guest hook
         * (TANDYSND ISR) runs with the main CPU thread parked via signal. */
        if (*(dd*)&mem[0x20] == 0xf000fea5) rt_bios_int8();
        else if (!getenv("M2C_NOIRQ")) guest_irq0();
    }
    /* M2C_HOLD=hexmask: force bits in the int9 held-key mask (word_1D959) —
     * deterministic held-key injection for menu/POD debugging, bypasses SDL. */
    {   static int hold_mask = -1;
        static Uint32 hold_at = 0;
        if (hold_mask < 0){
            hold_mask = getenv("M2C_HOLD") ? (int)strtol(getenv("M2C_HOLD"), 0, 16) : 0;
            hold_at = getenv("M2C_HOLD_AT") ? (Uint32)strtol(getenv("M2C_HOLD_AT"), 0, 0) : 0;
        }
        if (hold_mask && SDL_GetTicks() >= hold_at)
            *(dw*)&mem[0xe8a0 + 0xad9] |= (dw)hold_mask;
    }
    /* M2C_SNAP=path: one-shot dump of mem[] once walk phase is live with a
     * placed ranger (X-int nonzero) — offline collision/movement replay. */
    {   static int snapped;
        const char *snap_to = getenv("M2C_SNAP");
        if (snap_to && !snapped && mem[0x1c3ea] == 1 && mem[0x1A81B] != 0){
            FILE *f = fopen(snap_to, "wb");
            if (f){ fwrite(mem, 1, 1<<20, f); fclose(f); snapped = 1;
                    fprintf(stderr, "SNAP wrote %s\n", snap_to); }
        }
    }
    Uint32 now = SDL_GetTicks();
    static Uint32 last_feed;
    if (now - last_feed >= 40){ rt_script_feed(); last_feed = now; }
    if (now - last_frame >= 33){ rt_present(0); last_frame = now; }
    if (getenv("M2C_TRATE")){
        static Uint32 t5;
        if (now - t5 >= 5000){ extern void rt_trate_report(void); rt_trate_report(); t5 = now; }
    }
    if (getenv("M2C_STATE")){
        static Uint32 s0;
        if (now - s0 >= 500){
            fprintf(stderr, "ST ca=%d ce=%d d0=%d e2=%d e1=%d 1dcff=%04x 1dd1f=%04x 1dd3f=%04x 26de4=%04x 26de6=%d 2962=%d 1d94d=%d 1d91e=%d mode=%d dv=%d held=%04x clk=%d%d%d ammo=%d,%d,%d,%d t=%u r955=%u r949=%u a945=%u sae=%d i66=%x i74=%d i6a=%d\n",
                    mem[0x1c3ea], *(dw*)&mem[0x1c3ee], *(dw*)&mem[0x1c3f0],
                    mem[0x1c402], mem[0x1c401],
                    *(dw*)&mem[0xf71f], *(dw*)&mem[0xf73f], *(dw*)&mem[0xf75f],
                    *(dw*)&mem[0x18804], mem[0x18806],
                    *(dw*)&mem[0x1c382], *(dw*)&mem[0xf36d],
                    *(dw*)&mem[0xf33e], cur_mode,
                    *(dw*)&mem[0xe8a0 + 0xad7], *(dw*)&mem[0xe8a0 + 0xad9],
                    mem[0x1a6f4], mem[0x1a6f5], mem[0x1a6f6],
                    mem[0x1b135], mem[0x1b136], mem[0x1b137], mem[0x1b138],
                    *(dw*)&mem[0xf102], *(dw*)&mem[0xf375], *(dw*)&mem[0xf369],
                    *(dw*)&mem[0x17fca], *(dw*)&mem[0xf365],
                    *(dw*)&mem[0x17f86], *(dw*)&mem[0x17f94],
                    *(dw*)&mem[0x17f8a]);
            fprintf(stderr, "  spriteXY=%02x,%02x(%02x) cellres=%d slide=%d cell=%d,%d | glyph=%d w934=%d s10=%04x s12=%04x s0e=%04x | f8=%d b70=%d a961=%d a99d=%04x\n",
                    mem[0xf721], mem[0xf761], mem[0xf741],
                    *(dw*)&mem[0x1c42d], *(dw*)&mem[0x1c41b], mem[0x1c3ff], mem[0x1c400],
                    m2c_glyph_calls, *(dw*)&mem[0xf354],
                    *(dw*)&mem[0x1a30], *(dw*)&mem[0x1a32], *(dw*)&mem[0x1a2e],
                    *(dw*)&mem[0xf318], *(dw*)&mem[0x1ca90], mem[0x1c381], *(dw*)&mem[0x1c3bd]);
            {   /* buffer checksums: present(0x48c5) compose(0x38c5) vga */
                unsigned long cp=0,cc=0,cv=0;
                for (long i=0;i<64000;i+=37){ cp=cp*131+mem[0x48c50+i]; cc=cc*131+mem[0x38c50+i]; cv=cv*131+mem[0xa0000+i]; }
                fprintf(stderr, "  px=%d py=%d camx=%d camy=%d pf=%d,%d th=%d,%d a8=%d,%d 5ae=%d r94b=%d held=%04x dir=%04x thr=%04x db0=%d scr=%d sdir=%02x dtc=%d dmc=%d/%d ck p=%lx c=%lx v=%lx\n",
                    mem[0x1c380], mem[0x1c381], *(dw*)&mem[0x1b0a3], *(dw*)&mem[0x1b0a1],
                    *(dw*)&mem[0x1b09b], *(dw*)&mem[0x1b09d], *(dw*)&mem[0x1b234], *(dw*)&mem[0x1b236],
                    *(dw*)&mem[0x1b088], *(dw*)&mem[0x1b08a],
                    *(dw*)&mem[0x1c2c0], *(dw*)&mem[0x1c2c2],
                    *(dw*)&mem[0x17fce], *(dw*)&mem[0xf36b], *(dw*)&mem[0x1a36],
                    *(dw*)&mem[0xf7d0], m2c_scroll_calls, mem[0x1b09a],
                    m2c_dtc_calls, m2c_dmc_rects, m2c_dmc_calls, cp,cc,cv);
            }
            /* DBG: divert-table entries for the 4 arrows (scan*2 word) at 0xf1f0 */
            fprintf(stderr, "  dvT up=%04x lf=%04x dn=%04x rt=%04x | ent=%04x sp=%04x ctl=%04x alt=%04x esc=%04x | d3=%d ce=%d\n",
                    *(dw*)&mem[0xf1f0 + 0x48*2], *(dw*)&mem[0xf1f0 + 0x4b*2],
                    *(dw*)&mem[0xf1f0 + 0x50*2], *(dw*)&mem[0xf1f0 + 0x4d*2],
                    *(dw*)&mem[0xf1f0 + 0x1c*2], *(dw*)&mem[0xf1f0 + 0x39*2],
                    *(dw*)&mem[0xf1f0 + 0x1d*2], *(dw*)&mem[0xf1f0 + 0x38*2],
                    *(dw*)&mem[0xf1f0 + 0x01*2],
                    mem[0x1c3f3], *(dw*)&mem[0x1c3ee]);
            {   /* free-roam: word_265AA frame counter + object-field region dump */
                fprintf(stderr, "  free fcnt=%d a8a0=%d 26dd5=%d 1dcb0=%d obj[%02x %02x %02x %02x %02x %02x %02x %02x]\n",
                        *(dw*)&mem[0x17fca],
                        *(dw*)&mem[0x1c2c0], mem[0x187f5], mem[0xf6d0],
                        mem[0xA87F], mem[0xA880], mem[0xA881], mem[0xA88F],
                        mem[0xA890], mem[0xAEB7+1], mem[0xAEA3+1], mem[0xAECB+1]);
            }
            {   /* M2C_MDUMP=<hexaddr>:<hexlen> — text dump of a mem region */
                static long ma = -1, ml;
                if (ma < 0){ const char *s = getenv("M2C_MDUMP"); char *e;
                    ma = s ? strtol(s,&e,16) : 0;
                    ml = (s && *e==':') ? strtol(e+1,0,16) : 0; }
                if (ma > 0 && ml > 0){
                    fprintf(stderr, "MD %05lx: ", ma);
                    for (long i=0;i<ml;i++){ unsigned char c=mem[ma+i];
                        fputc(c>=32&&c<127?c:'.',stderr); }
                    fprintf(stderr, "\n");
                }
                /* one-shot second region: M2C_MDUMP2=addr:len — nonzero stats */
                static long m2 = -2, m2l;
                if (m2 == -2){ const char *s = getenv("M2C_MDUMP2"); char *e;
                    m2 = s ? strtol(s,&e,16) : 0;
                    m2l = (s && *e==':') ? strtol(e+1,0,16) : 0; }
                if (m2 > 0 && m2l > 0){
                    long nz=0; for(long i=0;i<m2l;i++) if(mem[m2+i]) nz++;
                    fprintf(stderr, "MD2 %05lx nz=%ld/%ld: ", m2, nz, m2l);
                    for (long i=0;i<m2l && i<256;i++){ if (i && i%32==0) fprintf(stderr,"\n      ");
                        fprintf(stderr,"%02x",mem[m2+i]); }
                    fprintf(stderr,"\n");
                }
                /* compose-buffer diff: track which byte ranges change between ticks */
                if (getenv("M2C_CDIFF")){
                    static unsigned char *prev; static int init;
                    if (!prev){ prev = malloc(64000); }
                    if (!init){ memcpy(prev,&mem[0x38c50],64000); init=1; }
                    else {
                        long lo=64000,hi=-1,nd=0;
                        for (long i=0;i<64000;i++) if (mem[0x38c50+i]!=prev[i]){ nd++; if(i<lo)lo=i; if(i>hi)hi=i; }
                        fprintf(stderr,"  CDIFF nd=%ld range=[%ld..%ld]\n",nd,lo,hi);
                        memcpy(prev,&mem[0x38c50],64000);
                    }
                }
            }
            s0 = now;
        }
    }
    if (getenv("M2C_TICKSTAT")){
        extern dd rt_i8_cnt, rt_1c_cnt;
        static Uint32 t0; static dd p_i8, p_1c; static unsigned p_g8;
        if (!t0){ t0 = now; p_i8 = rt_i8_cnt; p_1c = rt_1c_cnt; p_g8 = g8_cnt; }
        if (now - t0 >= 2000){
            double s = (now - t0) / 1000.0;
            fprintf(stderr, "TICKSTAT irq0=%.1f/s bios8=%.1f/s int1c=%.1f/s pit=%.1fms\n",
                    (g8_cnt - p_g8)/s, (rt_i8_cnt - p_i8)/s, (rt_1c_cnt - p_1c)/s,
                    pit_period_ms);
            t0 = now; p_i8 = rt_i8_cnt; p_1c = rt_1c_cnt; p_g8 = g8_cnt;
        }
    }
    return interval;
}

void video_init(void){
#ifdef __ANDROID__
    SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight");
#endif
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);
#ifdef __ANDROID__
    win = SDL_CreateWindow("Airborne Ranger", 0, 0, VW, VH,
                           SDL_WINDOW_FULLSCREEN);
#else
    win = SDL_CreateWindow("Airborne Ranger", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                           VW*3, VH*3, SDL_WINDOW_SHOWN);
#endif
    /* software renderer: SDL calls happen only on the main thread
     * (video_run_loop) — see rt_present note */
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
    tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, TW, TH);
    /* worker-park hook must exist before the timer starts */
#ifndef _WIN32
    signal(SIGUSR1, isr_hold);
#endif
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

/* Key-mapping menu overlay, drawn into staging on top of the frame.
 * Colors are direct ARGB — the game remaps the mode13 palette, so attr-text
 * would inherit random colors. */
static void vkb_text(Uint32 *dst, int x, int y, const char *s, Uint32 fg){
    for (int i = 0; s[i]; i++){
        const uint8_t *g = (s[i] >= 0x20 && s[i] < 0x80)
                           ? g_font1_bitmaps[s[i] - 0x20] : NULL;
        for (int r = 0; r < 16 && y + r < TH; r++){
            int bits = g ? g[r >> 1] : 0;
            Uint32 *row = dst + (y + r) * TW + x + i * 8;
            for (int c = 0; c < 8 && x + i*8 + c < TW; c++)
                if (bits & (0x80 >> c)) row[c] = fg;
        }
    }
}

static void kmap_draw(Uint32 *pix){
    const int rows = rt_kmap_rows();
    const int sel = rt_kmap_cur();
    const int cap = rt_kmap_capture();
    const int ncol = 2, nrows = (rows + ncol - 1) / ncol;
    const int cw = 292, rowh = 21, pd = 10, hdr = 22;
    const int w = ncol*cw + pd*2, h = nrows*rowh + hdr + pd*2;
    const int x0 = (TW - w)/2, y0 = (TH - h)/2;
    for (int y = y0; y < y0+h; y++)
        for (int x = x0; x < x0+w; x++)
            pix[y*TW + x] = 0xff141824;
    for (int x = x0; x < x0+w; x++){
        pix[y0*TW+x] = pix[(y0+h-1)*TW+x] = 0xff5a6478;
    }
    for (int y = y0; y < y0+h; y++){
        pix[y*TW+x0] = pix[y*TW+x0+w-1] = 0xff5a6478;
    }
    vkb_text(pix, x0+pd, y0+4,
             cap ? "PRESS A BUTTON ON THE REMOTE...  (Back/Esc cancels)"
                 : "KEY MAPPING   up/down:select  OK:bind  Back:close",
             cap ? 0xffffd040 : 0xff8a94a8);
    for (int i = 0; i < rows; i++){
        int c = i / nrows, r = i % nrows;
        int px = x0 + pd + c*cw, py = y0 + hdr + r*rowh;
        int cur = (i == sel);
        Uint32 bg = cur ? 0xff3a4420 : 0xff1e2434;
        Uint32 fg = cur ? 0xffffd040 : 0xffc8ccd4;
        for (int y = py; y < py+rowh-3; y++)
            for (int x = px; x < px+cw-6; x++)
                pix[y*TW+x] = bg;
        vkb_text(pix, px + 4, py + 2, rt_kmap_name(i), fg);
        const char *b = rt_kmap_bind(i);
        int n = 0; while (b[n]) n++;
        vkb_text(pix, px + cw - 10 - n*8, py + 2, b, 0xff7cc8a0);
    }
}

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
    if (rt_kmap_open()) kmap_draw(pix);
    frame_dirty = 1;

    static int fno;
    const char *dump = getenv("M2C_DUMP");
    int every = getenv("M2C_DUMP_EVERY") ? atoi(getenv("M2C_DUMP_EVERY")) : 30;
    if (dump && ++fno % every == 0){
        if (getenv("M2C_VSTAT")){
            int nz = 0; for (int i = 0; i < 64000; i++) nz += mem[VGA_BASE+i] != 0;
            int npal = 0; for (int i = 0; i < 256; i++) npal += pal32[i] != 0xff000000;
            /* seg_1000E/10/12/18 live in the image header block at cs:0 */
            dw s0e = *(dw*)&mem[0x1a20+0x0E], s10 = *(dw*)&mem[0x1a20+0x10],
               s12 = *(dw*)&mem[0x1a20+0x12], s18 = *(dw*)&mem[0x1a20+0x18];
            int nz12 = 0, nz10 = 0;
            for (int i = 0; i < 64000; i++){ nz12 += mem[((dd)s12<<4)+i] != 0; nz10 += mem[((dd)s10<<4)+i] != 0; }
            /* intro tilemap regions: decompressed at ds(0e8a):3f22 (lo) and ds:8722 (hi) */
            int tmlo = 0, tmhi = 0;
            for (int i = 0; i < 0x4800; i++){ tmlo += mem[0xE8A0+0x3F22+i] != 0; tmhi += mem[0xE8A0+0x8722+i] != 0; }
            fprintf(stderr, "VSTAT %d mode=%x vga=%d pal=%d s10=%x:%d s12=%x:%d s0e=%x s18=%x tmlo=%d tmhi=%d\n",
                    fno, cur_mode, nz, npal, s10, nz10, s12, nz12, s0e, s18, tmlo, tmhi);
        }
        char p[256]; snprintf(p, sizeof p, "%s.%04d.ppm", dump, fno/every);
        FILE *f = fopen(p, "wb");
        if (f){ fprintf(f, "P6\n%d %d\n255\n", TW, TH);
            for (int i=0;i<TW*TH;i++){ Uint32 c=pix[i]; fputc(c>>16,f);fputc(c>>8,f);fputc(c,f);} fclose(f);}
        if (getenv("M2C_DUMP_RAW")){
            char r[256]; snprintf(r, sizeof r, "%s.%04d.idx", dump, fno/every);
            if ((f = fopen(r, "wb"))){ fwrite(mem+VGA_BASE, 1, 64000, f); fclose(f); }
            snprintf(r, sizeof r, "%s.%04d.pal", dump, fno/every);
            if ((f = fopen(r, "wb"))){ fwrite(pal, 1, 768, f); fclose(f); }
        }
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
