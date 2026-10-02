/* dump the ground-walk direction/facing/velocity tables (move_dir_tick /
 * loc_1ADD7) to see how the active-low direction nibble maps to facing and
 * how facing maps to signed X/Y velocity. */
#include "../rt.h"
#include "../procs.h"
#include <stdio.h>

int main(void){
    mem_load_image();
    mem_apply_fixups();
    dw dsv = *(dw*)&mem[0x1a20];       /* seg_10000 -> runtime ds */
    printf("ds=%04x  flatbase=%06x\n", dsv, (dsv<<4));
    /* dir table: raddr(ds, si-0x24CF) -> flat (ds<<4) + ((si-0x24CF)&0xFFFF)
     *          = base + si + 0xDB31 (mod 2^16)                        */
    printf("dir table [si-0x24CF], si=0..15:\n  ");
    for (int i=0;i<16;i++)
        printf("%02x ", mem[(dsv<<4)+((i-0x24CF)&0xFFFF)]);
    printf("\n");
    /* velocity tables: raddr(ds, si-0x249B) -> base + si + 0xDB65 */
    printf("xvel [si-0x249B], si=0,2,..14 (signed words):\n  ");
    for (int i=0;i<16;i+=2)
        printf("%04x ", *(dw*)&mem[(dsv<<4)+((i-0x249B)&0xFFFF)]);
    printf("\nyvel [si-0x2497], si=0,2,..14 (signed words):\n  ");
    for (int i=0;i<16;i+=2)
        printf("%04x ", *(dw*)&mem[(dsv<<4)+((i-0x2497)&0xFFFF)]);
    printf("\n");
    return 0;
}
