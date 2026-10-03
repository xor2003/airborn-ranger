# Airborne Ranger — C port / decompilation

A C port of the DOS game *Airborne Ranger* (MicroProse, 1987), produced by
lifting `AR.EXE` + its `TANDYSND.EXE` overlay through a masm2c-style
translator and hand-verifying the result against the reference binary.
SDL2 frontend; the guest CPU registers/flags and 1MB flat memory model live
in `port/` alongside generated sources in `port/gen/` and the lifted tree in
`lifted/`.

## Bring your own copy

**No original game files are distributed** — not in this repo, not in release
artifacts. To run the port, supply your own DOS copy of the game:

- **Linux/SDL2**: `make -C port` then run `./port/ar_port` from the directory
  containing the game files (`*.DTX`, `*.DAT`, `*.MIJ`, `*.EXE`).
- **Windows x64**: `tools/build_windows.sh` (MinGW cross-build) — or grab the
  release zip and drop the game files next to `ar_port.exe`.
- **Android (TV)**: `sh android/fetch-sdl.sh && gradle -p android
  assembleDebug`; the APK imports your files on first launch (folder picker
  or `adb push`). Remote/gamepad key mapping is built in.

Regenerating the port pipeline (`port/memimg.c`, `port/gen/`,
`port/data_syms.h`) needs the original binaries locally —
`tools/gen_port.py` reads `AR_rebuilt.exe`, `AR.EXE.map`, `AR.EXE.lst`.

## Controls

arrows = move, Enter / KP5 / Ins = fire & confirm, Esc = back/menu,
digits = weapon select, `+`/`-` = throttle. Gamepads: A=fire, B=back,
dpad/stick=move, LB/RB=−/+.

Env: `M2C_GOD=1` god mode,
`M2C_KEYS`/`M2C_KEYS_DELAY` scripted input (see `port/tests/e2e.sh`).

## CI

`.github/workflows/`: `ci.yml` (build + tests + e2e), `android.yml`,
`windows.yml` (artifacts), `release.yml` (`v*` tag → GitHub release with
APK + Windows zip), `wasm.yml` (experimental skeleton).
