#!/bin/sh
# e2e smoke: boot the port headless, drive through the menu flow to the POD
# screen, then cycle arrows+Enter until DONE launches the airdrop; verify the
# intro animates, POD is reached, and the parafoil descent renders.
# Run from the port dir; binary expects game data in the parent dir.
set -e
cd "$(dirname "$0")/.."
T=/tmp/ar_e2e
rm -f "$T".*.ppm "$T".*.txt "$T".log
mkdir -p /tmp

cd ..
# Binary is not part of `make check` — ensure it's built and current rather
# than fail on status 127 (missing) or silently test a stale build.
make -C port ar_port >/dev/null || { echo "FAIL: ar_port build failed"; exit 1; }
# Flow: 4=VGA/MCGA at the device menu, 2=keyboard-directional. Enters walk the
# title->credits->mission-select->difficulty->briefing chain. On the POD screen
# the divert mask only consumes arrows+Enter, so cycle focus over the item grid
# and the CLEAR/STANDARD/DONE row until Enter lands on DONE. Then idle so the
# airdrop (Osprey -> pod drop -> parafoil descent) runs on its own.
rc=0
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy \
M2C_DUMP="$T" M2C_DUMP_EVERY=120 \
M2C_KEYS="..2..\r\r\r\r\r\r\r\r\r\r\r\r\r\r..\R..\d..\r..\R..\d..\r..\l..\d..\r..\R..\d..\r..\u..\r..\R..\R..\d..\r..\R..\l..\u..\r..\R..\d..\r............................................................................................................................................................................................................................................................................................" \
M2C_KEYS_DELAY=10 \
timeout 150 ./port/ar_port 2>"$T".log || rc=$?
# 0 = guest exited on its own, 124 = killed by timeout, 128 = on_signal's
# _exit(128). Anything else (segv=139, abort=134, ...) is a real crash even
# if frames were already dumped.
case "$rc" in
0|124|128) ;;
*) echo "FAIL: ar_port terminated abnormally (status $rc)"; exit 1;;
esac

cd port
python3 - "$T" <<'EOF'
import glob, hashlib, struct, sys
tag = sys.argv[1]
frames = sorted(glob.glob(tag + ".*.ppm"))
hashes = {hashlib.md5(open(f,'rb').read()).hexdigest() for f in frames}
log = open(tag + ".log", errors='replace').read()

def rgb_count(path, rgb):
    # P6 ppm: approximate count of pixels matching rgb via substring scan.
    # Solid-color regions are long contiguous runs, so non-overlap counting is
    # exact for them; thresholds are set far from boundary cases anyway.
    d = open(path, 'rb').read()
    i = d.index(b'\n255\n') + 5
    return d[i:].count(bytes(rgb))

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
# Airdrop signature: the parafoil descent fills >55% of the frame with a single
# sky color — teal (0,168,168) on day missions, yellow (252,252,84) on dusk.
sky = 0
for f in reversed(frames):
    sky = max(rgb_count(f, (0, 168, 168)), rgb_count(f, (252, 252, 84)))
    if sky >= 150000: break
if sky < 150000:                print(f"FAIL: airdrop not reached (sky px={sky})"); ok = False
if "unresolved ind" in log:     print("FAIL: unresolved indirect calls"); ok = False
print(("e2e: PASS" if ok else "e2e: FAIL"),
      f"({len(frames)} frames, {len(hashes)} distinct)")
sys.exit(0 if ok else 1)
EOF
