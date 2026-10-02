#include "lifted.h"
#include "lifted_data.h"
#include "lifted_procs.h"

void edummylabel1(void) {
    return;
}
void uninstall_timer(void) {
    dd _sa = 0, _sb = 0;
    IF = 0;
    push(ds);
    bx = *(dw*)(&dword_103de);
    cx = *(dw*)(((db*)&dword_103de)+2);
    { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
    m2c::set_segment_register(ds, ax);
    *(dw*)(raddr(ds,0x20)) = bx;
    *(dw*)(raddr(ds,0x22)) = cx;
    ds = pop();
    _sa = (ax);
    out(0x43, 0x36);
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    out(0x40, al);
    al = ah;
    out(0x40, al);
    IF = 1;
    return;
}
void edummylabel11(void) {
    dd _sa = 0, _sb = 0;
    out(0x0C0, al);
    out(0x0C0, 0x0BF);
    out(0x0C0, 0x0DF);
    al = 0x0FF;
    out(0x0C0, al);
    cx = 0x38;
    bx = offset(seg001,unk_1009f);
loc_104bb:
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15); }
loc_104bf:
    do {
        (bx)--; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        _sa = (bx);
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void edummylabel12(void) {
    dd _sa = 0, _sb = 0;
    if (!ZF) {
        CF = (dd)*(dw*)(raddr(ds,0x63)) < (dd)0; ZF = ((dw)((*(dw*)(raddr(ds,0x63))) - (0)) == 0); SF = (((dw)((*(dw*)(raddr(ds,0x63))) - (0))) >> 15);
        if (*(dw*)(raddr(ds,0x63)) != 0) {
            CF = (dd)*(raddr(ds,0x49)) < (dd)0; ZF = ((dw)((*(raddr(ds,0x49))) - (0)) == 0); SF = (((dw)((*(raddr(ds,0x49))) - (0))) >> 15);
            if (*(raddr(ds,0x49)) == 0) {
                bx = *(dw*)raddr(ds,0x63);
                sub_105b0(); return;
            }
        }
loc_104e1:
        CF = (dd)*(raddr(ds,0x65)) < (dd)0; ZF = ((dw)((*(raddr(ds,0x65))) - (0)) == 0); SF = (((dw)((*(raddr(ds,0x65))) - (0))) >> 15);
        if (*(raddr(ds,0x65)) != 0) {
            CF = (dd)*(raddr(ds,0x66)) < (dd)0; ZF = ((dw)((*(raddr(ds,0x66))) - (0)) == 0); SF = (((dw)((*(raddr(ds,0x66))) - (0))) >> 15);
            if ((short)*(raddr(ds,0x66)) <= (short)0) {
                _sa = (*(raddr(ds,0x66)));
                *(raddr(ds,0x66)) = 0x1E;
                CF = (dd)*(raddr(ds,0x2F)) < (dd)0; ZF = ((dw)((*(raddr(ds,0x2F))) - (0)) == 0); SF = (((dw)((*(raddr(ds,0x2F))) - (0))) >> 15);
                if (*(raddr(ds,0x2F)) == 0) {
                    bx = 0x1B6;
                    sub_105b0(); return;
                }
            }
loc_10502:
            (*(raddr(ds,0x66)))--; ZF = ((dw)(*(raddr(ds,0x66))) == 0); SF = (((dw)(*(raddr(ds,0x66)))) >> 15);
        }
    }
loc_10506:
    di = 0x2F;
    sub_1051f();
    di = 0x3C;
    sub_1051f();
    di = 0x49;
    sub_1051f();
    di = 0x56;
sub_1051f:
    CF = (dd)*(raddr(ds,di)) < (dd)0; ZF = ((dw)((*(raddr(ds,di))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di))) - (0))) >> 15);
    if (*(raddr(ds,di)) == 0) { return; }
    (*(raddr(ds,di)))--; ZF = ((dw)(*(raddr(ds,di))) == 0); SF = (((dw)(*(raddr(ds,di)))) >> 15);
    if (*(raddr(ds,di)) != 0) {
        CF = (dd)*(dw*)(raddr(ds,di+2)) < (dd)0; ZF = ((dw)((*(dw*)(raddr(ds,di+2))) - (0)) == 0); SF = (((dw)((*(dw*)(raddr(ds,di+2))) - (0))) >> 15);
        if (*(dw*)(raddr(ds,di+2)) == 0) { return; }
        al = *(db*)raddr(ds,di+7);
        al |= al; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
        if (al != 0) {
            bl = al;
            al &= 0x0F0; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
            if (al != 0) {
                (*(raddr(ds,di+4)))--; ZF = ((dw)(*(raddr(ds,di+4))) == 0); SF = (((dw)(*(raddr(ds,di+4)))) >> 15);
                if (*(raddr(ds,di+4)) == 0) {
                    al = *(db*)raddr(ds,di+6);
                    al &= 0x0F0; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
                    _sa = (al);
                    ax = (char)al;
                    cx = *(dw*)raddr(ds,di+2);
                    { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
                    ax &= 0x3FF0; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
                    cx &= 0x0C00F; CF = 0; OF = 0; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
                    ax |= cx; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
                    *(dw*)(raddr(ds,di+2)) = ax;
                    _sa = (ax);
                    al = bl;
                    al &= 0x0F0; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
                    *(db*)raddr(ds,di+4) = al;
                }
            }
loc_10566:
            al = bl;
            al &= 0x0F; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
            if (al != 0) {
                (*(raddr(ds,di+5)))--; ZF = ((dw)(*(raddr(ds,di+5))) == 0); SF = (((dw)(*(raddr(ds,di+5)))) >> 15);
                if (*(raddr(ds,di+5)) == 0) {
                    al = *(db*)raddr(ds,di+6);
                    al &= 0x0F; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
                    cx = *(dw*)raddr(ds,di+2);
                    { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
                    ax &= 0x0F; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
                    cx &= 0x0FFF0; CF = 0; OF = 0; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
                    ax |= cx; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
                    *(dw*)(raddr(ds,di+2)) = ax;
                    bl &= 0x0F; CF = 0; OF = 0; ZF = ((db)(bl) == 0); SF = (((db)(bl)) >> 7);
                    *(db*)raddr(ds,di+5) = bl;
                }
            }
        }
loc_1058c:
        bx = *(dw*)raddr(ds,di+2);
        sub_10889(); return;
    }
loc_10592:
    bx = *(dw*)raddr(ds,di+0x0B);
    al = *(db*)raddr(ds,bx);
    al |= al; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
    if (al == 0) {
sub_1059b:
        bx = *(dw*)raddr(ds,di+2);
        bx |= bx; CF = 0; OF = 0; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        if (bx != 0) {
            bl |= 0x0F; CF = 0; OF = 0; ZF = ((db)(bl) == 0); SF = (((db)(bl)) >> 7);
            sub_10889();
        }
loc_105a8:
        bx = di;
        cx = 0x0D;
        goto loc_104bb;
    }
loc_105d2:
    al = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    ax &= 0x7F; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
    CF = 0; OF = 0; ZF = ((db)((al & 0x88)) == 0); SF = (((db)((al & 0x88))) >> 7);
    if ((al & 0x88) != 0) {
        push(bx);
        push(ax);
        _sa = ((al & 0x88));
        ax = *(dw*)raddr(ds,di+9);
        push(ax);
        al = *(db*)raddr(ds,di+8);
        push(ax);
        sub_1059b();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto loc_10630;
loc_105f6:
        CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
        if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
loc_105fd:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
loc_10605:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0; OF = 0; ZF = ((db)((al & 0x0B0)) == 0); SF = (((db)((al & 0x0B0))) >> 7);
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
        goto loc_1062d;
    }
loc_10615:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0A0)) == 0); SF = (((db)((al & 0x0A0))) >> 7);
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+4) = cl;
    }
loc_1061f:
    CF = 0; OF = 0; ZF = ((db)((al & 0x90)) == 0); SF = (((db)((al & 0x90))) >> 7);
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+5) = cl;
    }
loc_10629:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) == 0) goto loc_105f6;
    cl = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
        if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
        *(dw*)(raddr(ds,di+9)) = cx;
        goto loc_105fa;
    }
loc_1064a:
    *(db*)raddr(ds,di+8) = cl;
loc_1064d:
    cx = *(dw*)raddr(ds,di+9);
    goto loc_105fd;
loc_104bb:
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15); }
loc_104bf:
    do {
        (bx)--; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        _sa = (bx);
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void edummylabel13(void) {
    dd _sa = 0, _sb = 0;
    al = *(db*)raddr(ds,bx);
    al |= al; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
    if (al == 0) { return; }
    al &= 3; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
    _sa = (al);
    ax = (char)al;
    {dd r = (dd)ax * 0x0D; ax = r; dx = r >> 16;}
    cx = 0x2F;
    { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
sub_105c4:
    di = ax;
    push(bx);
    push(*(dw*)(raddr(ds,di)));
    sub_1059b();
    *(dw*)(raddr(ds,di)) = pop();
    bx = pop();
    *(dw*)(raddr(ds,di+9)) = bx;
loc_105d2:
    al = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    ax &= 0x7F; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
    CF = 0; OF = 0; ZF = ((db)((al & 0x88)) == 0); SF = (((db)((al & 0x88))) >> 7);
    if ((al & 0x88) != 0) {
        push(bx);
        push(ax);
        _sa = ((al & 0x88));
        ax = *(dw*)raddr(ds,di+9);
        push(ax);
        al = *(db*)raddr(ds,di+8);
        push(ax);
        sub_1059b();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto loc_10630;
loc_105f6:
        CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
        if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
loc_105fd:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
loc_10605:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0; OF = 0; ZF = ((db)((al & 0x0B0)) == 0); SF = (((db)((al & 0x0B0))) >> 7);
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
        goto loc_1062d;
    }
loc_10615:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0A0)) == 0); SF = (((db)((al & 0x0A0))) >> 7);
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+4) = cl;
    }
loc_1061f:
    CF = 0; OF = 0; ZF = ((db)((al & 0x90)) == 0); SF = (((db)((al & 0x90))) >> 7);
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+5) = cl;
    }
loc_10629:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) == 0) goto loc_105f6;
    cl = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
        if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
        *(dw*)(raddr(ds,di+9)) = cx;
        goto loc_105fa;
    }
loc_1064a:
    *(db*)raddr(ds,di+8) = cl;
loc_1064d:
    cx = *(dw*)raddr(ds,di+9);
    goto loc_105fd;
}
void edummylabel14(void) {
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x0F0;
    sub_107b2(); return;
locret_1069d:
    return;
}
void edummylabel15(void) {
    al = 0x1E;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x108;
    sub_107b2();
    bx = 0x100;
    sub_107b5(); return;
locret_1069d:
    return;
}
void edummylabel16(void) {
    bx = 0x9D;
    sub_107b5(); return;
}
void edummylabel17(void) {
    al = 0x64;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    bx = 0x198;
    if (al >= *(db*)raddr(ds,0x30)) {
        sub_107b2();
        bx = 0x1A7;
        sub_107b5(); return;
    }
loc_10694:
    IF = 0;
    ax = 0x3C;
    sub_105c4();
    IF = 1;
locret_1069d:
    return;
}
void edummylabel18(void) {
    bx = 0x1BB;
    goto loc_106a9;
loc_106a9:
    *(dw*)(raddr(ds,0x63)) = bx;
    sub_107b5(); return;
}
void edummylabel19(void) {
    bx = 0x1C3;
loc_106a9:
    *(dw*)(raddr(ds,0x63)) = bx;
    sub_107b5(); return;
}
void edummylabel2(void) {
    return;
}
void edummylabel20(void) {
    dd _sa = 0, _sb = 0;
    *(dw*)(raddr(ds,0x63)) = 0;
    CF = (dd)*(raddr(ds,0x49)) < (dd)0; ZF = ((dw)((*(raddr(ds,0x49))) - (0)) == 0); SF = (((dw)((*(raddr(ds,0x49))) - (0))) >> 15);
    if (*(raddr(ds,0x49)) == 0) { return; }
    _sa = (*(raddr(ds,0x49)));
    *(raddr(ds,0x49)) = 1;
locret_106c2:
    return;
}
void edummylabel21(void) {
    *(dw*)(raddr(ds,0x63)) = 0;
    bx = 0x74;
    sub_107b5(); return;
}
void edummylabel22(void) {
    bx = 0x72;
    sub_107b5(); return;
}
void edummylabel23(void) {
    *(raddr(ds,0x66)) = 0x1E;
    *(raddr(ds,0x65)) = 0x0FF;
    return;
}
void edummylabel24(void) {
    *(raddr(ds,0x65)) = 0;
locret_106e7:
    return;
}
void edummylabel25(void) {
    al = 0x4B;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x153;
loc_106f4:
    sub_107b2();
    bx = 0x143;
    sub_107b5(); return;
locret_106e7:
    return;
}
void edummylabel26(void) {
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x8D;
    sub_107b2(); return;
locret_106e7:
    return;
}
void edummylabel27(void) {
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x11F;
    sub_107b2(); return;
locret_106e7:
    return;
}
void edummylabel28(void) {
    al = 0x41;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x15B;
    goto loc_106f4;
loc_106f4:
    sub_107b2();
    bx = 0x143;
    sub_107b5(); return;
}
void edummylabel29(void) {
    bx = 0x16A;
    goto loc_10735;
loc_10735:
    sub_107b5();
    bx = 0x163;
    sub_107b5(); return;
}
void edummylabel3(void) {
    push(cs);
    ds = pop();
    sub_104a4();
    return;
}
void edummylabel30(void) {
    bx = 0x174;
loc_10735:
    sub_107b5();
    bx = 0x163;
    sub_107b5(); return;
}
void edummylabel31(void) {
    bx = 0x16C;
    goto loc_1074a;
loc_1074a:
    al = 0x28;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    sub_107b2();
    bx = 0x165;
    sub_107b5(); return;
locret_107ba:
    return;
}
void edummylabel32(void) {
    bx = 0x176;
loc_1074a:
    al = 0x28;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    sub_107b2();
    bx = 0x165;
    sub_107b5(); return;
locret_107ba:
    return;
}
void edummylabel33(void) {
    al = 0x3C;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x0A6;
    sub_107b2(); return;
locret_107ba:
    return;
}
void edummylabel34(void) {
    al = 0x63;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x187;
    sub_107b2();
    bx = 0x17E;
    sub_107b5(); return;
locret_107ba:
    return;
}
void edummylabel35(void) {
    al = 0x46;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x0F8;
    goto loc_1079c;
loc_1079c:
    sub_107b2();
    bx = 0x133;
    sub_107b5(); return;
locret_107ba:
    return;
}
void edummylabel36(void) {
    al = 0x5F;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x13B;
loc_1079c:
    sub_107b2();
    bx = 0x133;
    sub_107b5(); return;
locret_107ba:
    return;
}
void edummylabel37(void) {
    al = 0x2E;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x126;
sub_107b2:
    *(db*)raddr(ds,0x30) = al;
sub_107b5:
    IF = 0;
    sub_105b0();
    IF = 1;
locret_107ba:
    return;
}
void edummylabel38(void) {
    al = 0x2D;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x12E;
    sub_107b2(); return;
locret_107ba:
    return;
}
void edummylabel39(void) {
    al = 0x55;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x190;
    sub_107b2(); return;
locret_107ba:
    return;
}
void edummylabel4(void) {
    push(cs);
    ds = pop();
    CF = (dd)byte_100d7 < (dd)0x0FF; ZF = ((db)((byte_100d7) - (0x0FF)) == 0); SF = (((db)((byte_100d7) - (0x0FF))) >> 7);
    if (byte_100d7 == 0x0FF) { return; }
    { vfn f_ = func_at((dd)0x30070 + (*(dw*)(((db*)&off_10380)+bx))); dw sp_ = sp; if (f_) f_(); else fprintf(stderr, "unresolved ind call %x\n", (dd)((dd)0x30070 + (*(dw*)(((db*)&off_10380)+bx)))); if ((short)(sp - sp_) > 0) { sp = sp_; return; } }
locret_1008c:
    return;
}
void edummylabel40(void) {
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x117;
    sub_107b2(); return;
locret_107ba:
    return;
}
void edummylabel41(void) {
    al = 0x50;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x14B;
    sub_107b2();
    bx = 0x153;
    sub_107b5(); return;
locret_107ba:
    return;
}
void edummylabel42(void) {
    bx = 0x0AE;
    goto loc_10805;
loc_10805:
    sub_107b5();
    bx = 0x0DA;
    sub_107b5(); return;
}
void edummylabel43(void) {
    bx = 0x0C4;
loc_10805:
    sub_107b5();
    bx = 0x0DA;
    sub_107b5(); return;
}
void edummylabel44(void) {
    bx = 0x0E1;
    sub_107b5(); return;
}
void edummylabel45(void) {
    bx = 0x1D0;
    sub_107b5(); return;
}
void edummylabel46(void) {
    bx = 0x1CB;
    sub_107b5(); return;
}
void edummylabel47(void) {
    bx = 0x6A;
    sub_107b5(); return;
}
void edummylabel48(void) {
    bx = 0x110;
    sub_107b5();
    bx = 0x84;
    sub_107b5(); return;
}
void edummylabel49(void) {
    bx = 0x1EF;
    sub_107b5();
    *(dw*)(raddr(ds,0x38)) = 0x1DF;
    bx = 0x23D;
    sub_107b5();
    bx = 0x235;
    *(dw*)(raddr(ds,0x45)) = bx;
sub_10851:
    bx = 0x28B;
    sub_107b5();
    *(dw*)(raddr(ds,0x52)) = 0x26B;
    bx = 0x2FD;
    sub_107b5(); return;
}
void edummylabel5(void) {
    push(cs);
    ds = pop();
    byte_100d7 = 0;
    return;
}
void edummylabel50(void) {
    sub_10833();
    *(dw*)(raddr(ds,0x38)) = 0x210;
    *(dw*)(raddr(ds,0x45)) = 0x246;
    bx = 0x2AC;
    *(dw*)(raddr(ds,0x52)) = bx;
    bl = 0x18;
    *(db*)raddr(ds,0x5E) = bl;
    return;
}
void edummylabel51(void) {
    dd _sa = 0, _sb = 0;
    CF = (dd)*(raddr(ds,0x67)) < (dd)0x0FF; ZF = ((dw)((*(raddr(ds,0x67))) - (0x0FF)) == 0); SF = (((dw)((*(raddr(ds,0x67))) - (0x0FF))) >> 15);
    if (*(raddr(ds,0x67)) == 0x0FF) { return; }
    ax = bx;
    CF = (dd)ah < (dd)0x0BF; ZF = ((db)((ah) - (0x0BF)) == 0); SF = (((db)((ah) - (0x0BF))) >> 7);
    if (ah <= 0x0BF) {
        { if (1) { CF = (ah >> ((1)-1)) & 1; ah = ah >> 1; ZF = ((db)(ah) == 0); SF = (((db)(ah)) >> 7); } }
        ah |= 0x90; CF = 0; OF = 0; ZF = ((db)(ah) == 0); SF = (((db)(ah)) >> 7);
        ah &= 0x0F0; CF = 0; OF = 0; ZF = ((db)(ah) == 0); SF = (((db)(ah)) >> 7);
        al &= 0x0F; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
        al |= ah; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
        out(0x0C0, al);
        ah &= 0x0E0; CF = 0; OF = 0; ZF = ((db)(ah) == 0); SF = (((db)(ah)) >> 7);
        _sa = (ah);
        al = bl;
        { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
        { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
        { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
        { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
        al |= ah; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
        out(0x0C0, al);
        _sa = (al);
        al = bh;
        al &= 0x3F; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
        out(0x0C0, al);
locret_108bc:
        return;
    }
loc_108bd:
    al = bh;
    al |= 0x0E0; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
    out(0x0C0, al);
    _sa = (al);
    al = bl;
    al |= 0x0F0; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
    out(0x0C0, al);
    return;
}
void edummylabel6(void) {
    push(cs);
    ds = pop();
    byte_100d7 = 0x0FF;
    return;
}
void edummylabel7(void) {
    CF = (dd)bx < (dd)0x52; ZF = ((dw)((bx) - (0x52)) == 0); SF = (((dw)((bx) - (0x52))) >> 15);
    if (bx > 0x52) { return; }
    { vfn f_ = func_at((dd)0x30070 + (*(dw*)(((db*)&off_10380)+bx))); dw sp_ = sp; if (f_) f_(); else fprintf(stderr, "unresolved ind call %x\n", (dd)((dd)0x30070 + (*(dw*)(((db*)&off_10380)+bx)))); if ((short)(sp - sp_) > 0) { sp = sp_; return; } }
locret_103dd:
    return;
}
void timer_isr(void) {
    dd _sa = 0, _sb = 0;
    CF = (dd)byte_103e2 < (dd)0x0FF; ZF = ((db)((byte_103e2) - (0x0FF)) == 0); SF = (((db)((byte_103e2) - (0x0FF))) >> 7);
    if (byte_103e2 == 0x0FF) { return; }
    push(ax);
    push(ds);
    push(cs);
    ds = pop();
    push(di);
    push(cx);
    push(bx);
    seq_tick();
    bx = pop();
    cx = pop();
    di = pop();
    (byte_100d9)--; ZF = ((db)(byte_100d9) == 0); SF = (((db)(byte_100d9)) >> 7);
    if (byte_100d9 != 0) {
        ds = pop();
        al = 0x20;
        out(0x20, al);
        ax = pop();
        byte_103e2 = 0;
locret_1040a:
        return;
sub_1040b:
        *(raddr(ds,0x67)) = 0;
edummylabel9:
        *(raddr(ds,0x65)) = 0;
        *(dw*)(raddr(ds,0x63)) = 0;
        byte_103e2 = 0;
        IF = 0;
        CF = (dd)*(raddr(ds,0x68)) < (dd)0; ZF = ((dw)((*(raddr(ds,0x68))) - (0)) == 0); SF = (((dw)((*(raddr(ds,0x68))) - (0))) >> 15);
        if (*(raddr(ds,0x68)) != 0) { return; }
        _sa = (*(raddr(ds,0x68)));
        *(raddr(ds,0x68)) = ~*(raddr(ds,0x68));
        push(ds);
        { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
        m2c::set_segment_register(ds, ax);
        bx = *(dw*)raddr(ds,0x20);
        cx = *(dw*)raddr(ds,0x22);
        *(dw*)(&dword_103de) = bx;
        *(dw*)(((db*)&dword_103de)+2) = cx;
        ds = pop();
        IF = 0;
        *(raddr(ds,0x69)) = 3;
        push(ds);
        { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
        m2c::set_segment_register(ds, ax);
        _sa = (ax);
        ax = 0x373;
        *(dw*)(raddr(ds,0x20)) = ax;
        *(dw*)(raddr(ds,0x22)) = cs;
        ds = pop();
        out(0x43, 0x36);
        ax = 0x4DAE;
        out(0x40, al);
        al = ah;
        out(0x40, al);
        IF = 1;
locret_1046a:
        return;
    }
loc_1046b:
    *(raddr(ds,0x69)) = 3;
    byte_103e2 = 0;
    ds = pop();
    ax = pop();
    { vfn f_ = func_at(rt_far(dword_103de)); if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n", (dd)(rt_far(dword_103de))); return; }
edummylabel10:
    IF = 0;
    push(ds);
    bx = *(dw*)(&dword_103de);
    cx = *(dw*)(((db*)&dword_103de)+2);
    { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
    m2c::set_segment_register(ds, ax);
    *(dw*)(raddr(ds,0x20)) = bx;
    *(dw*)(raddr(ds,0x22)) = cx;
    ds = pop();
    _sa = (ax);
    out(0x43, 0x36);
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    out(0x40, al);
    al = ah;
    out(0x40, al);
    IF = 1;
    return;
}
void install_timer_2(void) {
    dd _sa = 0, _sb = 0;
    *(raddr(ds,0x65)) = 0;
    *(dw*)(raddr(ds,0x63)) = 0;
    byte_103e2 = 0;
    IF = 0;
    CF = (dd)*(raddr(ds,0x68)) < (dd)0; ZF = ((dw)((*(raddr(ds,0x68))) - (0)) == 0); SF = (((dw)((*(raddr(ds,0x68))) - (0))) >> 15);
    if (*(raddr(ds,0x68)) != 0) { return; }
    _sa = (*(raddr(ds,0x68)));
    *(raddr(ds,0x68)) = ~*(raddr(ds,0x68));
    push(ds);
    { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
    m2c::set_segment_register(ds, ax);
    bx = *(dw*)raddr(ds,0x20);
    cx = *(dw*)raddr(ds,0x22);
    *(dw*)(&dword_103de) = bx;
    *(dw*)(((db*)&dword_103de)+2) = cx;
    ds = pop();
    IF = 0;
    *(raddr(ds,0x69)) = 3;
    push(ds);
    { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
    m2c::set_segment_register(ds, ax);
    _sa = (ax);
    ax = 0x373;
    *(dw*)(raddr(ds,0x20)) = ax;
    *(dw*)(raddr(ds,0x22)) = cs;
    ds = pop();
    out(0x43, 0x36);
    ax = 0x4DAE;
    out(0x40, al);
    al = ah;
    out(0x40, al);
    IF = 1;
locret_1046a:
    return;
}
void timer_isr_chain(void) {
    dd _sa = 0, _sb = 0;
    *(raddr(ds,0x69)) = 3;
    byte_103e2 = 0;
    ds = pop();
    ax = pop();
    { vfn f_ = func_at(rt_far(dword_103de)); if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n", (dd)(rt_far(dword_103de))); return; }
edummylabel10:
    IF = 0;
    push(ds);
    bx = *(dw*)(&dword_103de);
    cx = *(dw*)(((db*)&dword_103de)+2);
    { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
    m2c::set_segment_register(ds, ax);
    *(dw*)(raddr(ds,0x20)) = bx;
    *(dw*)(raddr(ds,0x22)) = cx;
    ds = pop();
    _sa = (ax);
    out(0x43, 0x36);
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    out(0x40, al);
    al = ah;
    out(0x40, al);
    IF = 1;
    return;
}
void loc_104bb(void) {
    dd _sa = 0, _sb = 0;
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15); }
loc_104bf:
    do {
        (bx)--; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        _sa = (bx);
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void loc_104bf(void) {
    dd _sa = 0, _sb = 0;
loc_104bf:
    do {
        (bx)--; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        _sa = (bx);
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void loc_104e1(void) {
    dd _sa = 0, _sb = 0;
    CF = (dd)*(raddr(ds,0x65)) < (dd)0; ZF = ((dw)((*(raddr(ds,0x65))) - (0)) == 0); SF = (((dw)((*(raddr(ds,0x65))) - (0))) >> 15);
    if (*(raddr(ds,0x65)) != 0) {
        CF = (dd)*(raddr(ds,0x66)) < (dd)0; ZF = ((dw)((*(raddr(ds,0x66))) - (0)) == 0); SF = (((dw)((*(raddr(ds,0x66))) - (0))) >> 15);
        if ((short)*(raddr(ds,0x66)) <= (short)0) {
            _sa = (*(raddr(ds,0x66)));
            *(raddr(ds,0x66)) = 0x1E;
            CF = (dd)*(raddr(ds,0x2F)) < (dd)0; ZF = ((dw)((*(raddr(ds,0x2F))) - (0)) == 0); SF = (((dw)((*(raddr(ds,0x2F))) - (0))) >> 15);
            if (*(raddr(ds,0x2F)) == 0) {
                bx = 0x1B6;
                sub_105b0(); return;
            }
        }
loc_10502:
        (*(raddr(ds,0x66)))--; ZF = ((dw)(*(raddr(ds,0x66))) == 0); SF = (((dw)(*(raddr(ds,0x66)))) >> 15);
    }
loc_10506:
    di = 0x2F;
    sub_1051f();
    di = 0x3C;
    sub_1051f();
    di = 0x49;
    sub_1051f();
    di = 0x56;
sub_1051f:
    CF = (dd)*(raddr(ds,di)) < (dd)0; ZF = ((dw)((*(raddr(ds,di))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di))) - (0))) >> 15);
    if (*(raddr(ds,di)) == 0) { return; }
    (*(raddr(ds,di)))--; ZF = ((dw)(*(raddr(ds,di))) == 0); SF = (((dw)(*(raddr(ds,di)))) >> 15);
    if (*(raddr(ds,di)) != 0) {
        CF = (dd)*(dw*)(raddr(ds,di+2)) < (dd)0; ZF = ((dw)((*(dw*)(raddr(ds,di+2))) - (0)) == 0); SF = (((dw)((*(dw*)(raddr(ds,di+2))) - (0))) >> 15);
        if (*(dw*)(raddr(ds,di+2)) == 0) { return; }
        al = *(db*)raddr(ds,di+7);
        al |= al; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
        if (al != 0) {
            bl = al;
            al &= 0x0F0; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
            if (al != 0) {
                (*(raddr(ds,di+4)))--; ZF = ((dw)(*(raddr(ds,di+4))) == 0); SF = (((dw)(*(raddr(ds,di+4)))) >> 15);
                if (*(raddr(ds,di+4)) == 0) {
                    al = *(db*)raddr(ds,di+6);
                    al &= 0x0F0; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
                    _sa = (al);
                    ax = (char)al;
                    cx = *(dw*)raddr(ds,di+2);
                    { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
                    ax &= 0x3FF0; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
                    cx &= 0x0C00F; CF = 0; OF = 0; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
                    ax |= cx; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
                    *(dw*)(raddr(ds,di+2)) = ax;
                    _sa = (ax);
                    al = bl;
                    al &= 0x0F0; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
                    *(db*)raddr(ds,di+4) = al;
                }
            }
loc_10566:
            al = bl;
            al &= 0x0F; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
            if (al != 0) {
                (*(raddr(ds,di+5)))--; ZF = ((dw)(*(raddr(ds,di+5))) == 0); SF = (((dw)(*(raddr(ds,di+5)))) >> 15);
                if (*(raddr(ds,di+5)) == 0) {
                    al = *(db*)raddr(ds,di+6);
                    al &= 0x0F; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
                    cx = *(dw*)raddr(ds,di+2);
                    { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
                    ax &= 0x0F; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
                    cx &= 0x0FFF0; CF = 0; OF = 0; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
                    ax |= cx; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
                    *(dw*)(raddr(ds,di+2)) = ax;
                    bl &= 0x0F; CF = 0; OF = 0; ZF = ((db)(bl) == 0); SF = (((db)(bl)) >> 7);
                    *(db*)raddr(ds,di+5) = bl;
                }
            }
        }
loc_1058c:
        bx = *(dw*)raddr(ds,di+2);
        sub_10889(); return;
    }
loc_10592:
    bx = *(dw*)raddr(ds,di+0x0B);
    al = *(db*)raddr(ds,bx);
    al |= al; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
    if (al == 0) {
sub_1059b:
        bx = *(dw*)raddr(ds,di+2);
        bx |= bx; CF = 0; OF = 0; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        if (bx != 0) {
            bl |= 0x0F; CF = 0; OF = 0; ZF = ((db)(bl) == 0); SF = (((db)(bl)) >> 7);
            sub_10889();
        }
loc_105a8:
        bx = di;
        cx = 0x0D;
        goto loc_104bb;
    }
loc_105d2:
    al = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    ax &= 0x7F; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
    CF = 0; OF = 0; ZF = ((db)((al & 0x88)) == 0); SF = (((db)((al & 0x88))) >> 7);
    if ((al & 0x88) != 0) {
        push(bx);
        push(ax);
        _sa = ((al & 0x88));
        ax = *(dw*)raddr(ds,di+9);
        push(ax);
        al = *(db*)raddr(ds,di+8);
        push(ax);
        sub_1059b();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto loc_10630;
loc_105f6:
        CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
        if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
loc_105fd:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
loc_10605:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0; OF = 0; ZF = ((db)((al & 0x0B0)) == 0); SF = (((db)((al & 0x0B0))) >> 7);
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
        goto loc_1062d;
    }
loc_10615:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0A0)) == 0); SF = (((db)((al & 0x0A0))) >> 7);
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+4) = cl;
    }
loc_1061f:
    CF = 0; OF = 0; ZF = ((db)((al & 0x90)) == 0); SF = (((db)((al & 0x90))) >> 7);
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+5) = cl;
    }
loc_10629:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) == 0) goto loc_105f6;
    cl = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
        if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
        *(dw*)(raddr(ds,di+9)) = cx;
        goto loc_105fa;
    }
loc_1064a:
    *(db*)raddr(ds,di+8) = cl;
loc_1064d:
    cx = *(dw*)raddr(ds,di+9);
    goto loc_105fd;
loc_104bb:
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15); }
loc_104bf:
    do {
        (bx)--; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        _sa = (bx);
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void loc_10502(void) {
    dd _sa = 0, _sb = 0;
    (*(raddr(ds,0x66)))--; ZF = ((dw)(*(raddr(ds,0x66))) == 0); SF = (((dw)(*(raddr(ds,0x66)))) >> 15);
loc_10506:
    di = 0x2F;
    sub_1051f();
    di = 0x3C;
    sub_1051f();
    di = 0x49;
    sub_1051f();
    di = 0x56;
sub_1051f:
    CF = (dd)*(raddr(ds,di)) < (dd)0; ZF = ((dw)((*(raddr(ds,di))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di))) - (0))) >> 15);
    if (*(raddr(ds,di)) == 0) { return; }
    (*(raddr(ds,di)))--; ZF = ((dw)(*(raddr(ds,di))) == 0); SF = (((dw)(*(raddr(ds,di)))) >> 15);
    if (*(raddr(ds,di)) != 0) {
        CF = (dd)*(dw*)(raddr(ds,di+2)) < (dd)0; ZF = ((dw)((*(dw*)(raddr(ds,di+2))) - (0)) == 0); SF = (((dw)((*(dw*)(raddr(ds,di+2))) - (0))) >> 15);
        if (*(dw*)(raddr(ds,di+2)) == 0) { return; }
        al = *(db*)raddr(ds,di+7);
        al |= al; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
        if (al != 0) {
            bl = al;
            al &= 0x0F0; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
            if (al != 0) {
                (*(raddr(ds,di+4)))--; ZF = ((dw)(*(raddr(ds,di+4))) == 0); SF = (((dw)(*(raddr(ds,di+4)))) >> 15);
                if (*(raddr(ds,di+4)) == 0) {
                    al = *(db*)raddr(ds,di+6);
                    al &= 0x0F0; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
                    _sa = (al);
                    ax = (char)al;
                    cx = *(dw*)raddr(ds,di+2);
                    { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
                    ax &= 0x3FF0; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
                    cx &= 0x0C00F; CF = 0; OF = 0; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
                    ax |= cx; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
                    *(dw*)(raddr(ds,di+2)) = ax;
                    _sa = (ax);
                    al = bl;
                    al &= 0x0F0; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
                    *(db*)raddr(ds,di+4) = al;
                }
            }
loc_10566:
            al = bl;
            al &= 0x0F; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
            if (al != 0) {
                (*(raddr(ds,di+5)))--; ZF = ((dw)(*(raddr(ds,di+5))) == 0); SF = (((dw)(*(raddr(ds,di+5)))) >> 15);
                if (*(raddr(ds,di+5)) == 0) {
                    al = *(db*)raddr(ds,di+6);
                    al &= 0x0F; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
                    cx = *(dw*)raddr(ds,di+2);
                    { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
                    ax &= 0x0F; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
                    cx &= 0x0FFF0; CF = 0; OF = 0; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
                    ax |= cx; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
                    *(dw*)(raddr(ds,di+2)) = ax;
                    bl &= 0x0F; CF = 0; OF = 0; ZF = ((db)(bl) == 0); SF = (((db)(bl)) >> 7);
                    *(db*)raddr(ds,di+5) = bl;
                }
            }
        }
loc_1058c:
        bx = *(dw*)raddr(ds,di+2);
        sub_10889(); return;
    }
loc_10592:
    bx = *(dw*)raddr(ds,di+0x0B);
    al = *(db*)raddr(ds,bx);
    al |= al; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
    if (al == 0) {
sub_1059b:
        bx = *(dw*)raddr(ds,di+2);
        bx |= bx; CF = 0; OF = 0; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        if (bx != 0) {
            bl |= 0x0F; CF = 0; OF = 0; ZF = ((db)(bl) == 0); SF = (((db)(bl)) >> 7);
            sub_10889();
        }
loc_105a8:
        bx = di;
        cx = 0x0D;
        goto loc_104bb;
    }
loc_105d2:
    al = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    ax &= 0x7F; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
    CF = 0; OF = 0; ZF = ((db)((al & 0x88)) == 0); SF = (((db)((al & 0x88))) >> 7);
    if ((al & 0x88) != 0) {
        push(bx);
        push(ax);
        _sa = ((al & 0x88));
        ax = *(dw*)raddr(ds,di+9);
        push(ax);
        al = *(db*)raddr(ds,di+8);
        push(ax);
        sub_1059b();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto loc_10630;
loc_105f6:
        CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
        if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
loc_105fd:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
loc_10605:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0; OF = 0; ZF = ((db)((al & 0x0B0)) == 0); SF = (((db)((al & 0x0B0))) >> 7);
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
        goto loc_1062d;
    }
loc_10615:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0A0)) == 0); SF = (((db)((al & 0x0A0))) >> 7);
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+4) = cl;
    }
loc_1061f:
    CF = 0; OF = 0; ZF = ((db)((al & 0x90)) == 0); SF = (((db)((al & 0x90))) >> 7);
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+5) = cl;
    }
loc_10629:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) == 0) goto loc_105f6;
    cl = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
        if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
        *(dw*)(raddr(ds,di+9)) = cx;
        goto loc_105fa;
    }
loc_1064a:
    *(db*)raddr(ds,di+8) = cl;
loc_1064d:
    cx = *(dw*)raddr(ds,di+9);
    goto loc_105fd;
loc_104bb:
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15); }
loc_104bf:
    do {
        (bx)--; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        _sa = (bx);
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void loc_10506(void) {
    dd _sa = 0, _sb = 0;
    di = 0x2F;
    sub_1051f();
    di = 0x3C;
    sub_1051f();
    di = 0x49;
    sub_1051f();
    di = 0x56;
sub_1051f:
    CF = (dd)*(raddr(ds,di)) < (dd)0; ZF = ((dw)((*(raddr(ds,di))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di))) - (0))) >> 15);
    if (*(raddr(ds,di)) == 0) { return; }
    (*(raddr(ds,di)))--; ZF = ((dw)(*(raddr(ds,di))) == 0); SF = (((dw)(*(raddr(ds,di)))) >> 15);
    if (*(raddr(ds,di)) != 0) {
        CF = (dd)*(dw*)(raddr(ds,di+2)) < (dd)0; ZF = ((dw)((*(dw*)(raddr(ds,di+2))) - (0)) == 0); SF = (((dw)((*(dw*)(raddr(ds,di+2))) - (0))) >> 15);
        if (*(dw*)(raddr(ds,di+2)) == 0) { return; }
        al = *(db*)raddr(ds,di+7);
        al |= al; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
        if (al != 0) {
            bl = al;
            al &= 0x0F0; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
            if (al != 0) {
                (*(raddr(ds,di+4)))--; ZF = ((dw)(*(raddr(ds,di+4))) == 0); SF = (((dw)(*(raddr(ds,di+4)))) >> 15);
                if (*(raddr(ds,di+4)) == 0) {
                    al = *(db*)raddr(ds,di+6);
                    al &= 0x0F0; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
                    _sa = (al);
                    ax = (char)al;
                    cx = *(dw*)raddr(ds,di+2);
                    { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
                    ax &= 0x3FF0; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
                    cx &= 0x0C00F; CF = 0; OF = 0; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
                    ax |= cx; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
                    *(dw*)(raddr(ds,di+2)) = ax;
                    _sa = (ax);
                    al = bl;
                    al &= 0x0F0; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
                    *(db*)raddr(ds,di+4) = al;
                }
            }
loc_10566:
            al = bl;
            al &= 0x0F; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
            if (al != 0) {
                (*(raddr(ds,di+5)))--; ZF = ((dw)(*(raddr(ds,di+5))) == 0); SF = (((dw)(*(raddr(ds,di+5)))) >> 15);
                if (*(raddr(ds,di+5)) == 0) {
                    al = *(db*)raddr(ds,di+6);
                    al &= 0x0F; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
                    cx = *(dw*)raddr(ds,di+2);
                    { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
                    ax &= 0x0F; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
                    cx &= 0x0FFF0; CF = 0; OF = 0; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
                    ax |= cx; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
                    *(dw*)(raddr(ds,di+2)) = ax;
                    bl &= 0x0F; CF = 0; OF = 0; ZF = ((db)(bl) == 0); SF = (((db)(bl)) >> 7);
                    *(db*)raddr(ds,di+5) = bl;
                }
            }
        }
loc_1058c:
        bx = *(dw*)raddr(ds,di+2);
        sub_10889(); return;
    }
loc_10592:
    bx = *(dw*)raddr(ds,di+0x0B);
    al = *(db*)raddr(ds,bx);
    al |= al; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
    if (al == 0) {
sub_1059b:
        bx = *(dw*)raddr(ds,di+2);
        bx |= bx; CF = 0; OF = 0; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        if (bx != 0) {
            bl |= 0x0F; CF = 0; OF = 0; ZF = ((db)(bl) == 0); SF = (((db)(bl)) >> 7);
            sub_10889();
        }
loc_105a8:
        bx = di;
        cx = 0x0D;
        goto loc_104bb;
    }
loc_105d2:
    al = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    ax &= 0x7F; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
    CF = 0; OF = 0; ZF = ((db)((al & 0x88)) == 0); SF = (((db)((al & 0x88))) >> 7);
    if ((al & 0x88) != 0) {
        push(bx);
        push(ax);
        _sa = ((al & 0x88));
        ax = *(dw*)raddr(ds,di+9);
        push(ax);
        al = *(db*)raddr(ds,di+8);
        push(ax);
        sub_1059b();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto loc_10630;
loc_105f6:
        CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
        if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
loc_105fd:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
loc_10605:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0; OF = 0; ZF = ((db)((al & 0x0B0)) == 0); SF = (((db)((al & 0x0B0))) >> 7);
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
        goto loc_1062d;
    }
loc_10615:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0A0)) == 0); SF = (((db)((al & 0x0A0))) >> 7);
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+4) = cl;
    }
loc_1061f:
    CF = 0; OF = 0; ZF = ((db)((al & 0x90)) == 0); SF = (((db)((al & 0x90))) >> 7);
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+5) = cl;
    }
loc_10629:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) == 0) goto loc_105f6;
    cl = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
        if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
        *(dw*)(raddr(ds,di+9)) = cx;
        goto loc_105fa;
    }
loc_1064a:
    *(db*)raddr(ds,di+8) = cl;
loc_1064d:
    cx = *(dw*)raddr(ds,di+9);
    goto loc_105fd;
loc_104bb:
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15); }
loc_104bf:
    do {
        (bx)--; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        _sa = (bx);
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void loc_10566(void) {
    al = bl;
    al &= 0x0F; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
    if (al != 0) {
        (*(raddr(ds,di+5)))--; ZF = ((dw)(*(raddr(ds,di+5))) == 0); SF = (((dw)(*(raddr(ds,di+5)))) >> 15);
        if (*(raddr(ds,di+5)) == 0) {
            al = *(db*)raddr(ds,di+6);
            al &= 0x0F; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
            cx = *(dw*)raddr(ds,di+2);
            { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
            ax &= 0x0F; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
            cx &= 0x0FFF0; CF = 0; OF = 0; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
            ax |= cx; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
            *(dw*)(raddr(ds,di+2)) = ax;
            bl &= 0x0F; CF = 0; OF = 0; ZF = ((db)(bl) == 0); SF = (((db)(bl)) >> 7);
            *(db*)raddr(ds,di+5) = bl;
        }
    }
loc_1058c:
    bx = *(dw*)raddr(ds,di+2);
    sub_10889(); return;
}
void loc_1058c(void) {
    bx = *(dw*)raddr(ds,di+2);
    sub_10889(); return;
}
void loc_10592(void) {
    dd _sa = 0, _sb = 0;
    bx = *(dw*)raddr(ds,di+0x0B);
    al = *(db*)raddr(ds,bx);
    al |= al; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
    if (al == 0) {
sub_1059b:
        bx = *(dw*)raddr(ds,di+2);
        bx |= bx; CF = 0; OF = 0; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        if (bx != 0) {
            bl |= 0x0F; CF = 0; OF = 0; ZF = ((db)(bl) == 0); SF = (((db)(bl)) >> 7);
            sub_10889();
        }
loc_105a8:
        bx = di;
        cx = 0x0D;
        goto loc_104bb;
    }
loc_105d2:
    al = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    ax &= 0x7F; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
    CF = 0; OF = 0; ZF = ((db)((al & 0x88)) == 0); SF = (((db)((al & 0x88))) >> 7);
    if ((al & 0x88) != 0) {
        push(bx);
        push(ax);
        _sa = ((al & 0x88));
        ax = *(dw*)raddr(ds,di+9);
        push(ax);
        al = *(db*)raddr(ds,di+8);
        push(ax);
        sub_1059b();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto loc_10630;
loc_105f6:
        CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
        if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
loc_105fd:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
loc_10605:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0; OF = 0; ZF = ((db)((al & 0x0B0)) == 0); SF = (((db)((al & 0x0B0))) >> 7);
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
        goto loc_1062d;
    }
loc_10615:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0A0)) == 0); SF = (((db)((al & 0x0A0))) >> 7);
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+4) = cl;
    }
loc_1061f:
    CF = 0; OF = 0; ZF = ((db)((al & 0x90)) == 0); SF = (((db)((al & 0x90))) >> 7);
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+5) = cl;
    }
loc_10629:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) == 0) goto loc_105f6;
    cl = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
        if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
        *(dw*)(raddr(ds,di+9)) = cx;
        goto loc_105fa;
    }
loc_1064a:
    *(db*)raddr(ds,di+8) = cl;
loc_1064d:
    cx = *(dw*)raddr(ds,di+9);
    goto loc_105fd;
loc_104bb:
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15); }
loc_104bf:
    do {
        (bx)--; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        _sa = (bx);
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void loc_105a8(void) {
    dd _sa = 0, _sb = 0;
    bx = di;
    cx = 0x0D;
    goto loc_104bb;
loc_104bb:
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15); }
loc_104bf:
    do {
        (bx)--; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        _sa = (bx);
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void loc_105d2(void) {
    dd _sa = 0, _sb = 0;
    al = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    ax &= 0x7F; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
    CF = 0; OF = 0; ZF = ((db)((al & 0x88)) == 0); SF = (((db)((al & 0x88))) >> 7);
    if ((al & 0x88) != 0) {
        push(bx);
        push(ax);
        _sa = ((al & 0x88));
        ax = *(dw*)raddr(ds,di+9);
        push(ax);
        al = *(db*)raddr(ds,di+8);
        push(ax);
        sub_1059b();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto loc_10630;
loc_105f6:
        CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
        if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
loc_105fd:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
loc_10605:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0; OF = 0; ZF = ((db)((al & 0x0B0)) == 0); SF = (((db)((al & 0x0B0))) >> 7);
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
        goto loc_1062d;
    }
loc_10615:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0A0)) == 0); SF = (((db)((al & 0x0A0))) >> 7);
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+4) = cl;
    }
loc_1061f:
    CF = 0; OF = 0; ZF = ((db)((al & 0x90)) == 0); SF = (((db)((al & 0x90))) >> 7);
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+5) = cl;
    }
loc_10629:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) == 0) goto loc_105f6;
    cl = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
        if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
        *(dw*)(raddr(ds,di+9)) = cx;
        goto loc_105fa;
    }
loc_1064a:
    *(db*)raddr(ds,di+8) = cl;
loc_1064d:
    cx = *(dw*)raddr(ds,di+9);
    goto loc_105fd;
}
void loc_105f6(void) {
    CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
    if ((al & 0x84) == 0) {
loc_105fa:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
loc_105fd:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
loc_10642:
    cx = bx;
    (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
    *(dw*)(raddr(ds,di+9)) = cx;
    goto loc_105fa;
}
void loc_105fa(void) {
    cx = bx;
    (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
loc_105fd:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void loc_105fd(void) {
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void loc_10605(void) {
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0; OF = 0; ZF = ((db)((al & 0x0B0)) == 0); SF = (((db)((al & 0x0B0))) >> 7);
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
        goto loc_1062d;
    }
loc_10615:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0A0)) == 0); SF = (((db)((al & 0x0A0))) >> 7);
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+4) = cl;
    }
loc_1061f:
    CF = 0; OF = 0; ZF = ((db)((al & 0x90)) == 0); SF = (((db)((al & 0x90))) >> 7);
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+5) = cl;
    }
loc_10629:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
        if (*(raddr(ds,di+8)) != 0) {
            (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
            if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
            cx = bx;
            (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
            *(dw*)(raddr(ds,di+9)) = cx;
            goto loc_105fa;
        }
loc_1064a:
        *(db*)raddr(ds,di+8) = cl;
loc_1064d:
        cx = *(dw*)raddr(ds,di+9);
        goto loc_105fd;
    }
loc_105f6:
    CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
    if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
    cx = bx;
    (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
loc_105fd:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void loc_10615(void) {
    CF = 0; OF = 0; ZF = ((db)((al & 0x0A0)) == 0); SF = (((db)((al & 0x0A0))) >> 7);
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+4) = cl;
    }
loc_1061f:
    CF = 0; OF = 0; ZF = ((db)((al & 0x90)) == 0); SF = (((db)((al & 0x90))) >> 7);
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+5) = cl;
    }
loc_10629:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
        if (*(raddr(ds,di+8)) != 0) {
            (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
            if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
            cx = bx;
            (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
            *(dw*)(raddr(ds,di+9)) = cx;
            goto loc_105fa;
        }
loc_1064a:
        *(db*)raddr(ds,di+8) = cl;
loc_1064d:
        cx = *(dw*)raddr(ds,di+9);
        goto loc_105fd;
    }
loc_105f6:
    CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
    if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
    cx = bx;
    (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
loc_105fd:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void loc_1061f(void) {
    CF = 0; OF = 0; ZF = ((db)((al & 0x90)) == 0); SF = (((db)((al & 0x90))) >> 7);
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+5) = cl;
    }
loc_10629:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
        if (*(raddr(ds,di+8)) != 0) {
            (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
            if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
            cx = bx;
            (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
            *(dw*)(raddr(ds,di+9)) = cx;
            goto loc_105fa;
        }
loc_1064a:
        *(db*)raddr(ds,di+8) = cl;
loc_1064d:
        cx = *(dw*)raddr(ds,di+9);
        goto loc_105fd;
    }
loc_105f6:
    CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
    if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
    cx = bx;
    (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
loc_105fd:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void loc_10629(void) {
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
        if (*(raddr(ds,di+8)) != 0) {
            (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
            if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
            cx = bx;
            (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
            *(dw*)(raddr(ds,di+9)) = cx;
            goto loc_105fa;
        }
loc_1064a:
        *(db*)raddr(ds,di+8) = cl;
loc_1064d:
        cx = *(dw*)raddr(ds,di+9);
        goto loc_105fd;
    }
loc_105f6:
    CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
    if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
    cx = bx;
    (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
loc_105fd:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void loc_1062d(void) {
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
        if (*(raddr(ds,di+8)) != 0) {
            (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
            if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
            cx = bx;
            (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
            *(dw*)(raddr(ds,di+9)) = cx;
            goto loc_105fa;
        }
loc_1064a:
        *(db*)raddr(ds,di+8) = cl;
loc_1064d:
        cx = *(dw*)raddr(ds,di+9);
        goto loc_105fd;
    }
loc_105f6:
    CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
    if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
    cx = bx;
    (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
loc_105fd:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void loc_10630(void) {
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
        if (*(raddr(ds,di+8)) != 0) {
            (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
            if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
            cx = bx;
            (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
            *(dw*)(raddr(ds,di+9)) = cx;
            goto loc_105fa;
        }
loc_1064a:
        *(db*)raddr(ds,di+8) = cl;
loc_1064d:
        cx = *(dw*)raddr(ds,di+9);
        goto loc_105fd;
    }
loc_105f6:
    CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
    if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
    cx = bx;
    (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
loc_105fd:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void loc_10642(void) {
    cx = bx;
    (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
    *(dw*)(raddr(ds,di+9)) = cx;
    goto loc_105fa;
loc_105fa:
    cx = bx;
    (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
loc_105fd:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void loc_1064a(void) {
    *(db*)raddr(ds,di+8) = cl;
loc_1064d:
    cx = *(dw*)raddr(ds,di+9);
    goto loc_105fd;
loc_105fd:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void loc_1064d(void) {
    cx = *(dw*)raddr(ds,di+9);
    goto loc_105fd;
loc_105fd:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void loc_10694(void) {
    IF = 0;
    ax = 0x3C;
    sub_105c4();
    IF = 1;
locret_1069d:
    return;
}
void loc_106a9(void) {
    *(dw*)(raddr(ds,0x63)) = bx;
    sub_107b5(); return;
}
void loc_106f4(void) {
    sub_107b2();
    bx = 0x143;
    sub_107b5(); return;
}
void loc_10735(void) {
    sub_107b5();
    bx = 0x163;
    sub_107b5(); return;
}
void loc_1074a(void) {
    al = 0x28;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    sub_107b2();
    bx = 0x165;
    sub_107b5(); return;
locret_107ba:
    return;
}
void loc_1079c(void) {
    sub_107b2();
    bx = 0x133;
    sub_107b5(); return;
}
void loc_10805(void) {
    sub_107b5();
    bx = 0x0DA;
    sub_107b5(); return;
}
void loc_108bd(void) {
    dd _sa = 0, _sb = 0;
    al = bh;
    al |= 0x0E0; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
    out(0x0C0, al);
    _sa = (al);
    al = bl;
    al |= 0x0F0; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
    out(0x0C0, al);
    return;
}
void locret_1008c(void) {
    return;
}
void locret_103dd(void) {
    return;
}
void locret_1040a(void) {
    dd _sa = 0, _sb = 0;
    return;
sub_1040b:
    *(raddr(ds,0x67)) = 0;
edummylabel9:
    *(raddr(ds,0x65)) = 0;
    *(dw*)(raddr(ds,0x63)) = 0;
    byte_103e2 = 0;
    IF = 0;
    CF = (dd)*(raddr(ds,0x68)) < (dd)0; ZF = ((dw)((*(raddr(ds,0x68))) - (0)) == 0); SF = (((dw)((*(raddr(ds,0x68))) - (0))) >> 15);
    if (*(raddr(ds,0x68)) != 0) { return; }
    _sa = (*(raddr(ds,0x68)));
    *(raddr(ds,0x68)) = ~*(raddr(ds,0x68));
    push(ds);
    { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
    m2c::set_segment_register(ds, ax);
    bx = *(dw*)raddr(ds,0x20);
    cx = *(dw*)raddr(ds,0x22);
    *(dw*)(&dword_103de) = bx;
    *(dw*)(((db*)&dword_103de)+2) = cx;
    ds = pop();
    IF = 0;
    *(raddr(ds,0x69)) = 3;
    push(ds);
    { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
    m2c::set_segment_register(ds, ax);
    _sa = (ax);
    ax = 0x373;
    *(dw*)(raddr(ds,0x20)) = ax;
    *(dw*)(raddr(ds,0x22)) = cs;
    ds = pop();
    out(0x43, 0x36);
    ax = 0x4DAE;
    out(0x40, al);
    al = ah;
    out(0x40, al);
    IF = 1;
locret_1046a:
    return;
}
void locret_1046a(void) {
    return;
}
void locret_104c4(void) {
    return;
}
void locret_10604(void) {
    return;
}
void locret_1069d(void) {
    return;
}
void locret_106c2(void) {
    return;
}
void locret_106e7(void) {
    return;
}
void locret_107ba(void) {
    return;
}
void locret_108bc(void) {
    return;
}
void seg001_33e_proc(void) {
    dd _sa = 0, _sb = 0;
loc_1046b:
    *(raddr(ds,0x69)) = 3;
    byte_103e2 = 0;
    ds = pop();
    ax = pop();
    { vfn f_ = func_at(rt_far(dword_103de)); if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n", (dd)(rt_far(dword_103de))); return; }
edummylabel10:
    IF = 0;
    push(ds);
    bx = *(dw*)(&dword_103de);
    cx = *(dw*)(((db*)&dword_103de)+2);
    { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
    m2c::set_segment_register(ds, ax);
    *(dw*)(raddr(ds,0x20)) = bx;
    *(dw*)(raddr(ds,0x22)) = cx;
    ds = pop();
    _sa = (ax);
    out(0x43, 0x36);
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    out(0x40, al);
    al = ah;
    out(0x40, al);
    IF = 1;
    return;
}
void module_init(void) {
    install_timer();
edummylabel1:
    return;
}
void install_timer(void) {
    dd _sa = 0, _sb = 0;
    *(raddr(ds,0x67)) = 0;
edummylabel9:
    *(raddr(ds,0x65)) = 0;
    *(dw*)(raddr(ds,0x63)) = 0;
    byte_103e2 = 0;
    IF = 0;
    CF = (dd)*(raddr(ds,0x68)) < (dd)0; ZF = ((dw)((*(raddr(ds,0x68))) - (0)) == 0); SF = (((dw)((*(raddr(ds,0x68))) - (0))) >> 15);
    if (*(raddr(ds,0x68)) != 0) { return; }
    _sa = (*(raddr(ds,0x68)));
    *(raddr(ds,0x68)) = ~*(raddr(ds,0x68));
    push(ds);
    { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
    m2c::set_segment_register(ds, ax);
    bx = *(dw*)raddr(ds,0x20);
    cx = *(dw*)raddr(ds,0x22);
    *(dw*)(&dword_103de) = bx;
    *(dw*)(((db*)&dword_103de)+2) = cx;
    ds = pop();
    IF = 0;
    *(raddr(ds,0x69)) = 3;
    push(ds);
    { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
    m2c::set_segment_register(ds, ax);
    _sa = (ax);
    ax = 0x373;
    *(dw*)(raddr(ds,0x20)) = ax;
    *(dw*)(raddr(ds,0x22)) = cs;
    ds = pop();
    out(0x43, 0x36);
    ax = 0x4DAE;
    out(0x40, al);
    al = ah;
    out(0x40, al);
    IF = 1;
locret_1046a:
    return;
}
void sub_104a4(void) {
    dd _sa = 0, _sb = 0;
    al = 0x9F;
edummylabel11:
    out(0x0C0, al);
    out(0x0C0, 0x0BF);
    out(0x0C0, 0x0DF);
    al = 0x0FF;
    out(0x0C0, al);
    cx = 0x38;
    bx = offset(seg001,unk_1009f);
loc_104bb:
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15); }
loc_104bf:
    do {
        (bx)--; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        _sa = (bx);
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void seq_tick(void) {
    dd _sa = 0, _sb = 0;
    CF = (dd)*(raddr(ds,0x69)) < (dd)1; ZF = ((dw)((*(raddr(ds,0x69))) - (1)) == 0); SF = (((dw)((*(raddr(ds,0x69))) - (1))) >> 15);
edummylabel12:
    if (!ZF) {
        CF = (dd)*(dw*)(raddr(ds,0x63)) < (dd)0; ZF = ((dw)((*(dw*)(raddr(ds,0x63))) - (0)) == 0); SF = (((dw)((*(dw*)(raddr(ds,0x63))) - (0))) >> 15);
        if (*(dw*)(raddr(ds,0x63)) != 0) {
            CF = (dd)*(raddr(ds,0x49)) < (dd)0; ZF = ((dw)((*(raddr(ds,0x49))) - (0)) == 0); SF = (((dw)((*(raddr(ds,0x49))) - (0))) >> 15);
            if (*(raddr(ds,0x49)) == 0) {
                bx = *(dw*)raddr(ds,0x63);
                sub_105b0(); return;
            }
        }
loc_104e1:
        CF = (dd)*(raddr(ds,0x65)) < (dd)0; ZF = ((dw)((*(raddr(ds,0x65))) - (0)) == 0); SF = (((dw)((*(raddr(ds,0x65))) - (0))) >> 15);
        if (*(raddr(ds,0x65)) != 0) {
            CF = (dd)*(raddr(ds,0x66)) < (dd)0; ZF = ((dw)((*(raddr(ds,0x66))) - (0)) == 0); SF = (((dw)((*(raddr(ds,0x66))) - (0))) >> 15);
            if ((short)*(raddr(ds,0x66)) <= (short)0) {
                _sa = (*(raddr(ds,0x66)));
                *(raddr(ds,0x66)) = 0x1E;
                CF = (dd)*(raddr(ds,0x2F)) < (dd)0; ZF = ((dw)((*(raddr(ds,0x2F))) - (0)) == 0); SF = (((dw)((*(raddr(ds,0x2F))) - (0))) >> 15);
                if (*(raddr(ds,0x2F)) == 0) {
                    bx = 0x1B6;
                    sub_105b0(); return;
                }
            }
loc_10502:
            (*(raddr(ds,0x66)))--; ZF = ((dw)(*(raddr(ds,0x66))) == 0); SF = (((dw)(*(raddr(ds,0x66)))) >> 15);
        }
    }
loc_10506:
    di = 0x2F;
    sub_1051f();
    di = 0x3C;
    sub_1051f();
    di = 0x49;
    sub_1051f();
    di = 0x56;
sub_1051f:
    CF = (dd)*(raddr(ds,di)) < (dd)0; ZF = ((dw)((*(raddr(ds,di))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di))) - (0))) >> 15);
    if (*(raddr(ds,di)) == 0) { return; }
    (*(raddr(ds,di)))--; ZF = ((dw)(*(raddr(ds,di))) == 0); SF = (((dw)(*(raddr(ds,di)))) >> 15);
    if (*(raddr(ds,di)) != 0) {
        CF = (dd)*(dw*)(raddr(ds,di+2)) < (dd)0; ZF = ((dw)((*(dw*)(raddr(ds,di+2))) - (0)) == 0); SF = (((dw)((*(dw*)(raddr(ds,di+2))) - (0))) >> 15);
        if (*(dw*)(raddr(ds,di+2)) == 0) { return; }
        al = *(db*)raddr(ds,di+7);
        al |= al; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
        if (al != 0) {
            bl = al;
            al &= 0x0F0; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
            if (al != 0) {
                (*(raddr(ds,di+4)))--; ZF = ((dw)(*(raddr(ds,di+4))) == 0); SF = (((dw)(*(raddr(ds,di+4)))) >> 15);
                if (*(raddr(ds,di+4)) == 0) {
                    al = *(db*)raddr(ds,di+6);
                    al &= 0x0F0; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
                    _sa = (al);
                    ax = (char)al;
                    cx = *(dw*)raddr(ds,di+2);
                    { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
                    ax &= 0x3FF0; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
                    cx &= 0x0C00F; CF = 0; OF = 0; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
                    ax |= cx; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
                    *(dw*)(raddr(ds,di+2)) = ax;
                    _sa = (ax);
                    al = bl;
                    al &= 0x0F0; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
                    *(db*)raddr(ds,di+4) = al;
                }
            }
loc_10566:
            al = bl;
            al &= 0x0F; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
            if (al != 0) {
                (*(raddr(ds,di+5)))--; ZF = ((dw)(*(raddr(ds,di+5))) == 0); SF = (((dw)(*(raddr(ds,di+5)))) >> 15);
                if (*(raddr(ds,di+5)) == 0) {
                    al = *(db*)raddr(ds,di+6);
                    al &= 0x0F; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
                    cx = *(dw*)raddr(ds,di+2);
                    { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
                    ax &= 0x0F; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
                    cx &= 0x0FFF0; CF = 0; OF = 0; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
                    ax |= cx; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
                    *(dw*)(raddr(ds,di+2)) = ax;
                    bl &= 0x0F; CF = 0; OF = 0; ZF = ((db)(bl) == 0); SF = (((db)(bl)) >> 7);
                    *(db*)raddr(ds,di+5) = bl;
                }
            }
        }
loc_1058c:
        bx = *(dw*)raddr(ds,di+2);
        sub_10889(); return;
    }
loc_10592:
    bx = *(dw*)raddr(ds,di+0x0B);
    al = *(db*)raddr(ds,bx);
    al |= al; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
    if (al == 0) {
sub_1059b:
        bx = *(dw*)raddr(ds,di+2);
        bx |= bx; CF = 0; OF = 0; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        if (bx != 0) {
            bl |= 0x0F; CF = 0; OF = 0; ZF = ((db)(bl) == 0); SF = (((db)(bl)) >> 7);
            sub_10889();
        }
loc_105a8:
        bx = di;
        cx = 0x0D;
        goto loc_104bb;
    }
loc_105d2:
    al = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    ax &= 0x7F; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
    CF = 0; OF = 0; ZF = ((db)((al & 0x88)) == 0); SF = (((db)((al & 0x88))) >> 7);
    if ((al & 0x88) != 0) {
        push(bx);
        push(ax);
        _sa = ((al & 0x88));
        ax = *(dw*)raddr(ds,di+9);
        push(ax);
        al = *(db*)raddr(ds,di+8);
        push(ax);
        sub_1059b();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto loc_10630;
loc_105f6:
        CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
        if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
loc_105fd:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
loc_10605:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0; OF = 0; ZF = ((db)((al & 0x0B0)) == 0); SF = (((db)((al & 0x0B0))) >> 7);
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
        goto loc_1062d;
    }
loc_10615:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0A0)) == 0); SF = (((db)((al & 0x0A0))) >> 7);
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+4) = cl;
    }
loc_1061f:
    CF = 0; OF = 0; ZF = ((db)((al & 0x90)) == 0); SF = (((db)((al & 0x90))) >> 7);
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+5) = cl;
    }
loc_10629:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) == 0) goto loc_105f6;
    cl = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
        if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
        *(dw*)(raddr(ds,di+9)) = cx;
        goto loc_105fa;
    }
loc_1064a:
    *(db*)raddr(ds,di+8) = cl;
loc_1064d:
    cx = *(dw*)raddr(ds,di+9);
    goto loc_105fd;
loc_104bb:
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15); }
loc_104bf:
    do {
        (bx)--; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        _sa = (bx);
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void sub_1051f(void) {
    dd _sa = 0, _sb = 0;
    CF = (dd)*(raddr(ds,di)) < (dd)0; ZF = ((dw)((*(raddr(ds,di))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di))) - (0))) >> 15);
    if (*(raddr(ds,di)) == 0) { return; }
    (*(raddr(ds,di)))--; ZF = ((dw)(*(raddr(ds,di))) == 0); SF = (((dw)(*(raddr(ds,di)))) >> 15);
    if (*(raddr(ds,di)) != 0) {
        CF = (dd)*(dw*)(raddr(ds,di+2)) < (dd)0; ZF = ((dw)((*(dw*)(raddr(ds,di+2))) - (0)) == 0); SF = (((dw)((*(dw*)(raddr(ds,di+2))) - (0))) >> 15);
        if (*(dw*)(raddr(ds,di+2)) == 0) { return; }
        al = *(db*)raddr(ds,di+7);
        al |= al; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
        if (al != 0) {
            bl = al;
            al &= 0x0F0; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
            if (al != 0) {
                (*(raddr(ds,di+4)))--; ZF = ((dw)(*(raddr(ds,di+4))) == 0); SF = (((dw)(*(raddr(ds,di+4)))) >> 15);
                if (*(raddr(ds,di+4)) == 0) {
                    al = *(db*)raddr(ds,di+6);
                    al &= 0x0F0; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
                    _sa = (al);
                    ax = (char)al;
                    cx = *(dw*)raddr(ds,di+2);
                    { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
                    ax &= 0x3FF0; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
                    cx &= 0x0C00F; CF = 0; OF = 0; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
                    ax |= cx; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
                    *(dw*)(raddr(ds,di+2)) = ax;
                    _sa = (ax);
                    al = bl;
                    al &= 0x0F0; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
                    *(db*)raddr(ds,di+4) = al;
                }
            }
loc_10566:
            al = bl;
            al &= 0x0F; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
            if (al != 0) {
                (*(raddr(ds,di+5)))--; ZF = ((dw)(*(raddr(ds,di+5))) == 0); SF = (((dw)(*(raddr(ds,di+5)))) >> 15);
                if (*(raddr(ds,di+5)) == 0) {
                    al = *(db*)raddr(ds,di+6);
                    al &= 0x0F; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
                    cx = *(dw*)raddr(ds,di+2);
                    { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
                    ax &= 0x0F; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
                    cx &= 0x0FFF0; CF = 0; OF = 0; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
                    ax |= cx; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
                    *(dw*)(raddr(ds,di+2)) = ax;
                    bl &= 0x0F; CF = 0; OF = 0; ZF = ((db)(bl) == 0); SF = (((db)(bl)) >> 7);
                    *(db*)raddr(ds,di+5) = bl;
                }
            }
        }
loc_1058c:
        bx = *(dw*)raddr(ds,di+2);
        sub_10889(); return;
    }
loc_10592:
    bx = *(dw*)raddr(ds,di+0x0B);
    al = *(db*)raddr(ds,bx);
    al |= al; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
    if (al == 0) {
sub_1059b:
        bx = *(dw*)raddr(ds,di+2);
        bx |= bx; CF = 0; OF = 0; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        if (bx != 0) {
            bl |= 0x0F; CF = 0; OF = 0; ZF = ((db)(bl) == 0); SF = (((db)(bl)) >> 7);
            sub_10889();
        }
loc_105a8:
        bx = di;
        cx = 0x0D;
        goto loc_104bb;
    }
loc_105d2:
    al = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    ax &= 0x7F; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
    CF = 0; OF = 0; ZF = ((db)((al & 0x88)) == 0); SF = (((db)((al & 0x88))) >> 7);
    if ((al & 0x88) != 0) {
        push(bx);
        push(ax);
        _sa = ((al & 0x88));
        ax = *(dw*)raddr(ds,di+9);
        push(ax);
        al = *(db*)raddr(ds,di+8);
        push(ax);
        sub_1059b();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto loc_10630;
loc_105f6:
        CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
        if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
loc_105fd:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
loc_10605:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0; OF = 0; ZF = ((db)((al & 0x0B0)) == 0); SF = (((db)((al & 0x0B0))) >> 7);
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
        goto loc_1062d;
    }
loc_10615:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0A0)) == 0); SF = (((db)((al & 0x0A0))) >> 7);
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+4) = cl;
    }
loc_1061f:
    CF = 0; OF = 0; ZF = ((db)((al & 0x90)) == 0); SF = (((db)((al & 0x90))) >> 7);
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+5) = cl;
    }
loc_10629:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) == 0) goto loc_105f6;
    cl = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
        if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
        *(dw*)(raddr(ds,di+9)) = cx;
        goto loc_105fa;
    }
loc_1064a:
    *(db*)raddr(ds,di+8) = cl;
loc_1064d:
    cx = *(dw*)raddr(ds,di+9);
    goto loc_105fd;
loc_104bb:
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15); }
loc_104bf:
    do {
        (bx)--; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        _sa = (bx);
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void sub_1059b(void) {
    dd _sa = 0, _sb = 0;
    bx = *(dw*)raddr(ds,di+2);
    bx |= bx; CF = 0; OF = 0; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    if (bx != 0) {
        bl |= 0x0F; CF = 0; OF = 0; ZF = ((db)(bl) == 0); SF = (((db)(bl)) >> 7);
        sub_10889();
    }
loc_105a8:
    bx = di;
    cx = 0x0D;
    goto loc_104bb;
loc_104bb:
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15); }
loc_104bf:
    do {
        (bx)--; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        _sa = (bx);
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void sub_105b0(void) {
    dd _sa = 0, _sb = 0;
edummylabel13:
    al = *(db*)raddr(ds,bx);
    al |= al; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
    if (al == 0) { return; }
    al &= 3; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
    _sa = (al);
    ax = (char)al;
    {dd r = (dd)ax * 0x0D; ax = r; dx = r >> 16;}
    cx = 0x2F;
    { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
sub_105c4:
    di = ax;
    push(bx);
    push(*(dw*)(raddr(ds,di)));
    sub_1059b();
    *(dw*)(raddr(ds,di)) = pop();
    bx = pop();
    *(dw*)(raddr(ds,di+9)) = bx;
loc_105d2:
    al = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    ax &= 0x7F; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
    CF = 0; OF = 0; ZF = ((db)((al & 0x88)) == 0); SF = (((db)((al & 0x88))) >> 7);
    if ((al & 0x88) != 0) {
        push(bx);
        push(ax);
        _sa = ((al & 0x88));
        ax = *(dw*)raddr(ds,di+9);
        push(ax);
        al = *(db*)raddr(ds,di+8);
        push(ax);
        sub_1059b();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto loc_10630;
loc_105f6:
        CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
        if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
loc_105fd:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
loc_10605:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0; OF = 0; ZF = ((db)((al & 0x0B0)) == 0); SF = (((db)((al & 0x0B0))) >> 7);
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
        goto loc_1062d;
    }
loc_10615:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0A0)) == 0); SF = (((db)((al & 0x0A0))) >> 7);
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+4) = cl;
    }
loc_1061f:
    CF = 0; OF = 0; ZF = ((db)((al & 0x90)) == 0); SF = (((db)((al & 0x90))) >> 7);
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+5) = cl;
    }
loc_10629:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) == 0) goto loc_105f6;
    cl = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
        if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
        *(dw*)(raddr(ds,di+9)) = cx;
        goto loc_105fa;
    }
loc_1064a:
    *(db*)raddr(ds,di+8) = cl;
loc_1064d:
    cx = *(dw*)raddr(ds,di+9);
    goto loc_105fd;
}
void sub_105c4(void) {
    dd _sa = 0, _sb = 0;
    di = ax;
    push(bx);
    push(*(dw*)(raddr(ds,di)));
    sub_1059b();
    *(dw*)(raddr(ds,di)) = pop();
    bx = pop();
    *(dw*)(raddr(ds,di+9)) = bx;
loc_105d2:
    al = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    ax &= 0x7F; CF = 0; OF = 0; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15);
    CF = 0; OF = 0; ZF = ((db)((al & 0x88)) == 0); SF = (((db)((al & 0x88))) >> 7);
    if ((al & 0x88) != 0) {
        push(bx);
        push(ax);
        _sa = ((al & 0x88));
        ax = *(dw*)raddr(ds,di+9);
        push(ax);
        al = *(db*)raddr(ds,di+8);
        push(ax);
        sub_1059b();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto loc_10630;
loc_105f6:
        CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
        if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
loc_105fd:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
loc_10605:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0; OF = 0; ZF = ((db)((al & 0x0B0)) == 0); SF = (((db)((al & 0x0B0))) >> 7);
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
        goto loc_1062d;
    }
loc_10615:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0A0)) == 0); SF = (((db)((al & 0x0A0))) >> 7);
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+4) = cl;
    }
loc_1061f:
    CF = 0; OF = 0; ZF = ((db)((al & 0x90)) == 0); SF = (((db)((al & 0x90))) >> 7);
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+5) = cl;
    }
loc_10629:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) == 0) goto loc_105f6;
    cl = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
        if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
        *(dw*)(raddr(ds,di+9)) = cx;
        goto loc_105fa;
    }
loc_1064a:
    *(db*)raddr(ds,di+8) = cl;
loc_1064d:
    cx = *(dw*)raddr(ds,di+9);
    goto loc_105fd;
}
void sub_10652(void) {
edummylabel14:
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x0F0;
    sub_107b2(); return;
locret_1069d:
    return;
}
void sub_10661(void) {
edummylabel15:
    al = 0x1E;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x108;
    sub_107b2();
    bx = 0x100;
    sub_107b5(); return;
locret_1069d:
    return;
}
void sub_10677(void) {
edummylabel16:
    bx = 0x9D;
    sub_107b5(); return;
}
void sub_1067e(void) {
edummylabel17:
    al = 0x64;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    bx = 0x198;
    if (al >= *(db*)raddr(ds,0x30)) {
        sub_107b2();
        bx = 0x1A7;
        sub_107b5(); return;
    }
loc_10694:
    IF = 0;
    ax = 0x3C;
    sub_105c4();
    IF = 1;
locret_1069d:
    return;
}
void sub_1069e(void) {
edummylabel18:
    bx = 0x1BB;
    goto loc_106a9;
loc_106a9:
    *(dw*)(raddr(ds,0x63)) = bx;
    sub_107b5(); return;
}
void sub_106a5(void) {
edummylabel19:
    bx = 0x1C3;
loc_106a9:
    *(dw*)(raddr(ds,0x63)) = bx;
    sub_107b5(); return;
}
void sub_106b0(void) {
    dd _sa = 0, _sb = 0;
edummylabel20:
    *(dw*)(raddr(ds,0x63)) = 0;
    CF = (dd)*(raddr(ds,0x49)) < (dd)0; ZF = ((dw)((*(raddr(ds,0x49))) - (0)) == 0); SF = (((dw)((*(raddr(ds,0x49))) - (0))) >> 15);
    if (*(raddr(ds,0x49)) == 0) { return; }
    _sa = (*(raddr(ds,0x49)));
    *(raddr(ds,0x49)) = 1;
locret_106c2:
    return;
}
void sub_106c3(void) {
edummylabel21:
    *(dw*)(raddr(ds,0x63)) = 0;
    bx = 0x74;
    sub_107b5(); return;
}
void sub_106d0(void) {
edummylabel22:
    bx = 0x72;
    sub_107b5(); return;
}
void sub_106d7(void) {
edummylabel23:
    *(raddr(ds,0x66)) = 0x1E;
    *(raddr(ds,0x65)) = 0x0FF;
    return;
}
void sub_106e2(void) {
edummylabel24:
    *(raddr(ds,0x65)) = 0;
locret_106e7:
    return;
}
void sub_106e8(void) {
edummylabel25:
    al = 0x4B;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x153;
loc_106f4:
    sub_107b2();
    bx = 0x143;
    sub_107b5(); return;
locret_106e7:
    return;
}
void sub_106fe(void) {
edummylabel26:
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x8D;
    sub_107b2(); return;
locret_106e7:
    return;
}
void sub_1070d(void) {
edummylabel27:
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x11F;
    sub_107b2(); return;
locret_106e7:
    return;
}
void sub_1071c(void) {
edummylabel28:
    al = 0x41;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x15B;
    goto loc_106f4;
loc_106f4:
    sub_107b2();
    bx = 0x143;
    sub_107b5(); return;
}
void sub_1072a(void) {
edummylabel29:
    bx = 0x16A;
    goto loc_10735;
loc_10735:
    sub_107b5();
    bx = 0x163;
    sub_107b5(); return;
}
void sub_10731(void) {
edummylabel30:
    bx = 0x174;
loc_10735:
    sub_107b5();
    bx = 0x163;
    sub_107b5(); return;
}
void sub_1073f(void) {
edummylabel31:
    bx = 0x16C;
    goto loc_1074a;
loc_1074a:
    al = 0x28;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    sub_107b2();
    bx = 0x165;
    sub_107b5(); return;
locret_107ba:
    return;
}
void sub_10746(void) {
edummylabel32:
    bx = 0x176;
loc_1074a:
    al = 0x28;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    sub_107b2();
    bx = 0x165;
    sub_107b5(); return;
locret_107ba:
    return;
}
void sub_1075c(void) {
edummylabel33:
    al = 0x3C;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x0A6;
    sub_107b2(); return;
locret_107ba:
    return;
}
void sub_1076b(void) {
edummylabel34:
    al = 0x63;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x187;
    sub_107b2();
    bx = 0x17E;
    sub_107b5(); return;
locret_107ba:
    return;
}
void sub_10781(void) {
edummylabel35:
    al = 0x46;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x0F8;
    goto loc_1079c;
loc_1079c:
    sub_107b2();
    bx = 0x133;
    sub_107b5(); return;
locret_107ba:
    return;
}
void sub_10790(void) {
edummylabel36:
    al = 0x5F;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x13B;
loc_1079c:
    sub_107b2();
    bx = 0x133;
    sub_107b5(); return;
locret_107ba:
    return;
}
void sub_107a6(void) {
edummylabel37:
    al = 0x2E;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x126;
sub_107b2:
    *(db*)raddr(ds,0x30) = al;
sub_107b5:
    IF = 0;
    sub_105b0();
    IF = 1;
locret_107ba:
    return;
}
void sub_107b2(void) {
    *(db*)raddr(ds,0x30) = al;
sub_107b5:
    IF = 0;
    sub_105b0();
    IF = 1;
locret_107ba:
    return;
}
void sub_107b5(void) {
    IF = 0;
    sub_105b0();
    IF = 1;
locret_107ba:
    return;
}
void sub_107bb(void) {
edummylabel38:
    al = 0x2D;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x12E;
    sub_107b2(); return;
locret_107ba:
    return;
}
void sub_107c9(void) {
edummylabel39:
    al = 0x55;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x190;
    sub_107b2(); return;
locret_107ba:
    return;
}
void sub_107d7(void) {
edummylabel40:
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x117;
    sub_107b2(); return;
locret_107ba:
    return;
}
void sub_107e5(void) {
edummylabel41:
    al = 0x50;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x14B;
    sub_107b2();
    bx = 0x153;
    sub_107b5(); return;
locret_107ba:
    return;
}
void sub_107fa(void) {
edummylabel42:
    bx = 0x0AE;
    goto loc_10805;
loc_10805:
    sub_107b5();
    bx = 0x0DA;
    sub_107b5(); return;
}
void sub_10801(void) {
edummylabel43:
    bx = 0x0C4;
loc_10805:
    sub_107b5();
    bx = 0x0DA;
    sub_107b5(); return;
}
void sub_1080e(void) {
edummylabel44:
    bx = 0x0E1;
    sub_107b5(); return;
}
void sub_10814(void) {
edummylabel45:
    bx = 0x1D0;
    sub_107b5(); return;
}
void sub_1081a(void) {
edummylabel46:
    bx = 0x1CB;
    sub_107b5(); return;
}
void sub_10820(void) {
edummylabel47:
    bx = 0x6A;
    sub_107b5(); return;
}
void sub_10826(void) {
edummylabel48:
    bx = 0x110;
    sub_107b5();
    bx = 0x84;
    sub_107b5(); return;
}
void sub_10833(void) {
edummylabel49:
    bx = 0x1EF;
    sub_107b5();
    *(dw*)(raddr(ds,0x38)) = 0x1DF;
    bx = 0x23D;
    sub_107b5();
    bx = 0x235;
    *(dw*)(raddr(ds,0x45)) = bx;
sub_10851:
    bx = 0x28B;
    sub_107b5();
    *(dw*)(raddr(ds,0x52)) = 0x26B;
    bx = 0x2FD;
    sub_107b5(); return;
}
void sub_10851(void) {
    bx = 0x28B;
    sub_107b5();
    *(dw*)(raddr(ds,0x52)) = 0x26B;
    bx = 0x2FD;
    sub_107b5(); return;
}
void sub_10867(void) {
edummylabel50:
    sub_10833();
    *(dw*)(raddr(ds,0x38)) = 0x210;
    *(dw*)(raddr(ds,0x45)) = 0x246;
    bx = 0x2AC;
    *(dw*)(raddr(ds,0x52)) = bx;
    bl = 0x18;
    *(db*)raddr(ds,0x5E) = bl;
    return;
}
void sub_10889(void) {
    dd _sa = 0, _sb = 0;
edummylabel51:
    CF = (dd)*(raddr(ds,0x67)) < (dd)0x0FF; ZF = ((dw)((*(raddr(ds,0x67))) - (0x0FF)) == 0); SF = (((dw)((*(raddr(ds,0x67))) - (0x0FF))) >> 15);
    if (*(raddr(ds,0x67)) == 0x0FF) { return; }
    ax = bx;
    CF = (dd)ah < (dd)0x0BF; ZF = ((db)((ah) - (0x0BF)) == 0); SF = (((db)((ah) - (0x0BF))) >> 7);
    if (ah <= 0x0BF) {
        { if (1) { CF = (ah >> ((1)-1)) & 1; ah = ah >> 1; ZF = ((db)(ah) == 0); SF = (((db)(ah)) >> 7); } }
        ah |= 0x90; CF = 0; OF = 0; ZF = ((db)(ah) == 0); SF = (((db)(ah)) >> 7);
        ah &= 0x0F0; CF = 0; OF = 0; ZF = ((db)(ah) == 0); SF = (((db)(ah)) >> 7);
        al &= 0x0F; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
        al |= ah; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
        out(0x0C0, al);
        ah &= 0x0E0; CF = 0; OF = 0; ZF = ((db)(ah) == 0); SF = (((db)(ah)) >> 7);
        _sa = (ah);
        al = bl;
        { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
        { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
        { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
        { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7); } }
        al |= ah; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
        out(0x0C0, al);
        _sa = (al);
        al = bh;
        al &= 0x3F; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
        out(0x0C0, al);
locret_108bc:
        return;
    }
loc_108bd:
    al = bh;
    al |= 0x0E0; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
    out(0x0C0, al);
    _sa = (al);
    al = bl;
    al |= 0x0F0; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
    out(0x0C0, al);
    return;
}