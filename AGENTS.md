# AGENTS.md

## Project purpose

Airborne Ranger DOS→C port: a stable, reproducible C decompilation of
`AR.EXE` — every function represented in generated C, recompiles cleanly,
gameplay/menus/input/graphics match the reference `build_sdl/ar_m2c`
behavior. Read `HANDOFF.md` first for current state and history;
`ROADMAP.md` for planned enhancements.

## Layout

- `lifted/` — generated C from the lifter (**do not hand-edit**)
- `port/gen/` — generated port sources (**do not hand-edit**)
- `port/` — SDL frontend + hand-written runtime (`input.c`, `video.c`
  MCGA-only, `rt.c`/`rt.h`, `dos.c`, `snd.c`, `memimg.c`)
- `port/tests/` — `make check` unit tests + `tests/e2e.sh` + probe tools
- `tools/` — pipeline: `lift.py`, `gen_port.py`, `rename.py`,
  `names.map`, `dat2png.py` (ST/Amiga resources), `dtx2png.py` (DOS .DTX),
  `snd2wav.py` (Amiga .snd), `build_windows.sh`
- `android/` — Gradle + ndk-build Android TV APK (`fetch-sdl.sh` stages SDL2)
- `.github/workflows/` — `ci.yml`, `android.yml`, `windows.yml`,
  `release.yml` (v* tag → release), `wasm.yml` (smoke only)
- `/home/xor/ghidra/ar_st/`, `/home/xor/ghidra/ar_amiga/` — ST and Amiga
  decompiles + extracted files + `NOTES.md` (format documentation)

## Build & test

```sh
# regen generated code (never hand-edit lifted/ or port/gen/)
python3 tools/lift.py && python3 tools/gen_port.py && python3 tools/rename.py

make -C port            # build ar_port
make -C port check      # unit checks (must print ALL TESTS PASSED)
bash port/tests/e2e.sh  # headless end-to-end run (needs game files in repo root)
```

Live headless run: `SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy
M2C_DUMP=/tmp/run M2C_KEYS=... ./port/ar_port` — see HANDOFF.md
"Live-test recipe" for the full env/script syntax.

Gotchas: a `make` racing regen compiles stale objects — clean rebuild if
phantom link errors appear; regen needs ~10 MB free in `/home`.

## Hard rules

- **Never commit original game assets** — `.DTX/.DAT/.MIJ/.EXE/.ADF/
  .STX/.SND/.STK` or decoded PNG/WAV of them. Releases are BYO-assets;
  the importer converts user-supplied files at runtime.
- Generated code is reproducible output: fix `tools/lift.py` /
  `gen_port.py` / `names.map`, then regen — never patch `lifted/` or
  `port/gen/`.
- Keep `names.map`, `gen_port.py`, and `memimg.c` fmap consistent on
  every regen.
- Commit style: concise message focused on "why", `Generated with
  [Devin](https://devin.ai)` + Co-Authored-By trailer.

## Verification before considering port changes done

1. `python3 tools/lift.py && python3 tools/gen_port.py && python3 tools/rename.py` — clean regen
2. `make -C port` — clean build
3. `make -C port check` — all checks pass
4. `bash port/tests/e2e.sh` — PASS

For tool-only changes (tools/dat2png.py etc.): `python3 -m py_compile`
plus a targeted decode run.

## Notes

- In `libdosbox`/`m2c` semantics, `m2c::m` is a live translated-program
  memory view — never replace it with an independent buffer.
- The `word_1d955` battle-pacing floor lives in `tools/lift.py` — a
  regen without it reintroduces the hyperspeed-battle regression.
- The ST/Amiga decompiles are the readability oracle for rewriting
  machine-shaped lifted code; the port itself is the correctness oracle.
