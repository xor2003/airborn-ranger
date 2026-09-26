/* unit tests for rt.c: stack ops, flag pack/unpack, rotates, raddr, mem helpers */
#include "../rt.h"
#include <stdio.h>

static int fails, checks;
#define CHECK(cond) do{ checks++; if(!(cond)){ fails++; \
    fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); } }while(0)

/* port I/O lives in video.c — stub it so rt.o links standalone */
dw rt_in(dw p){ (void)p; return 0; }
void rt_out(dw p, dw v){ (void)p; (void)v; }

int main(void){
    /* raddr_: seg:off -> linear, 16-bit wrap on both */
    CHECK(raddr_(0,0) == &mem[0]);
    CHECK(raddr_(0x1a2,0) == &mem[0x1a20]);
    CHECK(raddr_(0xffff,0xffff) == &mem[0x10ffef]);
    CHECK(raddr_(0xe8a,0xad7) == &mem[0xe8a0+0xad7]);

    /* mem helpers */
    mem_ww(0x100, 0x1234); CHECK(mem_w(0x100) == 0x1234);
    CHECK(mem_b(0x100) == 0x34 && mem_b(0x101) == 0x12);
    mem_wb(0x101, 0xab);   CHECK(mem_w(0x100) == 0xab34);

    /* push/pop LIFO on guest stack */
    ss = 0x1000; sp = 0x0100;
    push(0x1111); push(0x2222);
    CHECK(sp == 0xfc);
    CHECK(*(dw*)raddr_(ss,sp) == 0x2222);
    CHECK(pop() == 0x2222); CHECK(pop() == 0x1111);
    CHECK(sp == 0x100);

    /* pushf packs x86 FLAGS layout: CF0 PF2 AF4 ZF6 SF7 TF8 IF9 DF10 OF11 */
    CF=1; PF=1; AF=1; ZF=1; SF=1; TF=1; IF=1; DF=1; OF=1;
    pushf();
    CHECK(*(dw*)raddr_(ss,sp) == (1|4|0x10|0x40|0x80|0x100|0x200|0x400|0x800));
    CF=PF=AF=ZF=SF=TF=IF=DF=OF=0;
    popf();
    CHECK(CF&&PF&&AF&&ZF&&SF&&TF&&IF&&DF&&OF);

    /* flag restore semantics used by guest_irq0: snapshot/restore must be exact */
    CF=0;ZF=1;SF=0;OF=1;PF=0;AF=1;DF=0;IF=1;TF=0;
    int cf=CF,zf=ZF,sf=SF,of=OF,pf=PF,af=AF,df=DF,iff=IF,tf=TF;
    CF=ZF=SF=OF=PF=AF=DF=IF=TF=1;              /* ISR dirties everything */
    CF=cf; ZF=zf; SF=sf; OF=of; PF=pf; AF=af; DF=df; IF=iff; TF=tf;
    CHECK(CF==0&&ZF==1&&SF==0&&OF==1&&PF==0&&AF==1&&DF==0&&IF==1&&TF==0);

    /* rol16/ror16 */
    CHECK(rol16(0x8001,1) == 0x0003);
    CHECK(rol16(0x1234,8) == 0x3412);
    CHECK(ror16(0x8001,1) == 0xc000);
    CHECK(ror16(0x1234,4) == 0x4123);
    CHECK(rol16(0xffff,0) == 0xffff);

    /* rcl16: rotate through carry, count mod 17 */
    CF=0; CHECK(rcl16(0x8000,1) == 0x0000);     /* top bit -> CF */
    CF=1; CHECK(rcl16(0x0000,1) == 0x0001);     /* CF -> bit0 */
    CF=1; CHECK(rcl16(0x0000,17) == 0x0000);    /* 17 = full circle w/ carry */
    /* rcr16 */
    CF=1; CHECK(rcr16(0x0000,1) == 0x8000);     /* CF -> bit15 */
    CF=0; CHECK(rcr16(0x0001,1) == 0x0000);

    /* 8/16-bit register views share storage (partial-register aliasing) */
    eax = 0; al = 0x12; CHECK(ax == 0x12); ah = 0x34; CHECK(ax == 0x3412);
    ebx = 0; bx = 0xabcd; CHECK((dw)ebx == 0xabcd);

    fprintf(stderr, "test_rt: %d checks, %d failures\n", checks, fails);
    return fails ? 1 : 0;
}
