/* Tandy 3-voice sound (SN76496 / NCR8496) on port 0xC0.
 * Chip model from MAME's sn76496 (via dosbox-staging), rendered through SDL2.
 * PSG clock = 14318180/4 = 3579545 Hz. Two engines share register state:
 *   midi (default): tone chans as triangle voices w/ envelope; noise = percussion
 *   psg           : cycle-level square-wave emulation
 * Select: M2C_SND_ENGINE=midi|psg.  MIDI capture: M2C_MIDI_OUT=path (either engine).
 */
#include <SDL2/SDL.h>
#include <math.h>
#include <time.h>
#include "rt.h"

#define SND_CLOCK 3579545
#define SND_MAX_OUTPUT 0x7fff
#define SND_TICK_HZ (SND_CLOCK / 16.0)
#define SND_FEEDBACK_MASK 0x8000
#define SND_TAP1 0x02
#define SND_TAP2 0x20

static int32_t snd_reg[8];
static int last_reg;
static int32_t period[4], cnt[4], output[4], volume[4], vol_table[16];
static uint32_t rng;
static double tick_accum;
static int started;
static SDL_AudioDeviceID snd_dev;
static double ticks_per_sample;
static double srate;
static int engine_sel = -1;             /* 0=midi 1=psg */
static double mphase[4], mamp[4];

/* ---- MIDI capture (.mid file, opt-in via M2C_MIDI_OUT) ---- */
typedef struct { uint64_t us; uint8_t st,a,b; } MidiEvt;
static MidiEvt *mev; static int mevn, mevcap;
static int note_state[4] = {-1,-1,-1,-1};
static int midi_on;
static uint64_t t0;

static uint64_t now_us(void){
    struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec*1000000 + ts.tv_nsec/1000;
}
static int freq_to_midi(int p){
    if (p <= 0) return -1;
    double f = SND_CLOCK / (32.0 * p);
    if (f < 8.0) return -1;
    int n = (int)lround(69.0 + 12.0 * log2(f / 440.0));
    return n < 0 ? 0 : (n > 127 ? 127 : n);
}
static void midi_push(uint8_t st, uint8_t a, uint8_t b){
    if (mevn == mevcap){ mevcap = mevcap ? mevcap*2 : 4096; mev = realloc(mev, mevcap*sizeof(MidiEvt)); }
    mev[mevn].us = now_us() - t0; mev[mevn].st = st; mev[mevn].a = a; mev[mevn].b = b; mevn++;
}
static void midi_commit(int c){
    static int ntr, tstate[4] = {-2,-2,-2,-2};
    static uint64_t tt0;
    if (getenv("M2C_SND_TRACE")){
        if (!tt0) tt0 = now_us();
        int note = -1;
        if (volume[c] > 0) note = (c < 3) ? freq_to_midi(period[c]) : 38;
        if (note != tstate[c]){
            tstate[c] = note;
            fprintf(stderr, "nt %6ums ch%d note=%d per=%d\n",
                    (unsigned)((now_us()-tt0)/1000), c, note, (int)period[c]);
        }
    }
    if (!midi_on) return;
    int note = -1;
    if (volume[c] > 0) note = (c < 3) ? freq_to_midi(period[c]) : 38;
    if (note == note_state[c]) return;
    int mch = (c < 3) ? c : 9;
    if (note_state[c] >= 0) midi_push(0x80|mch, note_state[c], 0);
    if (note >= 0)            midi_push(0x90|mch, note, 100);
    note_state[c] = note;
}
static void vlq(uint32_t v, FILE *f){
    uint8_t b[5]; int n=0; b[n++]=v&0x7f;
    while (v>>=7) b[n++]=0x80|(v&0x7f);
    while (n--) fputc(b[n], f);
}
static int evt_cmp(const void *x, const void *y){
    uint64_t a=((const MidiEvt*)x)->us, b=((const MidiEvt*)y)->us;
    return a<b ? -1 : a>b ? 1 : 0;
}
static void midi_flush(void){
    if (!mevn) return;
    const char *path = getenv("M2C_MIDI_OUT");
    if (!path || !*path) return;
    FILE *f = fopen(path, "wb"); if (!f) return;
    qsort(mev, mevn, sizeof(MidiEvt), evt_cmp);
    fwrite("MThd",1,4,f); fputc(0,f);fputc(0,f);fputc(0,f);fputc(6,f);
    fputc(0,f);fputc(0,f); fputc(0,f);fputc(1,f); fputc(0,f);fputc(96,f);
    FILE *tmp = tmpfile();
    const double us_per_tick = 500000.0/96.0;
    uint64_t prev = mev[0].us;
    for (int i=0;i<mevn;i++){
        vlq((uint32_t)((mev[i].us - prev)/us_per_tick + 0.5), tmp);
        prev = mev[i].us;
        fputc(mev[i].st,tmp); fputc(mev[i].a,tmp); fputc(mev[i].b,tmp);
    }
    vlq(0,tmp); fputc(0xff,tmp); fputc(0x2f,tmp); fputc(0x00,tmp);
    long len = ftell(tmp); rewind(tmp);
    fwrite("MTrk",1,4,f);
    fputc((len>>24)&255,f);fputc((len>>16)&255,f);fputc((len>>8)&255,f);fputc(len&255,f);
    for (long i=0;i<len;i++) fputc(fgetc(tmp), f);
    fclose(tmp); fclose(f);
    fprintf(stderr, "MIDI: wrote %d events to %s\n", mevn, path);
}
static void midi_init(void){
    const char *p = getenv("M2C_MIDI_OUT");
    if (!p || !*p) return;
    midi_on = 1; t0 = now_us(); atexit(midi_flush);
}
/* ---- end capture ---- */

static void snd_reset(void){
    double out = SND_MAX_OUTPUT/4; int gain = 16;
    while (gain-- > 0) out *= 1.023292992;
    for (int i=0;i<15;i++){ vol_table[i] = (int32_t)out < SND_MAX_OUTPUT/4 ? (int32_t)out : SND_MAX_OUTPUT/4; out /= 1.258925412; }
    vol_table[15] = 0;
    memset(snd_reg,0,sizeof snd_reg); memset(period,0,sizeof period);
    memset(cnt,0,sizeof cnt); memset(output,0,sizeof output); memset(volume,0,sizeof volume);
    last_reg = 3; rng = SND_FEEDBACK_MASK; output[3] = 1; tick_accum = 0.0;
}

static int in_noise_mode(void){ return (snd_reg[6] & 4) != 0; }

static void snd_write(uint8_t data){
    int r;
    if (data & 0x80){
        r = (data & 0x70) >> 4; last_reg = r;
        if (r == 6 && ((data & 4) != (snd_reg[6] & 4))) rng = SND_FEEDBACK_MASK;
        snd_reg[r] = (snd_reg[r] & 0x3f0) | (data & 0x0f);
    } else {
        r = last_reg;
        if ((r & 1) || (r == 6)) return;
    }
    int c = r >> 1;
    switch (r){
    case 0: case 2: case 4:
        if (!(data & 0x80)) snd_reg[r] = (snd_reg[r] & 0x0f) | ((data & 0x3f) << 4);
        period[c] = snd_reg[r] ? snd_reg[r] : 0x400;
        if (r == 4 && (snd_reg[6] & 3) == 3) period[3] = period[2] << 1;
        if (!(data & 0x80)) midi_commit(c);
        break;
    case 1: case 3: case 5: case 7:
        volume[c] = vol_table[data & 0x0f];
        if (!(data & 0x80)) snd_reg[r] = (snd_reg[r] & 0x3f0) | (data & 0x0f);
        midi_commit(c);
        break;
    case 6:
        if (!(data & 0x80)) snd_reg[r] = (snd_reg[r] & 0x3f0) | (data & 0x0f);
        { int n = snd_reg[6]; period[3] = ((n & 3) == 3) ? (period[2] << 1) : (1 << (5 + (n & 3))); }
        midi_commit(3);
        break;
    }
}

static void snd_tick(void){
    for (int i=0;i<3;i++)
        if (--cnt[i] <= 0){ output[i] ^= 1; cnt[i] = period[i]; }
    if (--cnt[3] <= 0){
        if (((rng & SND_TAP1) != 0) != (((rng & SND_TAP2) != SND_TAP2) && in_noise_mode()))
            { rng >>= 1; rng |= SND_FEEDBACK_MASK; }
        else rng >>= 1;
        output[3] = rng & 1; cnt[3] = period[3];
    }
}
static int snd_sample(void){
    int o = (output[0]?volume[0]:0)+(output[1]?volume[1]:0)+(output[2]?volume[2]:0)+(output[3]?volume[3]:0);
    return -o; /* SND_NEGATE */
}
static int midi_synth_sample(double dt){
    int acc = 0;
    for (int c=0;c<3;c++){
        double f = period[c] > 0 ? SND_CLOCK/(32.0*period[c]) : 0.0;
        double tgt = (double)volume[c] / SND_MAX_OUTPUT;
        double rate = tgt > mamp[c] ? 400.0 : 50.0;
        double k = rate*dt > 1.0 ? 1.0 : rate*dt;
        mamp[c] += (tgt - mamp[c]) * k;
        mphase[c] += f*dt; mphase[c] -= floor(mphase[c]);
        double t = mphase[c];
        double tri = t < 0.5 ? 4.0*t - 1.0 : 3.0 - 4.0*t;
        acc += (int)(tri * mamp[c] * SND_MAX_OUTPUT);
    }
    acc += output[3] ? volume[3] : 0;
    return -acc;
}
static int snd_engine(void){
    if (engine_sel >= 0) return engine_sel;
    const char *e = getenv("M2C_SND_ENGINE");
    engine_sel = (e && (!strcmp(e,"psg")||!strcmp(e,"square")||!strcmp(e,"tandy")||!strcmp(e,"chip"))) ? 1 : 0;
    return engine_sel;
}
static void snd_cb(void *u, Uint8 *stream, int len){
    int16_t *o = (int16_t*)stream; int n = len/2; (void)u;
    int midi = snd_engine() == 0;
    double dt = srate > 0 ? 1.0/srate : 0.0;
    for (int i=0;i<n;i++){
        tick_accum += ticks_per_sample;
        while (tick_accum >= 1.0){ tick_accum -= 1.0; snd_tick(); }
        o[i] = (int16_t)(midi ? midi_synth_sample(dt) : snd_sample());
    }
}
static void snd_init(void){
    if (started) return;
    started = 1; snd_reset(); midi_init();
    if (!(SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO)) SDL_InitSubSystem(SDL_INIT_AUDIO);
    SDL_AudioSpec want = {0}, got;
    want.freq = 22050; want.format = AUDIO_S16SYS; want.channels = 1; want.samples = 512; want.callback = snd_cb;
    snd_dev = SDL_OpenAudioDevice(NULL, 0, &want, &got, 0);
    if (!snd_dev){ fprintf(stderr, "Tandy audio failed: %s\n", SDL_GetError()); return; }
    ticks_per_sample = SND_TICK_HZ / got.freq; srate = got.freq;
    SDL_PauseAudioDevice(snd_dev, 0);
}

void rt_tnd_write(dw p, dw v){
    (void)p;
    snd_init();
    static dd nw;
    if (getenv("M2C_SND_STAT") && (++nw % 2000) == 0)
        fprintf(stderr, "snd[%u] f0=%03x v0=%x f1=%03x v1=%x f2=%03x v2=%x v3=%x\n",
                nw, snd_reg[0]&0x3ff, snd_reg[1]&0xf, snd_reg[2]&0x3ff,
                snd_reg[3]&0xf, snd_reg[4]&0x3ff, snd_reg[5]&0xf, snd_reg[7]&0xf);
    /* In ISR context (rt_isr_ctx) the main thread is parked by the timer
     * thread — taking the audio lock could deadlock if main held it, so apply
     * the PSG register write unlocked (worst case: one torn sample). */
    if (snd_dev && !rt_isr_ctx) SDL_LockAudioDevice(snd_dev);
    snd_write((uint8_t)v);
    if (snd_dev && !rt_isr_ctx) SDL_UnlockAudioDevice(snd_dev);
}
void rt_speaker(dw v){ (void)v; /* PC speaker: Tandy build routes all sound via 0xc0 */ }
