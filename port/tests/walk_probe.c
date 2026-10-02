/* Direct probe of the object-path player handler sub_189e2 (funcs_1892F[1]).
 * Sets a fake ranger object at slot bx, feeds each held-direction nibble via
 * word_26DE4 (active-low), ticks the handler, and reports the 24-bit fixed-point
 * position (X=[4041]:[4085]:[4151], Y=[401F]:[4063]:[412F]).
 * Goal: verify the ground-move path translates for all 4 directions. */
#include "../rt.h"
#include "../data_syms.h"
#include "../procs.h"
#include <stdio.h>
#include <string.h>

extern void sub_189e2(void);   /* player tick (object type 1) */

#define O(off) (*(volatile db*)raddr(ds, (unsigned)(bx - (off))))

static unsigned getx(unsigned b){ /* 24-bit pos integer part: int:low */
    return (unsigned)mem[0xE8A0 + ((b - 0x4041) & 0xFFFF)] * 256 +
           (unsigned)mem[0xE8A0 + ((b - 0x4085) & 0xFFFF)];
}
static unsigned gety(unsigned b){
    return (unsigned)mem[0xE8A0 + ((b - 0x401F) & 0xFFFF)] * 256 +
           (unsigned)mem[0xE8A0 + ((b - 0x4063) & 0xFFFF)];
}

static void setup(unsigned b){
    /* global gates for clean directional walk */
    byte_26dd5 = 1;      /* directional control */
    byte_1dcb0 = 0;
    byte_2a7aa = 0;
    word_2a8a2 = 0;
    word_2a8a0 = 0;
    byte_29712 = 0;      /* wounds */
    byte_29711 = 0;
    byte_26de6 = 0; byte_26de7 = 0;
    byte_28cc0 = 0; byte_28ca2 = 0; byte_28cd2 = 0;
    word_265aa = 0;
    byte_2aa63 = 0x0B;   /* makes sub_16b72 early-path benign */
    byte_2aa5c = 0; byte_2aa60 = 0; byte_2aa62 = 0;
    /* object record */
    bx = b;
    O(0x4173) = 1;       /* type: ranger */
    O(0x3B79) = 0;       /* alive */
    O(0x3C67) = 0xFF;    /* posture: negative -> walk allowed */
    O(0x3EA9) = 0;       /* state */
    O(0x3ECB) = 0;       /* anim frame */
    O(0x3A47) = 0;       /* pace counter -> fires immediately */
    O(0x3E0F) = 0;
    O(0x3999) = 0;
    O(0x3E65) = 0;
    O(0x3A69) = 0;
    O(0x3E43) = 0;
    O(0x3F0F) = 0;
    O(0x3E87) = 0;       /* rotate momentum */
    O(0x3EED) = 0xFF;    /* no target initially */
    O(0x3E21) = 0x10;    /* facing = south */
    O(0x4151) = 0; O(0x4085) = 0x80; O(0x4041) = 0x40;   /* X = 0x4080 */
    O(0x412F) = 0; O(0x4063) = 0x80; O(0x401F) = 0x40;   /* Y = 0x4080 */
}

static void run(const char *name, unsigned held){
    setup(0);
    word_26de4 = (~held) & 0x3F;
    unsigned x0=getx(0), y0=gety(0);
    int moved=0;
    for (int i=0;i<120;i++){
        byte_2a7aa=0; /* handler clears/reads this */
        sub_189e2();
        unsigned x=getx(0),y=gety(0);
        if (x!=x0||y!=y0) moved=1;
        /* keep it alive: reassert clean walk state each tick */
        O(0x3B79)=0; byte_29712=0;
    }
    printf("%-5s held=%02x de4=%02x nib=%02x  X %4x->%4x  Y %4x->%4x  face=%02x tgt=%02x %s\n",
        name, held, (unsigned)word_26de4&0xff, byte_2976c,
        x0, getx(0), y0, gety(0),
        mem[0xE8A0+((0-0x3E21)&0xFFFF)], mem[0xE8A0+((0-0x3EED)&0xFFFF)],
        moved?"MOVED":"stuck");
}

int main(void){
    mem_load_image(); mem_apply_fixups();
    ds = mem_w(0x1a20); es = ds;
    printf("ds=%04x\n", ds);
    run("UP",   0x01);
    run("DOWN", 0x02);
    run("LEFT", 0x04);
    run("RIGHT",0x08);
    return 0;
}
