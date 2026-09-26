#!/usr/bin/env python3
"""Recover the original AR.EXE from the memory image (orig_image.bin).

orig_image.bin is the loaded (relocated) program image.  Segment words at
relocation sites hold <stored_para> + <load_para>; subtract load_para to get
the stored file value.  Reloc sites are located by scanning the .lst for the
'seg <name>' operator (and the hand-mapped dd table), whose word occupies the
last 2 bytes of that line's byte span.
"""
import re, struct, sys

LST = "AR.EXE.lst"
IMG = "orig_image.bin"
LOAD_PARA = 0x1A2  # relocated - stored, consistent across seg table

IMG_BASE = {"seg000": 0x00000, "seg001": 0x0CA3F, "seg002": 0x0CE80, "seg003": 0x1B510}
SEG_END  = {"seg000": 0x0CA3F, "seg001": 0x0CE80, "seg002": 0x1B510, "seg003": 0x28030}

LINE_RE = re.compile(r"^(seg\d+):([0-9A-Fa-f]{4,8})(?:[ \t]+(.*))?$")

def lines():
    out = []
    for ln in open(LST, encoding="utf-8", errors="replace").read().splitlines():
        m = LINE_RE.match(ln)
        if m:
            out.append((m.group(1), int(m.group(2), 16), (m.group(3) or "")))
    return out

def span_lengths(items):
    """orig byte length of each line = offset delta to next line in same seg."""
    res = []
    n = len(items)
    for i, (seg, off, txt) in enumerate(items):
        nxt = None
        for j in range(i + 1, n):
            if items[j][0] == seg and items[j][1] > off:
                nxt = items[j][1]
                break
            if items[j][0] is not None and items[j][0] != seg:
                break
        if seg not in IMG_BASE:
            res.append(0)
            continue
        end = nxt if nxt is not None else SEG_END[seg]
        res.append(max(0, end - off))
    return res

# does this line's bytes contain a relocated segment word (as last 2 bytes)?
RELOC_RE = re.compile(
    r"\bseg\s+seg\w"                 # 'seg segNNN' operator
    r"|\b(call|jmp)\s+far\s+ptr\s+[A-Za-z_]"  # far call/jmp to label: seg word
    r"|\bdd\s+[A-Za-z_]"             # dd <label>: far pointer, seg in hi word
    r"|,\s*seg\s+seg\w", re.I)
# extra dd sites whose high word is a relocated segment (from RELOC_DD map)
EXTRA_DD = {("seg002", o) for o in (0x03D8,0x0402,0x0408,0x0438,0x0462,0x0618,0x0642,0x0648,0x0672,0x0678,0x0738)}

def main():
    items = lines()
    lens = span_lengths(items)
    img = bytearray(open(IMG, "rb").read())
    fileimg = bytearray(img)
    relocs = []
    for (seg, off, txt), ln in zip(items, lens):
        if seg is None or ln == 0:
            continue
        base = IMG_BASE[seg]
        has_reloc = bool(RELOC_RE.search(txt)) or (seg, off) in EXTRA_DD
        if not has_reloc:
            continue
        # reloc word = last 2 bytes of this line's span
        pos = base + off + ln - 2
        v = struct.unpack("<H", img[pos:pos+2])[0]
        # only subtract when it plausibly is a relocated para (>= load base)
        stored = v - LOAD_PARA
        struct.pack_into("<H", fileimg, pos, stored & 0xFFFF)
        relocs.append(pos - off - (ln - 2) + off)  # == pos
    # build header
    pages = (0x280 + len(fileimg) + 511) // 512
    last = (0x280 + len(fileimg)) % 512
    hdr = bytearray(0x280)
    struct.pack_into("<H", hdr, 0x00, 0x5A4D)          # MZ
    struct.pack_into("<H", hdr, 0x02, last)            # bytes in last page
    struct.pack_into("<H", hdr, 0x04, pages)           # pages
    struct.pack_into("<H", hdr, 0x06, len(relocs))     # reloc count
    struct.pack_into("<H", hdr, 0x08, 0x28)            # header paras
    struct.pack_into("<H", hdr, 0x0A, 0x1B20)          # minalloc
    struct.pack_into("<H", hdr, 0x0C, 0xFFFF)          # maxalloc
    struct.pack_into("<H", hdr, 0x0E, 0x1B51)          # SS (rel to load)
    struct.pack_into("<H", hdr, 0x10, 0xCB14)          # SP
    struct.pack_into("<H", hdr, 0x12, 0)               # checksum
    struct.pack_into("<H", hdr, 0x14, 0x0000)          # IP
    struct.pack_into("<H", hdr, 0x16, 0x0020)          # CS (rel to load)
    struct.pack_into("<H", hdr, 0x18, 0x1C)            # reloc table offset
    struct.pack_into("<H", hdr, 0x1A, 0)               # overlay
    # reloc entries: seg:off form (seg relative to load base)
    for i, pos in enumerate(sorted(relocs)):
        struct.pack_into("<HH", hdr, 0x1C + i*4, pos & 0xF, pos >> 4)
    open("AR_recovered.EXE", "wb").write(bytes(hdr) + bytes(fileimg))
    import hashlib
    h = hashlib.sha256(bytes(hdr) + bytes(fileimg)).hexdigest()
    print("relocs:", len(relocs), "sha256:", h)
    print("expect : EB37FD883BDD135A2D5714C40570EF74FCFCC460F65402AD3EB39F9903338B2D")

main()
