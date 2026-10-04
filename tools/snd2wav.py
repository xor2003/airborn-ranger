"""snd2wav - extract 8SVX samples from Amiga Airborne Ranger .snd containers.

The .snd files (game.snd / map.snd / armdat.snd) are player-driver blobs
with embedded IFF 8SVX samples (FORM....8SVX / VHDR / NAME / BODY).
Each sample is written as an 8-bit signed PCM WAV at its VHDR rate.

Usage:
  snd2wav.py [-o OUTDIR] FILE.snd [...]
"""

import os
import struct
import sys


def u32(d, o):
    return int.from_bytes(d[o:o + 4], "big")


def wav_write(path, pcm_s8, rate):
    data = bytes((b + 128) & 0xFF for b in pcm_s8)  # s8 -> u8 for WAV
    hdr = (b"RIFF" + struct.pack("<I", 36 + len(data)) + b"WAVE"
           + b"fmt " + struct.pack("<IHHIIHH", 16, 1, 1, rate, rate, 1, 8)
           + b"data" + struct.pack("<I", len(data)))
    with open(path, "wb") as f:
        f.write(hdr + data)


def parse_8svx(d, off):
    """Parse one FORM..8SVX at off -> (end_off, name, rate, pcm_s8)."""
    if d[off:off + 4] != b"FORM" or d[off + 8:off + 12] != b"8SVX":
        return None
    end = off + 8 + u32(d, off + 4)
    pos, name, rate, body = off + 12, None, 8363, b""
    while pos + 8 <= len(d):
        cid, clen = d[pos:pos + 4], u32(d, pos + 4)
        cdata = d[pos + 8:pos + 8 + clen]
        if cid == b"VHDR" and len(cdata) >= 20:
            rate = struct.unpack_from(">H", cdata, 12)[0] or 8363
        elif cid == b"NAME":
            name = cdata.split(b"\x00")[0].decode("latin1", "replace").strip()
        elif cid == b"BODY":
            body = cdata
        pos += 8 + clen + (clen & 1)
    pcm = [b - 256 if b > 127 else b for b in body]
    return end, name, rate, pcm


def convert(path, outdir):
    d = open(path, "rb").read()
    base = os.path.splitext(os.path.basename(path))[0]
    made = []
    idx = 0
    pos = 0
    while True:
        i = d.find(b"FORM", pos)
        if i < 0:
            break
        pos = i + 1
        if d[i + 8:i + 12] != b"8SVX":
            continue
        r = parse_8svx(d, i)
        if not r:
            continue
        end, name, rate, pcm = r
        if not pcm:
            continue
        fn = f"{base}_{idx:02d}_{name or 'sample'}.wav".replace("/", "_")
        wav_write(os.path.join(outdir, fn), pcm, rate)
        made.append(fn)
        print(f"{fn}  {len(pcm)}B @ {rate}Hz")
        idx += 1
        pos = end
    if not made:
        # no 8SVX: dump plausible PCM bank (armdat-style) as one wav
        print(f"{base}: no 8SVX forms found")
    return made


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
