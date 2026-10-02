#include "../rt.h"
#include "../data_syms.h"
#include "../procs.h"
#include <stdio.h>
extern void move_dir_tick(void);
/* dump map region to see if real tile data is loaded */
static void dumpmap(void){
    dd base = ((dd)ds<<4) + 0x9736;
    int nz=0; printf("map[0x9736] first row: "); 
    for(int i=0;i<32;i++){printf("%02x ",mem[base+i]); if(mem[base+i])nz++;}
    printf("\n map nonzero bytes in first 256: ");
    int c=0; for(int i=0;i<256;i++) if(mem[base+i])c++;
    printf("%d\n", c);
    printf("word_2b253(tileattr table)=%04x word_2a9ce(camY)=%d\n",word_2b253,word_2a9ce);
    /* tile attr table sample */
    dd tb=((dd)ds<<4)+word_2b253;
    printf("tileattr[0..15]:"); for(int i=0;i<16;i++)printf(" %02x",mem[tb+i]); printf("\n");
}
static void setpos(int xlo,int xhi,int y){
    *(db*)(&word_1dcff)=0x80; *(db*)(((db*)&word_1dcff)+1)=xlo;
    *(db*)(&word_1dd1f)=0x01; *(db*)(((db*)&word_1dd1f)+1)=xhi;
    *(db*)(&word_1dd3f)=0x80; *(db*)(((db*)&word_1dd3f)+1)=y;
}
static void run(const char*nm,unsigned held,unsigned face){
    setpos(0x60,0x00,0x64);          /* X=0x0060 -> cellX=(0x60-0x1a)>>3=8 (in bounds) */
    byte_2a9e3=0; byte_2a9e4=0; byte_2a9f9=face; byte_2a9e2=0xF0;
    word_26de4=(~held)&0x3F;
    int cross=0,moved=0,res3=0;
    for(int i=0;i<400;i++){
        byte_2a9e1=0; CF=0;
        dw p0=word_1dcff,q0=word_1dd3f,f0=word_1dd1f;
        move_dir_tick();
        if(CF){cross++; if(word_2aa0d==3)res3++;}
        if(word_1dcff!=p0||word_1dd3f!=q0||word_1dd1f!=f0)moved++;
    }
    printf("%-5s face=%02x->%02x  X=%02x:%02x Y=%02x  moved=%d cross=%d blocked=%d lastres=%d cell=(%d,%d)\n",
        nm,face,byte_2a9f9,*(db*)(((db*)&word_1dd1f)+1),*(db*)(((db*)&word_1dcff)+1),
        *(db*)(((db*)&word_1dd3f)+1),moved,cross,res3,word_2aa0d,byte_2a9df,byte_2a9e0);
}
int main(void){
    mem_load_image(); mem_apply_fixups(); ds=mem_w(0x1a20); es=ds;
    dumpmap();
    run("UP",0x01,0x00); run("DOWN",0x02,0x10); run("LEFT",0x04,0x08); run("RIGHT",0x08,0x18);
    return 0;
}
