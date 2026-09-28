#!/usr/bin/env python3
"""Materialize ZF/SF/OF flag globals at every flag-defining emitted statement.

lift.py folds flag ops into `pending` for branch fusion, but CALL/RETN/JMP
clear pending — so flags that must survive a call boundary (the asm
`or al,al; retn` boolean-return idiom, wait-loops, sign tests) were never
written to the ZF/SF/OF globals. Callers then read stale flags.

This pass appends the missing flag writes inline, matching the semantics
the real instructions produce:
  and/or/xor/test : CF=OF=0, ZF=(res==0), SF=sign(res)
  add/sub/adc/sbb : CF emitted already, ZF/SF of stored result
  cmp             : ZF=(a==b), SF=sign(a-b)   (CF already emitted)
  inc/dec         : ZF/SF of result (CF untouched on hw, left alone)
  neg             : CF emitted, ZF/SF of result
  shl/shr/sar     : CF emitted, ZF/SF of result (inside the if(n) guard)

Idempotent: lines already containing `ZF =` are skipped.
"""
import re, sys

BYTEREG = {'al','ah','bl','bh','cl','ch','dl','dh'}

def width(e):
    e = e.strip()
    if e in BYTEREG or 'byte_' in e or '(db*)' in e or e.startswith('(db)'):
        return 'b'
    return 'w'

def zsf(res, w):
    T = 'db' if w == 'b' else 'dw'
    s = 7 if w == 'b' else 15
    return f"ZF = (({T})({res}) == 0); SF = ((({T})({res})) >> {s});"

def _names(o):
    o = re.sub(r'\b(\d[\da-fA-F]*)h\b', r'0x\1', o)
    return re.sub(r'\b([A-Za-z_]\w*)\b',
                  lambda mm: mm.group(1).lower()
                  if mm.group(1).startswith(('word_', 'byte_', 'off_', 'loc_'))
                  else mm.group(1), o)

def a2c(o, w):
    """asm operand -> C expr, mirroring lift.py's emitted forms."""
    o = o.strip()
    m = re.match(r'^(byte|word|dword) ptr (.*)$', o)
    T = 'db' if (m and m.group(1) == 'byte') or (not m and w == 'b') else 'dw'
    if m:
        o = m.group(2).strip()
    inner = _names(o)
    mm = re.match(r'^(?:([de]s):)?\[(.+)\]$', inner) or re.match(r'^([de]s):([^:].*)$', inner)
    if mm:
        seg = mm.group(1) or 'ds'
        return f"*({T}*)raddr({seg}, {mm.group(2)})"
    if m:
        return f"*({T}*)(&{inner})"
    return inner

def xform(line, prev_asm):
    if 'ZF =' in line:
        return line
    s = line.strip()
    ind = line[:len(line) - len(line.lstrip())]

    # xor r,r / xor x,x  ->  X = 0; CF = 0;
    m = re.match(r'^(\w[\w\*\(\)&\., \+\->]*) = 0; CF = 0;$', s)
    if m:
        return line.rstrip() + " OF = 0; ZF = 1; SF = 0;\n"

    # and/or/xor  ->  X op= Y; CF = 0;
    m = re.match(r'^(.+?) ([&|^])= (.+); CF = 0;$', s)
    if m:
        a = m.group(1)
        return line.rstrip() + " OF = 0; " + zsf(a, width(a)) + "\n"

    # neg  ->  X = -(X); CF = (X != 0);
    m = re.match(r'^(.+?) = -\(.+?\); CF = \(.+ != 0\);$', s)
    if m:
        a = m.group(1)
        return line.rstrip() + " " + zsf(a, width(a)) + "\n"

    # cmp bare:  CF = (dd)A < (dd)B;
    m = re.match(r'^CF = \(dd\)(.+) < \(dd\)(.+);$', s)
    if m:
        a, b = m.group(1), m.group(2)
        w = width(a)
        T = 'db' if w == 'b' else 'dw'
        sc = 7 if w == 'b' else 15
        return (line.rstrip() + f" ZF = (({T})(({a}) - ({b})) == 0);"
                f" SF = ((({T})(({a}) - ({b}))) >> {sc});\n")

    # add/sub/adc/sbb block: { dd t_ = ...; CF = ...; X = t_; }
    if s.startswith('{ dd t_ =') and s.endswith('= t_; }'):
        body = s[:-2]  # drop trailing '}'
        m = re.search(r'([^;=]+) = t_;$', body)
        if m:
            a = m.group(1).strip()
            return ind + body + " " + zsf(a, width(a)) + " }\n"

    # inc/dec: (X)++;  (X)--;
    m = re.match(r'^\((.+)\)(\+\+|--);$', s)
    if m:
        a = m.group(1)
        return line.rstrip() + " " + zsf(a, width(a)) + "\n"

    # shifts: { if (n) { CF = ...; X <<= n; } } / X = cast X >> n;
    m = re.match(r'^(\{ if \(.+\) \{ CF = .+; )(.+) (<<=) (.+); \} \}$', s)
    if m:
        a = m.group(2)
        return ind + m.group(1) + f"{a} <<= {m.group(4)}; " + zsf(a, width(a)) + " } }\n"
    m = re.match(r'^(\{ if \(.+\) \{ CF = .+; )(.+) = (.*) >> (.+); \} \}$', s)
    if m:
        a = m.group(2)
        return ind + m.group(1) + f"{a} = {m.group(3)} >> {m.group(4)}; " + zsf(a, width(a)) + " } }\n"

    # test A,B -> bare CF = 0;  (operands live only in the asm comment)
    if s == 'CF = 0;' and prev_asm:
        m = re.match(r'^\s*test\s+(.+?),\s*(.+?)\s*$', prev_asm)
        if m:
            aa, bb = m.group(1), m.group(2)
            w = 'b' if (aa.strip() in BYTEREG or 'byte' in aa or
                        bb.strip() in BYTEREG or 'byte' in bb) else 'w'
            res = f"({a2c(aa, w)} & {a2c(bb, w)})"
            return ind + "CF = 0; OF = 0; " + zsf(res, w) + "\n"
    return line

def main(path):
    lines = open(path).read().splitlines(True)
    out, prev_asm, n = [], None, 0
    for line in lines:
        am = re.match(r'\s*/\* (\w+.*?)\s*;~', line)
        new = xform(line, prev_asm)
        if new != line:
            n += 1
        out.append(new)
        prev_asm = am.group(1) if am else None
    open(path, 'w').writelines(out)
    print(f"{path}: {n} statements gained flag writes")

if __name__ == '__main__':
    for p in sys.argv[1:]:
        main(p)
