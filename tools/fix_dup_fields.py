#!/usr/bin/env python3
"""Re-expand data directives that the old generator mis-parsed.

The stale ar.exe.cpp initializer was produced by a generator that read an
all-decimal-digit hex count like `76h` as decimal 76 instead of 118. That
shrinks the leading `dup(0)` run so any trailing bytes land at the wrong
offset inside the field.  This script re-parses each `db/dw/dd` line in the
.lst, expands `dup` counts correctly, and reports/repairs the generated
init arrays in ar.exe.cpp's Initializer_ar_exe.
"""
import re, sys

LST = 'AR.EXE.lst'
CPP = 'ar.exe.cpp'
SEG2LS = {'seg000': 0x1a2, 'seg001': 0xe45, 'seg002': 0xe8a, 'seg003': 0x1cf3}


def parse_num(tok):
    tok = tok.strip()
    if tok == '?':
        return 0
    if re.fullmatch(r'[0-9][0-9a-fA-F]*[Hh]', tok):
        return int(tok[:-1], 16)
    if re.fullmatch(r'[0-9]+[Bb]', tok):
        return int(tok[:-1], 2)
    if re.fullmatch(r'[0-9]+[OoQq]', tok):
        return int(tok[:-1], 8)
    if re.fullmatch(r'[0-9]+[Dd]?', tok):
        return int(tok, 10)
    if re.fullmatch(r'0[xX][0-9a-fA-F]+', tok):
        return int(tok, 16)
    return None


def split_top(s):
    """Split on top-level commas (respecting parens and quotes)."""
    out, depth, q, cur = [], 0, None, ''
    for c in s:
        if q:
            cur += c
            if c == q:
                q = None
            continue
        if c in '\'"':
            q = c; cur += c; continue
        if c == '(':
            depth += 1
        elif c == ')':
            depth -= 1
        if c == ',' and depth == 0:
            out.append(cur); cur = ''
        else:
            cur += c
    if cur.strip():
        out.append(cur)
    return out


def parse_num_buggy(tok):
    """The old generator's number parse: all-decimal-digit `NNh` -> decimal."""
    tok = tok.strip()
    if re.fullmatch(r'[0-9]{2,}[Hh]', tok):
        return int(tok[:-1], 10)   # BUG: treated hex-looking digits as decimal
    return parse_num(tok)


def expand_values(body, width, buggy=False):
    """Expand a db/dw/dd body to a list of integer element values."""
    vals = []
    for tok in split_top(body):
        tok = tok.strip()
        if not tok:
            continue
        dm = re.match(r'(.*?)\s+dup\s*\((.*)\)\s*$', tok, re.I | re.S)
        if dm:
            cnt = (parse_num_buggy if buggy else parse_num)(dm.group(1).strip())
            inner = expand_values(dm.group(2), width, buggy)
            if cnt is None:
                return None
            vals += inner * cnt
            continue
        if tok.startswith(("'", '"')):
            vals += [ord(c) for c in tok[1:-1]]
            continue
        n = parse_num(tok)
        if n is None:
            return None  # expression we can't eval (offset/seg/label)
        if width == 1:
            vals.append(n & 0xFF)
        elif width == 2:
            vals.append(n & 0xFFFF)
        elif width == 4:
            vals.append(n & 0xFFFFFFFF)
    return vals


def main():
    lines = open(LST).read().splitlines()
    try:
        orig = open('orig_image.bin', 'rb').read()
    except OSError:
        orig = b''
    # collect every data directive offset per segment (even unparseable) so we
    # can compute each field's true element span = next_offset - this_offset
    width_of = {'db': 1, 'dw': 2, 'dd': 4}
    seg_offs = {}
    for l in lines:
        m = re.match(r'(seg\d+):([0-9A-Fa-f]+)\s+(d[bwd])\s+(.*)', l)
        if m and m.group(1) in SEG2LS:
            seg_offs.setdefault(m.group(1), []).append(int(m.group(2), 16))
    for s in seg_offs:
        seg_offs[s].sort()

    def span(seg, off, width):
        """Bytes until the next data directive, in elements."""
        for o in seg_offs[seg]:
            if o > off:
                return (o - off) // width
        return None

    # (loadseg, offset) -> (typ, correct element values)
    correct = {}
    for l in lines:
        m = re.match(r'(seg\d+):([0-9A-Fa-f]+)\s+(d[bwd])\s+(.*)', l)
        if not m or m.group(1) not in SEG2LS:
            continue
        seg, off, typ, body = m.group(1), int(m.group(2), 16), m.group(3), m.group(4)
        body = body.split(';')[0].strip()
        # only trust lines we can fully expand AND that contain a
        # potentially-misparsed all-decimal-digit hex dup count (NNh, NN>=10)
        if not re.search(r'(?<![0-9a-zA-Z_])[0-9]{2,}[Hh]\s+dup', body):
            continue
        v = expand_values(body, width_of[typ])
        if v is None:
            continue
        sp = span(seg, off, width_of[typ])
        if sp is not None and len(v) > sp:
            continue
        # buggy re-expansion must reproduce the generated array to be trusted
        correct[(SEG2LS[seg], off)] = (typ, v, expand_values(body, width_of[typ], buggy=True))

    cpp = open(CPP).read().split('\n')
    fixed = 0
    write = '--write' in sys.argv
    for i, l in enumerate(cpp):
        cm = re.search(r'//\s*([0-9a-fA-F]{4}):([0-9a-fA-F]{4})', l)
        if not cm:
            continue
        key = (int(cm.group(1), 16), int(cm.group(2), 16))
        if key not in correct:
            continue
        typ, want, buggy = correct[key]
        am = re.search(r'(d[bwd])\s+tmp999(\[\d+\])?=\{?([^};]*)\}?;', l)
        if not am:
            continue
        gen = [x.strip() for x in am.group(3).split(',') if x.strip() != '']
        gen = [int(x, 0) for x in gen]
        if gen == want:
            continue
        # validate: buggy re-expansion must reproduce the generated array exactly;
        # that proves our parser is faithful so `want` is the right fix.
        if buggy != gen:
            print('SKIP line %d %s: buggy-expansion != gen (parser divergence)' % (i + 1, cm.group(0)))
            continue
        # cross-check `want` against orig_image.bin (flat image, base seg 0x1a2)
        m_off = key[0] * 16 + key[1]
        wbytes = b''.join(v.to_bytes(width_of[typ], 'little') for v in want)
        img = orig[m_off - 0x1a20:m_off - 0x1a20 + len(wbytes)]
        tag = 'OK' if img == wbytes else 'imgdiff'
        if len(want) == 1:
            arr = '%s tmp999=%d' % (typ, want[0])
        else:
            arr = '%s tmp999[%d]={%s}' % (typ, len(want), ','.join(map(str, want)))
        nl = re.sub(r'd[bwd]\s+tmp999(\[\d+\])?=\{?[^};]*\}?;', arr + ';', l)
        print('line %d  %s: gen %d -> want %d  %s' % (i + 1, cm.group(0), len(gen), len(want), tag))
        cpp[i] = nl
        fixed += 1
    if write:
        open(CPP, 'w').write('\n'.join(cpp))
        print('WROTE', fixed)
    else:
        print('WOULD FIX', fixed, '(run with --write)')


if __name__ == '__main__':
    main()
