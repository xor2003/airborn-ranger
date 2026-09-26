#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
M2C_ROOT=${M2C_ROOT:-/home/xor/masm2c}
mkdir -p build_sdl

# Recompile the runtime + tandy module + game code from source. The AR data
# objects (_data.o, _data_refs_*.o) are reused from build/ (the original
# consistent generation) because the root _data.cpp source was regenerated
# under a different segmentation and no longer matches ar.exe.cpp.
for src in ar.exe.cpp tnd_module.cpp "$M2C_ROOT/asm.cpp" "$M2C_ROOT/memmgr.cpp"; do
    obj="build_sdl/$(basename "$src" .cpp).o"
    g++ -m32 -mno-ms-bitfields -O0 -ggdb3 -Wno-multichar -Wno-address-of-packed-member \
        -I. -I"$M2C_ROOT" -I/usr/include/x86_64-linux-gnu -I/usr/include/SDL2 \
        -D_REENTRANT -c "$src" -o "$obj"
done

g++ -m32 -mno-ms-bitfields -ggdb3 \
    build_sdl/ar.exe.o build/_data.o build/_data_refs_000.o build/_data_refs_001.o \
    build_sdl/tnd_module.o build_sdl/asm.o build_sdl/memmgr.o \
    -o build_sdl/ar_m2c /usr/lib/i386-linux-gnu/libSDL2-2.0.so.0 \
    /usr/lib/i386-linux-gnu/libncurses.so.6 /usr/lib/i386-linux-gnu/libtinfo.so.6
