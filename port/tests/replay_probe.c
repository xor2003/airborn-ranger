/* Replay probe: load a live-combat mem[] snapshot (M2C_SNAP dump), then drive
 * the real player handler sub_189e2 (bx=0) with each held direction via
 * word_26DE4 (active-low). Reports the ranger's 24-bit X/Y plus the collision
 * decision inputs (staged xy, cell, terrain, flag bytes) each tick so LEFT
 * rollback can be compared against RIGHT on identical state. */
#include "../rt.h"
#include "../data_syms.h"
#include "../procs.h"
#include <stdio.h>
#include <string.h>

extern void sub_189e2(void);   /* player tick (object type 1) */

#define O(off) (*(volatile db*)raddr(ds, (unsigned)(bx - (off))))

static unsigned getx(unsigned b){
    return (unsigned)mem[0xE8A0 + ((b - 0x4041) & 0xFFFF)] * 256 +
           (unsigned)mem[0xE8A0 + ((b - 0x4085) & 0xFFFF)];
}
static unsigned gety(unsigned b){
    return (unsigned)mem[0xE8A0 + ((b - 0x401F) & 0xFFFF)] * 256 +
           (unsigned)mem[0xE8A0 + ((b - 0x4063) & 0xFFFF)];
}

static int load(const char *path){
    FILE *f = fopen(path, "rb");
    if (!f) return 0;
    fread(mem, 1, 1 << 20, f); fclose(f);
    return 1;
}

static void run(const char *name, unsigned held, unsigned face){
    if (!load("/tmp/ar_mem.bin")){ puts("no snap"); return; }
    ds = 0xE8A; es = ds;
    bx = 0;                        /* ranger slot */
    /* converge facing/target to the direction so the move (not rotate) runs */
    O(0x3E21) = face;              /* facing */
    O(0x3EED) = face;              /* target */
    O(0x3C67) = 0xFF;              /* walk-allowed posture */
    O(0x3ECB) = 0x80;              /* force move-cycle armed */
    O(0x3EA9) = 0;
    byte_29b50 = face;
    word_26de4 = (~held) & 0x3F;
    unsigned x0 = getx(0), y0 = gety(0);
    printf("== %-5s held=%02x face=%02x start X=%x Y=%x\n", name, held, face, x0, y0);
    for (int i = 0; i < 8; i++){
        sub_189e2();
        printf("   t%d X=%x Y=%x nib=%02x face=%02x tgt=%02x gate=%02x | staged=%04x,%04x cell=%02x,%02x "
               "terr=%02x attr=%02x f5e=%02x f5f=%02x f62=%02x f64=%02x f63=%02x sf9=%02x\n",
               i, getx(0), gety(0), byte_2976c,
               O(0x3E21), O(0x3EED), O(0x3ECB),
               word_2aa69, word_2aa6b, byte_2a960, byte_2a961,
               byte_2aa61, byte_2aa63, byte_2aa5e, byte_2aa5f,
               byte_2aa62, byte_2aa64, byte_2aa63, byte_29b50);
        O(0x3B79) = 0; byte_29712 = 0;          /* keep alive */
    }
}

int main(void){
    mem_load_image(); mem_apply_fixups();      /* base image first (tables etc.) */
    run("RIGHT", 0x08, 0x18);
    run("LEFT",  0x04, 0x08);
    run("DOWN",  0x02, 0x10);
    run("UP",    0x01, 0x00);
    return 0;
}
