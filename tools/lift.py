#!/usr/bin/env python3
"""Lift masm2c fake-C into readable decompiled-style C.

Parses the decomp sources (build_ida/src/) into labeled basic blocks,
folds flag ops into branch conditions, names DOS/BIOS interrupt calls,
and emits one C function per proc entry point (goto-closure over shared
loc_ tails, same as a real decompiler produces). Original asm is kept as
comments alongside each lifted statement.
"""
import re, os, glob

SRC = "/home/xor/games/airborn/build_ida/src"
OUT = "/home/xor/games/airborn/lifted"

UNSIGNED = {'JC':'<','JB':'<','JNAE':'<','JNC':'>=','JAE':'>=','JNB':'>=',
            'JA':'>','JNBE':'>','JBE':'<=','JNA':'<='}
SIGNED   = {'JL':'<','JNGE':'<','JGE':'>=','JNL':'>=','JG':'>','JNLE':'>','JLE':'<=','JNG':'<='}
EQUAL    = {'JZ':'==','JE':'==','JNZ':'!=','JNE':'!='}
BYTEREGS = {'al','ah','bl','bh','cl','ch','dl','dh'}
WORDREGS = {'ax','bx','cx','dx','si','di','bp','sp','cs','ds','es','ss'}

def opwidth(e):
    if e in BYTEREGS or e.startswith('byte_') or 'db*' in e: return 'b'
    return 'w'

def signedcast(e, w):
    return ('(signed char)' if w == 'b' else '(short)') + e
FLAGREAD = {'JC':'CF','JB':'CF','JNAE':'CF','JNC':'!CF','JAE':'!CF','JNB':'!CF',
            'JZ':'ZF','JE':'ZF','JNZ':'!ZF','JNE':'!ZF','JS':'SF','JNS':'!SF',
            'JO':'OF','JNO':'!OF','JP':'PF','JPE':'PF','JNP':'!PF','JPO':'!PF',
            'JA':'(!CF && !ZF)','JNBE':'(!CF && !ZF)','JBE':'(CF || ZF)','JNA':'(CF || ZF)',
            'JL':'(SF != OF)','JNGE':'(SF != OF)','JGE':'(SF == OF)','JNL':'(SF == OF)',
            'JG':'(!ZF && SF == OF)','JNLE':'(!ZF && SF == OF)','JLE':'(ZF || SF != OF)','JNG':'(ZF || SF != OF)'}

DOS21 = {0x00:'dos_exit',0x01:'dos_read_char_echo',0x02:'dos_write_char',0x06:'dos_direct_io',
    0x07:'dos_read_char_noecho',0x08:'dos_read_char_noecho',0x09:'dos_print_string',
    0x0A:'dos_read_string',0x0B:'dos_check_stdin',0x19:'dos_get_drive',0x1A:'dos_set_dta',
    0x25:'dos_set_int_vector',0x2A:'dos_get_date',0x2C:'dos_get_time',0x30:'dos_get_version',
    0x35:'dos_get_int_vector',0x3D:'dos_open',0x3E:'dos_close',0x3F:'dos_read',
    0x40:'dos_write',0x41:'dos_delete',0x42:'dos_seek',0x43:'dos_get_attr',0x44:'dos_ioctl',
    0x48:'dos_alloc',0x49:'dos_free',0x4A:'dos_resize',0x4B:'dos_exec',0x4C:'dos_exit_code',
    0x4E:'dos_find_first',0x4F:'dos_find_next',0x54:'dos_get_verify',0x56:'dos_rename',
    0x57:'dos_file_datetime',0x5A:'dos_create_temp',0x5B:'dos_create_new',0x62:'dos_get_psp'}
BIOS10 = {0x00:'bios_set_mode',0x02:'bios_set_cursor',0x06:'bios_scroll',0x0E:'bios_putc',0x10:'bios_palette'}
BIOS16 = {0x00:'bios_getch',0x01:'bios_kbhit'}

class _FakeMatch:
    def __init__(self, op, args): self._g = (op, args)
    def group(self, n): return self._g[n - 1]

def split_args(s):
    out, depth, cur = [], 0, ''
    for c in s:
        if c in '([{': depth += 1
        if c in ')]}': depth -= 1
        if c == ',' and depth == 0: out.append(cur); cur = ''
        else: cur += c
    out.append(cur)
    return [a.strip() for a in out if a.strip() != '']

class Lifter:
    def __init__(self):
        self.pending = None   # (a,b,kind) flag producer awaiting fold; kind: cmp|result
        self.ah = None        # last literal -> ah
        self.goto_targets = []  # labels referenced by goto/if-goto (for closure)
        self.disp = None      # last __disp = EXPR (indirect call/jump operand)
        self.segbase = 0x1a20 # mem base of the segment this function runs in
        self.fnmem = 0        # mem addr of this function (for cs:eip+1 far operands)

    def flagval(self, jcc):
        w = self.pending[3] if (self.pending is not None and len(self.pending) > 3) else 'w'
        if jcc in ('JS','JNS'):
            if self.pending is not None:
                a = self.pending[0]
                c = '(signed char)' if w == 'b' else '(short)'
                if self.pending[2] == 'cmp' and self.pending[1] not in ('0', '0x0', '0x00'):
                    return f"{c}(({a})-({self.pending[1]})) < 0" if jcc == 'JS' else f"{c}(({a})-({self.pending[1]})) >= 0"
                return f"{c}({a}) < 0" if jcc == 'JS' else f"{c}({a}) >= 0"
            return 'SF' if jcc == 'JS' else '!SF'
        if jcc in ('JO','JNO','JP','JNP','JPE','JPO'):
            return FLAGREAD.get(jcc, '0')
        if self.pending is None:
            return FLAGREAD.get(jcc, '0')
        a, b, kind = self.pending[:3]
        if kind == 'cmp':
            if jcc in UNSIGNED: return f"{a} {UNSIGNED[jcc]} {b}"
            if jcc in EQUAL:    return f"{a} {EQUAL[jcc]} {b}"
            if jcc in SIGNED:
                ea = a if a.startswith('(') else signedcast(a, w)
                return f"{ea} {SIGNED[jcc]} {signedcast(b, w)}"
        else:  # result of last op (stored) — ZF/SF folds; CF/OF read flag globals
            if jcc in EQUAL:    return f"{a} {EQUAL[jcc]} 0"
            if jcc in UNSIGNED or jcc in ('JA','JBE','JNBE','JNA','JA_'):
                return FLAGREAD.get(jcc, '0')
            if jcc in SIGNED:   return f"{signedcast(a, w)} {SIGNED[jcc]} 0"
        return FLAGREAD.get(jcc, '0')

    def zsf(self, res, w):
        """ZF/SF materialization for a flag-producing op's result. Flags live in
        globals so they survive call boundaries (asm returns flags via retn)."""
        T = 'db' if w == 'b' else 'dw'
        s = 7 if w == 'b' else 15
        return f"ZF = (({T})({res}) == 0); SF = ((({T})({res})) >> {s});"

    def op(self, inner, asm=''):
        inner = inner.strip()
        out = []
        self._width = 'db' if 'byte ptr' in asm else ('dw' if 'word ptr' in asm else None)
        if self._width is None:
            if re.search(r'\b(b[lx]|c[lx]|d[lx]|al|ah)\b\s*,\s*byte', asm): self._width = 'db'
        m = re.match(r'^(_?[A-Z][A-Z_0-9]*)\((.*)\)\s*;?\s*$', inner, re.S)
        if not m and re.match(r'^[A-Z_][A-Z_0-9 ]*;?$', inner):
            # bare op, possibly REP-prefixed string op
            words = inner.rstrip(';').split()
            rep = ''
            if words[0].startswith('REP'): rep, words = words[0] + ' ', words[1:]
            m = _FakeMatch(words[0], '')
            self._rep = rep
        else: self._rep = ''
        if not m:
            s = inner.rstrip(';').strip()
            if re.match(r'^(ZF|SF|CF|OF|PF|AF)\s*=', s):
                return out  # flag stores absorbed into pending/ignored
            m2 = re.match(r'^(ah|ax)\s*=\s*(0x[0-9a-fA-F]+|\d+)$', s)
            if m2:
                v = int(m2.group(2), 0)
                self.ah = (v & 0xFF) if m2.group(1) == 'ah' else (v >> 8 & 0xFF)
            elif re.match(r'^(ah|ax)\s*=', s): self.ah = None
            out.append(s + ';'); return out
        op, args = m.group(1).lstrip('_'), split_args(m.group(2))
        BYTEREG = {'al','ah','bl','bh','cl','ch','dl','dh'}
        WORDREG = {'ax','bx','cx','dx','si','di','bp','sp','cs','ds','es','ss'}
        def widthcast(e, reg):
            t = 'db' if reg in BYTEREG else ('dw' if reg in WORDREG else self._width)
            if t is None or 'raddr' not in e: return e
            return re.sub(r'\*\((?:db|dw)\*\)?\(raddr_?\((.*?)\)\)|\*\(raddr_?\((.*?)\)\)',
                          lambda mm: f"*({t}*)raddr({mm.group(1) or mm.group(2)})", e)
        def wmask(e):
            if e in BYTEREG or 'db*' in e: return '0xFF'
            if e in WORDREG or 'dw*' in e: return '0xFFFF'
            return '0xFF' if self._width == 'db' else '0xFFFF'
        if op == 'MOV' and len(args) == 2:
            src = widthcast(args[1], args[0])
            if re.match(r'^\*\(raddr', args[0]):
                dst = widthcast(args[0], args[1])
            else: dst = args[0]
            out.append(f"{dst} = {src};")
            a0 = args[0]
            if a0 == 'ah' and re.match(r'^(0x|\d)', args[1]): self.ah = int(args[1], 0)
            elif a0 == 'ax' and re.match(r'^(0x|\d)', args[1]): self.ah = int(args[1], 0) >> 8 & 0xFF
            elif a0 in ('ax','ah'): self.ah = None
        elif op == 'CMP' and len(args) == 2:
            a0 = widthcast(args[0], args[0])
            b = widthcast(args[1], args[0])
            w_ = opwidth(a0)
            self.pending = (a0, b, 'cmp', w_)
            out.append(f"CF = (dd){a0} < (dd){b}; " + self.zsf(f"({a0}) - ({b})", w_))
        elif op == 'TEST' and len(args) == 2:
            a0 = widthcast(args[0], args[0])
            if args[0] == args[1]:
                self.pending = (a0, '0', 'result', opwidth(a0)); res = a0
            else:
                res = f"({a0} & {widthcast(args[1], args[0])})"
                self.pending = (res, '0', 'result', opwidth(args[0]))
            out.append("CF = 0; OF = 0; " + self.zsf(res, opwidth(args[0])))
        elif op in ('ADD','SUB','AND','OR','XOR','ADC','SBB'):
            sym = {'ADD':'+','SUB':'-','AND':'&','OR':'|','XOR':'^','ADC':'+','SBB':'-'}[op]
            a0 = widthcast(args[0], args[0])
            b = widthcast(args[1], args[0])
            M = wmask(args[0])
            if op == 'XOR' and args[0] == args[1]:
                self.pending = ('0', '0', 'result', 'w')
                out.append(f"{a0} = 0; CF = 0; OF = 0; ZF = 1; SF = 0;")
            elif op == 'ADD':
                self.pending = (a0, '0', 'result', opwidth(a0))
                out.append(f"{{ dd t_ = (dd){a0} + (dd){b}; CF = t_ > {M}; {a0} = t_; " + self.zsf(a0, opwidth(a0)) + " }")
            elif op == 'ADC':
                self.pending = (a0, '0', 'result', opwidth(a0))
                out.append(f"{{ dd t_ = (dd){a0} + (dd){b} + CF; CF = t_ > {M}; {a0} = t_; " + self.zsf(a0, opwidth(a0)) + " }")
            elif op == 'SUB':
                self.pending = (a0, '0', 'result', opwidth(a0))
                out.append(f"{{ dd t_ = (dd){a0} - (dd){b}; CF = (dd){a0} < (dd){b}; {a0} = t_; " + self.zsf(a0, opwidth(a0)) + " }")
            elif op == 'SBB':
                self.pending = (a0, '0', 'result', opwidth(a0))
                out.append(f"{{ dd t_ = (dd){a0} - (dd){b} - CF; CF = (dd){a0} < (dd){b} + CF; {a0} = t_; " + self.zsf(a0, opwidth(a0)) + " }")
            else:
                self.pending = (a0, '0', 'result', opwidth(a0))   # flags reflect stored result
                out.append(f"{a0} {sym}= {b}; CF = 0; OF = 0; " + self.zsf(a0, opwidth(a0)))
        elif op in ('INC','DEC'):
            a0 = widthcast(args[0], args[0])
            d = '+' if op=='INC' else '-'
            self.pending = (a0, '0', 'result', opwidth(a0))
            out.append(f"({a0}){d}{d}; " + self.zsf(a0, opwidth(a0)))
        elif op == 'NEG':
            a0 = widthcast(args[0], args[0])
            self.pending = (a0, '0', 'result', opwidth(a0))
            out.append(f"{a0} = -({a0}); CF = ({a0} != 0); " + self.zsf(a0, opwidth(a0)))
        elif op == 'NOT':
            a0 = widthcast(args[0], args[0])
            out.append(f"{a0} = ~{a0};")
        elif op in ('SHL','SHR','SAR'):
            a0 = widthcast(args[0], args[0])
            sym = '<<' if op=='SHL' else '>>'
            cast = signedcast('', 'b' if (args[0] in BYTEREGS or wmask(args[0]) == '0xFF') else 'w') if op=='SAR' else ''
            bits = '8' if wmask(args[0]) == '0xFF' else '16'
            self.pending = (a0, '0', 'result', opwidth(a0))
            n = args[1]
            if op == 'SHL':
                out.append(f"{{ if ({n}) {{ CF = (((dd){a0} << ({n})) >> {bits}) & 1; {a0} <<= {n}; " + self.zsf(a0, opwidth(a0)) + " } }}")
            else:
                out.append(f"{{ if ({n}) {{ CF = ({a0} >> (({n})-1)) & 1; {a0} = {cast}{a0} >> {n}; " + self.zsf(a0, opwidth(a0)) + " } }}")
        elif op in ('ROL','ROR','RCL','RCR'):
            a0 = widthcast(args[0], 'ax')  # rotates are word ops here
            out.append(f"{a0} = {op.lower()}16({a0}, {args[1]});")
        elif op == 'CLC': out.append("CF = 0;")
        elif op == 'STC': out.append("CF = 1;")
        elif op == 'CMC': out.append("CF = !CF;")
        elif op in ('CLD','STD'): out.append(f"DF = {0 if op=='CLD' else 1};")
        elif op in ('CLI','STI'): out.append(f"IF = {0 if op=='CLI' else 1};")
        elif op == 'PUSH': out.append(f"push({args[0]});")
        elif op in ('PUSHF','PUSHF16'): out.append("pushf();")
        elif op == 'POP': out.append(f"{args[0]} = pop();")
        elif op in ('POPF','POPF16'): out.append("popf();")
        elif op == 'XCHG': out.append(f"swap({args[0]}, {args[1]});")
        elif op == 'CBW': out.append("ax = (char)al;")
        elif op == 'CWD': out.append("dx = (short)ax < 0 ? 0xFFFF : 0;")
        # string ops honor DF (std = backward); si/di are 16-bit so -2 wraps like hw
        elif op == 'MOVSW': out.append(self._rep + "*(dw*)raddr_(es,di) = *(dw*)raddr_(ds,si); si += DF?-2:2; di += DF?-2:2;" if not self._rep else "while (cx--) { *(dw*)raddr_(es,di) = *(dw*)raddr_(ds,si); si += DF?-2:2; di += DF?-2:2; }")
        elif op == 'MOVSB': out.append("while (cx--) { *(db*)raddr_(es,di) = *(db*)raddr_(ds,si); si += DF?-1:1; di += DF?-1:1; }" if self._rep else "*(db*)raddr_(es,di) = *(db*)raddr_(ds,si); si += DF?-1:1; di += DF?-1:1;")
        elif op == 'LODSW': out.append("while (cx--) { ax = *(dw*)raddr_(ds,si); si += DF?-2:2; }" if self._rep else "ax = *(dw*)raddr_(ds,si); si += DF?-2:2;")
        elif op == 'LODSB': out.append("while (cx--) { al = *(db*)raddr_(ds,si); si += DF?-1:1; }" if self._rep else "al = *(db*)raddr_(ds,si); si += DF?-1:1;")
        elif op == 'STOSW': out.append("while (cx--) { *(dw*)raddr_(es,di) = ax; di += DF?-2:2; }" if self._rep else "*(dw*)raddr_(es,di) = ax; di += DF?-2:2;")
        elif op == 'STOSB': out.append("while (cx--) { *(db*)raddr_(es,di) = al; di += DF?-1:1; }" if self._rep else "*(db*)raddr_(es,di) = al; di += DF?-1:1;")
        elif op == 'CMPSW': self.pending=('ax','*(dw*)raddr_(es,di)','cmp','w'); out.append("si += DF?-2:2; di += DF?-2:2;")
        elif op == 'CMPSB': self.pending=('al','*(db*)raddr_(es,di)','cmp','b'); out.append("si += DF?-1:1; di += DF?-1:1;")
        elif op == 'SCASW': self.pending=('ax','*(dw*)raddr_(es,di)','cmp','w'); out.append("di += DF?-2:2;")
        elif op == 'SCASB': self.pending=('al','*(db*)raddr_(es,di)','cmp','b'); out.append("di += DF?-1:1;")
        elif op == 'MUL': out.append(f"{{unsigned long r = (unsigned long)ax * {args[0]}; ax = r; dx = r >> 16;}}")
        elif op == 'IMUL': out.append(f"{{long r = (long)(short)ax * (short){args[0]}; ax = r; dx = r >> 16;}}")
        elif op == 'DIV': out.append(f"{{unsigned long n = ((unsigned long)dx<<16)|ax; ax = n / {args[0]}; dx = n % {args[0]};}}")
        elif op == 'IDIV': out.append(f"{{long n = ((long)dx<<16)|ax; ax = n / (short){args[0]}; dx = n % (short){args[0]};}}")
        elif op in ('MUL1_1',): out.append(f"ax = (dw)al * {args[0]};")
        elif op in ('MUL1_2',): out.append(f"{{dd r = (dd)ax * {args[0]}; ax = r; dx = r >> 16;}}")
        elif op in ('IMUL1_1',): out.append(f"ax = (dw)((signed char)al * (signed char){args[0]});")
        elif op in ('IMUL1_2',): out.append(f"{{long r = (long)(short)ax * (short){args[0]}; ax = r; dx = r >> 16;}}")
        elif op in ('DIV1_1',): out.append(f"{{dw n = ax; al = n / {args[0]}; ah = n % {args[0]};}}")
        elif op in ('DIV1_2',): out.append(f"{{unsigned long n = ((unsigned long)dx<<16)|ax; ax = n / {args[0]}; dx = n % {args[0]};}}")
        elif op in ('IDIV1_1',): out.append(f"{{short n = (short)ax; al = n / (signed char){args[0]}; ah = n % (signed char){args[0]};}}")
        elif op in ('IDIV1_2',): out.append(f"{{long n = ((long)dx<<16)|ax; ax = n / (short){args[0]}; dx = n % (short){args[0]};}}")
        elif op == 'XLAT': out.append("al = *(db*)raddr_(ds, bx + al);")
        elif op == 'NOP': pass
        elif op == 'LEA': out.append(f"{args[0]} = &{args[1]};")
        elif op in ('LES','LDS'):
            seg = 'es' if op=='LES' else 'ds'
            out.append(f"{args[0]} = *(dw*)raddr_({seg if 0 else 'ds'},{args[1]}); {seg} = *(dw*)raddr_(ds,{args[1]}+2);")
        elif op == 'IN': out.append(f"{args[0]} = in({args[1]});")
        elif op == 'OUT': out.append(f"out({args[0]}, {args[1]});")
        elif op == 'INT' or op == '_INT':
            num = args[0]
            if num in ('0x21','0X21'):
                fn = DOS21.get(self.ah)
                out.append(f"{fn}();" if fn else "dos_int21(ax);")
            elif num in ('0x10','0X10'):
                fn = BIOS10.get(self.ah)
                out.append(f"{fn}();" if fn else "bios_video(ax);")
            elif num in ('0x16','0X16'):
                fn = BIOS16.get(self.ah)
                out.append(f"{fn}();" if fn else "bios_kbd(ax);")
            elif num in ('0x1A','0X1A'): out.append("bios_time();")
            else: out.append(f"swi({num});")
        elif op.startswith('REP'): pass  # prefix: folded into string op by codegen already
        else: out.append(f"/* {op}({', '.join(args)}); */")
        return out

    def jump(self, inner, asm=''):
        inner = inner.strip().rstrip(';')
        out = []
        if inner.startswith('return'):
            if '__dispatch_call_ext' in inner or '__dispatch_call' in inner:
                t = disp_target(self, self.disp, asm) if self.disp else None
                self.disp = None
                if t:
                    return [f"{{ vfn f_ = func_at({t}); if (f_) f_(); else fprintf(stderr, \"unresolved ind jmp %x\\n\", (dd)({t})); return; }}"]
                return ["indirect_jump(); return;"]
            # mid-entry tail jumps: return sub_X(m2c::kloc_Y, _state) → loc_Y(); or return sub_X(0,_state) → sub_X()
            tm = re.match(r'^return\s+(\w+)\((?:m2c::k?(\w+)|0),', inner)
            if tm:
                fn, t = tm.group(1), tm.group(2)
                t = t if t else fn
                return [f"{t}(); return;"] if t != '_begin' else ["return;"]
            return [inner + ';']
        mm = re.match(r'^(\w+)\((.*)\)$', inner, re.S)
        if not mm:
            if inner in ('IRET','RETN','RETF','RET'):
                self.pending = None
                return ["return;"]
            return [f"/* {inner} */"]
        op, args = mm.group(1), split_args(mm.group(2))
        if op in ('CALL','CALLF','CALLI','CALLFI'):
            self.pending = None
            if args and args[0] == '__dispatch_call_ext':
                e = args[1] if len(args) > 1 else ''
                if e == 'start':
                    out.append("start();")
                else:
                    t = disp_target(self, e, asm)
                    if t:
                        out.append(f"{{ vfn f_ = func_at({t}); if (f_) f_(); else fprintf(stderr, \"unresolved ind call %x\\n\", (dd)({t})); }}")
                    else:
                        out.append("__dispatch_call_ext();")
            else:
                out.append(f"{args[0]}();")
        elif op in ('RETN','RETF','RET'):
            n = args[0] if args else '0'
            pre = f"sp += {n}; " if re.match(r'^[1-9]', str(n)) else ''
            self.pending = None
            out.append(f"{pre}return;")
        elif op == 'IRET': self.pending=None; out.append("return;")
        elif op == 'JMP':
            self.pending=None
            tgt = args[0]
            if tgt.startswith('sub_') or tgt == 'start':
                out.append(f"{tgt}(); return;")  # tail-call to proc entry
            else:
                out.append(f"goto {tgt};")
        elif op == 'LOOP':
            self.goto_targets.append(args[0]); self.pending=None
            out.append(f"if (--cx != 0) goto {args[0]};")
        elif op in ('LOOPE','LOOPZ'):
            self.goto_targets.append(args[0]); self.pending=None
            out.append(f"if (--cx != 0 && ZF) goto {args[0]};")
        elif op in ('LOOPNE','LOOPNZ'):
            self.goto_targets.append(args[0]); self.pending=None
            out.append(f"if (--cx != 0 && !ZF) goto {args[0]};")
        elif op.startswith('J'):
            c = self.flagval(op)   # keep pending: consecutive Jcc share flags
            tgt = args[0]
            if tgt.startswith('sub_') or tgt == 'start':
                out.append(f"if ({c}) {{ {tgt}(); return; }}")
            else:
                out.append(f"if ({c}) goto {tgt};")
        else: out.append(f"/* {inner} */")
        return out

RE_FUNC = re.compile(r'^\s*(?:static\s+|__attribute__\(\(weak\)\)\s+)*bool\s+(\w+)\s*\(m2c::_offsets\s+_i,\s*struct\s+m2c::_STATE\*\s*_state\)\s*\{')
RE_LABEL = re.compile(r'^\s*(\w+):\s*$')
RE_R = re.compile(r'^\s*R\((.*)\)\s*;\s*(//.*)?$')
RE_J = re.compile(r'^\s*J\((.*)\)\s*;\s*(//.*)?$')
RE_ASM = re.compile(r'//\s*\d+\s+(.*?);~\s*([0-9A-Fa-f]+):([0-9A-Fa-f]+)')
RE_CASE = re.compile(r'case\s+m2c::k(\w+):\s*goto\s+(\w+)')

SKIP_PREFIX = ('X86_REGREF','__disp','if (__disp','else goto','__dispatch_call',
               'switch','default:','assert','S_(','{','}','DD tt;','struct m2c')

def _code_part(ln):
    """Line prefix before any `//` comment, honoring char/string literals."""
    out = []; i = 0; n = len(ln); sq = dq = False
    while i < n:
        c = ln[i]
        if sq or dq:
            if c == '\\': i += 2; continue
            if c == "'" and sq: sq = False
            elif c == '"' and dq: dq = False
        elif c == "'": sq = True
        elif c == '"': dq = True
        elif c == '/' and i + 1 < n and ln[i + 1] == '/':
            break
        out.append(c); i += 1
    return ''.join(out)

def parse_func(lines, i):
    """Parse one function body → (blocks, entries). blocks: [(label,[(stmt,asm)])], entries: [labels]"""
    blocks, cur, entries, aliases = [], None, [], {}
    depth = 1; i += 1; n = len(lines)
    pending_asm = []
    while i < n and depth > 0:
        ln = lines[i]; i += 1
        if not ln.lstrip().startswith('//'):
            # brace depth on code only: trailing `//` asm comments may quote
            # braces (e.g. `mov ax, 7Dh ; '}'`), which would corrupt depth
            depth += _code_part(ln).count('{') - _code_part(ln).count('}')
        s = ln.strip()
        if s.startswith('case '):
            cm = RE_CASE.search(ln)
            if cm and cm.group(1) == cm.group(2): entries.append(cm.group(2))
            continue
        dm = re.match(r'^__disp\s*=\s*(.*?);\s*$', s)
        if dm:
            e = dm.group(1)
            if e not in ('_i', '__i') and '__disp' not in e:
                if cur is None: cur = []; blocks.append(('_entry', cur))
                cur.append(('DISP', e, ''))
            continue
        if s.startswith(SKIP_PREFIX) or not s: continue
        lm = RE_LABEL.match(ln)
        if lm:
            lbl = lm.group(1)
            if lbl.startswith('__dispatch') or lbl == '_begin': continue
            if cur is not None and not cur:
                # empty previous block: labels are adjacent — alias to new name
                aliases[blocks[-1][0]] = lbl
                blocks[-1] = (lbl, blocks[-1][1])
            else:
                cur = []
                blocks.append((lbl, cur))
            continue
        rm = RE_R.match(ln)
        if rm:
            am = RE_ASM.search(ln)
            asm = (am.group(1).strip() + ' ;~ ' + am.group(2) + ':' + am.group(3)) if am else ''
            if cur is None: cur = []; blocks.append(('_entry', cur))
            cur.append(('R', rm.group(1), asm))
            continue
        jm = RE_J.match(ln)
        if jm:
            am = RE_ASM.search(ln)
            asm = (am.group(1).strip() + ' ;~ ' + am.group(2) + ':' + am.group(3)) if am else ''
            if cur is None: cur = []; blocks.append(('_entry', cur))
            cur.append(('J', jm.group(1), asm))
            continue
        am = RE_ASM.search(ln)
        if am and cur is not None and not cur:
            pass  # orphan comment w/o op; attach to nothing
        if am: pending_asm.append(am.group(1).strip() + ' ;~ ' + am.group(2) + ':' + am.group(3))
    return blocks, entries, aliases, i

def render_tiny_block(items):
    """Render a block with a fresh Lifter; return stmt list or None if not 'trivial'."""
    T = Lifter()
    stmts = []
    for kind, txt, asm in items:
        if kind == 'DISP': continue
        stmts += T.op(txt) if kind == 'R' else T.jump(txt, asm)
    return stmts

def is_tiny_target(stmts):
    """Block is a plain tail: 'return;' or 'f(); return;'."""
    if not stmts: return False
    if stmts == ['return;']: return True
    if len(stmts) == 2 and re.match(r'^\w+\(\);$', stmts[0]) and stmts[1] == 'return;':
        return True
    return False

def disp_target(L, e, asm=''):
    """__disp operand -> mem addr expr for func_at. Near targets are offsets in
    the jump site's code segment (from the ;~ SEG:OFF asm comment); far targets
    are seg:off pairs read from memory."""
    if not e: return None
    e = e.strip()
    if e in ('_i', '__i') or '__disp' in e: return None
    m2 = re.search(r';~\s*([0-9A-Fa-f]+):([0-9A-Fa-f]+)', asm)
    segbase = (int(m2.group(1), 16) << 4) if m2 else L.segbase
    if re.match(r'^\*\(dd\*\)\s*\(\s*raddr_?\(\s*cs\s*,\s*eip\+1\)', e):
        # `jmp far ptr x:y` with the operand embedded right after the opcode
        return f"rt_far(*(dd*)&mem[0x{segbase + int(m2.group(2), 16) + 1:x}])" if m2 else None
    if re.match(r'^\*\(dd\*\)', e) or e.startswith('dword_'):
        return f"rt_far({e})"
    return f"(dd)0x{segbase:x} + ({e})"

# TANDYSND.EXE overlay support: when OVL_BASE is set, lifted names are overlay
# linear addresses (IDA base 0x10000); the port loads the overlay image at
# tnd_base (0x30000) so port mem addr = OVL_BASE + linear - 0x10000. All overlay
# code lives in its seg001 (image offset 0x70 => mem base OVL_BASE+0x70).
OVL_BASE = 0
OVL_CSEG = 0x70

def name_segbase(n):
    """mem base of the code segment owning lifted function n."""
    if OVL_BASE: return OVL_BASE + OVL_CSEG
    if n == 'start': return 0x1a20
    m = re.match(r'(?:sub|loc|locret)_([0-9a-fA-F]+)$', n)
    if m:
        off = int(m.group(1), 16) - 0x10000
        return 0xe8a0 if 0xce80 <= off < 0x1b510 else 0x1a20   # seg002 vs seg000
    m = re.match(r'ret_([0-9a-fA-F]+)_[0-9a-fA-F]+', n)
    if m: return int(m.group(1), 16) << 4
    m = re.match(r'seg(\d+)_', n)
    if m: return {'seg000':0x1a20, 'seg002':0xe8a0}.get('seg'+m.group(1), 0x1a20)
    return 0x1a20

def name_fnmem(n):
    """mem addr of function n's first byte (for embedded far operands)."""
    if OVL_BASE:
        m = re.match(r'(?:sub|loc|locret)_([0-9a-fA-F]+)$', n)
        if m: return OVL_BASE + int(m.group(1), 16) - 0x10000
        m = re.match(r'seg(\d+)_([0-9a-fA-F]+)_proc', n)
        if m: return OVL_BASE + {0: 0, 1: OVL_CSEG}.get(int(m.group(1)), 0x70) + int(m.group(2), 16)
        return 0
    m = re.match(r'(?:sub|loc|locret)_([0-9a-fA-F]+)$', n)
    if m: return int(m.group(1), 16) - 0xe5e0
    m = re.match(r'ret_([0-9a-fA-F]+)_([0-9a-fA-F]+)', n)
    if m: return (int(m.group(1),16) << 4) + int(m.group(2),16)
    m = re.match(r'seg(\d+)_([0-9a-fA-F]+)_proc', n)
    if m: return {'seg000':0x1a20, 'seg002':0xe8a0}.get('seg'+m.group(1), 0x1a20) + int(m.group(2),16)
    return 0

def lift_block(items, L, out, defined, blockmap=None):
    for kind, txt, asm in items:
        if kind == 'DISP':
            L.disp = txt
            continue
        if kind == 'R':
            stmts = L.op(txt, asm)
        else:
            stmts = L.jump(txt, asm)
            # inline trivial jump targets: `goto locret`/`if (c) goto locret`
            # where the target block is just `return;` or `f(); return;`
            mm = re.match(r'^(JMP|J\w+|LOOP\w*)\((\w+)\)$', txt.strip())
            if mm and blockmap and mm.group(2) in blockmap:
                tgt = mm.group(2)
                tiny = render_tiny_block(blockmap[tgt])
                if is_tiny_target(tiny):
                    if mm.group(1) == 'JMP':
                        stmts = tiny
                    else:
                        cm = re.match(r'^if \((.*)\) goto', stmts[0]) if stmts else None
                        if cm:
                            inner = ' '.join(tiny)
                            stmts = [f"if ({cm.group(1)}) {{ {inner} }}"]
        if not stmts and asm:
            out.append(f"    /* {asm} */")   # folded op: keep the asm visible
        for st in stmts:
            if asm: out.append(f"    /* {asm} */")
            out.append(f"    {st}")

UNCOND = re.compile(r'^(JMP|RETN|RETF|IRET|RET)\(')

RE_IFGOTO = re.compile(r'^\s*if \((.+)\) goto (\w+);\s*$')
RE_GOTO   = re.compile(r'^\s*goto (\w+);\s*$')
RE_LABELL = re.compile(r'^(\w+):\s*$')

def neg_cond(c):
    """Negate a simple condition by swapping the relational operator."""
    c = c.strip()
    if '&&' in c or '||' in c: return f"!({c})"
    m = re.match(r'^(.+?)\s*(==|!=|<=|>=|<|>)\s*(.+)$', c)
    if m and m.group(1).count('(') == m.group(1).count(')') \
          and m.group(3).count('(') == m.group(3).count(')'):
        inv = {'==':'!=','!=':'==','<':'>=','>':'<=','<=':'>','>=':'<'}
        return f"{m.group(1)} {inv[m.group(2)]} {m.group(3)}"
    if c.startswith('!'): return c[1:]
    if re.match(r'^!?[A-Z]?F$', c): return c[1:] if c.startswith('!') else f"!{c}"
    return f"!({c})"

def structure(lines):
    """Turn label/goto pairs into do-while/if blocks where regions nest cleanly."""
    labidx = {}
    for i, ln in enumerate(lines):
        m = RE_LABELL.match(ln)
        if m: labidx[m.group(1)] = i
    cands = []
    for i, ln in enumerate(lines):
        m = RE_IFGOTO.match(ln)
        if m:
            cond, tgt = m.group(1), m.group(2)
            if tgt not in labidx: continue
            if labidx[tgt] < i: cands.append((labidx[tgt], i, 'do', cond, i))
            else:               cands.append((i, labidx[tgt], 'if', cond, i))
            continue
        m = RE_GOTO.match(ln)
        if m and m.group(1) in labidx and labidx[m.group(1)] < i:
            cands.append((labidx[m.group(1)], i, 'do1', '1', i))
    cands.sort(key=lambda c: (c[1]-c[0], c[0]))
    acc = []
    def overlaps(a, b):
        return (a[0] < b[0] <= a[1] < b[1]) or (b[0] < a[0] <= b[1] < a[1])
    for c in cands:
        if any(overlaps(c, a) for a in acc): continue
        acc.append(c)
    acc.sort(key=lambda c: (c[0], -c[1]))
    opens, closes, replace = {}, {}, {}
    for s, e, kind, cond, gl in acc:
        if kind in ('do','do1'):
            opens.setdefault(s, []).append('do {')
            replace[gl] = f'}} while ({"1" if kind == "do1" else cond});'
        else:
            nc = neg_cond(cond)
            replace[gl] = f'if ({nc}) {{'
            closes.setdefault(e, []).append('}')
    # `goto X` inside a do-region, X = label right after that loop's end → break
    # (binds to innermost enclosing do-region)
    for i, ln in enumerate(lines):
        if i in replace: continue
        gm = RE_GOTO.match(ln)
        if not gm: continue
        tgt = gm.group(1)
        if tgt not in labidx: continue
        innermost = None
        for s, e, kind, cond, gl in acc:
            if kind in ('do','do1') and s <= i < e:
                if innermost is None or (s > innermost[0] or (s == innermost[0] and e < innermost[1])):
                    innermost = (s, e)
        if innermost:
            # next non-comment line after the loop's end must be the target label
            j = innermost[1] + 1
            while j < len(lines) and lines[j].strip().startswith('/*'): j += 1
            if j < len(lines) and RE_LABELL.match(lines[j]) and lines[j].split(':')[0] == tgt:
                replace[i] = 'break;'
    out = []
    for i, ln in enumerate(lines):
        for c in closes.get(i, []): out.append(c)
        out.append(replace[i] if i in replace else ln)
        if i in opens:
            out.extend(opens[i])
    return out

REGS = 'ax bx cx dx si di bp sp al ah bl bh cl ch dl dh CF ZF SF OF'.split()
ASSIGN_RE = re.compile(r'^(\s*)(\w+)\s*=\s*(.+);\s*$')

def _reads_reg(line, reg):
    """line reads reg (RHS use, condition, or op-assign like +=)."""
    m = ASSIGN_RE.match(line)
    if m and m.group(2) == reg:
        return reg in m.group(3)  # 'R = ..R..' reads it; 'R = e' does not
    if re.match(r'^\s*\w+\s*(\+\+|--)\s*;', line):
        # R++/R-- reads the old value; another reg's ++ does not read reg
        return re.match(r'^\s*%s\b' % reg, line) is not None
    return re.search(r'\b%s\b' % reg, line) is not None

def _writes_reg(line, reg):
    m = ASSIGN_RE.match(line)
    if m and m.group(2) == reg and reg not in m.group(3):
        return True
    return re.match(r'^\s*%s\s*(\+\+|--|\+=|-=|&=|\|=|\^=|<<=|>>=)\s*' % reg, line) is not None

_CF_BOUNDARY = re.compile(
    r'^\s*(?:\w+\s*:|goto\b|if\b|else\b|switch\b|while\b|for\b|do\b'
    r'|break\b|continue\b|return\b|\{|})')

def _reg_dead_after(lines, j, reg):
    """True if reg's current value is never read after line j.

    Straight-line scan only: the first occurrence of reg decides
    (write-without-read => dead; read => live). Any label, control-flow
    line or call ends the scan pessimistically (live): a write seen later
    may not dominate reads entered through other paths, and calls may
    observe or clobber register globals invisibly.
    """
    for k in range(j + 1, len(lines)):
        ln = re.sub(r'/\*.*?\*/', '', lines[k]).strip()
        if not ln: continue
        if _CF_BOUNDARY.match(ln): return False
        if re.search(r'\w+\s*\(', ln): return False   # call/macro: may touch reg globals
        if re.search(r'\b%s\b' % reg, ln):
            # first occurrence decides: write-without-read => dead; read => live
            return not _reads_reg(ln, reg) if _writes_reg(ln, reg) else False
    return True

STRUCT_BARRIER = re.compile(r'^\s*(?:\w+:|do\b|\}|else|break;|goto\b|return\b|while\s*\(|for\s*\()')

def fold_temps(lines):
    """Fold `R = expr; T(...R...)` into T using expr — eliminates register temps
    when the value is provably dead after its single use. Safe rules:
    - R is a gp reg or flag var; RHS pure (no calls into call-containing T)
    - T immediately follows S (labels/`}`/`do`/`while` block adjacency)
    - T is not a labeled/loop-condition line; S is not labeled
    - R not read anywhere after T before rewrite
    """
    changed = True
    while changed:
        changed = False
        i = 0
        while i < len(lines) - 1:
            m = ASSIGN_RE.match(lines[i])
            if not m or m.group(2) not in REGS:
                i += 1; continue
            reg, rhs = m.group(2), m.group(3).strip()
            if reg in rhs: i += 1; continue  # self-ref
            # S must not be a label target (jumping past a folded expr breaks)
            if i > 0 and re.match(r'^\s*\w+:', lines[i - 1]):
                i += 1; continue
            j = i + 1
            # skip standalone comment lines between
            while j < len(lines) and re.match(r'^\s*/\*.*\*/\s*$', lines[j]): j += 1
            if j >= len(lines): break
            tline = lines[j]
            if STRUCT_BARRIER.match(tline): i += 1; continue
            # T must use reg as a pure operand, never in a write position
            # (`reg = ..`, `reg += ..`, `(reg)++` — substituting breaks lvalue)
            if re.match(r'^\s*\(?\s*%s\s*\)?\s*(=|\+\+|--|\+=|-=|&=|\|=|\^=|<<=|>>=)' % reg, tline):
                i += 1; continue
            # RHS must be a single pure expression (no multi-stmt/block/top comma)
            if ';' in rhs or '{' in rhs or '}' in rhs: i += 1; continue
            d = 0; bad = False
            for c in rhs:
                if c == '(': d += 1
                elif c == ')': d -= 1
                elif c == ',' and d == 0: bad = True; break
            if bad: i += 1; continue
            uses = [mm.start() for mm in re.finditer(r'\b%s\b' % reg, tline)]
            if len(uses) != 1: i += 1; continue
            if '(' in rhs and '(' in tline: i += 1; continue  # call ordering
            if not _reg_dead_after(lines, j, reg): i += 1; continue
            # substitute
            new_rhs = rhs if re.match(r'^[0-9a-fA-Fx()&|~+\-*<>\s]+$', rhs) else f"({rhs})"
            lines[j] = tline[:uses[0]] + new_rhs + tline[uses[0] + len(reg):]
            lines[i] = ''
            changed = True
        lines = [l for l in lines if l != '']
    return lines

def reindent(lines):
    out, d = [], 1
    for ln in lines:
        s = ln.strip()
        if not s: continue
        if s.startswith('void '): out.append(s); d = 1; continue
        if RE_LABELL.match(s): out.append(s); continue
        if s.startswith('}') and d > 0: d -= 1
        out.append('    ' * d + s)
        if s.endswith('{'): d += 1
    return out

def fix_orphan_labels(lines):
    """A label directly before `}` needs a null statement."""
    out = list(lines)
    for i, ln in enumerate(out):
        m = RE_LABELL.match(ln)
        if not m: continue
        nxt = out[i+1].strip() if i+1 < len(out) else ''
        if nxt.startswith('}'):
            out[i] = ln.rstrip() + ' ;'
    return out

def drop_dead_labels(lines):
    """Drop `L:` + tiny block when no goto references L and L can't fall through."""
    refs = {}
    for ln in lines:
        for g in re.findall(r'goto (\w+)', ln):
            refs[g] = refs.get(g, 0) + 1
    out = []
    skip_block = False
    for i, ln in enumerate(lines):
        m = RE_LABELL.match(ln.strip())
        if m:
            name = m.group(1)
            prev = out[-1].strip() if out else ''
            # A label right after `if (c) {`/`do {`/`else {` IS reachable (the
            # block executes when the condition holds) — `{` must not mark the
            # label unreachable. Only real flow terminators do.
            fallthrough = not (prev.startswith('return') or prev == '}' or prev.startswith('goto')
                               or prev.startswith('break') or prev.endswith('return; }'))
            # look ahead: is the block tiny? (comment(s) then 'return;' or 'f(); return;')
            j = i + 1; stmts = []
            while j < len(lines):
                s2 = lines[j].strip()
                if RE_LABELL.match(s2) or s2 == '}': break
                if not s2.startswith('/*'): stmts.append(s2)
                j += 1
            tiny = stmts == ['return;'] or (len(stmts) == 2 and stmts[0].endswith('();') and stmts[1] == 'return;')
            if refs.get(name, 0) == 0 and not fallthrough and tiny:
                skip_block = True
                continue
            skip_block = False
            out.append(ln); continue
        if skip_block:
            s = ln.strip()
            if RE_LABELL.match(s) or s == '}': skip_block = False; out.append(ln)
            continue
        out.append(ln)
    return out

def emit_entry(name, label, blockmap, order, group_entries):
    """Emit `void name()` = code reachable from `label` via goto-closure + fall-through."""
    L = Lifter()
    L.segbase, L.fnmem = name_segbase(name), name_fnmem(name)
    out = [f"void {name}(void) {{"]
    idx = {lb: n for n, (lb, _) in enumerate(order)}
    reach = set()
    work = [label]
    while work:
        lb = work.pop()
        if lb in reach or lb not in blockmap or lb not in idx: continue
        # walk forward from this block following fall-through
        j = idx[lb]
        while True:
            lb2, items = order[j]
            if lb2 in reach: break
            reach.add(lb2)
            term = None
            for k, t, a in items:
                if k != 'J': continue
                for g in re.findall(r'JMP\((\w+)\)|J\w+\((\w+)\)|LOOP\w*\((\w+)\)', t):
                    for g2 in g:
                        if g2 and g2 != lb2 and not g2.startswith('sub_') and g2 != 'start':
                            work.append(g2)
                term = t
            if term and UNCOND.match(term.strip()): break
            j += 1
            if j >= len(order): break
    need = set()
    for lb, items in order:
        if lb not in reach: continue
        for k, t, a in items:
            if k != 'J': continue
            for g in re.findall(r'JMP\((\w+)\)|J\w+\((\w+)\)|LOOP\w*\((\w+)\)', t):
                for g2 in g:
                    if g2 in reach: need.add(g2)
    # Emit reached blocks starting at the entry point. Blocks earlier in
    # `order` that were pulled in via goto (shared tails like loc_104BB
    # ending sub_104A4 but jumped to from sub_1059B) must go LAST — otherwise
    # they execute before the real entry code on every call.
    ei = idx.get(label, 0)
    reached_idx = [i for i, (lb, _) in enumerate(order) if lb in reach]
    seq = [i for i in reached_idx if i >= ei] + [i for i in reached_idx if i < ei]

    def _term(items):
        t = None
        for k, tt, a in items:
            if k == 'J': t = tt
        return t

    # fall-through fixups: if a block's natural successor is reached but is
    # not the next emitted block, it needs an explicit goto (and a printed label)
    for n, i in enumerate(seq):
        nxt = order[seq[n + 1]][0] if n + 1 < len(seq) else None
        succ = order[i + 1][0] if i + 1 < len(order) else None
        t = _term(order[i][1])
        if not (t and UNCOND.match(t.strip())) and succ in reach and succ != nxt:
            need.add(succ)
    for n, i in enumerate(seq):
        lb, items = order[i]
        if lb != name or lb in need: out.append(f"{lb}:")
        L.pending = None  # block boundary: never fold flag state across labels
        lift_block(items, L, out, reach, blockmap)
        nxt = order[seq[n + 1]][0] if n + 1 < len(seq) else None
        succ = order[i + 1][0] if i + 1 < len(order) else None
        t = _term(items)
        if not (t and UNCOND.match(t.strip())) and succ in reach and succ != nxt:
            out.append(f"    goto {succ};")
    out.append("}\n")
    out = structure(out)
    out = drop_dead_labels(out)
    out = fix_orphan_labels(out)
    out = fold_temps(out)
    out = reindent(out)
    # merge `ah = lit; [comment] al = lit;` → `ax = lit;` keeping comments
    merged = []
    i2 = 0
    while i2 < len(out):
        m1 = re.match(r'^(\s*)ah = (0x[0-9a-fA-F]+|\d+);', out[i2])
        if m1 and i2 + 1 < len(out):
            j2 = i2 + 1
            c2 = out[j2] if re.match(r'^\s*/\*.*\*/\s*$', out[j2]) else None
            if c2 is not None: j2 += 1
            m2 = re.match(r'^\s*al = (0x[0-9a-fA-F]+|\d+);\s*$', out[j2]) if j2 < len(out) else None
            if m2:
                hv, lv = int(m1.group(2), 0), int(m2.group(1), 0)
                if c2 is not None: merged.append(c2)
                merged.append(f"{m1.group(1)}ax = 0x{((hv << 8) | lv):04X};")
                i2 = j2 + 1; continue
        merged.append(out[i2]); i2 += 1
    return merged

def lift_file(path, out):
    lines = open(path).read().split('\n')
    i, n = 0, len(lines)
    while i < n:
        m = RE_FUNC.match(lines[i])
        if not m: i += 1; continue
        fname = m.group(1)
        blocks, entries, aliases, i = parse_func(lines, i)
        if not blocks: continue
        order = blocks
        blockmap = {}
        for lb, items in blocks:
            blockmap.setdefault(lb, []).extend(items)

        def resolve(lb):
            while lb in aliases: lb = aliases[lb]
            return lb

        first = blocks[0][0]
        # entry points: standalone func → its own name; group → sub_*/start labels in switch + leading label
        if fname.startswith('sub_') and fname not in blockmap and fname not in aliases:
            # standalone: body may sit under '_begin' or the sub_ label or _entry
            entry_labels = [b[0] for b in blocks]
            for e in emit_entry(fname, entry_labels[0], blockmap, order, entries):
                out.append(e)
            continue
        # group container: emit one function per sub_/start/seg entry label
        want = [e for e in entries if e != fname] + [first]
        seen = set()
        for lb in want:
            tgt = resolve(lb)
            if lb in seen or tgt not in blockmap: continue
            seen.add(lb)
            for e in emit_entry(lb, tgt, blockmap, order, entries):
                out.append(e)
        # fixup: emit functions for any called labels not yet emitted
        for _ in range(4):
            missing = set()
            for ln in out:
                for cm in re.finditer(r'^\s+((?:loc|seg|sub|edummy)\w*)\(\);', ln):
                    t = cm.group(1)
                    if resolve(t) in blockmap and t not in seen and t != '_entry':
                        missing.add(t)
            if not missing: break
            for lb in sorted(missing):
                seen.add(lb)
                for e in emit_entry(lb, resolve(lb), blockmap, order, entries):
                    out.append(e)

def main():
    os.makedirs(OUT, exist_ok=True)
    hdr = open(os.path.join(OUT, 'lifted.h'), 'w')
    hdr.write("""// lifted Airborne Ranger — readable decompilation artifact
#include <stdint.h>
typedef uint8_t db; typedef uint16_t dw; typedef uint32_t dd;
extern dd eax,ebx,ecx,edx,esi,edi,esp,ebp;
extern dw ax,bx,cx,dx,si,di,sp,bp,cs,ds,es,fs,gs,ss,ip;
extern db al,ah,bl,bh,cl,ch,dl,dh;
extern int CF,ZF,SF,OF,PF,AF,DF,IF,TF;
extern db mem[];
__attribute__((always_inline)) static inline db *raddr_(int seg,int off){return &mem[((seg&0xffff)<<4)+(off&0xffff)];}
__attribute__((always_inline)) static inline db *raddr(int seg,int off){return raddr_(seg,off);}
void push(dw); dw pop(void); void pushf(void); void popf(void);
dw rol16(dw,int); dw ror16(dw,int); dw rcl16(dw,int); dw rcr16(dw,int);
dw in(dw); void out(dw,dw); void swi(int);
void dos_int21(dw); void bios_video(dw); void bios_kbd(dw); void bios_time(void);
#define swap(a,b) do{__typeof__(a)_t=(a);(a)=(b);(b)=_t;}while(0)
// named int21/int10/int16 wrappers
void dos_exit(void);void dos_read_char_echo(void);void dos_write_char(void);void dos_direct_io(void);
void dos_read_char_noecho(void);void dos_print_string(void);void dos_read_string(void);
void dos_check_stdin(void);void dos_get_drive(void);void dos_set_dta(void);void dos_set_int_vector(void);
void dos_get_date(void);void dos_get_time(void);void dos_get_version(void);void dos_get_int_vector(void);
void dos_open(void);void dos_close(void);void dos_read(void);void dos_write(void);void dos_delete(void);
void dos_seek(void);void dos_get_attr(void);void dos_ioctl(void);void dos_alloc(void);void dos_free(void);
void dos_resize(void);void dos_exec(void);void dos_exit_code(void);void dos_find_first(void);
void dos_find_next(void);void dos_get_verify(void);void dos_rename(void);void dos_file_datetime(void);
void dos_create_temp(void);void dos_create_new(void);void dos_get_psp(void);
void bios_set_mode(void);void bios_set_cursor(void);void bios_scroll(void);void bios_putc(void);
void bios_palette(void);void bios_getch(void);void bios_kbhit(void);
// data symbols live in the segment images (ar.exe.h)
""")
    hdr.close()
    # lifted_data.h: data symbol decls extracted from ar.exe.h
    dh = open(os.path.join(OUT, 'lifted_data.h'), 'w')
    dh.write("// data symbol declarations lifted from ar.exe.h\n")
    seen = set()
    for ln in open(os.path.join(SRC, 'ar.exe.h')):
        m = re.match(r'\s*extern\s+(db|dw|dd)&\s*(\w+)\s*;', ln)
        if m and m.group(2) not in seen:
            seen.add(m.group(2))
            dh.write(f"extern {m.group(1)} {m.group(2)};\n")
    dh.close()
    all_defs = []
    for f in sorted(glob.glob(os.path.join(SRC, 'ar.exe*.cpp'))):
        out = ['#include "lifted.h"\n#include "lifted_data.h"\n#include "lifted_procs.h"\n']
        lift_file(f, out)
        on = os.path.join(OUT, os.path.basename(f).replace('.cpp', '.c'))
        body = '\n'.join(out)
        open(on, 'w').write(body)
        all_defs += re.findall(r'^void (\w+)\(void\)', body, re.M)
        print(f"{on}: {len(out)} lines")

    # TANDYSND.EXE overlay (Tandy sound driver). Its procs use overlay-linear
    # names (sub_10xxx) mapped at the port's overlay load base (0x30000); keep
    # them out of lifted_procs.h so gen_port.py doesn't fmap them wrongly.
    global OVL_BASE
    OVL_BASE = 0x30000
    tnd_defs = []
    for f in sorted(glob.glob(os.path.join(os.path.dirname(os.path.dirname(SRC)), 'tandysnd.exe_seg001.cpp'))):
        out = ['#include "lifted.h"\n#include "lifted_data.h"\n#include "lifted_procs.h"\n']
        lift_file(f, out)
        on = os.path.join(OUT, os.path.basename(f).replace('.cpp', '.c'))
        body = '\n'.join(out)
        open(on, 'w').write(body)
        tnd_defs += re.findall(r'^void (\w+)\(void\)', body, re.M)
        print(f"{on}: {len(out)} lines (overlay)")
    OVL_BASE = 0
    with open(os.path.join(OUT, 'tnd_procs.h'), 'w') as tp:
        for d in sorted(set(tnd_defs)):
            tp.write(f"void {d}(void);\n")

    ph = open(os.path.join(OUT, 'lifted_procs.h'), 'w')
    ph.write("// lifted proc decls\n")
    for d in sorted(set(all_defs)):
        ph.write(f"void {d}(void);\n")
    # extern decls for called-but-not-lifted procs (wrappers/data refs)
    ph.write("void __dispatch_call_ext(void);\nvoid indirect_jump(void);\n")
    ph.close()

if __name__ == '__main__':
    main()
