#!/usr/bin/env python3
"""Apply tools/names.map renames in place (evidence-driven function names).

Map line format: `oldname newname [tnd]` — untagged renames AR.EXE functions,
'tnd'-tagged renames TANDYSND.EXE overlay procs (prefixed tnd_ in the port).

Sites updated:
  lifted/ar.exe*.c, port/gen/ar.exe*.c       `old(` -> `new(` + trace strings
  lifted/lifted_procs.h, port/procs.h        decls
  lifted/tandysnd.exe_seg001.c               `old(` -> `new(`
  lifted/tnd_procs.h                         decls
  port/gen/tandysnd_seg001.c, port/tnd_procs.h `tnd_old(` -> `tnd_new(`
  port/memimg.c                              fmap entries + extern + tw_ wrappers (bare)
  tools/gen_port.py                          TRACE_PROCS literals (bare)

Single pass per file: all old names are folded into one alternation (longest
first, so prefixes resolve the same way as the old per-name loop) and one
combined re.sub walk replaces every hit. ~40x faster on the 8 MB seg000 files.

Asm comments (`/* call sub_XXXX ;~ ... */`) intentionally keep original names —
they quote the listing, not the C namespace.
"""
import re, os, glob

AR = '/home/xor/games/airborn'
MAP = os.path.join(AR, 'tools', 'names.map')

main_map, tnd_map = {}, {}
for ln in open(MAP):
    f = ln.split('#')[0].split()
    if len(f) >= 2:
        (tnd_map if 'tnd' in f[2:] else main_map)[f[0]] = f[1]

def combined(map_):
    """One regex matching any old name; longest alternatives first."""
    alts = '|'.join(re.escape(o) for o in sorted(map_, key=lambda s: -len(s)))
    return re.compile(alts) if alts else None

def sub_call(src, pat, map_):
    """`old(` -> `new(` plus rt_tracef tags `"old"` / `"ENTER old`."""
    if not pat:
        return src
    call_re = re.compile(r'\b(%s)\s*\(' % pat.pattern)
    enter_re = re.compile(r'"ENTER (%s)' % pat.pattern)
    str_re = re.compile(r'"(%s)"' % pat.pattern)
    src = call_re.sub(lambda m: map_[m.group(1)] + '(', src)
    src = enter_re.sub(lambda m: '"ENTER ' + map_[m.group(1)], src)
    return str_re.sub(lambda m: '"' + map_[m.group(1)] + '"', src)

def sub_word(src, pat, map_):
    if not pat:
        return src
    word_re = re.compile(r'\b(%s)\b' % pat.pattern)
    return word_re.sub(lambda m: map_[m.group(1)], src)

def patch(path, fn):
    src = open(path, encoding='utf8', errors='replace').read()
    out = fn(src)
    if out != src:
        open(path, 'w').write(out)
        print('  ', path)
    return out != src

main_pat = combined(main_map)
hits = 0
for f in glob.glob(f'{AR}/lifted/ar.exe*.c') + glob.glob(f'{AR}/port/gen/ar.exe*.c'):
    hits += patch(f, lambda s: sub_call(s, main_pat, main_map))
for f in (f'{AR}/lifted/lifted_procs.h', f'{AR}/port/procs.h'):
    hits += patch(f, lambda s: sub_call(s, main_pat, main_map))
for f in (f'{AR}/port/memimg.c', f'{AR}/tools/gen_port.py'):
    hits += patch(f, lambda s: sub_word(s, main_pat, main_map))
for f in glob.glob(f'{AR}/port/tests/*.[ch]'):
    hits += patch(f, lambda s: sub_word(s, main_pat, main_map))

tnd_pat = combined(tnd_map)
hits += patch(f'{AR}/lifted/tandysnd.exe_seg001.c', lambda s: sub_call(s, tnd_pat, tnd_map))
hits += patch(f'{AR}/lifted/tnd_procs.h', lambda s: sub_call(s, tnd_pat, tnd_map))
tnd_pref = {f'tnd_{o}': f'tnd_{n}' for o, n in tnd_map.items()}
tnd_pref_pat = combined(tnd_pref)
for f in (f'{AR}/port/gen/tandysnd_seg001.c', f'{AR}/port/tnd_procs.h'):
    hits += patch(f, lambda s: sub_call(s, tnd_pref_pat, tnd_pref))
tw_map = {**{f'tw_tnd_{o}': f'tw_tnd_{n}' for o, n in tnd_map.items()}, **tnd_pref}
hits += patch(f'{AR}/port/memimg.c',
              lambda s: sub_word(s, combined(tw_map), tw_map))

print(f'{len(main_map)} main + {len(tnd_map)} tnd renames applied ({hits} files touched)')
