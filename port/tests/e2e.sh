#!/bin/sh
# e2e smoke: boot the port headless, drive to the POD screen, verify the
# intro animates (multiple distinct frames) and the game reaches POD.
# Run from the port dir; binary expects game data in the parent dir.
set -e
cd "$(dirname "$0")/.."
T=/tmp/ar_e2e
rm -f "$T".*.ppm "$T".*.txt "$T".log
mkdir -p /tmp

cd ..
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy \
M2C_DUMP="$T" M2C_DUMP_EVERY=120 \
M2C_KEYS="4..2..\r\r\r\r\r\r\r\r\r\r\r\r" M2C_KEYS_DELAY=8 \
timeout 90 ./port/ar_port 2>"$T".log || true

cd port
python3 - "$T" <<'EOF'
import glob, hashlib, struct, sys
tag = sys.argv[1]
frames = sorted(glob.glob(tag + ".*.ppm"))
hashes = {hashlib.md5(open(f,'rb').read()).hexdigest() for f in frames}
log = open(tag + ".log", errors='replace').read()

def rgb_count(path, rgb):
    # P6 ppm: count pixels matching rgb in a frame
    d = open(path, 'rb').read()
    i = d.index(b'\n255\n') + 5
    px = d[i:]
    return sum(1 for j in range(0, len(px) - 2, 3)
               if px[j:j+3] == bytes(rgb))

ok = True
if len(frames) < 10:            print("FAIL: too few frames:", len(frames)); ok = False
if len(hashes) < 4:             print("FAIL: screen never animated:", len(hashes)); ok = False
# POD screen signature: CLEAR/STANDARD/DONE buttons are solid (168,0,0) red,
# ~5.9k px when reached. Check any frame: queued Enters may advance past it.
red = 0
for f in reversed(frames):           # POD is reached late; stop at first hit
    red = rgb_count(f, (168, 0, 0))
    if red >= 2000: break
if red < 2000:                  print(f"FAIL: POD screen not reached (red px={red})"); ok = False
if "unresolved ind" in log:     print("FAIL: unresolved indirect calls"); ok = False
print(("e2e: PASS" if ok else "e2e: FAIL"),
      f"({len(frames)} frames, {len(hashes)} distinct)")
sys.exit(0 if ok else 1)
EOF
