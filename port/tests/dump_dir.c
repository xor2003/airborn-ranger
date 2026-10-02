#include "../rt.h"
#include "../data_syms.h"
#include <stdio.h>
int main(void){
    mem_load_image(); mem_apply_fixups();
    dw dsp = mem_w(0x1a20);
    printf("facing idx -> Xvel[dw](signed) Yvel[dw](signed):\n");
    for(int k=0;k<16;k+=2){
        dw xv=*(dw*)raddr(dsp,k-0x249B), yv=*(dw*)raddr(dsp,k-0x2497);
        printf("  faceidx=%2d Xvel=%04x(%5d) Yvel=%04x(%5d)\n",k,xv,(short)xv,yv,(short)yv);
    }
    return 0;
}
