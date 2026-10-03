/* port entry — mirrors what DOS does when loading AR.EXE plus SDL bring-up */
#ifdef _WIN32
#define SDL_MAIN_HANDLED   /* keep a plain main() so -mconsole links without SDL2main */
#endif
#include <SDL2/SDL.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#ifdef __ANDROID__
#include <SDL2/SDL_system.h>
#endif
#include "rt.h"

/* load module sits at para 0x1a2; PSP would be 0x192 (unused: game uses its own ds).
 * EXE header (rel paras): SS=0x1b51 SP=0xcb14  CS:IP=0x0020:0000 -> mem 0x1c20
 */
#define PSP_PARA   0x192
#define SS_ABS     (0x1a2 + 0x1b51)
#define SP_INIT    0xcb14

void start(void);

static void setup_psp(void){
    /* minimal PSP so int21 get-psp / cmd-tail probes behave */
    db *psp = &mem[PSP_PARA << 4];
    memset(psp, 0, 0x100);
    psp[0] = 0xcd; psp[1] = 0x20;              /* int 20h */
    *(dw*)(psp + 2) = 0xa000;                  /* top of memory paras */
    psp[0x2c] = 0; psp[0x2d] = 0;              /* env seg 0 */
    psp[0x80] = 1; psp[0x81] = ' '; psp[0x82] = 0x0d; /* empty cmd tail */
}

static void on_signal(int s){ (void)s; _exit(128); }

/* Guest CPU thread — all DOS/game code runs here; the main thread owns SDL
 * (video_run_loop) because X11 software-renderer presents only display when
 * made from the window's thread. */
static void *guest_main(void *arg){
    (void)arg;
    rt_set_cpu_thread();                 /* register as the IRQ0 park target */

    eax = ebx = ecx = edx = ebp = esi = edi = fs = gs = 0;
    DF = CF = ZF = SF = OF = AF = PF = IF = TF = 0;
    cx = 0xff;                                  /* dummy exe size per DOS */
    ds = es = PSP_PARA;
    ss = SS_ABS; sp = SP_INIT;
    cs = 0x1a2;                               /* seg000 para (code) */

    start();                                    /* never returns (dos_exit) */
    return NULL;
}

int main(int argc, char **argv){
    (void)argc; (void)argv;
#ifdef _WIN32
    SDL_SetMainReady();
#endif
    video_init();
#ifdef __ANDROID__
    /* bundled assets were extracted to filesDir by MainActivity before the
     * native thread started — game data files resolve relative to cwd */
    chdir(SDL_AndroidGetInternalStoragePath());
#endif
    /* SDL installs SIGINT/SIGTERM handlers that only post SDL_QUIT — useless if
     * the game isn't pumping events; force a real exit so timeouts work. */
    signal(SIGINT, on_signal); signal(SIGTERM, on_signal);
    mem_load_image();
    mem_apply_fixups();
    setup_psp();
    /* BIOS default int8 handler: F000:FEA5 -> rt_bios_int8 (tick count + int1c
     * chain). TANDYSND saves this as dword_103DE and chains to it every 3 IRQ0s
     * while a song plays (~20 Hz), keeping game countdown timers alive. */
    *(dw*)&mem[0x20] = 0xfea5;
    *(dw*)&mem[0x22] = 0xf000;

    /* Tandy/PCjr BIOS signature so the game's probe picks TANDYSND.EXE */
    mem[0xffffe] = 0xff;                        /* F000:FFFE model byte */
    mem[0xfc000] = 0x21;                        /* F000:C000 secondary marker */

    pthread_t gt;
    pthread_create(&gt, NULL, guest_main, NULL);
    pthread_detach(gt);
    video_run_loop();                           /* never returns */
    return 0;
}
