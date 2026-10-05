#include "../rt.h"
#include "../data_syms.h"
#include "../procs.h"
void ivt_stubs(void) {
    /* iret ;~ 0E8A:029E */
    return;
    /* iret ;~ 0E8A:02A4 */
    return;
sub_1e810:
    /* jmp     far ptr 0:0 ;~ 0E8A:1990 */
    { vfn f_ = func_at(rt_far(*(dd*)&mem[0x10231])); if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n", (dd)(rt_far(*(dd*)&mem[0x10231]))); return; }
}
void farjmp_ptr_10231(void) {
    /* jmp     far ptr 0:0 ;~ 0E8A:1990 */
    { vfn f_ = func_at(rt_far(*(dd*)&mem[0x10231])); if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n", (dd)(rt_far(*(dd*)&mem[0x10231]))); return; }
}
void farjmp_ptr_10236(void) {
    /* jmp     far ptr 0:0 ;~ 0E8A:1995 */
    { vfn f_ = func_at(rt_far(*(dd*)&mem[0x10236])); if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n", (dd)(rt_far(*(dd*)&mem[0x10236]))); return; }
}
void farjmp_ptr_1023b(void) {
    /* jmp     far ptr 0:0 ;~ 0E8A:199A */
    { vfn f_ = func_at(rt_far(*(dd*)&mem[0x1023b])); if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n", (dd)(rt_far(*(dd*)&mem[0x1023b]))); return; }
}
void farjmp_ptr_10240(void) {
    /* jmp     far ptr 0:0 ;~ 0E8A:199F */
    { vfn f_ = func_at(rt_far(*(dd*)&mem[0x10240])); if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n", (dd)(rt_far(*(dd*)&mem[0x10240]))); return; }
}
void farjmp_ptr_10245(void) {
    /* jmp     far ptr 0:0 ;~ 0E8A:19A4 */
    { vfn f_ = func_at(rt_far(*(dd*)&mem[0x10245])); if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n", (dd)(rt_far(*(dd*)&mem[0x10245]))); return; }
}
void patched_trampoline(void) {
    /* jmp     far ptr 0:0 ;~ 0E8A:19A9 */
    { vfn f_ = func_at(rt_far(*(dd*)&mem[0x1024a])); if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n", (dd)(rt_far(*(dd*)&mem[0x1024a]))); return; }
}
void farjmp_ptr_1024f(void) {
    /* jmp     far ptr 0:0 ;~ 0E8A:19AE */
    { vfn f_ = func_at(rt_far(*(dd*)&mem[0x1024f])); if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n", (dd)(rt_far(*(dd*)&mem[0x1024f]))); return; }
}




void ind_jmp_ptr(void) {
    /* jmp     word ptr [bx+si] ;~ 0E8A:B8B2 */
    { vfn f_ = func_at((dd)0xe8a0 + (*(dw*)(raddr(ds,bx+si)))); if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n", (dd)((dd)0xe8a0 + (*(dw*)(raddr(ds,bx+si))))); return; }
ret_e8a_cb30:
    /* push    ax ;~ 0E8A:CB30 */
    push(ax);
    /* retf ;~ 0E8A:CB31 */
    return;
}