/* Direct probe of the ground-phase walker sub_1ADCE (loc_1ADD7):
 * preset facing to the converged value for each direction, feed the
 * active-low held-nibble via word_26DE4, run one walk burst, and measure
 * the fixed-point position accumulators (X: word_1DCFF+1/1DD1F+1,
 * Y: word_1DD3F+1).  With the byte-carry fix, positive velocity must
 * accumulate the sub-tile fraction into the integer position bytes. */
#include "../rt.h"
#include "../data_syms.h"
#include "../procs.h"
#include <stdio.h>
#include <string.h>

extern void move_dir_tick(void);

static void run(const char *name, unsigned held, unsigned face){
    /* fresh position accumulators each run */
    *(db*)(&word_1dcff)=0; *(db*)(((db*)&word_1dcff)+1)=0x80;
    *(db*)(&word_1dd1f)=0; *(db*)(((db*)&word_1dd1f)+1)=0x01;
    *(db*)(&word_1dd3f)=0; *(db*)(((db*)&word_1dd3f)+1)=0x80;
    byte_2a9e3=0; byte_2a9e4=0;
    byte_2a9f9=face;            /* converged facing for this direction */
    byte_2a9e2=0xF0;            /* distance budget */
    word_26de4=(~held)&0x3F;    /* active-low direction nibble */

    int xi0=*(db*)(((db*)&word_1dcff)+1), xh0=*(db*)(((db*)&word_1dd1f)+1), y0=*(db*)(((db*)&word_1dd3f)+1);
    for(int i=0;i<20;i++){ byte_2a9e1=0; move_dir_tick(); }
    int xi1=*(db*)(((db*)&word_1dcff)+1), xh1=*(db*)(((db*)&word_1dd1f)+1), y1=*(db*)(((db*)&word_1dd3f)+1);
    printf("%-5s held=%02x de4=%02x  X(hi:lo) %02x:%02x->%02x:%02x  Y %02x->%02x  face=%02x\n",
           name, held, (unsigned)word_26de4&0xff, xh0,xi0, xh1,xi1, y0,y1, byte_2a9f9);
}

int main(void){
    mem_load_image(); mem_apply_fixups();
    ds = mem_w(0x1a20);      /* relocated data segment */
    es = ds;
    printf("ds=%04x\n", ds);
    run("UP",   0x01, 0x00);
    run("DOWN", 0x02, 0x10);
    run("LEFT", 0x04, 0x08);
    run("RIGHT",0x08, 0x18);
    return 0;
}
