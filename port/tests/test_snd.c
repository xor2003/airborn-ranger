/* unit tests for snd.c: SN76496 register model, latch semantics, volume
 * table, noise modes, MIDI helpers. includes snd.c to reach statics —
 * tests call the pure helpers directly (no audio device needed). */
#include <SDL2/SDL.h>
#include "../rt.h"
#include <stdio.h>
#include <math.h>

volatile int rt_isr_ctx;                           /* video.c provides for real */
dw rt_in(dw p){ (void)p; return 0; }
void rt_out(dw p, dw v){ (void)p; (void)v; }

#include "../snd.c"

static int fails, checks;
#define CHECK(cond) do{ checks++; if(!(cond)){ fails++; \
    fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); } }while(0)

int main(void){
    setenv("SDL_AUDIODRIVER", "dummy", 0);     /* snd_init must not hit real audio */

    /* --- volume table: non-increasing, capped top steps, 0xF = silence --- */
    snd_reset();
    CHECK(vol_table[15] == 0);
    for (int i = 0; i < 15; i++) CHECK(vol_table[i] >= vol_table[i+1]);
    CHECK(vol_table[0] == SND_MAX_OUTPUT/4);
    CHECK(vol_table[1] > vol_table[2]);         /* real attenuation kicks in */

    /* --- latch semantics: 0x80-bit selects reg+low nibble; data byte = high 6 bits */
    snd_reset();
    snd_write(0x80 | 0x05);          /* ch0 freq, low nibble 5 */
    CHECK((snd_reg[0] & 0x0f) == 5);
    snd_write(0x24);                 /* data: high 6 bits = 0x24 */
    CHECK(snd_reg[0] == ((0x24 << 4) | 0x05));
    CHECK(period[0] == ((0x24 << 4) | 0x05));
    snd_write(0x80 | 0x00);          /* low nibble 0 keeps high bits */
    CHECK(period[0] == (0x24 << 4));

    /* period 0 in reg -> clamped to 0x400 (1 << 10, "never toggle") */
    snd_write(0x80 | 0x00); snd_write(0x80 | 0x00); snd_write(0x00);
    CHECK(snd_reg[0] == 0);
    CHECK(period[0] == 0x400);

    /* --- volume regs: odd latch numbers write attenuation index --- */
    snd_write(0x80 | 0x10 | 0x0f);   /* ch0 volume = 15 -> silent */
    CHECK(volume[0] == 0);
    snd_write(0x80 | 0x10 | 0x00);   /* ch0 volume = 0 -> max */
    CHECK(volume[0] == vol_table[0]);

    /* --- noise channel reg 6 --- */
    snd_write(0x80 | 0x60 | 0x04);   /* white noise, rate bits 0 */
    CHECK((snd_reg[6] & 7) == 4);
    CHECK(period[3] == (1 << 5));    /* rate 0 -> 1<<(5+0) */
    snd_write(0x80 | 0x60 | 0x03);   /* rate 3 = steal ch2 period */
    snd_write(0x80 | 0x40 | 0x02);   /* ch2 low nibble 2 */
    snd_write(0x10);                 /* ch2 high bits 0x10 */
    CHECK(period[3] == period[2] * 2);

    /* --- freq_to_midi --- */
    CHECK(freq_to_midi(0) == -1);
    CHECK(freq_to_midi(255) == 69);  /* ~440Hz: 3579545/(32*255)=438.9 */
    CHECK(freq_to_midi(128) > 69);   /* higher freq = higher note */
    CHECK(freq_to_midi(1) > 0);      /* max freq clamps to <=127 */
    CHECK(freq_to_midi(1) <= 127);

    /* --- snd_tick toggles square output at period rate --- */
    snd_reset();
    snd_write(0x80 | 0x00 | 0x04); snd_write(0x00);   /* period 4 */
    CHECK(output[0] == 0);
    for (int i = 0; i < 4; i++) snd_tick();
    CHECK(output[0] == 1);
    for (int i = 0; i < 4; i++) snd_tick();
    CHECK(output[0] == 0);

    /* --- midi_synth_sample: silent when volumes 0 --- */
    snd_reset();
    CHECK(midi_synth_sample(1.0/22050) == 0);
    snd_write(0x80 | 0x10 | 0x00);   /* ch0 full vol */
    snd_write(0x80 | 0x00 | 0x05); snd_write(0x10);   /* period 0x105 */
    int s = 0; for (int i = 0; i < 100; i++) s |= midi_synth_sample(1.0/22050);
    CHECK(s != 0);                                  /* audible output */

    /* --- vlq encoding --- */
    FILE *f = tmpfile();
    vlq(0x00, f); vlq(0x7f, f); vlq(0x80, f); vlq(0x2000, f);
    rewind(f);
    CHECK(fgetc(f) == 0x00); CHECK(fgetc(f) == 0x7f);
    CHECK(fgetc(f) == 0x81); CHECK(fgetc(f) == 0x00);
    CHECK(fgetc(f) == 0xc0); CHECK(fgetc(f) == 0x00);
    fclose(f);

    /* --- in_noise_mode reflects reg6 bit2 --- */
    snd_write(0x80 | 0x60 | 0x04);
    CHECK(in_noise_mode() == 1);
    snd_write(0x80 | 0x60 | 0x00);
    CHECK(in_noise_mode() == 0);

    /* --- snd_sample: negated mix of gated volumes --- */
    snd_reset();
    output[0] = 1; volume[0] = vol_table[0];
    CHECK(snd_sample() == -vol_table[0]);
    output[0] = 0; output[1] = 1; volume[1] = vol_table[2];
    CHECK(snd_sample() == -vol_table[2]);
    output[0] = output[1] = 1;
    CHECK(snd_sample() == -(vol_table[0] + vol_table[2]));

    /* --- rt_tnd_write: the port-0xc0 entry point --- */
    rt_tnd_write(0xc0, 0x80 | 0x10 | 0x0f);       /* ch0 vol = silent */
    CHECK(volume[0] == 0);
    rt_tnd_write(0xc0, 0x80 | 0x00 | 0x07);       /* ch0 freq lo 7 */
    CHECK((snd_reg[0] & 0x0f) == 7);

    /* --- snd_init: idempotent; snd_cb renders into the stream buffer --- */
    snd_init();                                   /* second call early-returns */
    CHECK(started == 1);
    {   int16_t buf[256];
        engine_sel = 1;                           /* psg engine -> snd_sample */
        memset(buf, 0x11, sizeof buf);
        snd_cb(NULL, (Uint8*)buf, sizeof buf);
        int sum = 0; for (int i = 0; i < 256; i++) sum += buf[i] != 0x1111;
        CHECK(sum > 0);                           /* buffer was filled */
        engine_sel = 0;                           /* midi engine -> synth path */
        memset(buf, 0x11, sizeof buf);
        snd_cb(NULL, (Uint8*)buf, sizeof buf);
        sum = 0; for (int i = 0; i < 256; i++) sum += buf[i] != 0x1111;
        CHECK(sum > 0);
        engine_sel = -1;
    }

    /* --- rt_speaker: Tandy build routes all sound via 0xc0 (no-op) --- */
    rt_speaker(0); rt_speaker(3);

    /* --- evt_cmp ordering for the MIDI flush sort --- */
    {   MidiEvt a = {5,0,0,0}, b = {9,0,0,0}, c = {9,0,0,0};
        CHECK(evt_cmp(&a, &b) < 0);
        CHECK(evt_cmp(&b, &a) > 0);
        CHECK(evt_cmp(&b, &c) == 0);
    }

    /* --- MIDI capture: note on/off land in the file --- */
    {   const char *mp = "/tmp/ar_t_snd.mid";
        remove(mp);
        setenv("M2C_MIDI_OUT", mp, 1);
        mevn = 0; note_state[0] = -1;
        midi_init();
        CHECK(midi_on == 1);
        snd_reset();
        snd_write(0x80 | 0x00 | 0x05);            /* ch0 freq lo */
        snd_write(0x10);                          /* ch0 freq hi -> period set */
        snd_write(0x80 | 0x10 | 0x00);            /* ch0 vol max -> note on */
        int non = mevn;
        snd_write(0x80 | 0x10 | 0x0f);            /* ch0 vol 15 -> note off */
        CHECK(non >= 1 && mevn > non);            /* both events recorded */
        midi_flush();
        FILE *f = fopen(mp, "rb");
        CHECK(f != NULL);
        if (f){
            CHECK(fgetc(f) == 'M' && fgetc(f) == 'T' &&
                  fgetc(f) == 'h' && fgetc(f) == 'd');
            fclose(f);
        }
        unsetenv("M2C_MIDI_OUT"); midi_on = 0;
    }

    /* --- engine select --- */
    engine_sel = -1; unsetenv("M2C_SND_ENGINE"); CHECK(snd_engine() == 0);
    engine_sel = -1; setenv("M2C_SND_ENGINE", "psg", 1); CHECK(snd_engine() == 1);
    engine_sel = -1; setenv("M2C_SND_ENGINE", "midi", 1); CHECK(snd_engine() == 0);
    engine_sel = -1; unsetenv("M2C_SND_ENGINE");

    fprintf(stderr, "test_snd: %d checks, %d failures\n", checks, fails);
    return fails ? 1 : 0;
}
