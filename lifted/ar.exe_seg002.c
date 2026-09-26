#include "lifted.h"
#include "lifted_data.h"
#include "lifted_procs.h"

void seg002_29e_proc(void) {
    /* iret ;~ 0E8A:029E */
    return;
    /* iret ;~ 0E8A:02A4 */
    return;
sub_1e810:
    /* jmp     far ptr 0:0 ;~ 0E8A:1990 */
    { vfn f_ = func_at(rt_far(*(dd*)&mem[0x10231])); if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n", (dd)(rt_far(*(dd*)&mem[0x10231]))); return; }
}
void sub_1e810(void) {
    /* jmp     far ptr 0:0 ;~ 0E8A:1990 */
    { vfn f_ = func_at(rt_far(*(dd*)&mem[0x10231])); if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n", (dd)(rt_far(*(dd*)&mem[0x10231]))); return; }
}
void sub_1e815(void) {
    /* jmp     far ptr 0:0 ;~ 0E8A:1995 */
    { vfn f_ = func_at(rt_far(*(dd*)&mem[0x10236])); if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n", (dd)(rt_far(*(dd*)&mem[0x10236]))); return; }
}
void sub_1e81a(void) {
    /* jmp     far ptr 0:0 ;~ 0E8A:199A */
    { vfn f_ = func_at(rt_far(*(dd*)&mem[0x1023b])); if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n", (dd)(rt_far(*(dd*)&mem[0x1023b]))); return; }
}
void sub_1e81f(void) {
    /* jmp     far ptr 0:0 ;~ 0E8A:199F */
    { vfn f_ = func_at(rt_far(*(dd*)&mem[0x10240])); if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n", (dd)(rt_far(*(dd*)&mem[0x10240]))); return; }
}
void sub_1e824(void) {
    /* jmp     far ptr 0:0 ;~ 0E8A:19A4 */
    { vfn f_ = func_at(rt_far(*(dd*)&mem[0x10245])); if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n", (dd)(rt_far(*(dd*)&mem[0x10245]))); return; }
}
void sub_1e829(void) {
    /* jmp     far ptr 0:0 ;~ 0E8A:19A9 */
    { vfn f_ = func_at(rt_far(*(dd*)&mem[0x1024a])); if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n", (dd)(rt_far(*(dd*)&mem[0x1024a]))); return; }
}
void sub_1e82e(void) {
    /* jmp     far ptr 0:0 ;~ 0E8A:19AE */
    { vfn f_ = func_at(rt_far(*(dd*)&mem[0x1024f])); if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n", (dd)(rt_far(*(dd*)&mem[0x1024f]))); return; }
}
void ret_e8a_cb30(void) {
    /* push    ax ;~ 0E8A:CB30 */
    push(ax);
    /* retf ;~ 0E8A:CB31 */
    return;
}
void ret_e8a_d28f(void) {
    /* inc     word ptr [bp+di] ;~ 0E8A:D28F */
    (*(dw*)raddr(ss,bp+di))++;
    /* pop     es ;~ 0E8A:D291 */
    es = pop();
    /* add     [bp+si], al ;~ 0E8A:D292 */
    { dd t_ = (dd)*(raddr(ss,bp+si)) + (dd)al; CF = t_ > 0xFFFF; *(raddr(ss,bp+si)) = t_; }
    /* adc     ax, 1301h ;~ 0E8A:D294 */
    { dd t_ = (dd)ax + (dd)0x1301 + CF; CF = t_ > 0xFFFF; ax = t_; }
    /* add     dx, dx ;~ 0E8A:D297 */
    { dd t_ = (dd)dx + (dd)dx; CF = t_ > 0xFFFF; dx = t_; }
    /* rol     word ptr [bx+di], cl ;~ 0E8A:D299 */
    *(dw*)raddr(ds,bx+di) = rol16(*(dw*)raddr(ds,bx+di), cl);
    /* adc     al, 3 ;~ 0E8A:D29B */
    { dd t_ = (dd)al + (dd)3 + CF; CF = t_ > 0xFF; al = t_; }
    /* retf    1D2h ;~ 0E8A:D29D */
    return;
    /* jmp     word ptr [bx+si] ;~ 0E8A:D2C9 */
    { vfn f_ = func_at((dd)0xe8a0 + (*(dw*)(raddr(ds,bx+si)))); if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n", (dd)((dd)0xe8a0 + (*(dw*)(raddr(ds,bx+si))))); return; }
ret_e8a_d796:
    /* in      al, dx ;~ 0E8A:D796 */
    al = in(dx);
    /* push    word ptr [bp+si-1400h] ;~ 0E8A:D797 */
    push(*(dw*)(raddr(ss,bp+si-0x1400)));
    /* jmp     ax ;~ 0E8A:D79B */
    { vfn f_ = func_at((dd)0xe8a0 + (ax)); if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n", (dd)((dd)0xe8a0 + (ax))); return; }
ret_e8a_dab5:
    /* clc ;~ 0E8A:DAB5 */
    CF = 0;
    /* jmp     ax ;~ 0E8A:DAB6 */
    { vfn f_ = func_at((dd)0xe8a0 + (ax)); if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n", (dd)((dd)0xe8a0 + (ax))); return; }
}
void ret_e8a_d796(void) {
    /* in      al, dx ;~ 0E8A:D796 */
    al = in(dx);
    /* push    word ptr [bp+si-1400h] ;~ 0E8A:D797 */
    push(*(dw*)(raddr(ss,bp+si-0x1400)));
    /* jmp     ax ;~ 0E8A:D79B */
    { vfn f_ = func_at((dd)0xe8a0 + (ax)); if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n", (dd)((dd)0xe8a0 + (ax))); return; }
ret_e8a_dab5:
    /* clc ;~ 0E8A:DAB5 */
    CF = 0;
    /* jmp     ax ;~ 0E8A:DAB6 */
    { vfn f_ = func_at((dd)0xe8a0 + (ax)); if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n", (dd)((dd)0xe8a0 + (ax))); return; }
}
void ret_e8a_dab5(void) {
    /* clc ;~ 0E8A:DAB5 */
    CF = 0;
    /* jmp     ax ;~ 0E8A:DAB6 */
    { vfn f_ = func_at((dd)0xe8a0 + (ax)); if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n", (dd)((dd)0xe8a0 + (ax))); return; }
}
void seg002_b8b2_proc(void) {
    /* jmp     word ptr [bx+si] ;~ 0E8A:B8B2 */
    { vfn f_ = func_at((dd)0xe8a0 + (*(dw*)(raddr(ds,bx+si)))); if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n", (dd)((dd)0xe8a0 + (*(dw*)(raddr(ds,bx+si))))); return; }
ret_e8a_cb30:
    /* push    ax ;~ 0E8A:CB30 */
    push(ax);
    /* retf ;~ 0E8A:CB31 */
    return;
}