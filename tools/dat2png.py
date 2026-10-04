"""dat2png - decode Atari ST / Amiga Airborne Ranger resources to PNG.

Formats handled (auto-detected):
  * Amiga files = 4-byte tag prefix + identical ST payload.
  * 0x601A PRG-wrapped resources (TEXT.DAT, *STRIP.DAT, ...) - the 28-byte
    header is stripped, payload decoded as planar data.
  * Picture format (TITLEPIC, COMBAT): 128-byte header, 16-col palette at +4
    (ST $0RGB words), width/height at +0x3C, then interleaved bitplanes.
  * Raw planar graphics (.ST, BODGRA*, small .DAT): word-interleaved bitplanes,
    tiled as 16px-wide columns into a sheet.

Usage:
  dat2png.py [-o OUTDIR] FILE [...]
  dat2png.py --planes 5 --pal-offset 4 FILE   # force layout
"""

import os
import struct
import sys
import zlib

_ST_DEFAULT_PAL = [0x777, 0x000, 0x700, 0x070, 0x007, 0x770, 0x077, 0x707,
                   0x555, 0x333, 0x733, 0x373, 0x337, 0x773, 0x757, 0x357]


def _rgb9(w):
    """Atari ST $0RGB (3 bits/channel) -> (r,g,b) 8-bit."""
    r, g, b = (w >> 8) & 7, (w >> 4) & 7, w & 7
    return (r * 255 // 7, g * 255 // 7, b * 255 // 7)


def _rgb12(w):
    """Amiga OCS $0RGB (4 bits/channel) -> (r,g,b) 8-bit."""
    r, g, b = (w >> 8) & 0xF, (w >> 4) & 0xF, w & 0xF
    return (r * 255 // 15, g * 255 // 15, b * 255 // 15)


def png_write(path, w, h, rgba):
    def chunk(tag, data):
        c = struct.pack(">I", len(data)) + tag + data
        return c + struct.pack(">I", zlib.crc32(tag + data) & 0xFFFFFFFF)
    png = (b"\x89PNG\r\n\x1a\n"
           + chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 6, 0, 0, 0))
           + chunk(b"IDAT", zlib.compress(b"".join(b"\x00" + bytes(rgba[y*w*4:(y+1)*w*4]) for y in range(h)), 9))
           + chunk(b"IEND", b""))
    with open(path, "wb") as f:
        f.write(png)


def planar_pixels(data, w, h, planes=4):
    """ST/Amiga interleaved bitplanes -> list of palette indices (row-major).

    Layout: each 16px group uses `planes` big-endian words, groups repeat
    across the row; rows are consecutive.
    """
    groups = w // 16
    out = bytearray(w * h)
    pos = 0
    for y in range(h):
        for gx in range(groups):
            pw = []
            for p in range(planes):
                if pos + 2 <= len(data):
                    pw.append(struct.unpack_from(">H", data, pos)[0])
                else:
                    pw.append(0)
                pos += 2
            for bx in range(16):
                idx = 0
                for p in range(planes):
                    idx |= ((pw[p] >> (15 - bx)) & 1) << p
                out[y * w + gx * 16 + bx] = idx
    return bytes(out)


def planar_pixels_blocked(data, w, h, planes=4):
    """Amiga contiguous bitplanes (one w*h/8 block per plane) -> indices."""
    psize = w * h // 8
    out = bytearray(w * h)
    for y in range(h):
        for x in range(w):
            b, bit = x // 8, 7 - (x & 7)
            idx = 0
            for p in range(planes):
                off = p * psize + y * (w // 8) + b
                if off < len(data):
                    idx |= ((data[off] >> bit) & 1) << p
            out[y * w + x] = idx
    return bytes(out)


def looks_like_palette(d, off, n=16):
    if off + 2 * n > len(d):
        return False
    words = [struct.unpack_from(">H", d, off + 2 * i)[0] for i in range(n)]
    return all(w <= 0x777 for w in words) and len(set(words)) > 3


def read_pic(d, off):
    """128-byte header picture: palette somewhere in +0..0x10, then planes.

    Returns (w, h, idx, rgb_palette) or None.
    """
    pal = None
    for po in range(0, 0x12, 2):
        words = [struct.unpack_from(">H", d, off + po + 2 * i)[0] for i in range(16)]
        if all(w <= 0x777 for w in words) and len(set(words)) > 4:
            pal = [_rgb9(w) for w in words]
            break
    if pal is None:
        return None
    data = d[off + 0x80:]
    # dims: header at +0x3A/+0x3C when present, else infer 320x200
    w = struct.unpack_from(">H", d, off + 0x3A)[0]
    h = struct.unpack_from(">H", d, off + 0x3C)[0]
    if not (16 <= w <= 640 and 16 <= h <= 256) or w % 16:
        w, h = 320, 200
    planes = len(data) * 8 // (w * h)
    if planes not in (4, 5) or len(data) < w * h * planes // 8:
        return None
    px = planar_pixels(data[:w * h * planes // 8], w, h, planes)
    return w, h, px, pal


def read_amiga_pic(d):
    """[4-byte prefix] + 5-plane 320x200 + trailing $0RGB-4bit palette (30 or 32 colors)."""
    for off in (4, 0):
        body = d[off:]
        extra = len(body) - 40000
        if extra <= 0 or extra > 0x80 or extra % 2:
            continue
        ncol = extra // 2
        palw = [struct.unpack_from(">H", body, 40000 + 2 * i)[0] & 0xFFF for i in range(ncol)]
        if len(set(palw)) < 4:
            continue
        pal = [_rgb12(w) for w in palw]
        pal += [pal[-1]] * (32 - ncol)
        px = planar_pixels_blocked(body[:40000], 320, 200, 5)
        return 320, 200, px, pal
    return None


def convert(path, outdir, planes_hint=None, pal_off=None):
    d = open(path, "rb").read()
    base = os.path.splitext(os.path.basename(path))[0]

    # strip 4-byte Amiga tag: real content starts at a format boundary
    off = 0
    if len(d) > 4 and d[4:6] == b"\x60\x1a":
        off = 4
    elif len(d) > 4 and d[:6] == b"\xda\x5e\x01\x60":
        off = 4
    elif len(d) > 4 and d[4] == 0 and d[5] == 0x40 and looks_like_palette(d, 8):
        off = 4
    elif len(d) > 4 and not looks_like_palette(d, 4) and looks_like_palette(d, 8):
        off = 4

    dd = d[off:]

    # PRG-wrapped resource
    if dd[:2] == b"\x60\x1a":
        tlen = struct.unpack_from(">I", dd, 2)[0]
        payload = dd[0x1C:0x1C + tlen]
        return emit_sheet(path, outdir, base, payload, planes_hint)

    # picture formats
    if len(dd) >= 0x80:
        r = read_amiga_pic(d) or read_pic(dd, 0)
        if r:
            w, h, px, pal = r
            rgba = bytearray()
            for b in px:
                rgba += bytes(pal[b % len(pal)] + (255,))
            out = os.path.join(outdir, base + ".png")
            png_write(out, w, h, rgba)
            print(f"{out}  {w}x{h} (pal)")
            return [out]

    # fallback: raw planar sheet
    return emit_sheet(path, outdir, base, dd, planes_hint)


def emit_sheet(path, outdir, base, data, planes_hint=None):
    """Lay out planar data as 16px-wide column groups in a grid sheet."""
    planes = planes_hint or 4
    groups = len(data) // (planes * 2)          # 16px groups total
    if not groups:
        return []
    cols = 20 if groups >= 20 else groups       # 320px wide rows by default
    rows = (groups + cols - 1) // cols
    w, h = cols * 16, rows
    px = planar_pixels(data[:groups * planes * 2], w, h, planes)
    pal = [_rgb9(c) for c in _ST_DEFAULT_PAL]
    rgba = bytearray()
    for b in px:
        rgba += bytes(pal[b % 16] + (255,))
    out = os.path.join(outdir, base + "_sheet.png")
    png_write(out, w, h, rgba)
    print(f"{out}  {w}x{h} sheet ({groups} 16px groups, {planes} planes)")
    return [out]


def main():
    outdir = "."
    planes = None
    files = []
    it = iter(sys.argv[1:])
    for a in it:
        if a == "-o":
            outdir = next(it)
        elif a == "--planes":
            planes = int(next(it))
        elif a.startswith("-"):
            print(__doc__); return 1
        else:
            files.append(a)
    if not files:
        print(__doc__); return 1
    os.makedirs(outdir, exist_ok=True)
    for f in files:
        convert(f, outdir, planes)


if __name__ == "__main__":
    sys.exit(main())
