"""dat2png - decode Atari ST / Amiga Airborne Ranger resources to PNG.

Formats handled (auto-detected):
  * Amiga files: 4-byte tag prefix + ST-compatible payload.
  * Compressed records: custom backward bitstream LZ (descriptor = last 12
    bytes [bitlong0][seed][outsize]); decompresses to another format below.
  * 0x601A PRG-wrapped resources: 28-byte header stripped, payload decoded.
  * Picture format (TITLEPIC, COMBAT, MENUP1-5): 128-byte header, 16-col
    palette (ST words stored $0BGR), then interleaved bitplanes.
  * Sprite banks: headerless records of [p0,p1,p2,mask] big-endian words,
    8-colour planes + visibility mask, row-interleaved.  Record dims are
    implicit per file (see SPRITE_BANKS).
  * Plain 4-plane bitmaps (.ST panels, CURSOR, FGRAPH): interleaved words,
    16 colours, no mask.
  * Text (TEXT, TEXTM, MEMBER): dumped to .txt.

Usage:
  dat2png.py [-o OUTDIR] FILE [...]
"""

import os
import struct
import sys
import zlib

_ST_DEFAULT_PAL = [0x777, 0x000, 0x700, 0x070, 0x007, 0x770, 0x077, 0x707,
                   0x555, 0x333, 0x733, 0x373, 0x337, 0x773, 0x757, 0x357]

# Sprite banks: name -> (w, h) of one record; count = filesize/(w*h/2).
# [p0,p1,p2,mask] 4-word groups, 8 colours + mask, row-interleaved.
SPRITE_BANKS = {
    "CHUTE":    (16, 16),   # 26 parachute frames
    "BODGRA2":  (32, 16),   # 8
    "BODGRA3":  (32, 16),   # 16
    "BODGRA4":  (32, 16),   # 16
    "BODGRA5":  (32, 32),   # 24
    "BODGRA6":  (32, 16),   # 16
    "TANKS":    (32, 16),   # 8 tanks
    "ARROW":    (16, 24),   # 4
    "SIGHT":    (16, 16),   # 1
    "PLANE":    (64, 64),   # 5 aircraft
    "LIGHT":    (16, 16),   # 5 lamps
}

# Plain 4-plane bitmaps (no mask): name -> width.
BITMAPS = {
    "AIRPANEL": 320, "AIRWINGS": 320,   # 320x54 title/panel art
    "CURSOR":   256,                     # weapon/item icon sheet
    "BODGRA":   256,                     # decompressed -> 256x224 body sheet
}

# Byte-interleaved 4-plane bitmaps (4 bytes per 8px group): name -> width.
BITMAPS_BI = {
    "AIRCHARS": 240,                     # font sheet, 240x16
}

TEXT_FILES = {"TEXT", "TEXTM", "MEMBER"}


def _rgb9(w):
    """Atari ST $0RGB (3 bits/channel) -> (r,g,b) 8-bit."""
    r, g, b = (w >> 8) & 7, (w >> 4) & 7, w & 7
    return (r * 255 // 7, g * 255 // 7, b * 255 // 7)


def _bgr9(w):
    """ST palette words as stored inside .DAT picture headers: $0BGR."""
    r, g, b = w & 7, (w >> 4) & 7, (w >> 8) & 7
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


# ---------------------------------------------------------------- decompress

def _dec_run(stream, bitlong0, outsize):
    """Core backward-bitstream decode.  Stream longs are read descending;
    each refill yields 31 bits.  Output is written backward from outsize.
    Returns (bytes, stream_pos) or (None, stream_pos)."""
    out = bytearray(outsize)
    op = outsize
    pos = len(stream)
    d0 = bitlong0

    def refill():
        nonlocal pos, d0
        pos -= 4
        if pos < 0:
            raise ValueError("eos")
        v = struct.unpack_from(">I", stream, pos)[0]
        d0 = (v >> 1) | 0x80000000
        return v & 1

    def nxt():
        nonlocal d0
        c = d0 & 1
        d0 >>= 1
        if d0 == 0:
            c = refill()
        return c

    def rb(n):
        v = 0
        for _ in range(n):
            v = (v << 1) | nxt()
        return v

    try:
        while op > 0:
            if nxt():
                sel = rb(2)
                if sel < 2:
                    cnt, dist = sel + 3, rb(9 + sel)
                    for _ in range(cnt):
                        op -= 1
                        if op < 0:
                            raise ValueError("underrun")
                        _copy(out, op + 1, dist, outsize)
                elif sel == 2:
                    cnt, dist = rb(8) + 1, rb(12)
                    for _ in range(cnt):
                        op -= 1
                        if op < 0:
                            raise ValueError("underrun")
                        _copy(out, op + 1, dist, outsize)
                else:
                    for _ in range(rb(8) + 9):
                        op -= 1
                        if op < 0:
                            raise ValueError("underrun")
                        out[op] = rb(8) & 0xFF
            else:
                if nxt():
                    dist = rb(8)
                    for _ in range(2):
                        op -= 1
                        if op < 0:
                            raise ValueError("underrun")
                        _copy(out, op + 1, dist, outsize)
                else:
                    for _ in range(rb(3) + 1):
                        op -= 1
                        if op < 0:
                            raise ValueError("underrun")
                        out[op] = rb(8) & 0xFF
    except (ValueError, IndexError):
        return None, pos
    return bytes(out), pos


def _copy(out, op, dist, outsize):
    if not (0 <= op - 1 < outsize and op - 1 + dist < outsize):
        raise ValueError("dist out of range")
    out[op - 1] = out[op - 1 + dist]


def decompress(d):
    """Compressed resource record (ST and Amiga share the format):
    [stream][bitlong0][seed][outsize] — 12-byte trailer, stream longs read
    backward.  Verified by checksum: seed == bitlong0 ^ XOR(stream longs).
    Returns decompressed bytes or None."""
    if len(d) < 24 or len(d) % 4:
        return None
    bitlong0, seed, outsize = struct.unpack_from(">3I", d, len(d) - 12)
    if not (0 < outsize <= 1 << 20):
        return None
    stream = d[:-12]
    x = bitlong0
    for i in range(0, len(stream) - 3, 4):
        x ^= struct.unpack_from(">I", stream, i)[0]
    if x != seed:
        return None
    raw, pos = _dec_run(stream, bitlong0, outsize)
    if raw is None or pos != 0:
        return None
    return raw


# ---------------------------------------------------------------- pixels

def planar_pixels(data, w, h, planes=4):
    """ST/Amiga word-interleaved bitplanes -> palette indices (row-major)."""
    groups = w // 16
    out = bytearray(w * h)
    pos = 0
    for y in range(h):
        for gx in range(groups):
            pw = []
            for p in range(planes):
                pw.append(struct.unpack_from(">H", data, pos)[0] if pos + 2 <= len(data) else 0)
                pos += 2
            for bx in range(16):
                idx = 0
                for p in range(planes):
                    idx |= ((pw[p] >> (15 - bx)) & 1) << p
                out[y * w + gx * 16 + bx] = idx
    return bytes(out)


def planar_pixels_byte(data, w, h, planes=4):
    """Byte-interleaved bitplanes: groups of `planes` bytes per 8px."""
    groups = w // 8
    out = bytearray(w * h)
    pos = 0
    for y in range(h):
        for gx in range(groups):
            for p in range(planes):
                v = data[pos] if pos < len(data) else 0
                pos += 1
                for bx in range(8):
                    if (v >> (7 - bx)) & 1:
                        out[y * w + gx * 8 + bx] |= 1 << p
    return bytes(out)


def planar_pixels_blocked(data, w, h, planes=5):
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


def sprite_pixels(data, w, h):
    """One sprite record: [p0,p1,p2,mask] words, 8 colours + transparent."""
    groups = w // 16
    out = bytearray(w * h)          # index+1, 0 = transparent
    pos = 0
    for y in range(h):
        for gx in range(groups):
            p0, p1, p2, m = struct.unpack_from(">4H", data, pos)
            pos += 8
            for bx in range(16):
                if (m >> (15 - bx)) & 1:
                    idx = ((p0 >> (15 - bx)) & 1) | (((p1 >> (15 - bx)) & 1) << 1) | (((p2 >> (15 - bx)) & 1) << 2)
                    out[y * w + gx * 16 + bx] = idx + 1
    return bytes(out)


def mask_score(data):
    """Share of [p0,p1,p2,mask] groups where mask is a superset; 1.0 = sprite bank."""
    w = [struct.unpack_from(">H", data, i)[0] for i in range(0, len(data) - 7, 2)]
    ok = tot = 0
    for i in range(0, len(w) - 3, 4):
        tot += 1
        if (w[i] | w[i + 1] | w[i + 2]) & ~w[i + 3] == 0:
            ok += 1
    return ok / tot if tot else 0


# ---------------------------------------------------------------- emitters

SPRITE_PAL = [(0, 0, 0), (90, 90, 220), (40, 170, 40), (200, 150, 40),
              (210, 60, 40), (170, 80, 200), (60, 190, 200), (230, 230, 230)]


def emit_sprites(outdir, base, data, sw, sh):
    """Tile a sprite bank into a sheet of sw x sh cells."""
    rec = sw * sh // 2
    n = len(data) // rec
    if not n or len(data) % rec:
        return []
    cols = max(1, min(n, 256 // sw))
    rows = (n + cols - 1) // cols
    sheet = bytearray(cols * sw * rows * sh * 4)
    for i in range(n):
        px = sprite_pixels(data[i * rec:(i + 1) * rec], sw, sh)
        ox, oy = (i % cols) * sw, (i // cols) * sh
        for y in range(sh):
            for x in range(sw):
                v = px[y * sw + x]
                if v:
                    off = ((oy + y) * cols * sw + ox + x) * 4
                    sheet[off:off + 4] = bytes(SPRITE_PAL[(v - 1) & 7] + (255,))
    out = os.path.join(outdir, base + ".png")
    png_write(out, cols * sw, rows * sh, sheet)
    print(f"{out}  {n} sprites {sw}x{sh}")
    return [out]


def emit_bitmap(outdir, base, data, w, pal, tag=""):
    h = len(data) * 8 // (w * 4)
    if len(data) * 8 % (w * 4) or h < 1:
        return []
    px = planar_pixels(data, w, h, 4)
    rgba = bytearray()
    for b in px:
        rgba += bytes(pal[b % len(pal)] + (255,))
    out = os.path.join(outdir, base + ".png")
    png_write(out, w, h, rgba)
    print(f"{out}  {w}x{h}{tag}")
    return [out]


def looks_like_palette(d, off, n=16):
    if off + 2 * n > len(d):
        return False
    words = [struct.unpack_from(">H", d, off + 2 * i)[0] for i in range(n)]
    return all(w <= 0x777 for w in words) and len(set(words)) > 3


def read_pic(d, off):
    """128-byte header picture: $0BGR palette + dims, then 4-plane data."""
    pal = None
    for po in range(0, 0x12, 2):
        words = [struct.unpack_from(">H", d, off + po + 2 * i)[0] for i in range(16)]
        if all(w <= 0x777 for w in words) and len(set(words)) > 4:
            pal = [_bgr9(w) for w in words]
            break
    if pal is None:
        return None
    data = d[off + 0x80:]
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
    """[4-byte prefix] + 5-plane blocked 320x200 + trailing $0RGB palette."""
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


# ---------------------------------------------------------------- driver

def convert(path, outdir, depth=0):
    d = open(path, "rb").read()
    base = os.path.splitext(os.path.basename(path))[0]
    return _convert_payload(d, base, path, outdir, depth)


def _convert_payload(d, base, path, outdir, depth):
    name = base.upper()
    if name.startswith("AIRBORNE_"):
        name = name[9:]
    for suf in ("_ST", "_AMIGA", "_DEC"):
        if name.endswith(suf):
            name = name[:-len(suf)]

    # Amiga files carry a 4-byte tag before the (ST-format) payload. Only
    # strip it when the resulting bytes actually decode as a known format.
    offs = [0]
    if len(d) % 8 == 4 and len(d) > 8:
        offs.append(4)
    for off in offs:
        dd = d[off:]
        if _known_format(dd, name, path):
            return _emit(dd, base, path, outdir, name, depth)
    # unknown: emit a sheet of the most plausible payload
    dd = d[4:] if len(offs) == 2 else d
    return _emit(dd, base, path, outdir, name, depth)


def _known_format(dd, name, path):
    if dd[:2] == b"\x60\x1a":          # PRG wrapper
        return True
    if name in TEXT_FILES:
        return True
    if name in SPRITE_BANKS:
        w, h = SPRITE_BANKS[name]
        if len(dd) % (w * h // 2) == 0:
            return True
    if name in BITMAPS and len(dd) % (BITMAPS[name] // 2) == 0:
        return True
    if name in BITMAPS_BI and len(dd) % (BITMAPS_BI[name] // 2) == 0:
        return True
    if len(dd) >= 0x80 and read_pic(dd, 0):
        return True
    if len(dd) >= 40032 and read_amiga_pic(dd):
        return True
    if mask_score(dd) >= 0.98:
        return True
    if decompress(dd) is not None:
        return True
    return False


def _emit(dd, base, path, outdir, name, depth):
    # PRG-wrapped resource -> decode inner payload
    if dd[:2] == b"\x60\x1a":
        tlen = struct.unpack_from(">I", dd, 2)[0]
        payload = dd[0x1C:0x1C + tlen]
        # *STRIP tables / PODWEPS data aren't graphics - emit raw sheet anyway
        return _convert_inner(payload, base, path, outdir, name, depth)

    return _convert_inner(dd, base, path, outdir, name, depth)


def _convert_inner(dd, base, path, outdir, name, depth):
    # text resources
    if name in TEXT_FILES or (dd and sum(32 <= b < 127 or b in (9, 10, 13) for b in dd) / len(dd) > 0.9):
        out = os.path.join(outdir, base + ".txt")
        open(out, "wb").write(dd)
        print(f"{out}  text")
        return [out]

    # compressed record?  decompress (checksum-gated), then recurse
    if depth < 2:
        raw = decompress(dd)
        if raw is not None and len(raw) > 128:
            return _convert_payload(raw, base + "_dec", path, outdir, depth + 1)

    # picture with embedded palette (ST 4-plane / Amiga 5-plane)
    if len(dd) >= 0x80:
        r = read_pic(dd, 0)
        if r is None:
            r = read_amiga_pic(dd)
        if r:
            w, h, px, pal = r
            rgba = bytearray()
            for b in px:
                rgba += bytes(pal[b % len(pal)] + (255,))
            out = os.path.join(outdir, base + ".png")
            png_write(out, w, h, rgba)
            print(f"{out}  {w}x{h} (pal)")
            return [out]

    # sprite bank?
    if name in SPRITE_BANKS:
        return emit_sprites(outdir, base, dd, *SPRITE_BANKS[name])

    # known plain bitmaps (size must tile at the known width)
    if name in BITMAPS and len(dd) % (BITMAPS[name] // 2) == 0:
        pal = [_rgb9(c) for c in _ST_DEFAULT_PAL]
        return emit_bitmap(outdir, base, dd, BITMAPS[name], pal)

    if name in BITMAPS_BI and len(dd) % (BITMAPS_BI[name] // 2) == 0:
        w = BITMAPS_BI[name]
        h = len(dd) * 8 // (w * 4)
        pal = [_rgb9(c) for c in _ST_DEFAULT_PAL]
        px = planar_pixels_byte(dd, w, h)
        rgba = bytearray()
        for b in px:
            rgba += bytes(pal[b % 16] + (255,))
        out = os.path.join(outdir, base + ".png")
        png_write(out, w, h, rgba)
        print(f"{out}  {w}x{h} (byte-int)")
        return [out]

    if mask_score(dd) >= 0.98 and len(dd) >= 64:
        # auto-fit: biggest sprite dims that tile the file
        for sw, sh in ((16, 16), (32, 16), (32, 32), (16, 24), (16, 32), (48, 32), (64, 32), (64, 64), (16, 64)):
            rec = sw * sh // 2
            if len(dd) % rec == 0:
                return emit_sprites(outdir, base, dd, sw, sh)

    # generic fallback: planar sheet
    return emit_sheet(outdir, base, dd)


def emit_sheet(outdir, base, data):
    planes = 4
    groups = len(data) // (planes * 2)
    if not groups:
        return []
    cols = 20 if groups >= 20 else groups
    rows = (groups + cols - 1) // cols
    w, h = cols * 16, rows
    px = planar_pixels(data[:groups * planes * 2], w, h, planes)
    pal = [_rgb9(c) for c in _ST_DEFAULT_PAL]
    rgba = bytearray()
    for b in px:
        rgba += bytes(pal[b % 16] + (255,))
    out = os.path.join(outdir, base + "_sheet.png")
    png_write(out, w, h, rgba)
    print(f"{out}  {w}x{h} sheet")
    return [out]


def main():
    outdir = "."
    files = []
    it = iter(sys.argv[1:])
    for a in it:
        if a == "-o":
            outdir = next(it)
        elif a.startswith("-"):
            print(__doc__); return 1
        else:
            files.append(a)
    if not files:
        print(__doc__); return 1
    os.makedirs(outdir, exist_ok=True)
    for f in files:
        convert(f, outdir)


if __name__ == "__main__":
    sys.exit(main())
