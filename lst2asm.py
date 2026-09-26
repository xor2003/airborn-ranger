#!/usr/bin/env python3
"""Convert IDA .lst listing to MASM-compatible .asm for uasm reassembly.

Handles:
- strips 'segNNN:OOOO ' address prefixes (tracks them for align padding)
- 'align N' -> 'db <pad> dup(0)' computed from next line offset
- strips IDA comment continuation lines that would confuse the assembler
"""
import re
import sys

LINE_RE = re.compile(r"^(seg\d+):([0-9A-Fa-f]{4,8})(?:[ \t]+(.*))?$")
# lines without offset prefix (pure comments/blank) are passed through

# image offset of each segment in the EXE load module (from AR.EXE.map)
IMG_BASE = {"seg000": 0x00000, "seg001": 0x0CA3F, "seg002": 0x0CE80, "seg003": 0x1B510}
SEG_END  = {"seg000": 0x0CA3F, "seg001": 0x0CE80, "seg002": 0x1B510, "seg003": 0x28030}

# lines whose final word is a relocated segment (don't byte-compare / db them)
RELOC_RE = re.compile(
    r"\bseg\s+seg\w|\b(call|jmp)\s+far\s+ptr\s+[A-Za-z_]|\bdd\s+[A-Za-z_]|,\s*seg\s+seg\w", re.I)
# directives/labels that emit no bytes
NOBYTE_RE = re.compile(
    r"^([A-Za-z_]\w*:|\s*$|;|\.|assume\b|.*\b(proc|endp|ends|segment|label|public|extrn|equ|include|org|group)\b)",
    re.I)

# --- Reassembly fixups -----------------------------------------------------
# IDA emitted some segment-relative offsets as bare numeric literals.  After
# reassembly the labels they point at land on different offsets, so the
# literal goes stale.  Map each such instruction to the real target; a
# synthetic label (afx_<seg>_<off>) is emitted at the target line and the
# operand becomes 'offset afx_<seg>_<off>[+delta]'.
#
#   (seg, insn_off) -> (literal_text, target_seg, target_off)
OFFSET_FIXUPS = {
    ("seg000", 0x033D): ("100h",  "seg002", 0x0100),  # rep movsb source (ds:)
    ("seg000", 0x0287): ("100h",  "seg002", 0x0100),  # di -> [di+18h] table (ds:)
    ("seg000", 0x048E): ("0FCh",  "seg002", 0x00FC),  # mov si,[di] ptr table (ds:)
    ("seg000", 0x06C8): ("1B52h", "seg002", 0x1B52),  # mov al,[si] (ds:)
    ("seg000", 0x09A8): ("13Eh",  "seg002", 0x013E),  # mov [di],al dest (ds:)
    ("seg000", 0x149E): ("1484h", "seg000", 0x1484),  # cs: scratch cell (old int8)
    ("seg000", 0x14C2): ("1484h", "seg000", 0x1484),
    ("seg000", 0x153E): ("1488h", "seg000", 0x1488),  # cs: scratch cell (int8)
    ("seg000", 0x155D): ("1488h", "seg000", 0x1488),
    ("seg000", 0x2553): ("2599h", "seg000", 0x2599),  # cs: scratch cell (int9)
    ("seg000", 0x2581): ("2599h", "seg000", 0x2599),
    ("seg000", 0x260E): ("259Dh", "seg000", 0x259D),  # cs: scratch cell
    ("seg000", 0x14A8): ("14D6h", "seg000", 0x14D6),  # int1C handler offset
    ("seg000", 0x261A): ("2643h", "seg000", 0x2643),  # int9 handler offset
    ("seg000", 0x15DF): ("1B52h", "seg002", 0x1B52),  # mov word ptr ds:0A96h,imm
    ("seg000", 0x7C53): ("139h",  "seg002", 0x0139),  # bp -> [bp] table (ds:)
}

# Data directives whose bytes contain a relocated segment word that the .lst
# shows as a plain literal.  Emit 'dw <low16>, seg <name>' so the linker emits
# the relocation.  (seg, off) -> segment name for the high word.
# Values verified against the original EXE relocation table.
RELOC_DD = {
    ("seg002", 0x03D8): "seg004",
    ("seg002", 0x0402): "seg005",
    ("seg002", 0x0408): "seg004",
    ("seg002", 0x0438): "seg004",
    ("seg002", 0x0462): "seg005",
    ("seg002", 0x0618): "seg004",
    ("seg002", 0x0642): "seg005",
    ("seg002", 0x0648): "seg004",
    ("seg002", 0x0672): "seg005",
    ("seg002", 0x0678): "seg004",
    ("seg002", 0x0738): "seg004",
}


def main(infile, outfile, origfile=None, dbfixfile=None):
    with open(infile, encoding="utf-8", errors="replace") as f:
        raw = f.read().splitlines()

    orig_img = None
    if origfile:
        orig_img = open(origfile, "rb").read()
    dbfix = set()
    if dbfixfile:
        for tok in open(dbfixfile).read().split():
            s, o = tok.split(":")
            dbfix.add((s, int(o, 16)))

    # first pass: collect (seg, off, text) for lines that have an offset
    items = []  # (seg, off_int, text)
    for ln in raw:
        m = LINE_RE.match(ln)
        if m:
            items.append((m.group(1), int(m.group(2), 16), m.group(3)))
        else:
            items.append((None, None, ln))

    # span of each line = offset delta to the next *greater* offset in its seg
    n = len(items)
    spans = [0] * n
    for i, (seg, off, _t) in enumerate(items):
        if seg is None or seg not in IMG_BASE:
            continue
        nxt = None
        for j in range(i + 1, n):
            if items[j][0] == seg and items[j][1] > off:
                nxt = items[j][1]
                break
            if items[j][0] is not None and items[j][0] != seg:
                break
        spans[i] = (nxt if nxt is not None else SEG_END[seg]) - off

    # Resolve fixup targets: for each (seg,off) target find the line that
    # contains it (greatest line offset <= off in that segment) and prepare a
    # label to emit there.
    target_label = {}   # (seg, line_off) -> label name
    pending_labels = {} # (seg, line_off) -> [names]
    for (iseg, ioff), (lit, tseg, toff) in OFFSET_FIXUPS.items():
        line_off = None
        for s2, o2, _t2 in items:
            if s2 != tseg or o2 is None:
                continue
            if o2 > toff:
                break
            line_off = o2
        if line_off is None:
            continue
        key = (tseg, line_off)
        if key not in target_label:
            # reuse an existing label on that line when present
            ttxt = next((t for s2, o2, t in items if s2 == tseg and o2 == line_off), "") or ""
            lm = re.match(r"^([A-Za-z_]\w*)\s*:", ttxt.strip()) or \
                 re.match(r"^([A-Za-z_]\w*)\s+(proc|label|db|dw|dd|endp)\b", ttxt.strip())
            name = lm.group(1) if lm else f"afx_{tseg}_{line_off:04x}"
            target_label[key] = name
            if not lm:  # existing labels need no extra label line
                pending_labels.setdefault(key, []).append(name)

    emitted_labels = set()
    out = []
    n = len(items)
    for i, (seg, off, text) in enumerate(items):
        if seg is None:
            out.append(text)
            continue

        stripped = (text or "").strip()

        # emit synthetic anchor labels for fixup targets (once per address)
        for name in pending_labels.get((seg, off), []):
            if name not in emitted_labels:
                emitted_labels.add(name)
                out.append(f"{name}:")

        span = spans[i]
        orig_bytes = None
        if orig_img is not None and seg in IMG_BASE and span > 0:
            orig_bytes = orig_img[IMG_BASE[seg] + off: IMG_BASE[seg] + off + span]

        # forced byte-exact emission for encodings that differ from original
        if ((seg, off) in dbfix and orig_bytes and stripped
                and not stripped.startswith(";") and not NOBYTE_RE.match(stripped)):
            out.append("db " + ",".join(f"0{b:02X}h" for b in orig_bytes)
                       + " ; " + stripped)
            continue

        # position marker for the encoding checker (stripped by assembler)
        if (orig_img is not None and stripped and not stripped.startswith(";")
                and not NOBYTE_RE.match(stripped)):
            if RELOC_RE.search(stripped):
                cls = "r"           # reloc word: uasm emits it, don't compare
            elif re.search(r"far\s+ptr\s+[0-9A-Fa-f]+h?\s*:", stripped, re.I):
                cls = "r"           # literal seg:off far jump: self-modified
            elif re.match(r"^(\w+\s+)?(db|dw|dd|dt|dq|dp)\b", stripped, re.I):
                cls = "d"           # data directive: literals are exact
            else:
                cls = "i"           # instruction: bytes must match original
            out.append(f";@O {seg}:{off:X}:{span}:{cls}")

        # dd whose high word must relocate -> 'dw low, seg <name>'
        if (seg, off) in RELOC_DD:
            dm = re.match(r"^(\w+\s+)?dd\s+([0-9A-Fa-f]+)h\s*(;.*)?$", stripped)
            if dm:
                low = int(dm.group(2), 16) & 0xFFFF
                label = (dm.group(1) or "")
                cmt = dm.group(3) or ""
                out.append(f"{label}dw {low}h, seg {RELOC_DD[(seg, off)]} {cmt}")
            else:
                out.append(text or "")
            continue

        # bare literal that is really a segment offset -> 'offset label[+d]'
        if (seg, off) in OFFSET_FIXUPS:
            lit, tseg, toff = OFFSET_FIXUPS[(seg, off)]
            # containing line for the target
            line_off = None
            for s2, o2, _t2 in items:
                if s2 != tseg or o2 is None:
                    continue
                if o2 > toff:
                    break
                line_off = o2
            lbl = target_label.get((tseg, line_off)) if line_off is not None else None
            if lbl:
                delta = toff - line_off
                op = f"offset {lbl}" + (f"+{delta}" if delta else "")
                code, sep, comment = (text or "").partition(";")
                code = re.sub(r"\b" + re.escape(lit) + r"\b", op, code, count=1)
                text = code + (sep + comment if sep else "")
            out.append(text or "")
            continue


        # align N -> explicit zero padding to next listed offset
        am = re.match(r"^align\s+([0-9A-Fa-f]+)h?\s*(;.*)?$", stripped, re.I)
        if am:
            boundary = int(am.group(1), 16) if am.group(1)[-1] in "hH" or am.group(1).isalpha() is False and any(c in "abcdefABCDEF" for c in am.group(1)) else int(am.group(1), 16) if am.group(1).lower().endswith("h") else int(am.group(1), 10) if am.group(1).isdigit() else int(am.group(1), 16)
            # simpler: IDA writes hex possibly without h? check: it writes 'align 10h', 'align 4'
            s = am.group(1)
            boundary = int(s[:-1], 16) if s.lower().endswith("h") else int(s, 10)
            # find next line with offset in same segment
            pad = None
            for j in range(i + 1, n):
                nseg, noff, ntext = items[j]
                if nseg is None:
                    continue
                nts = (ntext or "").strip()
                if nts.startswith(";"):
                    continue
                if nseg != seg:
                    pad = 0
                    break
                # stop also at 'ends'
                if re.match(r"^\w+\s+ends\b", nts):
                    pad = 0
                    break
                pad = noff - off
                if pad < 0:
                    pad = 0
                # sanity: never pad more than boundary-1... trust listing gap anyway
                break
            if pad:
                out.append(f"db {pad} dup(0) ; was align {boundary}")
            else:
                out.append(f"; was align {boundary} (no pad)")
            continue

        # Drop 'short' from jumps: uasm then picks near form when the target
        # has drifted out of the -128..127 window (encodings differ from the
        # original assembler, so some short jumps would otherwise overflow).
        sm = re.match(r"^(\w+\s+)?(j\w+|jmp)\s+short\s+(\w+)(.*)$", stripped, re.I)
        if sm:
            text = re.sub(r"\bshort\s+", "", text, count=1)
            stripped = re.sub(r"\bshort\s+", "", stripped, count=1)

        # jmp far ptr SEG:OFF -> EA imm32 (uasm rejects literal far constants)
        jm = re.match(r"^(\w+\s+)?jmp\s+far\s+ptr\s+([0-9A-Fa-f]+)h?\s*:\s*([0-9A-Fa-f]+)h?\s*(;.*)?$", stripped, re.I)
        if jm:
            label = jm.group(1) or ""
            segv = int(jm.group(2).rstrip("hH"), 16)
            offv = int(jm.group(3).rstrip("hH"), 16)
            out.append(f"{label}db 0EAh ; jmp far ptr {segv:04X}:{offv:04X}")
            out.append(f"dw {offv}, {segv}")
            continue

        out.append(text or "")

    with open(outfile, "w", encoding="ascii", errors="replace") as f:
        f.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main(sys.argv[1], sys.argv[2],
         sys.argv[3] if len(sys.argv) > 3 else None,
         sys.argv[4] if len(sys.argv) > 4 else None)
