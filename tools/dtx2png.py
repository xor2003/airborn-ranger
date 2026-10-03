#!/usr/bin/env python3
"""dtx2png — extract Airborne Ranger .DTX resources to PNG.

.DTX payload = MicroProse LZW (reverse-engineered from the decompiled
decompress_res @ ar.exe_seg000.c):
  byte 0     max code width; dictionary silently resets above it
  byte 1..   codes packed LSB-first (GIF-style), initial width 9
  dict:      literals 0-255; entries >= 0x100 = (parent, last_byte);
             one entry added per decoded code including the first

Verified payload layouts (from the exe's resource table + blit code):
  *CHR.DTX   u16 tile_count, u32 pad, 60-byte fill/pattern block,
             then tile_count 8x8 tiles of 32 bytes (4 bytes/row,
             4bpp packed, hi nibble = left px); tiles start at +66
             (game stages CHR at ds:3EE0, glyph_rows = ds:3F22)
  *SCR.DTX   40x25 cell map of u16 tile indices into the companion
             *CHR bank (menu screens); region maps are record streams
  COL*.DTX   per-adapter LUT/remap tables staged at ds:3B70; the
             +0x100/+0x200 byte->2px LUTs are identity for MCGA
             (no RGB data here — the MCGA palette is the fixed
             16-colour table at ds:00B8/C8/D8 in the exe image)
  *COL_*.DTX theatre remap tables
  *SPR.DTX   u16 offset table -> variable-size sprite records (WIP)
  MAP*.DTX   campaign map data (dumped raw)

Usage:
  dtx2png.py [-o OUTDIR] FILE.DTX [...]
  dtx2png.py --dump FILE.DTX            # decompressed .bin only
"""

import os
import struct
import sys
import zlib

# ------------------------------------------------------------------ LZW

def decompress(data: bytes) -> bytes:
    """Faithful port of decompress_res / sprtab_init."""
    if len(data) < 2:
        return b""
    out = bytearray()
    parent = [-1] * 0x800
    char = list(range(256)) + [0] * (0x800 - 256)

    maxw = data[0]
    pos = 2
    acc = data[1]          # e76 = 8 pending bits = file[1]
    nbits = 8
    width = 9
    mask = 0x1FF
    next_code = 0x100
    prev_code = 0
    prev_first = 0
    end = len(data)

    while True:
        while nbits < width:
            if pos + 1 < end:
                w = data[pos] | (data[pos + 1] << 8)
            elif pos < end:
                w = data[pos]
            else:
                w = 0
            pos += 2
            acc |= w << nbits
            nbits += 16
        code = acc & mask
        acc >>= width
        nbits -= width

        # expand string (KwKwK handled via prev_code/prev_first)
        if code >= next_code:
            seq = [prev_first]
            walk = prev_code
            cx = next_code
        else:
            seq = []
            walk = code
            cx = code
        while 0 <= walk < 0x800 and parent[walk] != -1:
            seq.append(char[walk])
            walk = parent[walk]
        first = char[walk] if 0 <= walk < 0x800 else 0
        prev_first = first
        out.append(first)
        out.extend(reversed(seq))

        # dict grows unconditionally (mirrors the asm)
        if next_code < 0x800:
            char[next_code] = first
            parent[next_code] = prev_code
        next_code += 1
        if next_code > mask:
            width += 1
            mask = (mask << 1) | 1
        if width > maxw:                    # silent reset
            width = 9
            mask = 0x1FF
            next_code = 0x100
            parent = [-1] * 0x800
            char = list(range(256)) + [0] * (0x800 - 256)
        prev_code = cx
        if pos >= end and nbits < width:
            break
    return bytes(out)

# ------------------------------------------------------------------ PNG

def png_write(path, w, h, rgba):
    def chunk(tag, data):
        c = struct.pack(">I", len(data)) + tag + data
        return c + struct.pack(">I", zlib.crc32(tag + data) & 0xFFFFFFFF)
    raw = b"".join(b"\x00" + bytes(rgba[y * w * 4:(y + 1) * w * 4])
                   for y in range(h))
    png = (b"\x89PNG\r\n\x1a\n"
           + chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 6, 0, 0, 0))
           + chunk(b"IDAT", zlib.compress(raw, 9))
           + chunk(b"IEND", b""))
    with open(path, "wb") as f:
        f.write(png)

# ------------------------------------------------------------- palette

# The game's MCGA palette: 16 colours held as three byte-planes at
# ds:00B8 (R), ds:00C8 (G), ds:00D8 (B), 6-bit VGA values — uploaded by
# pal_upload_mcga via int10 AX=1010h.  Index 13 is deliberately black
# (the attr map routes it to colour 0).  COL*.DTX has no RGB data —
# its +0x100/+0x200 tables are the byte->2px LUTs (identity for MCGA,
# so 4bpp nibbles are literal colour indices).
_vga6 = lambda v: (v << 2) | (v >> 4)
_MCGA6 = [(0,0,0),(0,0,42),(0,42,0),(0,42,42),(42,0,0),(42,0,42),
          (42,21,0),(42,42,42),(21,21,21),(21,21,63),(21,63,21),
          (21,63,63),(63,21,21),(0,0,0),(63,63,21),(63,63,63)]

def default_pal():
    return [(_vga6(r), _vga6(g), _vga6(b)) for r, g, b in _MCGA6]

def col_palette(raw):
    return default_pal()

def grey_pal():
    return [(i, i, i) for i in range(256)]

# ------------------------------------------------------------- tiles

TILE = 32          # 8x8 px, 4bpp packed
CHRHDR = 66        # u16 count + u32 pad + 60-byte pattern block

def tile_pixels(raw, toff):
    """Yield 64 pixel indices (0-15) from a 32-byte tile."""
    for y in range(8):
        row = raw[toff + y * 4:toff + y * 4 + 4]
        for b in row:
            yield b >> 4
            yield b & 15

def parse_chr(raw):
    """CHR bank: 66-byte header, count*32B tiles.  Returns tile
    pixel arrays, or None."""
    if len(raw) < CHRHDR:
        return None
    count = struct.unpack_from("<H", raw, 0)[0]
    if count == 0 or not (0 <= len(raw) - (CHRHDR + count * TILE) <= 64):
        return None
    return [bytes(tile_pixels(raw, CHRHDR + i * TILE)) for i in range(count)]

def draw_indexed(px, w, h, pal, transparent=True):
    rgba = bytearray(w * h * 4)
    for i, c in enumerate(px):
        if i >= w * h:
            break
        r, g, b = pal[c] if c < len(pal) else grey_pal()[c]
        a = 0 if (transparent and c == 0) else 255
        rgba[i * 4:i * 4 + 4] = bytes((r, g, b, a))
    return rgba

def compose_scr(raw, tiles, pal):
    """40x25 word map -> 320x200 image using companion CHR tiles."""
    if len(raw) < 2000 or not tiles:
        return None
    cells = struct.unpack_from("<1000H", raw, 0)
    img = bytearray(320 * 200)
    for i, t in enumerate(cells):
        if t >= len(tiles):
            continue
        tx, ty = (i % 40) * 8, (i // 40) * 8
        tp = tiles[t]
        for y in range(8):
            img[(ty + y) * 320 + tx: (ty + y) * 320 + tx + 8] = tp[y * 8:y * 8 + 8]
    return draw_indexed(img, 320, 200, pal, transparent=False)

def tile_sheet(tiles, pal, cols=16):
    rows = (len(tiles) + cols - 1) // cols
    w, h = cols * 8, rows * 8
    img = bytearray(w * h)
    for i, tp in enumerate(tiles):
        tx, ty = (i % cols) * 8, (i // cols) * 8
        for y in range(8):
            img[(ty + y) * w + tx:(ty + y) * w + tx + 8] = tp[y * 8:y * 8 + 8]
    return draw_indexed(img, w, h, pal, transparent=True), w, h

# ------------------------------------------------------------ sprites

def parse_spr(raw):
    """SPR bank: u16 offset table (first offset = table size), each
    record: x0,y0,w,h bytes then 4bpp rows (ceil(w/2) bytes/row)."""
    if len(raw) < 4:
        return []
    first = struct.unpack_from("<H", raw, 0)[0]
    if first < 4 or first % 2 or first >= len(raw):
        return []
    n = first // 2
    offs = struct.unpack_from("<%dH" % n, raw, 0)
    recs = []
    for i, o in enumerate(offs):
        if o < first or o + 4 > len(raw):
            recs.append(None)
            continue
        x0, y0, w, h = raw[o], raw[o + 1], raw[o + 2], raw[o + 3]
        need = 4 + ((w + 1) // 2) * h
        if not (0 < w <= 160 and 0 < h <= 200 and o + need <= len(raw)):
            recs.append(None)
            continue
        px = bytearray(w * h)
        p = o + 4
        for y in range(h):
            rb = raw[p:p + (w + 1) // 2]
            p += (w + 1) // 2
            for x in range(w):
                px[y * w + x] = (rb[x // 2] >> 4) if x % 2 == 0 else (rb[x // 2] & 15)
        recs.append((x0, y0, w, h, bytes(px)))
    return recs

# ------------------------------------------------------------- driver

def convert(path, outdir, dump_only=False):
    data = open(path, "rb").read()
    raw = decompress(data)
    base = os.path.splitext(os.path.basename(path))[0]
    os.makedirs(outdir, exist_ok=True)
    if dump_only:
        out = os.path.join(outdir, base + ".bin")
        open(out, "wb").write(raw)
        print("%s -> %s (%d -> %d bytes)" % (path, out, len(data), len(raw)))
        return

    pal = col_palette(raw) if base.startswith("COL") else default_pal()

    # palette-only resource
    if base.startswith("COL"):
        rgba = bytearray()
        for c in pal:
            rgba += bytes(c) + b"\xff"
        png_write(os.path.join(outdir, base + "_pal.png"), len(pal), 1, rgba)
        print("%s: palette (%d colors)" % (path, len(pal)))
        return

    # SCR tilemap -> compose with companion CHR in same dir
    if base.endswith("SCR") and len(raw) >= 2000:
        chr_name = os.path.join(os.path.dirname(path),
                                base[:-3] + "CHR.DTX")
        ctiles = None
        if os.path.exists(chr_name):
            ctiles = parse_chr(decompress(open(chr_name, "rb").read()))
        img = compose_scr(raw, ctiles, pal)
        if img:
            out = os.path.join(outdir, base + ".png")
            png_write(out, 320, 200, img)
            print("%s -> %s (tilemap, %s)" %
                  (path, out, "CHR found" if ctiles else "no CHR"))
            return

    # CHR tile bank -> tile sheet + per-tile PNGs
    tiles = parse_chr(raw)
    if tiles:
        rgba, w, h = tile_sheet(tiles, pal)
        out = os.path.join(outdir, base + "_sheet.png")
        png_write(out, w, h, rgba)
        tdir = os.path.join(outdir, base)
        os.makedirs(tdir, exist_ok=True)
        for i, tp in enumerate(tiles):
            png_write(os.path.join(tdir, "%03d.png" % i), 8, 8,
                      draw_indexed(tp, 8, 8, pal))
        print("%s: %d tiles -> sheet + %s/" % (path, len(tiles), tdir))
        return

    # SPR bank -> per-sprite PNGs (record format best-effort)
    if base.endswith("SPR"):
        recs = parse_spr(raw)
        ok = [r for r in recs if r]
        sdir = os.path.join(outdir, base)
        os.makedirs(sdir, exist_ok=True)
        for i, r in enumerate(recs):
            if not r:
                continue
            x0, y0, w, h, px = r
            png_write(os.path.join(sdir, "%03d.png" % i), w, h,
                      draw_indexed(px, w, h, pal))
        print("%s: %d/%d sprites decoded" % (path, len(ok), len(recs)))
        if ok:
            return

    out = os.path.join(outdir, base + ".bin")
    open(out, "wb").write(raw)
    print("%s -> %s (raw, %d bytes)" % (path, out, len(raw)))


def main(argv):
    outdir = "png"
    dump = False
    files = []
    it = iter(argv[1:])
    for a in it:
        if a == "-o":
            outdir = next(it)
        elif a == "--dump":
            dump = True
        else:
            files.append(a)
    if not files:
        print(__doc__)
        return 1
    for f in files:
        convert(f, outdir, dump)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
