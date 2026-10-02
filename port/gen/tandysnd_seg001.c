#include "../rt.h"
#include "../tnd_syms.h"
#include "../tnd_procs.h"
void tnd_edummylabel1(void) {
    return;
}
void tnd_uninstall_timer(void) {
    dd _sa = 0, _sb = 0;
    IF = 0;
    push(ds);
    bx = *(dw*)(&dword_103de);
    cx = *(dw*)(((db*)&dword_103de)+2);
    { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
    ds = ax;
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
void tnd_edummylabel11(void) {
    dd _sa = 0, _sb = 0;
    out(0x0C0, al);
    out(0x0C0, 0x0BF);
    out(0x0C0, 0x0DF);
    al = 0x0FF;
    out(0x0C0, al);
    cx = 0x38;
    bx = 0x2f;
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
void tnd_edummylabel12(void) {
    dd _sa = 0, _sb = 0;
    if (!ZF) {
        CF = (dd)*(dw*)(raddr(ds,0x63)) < (dd)0; ZF = ((dw)((*(dw*)(raddr(ds,0x63))) - (0)) == 0); SF = (((dw)((*(dw*)(raddr(ds,0x63))) - (0))) >> 15);
        if (*(dw*)(raddr(ds,0x63)) != 0) {
            CF = (dd)*(raddr(ds,0x49)) < (dd)0; ZF = ((dw)((*(raddr(ds,0x49))) - (0)) == 0); SF = (((dw)((*(raddr(ds,0x49))) - (0))) >> 15);
            if (*(raddr(ds,0x49)) == 0) {
                bx = *(dw*)raddr(ds,0x63);
                tnd_stream_start(); return;
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
                    tnd_stream_start(); return;
                }
            }
loc_10502:
            (*(raddr(ds,0x66)))--; ZF = ((dw)(*(raddr(ds,0x66))) == 0); SF = (((dw)(*(raddr(ds,0x66)))) >> 15);
        }
    }
loc_10506:
    di = 0x2F;
    tnd_chan_tick();
    di = 0x3C;
    tnd_chan_tick();
    di = 0x49;
    tnd_chan_tick();
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
        tnd_snd_reg_write(); return;
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
            tnd_snd_reg_write();
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
        tnd_note_out();
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
void tnd_edummylabel13(void) {
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
    tnd_note_out();
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
        tnd_note_out();
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
void tnd_edummylabel14(void) {
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x0F0;
    tnd_sfx_play_pri(); return;
locret_1069d:
    return;
}
void tnd_edummylabel15(void) {
    al = 0x1E;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x108;
    tnd_sfx_play_pri();
    bx = 0x100;
    tnd_sfx_play(); return;
locret_1069d:
    return;
}
void tnd_edummylabel16(void) {
    bx = 0x9D;
    tnd_sfx_play(); return;
}
void tnd_edummylabel17(void) {
    al = 0x64;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    bx = 0x198;
    if (al >= *(db*)raddr(ds,0x30)) {
        tnd_sfx_play_pri();
        bx = 0x1A7;
        tnd_sfx_play(); return;
    }
loc_10694:
    IF = 0;
    ax = 0x3C;
    tnd_voice_update();
    IF = 1;
locret_1069d:
    return;
}
void tnd_edummylabel18(void) {
    bx = 0x1BB;
    goto loc_106a9;
loc_106a9:
    *(dw*)(raddr(ds,0x63)) = bx;
    tnd_sfx_play(); return;
}
void tnd_edummylabel19(void) {
    bx = 0x1C3;
loc_106a9:
    *(dw*)(raddr(ds,0x63)) = bx;
    tnd_sfx_play(); return;
}
void tnd_edummylabel2(void) {
    return;
}
void tnd_edummylabel20(void) {
    dd _sa = 0, _sb = 0;
    *(dw*)(raddr(ds,0x63)) = 0;
    CF = (dd)*(raddr(ds,0x49)) < (dd)0; ZF = ((dw)((*(raddr(ds,0x49))) - (0)) == 0); SF = (((dw)((*(raddr(ds,0x49))) - (0))) >> 15);
    if (*(raddr(ds,0x49)) == 0) { return; }
    _sa = (*(raddr(ds,0x49)));
    *(raddr(ds,0x49)) = 1;
locret_106c2:
    return;
}
void tnd_edummylabel21(void) {
    *(dw*)(raddr(ds,0x63)) = 0;
    bx = 0x74;
    tnd_sfx_play(); return;
}
void tnd_edummylabel22(void) {
    bx = 0x72;
    tnd_sfx_play(); return;
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
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x153;
loc_106f4:
    tnd_sfx_play_pri();
    bx = 0x143;
    tnd_sfx_play(); return;
locret_106e7:
    return;
}
void tnd_edummylabel26(void) {
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x8D;
    tnd_sfx_play_pri(); return;
locret_106e7:
    return;
}
void tnd_edummylabel27(void) {
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x11F;
    tnd_sfx_play_pri(); return;
locret_106e7:
    return;
}
void tnd_edummylabel28(void) {
    al = 0x41;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x15B;
    goto loc_106f4;
loc_106f4:
    tnd_sfx_play_pri();
    bx = 0x143;
    tnd_sfx_play(); return;
}
void tnd_edummylabel29(void) {
    bx = 0x16A;
    goto loc_10735;
loc_10735:
    tnd_sfx_play();
    bx = 0x163;
    tnd_sfx_play(); return;
}
void tnd_edummylabel3(void) {
    push(cs);
    ds = pop();
    tnd_snd_all_off();
    return;
}
void tnd_edummylabel30(void) {
    bx = 0x174;
loc_10735:
    tnd_sfx_play();
    bx = 0x163;
    tnd_sfx_play(); return;
}
void tnd_edummylabel31(void) {
    bx = 0x16C;
    goto loc_1074a;
loc_1074a:
    al = 0x28;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    tnd_sfx_play_pri();
    bx = 0x165;
    tnd_sfx_play(); return;
locret_107ba:
    return;
}
void tnd_edummylabel32(void) {
    bx = 0x176;
loc_1074a:
    al = 0x28;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    tnd_sfx_play_pri();
    bx = 0x165;
    tnd_sfx_play(); return;
locret_107ba:
    return;
}
void tnd_edummylabel33(void) {
    al = 0x3C;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x0A6;
    tnd_sfx_play_pri(); return;
locret_107ba:
    return;
}
void tnd_edummylabel34(void) {
    al = 0x63;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x187;
    tnd_sfx_play_pri();
    bx = 0x17E;
    tnd_sfx_play(); return;
locret_107ba:
    return;
}
void tnd_edummylabel35(void) {
    al = 0x46;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x0F8;
    goto loc_1079c;
loc_1079c:
    tnd_sfx_play_pri();
    bx = 0x133;
    tnd_sfx_play(); return;
locret_107ba:
    return;
}
void tnd_edummylabel36(void) {
    al = 0x5F;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x13B;
loc_1079c:
    tnd_sfx_play_pri();
    bx = 0x133;
    tnd_sfx_play(); return;
locret_107ba:
    return;
}
void tnd_edummylabel37(void) {
    al = 0x2E;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x126;
sub_107b2:
    *(db*)raddr(ds,0x30) = al;
sub_107b5:
    IF = 0;
    tnd_stream_start();
    IF = 1;
locret_107ba:
    return;
}
void tnd_edummylabel38(void) {
    al = 0x2D;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x12E;
    tnd_sfx_play_pri(); return;
locret_107ba:
    return;
}
void tnd_edummylabel39(void) {
    al = 0x55;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x190;
    tnd_sfx_play_pri(); return;
locret_107ba:
    return;
}
void tnd_edummylabel4(void) {
    push(cs);
    ds = pop();
    CF = (dd)byte_100d7 < (dd)0x0FF; ZF = ((db)((byte_100d7) - (0x0FF)) == 0); SF = (((db)((byte_100d7) - (0x0FF))) >> 7);
    if (byte_100d7 == 0x0FF) { return; }
    { vfn f_ = func_at(tnd_cbase + (*(dw*)(((db*)&off_10380)+bx))); dw sp_ = sp; if (f_) f_(); else fprintf(stderr, "unresolved ind call %x\n", (dd)(tnd_cbase + (*(dw*)(((db*)&off_10380)+bx)))); if ((short)(sp - sp_) > 0) { sp = sp_; return; } }
locret_1008c:
    return;
}
void tnd_edummylabel40(void) {
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x117;
    tnd_sfx_play_pri(); return;
locret_107ba:
    return;
}
void tnd_edummylabel41(void) {
    al = 0x50;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x14B;
    tnd_sfx_play_pri();
    bx = 0x153;
    tnd_sfx_play(); return;
locret_107ba:
    return;
}
void tnd_edummylabel42(void) {
    bx = 0x0AE;
    goto loc_10805;
loc_10805:
    tnd_sfx_play();
    bx = 0x0DA;
    tnd_sfx_play(); return;
}
void tnd_edummylabel43(void) {
    bx = 0x0C4;
loc_10805:
    tnd_sfx_play();
    bx = 0x0DA;
    tnd_sfx_play(); return;
}
void tnd_edummylabel44(void) {
    bx = 0x0E1;
    tnd_sfx_play(); return;
}
void tnd_edummylabel45(void) {
    bx = 0x1D0;
    tnd_sfx_play(); return;
}
void tnd_edummylabel46(void) {
    bx = 0x1CB;
    tnd_sfx_play(); return;
}
void tnd_edummylabel47(void) {
    bx = 0x6A;
    tnd_sfx_play(); return;
}
void tnd_edummylabel48(void) {
    bx = 0x110;
    tnd_sfx_play();
    bx = 0x84;
    tnd_sfx_play(); return;
}
void tnd_edummylabel49(void) {
    bx = 0x1EF;
    tnd_sfx_play();
    *(dw*)(raddr(ds,0x38)) = 0x1DF;
    bx = 0x23D;
    tnd_sfx_play();
    bx = 0x235;
    *(dw*)(raddr(ds,0x45)) = bx;
sub_10851:
    bx = 0x28B;
    tnd_sfx_play();
    *(dw*)(raddr(ds,0x52)) = 0x26B;
    bx = 0x2FD;
    tnd_sfx_play(); return;
}
void tnd_edummylabel5(void) {
    push(cs);
    ds = pop();
    byte_100d7 = 0;
    return;
}
void tnd_edummylabel50(void) {
    tnd_sfx3_1ef_23d();
    *(dw*)(raddr(ds,0x38)) = 0x210;
    *(dw*)(raddr(ds,0x45)) = 0x246;
    bx = 0x2AC;
    *(dw*)(raddr(ds,0x52)) = bx;
    bl = 0x18;
    *(db*)raddr(ds,0x5E) = bl;
    return;
}
void tnd_edummylabel51(void) {
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
void tnd_edummylabel6(void) {
    push(cs);
    ds = pop();
    byte_100d7 = 0x0FF;
    return;
}
void tnd_edummylabel7(void) {
    CF = (dd)bx < (dd)0x52; ZF = ((dw)((bx) - (0x52)) == 0); SF = (((dw)((bx) - (0x52))) >> 15);
    if (bx > 0x52) { return; }
    { vfn f_ = func_at(tnd_cbase + (*(dw*)(((db*)&off_10380)+bx))); dw sp_ = sp; if (f_) f_(); else fprintf(stderr, "unresolved ind call %x\n", (dd)(tnd_cbase + (*(dw*)(((db*)&off_10380)+bx)))); if ((short)(sp - sp_) > 0) { sp = sp_; return; } }
locret_103dd:
    return;
}
void tnd_timer_isr(void) {
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
    tnd_seq_tick();
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
        ds = ax;
        bx = *(dw*)raddr(ds,0x20);
        cx = *(dw*)raddr(ds,0x22);
        *(dw*)(&dword_103de) = bx;
        *(dw*)(((db*)&dword_103de)+2) = cx;
        ds = pop();
        IF = 0;
        *(raddr(ds,0x69)) = 3;
        push(ds);
        { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
        ds = ax;
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
    ds = ax;
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
void tnd_install_timer_2(void) {
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
    ds = ax;
    bx = *(dw*)raddr(ds,0x20);
    cx = *(dw*)raddr(ds,0x22);
    *(dw*)(&dword_103de) = bx;
    *(dw*)(((db*)&dword_103de)+2) = cx;
    ds = pop();
    IF = 0;
    *(raddr(ds,0x69)) = 3;
    push(ds);
    { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
    ds = ax;
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
void tnd_timer_isr_chain(void) {
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
    ds = ax;
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
void tnd_memfill_dn(void) {
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
void tnd_memfill_dn_loop(void) {
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
void tnd_music_tick_restart(void) {
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
                tnd_stream_start(); return;
            }
        }
loc_10502:
        (*(raddr(ds,0x66)))--; ZF = ((dw)(*(raddr(ds,0x66))) == 0); SF = (((dw)(*(raddr(ds,0x66)))) >> 15);
    }
loc_10506:
    di = 0x2F;
    tnd_chan_tick();
    di = 0x3C;
    tnd_chan_tick();
    di = 0x49;
    tnd_chan_tick();
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
        tnd_snd_reg_write(); return;
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
            tnd_snd_reg_write();
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
        tnd_note_out();
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
void tnd_chans_tick_gate(void) {
    dd _sa = 0, _sb = 0;
    (*(raddr(ds,0x66)))--; ZF = ((dw)(*(raddr(ds,0x66))) == 0); SF = (((dw)(*(raddr(ds,0x66)))) >> 15);
loc_10506:
    di = 0x2F;
    tnd_chan_tick();
    di = 0x3C;
    tnd_chan_tick();
    di = 0x49;
    tnd_chan_tick();
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
        tnd_snd_reg_write(); return;
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
            tnd_snd_reg_write();
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
        tnd_note_out();
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
void tnd_chans_tick(void) {
    dd _sa = 0, _sb = 0;
    di = 0x2F;
    tnd_chan_tick();
    di = 0x3C;
    tnd_chan_tick();
    di = 0x49;
    tnd_chan_tick();
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
        tnd_snd_reg_write(); return;
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
            tnd_snd_reg_write();
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
        tnd_note_out();
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
void tnd_env_step(void) {
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
    tnd_snd_reg_write(); return;
}
void tnd_note_out_tail(void) {
    bx = *(dw*)raddr(ds,di+2);
    tnd_snd_reg_write(); return;
}
void tnd_chan_init(void) {
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
            tnd_snd_reg_write();
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
        tnd_note_out();
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
void tnd_evt_seek(void) {
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
void tnd_evt_read(void) {
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
        tnd_note_out();
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
void tnd_evt_maybe_dur(void) {
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
void tnd_evt_commit(void) {
    cx = bx;
    (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
loc_105fd:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void tnd_evt_commit_b(void) {
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void tnd_evt_dur(void) {
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
void tnd_evt_maybe_att(void) {
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
void tnd_evt_maybe_rel(void) {
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
void tnd_evt_loop_word(void) {
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
void tnd_evt_loop_store(void) {
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
void tnd_evt_maybe_rpt(void) {
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
void tnd_evt_loop_set(void) {
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
void tnd_evt_rpt_set(void) {
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
void tnd_evt_loop_back(void) {
    cx = *(dw*)raddr(ds,di+9);
    goto loc_105fd;
loc_105fd:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void tnd_chan3_update(void) {
    IF = 0;
    ax = 0x3C;
    tnd_voice_update();
    IF = 1;
locret_1069d:
    return;
}
void tnd_music_go(void) {
    *(dw*)(raddr(ds,0x63)) = bx;
    tnd_sfx_play(); return;
}
void tnd_sfx_pair_143(void) {
    tnd_sfx_play_pri();
    bx = 0x143;
    tnd_sfx_play(); return;
}
void tnd_sfx_pair_163(void) {
    tnd_sfx_play();
    bx = 0x163;
    tnd_sfx_play(); return;
}
void tnd_sfx_pair_165(void) {
    al = 0x28;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    tnd_sfx_play_pri();
    bx = 0x165;
    tnd_sfx_play(); return;
locret_107ba:
    return;
}
void tnd_sfx_pair_133(void) {
    tnd_sfx_play_pri();
    bx = 0x133;
    tnd_sfx_play(); return;
}
void tnd_sfx_pair_da(void) {
    tnd_sfx_play();
    bx = 0x0DA;
    tnd_sfx_play(); return;
}
void tnd_snd_reg_out(void) {
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
void tnd_locret_1008c(void) {
    return;
}
void tnd_locret_103dd(void) {
    return;
}
void tnd_locret_1040a(void) {
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
    ds = ax;
    bx = *(dw*)raddr(ds,0x20);
    cx = *(dw*)raddr(ds,0x22);
    *(dw*)(&dword_103de) = bx;
    *(dw*)(((db*)&dword_103de)+2) = cx;
    ds = pop();
    IF = 0;
    *(raddr(ds,0x69)) = 3;
    push(ds);
    { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
    ds = ax;
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
void tnd_irq8_chain(void) {
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
    ds = ax;
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
void tnd_module_init(void) {
    tnd_install_timer();
edummylabel1:
    return;
}
void tnd_install_timer(void) {
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
    ds = ax;
    bx = *(dw*)raddr(ds,0x20);
    cx = *(dw*)raddr(ds,0x22);
    *(dw*)(&dword_103de) = bx;
    *(dw*)(((db*)&dword_103de)+2) = cx;
    ds = pop();
    IF = 0;
    *(raddr(ds,0x69)) = 3;
    push(ds);
    { dd t_ = (dd)ax - (dd)ax; CF = (dd)ax < (dd)ax; ax = t_; ZF = ((dw)(ax) == 0); SF = (((dw)(ax)) >> 15); }
    ds = ax;
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
void tnd_snd_all_off(void) {
    dd _sa = 0, _sb = 0;
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
void tnd_seq_tick(void) {
    dd _sa = 0, _sb = 0;
    CF = (dd)*(raddr(ds,0x69)) < (dd)1; ZF = ((dw)((*(raddr(ds,0x69))) - (1)) == 0); SF = (((dw)((*(raddr(ds,0x69))) - (1))) >> 15);
edummylabel12:
    if (!ZF) {
        CF = (dd)*(dw*)(raddr(ds,0x63)) < (dd)0; ZF = ((dw)((*(dw*)(raddr(ds,0x63))) - (0)) == 0); SF = (((dw)((*(dw*)(raddr(ds,0x63))) - (0))) >> 15);
        if (*(dw*)(raddr(ds,0x63)) != 0) {
            CF = (dd)*(raddr(ds,0x49)) < (dd)0; ZF = ((dw)((*(raddr(ds,0x49))) - (0)) == 0); SF = (((dw)((*(raddr(ds,0x49))) - (0))) >> 15);
            if (*(raddr(ds,0x49)) == 0) {
                bx = *(dw*)raddr(ds,0x63);
                tnd_stream_start(); return;
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
                    tnd_stream_start(); return;
                }
            }
loc_10502:
            (*(raddr(ds,0x66)))--; ZF = ((dw)(*(raddr(ds,0x66))) == 0); SF = (((dw)(*(raddr(ds,0x66)))) >> 15);
        }
    }
loc_10506:
    di = 0x2F;
    tnd_chan_tick();
    di = 0x3C;
    tnd_chan_tick();
    di = 0x49;
    tnd_chan_tick();
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
        tnd_snd_reg_write(); return;
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
            tnd_snd_reg_write();
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
        tnd_note_out();
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
void tnd_chan_tick(void) {
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
        tnd_snd_reg_write(); return;
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
            tnd_snd_reg_write();
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
        tnd_note_out();
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
void tnd_note_out(void) {
    dd _sa = 0, _sb = 0;
    bx = *(dw*)raddr(ds,di+2);
    bx |= bx; CF = 0; OF = 0; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    if (bx != 0) {
        bl |= 0x0F; CF = 0; OF = 0; ZF = ((db)(bl) == 0); SF = (((db)(bl)) >> 7);
        tnd_snd_reg_write();
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
void tnd_stream_start(void) {
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
    tnd_note_out();
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
        tnd_note_out();
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
void tnd_voice_update(void) {
    dd _sa = 0, _sb = 0;
    di = ax;
    push(bx);
    push(*(dw*)(raddr(ds,di)));
    tnd_note_out();
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
        tnd_note_out();
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
void tnd_sfx_0f0(void) {
edummylabel14:
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x0F0;
    tnd_sfx_play_pri(); return;
locret_1069d:
    return;
}
void tnd_sfx_108_100(void) {
edummylabel15:
    al = 0x1E;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x108;
    tnd_sfx_play_pri();
    bx = 0x100;
    tnd_sfx_play(); return;
locret_1069d:
    return;
}
void tnd_sfx_09d(void) {
edummylabel16:
    bx = 0x9D;
    tnd_sfx_play(); return;
}
void tnd_sfx_198_1a7(void) {
edummylabel17:
    al = 0x64;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    bx = 0x198;
    if (al >= *(db*)raddr(ds,0x30)) {
        tnd_sfx_play_pri();
        bx = 0x1A7;
        tnd_sfx_play(); return;
    }
loc_10694:
    IF = 0;
    ax = 0x3C;
    tnd_voice_update();
    IF = 1;
locret_1069d:
    return;
}
void tnd_music_play_a(void) {
edummylabel18:
    bx = 0x1BB;
    goto loc_106a9;
loc_106a9:
    *(dw*)(raddr(ds,0x63)) = bx;
    tnd_sfx_play(); return;
}
void tnd_music_play_b(void) {
edummylabel19:
    bx = 0x1C3;
loc_106a9:
    *(dw*)(raddr(ds,0x63)) = bx;
    tnd_sfx_play(); return;
}
void tnd_music_stop(void) {
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
void tnd_sfx_74(void) {
edummylabel21:
    *(dw*)(raddr(ds,0x63)) = 0;
    bx = 0x74;
    tnd_sfx_play(); return;
}
void tnd_sfx_72(void) {
edummylabel22:
    bx = 0x72;
    tnd_sfx_play(); return;
}
void tnd_seq_tempo_on(void) {
edummylabel23:
    *(raddr(ds,0x66)) = 0x1E;
    *(raddr(ds,0x65)) = 0x0FF;
    return;
}
void tnd_seq_tempo_off(void) {
edummylabel24:
    *(raddr(ds,0x65)) = 0;
locret_106e7:
    return;
}
void tnd_sfx_153_143(void) {
edummylabel25:
    al = 0x4B;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x153;
loc_106f4:
    tnd_sfx_play_pri();
    bx = 0x143;
    tnd_sfx_play(); return;
locret_106e7:
    return;
}
void tnd_sfx_08d(void) {
edummylabel26:
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x8D;
    tnd_sfx_play_pri(); return;
locret_106e7:
    return;
}
void tnd_sfx_11f(void) {
edummylabel27:
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x11F;
    tnd_sfx_play_pri(); return;
locret_106e7:
    return;
}
void tnd_sfx_15b_143(void) {
edummylabel28:
    al = 0x41;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x15B;
    goto loc_106f4;
loc_106f4:
    tnd_sfx_play_pri();
    bx = 0x143;
    tnd_sfx_play(); return;
}
void tnd_sfx_16a_163(void) {
edummylabel29:
    bx = 0x16A;
    goto loc_10735;
loc_10735:
    tnd_sfx_play();
    bx = 0x163;
    tnd_sfx_play(); return;
}
void tnd_sfx_174_163(void) {
edummylabel30:
    bx = 0x174;
loc_10735:
    tnd_sfx_play();
    bx = 0x163;
    tnd_sfx_play(); return;
}
void tnd_sfx_16c_165(void) {
edummylabel31:
    bx = 0x16C;
    goto loc_1074a;
loc_1074a:
    al = 0x28;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    tnd_sfx_play_pri();
    bx = 0x165;
    tnd_sfx_play(); return;
locret_107ba:
    return;
}
void tnd_sfx_176_165(void) {
edummylabel32:
    bx = 0x176;
loc_1074a:
    al = 0x28;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    tnd_sfx_play_pri();
    bx = 0x165;
    tnd_sfx_play(); return;
locret_107ba:
    return;
}
void tnd_sfx_0a6(void) {
edummylabel33:
    al = 0x3C;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x0A6;
    tnd_sfx_play_pri(); return;
locret_107ba:
    return;
}
void tnd_sfx_187_17e(void) {
edummylabel34:
    al = 0x63;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x187;
    tnd_sfx_play_pri();
    bx = 0x17E;
    tnd_sfx_play(); return;
locret_107ba:
    return;
}
void tnd_sfx_0f8_133(void) {
edummylabel35:
    al = 0x46;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x0F8;
    goto loc_1079c;
loc_1079c:
    tnd_sfx_play_pri();
    bx = 0x133;
    tnd_sfx_play(); return;
locret_107ba:
    return;
}
void tnd_sfx_13b_133(void) {
edummylabel36:
    al = 0x5F;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x13B;
loc_1079c:
    tnd_sfx_play_pri();
    bx = 0x133;
    tnd_sfx_play(); return;
locret_107ba:
    return;
}
void tnd_sfx_126(void) {
edummylabel37:
    al = 0x2E;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x126;
sub_107b2:
    *(db*)raddr(ds,0x30) = al;
sub_107b5:
    IF = 0;
    tnd_stream_start();
    IF = 1;
locret_107ba:
    return;
}
void tnd_sfx_play_pri(void) {
    *(db*)raddr(ds,0x30) = al;
sub_107b5:
    IF = 0;
    tnd_stream_start();
    IF = 1;
locret_107ba:
    return;
}
void tnd_sfx_play(void) {
    IF = 0;
    tnd_stream_start();
    IF = 1;
locret_107ba:
    return;
}
void tnd_sfx_12e(void) {
edummylabel38:
    al = 0x2D;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x12E;
    tnd_sfx_play_pri(); return;
locret_107ba:
    return;
}
void tnd_sfx_190(void) {
edummylabel39:
    al = 0x55;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x190;
    tnd_sfx_play_pri(); return;
locret_107ba:
    return;
}
void tnd_sfx_117(void) {
edummylabel40:
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x117;
    tnd_sfx_play_pri(); return;
locret_107ba:
    return;
}
void tnd_sfx_14b_153(void) {
edummylabel41:
    al = 0x50;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x14B;
    tnd_sfx_play_pri();
    bx = 0x153;
    tnd_sfx_play(); return;
locret_107ba:
    return;
}
void tnd_sfx_pair_ae_da(void) {
edummylabel42:
    bx = 0x0AE;
    goto loc_10805;
loc_10805:
    tnd_sfx_play();
    bx = 0x0DA;
    tnd_sfx_play(); return;
}
void tnd_sfx_pair_c4_da(void) {
edummylabel43:
    bx = 0x0C4;
loc_10805:
    tnd_sfx_play();
    bx = 0x0DA;
    tnd_sfx_play(); return;
}
void tnd_sfx_0e1(void) {
edummylabel44:
    bx = 0x0E1;
    tnd_sfx_play(); return;
}
void tnd_sfx_1d0(void) {
edummylabel45:
    bx = 0x1D0;
    tnd_sfx_play(); return;
}
void tnd_sfx_1cb(void) {
edummylabel46:
    bx = 0x1CB;
    tnd_sfx_play(); return;
}
void tnd_sfx_06a(void) {
edummylabel47:
    bx = 0x6A;
    tnd_sfx_play(); return;
}
void tnd_sfx_110_84(void) {
edummylabel48:
    bx = 0x110;
    tnd_sfx_play();
    bx = 0x84;
    tnd_sfx_play(); return;
}
void tnd_sfx3_1ef_23d(void) {
edummylabel49:
    bx = 0x1EF;
    tnd_sfx_play();
    *(dw*)(raddr(ds,0x38)) = 0x1DF;
    bx = 0x23D;
    tnd_sfx_play();
    bx = 0x235;
    *(dw*)(raddr(ds,0x45)) = bx;
sub_10851:
    bx = 0x28B;
    tnd_sfx_play();
    *(dw*)(raddr(ds,0x52)) = 0x26B;
    bx = 0x2FD;
    tnd_sfx_play(); return;
}
void tnd_sfx3_26b_2fd(void) {
    bx = 0x28B;
    tnd_sfx_play();
    *(dw*)(raddr(ds,0x52)) = 0x26B;
    bx = 0x2FD;
    tnd_sfx_play(); return;
}
void tnd_sfx3_210_246(void) {
edummylabel50:
    tnd_sfx3_1ef_23d();
    *(dw*)(raddr(ds,0x38)) = 0x210;
    *(dw*)(raddr(ds,0x45)) = 0x246;
    bx = 0x2AC;
    *(dw*)(raddr(ds,0x52)) = bx;
    bl = 0x18;
    *(db*)raddr(ds,0x5E) = bl;
    return;
}
void tnd_snd_reg_write(void) {
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