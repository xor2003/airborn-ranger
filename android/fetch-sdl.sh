#!/bin/sh
# Stage SDL2 for the ndk-build: source tree into app/jni/SDL, the
# org.libsdl.app Java glue into app/src/main/java, and an include shim so the
# port's <SDL2/SDL.h> includes resolve. Idempotent; re-run to change SDL_VER.
set -eu

SDL_VER="${SDL_VER:-2.32.10}"
HERE=$(cd "$(dirname "$0")" && pwd)
APP="$HERE/app"
TARBALL="/tmp/SDL2-$SDL_VER.tar.gz"
URL="https://github.com/libsdl-org/SDL/releases/download/release-$SDL_VER/SDL2-$SDL_VER.tar.gz"

if [ ! -f "$TARBALL" ]; then
    curl -fL --retry 3 -o "$TARBALL" "$URL"
fi

# SDL2 source for the ndk-build (its Android.mk builds libSDL2.so)
rm -rf "$APP/jni/SDL"
mkdir -p "$APP/jni/SDL"
tar -xzf "$TARBALL" -C "$APP/jni/SDL" --strip-components=1

# SDLActivity + helpers (org.libsdl.app.*) from SDL's android-project template
rm -rf "$APP/src/main/java/org/libsdl"
mkdir -p "$APP/src/main/java"
cp -r "$APP/jni/SDL/android-project/app/src/main/java/org" "$APP/src/main/java/"

# <SDL2/SDL.h> shim: LOCAL_C_INCLUDES points at jni/include
mkdir -p "$APP/jni/include"
ln -sfn ../SDL/include "$APP/jni/include/SDL2"

echo "SDL2 $SDL_VER staged in android/app/jni"
