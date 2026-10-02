/* core runtime: register file, memory, stack, port dispatch */
#include "rt.h"

dd eax,ebx,ecx,edx,esi,edi,esp,ebp;
dw cs,ds,es,fs,gs,ss,ip;
int CF,ZF,SF,OF,PF,AF,DF,IF,TF;
db mem[1<<20];

dw mem_w(dd a){ return *(dw*)&mem[a]; }
db mem_b(dd a){ return mem[a]; }
void mem_ww(dd a, dw v){ *(dw*)&mem[a] = v; }
void mem_wb(dd a, db v){ mem[a] = v; }

void push(dw v){ sp -= 2; *(dw*)raddr_(ss,sp) = v; }
dw pop(void){ dw v = *(dw*)raddr_(ss,sp); sp += 2; return v; }
void pushf(void){ push((dw)(CF|(PF<<2)|(AF<<4)|(ZF<<6)|(SF<<7)|(TF<<8)|(IF<<9)|(DF<<10)|(OF<<11))); }
void popf(void){ dw f = pop(); CF=f&1; PF=(f>>2)&1; AF=(f>>4)&1; ZF=(f>>6)&1; SF=(f>>7)&1; TF=(f>>8)&1; IF=(f>>9)&1; DF=(f>>10)&1; OF=(f>>11)&1; }

dw rol16(dw v,int c){ c&=15; return (dw)((v<<c)|(v>>(16-c))); }
dw ror16(dw v,int c){ c&=15; return (dw)((v>>c)|(v<<(16-c))); }
dw rcl16(dw v,int c){ c%=17; if(!c) return v; return (dw)((v<<c)|(v>>(17-c))|((dd)CF<<(c-1))); }
dw rcr16(dw v,int c){ c%=17; dd t=v|((dd)CF<<16); return (dw)(t>>c); }

/* port I/O — dispatch to video/sound/timer handlers */
dw in(dw p){ return rt_in(p); }
void out(dw p,dw v){ rt_out(p,v); }
void swi(int n){ /* printer/misc interrupt — not used by game */ }

void indirect_jump(void){ fprintf(stderr,"indirect jump hit — unmodeled\n"); }
void __dispatch_call_ext(void){ fprintf(stderr,"dispatch_call_ext hit\n"); }

void rt_exit(int code){ fprintf(stderr,"exit code %d caller=%p\n", code, __builtin_return_address(0)); exit(code); }

int rt_trace;
dd rt_i8_cnt, rt_1c_cnt;   /* TICKSTAT: BIOS int8 / int1c dispatch counts */
__attribute__((constructor)) static void rt_trace_init(void){ rt_trace = getenv("M2C_TRACE") ? 1 : 0; }

/* M2C_WPOLL=<linear-addr>: spawn a thread polling a mem cell; on each change
 * dump the guest stack so the writing proc can be identified offline.
 * M2C_WTRAP=1: also SIGUSR2 the main thread; its handler prints the host RIP
 * (interrupted C proc) from ucontext. */
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <ucontext.h>
#ifndef REG_RIP
#define REG_RIP 16   /* x86_64 gregs index */
#endif
static pthread_t wsig_tid;
static void wsig(int s_, siginfo_t *sinfo, void *uctx){
    ucontext_t *u = uctx; (void)s_; (void)sinfo;
    extern int main(int, char**);
    fprintf(stderr, "WSIG rip=%p off_main=%lx\n", (void*)u->uc_mcontext.gregs[REG_RIP],
            (long)((char*)u->uc_mcontext.gregs[REG_RIP] - (char*)main));
}
static void *wpoll_th(void *arg){
    dd wa = strtoul(getenv("M2C_WPOLL"), 0, 16);
    db last = mem[wa];
    for(;;){
        db v = mem[wa];
        if (v != last){
            fprintf(stderr, "WPOLL %x: %02x -> %02x  ss:sp=%x:%x  ds:si=%x:%x es:di=%x:%x ax=%x bx=%x cx=%x bp=%x\n",
                    wa, last, v, ss, sp, ds, si, es, di, ax, bx, cx, bp);
            for (int i = 0; i < 24; i++)
                fprintf(stderr, "   [sp+%02x] %04x", i*2, *(dw*)raddr_(ss, sp+i*2));
            fprintf(stderr, "\n");
            last = v;
            if (getenv("M2C_WTRAP") &&
                v >= (getenv("M2C_WTRAP_GE") ? strtoul(getenv("M2C_WTRAP_GE"),0,16) : 0))
                pthread_kill(wsig_tid, SIGUSR2);
        }
        usleep(2);
    }
}
__attribute__((constructor)) static void wpoll_init(void){
    if (getenv("M2C_WPOLL")){
        wsig_tid = pthread_self();
        struct sigaction sa = {0}; sa.sa_sigaction = wsig; sa.sa_flags = SA_SIGINFO;
        sigaction(SIGUSR2, &sa, 0);
        pthread_t t; pthread_create(&t, 0, wpoll_th, 0); pthread_detach(t);
    }
}
/* M2C_TRATE=1: per-tag call counters for rt_tracef — reported by tick_cb */
struct trc_ent { const char *t; unsigned n; };
static struct trc_ent *trate_tbl; static int trate_n;
static void rt_trate_poke(void *tbl, int n){ trate_tbl = tbl; trate_n = n; }
void rt_trate_report(void){
    static unsigned prev[512];
    if (!trate_tbl) return;
    fprintf(stderr, "TRATE:");
    /* print top 8 movers since last report */
    struct { const char *t; unsigned d; } top[8]; int nt = 0;
    for (int i = 0; i < trate_n; i++){
        unsigned d = trate_tbl[i].n - prev[i]; prev[i] = trate_tbl[i].n;
        if (!d) continue;
        int j = nt < 8 ? nt++ : 7;
        while (j > 0 && top[j-1].d < d){ if (j<8) top[j]=top[j-1]; j--; }
        if (j < 8) top[j].t = trate_tbl[i].t, top[j].d = d;
    }
    for (int i = 0; i < nt; i++) fprintf(stderr, " %s=%u", top[i].t, top[i].d);
    fputc('\n', stderr);
}
void rt_tracef(const char *tag){
    static int trate = -1;
    if (trate < 0) trate = getenv("M2C_TRATE") != NULL;
    if (trate){
        /* cheap per-tag call counter; report rates every 5s from tick_cb */
        struct trc { const char *t; unsigned n; };
        static struct trc cnt[512]; static int ncnt;
        int i;
        for (i = 0; i < ncnt && cnt[i].t != tag; i++);
        if (i == ncnt && ncnt < 512){ cnt[ncnt].t = tag; cnt[ncnt].n = 0; ncnt++; }
        if (i < ncnt) cnt[i].n++;
        rt_trate_poke(cnt, ncnt);
        return;
    }
    if (!rt_trace) return;
    fprintf(stderr, "%-12s ax=%04x bx=%04x cx=%04x dx=%04x si=%04x di=%04x ds=%04x es=%04x sp=%04x\n",
            tag, (unsigned)ax, (unsigned)bx, (unsigned)cx, (unsigned)dx,
            (unsigned)si, (unsigned)di, (unsigned)ds, (unsigned)es, (unsigned)sp);
}
