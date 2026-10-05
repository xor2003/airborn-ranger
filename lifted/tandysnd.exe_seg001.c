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
memfill_dn:
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15); }
memfill_dn_loop:
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
                stream_start(); return;
            }
        }
music_tick_restart:
        CF = (dd)*(raddr(ds,0x65)) < (dd)0; ZF = ((dw)((*(raddr(ds,0x65))) - (0)) == 0); SF = (((dw)((*(raddr(ds,0x65))) - (0))) >> 15);
        if (*(raddr(ds,0x65)) != 0) {
            CF = (dd)*(raddr(ds,0x66)) < (dd)0; ZF = ((dw)((*(raddr(ds,0x66))) - (0)) == 0); SF = (((dw)((*(raddr(ds,0x66))) - (0))) >> 15);
            if ((short)*(raddr(ds,0x66)) <= (short)0) {
                _sa = (*(raddr(ds,0x66)));
                *(raddr(ds,0x66)) = 0x1E;
                CF = (dd)*(raddr(ds,0x2F)) < (dd)0; ZF = ((dw)((*(raddr(ds,0x2F))) - (0)) == 0); SF = (((dw)((*(raddr(ds,0x2F))) - (0))) >> 15);
                if (*(raddr(ds,0x2F)) == 0) {
                    bx = 0x1B6;
                    stream_start(); return;
                }
            }
chans_tick_gate:
            (*(raddr(ds,0x66)))--; ZF = ((dw)(*(raddr(ds,0x66))) == 0); SF = (((dw)(*(raddr(ds,0x66)))) >> 15);
        }
    }
chans_tick:
    di = 0x2F;
    chan_tick();
    di = 0x3C;
    chan_tick();
    di = 0x49;
    chan_tick();
    di = 0x56;
chan_tick:
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
env_step:
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
note_out_tail:
        bx = *(dw*)raddr(ds,di+2);
        snd_reg_write(); return;
    }
chan_init:
    bx = *(dw*)raddr(ds,di+0x0B);
    al = *(db*)raddr(ds,bx);
    al |= al; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
    if (al == 0) {
note_out:
        bx = *(dw*)raddr(ds,di+2);
        bx |= bx; CF = 0; OF = 0; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        if (bx != 0) {
            bl |= 0x0F; CF = 0; OF = 0; ZF = ((db)(bl) == 0); SF = (((db)(bl)) >> 7);
            snd_reg_write();
        }
evt_seek:
        bx = di;
        cx = 0x0D;
        goto memfill_dn;
    }
evt_read:
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
        note_out();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto evt_maybe_rpt;
evt_maybe_dur:
        CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
        if ((al & 0x84) != 0) goto evt_loop_set;
evt_commit:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
evt_commit_b:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
evt_dur:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0; OF = 0; ZF = ((db)((al & 0x0B0)) == 0); SF = (((db)((al & 0x0B0))) >> 7);
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
        goto evt_loop_store;
    }
evt_maybe_att:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0A0)) == 0); SF = (((db)((al & 0x0A0))) >> 7);
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+4) = cl;
    }
evt_maybe_rel:
    CF = 0; OF = 0; ZF = ((db)((al & 0x90)) == 0); SF = (((db)((al & 0x90))) >> 7);
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+5) = cl;
    }
evt_loop_word:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
evt_loop_store:
    *(dw*)(raddr(ds,di+6)) = cx;
evt_maybe_rpt:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) == 0) goto evt_maybe_dur;
    cl = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
        if (*(raddr(ds,di+8)) != 0) goto evt_loop_back;
evt_loop_set:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
        *(dw*)(raddr(ds,di+9)) = cx;
        goto evt_commit;
    }
evt_rpt_set:
    *(db*)raddr(ds,di+8) = cl;
evt_loop_back:
    cx = *(dw*)raddr(ds,di+9);
    goto evt_commit_b;
memfill_dn:
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15); }
memfill_dn_loop:
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
voice_update:
    di = ax;
    push(bx);
    push(*(dw*)(raddr(ds,di)));
    note_out();
    *(dw*)(raddr(ds,di)) = pop();
    bx = pop();
    *(dw*)(raddr(ds,di+9)) = bx;
evt_read:
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
        note_out();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto evt_maybe_rpt;
evt_maybe_dur:
        CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
        if ((al & 0x84) != 0) goto evt_loop_set;
evt_commit:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
evt_commit_b:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
evt_dur:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0; OF = 0; ZF = ((db)((al & 0x0B0)) == 0); SF = (((db)((al & 0x0B0))) >> 7);
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
        goto evt_loop_store;
    }
evt_maybe_att:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0A0)) == 0); SF = (((db)((al & 0x0A0))) >> 7);
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+4) = cl;
    }
evt_maybe_rel:
    CF = 0; OF = 0; ZF = ((db)((al & 0x90)) == 0); SF = (((db)((al & 0x90))) >> 7);
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+5) = cl;
    }
evt_loop_word:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
evt_loop_store:
    *(dw*)(raddr(ds,di+6)) = cx;
evt_maybe_rpt:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) == 0) goto evt_maybe_dur;
    cl = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
        if (*(raddr(ds,di+8)) != 0) goto evt_loop_back;
evt_loop_set:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
        *(dw*)(raddr(ds,di+9)) = cx;
        goto evt_commit;
    }
evt_rpt_set:
    *(db*)raddr(ds,di+8) = cl;
evt_loop_back:
    cx = *(dw*)raddr(ds,di+9);
    goto evt_commit_b;
}
void edummylabel14(void) {
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x0F0;
    sfx_play_pri(); return;
locret_1069d:
    return;
}
void edummylabel15(void) {
    al = 0x1E;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x108;
    sfx_play_pri();
    bx = 0x100;
    sfx_play(); return;
locret_1069d:
    return;
}
void edummylabel16(void) {
    bx = 0x9D;
    sfx_play(); return;
}
void edummylabel17(void) {
    al = 0x64;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    bx = 0x198;
    if (al >= *(db*)raddr(ds,0x30)) {
        sfx_play_pri();
        bx = 0x1A7;
        sfx_play(); return;
    }
chan3_update:
    IF = 0;
    ax = 0x3C;
    voice_update();
    IF = 1;
locret_1069d:
    return;
}
void edummylabel18(void) {
    bx = 0x1BB;
    goto music_go;
music_go:
    *(dw*)(raddr(ds,0x63)) = bx;
    sfx_play(); return;
}
void edummylabel19(void) {
    bx = 0x1C3;
music_go:
    *(dw*)(raddr(ds,0x63)) = bx;
    sfx_play(); return;
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
    sfx_play(); return;
}
void edummylabel22(void) {
    bx = 0x72;
    sfx_play(); return;
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
sfx_pair_143:
    sfx_play_pri();
    bx = 0x143;
    sfx_play(); return;
locret_106e7:
    return;
}
void edummylabel26(void) {
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x8D;
    sfx_play_pri(); return;
locret_106e7:
    return;
}
void edummylabel27(void) {
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x11F;
    sfx_play_pri(); return;
locret_106e7:
    return;
}
void edummylabel28(void) {
    al = 0x41;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x15B;
    goto sfx_pair_143;
sfx_pair_143:
    sfx_play_pri();
    bx = 0x143;
    sfx_play(); return;
}
void edummylabel29(void) {
    bx = 0x16A;
    goto sfx_pair_163;
sfx_pair_163:
    sfx_play();
    bx = 0x163;
    sfx_play(); return;
}
void edummylabel3(void) {
    push(cs);
    ds = pop();
    snd_all_off();
    return;
}
void edummylabel30(void) {
    bx = 0x174;
sfx_pair_163:
    sfx_play();
    bx = 0x163;
    sfx_play(); return;
}
void edummylabel31(void) {
    bx = 0x16C;
    goto sfx_pair_165;
sfx_pair_165:
    al = 0x28;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    sfx_play_pri();
    bx = 0x165;
    sfx_play(); return;
locret_107ba:
    return;
}
void edummylabel32(void) {
    bx = 0x176;
sfx_pair_165:
    al = 0x28;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    sfx_play_pri();
    bx = 0x165;
    sfx_play(); return;
locret_107ba:
    return;
}
void edummylabel33(void) {
    al = 0x3C;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x0A6;
    sfx_play_pri(); return;
locret_107ba:
    return;
}
void edummylabel34(void) {
    al = 0x63;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x187;
    sfx_play_pri();
    bx = 0x17E;
    sfx_play(); return;
locret_107ba:
    return;
}
void edummylabel35(void) {
    al = 0x46;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x0F8;
    goto sfx_pair_133;
sfx_pair_133:
    sfx_play_pri();
    bx = 0x133;
    sfx_play(); return;
locret_107ba:
    return;
}
void edummylabel36(void) {
    al = 0x5F;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x13B;
sfx_pair_133:
    sfx_play_pri();
    bx = 0x133;
    sfx_play(); return;
locret_107ba:
    return;
}
void edummylabel37(void) {
    al = 0x2E;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x126;
sfx_play_pri:
    *(db*)raddr(ds,0x30) = al;
sfx_play:
    IF = 0;
    stream_start();
    IF = 1;
locret_107ba:
    return;
}
void edummylabel38(void) {
    al = 0x2D;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x12E;
    sfx_play_pri(); return;
locret_107ba:
    return;
}
void edummylabel39(void) {
    al = 0x55;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x190;
    sfx_play_pri(); return;
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
    sfx_play_pri(); return;
locret_107ba:
    return;
}
void edummylabel41(void) {
    al = 0x50;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x14B;
    sfx_play_pri();
    bx = 0x153;
    sfx_play(); return;
locret_107ba:
    return;
}
void edummylabel42(void) {
    bx = 0x0AE;
    goto sfx_pair_da;
sfx_pair_da:
    sfx_play();
    bx = 0x0DA;
    sfx_play(); return;
}
void edummylabel43(void) {
    bx = 0x0C4;
sfx_pair_da:
    sfx_play();
    bx = 0x0DA;
    sfx_play(); return;
}
void edummylabel44(void) {
    bx = 0x0E1;
    sfx_play(); return;
}
void edummylabel45(void) {
    bx = 0x1D0;
    sfx_play(); return;
}
void edummylabel46(void) {
    bx = 0x1CB;
    sfx_play(); return;
}
void edummylabel47(void) {
    bx = 0x6A;
    sfx_play(); return;
}
void edummylabel48(void) {
    bx = 0x110;
    sfx_play();
    bx = 0x84;
    sfx_play(); return;
}
void edummylabel49(void) {
    bx = 0x1EF;
    sfx_play();
    *(dw*)(raddr(ds,0x38)) = 0x1DF;
    bx = 0x23D;
    sfx_play();
    bx = 0x235;
    *(dw*)(raddr(ds,0x45)) = bx;
sfx3_26b_2fd:
    bx = 0x28B;
    sfx_play();
    *(dw*)(raddr(ds,0x52)) = 0x26B;
    bx = 0x2FD;
    sfx_play(); return;
}
void edummylabel5(void) {
    push(cs);
    ds = pop();
    byte_100d7 = 0;
    return;
}
void edummylabel50(void) {
    sfx3_1ef_23d();
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
snd_reg_out:
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
install_timer:
        *(raddr(ds,0x67)) = 0;
install_timer_2:
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
timer_isr_chain:
    *(raddr(ds,0x69)) = 3;
    byte_103e2 = 0;
    ds = pop();
    ax = pop();
    { vfn f_ = func_at(rt_far(dword_103de)); if (f_) f_(); else fprintf(stderr, "unresolved ind jmp %x\n", (dd)(rt_far(dword_103de))); return; }
uninstall_timer:
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
uninstall_timer:
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
void memfill_dn(void) {
    dd _sa = 0, _sb = 0;
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15); }
memfill_dn_loop:
    do {
        (bx)--; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        _sa = (bx);
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void memfill_dn_loop(void) {
    dd _sa = 0, _sb = 0;
memfill_dn_loop:
    do {
        (bx)--; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        _sa = (bx);
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void music_tick_restart(void) {
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
                stream_start(); return;
            }
        }
chans_tick_gate:
        (*(raddr(ds,0x66)))--; ZF = ((dw)(*(raddr(ds,0x66))) == 0); SF = (((dw)(*(raddr(ds,0x66)))) >> 15);
    }
chans_tick:
    di = 0x2F;
    chan_tick();
    di = 0x3C;
    chan_tick();
    di = 0x49;
    chan_tick();
    di = 0x56;
chan_tick:
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
env_step:
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
note_out_tail:
        bx = *(dw*)raddr(ds,di+2);
        snd_reg_write(); return;
    }
chan_init:
    bx = *(dw*)raddr(ds,di+0x0B);
    al = *(db*)raddr(ds,bx);
    al |= al; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
    if (al == 0) {
note_out:
        bx = *(dw*)raddr(ds,di+2);
        bx |= bx; CF = 0; OF = 0; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        if (bx != 0) {
            bl |= 0x0F; CF = 0; OF = 0; ZF = ((db)(bl) == 0); SF = (((db)(bl)) >> 7);
            snd_reg_write();
        }
evt_seek:
        bx = di;
        cx = 0x0D;
        goto memfill_dn;
    }
evt_read:
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
        note_out();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto evt_maybe_rpt;
evt_maybe_dur:
        CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
        if ((al & 0x84) != 0) goto evt_loop_set;
evt_commit:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
evt_commit_b:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
evt_dur:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0; OF = 0; ZF = ((db)((al & 0x0B0)) == 0); SF = (((db)((al & 0x0B0))) >> 7);
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
        goto evt_loop_store;
    }
evt_maybe_att:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0A0)) == 0); SF = (((db)((al & 0x0A0))) >> 7);
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+4) = cl;
    }
evt_maybe_rel:
    CF = 0; OF = 0; ZF = ((db)((al & 0x90)) == 0); SF = (((db)((al & 0x90))) >> 7);
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+5) = cl;
    }
evt_loop_word:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
evt_loop_store:
    *(dw*)(raddr(ds,di+6)) = cx;
evt_maybe_rpt:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) == 0) goto evt_maybe_dur;
    cl = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
        if (*(raddr(ds,di+8)) != 0) goto evt_loop_back;
evt_loop_set:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
        *(dw*)(raddr(ds,di+9)) = cx;
        goto evt_commit;
    }
evt_rpt_set:
    *(db*)raddr(ds,di+8) = cl;
evt_loop_back:
    cx = *(dw*)raddr(ds,di+9);
    goto evt_commit_b;
memfill_dn:
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15); }
memfill_dn_loop:
    do {
        (bx)--; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        _sa = (bx);
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void chans_tick_gate(void) {
    dd _sa = 0, _sb = 0;
    (*(raddr(ds,0x66)))--; ZF = ((dw)(*(raddr(ds,0x66))) == 0); SF = (((dw)(*(raddr(ds,0x66)))) >> 15);
chans_tick:
    di = 0x2F;
    chan_tick();
    di = 0x3C;
    chan_tick();
    di = 0x49;
    chan_tick();
    di = 0x56;
chan_tick:
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
env_step:
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
note_out_tail:
        bx = *(dw*)raddr(ds,di+2);
        snd_reg_write(); return;
    }
chan_init:
    bx = *(dw*)raddr(ds,di+0x0B);
    al = *(db*)raddr(ds,bx);
    al |= al; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
    if (al == 0) {
note_out:
        bx = *(dw*)raddr(ds,di+2);
        bx |= bx; CF = 0; OF = 0; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        if (bx != 0) {
            bl |= 0x0F; CF = 0; OF = 0; ZF = ((db)(bl) == 0); SF = (((db)(bl)) >> 7);
            snd_reg_write();
        }
evt_seek:
        bx = di;
        cx = 0x0D;
        goto memfill_dn;
    }
evt_read:
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
        note_out();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto evt_maybe_rpt;
evt_maybe_dur:
        CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
        if ((al & 0x84) != 0) goto evt_loop_set;
evt_commit:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
evt_commit_b:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
evt_dur:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0; OF = 0; ZF = ((db)((al & 0x0B0)) == 0); SF = (((db)((al & 0x0B0))) >> 7);
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
        goto evt_loop_store;
    }
evt_maybe_att:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0A0)) == 0); SF = (((db)((al & 0x0A0))) >> 7);
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+4) = cl;
    }
evt_maybe_rel:
    CF = 0; OF = 0; ZF = ((db)((al & 0x90)) == 0); SF = (((db)((al & 0x90))) >> 7);
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+5) = cl;
    }
evt_loop_word:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
evt_loop_store:
    *(dw*)(raddr(ds,di+6)) = cx;
evt_maybe_rpt:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) == 0) goto evt_maybe_dur;
    cl = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
        if (*(raddr(ds,di+8)) != 0) goto evt_loop_back;
evt_loop_set:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
        *(dw*)(raddr(ds,di+9)) = cx;
        goto evt_commit;
    }
evt_rpt_set:
    *(db*)raddr(ds,di+8) = cl;
evt_loop_back:
    cx = *(dw*)raddr(ds,di+9);
    goto evt_commit_b;
memfill_dn:
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15); }
memfill_dn_loop:
    do {
        (bx)--; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        _sa = (bx);
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void chans_tick(void) {
    dd _sa = 0, _sb = 0;
    di = 0x2F;
    chan_tick();
    di = 0x3C;
    chan_tick();
    di = 0x49;
    chan_tick();
    di = 0x56;
chan_tick:
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
env_step:
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
note_out_tail:
        bx = *(dw*)raddr(ds,di+2);
        snd_reg_write(); return;
    }
chan_init:
    bx = *(dw*)raddr(ds,di+0x0B);
    al = *(db*)raddr(ds,bx);
    al |= al; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
    if (al == 0) {
note_out:
        bx = *(dw*)raddr(ds,di+2);
        bx |= bx; CF = 0; OF = 0; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        if (bx != 0) {
            bl |= 0x0F; CF = 0; OF = 0; ZF = ((db)(bl) == 0); SF = (((db)(bl)) >> 7);
            snd_reg_write();
        }
evt_seek:
        bx = di;
        cx = 0x0D;
        goto memfill_dn;
    }
evt_read:
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
        note_out();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto evt_maybe_rpt;
evt_maybe_dur:
        CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
        if ((al & 0x84) != 0) goto evt_loop_set;
evt_commit:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
evt_commit_b:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
evt_dur:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0; OF = 0; ZF = ((db)((al & 0x0B0)) == 0); SF = (((db)((al & 0x0B0))) >> 7);
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
        goto evt_loop_store;
    }
evt_maybe_att:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0A0)) == 0); SF = (((db)((al & 0x0A0))) >> 7);
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+4) = cl;
    }
evt_maybe_rel:
    CF = 0; OF = 0; ZF = ((db)((al & 0x90)) == 0); SF = (((db)((al & 0x90))) >> 7);
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+5) = cl;
    }
evt_loop_word:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
evt_loop_store:
    *(dw*)(raddr(ds,di+6)) = cx;
evt_maybe_rpt:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) == 0) goto evt_maybe_dur;
    cl = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
        if (*(raddr(ds,di+8)) != 0) goto evt_loop_back;
evt_loop_set:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
        *(dw*)(raddr(ds,di+9)) = cx;
        goto evt_commit;
    }
evt_rpt_set:
    *(db*)raddr(ds,di+8) = cl;
evt_loop_back:
    cx = *(dw*)raddr(ds,di+9);
    goto evt_commit_b;
memfill_dn:
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15); }
memfill_dn_loop:
    do {
        (bx)--; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        _sa = (bx);
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void env_step(void) {
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
note_out_tail:
    bx = *(dw*)raddr(ds,di+2);
    snd_reg_write(); return;
}
void note_out_tail(void) {
    bx = *(dw*)raddr(ds,di+2);
    snd_reg_write(); return;
}
void chan_init(void) {
    dd _sa = 0, _sb = 0;
    bx = *(dw*)raddr(ds,di+0x0B);
    al = *(db*)raddr(ds,bx);
    al |= al; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
    if (al == 0) {
note_out:
        bx = *(dw*)raddr(ds,di+2);
        bx |= bx; CF = 0; OF = 0; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        if (bx != 0) {
            bl |= 0x0F; CF = 0; OF = 0; ZF = ((db)(bl) == 0); SF = (((db)(bl)) >> 7);
            snd_reg_write();
        }
evt_seek:
        bx = di;
        cx = 0x0D;
        goto memfill_dn;
    }
evt_read:
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
        note_out();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto evt_maybe_rpt;
evt_maybe_dur:
        CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
        if ((al & 0x84) != 0) goto evt_loop_set;
evt_commit:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
evt_commit_b:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
evt_dur:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0; OF = 0; ZF = ((db)((al & 0x0B0)) == 0); SF = (((db)((al & 0x0B0))) >> 7);
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
        goto evt_loop_store;
    }
evt_maybe_att:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0A0)) == 0); SF = (((db)((al & 0x0A0))) >> 7);
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+4) = cl;
    }
evt_maybe_rel:
    CF = 0; OF = 0; ZF = ((db)((al & 0x90)) == 0); SF = (((db)((al & 0x90))) >> 7);
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+5) = cl;
    }
evt_loop_word:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
evt_loop_store:
    *(dw*)(raddr(ds,di+6)) = cx;
evt_maybe_rpt:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) == 0) goto evt_maybe_dur;
    cl = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
        if (*(raddr(ds,di+8)) != 0) goto evt_loop_back;
evt_loop_set:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
        *(dw*)(raddr(ds,di+9)) = cx;
        goto evt_commit;
    }
evt_rpt_set:
    *(db*)raddr(ds,di+8) = cl;
evt_loop_back:
    cx = *(dw*)raddr(ds,di+9);
    goto evt_commit_b;
memfill_dn:
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15); }
memfill_dn_loop:
    do {
        (bx)--; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        _sa = (bx);
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void evt_seek(void) {
    dd _sa = 0, _sb = 0;
    bx = di;
    cx = 0x0D;
    goto memfill_dn;
memfill_dn:
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15); }
memfill_dn_loop:
    do {
        (bx)--; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        _sa = (bx);
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void evt_read(void) {
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
        note_out();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto evt_maybe_rpt;
evt_maybe_dur:
        CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
        if ((al & 0x84) != 0) goto evt_loop_set;
evt_commit:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
evt_commit_b:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
evt_dur:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0; OF = 0; ZF = ((db)((al & 0x0B0)) == 0); SF = (((db)((al & 0x0B0))) >> 7);
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
        goto evt_loop_store;
    }
evt_maybe_att:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0A0)) == 0); SF = (((db)((al & 0x0A0))) >> 7);
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+4) = cl;
    }
evt_maybe_rel:
    CF = 0; OF = 0; ZF = ((db)((al & 0x90)) == 0); SF = (((db)((al & 0x90))) >> 7);
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+5) = cl;
    }
evt_loop_word:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
evt_loop_store:
    *(dw*)(raddr(ds,di+6)) = cx;
evt_maybe_rpt:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) == 0) goto evt_maybe_dur;
    cl = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
        if (*(raddr(ds,di+8)) != 0) goto evt_loop_back;
evt_loop_set:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
        *(dw*)(raddr(ds,di+9)) = cx;
        goto evt_commit;
    }
evt_rpt_set:
    *(db*)raddr(ds,di+8) = cl;
evt_loop_back:
    cx = *(dw*)raddr(ds,di+9);
    goto evt_commit_b;
}
void evt_maybe_dur(void) {
    CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
    if ((al & 0x84) == 0) {
evt_commit:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
evt_commit_b:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
evt_loop_set:
    cx = bx;
    (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
    *(dw*)(raddr(ds,di+9)) = cx;
    goto evt_commit;
}
void evt_commit(void) {
    cx = bx;
    (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
evt_commit_b:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void evt_commit_b(void) {
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void evt_dur(void) {
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0; OF = 0; ZF = ((db)((al & 0x0B0)) == 0); SF = (((db)((al & 0x0B0))) >> 7);
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
        goto evt_loop_store;
    }
evt_maybe_att:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0A0)) == 0); SF = (((db)((al & 0x0A0))) >> 7);
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+4) = cl;
    }
evt_maybe_rel:
    CF = 0; OF = 0; ZF = ((db)((al & 0x90)) == 0); SF = (((db)((al & 0x90))) >> 7);
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+5) = cl;
    }
evt_loop_word:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
evt_loop_store:
    *(dw*)(raddr(ds,di+6)) = cx;
evt_maybe_rpt:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
        if (*(raddr(ds,di+8)) != 0) {
            (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
            if (*(raddr(ds,di+8)) != 0) goto evt_loop_back;
evt_loop_set:
            cx = bx;
            (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
            *(dw*)(raddr(ds,di+9)) = cx;
            goto evt_commit;
        }
evt_rpt_set:
        *(db*)raddr(ds,di+8) = cl;
evt_loop_back:
        cx = *(dw*)raddr(ds,di+9);
        goto evt_commit_b;
    }
evt_maybe_dur:
    CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
    if ((al & 0x84) != 0) goto evt_loop_set;
evt_commit:
    cx = bx;
    (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
evt_commit_b:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void evt_maybe_att(void) {
    CF = 0; OF = 0; ZF = ((db)((al & 0x0A0)) == 0); SF = (((db)((al & 0x0A0))) >> 7);
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+4) = cl;
    }
evt_maybe_rel:
    CF = 0; OF = 0; ZF = ((db)((al & 0x90)) == 0); SF = (((db)((al & 0x90))) >> 7);
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+5) = cl;
    }
evt_loop_word:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
evt_loop_store:
    *(dw*)(raddr(ds,di+6)) = cx;
evt_maybe_rpt:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
        if (*(raddr(ds,di+8)) != 0) {
            (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
            if (*(raddr(ds,di+8)) != 0) goto evt_loop_back;
evt_loop_set:
            cx = bx;
            (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
            *(dw*)(raddr(ds,di+9)) = cx;
            goto evt_commit;
        }
evt_rpt_set:
        *(db*)raddr(ds,di+8) = cl;
evt_loop_back:
        cx = *(dw*)raddr(ds,di+9);
        goto evt_commit_b;
    }
evt_maybe_dur:
    CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
    if ((al & 0x84) != 0) goto evt_loop_set;
evt_commit:
    cx = bx;
    (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
evt_commit_b:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void evt_maybe_rel(void) {
    CF = 0; OF = 0; ZF = ((db)((al & 0x90)) == 0); SF = (((db)((al & 0x90))) >> 7);
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+5) = cl;
    }
evt_loop_word:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
evt_loop_store:
    *(dw*)(raddr(ds,di+6)) = cx;
evt_maybe_rpt:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
        if (*(raddr(ds,di+8)) != 0) {
            (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
            if (*(raddr(ds,di+8)) != 0) goto evt_loop_back;
evt_loop_set:
            cx = bx;
            (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
            *(dw*)(raddr(ds,di+9)) = cx;
            goto evt_commit;
        }
evt_rpt_set:
        *(db*)raddr(ds,di+8) = cl;
evt_loop_back:
        cx = *(dw*)raddr(ds,di+9);
        goto evt_commit_b;
    }
evt_maybe_dur:
    CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
    if ((al & 0x84) != 0) goto evt_loop_set;
evt_commit:
    cx = bx;
    (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
evt_commit_b:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void evt_loop_word(void) {
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
evt_loop_store:
    *(dw*)(raddr(ds,di+6)) = cx;
evt_maybe_rpt:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
        if (*(raddr(ds,di+8)) != 0) {
            (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
            if (*(raddr(ds,di+8)) != 0) goto evt_loop_back;
evt_loop_set:
            cx = bx;
            (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
            *(dw*)(raddr(ds,di+9)) = cx;
            goto evt_commit;
        }
evt_rpt_set:
        *(db*)raddr(ds,di+8) = cl;
evt_loop_back:
        cx = *(dw*)raddr(ds,di+9);
        goto evt_commit_b;
    }
evt_maybe_dur:
    CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
    if ((al & 0x84) != 0) goto evt_loop_set;
evt_commit:
    cx = bx;
    (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
evt_commit_b:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void evt_loop_store(void) {
    *(dw*)(raddr(ds,di+6)) = cx;
evt_maybe_rpt:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
        if (*(raddr(ds,di+8)) != 0) {
            (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
            if (*(raddr(ds,di+8)) != 0) goto evt_loop_back;
evt_loop_set:
            cx = bx;
            (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
            *(dw*)(raddr(ds,di+9)) = cx;
            goto evt_commit;
        }
evt_rpt_set:
        *(db*)raddr(ds,di+8) = cl;
evt_loop_back:
        cx = *(dw*)raddr(ds,di+9);
        goto evt_commit_b;
    }
evt_maybe_dur:
    CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
    if ((al & 0x84) != 0) goto evt_loop_set;
evt_commit:
    cx = bx;
    (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
evt_commit_b:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void evt_maybe_rpt(void) {
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
        if (*(raddr(ds,di+8)) != 0) {
            (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
            if (*(raddr(ds,di+8)) != 0) goto evt_loop_back;
evt_loop_set:
            cx = bx;
            (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
            *(dw*)(raddr(ds,di+9)) = cx;
            goto evt_commit;
        }
evt_rpt_set:
        *(db*)raddr(ds,di+8) = cl;
evt_loop_back:
        cx = *(dw*)raddr(ds,di+9);
        goto evt_commit_b;
    }
evt_maybe_dur:
    CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
    if ((al & 0x84) != 0) goto evt_loop_set;
evt_commit:
    cx = bx;
    (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
evt_commit_b:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void evt_loop_set(void) {
    cx = bx;
    (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
    *(dw*)(raddr(ds,di+9)) = cx;
    goto evt_commit;
evt_commit:
    cx = bx;
    (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
evt_commit_b:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void evt_rpt_set(void) {
    *(db*)raddr(ds,di+8) = cl;
evt_loop_back:
    cx = *(dw*)raddr(ds,di+9);
    goto evt_commit_b;
evt_commit_b:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void evt_loop_back(void) {
    cx = *(dw*)raddr(ds,di+9);
    goto evt_commit_b;
evt_commit_b:
    *(dw*)(raddr(ds,di+0x0B)) = cx;
    cl = *(db*)raddr(ds,bx);
    *(db*)raddr(ds,di) = cl;
locret_10604:
    return;
}
void chan3_update(void) {
    IF = 0;
    ax = 0x3C;
    voice_update();
    IF = 1;
locret_1069d:
    return;
}
void music_go(void) {
    *(dw*)(raddr(ds,0x63)) = bx;
    sfx_play(); return;
}
void sfx_pair_143(void) {
    sfx_play_pri();
    bx = 0x143;
    sfx_play(); return;
}
void sfx_pair_163(void) {
    sfx_play();
    bx = 0x163;
    sfx_play(); return;
}
void sfx_pair_165(void) {
    al = 0x28;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    sfx_play_pri();
    bx = 0x165;
    sfx_play(); return;
locret_107ba:
    return;
}
void sfx_pair_133(void) {
    sfx_play_pri();
    bx = 0x133;
    sfx_play(); return;
}
void sfx_pair_da(void) {
    sfx_play();
    bx = 0x0DA;
    sfx_play(); return;
}
void snd_reg_out(void) {
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
install_timer:
    *(raddr(ds,0x67)) = 0;
install_timer_2:
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

void module_init(void) {
    install_timer();
edummylabel1:
    return;
}
void install_timer(void) {
    dd _sa = 0, _sb = 0;
    *(raddr(ds,0x67)) = 0;
install_timer_2:
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
void snd_all_off(void) {
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
memfill_dn:
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15); }
memfill_dn_loop:
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
                stream_start(); return;
            }
        }
music_tick_restart:
        CF = (dd)*(raddr(ds,0x65)) < (dd)0; ZF = ((dw)((*(raddr(ds,0x65))) - (0)) == 0); SF = (((dw)((*(raddr(ds,0x65))) - (0))) >> 15);
        if (*(raddr(ds,0x65)) != 0) {
            CF = (dd)*(raddr(ds,0x66)) < (dd)0; ZF = ((dw)((*(raddr(ds,0x66))) - (0)) == 0); SF = (((dw)((*(raddr(ds,0x66))) - (0))) >> 15);
            if ((short)*(raddr(ds,0x66)) <= (short)0) {
                _sa = (*(raddr(ds,0x66)));
                *(raddr(ds,0x66)) = 0x1E;
                CF = (dd)*(raddr(ds,0x2F)) < (dd)0; ZF = ((dw)((*(raddr(ds,0x2F))) - (0)) == 0); SF = (((dw)((*(raddr(ds,0x2F))) - (0))) >> 15);
                if (*(raddr(ds,0x2F)) == 0) {
                    bx = 0x1B6;
                    stream_start(); return;
                }
            }
chans_tick_gate:
            (*(raddr(ds,0x66)))--; ZF = ((dw)(*(raddr(ds,0x66))) == 0); SF = (((dw)(*(raddr(ds,0x66)))) >> 15);
        }
    }
chans_tick:
    di = 0x2F;
    chan_tick();
    di = 0x3C;
    chan_tick();
    di = 0x49;
    chan_tick();
    di = 0x56;
chan_tick:
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
env_step:
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
note_out_tail:
        bx = *(dw*)raddr(ds,di+2);
        snd_reg_write(); return;
    }
chan_init:
    bx = *(dw*)raddr(ds,di+0x0B);
    al = *(db*)raddr(ds,bx);
    al |= al; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
    if (al == 0) {
note_out:
        bx = *(dw*)raddr(ds,di+2);
        bx |= bx; CF = 0; OF = 0; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        if (bx != 0) {
            bl |= 0x0F; CF = 0; OF = 0; ZF = ((db)(bl) == 0); SF = (((db)(bl)) >> 7);
            snd_reg_write();
        }
evt_seek:
        bx = di;
        cx = 0x0D;
        goto memfill_dn;
    }
evt_read:
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
        note_out();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto evt_maybe_rpt;
evt_maybe_dur:
        CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
        if ((al & 0x84) != 0) goto evt_loop_set;
evt_commit:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
evt_commit_b:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
evt_dur:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0; OF = 0; ZF = ((db)((al & 0x0B0)) == 0); SF = (((db)((al & 0x0B0))) >> 7);
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
        goto evt_loop_store;
    }
evt_maybe_att:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0A0)) == 0); SF = (((db)((al & 0x0A0))) >> 7);
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+4) = cl;
    }
evt_maybe_rel:
    CF = 0; OF = 0; ZF = ((db)((al & 0x90)) == 0); SF = (((db)((al & 0x90))) >> 7);
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+5) = cl;
    }
evt_loop_word:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
evt_loop_store:
    *(dw*)(raddr(ds,di+6)) = cx;
evt_maybe_rpt:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) == 0) goto evt_maybe_dur;
    cl = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
        if (*(raddr(ds,di+8)) != 0) goto evt_loop_back;
evt_loop_set:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
        *(dw*)(raddr(ds,di+9)) = cx;
        goto evt_commit;
    }
evt_rpt_set:
    *(db*)raddr(ds,di+8) = cl;
evt_loop_back:
    cx = *(dw*)raddr(ds,di+9);
    goto evt_commit_b;
memfill_dn:
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15); }
memfill_dn_loop:
    do {
        (bx)--; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        _sa = (bx);
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void chan_tick(void) {
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
env_step:
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
note_out_tail:
        bx = *(dw*)raddr(ds,di+2);
        snd_reg_write(); return;
    }
chan_init:
    bx = *(dw*)raddr(ds,di+0x0B);
    al = *(db*)raddr(ds,bx);
    al |= al; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
    if (al == 0) {
note_out:
        bx = *(dw*)raddr(ds,di+2);
        bx |= bx; CF = 0; OF = 0; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        if (bx != 0) {
            bl |= 0x0F; CF = 0; OF = 0; ZF = ((db)(bl) == 0); SF = (((db)(bl)) >> 7);
            snd_reg_write();
        }
evt_seek:
        bx = di;
        cx = 0x0D;
        goto memfill_dn;
    }
evt_read:
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
        note_out();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto evt_maybe_rpt;
evt_maybe_dur:
        CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
        if ((al & 0x84) != 0) goto evt_loop_set;
evt_commit:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
evt_commit_b:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
evt_dur:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0; OF = 0; ZF = ((db)((al & 0x0B0)) == 0); SF = (((db)((al & 0x0B0))) >> 7);
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
        goto evt_loop_store;
    }
evt_maybe_att:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0A0)) == 0); SF = (((db)((al & 0x0A0))) >> 7);
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+4) = cl;
    }
evt_maybe_rel:
    CF = 0; OF = 0; ZF = ((db)((al & 0x90)) == 0); SF = (((db)((al & 0x90))) >> 7);
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+5) = cl;
    }
evt_loop_word:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
evt_loop_store:
    *(dw*)(raddr(ds,di+6)) = cx;
evt_maybe_rpt:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) == 0) goto evt_maybe_dur;
    cl = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
        if (*(raddr(ds,di+8)) != 0) goto evt_loop_back;
evt_loop_set:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
        *(dw*)(raddr(ds,di+9)) = cx;
        goto evt_commit;
    }
evt_rpt_set:
    *(db*)raddr(ds,di+8) = cl;
evt_loop_back:
    cx = *(dw*)raddr(ds,di+9);
    goto evt_commit_b;
memfill_dn:
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15); }
memfill_dn_loop:
    do {
        (bx)--; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        _sa = (bx);
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void note_out(void) {
    dd _sa = 0, _sb = 0;
    bx = *(dw*)raddr(ds,di+2);
    bx |= bx; CF = 0; OF = 0; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    if (bx != 0) {
        bl |= 0x0F; CF = 0; OF = 0; ZF = ((db)(bl) == 0); SF = (((db)(bl)) >> 7);
        snd_reg_write();
    }
evt_seek:
    bx = di;
    cx = 0x0D;
    goto memfill_dn;
memfill_dn:
    ax = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
    { dd t_ = (dd)bx + (dd)cx; CF = t_ > 0xFFFF; bx = t_; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15); }
memfill_dn_loop:
    do {
        (bx)--; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        _sa = (bx);
        *(db*)raddr(ds,bx) = al;
    } while (--cx != 0);
locret_104c4:
    return;
}
void stream_start(void) {
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
voice_update:
    di = ax;
    push(bx);
    push(*(dw*)(raddr(ds,di)));
    note_out();
    *(dw*)(raddr(ds,di)) = pop();
    bx = pop();
    *(dw*)(raddr(ds,di+9)) = bx;
evt_read:
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
        note_out();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto evt_maybe_rpt;
evt_maybe_dur:
        CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
        if ((al & 0x84) != 0) goto evt_loop_set;
evt_commit:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
evt_commit_b:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
evt_dur:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0; OF = 0; ZF = ((db)((al & 0x0B0)) == 0); SF = (((db)((al & 0x0B0))) >> 7);
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
        goto evt_loop_store;
    }
evt_maybe_att:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0A0)) == 0); SF = (((db)((al & 0x0A0))) >> 7);
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+4) = cl;
    }
evt_maybe_rel:
    CF = 0; OF = 0; ZF = ((db)((al & 0x90)) == 0); SF = (((db)((al & 0x90))) >> 7);
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+5) = cl;
    }
evt_loop_word:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
evt_loop_store:
    *(dw*)(raddr(ds,di+6)) = cx;
evt_maybe_rpt:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) == 0) goto evt_maybe_dur;
    cl = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
        if (*(raddr(ds,di+8)) != 0) goto evt_loop_back;
evt_loop_set:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
        *(dw*)(raddr(ds,di+9)) = cx;
        goto evt_commit;
    }
evt_rpt_set:
    *(db*)raddr(ds,di+8) = cl;
evt_loop_back:
    cx = *(dw*)raddr(ds,di+9);
    goto evt_commit_b;
}
void voice_update(void) {
    dd _sa = 0, _sb = 0;
    di = ax;
    push(bx);
    push(*(dw*)(raddr(ds,di)));
    note_out();
    *(dw*)(raddr(ds,di)) = pop();
    bx = pop();
    *(dw*)(raddr(ds,di+9)) = bx;
evt_read:
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
        note_out();
        ax = pop();
        *(db*)raddr(ds,di+8) = al;
        ax = pop();
        *(dw*)(raddr(ds,di+9)) = ax;
        ax = pop();
        bx = pop();
        goto evt_maybe_rpt;
evt_maybe_dur:
        CF = 0; OF = 0; ZF = ((db)((al & 0x84)) == 0); SF = (((db)((al & 0x84))) >> 7);
        if ((al & 0x84) != 0) goto evt_loop_set;
evt_commit:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
evt_commit_b:
        *(dw*)(raddr(ds,di+0x0B)) = cx;
        cl = *(db*)raddr(ds,bx);
        *(db*)raddr(ds,di) = cl;
locret_10604:
        return;
    }
evt_dur:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    *(dw*)(raddr(ds,di+2)) = cx;
    CF = 0; OF = 0; ZF = ((db)((al & 0x0B0)) == 0); SF = (((db)((al & 0x0B0))) >> 7);
    if ((al & 0x0B0) == 0) {
        cx = 0; CF = 0; OF = 0; ZF = 1; SF = 0;
        goto evt_loop_store;
    }
evt_maybe_att:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0A0)) == 0); SF = (((db)((al & 0x0A0))) >> 7);
    if ((al & 0x0A0) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+4) = cl;
    }
evt_maybe_rel:
    CF = 0; OF = 0; ZF = ((db)((al & 0x90)) == 0); SF = (((db)((al & 0x90))) >> 7);
    if ((al & 0x90) != 0) {
        cl = *(db*)raddr(ds,bx);
        (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
        *(db*)raddr(ds,di+5) = cl;
    }
evt_loop_word:
    cx = *(dw*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
evt_loop_store:
    *(dw*)(raddr(ds,di+6)) = cx;
evt_maybe_rpt:
    CF = 0; OF = 0; ZF = ((db)((al & 0x0C0)) == 0); SF = (((db)((al & 0x0C0))) >> 7);
    if ((al & 0x0C0) == 0) goto evt_maybe_dur;
    cl = *(db*)raddr(ds,bx);
    (bx)++; ZF = ((dw)(bx) == 0); SF = (((dw)(bx)) >> 15);
    CF = (dd)*(raddr(ds,di+8)) < (dd)0; ZF = ((dw)((*(raddr(ds,di+8))) - (0)) == 0); SF = (((dw)((*(raddr(ds,di+8))) - (0))) >> 15);
    if (*(raddr(ds,di+8)) != 0) {
        (*(raddr(ds,di+8)))--; ZF = ((dw)(*(raddr(ds,di+8))) == 0); SF = (((dw)(*(raddr(ds,di+8)))) >> 15);
        if (*(raddr(ds,di+8)) != 0) goto evt_loop_back;
evt_loop_set:
        cx = bx;
        (cx)++; ZF = ((dw)(cx) == 0); SF = (((dw)(cx)) >> 15);
        *(dw*)(raddr(ds,di+9)) = cx;
        goto evt_commit;
    }
evt_rpt_set:
    *(db*)raddr(ds,di+8) = cl;
evt_loop_back:
    cx = *(dw*)raddr(ds,di+9);
    goto evt_commit_b;
}
void sfx_0f0(void) {
edummylabel14:
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x0F0;
    sfx_play_pri(); return;
locret_1069d:
    return;
}
void sfx_108_100(void) {
edummylabel15:
    al = 0x1E;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x108;
    sfx_play_pri();
    bx = 0x100;
    sfx_play(); return;
locret_1069d:
    return;
}
void sfx_09d(void) {
edummylabel16:
    bx = 0x9D;
    sfx_play(); return;
}
void sfx_198_1a7(void) {
edummylabel17:
    al = 0x64;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    bx = 0x198;
    if (al >= *(db*)raddr(ds,0x30)) {
        sfx_play_pri();
        bx = 0x1A7;
        sfx_play(); return;
    }
chan3_update:
    IF = 0;
    ax = 0x3C;
    voice_update();
    IF = 1;
locret_1069d:
    return;
}
void music_play_a(void) {
edummylabel18:
    bx = 0x1BB;
    goto music_go;
music_go:
    *(dw*)(raddr(ds,0x63)) = bx;
    sfx_play(); return;
}
void music_play_b(void) {
edummylabel19:
    bx = 0x1C3;
music_go:
    *(dw*)(raddr(ds,0x63)) = bx;
    sfx_play(); return;
}
void music_stop(void) {
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
void sfx_74(void) {
edummylabel21:
    *(dw*)(raddr(ds,0x63)) = 0;
    bx = 0x74;
    sfx_play(); return;
}
void sfx_72(void) {
edummylabel22:
    bx = 0x72;
    sfx_play(); return;
}
void seq_tempo_on(void) {
edummylabel23:
    *(raddr(ds,0x66)) = 0x1E;
    *(raddr(ds,0x65)) = 0x0FF;
    return;
}
void seq_tempo_off(void) {
edummylabel24:
    *(raddr(ds,0x65)) = 0;
locret_106e7:
    return;
}
void sfx_153_143(void) {
edummylabel25:
    al = 0x4B;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x153;
sfx_pair_143:
    sfx_play_pri();
    bx = 0x143;
    sfx_play(); return;
locret_106e7:
    return;
}
void sfx_08d(void) {
edummylabel26:
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x8D;
    sfx_play_pri(); return;
locret_106e7:
    return;
}
void sfx_11f(void) {
edummylabel27:
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x11F;
    sfx_play_pri(); return;
locret_106e7:
    return;
}
void sfx_15b_143(void) {
edummylabel28:
    al = 0x41;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x15B;
    goto sfx_pair_143;
sfx_pair_143:
    sfx_play_pri();
    bx = 0x143;
    sfx_play(); return;
}
void sfx_16a_163(void) {
edummylabel29:
    bx = 0x16A;
    goto sfx_pair_163;
sfx_pair_163:
    sfx_play();
    bx = 0x163;
    sfx_play(); return;
}
void sfx_174_163(void) {
edummylabel30:
    bx = 0x174;
sfx_pair_163:
    sfx_play();
    bx = 0x163;
    sfx_play(); return;
}
void sfx_16c_165(void) {
edummylabel31:
    bx = 0x16C;
    goto sfx_pair_165;
sfx_pair_165:
    al = 0x28;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    sfx_play_pri();
    bx = 0x165;
    sfx_play(); return;
locret_107ba:
    return;
}
void sfx_176_165(void) {
edummylabel32:
    bx = 0x176;
sfx_pair_165:
    al = 0x28;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    sfx_play_pri();
    bx = 0x165;
    sfx_play(); return;
locret_107ba:
    return;
}
void sfx_0a6(void) {
edummylabel33:
    al = 0x3C;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x0A6;
    sfx_play_pri(); return;
locret_107ba:
    return;
}
void sfx_187_17e(void) {
edummylabel34:
    al = 0x63;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x187;
    sfx_play_pri();
    bx = 0x17E;
    sfx_play(); return;
locret_107ba:
    return;
}
void sfx_0f8_133(void) {
edummylabel35:
    al = 0x46;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x0F8;
    goto sfx_pair_133;
sfx_pair_133:
    sfx_play_pri();
    bx = 0x133;
    sfx_play(); return;
locret_107ba:
    return;
}
void sfx_13b_133(void) {
edummylabel36:
    al = 0x5F;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x13B;
sfx_pair_133:
    sfx_play_pri();
    bx = 0x133;
    sfx_play(); return;
locret_107ba:
    return;
}
void sfx_126(void) {
edummylabel37:
    al = 0x2E;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x126;
sfx_play_pri:
    *(db*)raddr(ds,0x30) = al;
sfx_play:
    IF = 0;
    stream_start();
    IF = 1;
locret_107ba:
    return;
}
void sfx_play_pri(void) {
    *(db*)raddr(ds,0x30) = al;
sfx_play:
    IF = 0;
    stream_start();
    IF = 1;
locret_107ba:
    return;
}
void sfx_play(void) {
    IF = 0;
    stream_start();
    IF = 1;
locret_107ba:
    return;
}
void sfx_12e(void) {
edummylabel38:
    al = 0x2D;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x12E;
    sfx_play_pri(); return;
locret_107ba:
    return;
}
void sfx_190(void) {
edummylabel39:
    al = 0x55;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x190;
    sfx_play_pri(); return;
locret_107ba:
    return;
}
void sfx_117(void) {
edummylabel40:
    al = 0x32;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x117;
    sfx_play_pri(); return;
locret_107ba:
    return;
}
void sfx_14b_153(void) {
edummylabel41:
    al = 0x50;
    CF = (dd)al < (dd)*(db*)raddr(ds,0x30); ZF = ((db)((al) - (*(db*)raddr(ds,0x30))) == 0); SF = (((db)((al) - (*(db*)raddr(ds,0x30)))) >> 7);
    if (al < *(db*)raddr(ds,0x30)) { return; }
    bx = 0x14B;
    sfx_play_pri();
    bx = 0x153;
    sfx_play(); return;
locret_107ba:
    return;
}
void sfx_pair_ae_da(void) {
edummylabel42:
    bx = 0x0AE;
    goto sfx_pair_da;
sfx_pair_da:
    sfx_play();
    bx = 0x0DA;
    sfx_play(); return;
}
void sfx_pair_c4_da(void) {
edummylabel43:
    bx = 0x0C4;
sfx_pair_da:
    sfx_play();
    bx = 0x0DA;
    sfx_play(); return;
}
void sfx_0e1(void) {
edummylabel44:
    bx = 0x0E1;
    sfx_play(); return;
}
void sfx_1d0(void) {
edummylabel45:
    bx = 0x1D0;
    sfx_play(); return;
}
void sfx_1cb(void) {
edummylabel46:
    bx = 0x1CB;
    sfx_play(); return;
}
void sfx_06a(void) {
edummylabel47:
    bx = 0x6A;
    sfx_play(); return;
}
void sfx_110_84(void) {
edummylabel48:
    bx = 0x110;
    sfx_play();
    bx = 0x84;
    sfx_play(); return;
}
void sfx3_1ef_23d(void) {
edummylabel49:
    bx = 0x1EF;
    sfx_play();
    *(dw*)(raddr(ds,0x38)) = 0x1DF;
    bx = 0x23D;
    sfx_play();
    bx = 0x235;
    *(dw*)(raddr(ds,0x45)) = bx;
sfx3_26b_2fd:
    bx = 0x28B;
    sfx_play();
    *(dw*)(raddr(ds,0x52)) = 0x26B;
    bx = 0x2FD;
    sfx_play(); return;
}
void sfx3_26b_2fd(void) {
    bx = 0x28B;
    sfx_play();
    *(dw*)(raddr(ds,0x52)) = 0x26B;
    bx = 0x2FD;
    sfx_play(); return;
}
void sfx3_210_246(void) {
edummylabel50:
    sfx3_1ef_23d();
    *(dw*)(raddr(ds,0x38)) = 0x210;
    *(dw*)(raddr(ds,0x45)) = 0x246;
    bx = 0x2AC;
    *(dw*)(raddr(ds,0x52)) = bx;
    bl = 0x18;
    *(db*)raddr(ds,0x5E) = bl;
    return;
}
void snd_reg_write(void) {
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
snd_reg_out:
    al = bh;
    al |= 0x0E0; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
    out(0x0C0, al);
    _sa = (al);
    al = bl;
    al |= 0x0F0; CF = 0; OF = 0; ZF = ((db)(al) == 0); SF = (((db)(al)) >> 7);
    out(0x0C0, al);
    return;
}