#!/bin/sh
# Cross-compile ar_port.exe for Windows (MinGW) and stage a distributable dir.
# Usage: SDL2_MINGW=<path-to-SDL2-x.x.x/x86_64-w64-mingw32> sh tools/build_windows.sh
set -eu

ROOT=$(cd "$(dirname "$0")/.." && pwd)
cd "$ROOT"

CC="${MINGW_CC:-x86_64-w64-mingw32-gcc}"
SDL2_MINGW="${SDL2_MINGW:-/tmp/SDL2-2.32.10/x86_64-w64-mingw32}"
OUT="${OUT:-dist/airborn-win64}"

CFLAGS="-O2 -fcommon -Wno-implicit-function-declaration -fno-strict-aliasing"
CFLAGS="$CFLAGS -I$SDL2_MINGW/include -Iport"

SRCS="port/rt.c port/dos.c port/video.c port/input.c port/snd.c port/main.c \
port/memimg.c port/data_defs.c \
port/gen/ar.exe.c port/gen/ar.exe_seg000.c port/gen/ar.exe_seg001.c \
port/gen/ar.exe_seg002.c port/gen/ar.exe_seg003.c \
port/gen/ar.exe_default_seg.c port/gen/tandysnd_seg001.c"

mkdir -p "$OUT"
# shellcheck disable=SC2086
$CC $CFLAGS -o "$OUT/ar_port.exe" $SRCS \
    -L"$SDL2_MINGW/lib" -lSDL2 -mconsole \
    -static-libgcc -Wl,-Bstatic -lpthread -Wl,-Bdynamic -lm

cp "$SDL2_MINGW/bin/SDL2.dll" "$OUT/"
cp ./*.DTX ./*.DAT ./*.MIJ ./*.EXE "$OUT/"
cat > "$OUT/README.txt" <<'EOF'
Airborne Ranger — C port (Windows build)
=========================================
Run ar_port.exe from this directory (game data files must sit next to it).

Controls: arrows = move, Enter/KP5/Ins = fire & confirm, Esc = back/menu,
number keys = weapon select, +/- = throttle. Gamepads work too
(A=fire, B=back, dpad/stick=move, LB/RB = -/+).

M2C_GOD=1 enables god mode (infinite time past the evac window, pinned
wounds/ammo). M2C_GFXMODE=1..5 pre-selects the graphics adapter.
EOF
echo "staged $OUT"
