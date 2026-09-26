#!/usr/bin/env python3
# Patch ghidra_c/*.c: replace swi(0xNN)+(*pcVar)() artifacts with the named
# DOS/BIOS calls resolved by tools/lift.py for the same proc address.
import os, re, glob

GH = '/home/xor/games/airborn/ghidra_c'
LIFT = '/home/xor/games/airborn/lifted'

# 1) index lifted functions: name -> ordered list of named int calls + register
#    results written right after (mov mem,ax etc. stay explicit in lifted code)
lift_calls = {}
for lf in glob.glob(os.path.join(LIFT, '*.c')):
    src = open(lf, encoding='utf8', errors='replace').read()
    for m in re.finditer(r'^void (\w+)\(void\) \{(.*?)\n\}', src, re.S | re.M):
        name, body = m.group(1), m.group(2)
        calls = re.findall(r'\b(dos_\w+|bios_\w+|swi)\s*\(', body)
        if calls:
            lift_calls[name.lower()] = calls

patched = nswi = 0
for fn in glob.glob(os.path.join(GH, '*.c')):
    src = open(fn, encoding='utf8', errors='replace').read()
    if 'swi' not in src:
        continue
    # proc name = part before first '_' of basename: sub_108C6_1000_08c6.c
    base = os.path.basename(fn)
    m = re.match(r'(sub_[0-9a-fA-F]+|start|entry)_', base)
    if not m:
        continue
    lname = m.group(1).lower()
    calls = list(lift_calls.get(lname, []))
    call_iter = iter(calls)

    def repl_swi(mm):
        global nswi
        nswi += 1
        try:
            name = next(call_iter)
            return '/*int*/ %s();' % name
        except StopIteration:
            return 'swi(%s);' % mm.group(1)

    # pattern A:  pcVarN = (code *)swi(0xNN);   ->   /*int*/ dos_xxx();
    src = re.sub(r'(\w+)\s*=\s*\(code \*\)swi\((0x[0-9a-fA-F]+)\);', repl_swi, src)
    # pattern B:  DEST = (*pcVarN)();   ->   DEST = ax;
    src = re.sub(r'=\s*\(\*\w+\)\(\);', '= ax;', src)
    # pattern C:  (*pcVarN)();        ->   (void)ax;
    src = re.sub(r'\(\*\w+\)\(\);', '(void)ax;', src)
    # pattern D:  bare swi(0xNN) not part of pattern A
    src = re.sub(r'\bswi\((0x[0-9a-fA-F]+)\)', repl_swi, src)
    # drop now-unused decl lines:  code *pcVarN;  char *pcVarN;
    src = re.sub(r'^\s*(code|char) \*pcVar\d+;\s*\n', '', src, flags=re.M)
    # declare ax if referenced
    if re.search(r'\bax\b', src) and 'extern' not in src.split('{')[0]:
        src = 'extern unsigned short ax; /* ax after int call */\n' + src
    open(fn, 'w').write(src)
    patched += 1

print(f"patched {patched} files, {nswi} swi sites")
