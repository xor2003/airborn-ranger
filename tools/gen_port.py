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

AR = '/home/xor/games/airborn'
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

# --- port/data_syms.h ---
w = open(os.path.join(OUT, 'data_syms.h'), 'w')
w.write('// data symbol -> mem[] aliases (lst-linear addressing)\n#pragma once\n')
emitted = set()
for name, para, off, addr in syms:
    if re.match(r'(sub_|loc_|locret_|nullsub_|start$|entry|.*_proc$)', name):
        continue  # code addresses are real functions, not data
    t = symtype.get(name, guess_type(name))
    w.write(f'#define {name} (*(volatile {t}*)&mem[0x{addr:x}])\n')
    emitted.add(name)
# externs without a map entry (dummy arrays etc) -> keep as real vars.
# Case-insensitive alias first: fake-C lowercases symbol names (word_1D934 ->
# word_1d934) so a map symbol may be emitted under a different case.
bylower = {}
for name, para, off, addr in syms:
    bylower.setdefault(name.lower(), (name, addr))
extra = []
for ln in open(os.path.join(AR, 'lifted', 'lifted_data.h')):
    m = re.match(r'extern (db|dw|dd) (\w+);', ln)
    if m and m.group(2) not in emitted:
        t, n = m.groups()
        hit = bylower.get(n.lower())
        if hit and not re.match(r'(sub_|loc_|locret_|nullsub_|start$|entry|.*_proc$)', hit[0]):
            w.write(f'#define {n} (*(volatile {t}*)&mem[0x{hit[1]:x}]) /* alias {hit[0]} */\n')
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

# parse MYCOPY fixups from fake-C source
fixups = []
mcre = re.compile(r'\{(\w+)\s+tmp\d*\s*=\s*(.*?);\s*MYCOPY\((\w+)\)\}')
for fn in glob.glob(os.path.join(AR, '*.cpp')) + glob.glob(os.path.join(AR, 'build_ida/src/*.cpp')):
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
curlbl = None
for ln in open(os.path.join(AR, 'tandysnd.exe_seg001.cpp'), errors='replace'):
    lm = re.match(r'\s*(\w+):\s*$', ln)
    if lm: curlbl = lm.group(1); continue
    mm = re.search(r';~ None:([0-9A-Fa-f]+)', ln)
    if mm and curlbl and curlbl not in tndoffs:
        tndoffs[curlbl] = 0x30070 + int(mm.group(1), 16)

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
tndoffs = {'tnd_' + k: v for k, v in tndoffs.items()}

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
w.write('static void tnd_e0002(void){ dw _ocs = cs; cs = tnd_cseg; push(cs); ds = pop(); tnd_seg001_4_proc(); cs = _ocs; }\n')
w.write('static void rt_nullfn(void){}\n')
w.write('static const fent fmap[] = {\n')
for n in fns:
    hit = bylower.get(n.lower())
    if hit and re.match(r'(sub_|loc_|locret_|nullsub_|start$|entry|.*_proc$)', hit[0]):
        w.write('  {0x%x, %s},\n' % (hit[1], n))
        continue
    m = re.match(r'(sub_|loc_|locret_|start$)([0-9a-fA-F]*)$', n)
    if m:
        if n == 'start':
            continue
        lin = int(m.group(2), 16)
        if lin >= 0x10000:  # lst name = maplin + 0x10000 ; mem = 0x1a20 + maplin
            w.write('  {0x%x, %s},\n' % (0x1a20 + (lin - 0x10000), n))
        continue
    m = re.match(r'seg(\d+)_([0-9a-fA-F]+)_proc$', n)
    if m:
        # IDA chunk-entry names: seg+offset -> mem addr via segment base
        sbase = {0: 0x1a20, 1: 0xe45f, 2: 0xe8a0, 3: 0x1cf30}.get(int(m.group(1)))
        if sbase:
            w.write('  {0x%x, %s},\n' % (sbase + int(m.group(2), 16), n))
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
TRACE_PROCS = ('sub_107c3', 'sub_10841', 'sub_13998', 'loc_108dd', 'sub_108c6',
               'sub_13a94', 'sub_109a4', 'sub_1071e')
os.makedirs(os.path.join(OUT, 'gen'), exist_ok=True)
for f in glob.glob(os.path.join(AR, 'lifted', 'ar.exe*.c')):
    src = open(f, encoding='utf8', errors='replace').read()
    src = re.sub(r'#include "lifted[^"]*"\s*\n?', '', src)
    for tp in TRACE_PROCS:
        src = src.replace(f'void {tp}(void) {{', f'void {tp}(void) {{ rt_tracef("{tp}");')
    base = os.path.basename(f)
    open(os.path.join(OUT, 'gen', base), 'w').write(
        '#include "../rt.h"\n#include "../data_syms.h"\n#include "../procs.h"\n' + src)
    print("gen:", base)
