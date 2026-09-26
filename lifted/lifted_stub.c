/* stub definitions so lifted code links — analysis artifact only */
#include "lifted.h"
dd eax,ebx,ecx,edx,esi,edi,esp,ebp;
dw ax,bx,cx,dx,si,di,sp,bp,cs,ds,es,fs,gs,ss,ip;
db al,ah,bl,bh,cl,ch,dl,dh;
int CF,ZF,SF,OF,PF,AF,DF,IF,TF;
db mem[1<<20];
void push(dw v){ sp -= 2; *(dw*)raddr_(ss,sp) = v; }
dw pop(void){ dw v = *(dw*)raddr_(ss,sp); sp += 2; return v; }
void pushf(void){}
void popf(void){}
dw rol16(dw v,int c){c&=15;return (v<<c)|(v>>(16-c));}
dw ror16(dw v,int c){c&=15;return (v>>c)|(v<<(16-c));}
dw rcl16(dw v,int c){return v;}
dw rcr16(dw v,int c){return v;}
dw in(dw p){return 0;}
void out(dw p,dw v){}
void swi(int n){}
void dos_int21(dw a){}
void bios_video(dw a){}
void bios_kbd(dw a){}
void indirect_jump(void){}
void __dispatch_call_ext(void){}
