#include "../rt.h"
#include "../tnd_syms.h"
#include "../tnd_procs.h"
void tnd_edummylabel1(void) {
    return;
}
void tnd_edummylabel10(void) {
    IF = 0;
    push(ds);
    bx = *(dw*)(&dword_103de);
    cx = *(dw*)(((db*)&dword_103de)+2);
    { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; }
    ds = ax;
    *(dw*)(raddr(ds,0x20)) = bx;
    *(dw*)(raddr(ds,0x22)) = cx;
    ds = pop();
    al = 0x36;
    out(0x43, al);
    ax = 0; CF = 0;
    out(0x40, al);
    al = ah;
    out(0x40, al);
    IF = 1;
    return;
}
void tnd_edummylabel11(void) {
    out(0x0C0, al);
    out(0x0C0, 0x0BF);
    out(0x0C0, 0x0DF);
    al = 0x0FF;
    out(0x0C0, al);
    cx = 0x38;
    bx = 0x2f;
loc_104bb:
    ax = 0; CF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; }
loc_104bf:
    do {
        (bx)--;
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void tnd_edummylabel12(void) {
    if (!ZF) {
        CF = (dd)*(dw*)(raddr(ds,0x63)) < (dd)0;
        if (*(dw*)(raddr(ds,0x63)) != 0) {
            CF = (dd)*(raddr(ds,0x49)) < (dd)0;
            if (*(raddr(ds,0x49)) == 0) {
                bx = *(dw*)raddr(ds,0x63);
                tnd_sub_105b0(); return;
            }
        }
loc_104e1:
        CF = (dd)*(raddr(ds,0x65)) < (dd)0;
        if (*(raddr(ds,0x65)) != 0) {
            CF = (dd)*(raddr(ds,0x66)) < (dd)0;
            if ((short)*(raddr(ds,0x66)) <= (short)0) {
                *(raddr(ds,0x66)) = 0x1E;
                CF = (dd)*(raddr(ds,0x2F)) < (dd)0;
                if (*(raddr(ds,0x2F)) == 0) {
                    bx = 0x1B6;
                    tnd_sub_105b0(); return;
                }
            }
loc_10502:
            (*(raddr(ds,0x66)))--;
        }
    }
loc_10506:
    di = 0x2F;
    tnd_sub_1051f();
    di = 0x3C;
    tnd_sub_1051f();
    di = 0x49;
    tnd_sub_1051f();
    di = 0x56;
sub_1051f:
    CF = (dd)*(raddr(ds,di)) < (dd)0;
    if (*(raddr(ds,di)) == 0) { return; }
    (*(raddr(ds,di)))--;
    if (*(raddr(ds,di)) != 0) {
        CF = (dd)*(dw*)(raddr(ds,di+2)) < (dd)0;
        if (*(dw*)(raddr(ds,di+2)) == 0) { return; }
        al = *(db*)raddr(ds,di+7);
        al |= al; CF = 0;
        if (al != 0) {
            bl = al;
            al &= 0x0F0; CF = 0;
            if (al != 0) {
                (*(raddr(ds,di+4)))--;
                if (*(raddr(ds,di+4)) == 0) {
                    al = *(db*)raddr(ds,di+6);
                    al &= 0x0F0; CF = 0;
                    ax = (char)al;
                    cx = *(dw*)raddr(ds,di+2);
                    { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; }
                    ax &= 0x3FF0; CF = 0;
                    cx &= 0x0C00F; CF = 0;
                    ax |= cx; CF = 0;
                    *(dw*)(raddr(ds,di+2)) = ax;
                    al = bl;
                    al &= 0x0F0; CF = 0;
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
                    *(db*)raddr(ds,di+4) = al;
                }
            }
loc_10566:
            al = bl;
            al &= 0x0F; CF = 0;
            if (al != 0) {
                (*(raddr(ds,di+5)))--;
                if (*(raddr(ds,di+5)) == 0) {
                    al = *(db*)raddr(ds,di+6);
                    al &= 0x0F; CF = 0;
                    cx = *(dw*)raddr(ds,di+2);
                    { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; }
                    ax &= 0x0F; CF = 0;
                    cx &= 0x0FFF0; CF = 0;
                    ax |= cx; CF = 0;
                    *(dw*)(raddr(ds,di+2)) = ax;
                    bl &= 0x0F; CF = 0;
                    *(db*)raddr(ds,di+5) = bl;
                }
            }
        }
loc_1058c:
        bx = *(dw*)raddr(ds,di+2);
        tnd_sub_10889(); return;
    }
loc_10592:
    bx = *(dw*)raddr(ds,di+0x0B);
    al = *(db*)raddr(ds,bx);
    al |= al; CF = 0;
    if (al == 0) {
sub_1059b:
        bx = *(dw*)raddr(ds,di+2);
        bx |= bx; CF = 0;
        if (bx != 0) {
            bl |= 0x0F; CF = 0;
            tnd_sub_10889();
        }
loc_105a8:
        bx = di;
        cx = 0x0D;
        goto loc_104bb;
    }
loc_105d2:
    al = *(db*)raddr(ds,bx);
    (bx)++;
    ax &= 0x7F; CF = 0;
    CF = 0;
    if ((al & 0x88) != 0) {
        push(bx);
        push(ax);
        ax = *(dw*)raddr(ds,di+9);
        push(ax);
        al = *(db*)raddr(ds,di+8);
        push(ax);
        tnd_sub_1059b();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto loc_10630;
loc_105f6:
        CF = 0;
        if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
        cx = bx;
        (cx)++;
loc_105fd:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
loc_10605:
    cx = *(dw*)raddr(ds,bx);
    (bx)++;
    (bx)++;
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0;
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0;
        goto loc_1062d;
    }
loc_10615:
    CF = 0;
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        *(db*)raddr(ds,di+4) = cl;
    }
loc_1061f:
    CF = 0;
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        *(db*)raddr(ds,di+5) = cl;
    }
loc_10629:
    cx = *(dw*)raddr(ds,bx);
    (bx)++;
    (bx)++;
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0;
    if ((al & 0x0C0) == 0) goto loc_105f6;
    cl = *(db*)raddr(ds,bx);
    (bx)++;
    CF = (dd)*(raddr(ds,di+8)) < (dd)0;
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--;
        if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
        cx = bx;
        (cx)++;
        *(dw*)(raddr(ds,di+9)) = cx;
        goto loc_105fa;
    }
loc_1064a:
    *(db*)raddr(ds,di+8) = cl;
loc_1064d:
    cx = *(dw*)raddr(ds,di+9);
    goto loc_105fd;
loc_104bb:
    ax = 0; CF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; }
loc_104bf:
    do {
        (bx)--;
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void tnd_edummylabel13(void) {
    al = *(db*)raddr(ds,bx);
    al |= al; CF = 0;
    if (al == 0) { return; }
    al &= 3; CF = 0;
    ax = (char)al;
    {dd r = (dd)ax * 0x0D; ax = r; dx = r >> 16;}
    cx = 0x2F;
    { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; }
sub_105c4:
    di = ax;
    push(bx);
    push(*(dw*)(raddr(ds,di)));
    tnd_sub_1059b();
    *(dw*)(raddr(ds,di)) = pop();
    bx = pop();
    *(dw*)(raddr(ds,di+9)) = bx;
loc_105d2:
    al = *(db*)raddr(ds,bx);
    (bx)++;
    ax &= 0x7F; CF = 0;
    CF = 0;
    if ((al & 0x88) != 0) {
        push(bx);
        push(ax);
        ax = *(dw*)raddr(ds,di+9);
        push(ax);
        al = *(db*)raddr(ds,di+8);
        push(ax);
        tnd_sub_1059b();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto loc_10630;
loc_105f6:
        CF = 0;
        if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
        cx = bx;
        (cx)++;
loc_105fd:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
loc_10605:
    cx = *(dw*)raddr(ds,bx);
    (bx)++;
    (bx)++;
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0;
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0;
        goto loc_1062d;
    }
loc_10615:
    CF = 0;
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        *(db*)raddr(ds,di+4) = cl;
    }
loc_1061f:
    CF = 0;
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        *(db*)raddr(ds,di+5) = cl;
    }
loc_10629:
    cx = *(dw*)raddr(ds,bx);
    (bx)++;
    (bx)++;
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0;
    if ((al & 0x0C0) == 0) goto loc_105f6;
    cl = *(db*)raddr(ds,bx);
    (bx)++;
    CF = (dd)*(raddr(ds,di+8)) < (dd)0;
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--;
        if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
        cx = bx;
        (cx)++;
        *(dw*)(raddr(ds,di+9)) = cx;
        goto loc_105fa;
    }
loc_1064a:
    *(db*)raddr(ds,di+8) = cl;
loc_1064d:
    cx = *(dw*)raddr(ds,di+9);
    goto loc_105fd;
}
void tnd_edummylabel14(void) {
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x0F0;
    tnd_sub_107b2(); return;
locret_1069d:
    return;
}
void tnd_edummylabel15(void) {
    al = 0x1E;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x108;
    tnd_sub_107b2();
    bx = 0x100;
    tnd_sub_107b5(); return;
locret_1069d:
    return;
}
void tnd_edummylabel16(void) {
    bx = 0x9D;
    tnd_sub_107b5(); return;
}
void tnd_edummylabel17(void) {
    al = 0x64;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    bx = 0x198;
    if (al >= *(db*)raddr(ds,0x30)) {
        tnd_sub_107b2();
        bx = 0x1A7;
        tnd_sub_107b5(); return;
    }
loc_10694:
    IF = 0;
    ax = 0x3C;
    tnd_sub_105c4();
    IF = 1;
locret_1069d:
    return;
}
void tnd_edummylabel18(void) {
    bx = 0x1BB;
    goto loc_106a9;
loc_106a9:
    *(dw*)(raddr(ds,0x63)) = bx;
    tnd_sub_107b5(); return;
}
void tnd_edummylabel19(void) {
    bx = 0x1C3;
loc_106a9:
    *(dw*)(raddr(ds,0x63)) = bx;
    tnd_sub_107b5(); return;
}
void tnd_edummylabel2(void) {
    return;
}
void tnd_edummylabel20(void) {
    *(dw*)(raddr(ds,0x63)) = 0;
    CF = (dd)*(raddr(ds,0x49)) < (dd)0;
    if (*(raddr(ds,0x49)) == 0) { return; }
    *(raddr(ds,0x49)) = 1;
locret_106c2:
    return;
}
void tnd_edummylabel21(void) {
    *(dw*)(raddr(ds,0x63)) = 0;
    bx = 0x74;
    tnd_sub_107b5(); return;
}
void tnd_edummylabel22(void) {
    bx = 0x72;
    tnd_sub_107b5(); return;
}
void tnd_edummylabel23(void) {
    *(raddr(ds,0x66)) = 0x1E;
    *(raddr(ds,0x65)) = 0x0FF;
    return;
}
void tnd_edummylabel24(void) {
    *(raddr(ds,0x65)) = 0;
locret_106e7:
    return;
}
void tnd_edummylabel25(void) {
    al = 0x4B;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x153;
loc_106f4:
    tnd_sub_107b2();
    bx = 0x143;
    tnd_sub_107b5(); return;
locret_106e7:
    return;
}
void tnd_edummylabel26(void) {
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x8D;
    tnd_sub_107b2(); return;
locret_106e7:
    return;
}
void tnd_edummylabel27(void) {
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x11F;
    tnd_sub_107b2(); return;
locret_106e7:
    return;
}
void tnd_edummylabel28(void) {
    al = 0x41;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x15B;
    goto loc_106f4;
loc_106f4:
    tnd_sub_107b2();
    bx = 0x143;
    tnd_sub_107b5(); return;
}
void tnd_edummylabel29(void) {
    bx = 0x16A;
    goto loc_10735;
loc_10735:
    tnd_sub_107b5();
    bx = 0x163;
    tnd_sub_107b5(); return;
}
void tnd_edummylabel3(void) {
    push(cs);
    ds = pop();
    tnd_sub_104a4();
    return;
}
void tnd_edummylabel30(void) {
    bx = 0x174;
loc_10735:
    tnd_sub_107b5();
    bx = 0x163;
    tnd_sub_107b5(); return;
}
void tnd_edummylabel31(void) {
    bx = 0x16C;
    goto loc_1074a;
loc_1074a:
    al = 0x28;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    tnd_sub_107b2();
    bx = 0x165;
    tnd_sub_107b5(); return;
locret_107ba:
    return;
}
void tnd_edummylabel32(void) {
    bx = 0x176;
loc_1074a:
    al = 0x28;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    tnd_sub_107b2();
    bx = 0x165;
    tnd_sub_107b5(); return;
locret_107ba:
    return;
}
void tnd_edummylabel33(void) {
    al = 0x3C;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x0A6;
    tnd_sub_107b2(); return;
locret_107ba:
    return;
}
void tnd_edummylabel34(void) {
    al = 0x63;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x187;
    tnd_sub_107b2();
    bx = 0x17E;
    tnd_sub_107b5(); return;
locret_107ba:
    return;
}
void tnd_edummylabel35(void) {
    al = 0x46;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x0F8;
    goto loc_1079c;
loc_1079c:
    tnd_sub_107b2();
    bx = 0x133;
    tnd_sub_107b5(); return;
locret_107ba:
    return;
}
void tnd_edummylabel36(void) {
    al = 0x5F;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x13B;
loc_1079c:
    tnd_sub_107b2();
    bx = 0x133;
    tnd_sub_107b5(); return;
locret_107ba:
    return;
}
void tnd_edummylabel37(void) {
    al = 0x2E;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x126;
sub_107b2:
    *(db*)raddr(ds,0x30) = al;
sub_107b5:
    IF = 0;
    tnd_sub_105b0();
    IF = 1;
locret_107ba:
    return;
}
void tnd_edummylabel38(void) {
    al = 0x2D;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x12E;
    tnd_sub_107b2(); return;
locret_107ba:
    return;
}
void tnd_edummylabel39(void) {
    al = 0x55;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x190;
    tnd_sub_107b2(); return;
locret_107ba:
    return;
}
void tnd_edummylabel4(void) {
    push(cs);
    ds = pop();
    CF = (dd)byte_100d7 < (dd)0x0FF;
    if (byte_100d7 == 0x0FF) { return; }
    { vfn f_ = func_at(tnd_cbase + (*(dw*)(((db*)&off_10380)+bx))); if (f_) f_(); else fprintf(stderr, "unresolved ind call %x\n", (dd)(tnd_cbase + (*(dw*)(((db*)&off_10380)+bx)))); }
locret_1008c:
    return;
}
void tnd_edummylabel40(void) {
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x117;
    tnd_sub_107b2(); return;
locret_107ba:
    return;
}
void tnd_edummylabel41(void) {
    al = 0x50;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x14B;
    tnd_sub_107b2();
    bx = 0x153;
    tnd_sub_107b5(); return;
locret_107ba:
    return;
}
void tnd_edummylabel42(void) {
    bx = 0x0AE;
    goto loc_10805;
loc_10805:
    tnd_sub_107b5();
    bx = 0x0DA;
    tnd_sub_107b5(); return;
}
void tnd_edummylabel43(void) {
    bx = 0x0C4;
loc_10805:
    tnd_sub_107b5();
    bx = 0x0DA;
    tnd_sub_107b5(); return;
}
void tnd_edummylabel44(void) {
    bx = 0x0E1;
    tnd_sub_107b5(); return;
}
void tnd_edummylabel45(void) {
    bx = 0x1D0;
    tnd_sub_107b5(); return;
}
void tnd_edummylabel46(void) {
    bx = 0x1CB;
    tnd_sub_107b5(); return;
}
void tnd_edummylabel47(void) {
    bx = 0x6A;
    tnd_sub_107b5(); return;
}
void tnd_edummylabel48(void) {
    bx = 0x110;
    tnd_sub_107b5();
    bx = 0x84;
    tnd_sub_107b5(); return;
}
void tnd_edummylabel49(void) {
    bx = 0x1EF;
    tnd_sub_107b5();
    *(dw*)(raddr(ds,0x38)) = 0x1DF;
    bx = 0x23D;
    tnd_sub_107b5();
    bx = 0x235;
    *(dw*)(raddr(ds,0x45)) = bx;
sub_10851:
    bx = 0x28B;
    tnd_sub_107b5();
    *(dw*)(raddr(ds,0x52)) = 0x26B;
    bx = 0x2FD;
    tnd_sub_107b5(); return;
}
void tnd_edummylabel5(void) {
    push(cs);
    ds = pop();
    byte_100d7 = 0;
    return;
}
void tnd_edummylabel50(void) {
    tnd_sub_10833();
    *(dw*)(raddr(ds,0x38)) = 0x210;
    *(dw*)(raddr(ds,0x45)) = 0x246;
    bx = 0x2AC;
    *(dw*)(raddr(ds,0x52)) = bx;
    bl = 0x18;
    *(db*)raddr(ds,0x5E) = bl;
    return;
}
void tnd_edummylabel51(void) {
    CF = (dd)*(raddr(ds,0x67)) < (dd)0x0FF;
    if (*(raddr(ds,0x67)) == 0x0FF) { return; }
    ax = bx;
    CF = (dd)ah < (dd)0x0BF;
    if (ah <= 0x0BF) {
        { if (1) { CF = (ah >> ((1)-1)) & 1; ah = ah >> 1; } }
        ah |= 0x90; CF = 0;
        ah &= 0x0F0; CF = 0;
        al &= 0x0F; CF = 0;
        al |= ah; CF = 0;
        out(0x0C0, al);
        ah &= 0x0E0; CF = 0;
        al = bl;
        { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
        { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
        { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
        { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
        al |= ah; CF = 0;
        out(0x0C0, al);
        al = bh;
        al &= 0x3F; CF = 0;
        out(0x0C0, al);
locret_108bc:
        return;
    }
loc_108bd:
    al = bh;
    al |= 0x0E0; CF = 0;
    out(0x0C0, al);
    al = bl;
    al |= 0x0F0; CF = 0;
    out(0x0C0, al);
    return;
}
void tnd_edummylabel6(void) {
    push(cs);
    ds = pop();
    byte_100d7 = 0x0FF;
    return;
}
void tnd_edummylabel7(void) {
    CF = (dd)bx < (dd)0x52;
    if (bx > 0x52) { return; }
    { vfn f_ = func_at(tnd_cbase + (*(dw*)(((db*)&off_10380)+bx))); if (f_) f_(); else fprintf(stderr, "unresolved ind call %x\n", (dd)(tnd_cbase + (*(dw*)(((db*)&off_10380)+bx)))); }
locret_103dd:
    return;
}
void tnd_edummylabel8(void) {
    CF = (dd)byte_103e2 < (dd)0x0FF;
    if (byte_103e2 == 0x0FF) { return; }
    push(ax);
    push(ds);
    push(cs);
    ds = pop();
    push(di);
    push(cx);
    push(bx);
    tnd_sub_104c5();
    bx = pop();
    cx = pop();
    di = pop();
    (byte_100d9)--;
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
        CF = (dd)*(raddr(ds,0x68)) < (dd)0;
        if (*(raddr(ds,0x68)) != 0) { return; }
        *(raddr(ds,0x68)) = ~*(raddr(ds,0x68));
        push(ds);
        { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; }
        ds = ax;
        bx = *(dw*)raddr(ds,0x20);
        cx = *(dw*)raddr(ds,0x22);
        *(dw*)(&dword_103de) = bx;
        *(dw*)(((db*)&dword_103de)+2) = cx;
        ds = pop();
        IF = 0;
        *(raddr(ds,0x69)) = 3;
        push(ds);
        { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; }
        ds = ax;
        ax = 0x373;
        *(dw*)(raddr(ds,0x20)) = ax;
        *(dw*)(raddr(ds,0x22)) = cs;
        ds = pop();
        al = 0x36;
        out(0x43, al);
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
    { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; }
    ds = ax;
    *(dw*)(raddr(ds,0x20)) = bx;
    *(dw*)(raddr(ds,0x22)) = cx;
    ds = pop();
    al = 0x36;
    out(0x43, al);
    ax = 0; CF = 0;
    out(0x40, al);
    al = ah;
    out(0x40, al);
    IF = 1;
    return;
}
void tnd_edummylabel9(void) {
    *(raddr(ds,0x65)) = 0;
    *(dw*)(raddr(ds,0x63)) = 0;
    byte_103e2 = 0;
    IF = 0;
    CF = (dd)*(raddr(ds,0x68)) < (dd)0;
    if (*(raddr(ds,0x68)) != 0) { return; }
    *(raddr(ds,0x68)) = ~*(raddr(ds,0x68));
    push(ds);
    { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; }
    ds = ax;
    bx = *(dw*)raddr(ds,0x20);
    cx = *(dw*)raddr(ds,0x22);
    *(dw*)(&dword_103de) = bx;
    *(dw*)(((db*)&dword_103de)+2) = cx;
    ds = pop();
    IF = 0;
    *(raddr(ds,0x69)) = 3;
    push(ds);
    { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; }
    ds = ax;
    ax = 0x373;
    *(dw*)(raddr(ds,0x20)) = ax;
    *(dw*)(raddr(ds,0x22)) = cs;
    ds = pop();
    al = 0x36;
    out(0x43, al);
    ax = 0x4DAE;
    out(0x40, al);
    al = ah;
    out(0x40, al);
    IF = 1;
locret_1046a:
    return;
}
void tnd_loc_1046b(void) {
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
    { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; }
    ds = ax;
    *(dw*)(raddr(ds,0x20)) = bx;
    *(dw*)(raddr(ds,0x22)) = cx;
    ds = pop();
    al = 0x36;
    out(0x43, al);
    ax = 0; CF = 0;
    out(0x40, al);
    al = ah;
    out(0x40, al);
    IF = 1;
    return;
}
void tnd_loc_104bb(void) {
    ax = 0; CF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; }
loc_104bf:
    do {
        (bx)--;
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void tnd_loc_104bf(void) {
loc_104bf:
    do {
        (bx)--;
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void tnd_loc_104e1(void) {
    CF = (dd)*(raddr(ds,0x65)) < (dd)0;
    if (*(raddr(ds,0x65)) != 0) {
        CF = (dd)*(raddr(ds,0x66)) < (dd)0;
        if ((short)*(raddr(ds,0x66)) <= (short)0) {
            *(raddr(ds,0x66)) = 0x1E;
            CF = (dd)*(raddr(ds,0x2F)) < (dd)0;
            if (*(raddr(ds,0x2F)) == 0) {
                bx = 0x1B6;
                tnd_sub_105b0(); return;
            }
        }
loc_10502:
        (*(raddr(ds,0x66)))--;
    }
loc_10506:
    di = 0x2F;
    tnd_sub_1051f();
    di = 0x3C;
    tnd_sub_1051f();
    di = 0x49;
    tnd_sub_1051f();
    di = 0x56;
sub_1051f:
    CF = (dd)*(raddr(ds,di)) < (dd)0;
    if (*(raddr(ds,di)) == 0) { return; }
    (*(raddr(ds,di)))--;
    if (*(raddr(ds,di)) != 0) {
        CF = (dd)*(dw*)(raddr(ds,di+2)) < (dd)0;
        if (*(dw*)(raddr(ds,di+2)) == 0) { return; }
        al = *(db*)raddr(ds,di+7);
        al |= al; CF = 0;
        if (al != 0) {
            bl = al;
            al &= 0x0F0; CF = 0;
            if (al != 0) {
                (*(raddr(ds,di+4)))--;
                if (*(raddr(ds,di+4)) == 0) {
                    al = *(db*)raddr(ds,di+6);
                    al &= 0x0F0; CF = 0;
                    ax = (char)al;
                    cx = *(dw*)raddr(ds,di+2);
                    { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; }
                    ax &= 0x3FF0; CF = 0;
                    cx &= 0x0C00F; CF = 0;
                    ax |= cx; CF = 0;
                    *(dw*)(raddr(ds,di+2)) = ax;
                    al = bl;
                    al &= 0x0F0; CF = 0;
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
                    *(db*)raddr(ds,di+4) = al;
                }
            }
loc_10566:
            al = bl;
            al &= 0x0F; CF = 0;
            if (al != 0) {
                (*(raddr(ds,di+5)))--;
                if (*(raddr(ds,di+5)) == 0) {
                    al = *(db*)raddr(ds,di+6);
                    al &= 0x0F; CF = 0;
                    cx = *(dw*)raddr(ds,di+2);
                    { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; }
                    ax &= 0x0F; CF = 0;
                    cx &= 0x0FFF0; CF = 0;
                    ax |= cx; CF = 0;
                    *(dw*)(raddr(ds,di+2)) = ax;
                    bl &= 0x0F; CF = 0;
                    *(db*)raddr(ds,di+5) = bl;
                }
            }
        }
loc_1058c:
        bx = *(dw*)raddr(ds,di+2);
        tnd_sub_10889(); return;
    }
loc_10592:
    bx = *(dw*)raddr(ds,di+0x0B);
    al = *(db*)raddr(ds,bx);
    al |= al; CF = 0;
    if (al == 0) {
sub_1059b:
        bx = *(dw*)raddr(ds,di+2);
        bx |= bx; CF = 0;
        if (bx != 0) {
            bl |= 0x0F; CF = 0;
            tnd_sub_10889();
        }
loc_105a8:
        bx = di;
        cx = 0x0D;
        goto loc_104bb;
    }
loc_105d2:
    al = *(db*)raddr(ds,bx);
    (bx)++;
    ax &= 0x7F; CF = 0;
    CF = 0;
    if ((al & 0x88) != 0) {
        push(bx);
        push(ax);
        ax = *(dw*)raddr(ds,di+9);
        push(ax);
        al = *(db*)raddr(ds,di+8);
        push(ax);
        tnd_sub_1059b();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto loc_10630;
loc_105f6:
        CF = 0;
        if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
        cx = bx;
        (cx)++;
loc_105fd:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
loc_10605:
    cx = *(dw*)raddr(ds,bx);
    (bx)++;
    (bx)++;
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0;
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0;
        goto loc_1062d;
    }
loc_10615:
    CF = 0;
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        *(db*)raddr(ds,di+4) = cl;
    }
loc_1061f:
    CF = 0;
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        *(db*)raddr(ds,di+5) = cl;
    }
loc_10629:
    cx = *(dw*)raddr(ds,bx);
    (bx)++;
    (bx)++;
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0;
    if ((al & 0x0C0) == 0) goto loc_105f6;
    cl = *(db*)raddr(ds,bx);
    (bx)++;
    CF = (dd)*(raddr(ds,di+8)) < (dd)0;
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--;
        if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
        cx = bx;
        (cx)++;
        *(dw*)(raddr(ds,di+9)) = cx;
        goto loc_105fa;
    }
loc_1064a:
    *(db*)raddr(ds,di+8) = cl;
loc_1064d:
    cx = *(dw*)raddr(ds,di+9);
    goto loc_105fd;
loc_104bb:
    ax = 0; CF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; }
loc_104bf:
    do {
        (bx)--;
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void tnd_loc_10502(void) {
    (*(raddr(ds,0x66)))--;
loc_10506:
    di = 0x2F;
    tnd_sub_1051f();
    di = 0x3C;
    tnd_sub_1051f();
    di = 0x49;
    tnd_sub_1051f();
    di = 0x56;
sub_1051f:
    CF = (dd)*(raddr(ds,di)) < (dd)0;
    if (*(raddr(ds,di)) == 0) { return; }
    (*(raddr(ds,di)))--;
    if (*(raddr(ds,di)) != 0) {
        CF = (dd)*(dw*)(raddr(ds,di+2)) < (dd)0;
        if (*(dw*)(raddr(ds,di+2)) == 0) { return; }
        al = *(db*)raddr(ds,di+7);
        al |= al; CF = 0;
        if (al != 0) {
            bl = al;
            al &= 0x0F0; CF = 0;
            if (al != 0) {
                (*(raddr(ds,di+4)))--;
                if (*(raddr(ds,di+4)) == 0) {
                    al = *(db*)raddr(ds,di+6);
                    al &= 0x0F0; CF = 0;
                    ax = (char)al;
                    cx = *(dw*)raddr(ds,di+2);
                    { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; }
                    ax &= 0x3FF0; CF = 0;
                    cx &= 0x0C00F; CF = 0;
                    ax |= cx; CF = 0;
                    *(dw*)(raddr(ds,di+2)) = ax;
                    al = bl;
                    al &= 0x0F0; CF = 0;
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
                    *(db*)raddr(ds,di+4) = al;
                }
            }
loc_10566:
            al = bl;
            al &= 0x0F; CF = 0;
            if (al != 0) {
                (*(raddr(ds,di+5)))--;
                if (*(raddr(ds,di+5)) == 0) {
                    al = *(db*)raddr(ds,di+6);
                    al &= 0x0F; CF = 0;
                    cx = *(dw*)raddr(ds,di+2);
                    { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; }
                    ax &= 0x0F; CF = 0;
                    cx &= 0x0FFF0; CF = 0;
                    ax |= cx; CF = 0;
                    *(dw*)(raddr(ds,di+2)) = ax;
                    bl &= 0x0F; CF = 0;
                    *(db*)raddr(ds,di+5) = bl;
                }
            }
        }
loc_1058c:
        bx = *(dw*)raddr(ds,di+2);
        tnd_sub_10889(); return;
    }
loc_10592:
    bx = *(dw*)raddr(ds,di+0x0B);
    al = *(db*)raddr(ds,bx);
    al |= al; CF = 0;
    if (al == 0) {
sub_1059b:
        bx = *(dw*)raddr(ds,di+2);
        bx |= bx; CF = 0;
        if (bx != 0) {
            bl |= 0x0F; CF = 0;
            tnd_sub_10889();
        }
loc_105a8:
        bx = di;
        cx = 0x0D;
        goto loc_104bb;
    }
loc_105d2:
    al = *(db*)raddr(ds,bx);
    (bx)++;
    ax &= 0x7F; CF = 0;
    CF = 0;
    if ((al & 0x88) != 0) {
        push(bx);
        push(ax);
        ax = *(dw*)raddr(ds,di+9);
        push(ax);
        al = *(db*)raddr(ds,di+8);
        push(ax);
        tnd_sub_1059b();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto loc_10630;
loc_105f6:
        CF = 0;
        if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
        cx = bx;
        (cx)++;
loc_105fd:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
loc_10605:
    cx = *(dw*)raddr(ds,bx);
    (bx)++;
    (bx)++;
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0;
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0;
        goto loc_1062d;
    }
loc_10615:
    CF = 0;
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        *(db*)raddr(ds,di+4) = cl;
    }
loc_1061f:
    CF = 0;
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        *(db*)raddr(ds,di+5) = cl;
    }
loc_10629:
    cx = *(dw*)raddr(ds,bx);
    (bx)++;
    (bx)++;
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0;
    if ((al & 0x0C0) == 0) goto loc_105f6;
    cl = *(db*)raddr(ds,bx);
    (bx)++;
    CF = (dd)*(raddr(ds,di+8)) < (dd)0;
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--;
        if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
        cx = bx;
        (cx)++;
        *(dw*)(raddr(ds,di+9)) = cx;
        goto loc_105fa;
    }
loc_1064a:
    *(db*)raddr(ds,di+8) = cl;
loc_1064d:
    cx = *(dw*)raddr(ds,di+9);
    goto loc_105fd;
loc_104bb:
    ax = 0; CF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; }
loc_104bf:
    do {
        (bx)--;
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void tnd_loc_10506(void) {
    di = 0x2F;
    tnd_sub_1051f();
    di = 0x3C;
    tnd_sub_1051f();
    di = 0x49;
    tnd_sub_1051f();
    di = 0x56;
sub_1051f:
    CF = (dd)*(raddr(ds,di)) < (dd)0;
    if (*(raddr(ds,di)) == 0) { return; }
    (*(raddr(ds,di)))--;
    if (*(raddr(ds,di)) != 0) {
        CF = (dd)*(dw*)(raddr(ds,di+2)) < (dd)0;
        if (*(dw*)(raddr(ds,di+2)) == 0) { return; }
        al = *(db*)raddr(ds,di+7);
        al |= al; CF = 0;
        if (al != 0) {
            bl = al;
            al &= 0x0F0; CF = 0;
            if (al != 0) {
                (*(raddr(ds,di+4)))--;
                if (*(raddr(ds,di+4)) == 0) {
                    al = *(db*)raddr(ds,di+6);
                    al &= 0x0F0; CF = 0;
                    ax = (char)al;
                    cx = *(dw*)raddr(ds,di+2);
                    { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; }
                    ax &= 0x3FF0; CF = 0;
                    cx &= 0x0C00F; CF = 0;
                    ax |= cx; CF = 0;
                    *(dw*)(raddr(ds,di+2)) = ax;
                    al = bl;
                    al &= 0x0F0; CF = 0;
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
                    *(db*)raddr(ds,di+4) = al;
                }
            }
loc_10566:
            al = bl;
            al &= 0x0F; CF = 0;
            if (al != 0) {
                (*(raddr(ds,di+5)))--;
                if (*(raddr(ds,di+5)) == 0) {
                    al = *(db*)raddr(ds,di+6);
                    al &= 0x0F; CF = 0;
                    cx = *(dw*)raddr(ds,di+2);
                    { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; }
                    ax &= 0x0F; CF = 0;
                    cx &= 0x0FFF0; CF = 0;
                    ax |= cx; CF = 0;
                    *(dw*)(raddr(ds,di+2)) = ax;
                    bl &= 0x0F; CF = 0;
                    *(db*)raddr(ds,di+5) = bl;
                }
            }
        }
loc_1058c:
        bx = *(dw*)raddr(ds,di+2);
        tnd_sub_10889(); return;
    }
loc_10592:
    bx = *(dw*)raddr(ds,di+0x0B);
    al = *(db*)raddr(ds,bx);
    al |= al; CF = 0;
    if (al == 0) {
sub_1059b:
        bx = *(dw*)raddr(ds,di+2);
        bx |= bx; CF = 0;
        if (bx != 0) {
            bl |= 0x0F; CF = 0;
            tnd_sub_10889();
        }
loc_105a8:
        bx = di;
        cx = 0x0D;
        goto loc_104bb;
    }
loc_105d2:
    al = *(db*)raddr(ds,bx);
    (bx)++;
    ax &= 0x7F; CF = 0;
    CF = 0;
    if ((al & 0x88) != 0) {
        push(bx);
        push(ax);
        ax = *(dw*)raddr(ds,di+9);
        push(ax);
        al = *(db*)raddr(ds,di+8);
        push(ax);
        tnd_sub_1059b();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto loc_10630;
loc_105f6:
        CF = 0;
        if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
        cx = bx;
        (cx)++;
loc_105fd:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
loc_10605:
    cx = *(dw*)raddr(ds,bx);
    (bx)++;
    (bx)++;
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0;
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0;
        goto loc_1062d;
    }
loc_10615:
    CF = 0;
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        *(db*)raddr(ds,di+4) = cl;
    }
loc_1061f:
    CF = 0;
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        *(db*)raddr(ds,di+5) = cl;
    }
loc_10629:
    cx = *(dw*)raddr(ds,bx);
    (bx)++;
    (bx)++;
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0;
    if ((al & 0x0C0) == 0) goto loc_105f6;
    cl = *(db*)raddr(ds,bx);
    (bx)++;
    CF = (dd)*(raddr(ds,di+8)) < (dd)0;
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--;
        if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
        cx = bx;
        (cx)++;
        *(dw*)(raddr(ds,di+9)) = cx;
        goto loc_105fa;
    }
loc_1064a:
    *(db*)raddr(ds,di+8) = cl;
loc_1064d:
    cx = *(dw*)raddr(ds,di+9);
    goto loc_105fd;
loc_104bb:
    ax = 0; CF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; }
loc_104bf:
    do {
        (bx)--;
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void tnd_loc_10566(void) {
    al = bl;
    al &= 0x0F; CF = 0;
    if (al != 0) {
        (*(raddr(ds,di+5)))--;
        if (*(raddr(ds,di+5)) == 0) {
            al = *(db*)raddr(ds,di+6);
            al &= 0x0F; CF = 0;
            cx = *(dw*)raddr(ds,di+2);
            { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; }
            ax &= 0x0F; CF = 0;
            cx &= 0x0FFF0; CF = 0;
            ax |= cx; CF = 0;
            *(dw*)(raddr(ds,di+2)) = ax;
            bl &= 0x0F; CF = 0;
            *(db*)raddr(ds,di+5) = bl;
        }
    }
loc_1058c:
    bx = *(dw*)raddr(ds,di+2);
    tnd_sub_10889(); return;
}
void tnd_loc_1058c(void) {
    bx = *(dw*)raddr(ds,di+2);
    tnd_sub_10889(); return;
}
void tnd_loc_10592(void) {
    bx = *(dw*)raddr(ds,di+0x0B);
    al = *(db*)raddr(ds,bx);
    al |= al; CF = 0;
    if (al == 0) {
sub_1059b:
        bx = *(dw*)raddr(ds,di+2);
        bx |= bx; CF = 0;
        if (bx != 0) {
            bl |= 0x0F; CF = 0;
            tnd_sub_10889();
        }
loc_105a8:
        bx = di;
        cx = 0x0D;
        goto loc_104bb;
    }
loc_105d2:
    al = *(db*)raddr(ds,bx);
    (bx)++;
    ax &= 0x7F; CF = 0;
    CF = 0;
    if ((al & 0x88) != 0) {
        push(bx);
        push(ax);
        ax = *(dw*)raddr(ds,di+9);
        push(ax);
        al = *(db*)raddr(ds,di+8);
        push(ax);
        tnd_sub_1059b();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto loc_10630;
loc_105f6:
        CF = 0;
        if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
        cx = bx;
        (cx)++;
loc_105fd:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
loc_10605:
    cx = *(dw*)raddr(ds,bx);
    (bx)++;
    (bx)++;
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0;
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0;
        goto loc_1062d;
    }
loc_10615:
    CF = 0;
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        *(db*)raddr(ds,di+4) = cl;
    }
loc_1061f:
    CF = 0;
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        *(db*)raddr(ds,di+5) = cl;
    }
loc_10629:
    cx = *(dw*)raddr(ds,bx);
    (bx)++;
    (bx)++;
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0;
    if ((al & 0x0C0) == 0) goto loc_105f6;
    cl = *(db*)raddr(ds,bx);
    (bx)++;
    CF = (dd)*(raddr(ds,di+8)) < (dd)0;
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--;
        if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
        cx = bx;
        (cx)++;
        *(dw*)(raddr(ds,di+9)) = cx;
        goto loc_105fa;
    }
loc_1064a:
    *(db*)raddr(ds,di+8) = cl;
loc_1064d:
    cx = *(dw*)raddr(ds,di+9);
    goto loc_105fd;
loc_104bb:
    ax = 0; CF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; }
loc_104bf:
    do {
        (bx)--;
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void tnd_loc_105a8(void) {
    bx = di;
    cx = 0x0D;
    goto loc_104bb;
loc_104bb:
    ax = 0; CF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; }
loc_104bf:
    do {
        (bx)--;
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void tnd_loc_105d2(void) {
    al = *(db*)raddr(ds,bx);
    (bx)++;
    ax &= 0x7F; CF = 0;
    CF = 0;
    if ((al & 0x88) != 0) {
        push(bx);
        push(ax);
        ax = *(dw*)raddr(ds,di+9);
        push(ax);
        al = *(db*)raddr(ds,di+8);
        push(ax);
        tnd_sub_1059b();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto loc_10630;
loc_105f6:
        CF = 0;
        if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
        cx = bx;
        (cx)++;
loc_105fd:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
loc_10605:
    cx = *(dw*)raddr(ds,bx);
    (bx)++;
    (bx)++;
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0;
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0;
        goto loc_1062d;
    }
loc_10615:
    CF = 0;
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        *(db*)raddr(ds,di+4) = cl;
    }
loc_1061f:
    CF = 0;
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        *(db*)raddr(ds,di+5) = cl;
    }
loc_10629:
    cx = *(dw*)raddr(ds,bx);
    (bx)++;
    (bx)++;
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0;
    if ((al & 0x0C0) == 0) goto loc_105f6;
    cl = *(db*)raddr(ds,bx);
    (bx)++;
    CF = (dd)*(raddr(ds,di+8)) < (dd)0;
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--;
        if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
        cx = bx;
        (cx)++;
        *(dw*)(raddr(ds,di+9)) = cx;
        goto loc_105fa;
    }
loc_1064a:
    *(db*)raddr(ds,di+8) = cl;
loc_1064d:
    cx = *(dw*)raddr(ds,di+9);
    goto loc_105fd;
}
void tnd_loc_105f6(void) {
    CF = 0;
    if ((al & 0x84) == 0) {
loc_105fa:
        cx = bx;
        (cx)++;
loc_105fd:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
loc_10642:
    cx = bx;
    (cx)++;
    *(dw*)(raddr(ds,di+9)) = cx;
    goto loc_105fa;
}
void tnd_loc_105fa(void) {
    cx = bx;
    (cx)++;
loc_105fd:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void tnd_loc_105fd(void) {
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void tnd_loc_10605(void) {
    cx = *(dw*)raddr(ds,bx);
    (bx)++;
    (bx)++;
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0;
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0;
        goto loc_1062d;
    }
loc_10615:
    CF = 0;
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        *(db*)raddr(ds,di+4) = cl;
    }
loc_1061f:
    CF = 0;
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        *(db*)raddr(ds,di+5) = cl;
    }
loc_10629:
    cx = *(dw*)raddr(ds,bx);
    (bx)++;
    (bx)++;
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0;
    if ((al & 0x0C0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        CF = (dd)*(raddr(ds,di+8)) < (dd)0;
        if (*(raddr(ds,di+8)) != 0) {
            (*(raddr(ds,di+8)))--;
            if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
            cx = bx;
            (cx)++;
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
    CF = 0;
    if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
    cx = bx;
    (cx)++;
loc_105fd:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void tnd_loc_10615(void) {
    CF = 0;
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        *(db*)raddr(ds,di+4) = cl;
    }
loc_1061f:
    CF = 0;
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        *(db*)raddr(ds,di+5) = cl;
    }
loc_10629:
    cx = *(dw*)raddr(ds,bx);
    (bx)++;
    (bx)++;
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0;
    if ((al & 0x0C0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        CF = (dd)*(raddr(ds,di+8)) < (dd)0;
        if (*(raddr(ds,di+8)) != 0) {
            (*(raddr(ds,di+8)))--;
            if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
            cx = bx;
            (cx)++;
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
    CF = 0;
    if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
    cx = bx;
    (cx)++;
loc_105fd:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void tnd_loc_1061f(void) {
    CF = 0;
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        *(db*)raddr(ds,di+5) = cl;
    }
loc_10629:
    cx = *(dw*)raddr(ds,bx);
    (bx)++;
    (bx)++;
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0;
    if ((al & 0x0C0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        CF = (dd)*(raddr(ds,di+8)) < (dd)0;
        if (*(raddr(ds,di+8)) != 0) {
            (*(raddr(ds,di+8)))--;
            if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
            cx = bx;
            (cx)++;
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
    CF = 0;
    if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
    cx = bx;
    (cx)++;
loc_105fd:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void tnd_loc_10629(void) {
    cx = *(dw*)raddr(ds,bx);
    (bx)++;
    (bx)++;
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0;
    if ((al & 0x0C0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        CF = (dd)*(raddr(ds,di+8)) < (dd)0;
        if (*(raddr(ds,di+8)) != 0) {
            (*(raddr(ds,di+8)))--;
            if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
            cx = bx;
            (cx)++;
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
    CF = 0;
    if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
    cx = bx;
    (cx)++;
loc_105fd:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void tnd_loc_1062d(void) {
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0;
    if ((al & 0x0C0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        CF = (dd)*(raddr(ds,di+8)) < (dd)0;
        if (*(raddr(ds,di+8)) != 0) {
            (*(raddr(ds,di+8)))--;
            if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
            cx = bx;
            (cx)++;
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
    CF = 0;
    if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
    cx = bx;
    (cx)++;
loc_105fd:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void tnd_loc_10630(void) {
    CF = 0;
    if ((al & 0x0C0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        CF = (dd)*(raddr(ds,di+8)) < (dd)0;
        if (*(raddr(ds,di+8)) != 0) {
            (*(raddr(ds,di+8)))--;
            if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
            cx = bx;
            (cx)++;
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
    CF = 0;
    if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
    cx = bx;
    (cx)++;
loc_105fd:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void tnd_loc_10642(void) {
    cx = bx;
    (cx)++;
    *(dw*)(raddr(ds,di+9)) = cx;
    goto loc_105fa;
loc_105fa:
    cx = bx;
    (cx)++;
loc_105fd:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void tnd_loc_1064a(void) {
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
void tnd_loc_1064d(void) {
    cx = *(dw*)raddr(ds,di+9);
    goto loc_105fd;
loc_105fd:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void tnd_loc_10694(void) {
    IF = 0;
    ax = 0x3C;
    tnd_sub_105c4();
    IF = 1;
locret_1069d:
    return;
}
void tnd_loc_106a9(void) {
    *(dw*)(raddr(ds,0x63)) = bx;
    tnd_sub_107b5(); return;
}
void tnd_loc_106f4(void) {
    tnd_sub_107b2();
    bx = 0x143;
    tnd_sub_107b5(); return;
}
void tnd_loc_10735(void) {
    tnd_sub_107b5();
    bx = 0x163;
    tnd_sub_107b5(); return;
}
void tnd_loc_1074a(void) {
    al = 0x28;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    tnd_sub_107b2();
    bx = 0x165;
    tnd_sub_107b5(); return;
locret_107ba:
    return;
}
void tnd_loc_1079c(void) {
    tnd_sub_107b2();
    bx = 0x133;
    tnd_sub_107b5(); return;
}
void tnd_loc_10805(void) {
    tnd_sub_107b5();
    bx = 0x0DA;
    tnd_sub_107b5(); return;
}
void tnd_loc_108bd(void) {
    al = bh;
    al |= 0x0E0; CF = 0;
    out(0x0C0, al);
    al = bl;
    al |= 0x0F0; CF = 0;
    out(0x0C0, al);
    return;
}
void tnd_locret_1008c(void) {
    return;
}
void tnd_locret_103dd(void) {
    return;
}
void tnd_locret_1040a(void) {
    return;
sub_1040b:
    *(raddr(ds,0x67)) = 0;
edummylabel9:
    *(raddr(ds,0x65)) = 0;
    *(dw*)(raddr(ds,0x63)) = 0;
    byte_103e2 = 0;
    IF = 0;
    CF = (dd)*(raddr(ds,0x68)) < (dd)0;
    if (*(raddr(ds,0x68)) != 0) { return; }
    *(raddr(ds,0x68)) = ~*(raddr(ds,0x68));
    push(ds);
    { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; }
    ds = ax;
    bx = *(dw*)raddr(ds,0x20);
    cx = *(dw*)raddr(ds,0x22);
    *(dw*)(&dword_103de) = bx;
    *(dw*)(((db*)&dword_103de)+2) = cx;
    ds = pop();
    IF = 0;
    *(raddr(ds,0x69)) = 3;
    push(ds);
    { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; }
    ds = ax;
    ax = 0x373;
    *(dw*)(raddr(ds,0x20)) = ax;
    *(dw*)(raddr(ds,0x22)) = cs;
    ds = pop();
    al = 0x36;
    out(0x43, al);
    ax = 0x4DAE;
    out(0x40, al);
    al = ah;
    out(0x40, al);
    IF = 1;
locret_1046a:
    return;
}
void tnd_locret_1046a(void) {
    return;
}
void tnd_locret_104c4(void) {
    return;
}
void tnd_locret_10604(void) {
    return;
}
void tnd_locret_1069d(void) {
    return;
}
void tnd_locret_106c2(void) {
    return;
}
void tnd_locret_106e7(void) {
    return;
}
void tnd_locret_107ba(void) {
    return;
}
void tnd_locret_108bc(void) {
    return;
}
void tnd_seg001_33e_proc(void) {
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
    { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; }
    ds = ax;
    *(dw*)(raddr(ds,0x20)) = bx;
    *(dw*)(raddr(ds,0x22)) = cx;
    ds = pop();
    al = 0x36;
    out(0x43, al);
    ax = 0; CF = 0;
    out(0x40, al);
    al = ah;
    out(0x40, al);
    IF = 1;
    return;
}
void tnd_seg001_4_proc(void) {
    tnd_sub_1040b();
edummylabel1:
    return;
}
void tnd_sub_1040b(void) {
    *(raddr(ds,0x67)) = 0;
edummylabel9:
    *(raddr(ds,0x65)) = 0;
    *(dw*)(raddr(ds,0x63)) = 0;
    byte_103e2 = 0;
    IF = 0;
    CF = (dd)*(raddr(ds,0x68)) < (dd)0;
    if (*(raddr(ds,0x68)) != 0) { return; }
    *(raddr(ds,0x68)) = ~*(raddr(ds,0x68));
    push(ds);
    { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; }
    ds = ax;
    bx = *(dw*)raddr(ds,0x20);
    cx = *(dw*)raddr(ds,0x22);
    *(dw*)(&dword_103de) = bx;
    *(dw*)(((db*)&dword_103de)+2) = cx;
    ds = pop();
    IF = 0;
    *(raddr(ds,0x69)) = 3;
    push(ds);
    { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; }
    ds = ax;
    ax = 0x373;
    *(dw*)(raddr(ds,0x20)) = ax;
    *(dw*)(raddr(ds,0x22)) = cs;
    ds = pop();
    al = 0x36;
    out(0x43, al);
    ax = 0x4DAE;
    out(0x40, al);
    al = ah;
    out(0x40, al);
    IF = 1;
locret_1046a:
    return;
}
void tnd_sub_104a4(void) {
    al = 0x9F;
edummylabel11:
    out(0x0C0, al);
    out(0x0C0, 0x0BF);
    out(0x0C0, 0x0DF);
    al = 0x0FF;
    out(0x0C0, al);
    cx = 0x38;
    bx = 0x2f;
loc_104bb:
    ax = 0; CF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; }
loc_104bf:
    do {
        (bx)--;
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void tnd_sub_104c5(void) {
    CF = (dd)*(raddr(ds,0x69)) < (dd)1;
edummylabel12:
    if (!ZF) {
        CF = (dd)*(dw*)(raddr(ds,0x63)) < (dd)0;
        if (*(dw*)(raddr(ds,0x63)) != 0) {
            CF = (dd)*(raddr(ds,0x49)) < (dd)0;
            if (*(raddr(ds,0x49)) == 0) {
                bx = *(dw*)raddr(ds,0x63);
                tnd_sub_105b0(); return;
            }
        }
loc_104e1:
        CF = (dd)*(raddr(ds,0x65)) < (dd)0;
        if (*(raddr(ds,0x65)) != 0) {
            CF = (dd)*(raddr(ds,0x66)) < (dd)0;
            if ((short)*(raddr(ds,0x66)) <= (short)0) {
                *(raddr(ds,0x66)) = 0x1E;
                CF = (dd)*(raddr(ds,0x2F)) < (dd)0;
                if (*(raddr(ds,0x2F)) == 0) {
                    bx = 0x1B6;
                    tnd_sub_105b0(); return;
                }
            }
loc_10502:
            (*(raddr(ds,0x66)))--;
        }
    }
loc_10506:
    di = 0x2F;
    tnd_sub_1051f();
    di = 0x3C;
    tnd_sub_1051f();
    di = 0x49;
    tnd_sub_1051f();
    di = 0x56;
sub_1051f:
    CF = (dd)*(raddr(ds,di)) < (dd)0;
    if (*(raddr(ds,di)) == 0) { return; }
    (*(raddr(ds,di)))--;
    if (*(raddr(ds,di)) != 0) {
        CF = (dd)*(dw*)(raddr(ds,di+2)) < (dd)0;
        if (*(dw*)(raddr(ds,di+2)) == 0) { return; }
        al = *(db*)raddr(ds,di+7);
        al |= al; CF = 0;
        if (al != 0) {
            bl = al;
            al &= 0x0F0; CF = 0;
            if (al != 0) {
                (*(raddr(ds,di+4)))--;
                if (*(raddr(ds,di+4)) == 0) {
                    al = *(db*)raddr(ds,di+6);
                    al &= 0x0F0; CF = 0;
                    ax = (char)al;
                    cx = *(dw*)raddr(ds,di+2);
                    { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; }
                    ax &= 0x3FF0; CF = 0;
                    cx &= 0x0C00F; CF = 0;
                    ax |= cx; CF = 0;
                    *(dw*)(raddr(ds,di+2)) = ax;
                    al = bl;
                    al &= 0x0F0; CF = 0;
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
                    *(db*)raddr(ds,di+4) = al;
                }
            }
loc_10566:
            al = bl;
            al &= 0x0F; CF = 0;
            if (al != 0) {
                (*(raddr(ds,di+5)))--;
                if (*(raddr(ds,di+5)) == 0) {
                    al = *(db*)raddr(ds,di+6);
                    al &= 0x0F; CF = 0;
                    cx = *(dw*)raddr(ds,di+2);
                    { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; }
                    ax &= 0x0F; CF = 0;
                    cx &= 0x0FFF0; CF = 0;
                    ax |= cx; CF = 0;
                    *(dw*)(raddr(ds,di+2)) = ax;
                    bl &= 0x0F; CF = 0;
                    *(db*)raddr(ds,di+5) = bl;
                }
            }
        }
loc_1058c:
        bx = *(dw*)raddr(ds,di+2);
        tnd_sub_10889(); return;
    }
loc_10592:
    bx = *(dw*)raddr(ds,di+0x0B);
    al = *(db*)raddr(ds,bx);
    al |= al; CF = 0;
    if (al == 0) {
sub_1059b:
        bx = *(dw*)raddr(ds,di+2);
        bx |= bx; CF = 0;
        if (bx != 0) {
            bl |= 0x0F; CF = 0;
            tnd_sub_10889();
        }
loc_105a8:
        bx = di;
        cx = 0x0D;
        goto loc_104bb;
    }
loc_105d2:
    al = *(db*)raddr(ds,bx);
    (bx)++;
    ax &= 0x7F; CF = 0;
    CF = 0;
    if ((al & 0x88) != 0) {
        push(bx);
        push(ax);
        ax = *(dw*)raddr(ds,di+9);
        push(ax);
        al = *(db*)raddr(ds,di+8);
        push(ax);
        tnd_sub_1059b();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto loc_10630;
loc_105f6:
        CF = 0;
        if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
        cx = bx;
        (cx)++;
loc_105fd:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
loc_10605:
    cx = *(dw*)raddr(ds,bx);
    (bx)++;
    (bx)++;
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0;
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0;
        goto loc_1062d;
    }
loc_10615:
    CF = 0;
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        *(db*)raddr(ds,di+4) = cl;
    }
loc_1061f:
    CF = 0;
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        *(db*)raddr(ds,di+5) = cl;
    }
loc_10629:
    cx = *(dw*)raddr(ds,bx);
    (bx)++;
    (bx)++;
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0;
    if ((al & 0x0C0) == 0) goto loc_105f6;
    cl = *(db*)raddr(ds,bx);
    (bx)++;
    CF = (dd)*(raddr(ds,di+8)) < (dd)0;
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--;
        if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
        cx = bx;
        (cx)++;
        *(dw*)(raddr(ds,di+9)) = cx;
        goto loc_105fa;
    }
loc_1064a:
    *(db*)raddr(ds,di+8) = cl;
loc_1064d:
    cx = *(dw*)raddr(ds,di+9);
    goto loc_105fd;
loc_104bb:
    ax = 0; CF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; }
loc_104bf:
    do {
        (bx)--;
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void tnd_sub_1051f(void) {
    CF = (dd)*(raddr(ds,di)) < (dd)0;
    if (*(raddr(ds,di)) == 0) { return; }
    (*(raddr(ds,di)))--;
    if (*(raddr(ds,di)) != 0) {
        CF = (dd)*(dw*)(raddr(ds,di+2)) < (dd)0;
        if (*(dw*)(raddr(ds,di+2)) == 0) { return; }
        al = *(db*)raddr(ds,di+7);
        al |= al; CF = 0;
        if (al != 0) {
            bl = al;
            al &= 0x0F0; CF = 0;
            if (al != 0) {
                (*(raddr(ds,di+4)))--;
                if (*(raddr(ds,di+4)) == 0) {
                    al = *(db*)raddr(ds,di+6);
                    al &= 0x0F0; CF = 0;
                    ax = (char)al;
                    cx = *(dw*)raddr(ds,di+2);
                    { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; }
                    ax &= 0x3FF0; CF = 0;
                    cx &= 0x0C00F; CF = 0;
                    ax |= cx; CF = 0;
                    *(dw*)(raddr(ds,di+2)) = ax;
                    al = bl;
                    al &= 0x0F0; CF = 0;
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
                    { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
                    *(db*)raddr(ds,di+4) = al;
                }
            }
loc_10566:
            al = bl;
            al &= 0x0F; CF = 0;
            if (al != 0) {
                (*(raddr(ds,di+5)))--;
                if (*(raddr(ds,di+5)) == 0) {
                    al = *(db*)raddr(ds,di+6);
                    al &= 0x0F; CF = 0;
                    cx = *(dw*)raddr(ds,di+2);
                    { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; }
                    ax &= 0x0F; CF = 0;
                    cx &= 0x0FFF0; CF = 0;
                    ax |= cx; CF = 0;
                    *(dw*)(raddr(ds,di+2)) = ax;
                    bl &= 0x0F; CF = 0;
                    *(db*)raddr(ds,di+5) = bl;
                }
            }
        }
loc_1058c:
        bx = *(dw*)raddr(ds,di+2);
        tnd_sub_10889(); return;
    }
loc_10592:
    bx = *(dw*)raddr(ds,di+0x0B);
    al = *(db*)raddr(ds,bx);
    al |= al; CF = 0;
    if (al == 0) {
sub_1059b:
        bx = *(dw*)raddr(ds,di+2);
        bx |= bx; CF = 0;
        if (bx != 0) {
            bl |= 0x0F; CF = 0;
            tnd_sub_10889();
        }
loc_105a8:
        bx = di;
        cx = 0x0D;
        goto loc_104bb;
    }
loc_105d2:
    al = *(db*)raddr(ds,bx);
    (bx)++;
    ax &= 0x7F; CF = 0;
    CF = 0;
    if ((al & 0x88) != 0) {
        push(bx);
        push(ax);
        ax = *(dw*)raddr(ds,di+9);
        push(ax);
        al = *(db*)raddr(ds,di+8);
        push(ax);
        tnd_sub_1059b();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto loc_10630;
loc_105f6:
        CF = 0;
        if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
        cx = bx;
        (cx)++;
loc_105fd:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
loc_10605:
    cx = *(dw*)raddr(ds,bx);
    (bx)++;
    (bx)++;
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0;
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0;
        goto loc_1062d;
    }
loc_10615:
    CF = 0;
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        *(db*)raddr(ds,di+4) = cl;
    }
loc_1061f:
    CF = 0;
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        *(db*)raddr(ds,di+5) = cl;
    }
loc_10629:
    cx = *(dw*)raddr(ds,bx);
    (bx)++;
    (bx)++;
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0;
    if ((al & 0x0C0) == 0) goto loc_105f6;
    cl = *(db*)raddr(ds,bx);
    (bx)++;
    CF = (dd)*(raddr(ds,di+8)) < (dd)0;
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--;
        if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
        cx = bx;
        (cx)++;
        *(dw*)(raddr(ds,di+9)) = cx;
        goto loc_105fa;
    }
loc_1064a:
    *(db*)raddr(ds,di+8) = cl;
loc_1064d:
    cx = *(dw*)raddr(ds,di+9);
    goto loc_105fd;
loc_104bb:
    ax = 0; CF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; }
loc_104bf:
    do {
        (bx)--;
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void tnd_sub_1059b(void) {
    bx = *(dw*)raddr(ds,di+2);
    bx |= bx; CF = 0;
    if (bx != 0) {
        bl |= 0x0F; CF = 0;
        tnd_sub_10889();
    }
loc_105a8:
    bx = di;
    cx = 0x0D;
    goto loc_104bb;
loc_104bb:
    ax = 0; CF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; }
loc_104bf:
    do {
        (bx)--;
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void tnd_sub_105b0(void) {
edummylabel13:
    al = *(db*)raddr(ds,bx);
    al |= al; CF = 0;
    if (al == 0) { return; }
    al &= 3; CF = 0;
    ax = (char)al;
    {dd r = (dd)ax * 0x0D; ax = r; dx = r >> 16;}
    cx = 0x2F;
    { dd t_ = (dd)ax + (dd)cx; CF = t_ > 0xFFFF; ax = t_; }
sub_105c4:
    di = ax;
    push(bx);
    push(*(dw*)(raddr(ds,di)));
    tnd_sub_1059b();
    *(dw*)(raddr(ds,di)) = pop();
    bx = pop();
    *(dw*)(raddr(ds,di+9)) = bx;
loc_105d2:
    al = *(db*)raddr(ds,bx);
    (bx)++;
    ax &= 0x7F; CF = 0;
    CF = 0;
    if ((al & 0x88) != 0) {
        push(bx);
        push(ax);
        ax = *(dw*)raddr(ds,di+9);
        push(ax);
        al = *(db*)raddr(ds,di+8);
        push(ax);
        tnd_sub_1059b();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto loc_10630;
loc_105f6:
        CF = 0;
        if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
        cx = bx;
        (cx)++;
loc_105fd:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
loc_10605:
    cx = *(dw*)raddr(ds,bx);
    (bx)++;
    (bx)++;
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0;
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0;
        goto loc_1062d;
    }
loc_10615:
    CF = 0;
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        *(db*)raddr(ds,di+4) = cl;
    }
loc_1061f:
    CF = 0;
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        *(db*)raddr(ds,di+5) = cl;
    }
loc_10629:
    cx = *(dw*)raddr(ds,bx);
    (bx)++;
    (bx)++;
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0;
    if ((al & 0x0C0) == 0) goto loc_105f6;
    cl = *(db*)raddr(ds,bx);
    (bx)++;
    CF = (dd)*(raddr(ds,di+8)) < (dd)0;
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--;
        if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
        cx = bx;
        (cx)++;
        *(dw*)(raddr(ds,di+9)) = cx;
        goto loc_105fa;
    }
loc_1064a:
    *(db*)raddr(ds,di+8) = cl;
loc_1064d:
    cx = *(dw*)raddr(ds,di+9);
    goto loc_105fd;
}
void tnd_sub_105c4(void) {
    di = ax;
    push(bx);
    push(*(dw*)(raddr(ds,di)));
    tnd_sub_1059b();
    *(dw*)(raddr(ds,di)) = pop();
    bx = pop();
    *(dw*)(raddr(ds,di+9)) = bx;
loc_105d2:
    al = *(db*)raddr(ds,bx);
    (bx)++;
    ax &= 0x7F; CF = 0;
    CF = 0;
    if ((al & 0x88) != 0) {
        push(bx);
        push(ax);
        ax = *(dw*)raddr(ds,di+9);
        push(ax);
        al = *(db*)raddr(ds,di+8);
        push(ax);
        tnd_sub_1059b();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto loc_10630;
loc_105f6:
        CF = 0;
        if ((al & 0x84) != 0) goto loc_10642;
loc_105fa:
        cx = bx;
        (cx)++;
loc_105fd:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
loc_10605:
    cx = *(dw*)raddr(ds,bx);
    (bx)++;
    (bx)++;
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0;
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0;
        goto loc_1062d;
    }
loc_10615:
    CF = 0;
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        *(db*)raddr(ds,di+4) = cl;
    }
loc_1061f:
    CF = 0;
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++;
        *(db*)raddr(ds,di+5) = cl;
    }
loc_10629:
    cx = *(dw*)raddr(ds,bx);
    (bx)++;
    (bx)++;
loc_1062d:
    *(dw*)(raddr(ds,di+6)) = cx;
loc_10630:
    CF = 0;
    if ((al & 0x0C0) == 0) goto loc_105f6;
    cl = *(db*)raddr(ds,bx);
    (bx)++;
    CF = (dd)*(raddr(ds,di+8)) < (dd)0;
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--;
        if (*(raddr(ds,di+8)) != 0) goto loc_1064d;
loc_10642:
        cx = bx;
        (cx)++;
        *(dw*)(raddr(ds,di+9)) = cx;
        goto loc_105fa;
    }
loc_1064a:
    *(db*)raddr(ds,di+8) = cl;
loc_1064d:
    cx = *(dw*)raddr(ds,di+9);
    goto loc_105fd;
}
void tnd_sub_10652(void) {
edummylabel14:
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x0F0;
    tnd_sub_107b2(); return;
locret_1069d:
    return;
}
void tnd_sub_10661(void) {
edummylabel15:
    al = 0x1E;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x108;
    tnd_sub_107b2();
    bx = 0x100;
    tnd_sub_107b5(); return;
locret_1069d:
    return;
}
void tnd_sub_10677(void) {
edummylabel16:
    bx = 0x9D;
    tnd_sub_107b5(); return;
}
void tnd_sub_1067e(void) {
edummylabel17:
    al = 0x64;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    bx = 0x198;
    if (al >= *(db*)raddr(ds,0x30)) {
        tnd_sub_107b2();
        bx = 0x1A7;
        tnd_sub_107b5(); return;
    }
loc_10694:
    IF = 0;
    ax = 0x3C;
    tnd_sub_105c4();
    IF = 1;
locret_1069d:
    return;
}
void tnd_sub_1069e(void) {
edummylabel18:
    bx = 0x1BB;
    goto loc_106a9;
loc_106a9:
    *(dw*)(raddr(ds,0x63)) = bx;
    tnd_sub_107b5(); return;
}
void tnd_sub_106a5(void) {
edummylabel19:
    bx = 0x1C3;
loc_106a9:
    *(dw*)(raddr(ds,0x63)) = bx;
    tnd_sub_107b5(); return;
}
void tnd_sub_106b0(void) {
edummylabel20:
    *(dw*)(raddr(ds,0x63)) = 0;
    CF = (dd)*(raddr(ds,0x49)) < (dd)0;
    if (*(raddr(ds,0x49)) == 0) { return; }
    *(raddr(ds,0x49)) = 1;
locret_106c2:
    return;
}
void tnd_sub_106c3(void) {
edummylabel21:
    *(dw*)(raddr(ds,0x63)) = 0;
    bx = 0x74;
    tnd_sub_107b5(); return;
}
void tnd_sub_106d0(void) {
edummylabel22:
    bx = 0x72;
    tnd_sub_107b5(); return;
}
void tnd_sub_106d7(void) {
edummylabel23:
    *(raddr(ds,0x66)) = 0x1E;
    *(raddr(ds,0x65)) = 0x0FF;
    return;
}
void tnd_sub_106e2(void) {
edummylabel24:
    *(raddr(ds,0x65)) = 0;
locret_106e7:
    return;
}
void tnd_sub_106e8(void) {
edummylabel25:
    al = 0x4B;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x153;
loc_106f4:
    tnd_sub_107b2();
    bx = 0x143;
    tnd_sub_107b5(); return;
locret_106e7:
    return;
}
void tnd_sub_106fe(void) {
edummylabel26:
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x8D;
    tnd_sub_107b2(); return;
locret_106e7:
    return;
}
void tnd_sub_1070d(void) {
edummylabel27:
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x11F;
    tnd_sub_107b2(); return;
locret_106e7:
    return;
}
void tnd_sub_1071c(void) {
edummylabel28:
    al = 0x41;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x15B;
    goto loc_106f4;
loc_106f4:
    tnd_sub_107b2();
    bx = 0x143;
    tnd_sub_107b5(); return;
}
void tnd_sub_1072a(void) {
edummylabel29:
    bx = 0x16A;
    goto loc_10735;
loc_10735:
    tnd_sub_107b5();
    bx = 0x163;
    tnd_sub_107b5(); return;
}
void tnd_sub_10731(void) {
edummylabel30:
    bx = 0x174;
loc_10735:
    tnd_sub_107b5();
    bx = 0x163;
    tnd_sub_107b5(); return;
}
void tnd_sub_1073f(void) {
edummylabel31:
    bx = 0x16C;
    goto loc_1074a;
loc_1074a:
    al = 0x28;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    tnd_sub_107b2();
    bx = 0x165;
    tnd_sub_107b5(); return;
locret_107ba:
    return;
}
void tnd_sub_10746(void) {
edummylabel32:
    bx = 0x176;
loc_1074a:
    al = 0x28;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    tnd_sub_107b2();
    bx = 0x165;
    tnd_sub_107b5(); return;
locret_107ba:
    return;
}
void tnd_sub_1075c(void) {
edummylabel33:
    al = 0x3C;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x0A6;
    tnd_sub_107b2(); return;
locret_107ba:
    return;
}
void tnd_sub_1076b(void) {
edummylabel34:
    al = 0x63;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x187;
    tnd_sub_107b2();
    bx = 0x17E;
    tnd_sub_107b5(); return;
locret_107ba:
    return;
}
void tnd_sub_10781(void) {
edummylabel35:
    al = 0x46;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x0F8;
    goto loc_1079c;
loc_1079c:
    tnd_sub_107b2();
    bx = 0x133;
    tnd_sub_107b5(); return;
locret_107ba:
    return;
}
void tnd_sub_10790(void) {
edummylabel36:
    al = 0x5F;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x13B;
loc_1079c:
    tnd_sub_107b2();
    bx = 0x133;
    tnd_sub_107b5(); return;
locret_107ba:
    return;
}
void tnd_sub_107a6(void) {
edummylabel37:
    al = 0x2E;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x126;
sub_107b2:
    *(db*)raddr(ds,0x30) = al;
sub_107b5:
    IF = 0;
    tnd_sub_105b0();
    IF = 1;
locret_107ba:
    return;
}
void tnd_sub_107b2(void) {
    *(db*)raddr(ds,0x30) = al;
sub_107b5:
    IF = 0;
    tnd_sub_105b0();
    IF = 1;
locret_107ba:
    return;
}
void tnd_sub_107b5(void) {
    IF = 0;
    tnd_sub_105b0();
    IF = 1;
locret_107ba:
    return;
}
void tnd_sub_107bb(void) {
edummylabel38:
    al = 0x2D;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x12E;
    tnd_sub_107b2(); return;
locret_107ba:
    return;
}
void tnd_sub_107c9(void) {
edummylabel39:
    al = 0x55;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x190;
    tnd_sub_107b2(); return;
locret_107ba:
    return;
}
void tnd_sub_107d7(void) {
edummylabel40:
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x117;
    tnd_sub_107b2(); return;
locret_107ba:
    return;
}
void tnd_sub_107e5(void) {
edummylabel41:
    al = 0x50;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x14B;
    tnd_sub_107b2();
    bx = 0x153;
    tnd_sub_107b5(); return;
locret_107ba:
    return;
}
void tnd_sub_107fa(void) {
edummylabel42:
    bx = 0x0AE;
    goto loc_10805;
loc_10805:
    tnd_sub_107b5();
    bx = 0x0DA;
    tnd_sub_107b5(); return;
}
void tnd_sub_10801(void) {
edummylabel43:
    bx = 0x0C4;
loc_10805:
    tnd_sub_107b5();
    bx = 0x0DA;
    tnd_sub_107b5(); return;
}
void tnd_sub_1080e(void) {
edummylabel44:
    bx = 0x0E1;
    tnd_sub_107b5(); return;
}
void tnd_sub_10814(void) {
edummylabel45:
    bx = 0x1D0;
    tnd_sub_107b5(); return;
}
void tnd_sub_1081a(void) {
edummylabel46:
    bx = 0x1CB;
    tnd_sub_107b5(); return;
}
void tnd_sub_10820(void) {
edummylabel47:
    bx = 0x6A;
    tnd_sub_107b5(); return;
}
void tnd_sub_10826(void) {
edummylabel48:
    bx = 0x110;
    tnd_sub_107b5();
    bx = 0x84;
    tnd_sub_107b5(); return;
}
void tnd_sub_10833(void) {
edummylabel49:
    bx = 0x1EF;
    tnd_sub_107b5();
    *(dw*)(raddr(ds,0x38)) = 0x1DF;
    bx = 0x23D;
    tnd_sub_107b5();
    bx = 0x235;
    *(dw*)(raddr(ds,0x45)) = bx;
sub_10851:
    bx = 0x28B;
    tnd_sub_107b5();
    *(dw*)(raddr(ds,0x52)) = 0x26B;
    bx = 0x2FD;
    tnd_sub_107b5(); return;
}
void tnd_sub_10851(void) {
    bx = 0x28B;
    tnd_sub_107b5();
    *(dw*)(raddr(ds,0x52)) = 0x26B;
    bx = 0x2FD;
    tnd_sub_107b5(); return;
}
void tnd_sub_10867(void) {
edummylabel50:
    tnd_sub_10833();
    *(dw*)(raddr(ds,0x38)) = 0x210;
    *(dw*)(raddr(ds,0x45)) = 0x246;
    bx = 0x2AC;
    *(dw*)(raddr(ds,0x52)) = bx;
    bl = 0x18;
    *(db*)raddr(ds,0x5E) = bl;
    return;
}
void tnd_sub_10889(void) {
edummylabel51:
    CF = (dd)*(raddr(ds,0x67)) < (dd)0x0FF;
    if (*(raddr(ds,0x67)) == 0x0FF) { return; }
    ax = bx;
    CF = (dd)ah < (dd)0x0BF;
    if (ah <= 0x0BF) {
        { if (1) { CF = (ah >> ((1)-1)) & 1; ah = ah >> 1; } }
        ah |= 0x90; CF = 0;
        ah &= 0x0F0; CF = 0;
        al &= 0x0F; CF = 0;
        al |= ah; CF = 0;
        out(0x0C0, al);
        ah &= 0x0E0; CF = 0;
        al = bl;
        { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
        { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
        { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
        { if (1) { CF = (al >> ((1)-1)) & 1; al = al >> 1; } }
        al |= ah; CF = 0;
        out(0x0C0, al);
        al = bh;
        al &= 0x3F; CF = 0;
        out(0x0C0, al);
locret_108bc:
        return;
    }
loc_108bd:
    al = bh;
    al |= 0x0E0; CF = 0;
    out(0x0C0, al);
    al = bl;
    al |= 0x0F0; CF = 0;
    out(0x0C0, al);
    return;
}