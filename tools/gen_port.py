#!/usr/bin/env python3
# Generate the port/ tree pieces that come from mechanical sources:
#   port/data_syms.h  — #define word_XXXX (*(volatile dw*)&mem[addr]) aliases
#   port/memimg.c     — segment image bytes + MYCOPY fixups + addr->func map
#   port/gen_*.c      — lifted bodies with port headers
#
# Memory model (same as fake-C runtime):
#   mem[] flat; image base para 0x1a2 => seg000 at mem[0x1a20].
#   mem_addr(sym) = 0x1a20 + mapseg_para*16 + map_off
#   runtime segment value of a segment = (0x1a20 + mapstart) >> 4
import re, os, glob
from collections import Counter

AR = '/home/xor/games/airborn'

# semantic renames (tools/names.map): lifted sources may already carry the new
# names — fmap addr lookup + tndoffs keys must resolve via the ORIGINAL name.
NAMEMAP, TNDMAP = {}, {}
NAME2OLDS = {}  # semantic -> every raw name mapped to it (multi-map is real:
                # byte_2a7ad is BOTH alert_lvl and bar_val — a dict would
                # silently drop one side of the mapping)
for _ln in open(os.path.join(AR, 'tools', 'names.map')):
    _f = _ln.split('#')[0].split()
    if len(_f) >= 2:
        (TNDMAP if 'tnd' in _f[2:] else NAMEMAP)[_f[0]] = _f[1]
        if 'tnd' not in _f[2:]:
            NAME2OLDS.setdefault(_f[1], []).append(_f[0])
NAME2OLD = {v: k for k, v in NAMEMAP.items()}
TND2OLD = {v: k for k, v in TNDMAP.items()}
OUT = os.path.join(AR, 'port')
os.makedirs(OUT, exist_ok=True)

# --- map: segpara start + symbols ---
segstart = {}
syms = []  # (name, segpara, off, memaddr)
mode = 0
for ln in open(os.path.join(AR, 'AR.EXE.map')):
    m = re.match(r'\s*([0-9A-Fa-f]{5})H\s+([0-9A-Fa-f]{5})H\s+([0-9A-Fa-f]{5})H\s+(\w+)', ln)
    if m:
        segstart[m.group(4)] = int(m.group(1), 16); mode = 1; continue
    m = re.match(r'\s*([0-9A-Fa-f]{4}):([0-9A-Fa-f]{4})\s+(\w+)', ln)
    if m:
        para, off, name = int(m.group(1), 16), int(m.group(2), 16), m.group(3)
        syms.append((name, para, off, 0x1a20 + para * 16 + off))

symaddr = {n: a for n, p, o, a in syms}
# runtime para value for each segment name
segpara = {n: (0x1a20 + s) >> 4 for n, s in segstart.items()}

# symbol sizes from lifted_data.h externs (db/dw/dd) + array types from ar.exe.h
symtype = {}
for ln in open(os.path.join(AR, 'lifted', 'lifted_data.h')):
    m = re.match(r'extern (db|dw|dd) (\w+);', ln)
    if m: symtype[m.group(2)] = m.group(1)
def guess_type(name):
    if name.startswith(('word_', 'seg_')): return 'dw'
    if name.startswith(('dword_', 'far', 'jpt_', 'funcs_')): return 'dd'
    return 'db'

# decompress_res scratch block (seg002:7FF0..7FFB, above the ds:67F0 dict).
# The decoder runs with ds = the *compressed-source* segment (e.g. the 38c5
# staging buffer), so the original `mov ds:[7FFx]` hits scratch RAM there.
# Pinning these to image addresses puts them inside the TTLSCR decompression
# dest (e8a:3F22+) and the output overwrites the decoder's own state mid-run
# (TTLSCR stops at 16599 instead of 22000). Emit ds-relative defines — under
# the normal data segment (ds=e8a) they resolve to the same addresses anyway.
DSREL = {}
for _n, _p, _o, _a in syms:
    if _n.lower() in {'byte_24e70','byte_24e71','word_24e72','word_24e74',
                      'byte_24e76','word_24e77','word_24e79','byte_24e7b'}:
        DSREL[_n.lower()] = _o
DSREL.update({n.lower(): o for n, o in DSREL.items()})  # already lower

# dcomp_state member names for the ds-relative scratch block — semantic
# names (matching port/rewrite.c's usage) unioned with the raw map name.
DCOMP_MEMBER = {
    'byte_24e70': 'lim',      'byte_24e71': 'maxw',
    'word_24e72': 'mask',     'word_24e74': 'acc',
    'byte_24e76': 'npending', 'word_24e77': 'end',
    'word_24e79': 'prev',     'byte_24e7b': 'first',
}

# --- port/dseg.h: the data-segment layout as real structs ------------
# Symbols were historically `*(volatile T*)&mem[A]` aliases — a flat blob.
# The struct overlays mem[] one-to-one (offsetof == flat addr) so every
# define becomes a typed field access: `seg_data` -> `DSEG->seg.v_10000`.
# Field names keep the *map* name (v_-prefixed, lowercase): they stay
# stable when names.map renames semantics, and can never collide with an
# object-like macro (the token after -> must not be a macro name).
#
# Contiguous runs of >=3 fields become nested structs — named after the
# dominant semantic prefix (cam/mst/pri/...) or g_<addr> when unnamed.
# Every field stays an individual member (no array collapsing): each
# generated _Static_assert pins a leaf field to its address.
CODERE = re.compile(r'(sub_|loc_|locret_|nullsub_|start$|entry|.*_proc$)')
flat_syms = sorted(
    ((n, a, symtype.get(n, guess_type(n))) for n, p, o, a in syms
     if not CODERE.match(n) and n.lower() not in DSREL),
    key=lambda s: s[1])
sem_of = {n: NAMEMAP[n] for n, _, _ in flat_syms if n in NAMEMAP}
SZ = {'db': 1, 'dw': 2, 'dd': 4}

# contiguous runs (no padding between fields)
runs = []
for n, a, t in flat_syms:
    if runs and a == runs[-1][-1][1] + SZ[runs[-1][-1][2]]:
        runs[-1].append((n, a, t))
    else:
        runs.append([(n, a, t)])

# member path table: raw sym name -> accessor inside ar_dseg
# (flat: 'v_name', grouped: 'grp.v_name')
path_of = {}
groups = {}      # first-addr -> (name, [members])
used_gnames = set()
for r in runs:
    if len(r) < 3:
        for n, a, t in r:
            path_of[n] = f'v_{n.lower()}'
        continue
    # dominant semantic prefix names the group when it repeats
    pref = Counter(sem_of[n].split('_')[0] for n, _, _ in r if n in sem_of)
    gname = pref.most_common(1)[0][0] if pref and pref.most_common(1)[0][1] >= 2 \
        else f'g_{r[0][1]:x}'
    if gname in used_gnames:
        gname = f'{gname}_{r[0][1]:x}'
    used_gnames.add(gname)
    for n, a, t in r:
        path_of[n] = f'{gname}.v_{n.lower()}'
    groups[r[0][1]] = (gname, r)

with open(os.path.join(OUT, 'dseg.h'), 'w') as d:
    d.write('// data-segment layout overlaid on mem[] (generated)\n'
            '#pragma once\n#include <stddef.h>\n\n'
            '#pragma pack(push, 1)\nstruct ar_dseg {\n')
    prev_end = 0
    for r in runs:
        a0 = r[0][1]
        if a0 > prev_end:
            d.write(f'    db _pad_{prev_end:x}[0x{a0 - prev_end:x}];\n')
        if a0 in groups:
            gname, members = groups[a0]
            d.write(f'    struct {{ /* 0x{a0:x} */\n')
            for n, a, t in members:
                sem = f'  {sem_of[n]}' if n in sem_of else ''
                d.write(f'        {t} v_{n.lower()}; /* 0x{a:x}{sem} */\n')
            d.write(f'    }} {gname};\n')
            prev_end = r[-1][1] + SZ[r[-1][2]]
        else:
            for n, a, t in r:
                sem = f'  {sem_of[n]}' if n in sem_of else ''
                d.write(f'    {t} v_{n.lower()}; /* 0x{a:x}{sem} */\n')
                prev_end = a + SZ[t]
    d.write('};\n\n'
            '/* decompress_res scratch at ds:7FF0 — ds-relative (see\n'
            ' * gen_port.py DSREL note), so it gets its own overlay. */\n'
            'struct dcomp_state {\n')
    for raw, semn in DCOMP_MEMBER.items():
        t = symtype.get(raw, guess_type(raw))
        d.write(f'    union {{ {t} {semn}; {t} v_{raw}; }}; '
                f'/* ds:0x{DSREL[raw]:x} */\n')
    d.write('};\n#pragma pack(pop)\n\n'
            '#define DSEG  ((volatile struct ar_dseg *)mem)\n'
            '#define DCOMP ((volatile struct dcomp_state*)raddr_(ds,0x7ff0))\n\n')
    for n, a, t in flat_syms:
        d.write(f'_Static_assert(offsetof(struct ar_dseg, {path_of[n]}) '
                f'== 0x{a:x}, "{n}");\n')
    for raw, semn in sorted(DCOMP_MEMBER.items(), key=lambda kv: DSREL[kv[0]]):
        d.write(f'_Static_assert(offsetof(struct dcomp_state, {semn}) '
                f'== 0x{DSREL[raw] - 0x7ff0:x}, "{raw}");\n')
print(f'dseg.h: {len(flat_syms)} fields, {len(groups)} groups')

# --- port/data_syms.h ---
w = open(os.path.join(OUT, 'data_syms.h'), 'w')
w.write('// data symbol -> struct field aliases (lst-linear addressing)\n'
        '#pragma once\n#include "dseg.h"\n')
emitted = set()
for name, para, off, addr in syms:
    if CODERE.match(name):
        continue  # code addresses are real functions, not data
    if name.lower() in DSREL:
        w.write(f'#define {name} (DCOMP->v_{name.lower()})\n')
    else:
        w.write(f'#define {name} (DSEG->{path_of[name]})\n')
    emitted.add(name)
# externs without a map entry (dummy arrays etc) -> keep as real vars.
# Case-insensitive alias first: fake-C lowercases symbol names (word_1D934 ->
# word_1d934) so a map symbol may be emitted under a different case.
bylower = {}
for name, para, off, addr in syms:
    bylower.setdefault(name.lower(), (name, addr))
# extern names are already semantic (rename.py rewrote lifted_data.h), so a
# name like alert_lvl maps to several map symbols; emitting a define per
# hit reproduces the historical duplicate-#define stream — addr order keeps
# last-wins identical to the committed file (winners were the later/higher
# entry every time).
extra = []
for ln in open(os.path.join(AR, 'lifted', 'lifted_data.h')):
    m = re.match(r'extern (db|dw|dd) (\w+);', ln)
    if m and m.group(2) not in emitted:
        t, n = m.groups()
        hits = []
        for cn in [n] + NAME2OLDS.get(n, []):
            hit = bylower.get(cn.lower())
            if hit and not re.match(r'(sub_|loc_|locret_|nullsub_|start$|entry|.*_proc$)', hit[0]):
                hits.append(hit)
        emitted.add(n)
        if hits:
            for hn, ha in sorted(set(hits), key=lambda h: h[1]):
                if hn.lower() in DSREL:
                    w.write(f'#define {n} (DCOMP->v_{hn.lower()}) /* ds-rel alias {hn} */\n')
                else:
                    w.write(f'#define {n} (DSEG->{path_of[hn]}) /* alias {hn} */\n')
        else:
            extra.append(m.groups())
            w.write(f'extern {t} {n};\n')
w.close()
with open(os.path.join(OUT, 'data_defs.c'), 'w') as f:
    f.write('#include "rt.h"\n')
    for t, n in extra:
        f.write(f'{t} {n};\n')
print(f"data_syms: {len(emitted)} defines, {len(extra)} extern fallbacks")

# --- memimg.c: image bytes + fixups ---
# AR_rebuilt.exe is the authentic unpacked image — populated data everywhere.
# Its only defect: every `dw seg segNNN` field was collapsed to seg004's para
# (0x2803). We re-emit those 82 sites from the .lst+.map with correct paras.
# (AR_recovered.EXE is a runtime dump — its reused scratch regions are garbage.)
img = open(os.path.join(AR, 'AR_rebuilt.exe'), 'rb').read()[0x280:]  # 640B hdr
def segval(segname):  # para value for a segment name
    return segpara[segname]

# previous generated memimg.c, read BEFORE this run truncates it — the
# .cpp sources were pruned in ade9b1c, so the committed file carries the
# frozen fixup values and the tndoffs (overlay proc -> addr) table
_old_memimg = ''
_pm = os.path.join(OUT, 'memimg.c')
if os.path.exists(_pm):
    _old_memimg = open(_pm, encoding='utf8', errors='replace').read()

# parse MYCOPY fixups from fake-C source
fixups = []
mcre = re.compile(r'\{(\w+)\s+tmp\d*\s*=\s*(.*?);\s*MYCOPY\((\w+)\)\}')
found_cpp = False
for fn in glob.glob(os.path.join(AR, '*.cpp')) + glob.glob(os.path.join(AR, 'build_ida/src/*.cpp')):
    found_cpp = True
    for ln in open(fn, errors='replace'):
        mm = mcre.search(ln)
        if not mm: continue
        ty, expr, sym = mm.groups()
        if sym not in symaddr: continue
        # evaluate expr: seg_offset(segN), far_offset(segN,sym)[+-]N, plain int
        v = None
        e = expr.strip()
        m2 = re.fullmatch(r'seg_offset\((\w+)\)([+-]\d+)?', e)
        if m2:
            v = segval(m2.group(1)) + (int(m2.group(2) or 0))
        else:
            m2 = re.fullmatch(r'far_offset\((\w+),(\w+)\)([+-]\d+)?', e)
            if m2:
                segn, sym2, plus = m2.group(1), m2.group(2), m2.group(3)
                if sym2 in symaddr:
                    near = symaddr[sym2] - (0x1a20 + segstart.get('', 0))
                    # near offset = off within its map segment
                    for nn, pp, oo, aa in syms:
                        if nn == sym2: near = oo; break
                    v = ((segval(segn) << 16) | near) + int(plus or 0)
        if v is None:
            try: v = int(e, 0)
            except ValueError: continue
        fixups.append((symaddr[sym], ty, v, sym))
if not found_cpp:
    # the fake-C sources were pruned in ade9b1c; their fixup values are
    # frozen inputs, so harvest them back out of the generated file
    _infx = False
    for ln in _old_memimg.splitlines():
        if 'mem_apply_fixups' in ln: _infx = True; continue
        mm = re.match(r'\s+\*\((db|dw|dd)\*\)&mem\[0x([0-9a-f]+)\] = 0x([0-9a-f]+); /\* (\w+) \*/', ln)
        if _infx and mm and 'exe reloc' not in ln and 'dw seg' not in ln:
            fixups.append((int(mm.group(2), 16), mm.group(1), int(mm.group(3), 16), mm.group(4)))
print(f"fixups: {len(fixups)}")

w = open(os.path.join(OUT, 'memimg.c'), 'w')
w.write('// segment image + relocation fixups (generated)\n#include "rt.h"\n')
w.write(f'static const db img[{len(img)}] = {{\n')
for i in range(0, len(img), 16):
    w.write('  ' + ','.join(str(b) for b in img[i:i+16]) + ',\n')
w.write('};\n')
w.write('void mem_load_image(void){ memcpy(&mem[0x1a20], img, sizeof img); }\n')
# EXE MZ relocation table: word at img[seg*16+off] += load_para (0x1a2)
_exe = open(os.path.join(AR, 'AR_rebuilt.exe'), 'rb').read()
_hsize = int.from_bytes(_exe[8:10], 'little') * 16
_nreloc = int.from_bytes(_exe[6:8], 'little')
_reloff = int.from_bytes(_exe[0x18:0x1a], 'little')
relocs = []
for _i in range(_nreloc):
    _ro = int.from_bytes(_exe[_reloff+_i*4:_reloff+_i*4+2], 'little')
    _rs = int.from_bytes(_exe[_reloff+_i*4+2:_reloff+_i*4+4], 'little')
    relocs.append(_rs * 16 + _ro)
relocs_mem = {0x1a20 + r for r in relocs}
print(f"exe relocs: {len(relocs)}")

# .lst `dw seg segNNN` sites: rebuilt collapsed them to 0x2803; after the reloc
# pass (+0x1a2) overwrite each with the true runtime para (segstart/16 + 0x1a2).
segfix = []
for ln in open(os.path.join(AR, 'AR.EXE.lst'), errors='replace'):
    m = re.match(r'(seg\w+):([0-9A-Fa-f]+)\s+(?:\w+\s+)?dw\s+seg\s+(seg\w+)', ln)
    if m:
        src, so, tgt = m.groups()
        if src in segstart and tgt in segpara:
            segfix.append((segstart[src] + int(so, 16), segpara[tgt], f'{src}:{so} dw seg {tgt}'))
segfix.sort()
print(f"seg fixes (dw seg segNNN): {len(segfix)}")

w.write('void mem_apply_fixups(void){\n')
# exe relocs first (authoritative DOS loader semantics: word += load_para);
# skip fixups colliding with reloc addrs — the relocated raw value is truth.
for r in sorted(relocs_mem):
    w.write(f'  *(dw*)&mem[0x{r:x}] += 0x1a2; /* exe reloc img+{r - 0x1a20:x} */\n')
for a, v, c in segfix:
    w.write(f'  *(dw*)&mem[0x{0x1a20 + a:x}] = 0x{v:x}; /* {c} */\n')
for a, ty, v, sym in fixups:
    t = {'dw': 'dw', 'dd': 'dd', 'db': 'db'}.get(ty, 'dw')
    sz = {'dw': 2, 'dd': 4, 'db': 1}[t]
    if any((a + i) in relocs_mem for i in range(sz)): continue  # DOS reloc wins
    w.write(f'  *({t}*)&mem[0x{a:x}] = 0x{v:x}; /* {sym} */\n')
w.write('}\n')

# --- TANDYSND.EXE overlay: lifted sound driver ---
# Overlay image loads at runtime seg via EXEC param block; dos_exec records it
# in tnd_base. Overlay proc names are overlay-linear (IDA base 0x10000), and
# all code lives in overlay seg001 = image offset 0x70. Canonical fmap keys
# assume tnd_base=0x30000; func_at renormalizes when the load seg differs.
tndoffs = {}   # lifted proc name -> canonical mem addr (0x30070 + seg001 off)
_tndcpp = os.path.join(AR, 'tandysnd.exe_seg001.cpp')
if os.path.exists(_tndcpp):
    curlbl = None
    for ln in open(_tndcpp, errors='replace'):
        lm = re.match(r'\s*(\w+):\s*$', ln)
        if lm: curlbl = lm.group(1); continue
        mm = re.search(r';~ None:([0-9A-Fa-f]+)', ln)
        if mm and curlbl and curlbl not in tndoffs:
            tndoffs[curlbl] = 0x30070 + int(mm.group(1), 16)
else:
    # .cpp pruned in ade9b1c: recover name->addr from the generated fmap
    # (keys normalized to the pre-rename space so the TNDMAP join below
    # re-emits both forms exactly as before)
    for ln in _old_memimg.splitlines():
        mm = re.search(r'\{0x([0-9a-f]+), tw_tnd_(\w+)\}', ln)
        if mm:
            tndoffs[TND2OLD.get(mm.group(2), mm.group(2))] = int(mm.group(1), 16)

tsrc = open(os.path.join(AR, 'lifted', 'tandysnd.exe_seg001.c'),
            encoding='utf8', errors='replace').read()
tsrc = re.sub(r'#include "lifted[^"]*"\s*\n?', '', tsrc)
tsrc = re.sub(r'm2c::set_segment_register\((\w+),\s*([^;]+?)\)\s*;', r'\1 = \2;', tsrc)
tsrc = re.sub(r'offset\(seg(\d+),(\w+)\)',
              lambda m: '0x%x' % (int(m.group(2)[-5:], 16) - (0x10070 if m.group(1) == '001' else 0x10000)),
              tsrc)
tsrc = tsrc.replace('(dd)0x30070 +', 'tnd_cbase +')
# prefix overlay procs: loc_/sub_ names collide with the main exe's
for _n in sorted(set(re.findall(r'^void (\w+)\(void\)', tsrc, re.M)),
                 key=len, reverse=True):
    tsrc = re.sub(r'\b%s\s*\(' % re.escape(_n), 'tnd_%s(' % _n, tsrc)
tnames = re.findall(r'^void (\w+)\(void\)', tsrc, re.M)
# tndoffs keys are lifted names; a renamed lifted proc (edummylabel8 ->
# timer_isr) may appear in tsrc under either its raw label (tnd_edummylabel8)
# or its semantic name (tnd_timer_isr) depending on whether rename ran before
# gen_port. Key both forms to the same addr so the funmap join matches either
# way — running gen_port before or after rename must not drop ISR entries.
_tndoffs = {}
for _k, _v in tndoffs.items():
    _tndoffs['tnd_' + _k] = _v
    _tndoffs['tnd_' + TNDMAP.get(_k, _k)] = _v
tndoffs = _tndoffs

with open(os.path.join(OUT, 'tnd_syms.h'), 'w') as th:
    th.write('// TANDYSND overlay data symbols -> mem[tnd_base + image offset]\n#pragma once\n')
    for nm, off, ty in (('unk_1009f', 0x9f, 'db'), ('byte_100d7', 0xd7, 'db'),
                        ('byte_100d9', 0xd9, 'db'), ('off_10380', 0x380, 'dw'),
                        ('dword_103de', 0x3de, 'dd'), ('byte_103e2', 0x3e2, 'db')):
        th.write(f'#define {nm} (*(volatile {ty}*)&mem[tnd_base + 0x{off:x}])\n')
with open(os.path.join(OUT, 'tnd_procs.h'), 'w') as tp:
    tp.write('// TANDYSND overlay lifted procs\n#pragma once\n')
    for n in sorted(set(tnames)):
        tp.write(f'void {n}(void);\n')
open(os.path.join(OUT, 'gen', 'tandysnd_seg001.c'), 'w').write(
    '#include "../rt.h"\n#include "../tnd_syms.h"\n#include "../tnd_procs.h"\n' + tsrc)
print("gen: tandysnd_seg001.c")

# addr->function map for vector dispatch (ISRs) — lifted names are lst-linear
w.write('typedef void(*vfn)(void);\n')
w.write('typedef struct { dd addr; vfn f; } fent;\n')
fns = sorted(set(re.findall(r'^void (\w+)\(void\)', open(os.path.join(AR,'lifted','lifted_procs.h')).read(), re.M)))
fns = [n for n in fns if n not in ('__dispatch_call_ext', 'indirect_jump', 'swi')]
for n in fns:
    w.write('extern void %s(void);\n' % n)
for n in sorted(set(tnames)):
    w.write('extern void %s(void);\n' % n)
# overlay dispatch wrappers: overlay procs read `cs` (push cs/pop ds, vector
# install writes) — it must be the overlay code seg while they run
for n in sorted(set(tnames)):
    if n in tndoffs:
        w.write('static void tw_%s(void){ dw _ocs = cs; cs = tnd_cseg; %s(); cs = _ocs; }\n' % (n, n))
# overlay entry trampoline for seg001:0002 (`push cs; pop ds;` bytes IDA left
# undecoded — falls into seg001_4_proc = call sub_1040B; retf)
_tnd_entry = 'tnd_' + TNDMAP.get('seg001_4_proc', 'seg001_4_proc')
w.write('static void tnd_e0002(void){ dw _ocs = cs; cs = tnd_cseg; push(cs); ds = pop(); %s(); cs = _ocs; }\n' % _tnd_entry)
w.write('static void rt_nullfn(void){}\n')
w.write('static const fent fmap[] = {\n')
for n in fns:
    # renamed procs: addr comes from the old name. names.map chains are
    # multi-hop (hdr_walk_e10262 -> hdr_walk_e10262 -> hdr_walk_e10262) and
    # procs.h already holds the final name, so walk the chain to fixpoint.
    o, _seen = n, set()
    while o in NAME2OLD and o not in _seen:
        _seen.add(o); o = NAME2OLD[o]
    hit = bylower.get(o.lower())
    if hit and re.match(r'(sub_|loc_|locret_|nullsub_|start$|entry|.*_proc$)', hit[0]):
        w.write('  {0x%x, %s},\n' % (hit[1], n))
        continue
    m = re.match(r'(sub_|loc_|locret_|start$)([0-9a-fA-F]*)$', o)
    if m:
        if n == 'start':
            continue
        lin = int(m.group(2), 16)
        if lin >= 0x10000:  # lst name = maplin + 0x10000 ; mem = 0x1a20 + maplin
            w.write('  {0x%x, %s},\n' % (0x1a20 + (lin - 0x10000), n))
        continue
    m = re.match(r'seg(\d+)_([0-9a-fA-F]+)_proc$', o)
    if m:
        # IDA chunk-entry names: seg+offset -> mem addr via segment base
        sbase = {0: 0x1a20, 1: 0xe45f, 2: 0xe8a0, 3: 0x1cf30}.get(int(m.group(1)))
        if sbase:
            w.write('  {0x%x, %s},\n' % (sbase + int(m.group(2), 16), n))
        continue
    m = re.match(r'\w+_e([0-9a-fA-F]{5,})$', o)
    if m:
        # IDA chunk-entry suffix: e<lst-linear> is the entry's own address
        lin = int(m.group(1), 16)
        if lin >= 0x10000:
            w.write('  {0x%x, %s},\n' % (0x1a20 + (lin - 0x10000), n))
for n in sorted(set(tnames)):
    if n in tndoffs:
        w.write('  {0x%x, tw_%s},\n' % (tndoffs[n], n))
w.write('  {0x30072, tnd_e0002},\n')
w.write('};\n')
w.write('''vfn func_at(dd addr){
    if (!addr) return rt_nullfn;
    if (addr == 0xffea5) return (vfn)rt_bios_int8; /* BIOS int8 F000:FEA5 stub */
    if ((addr & ~0xfffffUL) == 0 && (mem[addr] == 0xC3 || mem[addr] == 0xCB))
        return rt_nullfn;   /* table slots holding a bare retn/retf = no-op */
    if (tnd_base && addr >= tnd_base && addr < tnd_base + 0x1000)
        addr = addr - tnd_base + 0x30000;
    for (unsigned i=0;i<sizeof fmap/sizeof*fmap;i++) if(fmap[i].addr==addr) return fmap[i].f;
    return 0; }
''')
w.close()
print("memimg.c + funmap written")

# --- port/gen_*.c: lifted bodies with port header ---
# procs that get an rt_tracef() probe injected at entry (M2C_TRACE env)
TRACE_PROCS = tuple(NAMEMAP.get(t, t) for t in
    ('load_resource', 'res_file_read', 'decompress_res', 'res_load_fail', 'open_res_file',
     'load_overlay', 'res_file_error', 'delay_3_ticks'))
# hand-written readable replacements in port/rewrite.c: the lifted body is
# renamed <name>_lifted (kept for A/B equivalence tests) while <name>()
# becomes a trampoline to the C version, so all callers — including the
# mid-function chunk entries — pick up the rewrite automatically.
REWRITES = {
    'decompress_res': 'decompress_res_c',
    'sprtab_init_a': 'sprtab_init_a_c',
    'sprtab_init_b': 'sprtab_init_b_c',
    # resource file I/O chain (seg000:07C3..0953) + error handler
    'load_resource': 'load_resource_c',
    'res_file_read': 'res_file_read_c',
    'res_file_read_e10846': 'res_file_read_e10846_c',
    'res_file_read_e10849': 'res_file_read_e10849_c',
    'seek_res_entry': 'seek_res_entry_c',
    'seek_res_entry_e108c1': 'seek_res_entry_e108c1_c',
    'open_res_file': 'open_res_file_c',
    'close_res_file': 'close_res_file_c',
    'res_load_fail': 'res_load_fail_c',
    'res_file_error': 'res_file_error_c',
    'res_file_error_e109ab': 'res_file_error_e109ab_c',
    'res_file_error_e109b7': 'res_file_error_e109b7_c',
    'res_file_error_e109d9': 'res_file_error_e109d9_c',
    'mode_rec_load': 'mode_rec_load_c',
    'write_res_file': 'write_res_file_c',
    'write_res_file_e1092d': 'write_res_file_e1092d_c',
    'write_res_file_e1092f': 'write_res_file_e1092f_c',
    'load_overlay': 'load_overlay_c',
    'load_overlay_e13ab6': 'load_overlay_e13ab6_c',
    'load_overlay_e13aca': 'load_overlay_e13aca_c',
    'load_overlay_e13af9': 'load_overlay_e13af9_c',
    'load_overlay_e13b08': 'load_overlay_e13b08_c',
    'load_overlay_e13b12': 'load_overlay_e13b12_c',
    'load_overlay_e13b46': 'load_overlay_e13b46_c',
    'load_overlay_e13b85': 'load_overlay_e13b85_c',
    'load_overlay_e13b9e': 'load_overlay_e13b9e_c',
    'load_overlay_e13baa': 'load_overlay_e13baa_c',
    # palette upload dispatch (seg000:036F..046C) + record walker
    'load_palette': 'load_palette_c',
    'load_palette_b': 'load_palette_b_c',
    'pal_upload': 'pal_upload_c',
    'load_palette_e103a5': 'load_palette_e103a5_c',
    'pal_upload_mcga': 'pal_upload_mcga_c',
    'pal_upload_mcga_e10421': 'pal_upload_mcga_e10421_c',
    'rec_walk_e10491': 'rec_walk_e10491_c',
    # mode-call dispatch (seg000:0797..083F)
    'glyph_conv_dispatch': 'glyph_conv_dispatch_c',
    'mode_call': 'mode_call_c',
    # render pacing pump (seg000:4ACD..4B0B)
    'render_tick': 'render_tick_c',
    'redraw_frame': 'redraw_frame_c',
    'redraw_frame_e14afe': 'redraw_frame_e14afe_c',
    # hit-flash timer (seg000:6D73..6D87)
    'hitflash_dec': 'hitflash_dec_c',
    'hitflash_dec_e16d7e': 'hitflash_dec_e16d7e_c',
    'hitflash_dec_e16d84': 'hitflash_dec_e16d84_c',
    # object slot tables + per-type tick dispatch (seg000:60E6..61C7, 891A..8966, 6B72..6B9F)
    'find_free_slot_b': 'find_free_slot_b_c',
    'find_free_slot_b_e160e9': 'find_free_slot_b_e160e9_c',
    'find_free_slot_b_e160f9': 'find_free_slot_b_e160f9_c',
    'obj_rec_clear': 'obj_rec_clear_c',
    'obj_tick_all': 'obj_tick_all_c',
    'obj_alive_mark': 'obj_alive_mark_c',
    'obj_alive_mark_e16b8b': 'obj_alive_mark_e16b8b_c',
    # AI weight/difficulty params (seg000:6D4E..6D72, 6D13..6D4D, 6B56..6B71)
    'slot_weight_sum': 'slot_weight_sum_c',
    'slot_weight_sum_e16d53': 'slot_weight_sum_e16d53_c',
    'slot_weight_sum_e16d5d': 'slot_weight_sum_e16d5d_c',
    'slot_weight_sum_e16d66': 'slot_weight_sum_e16d66_c',
    'ai_param_fetch': 'ai_param_fetch_c',
    'target_pri_decay': 'target_pri_decay_c',
    'farcall_ptr_a9e': 'farcall_ptr_a9e_c',
    'compose_frame_cond': 'compose_frame_cond_c',
    'compose_frame_cond_e11962': 'compose_frame_cond_e11962_c',
    'compose_frame_e11983': 'compose_frame_e11983_c',
    'compose_frame': 'compose_frame_c',
    'present_mcga': 'present_mcga_c',
    'flip_frame_e11a5c': 'flip_frame_e11a5c_c',
    'flip_mcga': 'flip_mcga_c',
    'flip_frame_e11b7a': 'flip_frame_e11b7a_c',
    'flip_frame': 'flip_frame_c',
    'vblank_wait_e11b84': 'vblank_wait_e11b84_c',
    'vblank_wait': 'vblank_wait_c',
    'compose_mcga': 'compose_mcga_c',
    'compose_mcga_11c0d': 'compose_mcga_11c0d_c',
    'locret_11c27': 'locret_11c27_c',
    'spr_state_copy_b': 'spr_state_copy_b_c',
    'set_border_color': 'set_border_color_c',
    'clear_backbuf': 'clear_backbuf_c',
    'adapter_compose_flip_e11c4c': 'adapter_compose_flip_e11c4c_c',
    'adapter_compose_flip_e11c53': 'adapter_compose_flip_e11c53_c',
    'adapter_compose_flip_e11c59': 'adapter_compose_flip_e11c59_c',
    'adapter_compose_flip': 'adapter_compose_flip_c',
    'compose_flip': 'compose_flip_c',
    'video_bufs_init': 'video_bufs_init_c',
    'video_bufs_setup': 'video_bufs_setup_c',
    'video_bufs_setup_e126c9': 'video_bufs_setup_e126c9_c',
    'clip_go_mcga': 'clip_go_mcga_c',
    'clip_go_mcga_e13047': 'clip_go_mcga_e13047_c',
    'clip_go_mcga_e130b0': 'clip_go_mcga_e130b0_c',
    'clip_go_mcga_e130b3': 'clip_go_mcga_e130b3_c',
    'mcga_dirty_update': 'mcga_dirty_update_c',
    'mcga_dirty_update_e1393c': 'mcga_dirty_update_e1393c_c',
    'mcga_dirty_update_e13943': 'mcga_dirty_update_e13943_c',
    'mcga_dirty_update_e13975': 'mcga_dirty_update_e13975_c',
    'hud_weapon_update': 'hud_weapon_update_c',
    'hud_weapon_update_e17b22': 'hud_weapon_update_e17b22_c',
    'hud_weapon_update_e17b38': 'hud_weapon_update_e17b38_c',
    'hud_weapon_update_e17b4a': 'hud_weapon_update_e17b4a_c',
    'hud_weapon_update_e17bc5': 'hud_weapon_update_e17bc5_c',
    'fx_overlay_fill': 'fx_overlay_fill_c',
    'fx_overlay_fill_e17cfd': 'fx_overlay_fill_e17cfd_c',
    'fx_overlay_fill_e17d0e': 'fx_overlay_fill_e17d0e_c',
    'fx_overlay_fill_e17d32': 'fx_overlay_fill_e17d32_c',
}
# Commit 4366e7c removed the gfx driver-selection menu ('1'..'5' getch
# loop → forced MCGA pick) by hand-editing the generated file, so every
# regen lost it and the interactive menu came back — which stalls the
# e2e key script on the wrong screen. Reproduce the excision here: cut
# the emitted statements belonging to asm range [01A2:065A, 01A2:0673)
# in whichever proc contains them, preserving the committed text.
def excise_driver_menu(src):
    fn = re.compile(r'^void \w+\(void\) \{.*?^\}', re.M | re.S)
    off = re.compile(r';~ 01A2:([0-9A-F]{4})')
    def cut(m):
        lines = m.group(0).split('\n')
        offs = {i: int(mm.group(1), 16)
                for i, ln in enumerate(lines) for mm in [off.search(ln)] if mm}
        inr = [i for i, o in offs.items() if 0x065A <= o < 0x0673]
        if not inr:
            return m.group(0)
        lo = min(inr)
        while lines[lo - 1].strip() in ('do {', '{'):
            lo -= 1                       # absorb orphaned loop openers
        hi = min(i for i, o in offs.items() if i > lo and o >= 0x0673)
        kept = [ln for ln in lines[lo:hi] if re.match(r'^\w+:$', ln)]
        note = ('    /* driver menu removed — no text-table draw / cursor set */\n'
                if offs[min(inr)] == 0x065A else '')
        lines[lo:hi] = [note + ''.join(l + '\n' for l in kept) +
                        "    /* driver menu removed — MCGA only, al = '4' pick */\n"
                        '    al = 0x34;']
        return '\n'.join(lines)
    return fn.sub(cut, src)
os.makedirs(os.path.join(OUT, 'gen'), exist_ok=True)
for f in glob.glob(os.path.join(AR, 'lifted', 'ar.exe*.c')):
    src = open(f, encoding='utf8', errors='replace').read()
    src = re.sub(r'#include "lifted[^"]*"\s*\n?', '', src)
    for old, new in REWRITES.items():
        src = src.replace(f'void {old}(void) {{',
                          f'void {old}(void) {{ {new}(); }}\nvoid {old}_lifted(void) {{', 1)
    src = excise_driver_menu(src)
    for tp in TRACE_PROCS:
        src = src.replace(f'void {tp}(void) {{', f'void {tp}(void) {{ rt_tracef("{tp}");')
    base = os.path.basename(f)
    open(os.path.join(OUT, 'gen', base), 'w').write(
        '#include "../rt.h"\n#include "../data_syms.h"\n#include "../procs.h"\n' + src)
    print("gen:", base)
