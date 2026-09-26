#!/usr/bin/env python3
"""Compare uasm-assembled bytes against the original image per lst line.

Reads the uasm -Fl listing (which preserves the ';@O seg:off:span' markers
lst2asm emits) and reports instructions whose bytes differ from the original.
Output: dbfix tokens 'seg:off' for lst2asm's db-fallback pass.

Usage: check_encoding.py <uasm.lst> <orig_image.bin> [--all]
"""
import re, sys

IMG_BASE = {"seg000": 0x00000, "seg001": 0x0CA3F, "seg002": 0x0CE80, "seg003": 0x1B510}
MARK_RE = re.compile(r";@O (seg\d+):([0-9A-F]+):([0-9A-F]+):([idr])")
# uasm listing byte line:  '00000012  8B D8 90        mov ...'
BYTE_RE = re.compile(r"^\s*[0-9A-Fa-f]{6,8}\s+((?:[0-9A-Fa-f]{2}\s*)+)\s")
# lines we never flag: reloc-word lines + literal far jumps (self-modified)
SKIP_RE = re.compile(
    r"\bseg\s+seg\w|\b(call|jmp)\s+far\s+ptr|\bdd\s+[A-Za-z_]|\b(dd|dw|db|dt|dq)\b",
    re.I)

def main():
    lstf, imgf = sys.argv[1], sys.argv[2]
    img = open(imgf, "rb").read()
    cur = None          # (seg, off, span, cls)
    acc = bytearray()
    bad = []
    def flush():
        if cur is None or cur[0] not in IMG_BASE or cur[3] != "i":
            return
        seg, off, span, _ = cur
        orig = img[IMG_BASE[seg] + off: IMG_BASE[seg] + off + span]
        if bytes(acc) != orig:
            bad.append((seg, off, span, bytes(acc), orig))

    for raw in open(lstf, encoding="utf-8", errors="replace").read().splitlines():
        m = MARK_RE.search(raw)
        if m:
            flush()
            cur = (m.group(1), int(m.group(2), 16), int(m.group(3), 16), m.group(4))
            acc = bytearray()
            continue
        bm = BYTE_RE.match(raw)
        if bm and cur is not None:
            acc += bytes.fromhex(re.sub(r"\s", "", bm.group(1)))
    flush()

    for seg, off, span, got, want in bad:
        print(f"{seg}:{off:X}  span={span} got={got.hex()} want={want.hex()}")
    print(f"{len(bad)} mismatched lines", file=sys.stderr)

main()
