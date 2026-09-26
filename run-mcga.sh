#!/bin/sh
set -eu
cd "$(dirname "$0")"
exec ./build_sdl/ar_m2c "$@"
