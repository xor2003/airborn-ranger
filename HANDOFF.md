# Handoff prompt — Airborne Ranger C port

You are continuing work on the Airborne Ranger DOS→C port at
`/home/xor/games/airborn`. Goal remains: a stable, reproducible C
decompilation of `AR.EXE` — every function represented in generated C,
recompiles cleanly, gameplay/menus/input/graphics match the reference
`build_sdl/ar_m2c` behavior. Read `HANDOFF.md` for the full history.

## State at handoff

Everything below is **verified working** (live dummy-driver run into
real battle + test suite). The tree is dirty — modified files plus
untracked `tools/names.map`, `tools/rename.py`, `NAMING.md`, probes;
uncommitted. No commit was made.

### Session +1 (naming batch 2 + fast rename)

- **`tools/rename.py` rewritten single-pass**: one combined longest-first
  alternation per file instead of one regex scan per rename — ~9 min →
  ~18 s. A/B-verified byte-identical output vs the old per-name loop on
  `lifted.bak` seg000 + tandysnd + memimg + gen_port inputs.
- **32 new evidence-based names** in `tools/names.map` (177 main + 7 tnd
  total): battle-frame-loop cluster (`battle_key_check`, `obj_tick_all`,
  `obj_motion_tick`, `render_tick`, `spawn_tick`, `enemy_spawn`,
  `find_free_slot_b`, `obj_rec_clear`, `pick_spawn_xy`, `slot_weight_sum`,
  `ai_param_fetch`, `target_pri_decay`, `sprite_reset`, `grid_cell_mark`,
  `action_dispatch`, `unit_draw_walk`, `unit_draw_emit`,
  `hud_weapon_update`, `fx_overlay_fill`), POD-screen cluster
  (`pod_cursor_move`, `pod_hit_test`, `sprite_blit_flagged`),
  descent/ground (`descent_steer`, `ground_map_draw`, `tile_blit`,
  `load_palette_b`), and the 6 overlay `farjmp_ptr_102xx` slots.
- Key decodes: `post_mission_loop` = ground-combat frame loop;
  `battle_key_check` handles `+`/`-`/`0` → `word_26582` speed, SPC →
  `byte_28cc0`, `K` → `byte_26dd5`, ext/digits → `byte_29711` weapon;
  `spawn_tick`/`enemy_spawn` = difficulty-scaled enemy director
  (`word_297f3` timer, `byte_2974f` alive-cap 2); `hud_setup` is the
  POD-screen init (`word_1d920=5`) and `loc_159ee` its interact loop
  (`pod_cursor_move` → `pod_hit_test` → `funcs_159e9` 3-way dispatch).
- Rebuilt + re-verified after both batches: `make` clean,
  `make check` 342/342, `tests/e2e.sh` PASS.
- `word_1d955` pacing floor intact in lifted + gen (12 sites).

### Build & test status (this session)

- `python3 tools/lift.py && python3 tools/gen_port.py && python3
  tools/rename.py` — full regen ran clean
- `cd port && make` — clean build of `ar_port`
- `cd port && make check` — **342/342 checks pass**
- `bash port/tests/e2e.sh` — **PASS** (632 frames, POD + airdrop reached)
- Live run (`M2C_KEYS` script + `M2C_STATE`/`M2C_DUMP`): reached the
  ground battle, ranger **moved** (position regs tracked), **killed 2
  soldiers / 100 merit points**, mission ended at the assessment screen
  ("not accomplished — did not survive" is expected for blind scripted
  input). Battle pacing confirmed sane: `word_1d955` reloads to **1**,
  frame counter ~18 Hz, mission clock ticks ~1/s.

### Fixed this session

**Hyperspeed battle / "everyone runs very fast"** — regression: the
`word_1d955` pacing floor existed only as a manual `lifted/` edit and was
wiped by the session's full regen. Now made durable:

- `tools/lift.py`, `Lifter.op()` bare-statement passthrough rewrites
  `word_1d955 = ax` → `word_1d955 = ax ? ax : 1` (12 emitted sites).
  `word_1d949` speed table is 0 at the default index → countdown 0 →
  `while(var!=0)` exits instantly → free-run. Floor of 1 int1c tick ≈
  18 Hz = the game's fastest nonzero pace.
- `HANDOFF.md` updated to point at the `lift.py` location.

**"Cannot move / invisible walls / background doesn't move"** — largely
collateral of the same bug: at ~1500 iter/s, key-hold windows compress to
microseconds (input effectively dropped) and the spinning guest thread
starves the render loop (screen appears frozen). Verified movement works
live post-fix. Remaining perceived walls are likely *legit*: trenches/
berms are collidable, and the game has **no edge-scroll** — the ranger
walks inside a fixed strip, Enter advances to the next map section
(`draw_pos_advance` / `byte_2a9ca`, budget `byte_2a9e2`).

### Carried forward from prior sessions (all verified)

- **POD DONE fix** — guest-`sp` delta early-return at indirect dispatch
  sites in `lift.py` (~line 309 idiom):

```c
{ vfn f_ = func_at(target); dw sp_ = sp;
  if (f_) f_(); else fprintf(stderr, "unresolved ind call %x\n", ...);
  if ((short)(sp - sp_) > 0) { sp = sp_; return; } }
```

  Propagated to 34 sites. Models callees that consume the guest return
  frame (`pop di` + tail `retn`).
- `fold_temps`/`_reg_dead_after` register-family liveness fix.
- Absolute mouse on POD (`pod_abs_cursor`, `word_1D920==5` gate).
- `M2C_HOLD` split into `M2C_HOLD`/`M2C_HOLD_AT` (video.c) and
  `M2C_HOLDWALK` (input.c walk phase).
- Intro pacing gates live in `build_ida/src/*.cpp` (lifter *input* —
  committed, so they survive regen; 14 `sub_14723`/`delay_ticks` +
  19 `word_1d94d` gate sites).

## Key mechanics / symbols

- Two frame loops, separate pacing vars: mission/airdrop `sub_1aaaf`
  gates on `word_1d94d` (reload `word_2a962`=1..2, never 0); ground
  combat `sub_14a10` gates on `word_1d955` (reload `word_1d949` table).
  AI cadence div: `word_265ae` ← `word_1d94b` table.
- In-game speed control: `-`/`_` dec, `+`/`0` inc speed index
  `word_26582` (default 4).
- `word_26de4` is **active-low** (`= held ^ 0xFFFF`); `0xFF` = idle.
  Divert table (seg002:0950): Up→1, Down→2, Left→4, Right→8,
  Enter/KP5/KP0→bit4 action. `byte_26de6` = bit4 edge latch.
- Walk/battle positions: `word_1DCFF`+`1DD1F` (X), `word_1DD3F` (Y);
  `byte_2a9ca` landed flag, `byte_2a9d3` phase, `byte_2a9e1/e2/e3`
  movement cadence/direction/budget, `word_2a9ce` descent counter.
- Input chain: SDL event → `xt_scan` → `int9_update` → `word_1D959` →
  `sub_1231C` → `word_1DCA2` → `word_26DE4`.
- Movement: `sub_189e2` = ranger object tick in battle;
  `move_dir_tick` (`sub_1adce`) in the walk/section-advance phase.
  Both verified by probes (`port/tests/mv_probe`, `walk_probe`) and
  live state dumps.
- Emulated regs are C globals shared with the TANDYSND ISR —
  `M2C_IRQSAVE=full` snapshots GPRs around `guest_irq0`.

## Regen pipeline (use it — never hand-edit `port/gen/` or `lifted/`)

```sh
cd /home/xor/games/airborn
python3 tools/lift.py          # lifted/*.c   (LIFT_OUT env override exists)
python3 tools/gen_port.py      # port/gen/*.c + port/memimg.c + procs.h
python3 tools/rename.py        # applies tools/names.map in place
cd port && make && make check && bash tests/e2e.sh
```

Gotchas seen:
- A `make` racing the regen compiles stale objects — clean rebuild
  (`rm -f gen/*.o *.o`) resolves phantom "undefined reference" noise.
- Regen needs ~10 MB free in `/home`; check `df -h /home` first.

## Live-test recipe

Headless (works, used this session):

```sh
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy \
M2C_DUMP=/tmp/run M2C_DUMP_EVERY=60 M2C_STATE=1 M2C_TICKSTAT=1 \
M2C_KEYS_DELAY=10 M2C_KEYS="..2..\r\r\r...(menus)...dirs+enters..." \
timeout 240 ./port/ar_port 2>/tmp/run.log
# /tmp/run.NNNN.ppm = frames, ST lines in the log carry game state
```

Xvfb interactive:

```sh
Xvfb :99 -screen 0 1280x800x24 &
DISPLAY=:99 M2C_MOUSEDBG=1 timeout 300 ./port/ar_port > /tmp/run.log &
W=$(DISPLAY=:99 xdotool search --name "Airborne Ranger" | tail -1)
DISPLAY=:99 xdotool windowfocus $W
DISPLAY=:99 xdotool key 2 / Return / Up / ...
```

`M2C_KEYS` escapes: `\r` Enter, `\e` Esc, `\u \d \l \R` arrows,
`\H` = hold next key (no release), `.` = pause one delay window.
`M2C_STATE` ST-line fields decoded in `port/input.c` dump routine.

## Watch list / open risks

- The `sp`-delta early-return is broad (34 indirect-call sites).
  Tests + e2e + live battle pass, but other screens aren't individually
  exercised. If a regression appears, suspect this first — a callee
  could legitimately net-`push`/`pop` unevenly.
- `M2C_POD` diagnostic instrumentation was wiped by regen (by design);
  re-add via `tools/gen_port.py` injection if it should be permanent.
- TANDYSND proc names map via `names.map` `tnd` tag; `memimg.c` fmap
  uses friendly names — keep `names.map`/`gen_port.py`/`memimg.c`
  consistent on future regens.
- `/home` hit 100% full mid-session: `~/.local/share/devin/cli/
  sessions.db` had grown to 11.5 GB (it freed itself later — flag to
  the user if it recurs). The regen needs only a few MB but make sure
  there is some.
- Nothing is committed. If asked to commit: `git status`/`git diff`,
  match prior style, sign-off footer per repo convention.

## Files that matter

`tools/lift.py` (lifter — source of truth for generated code, incl. the
pacing floor), `tools/gen_port.py`, `tools/rename.py`,
`tools/names.map`, `port/input.c` (mouse+key), `port/video.c`,
`port/rt.c`/`rt.h`, `port/dos.c`, `port/memimg.c` (fmap),
`port/procs.h`, `port/tnd_procs.h`, `port/tests/*.c` (incl. probes:
`mv_probe`, `walk_probe`, `col_probe`, `tbl_probe`, `replay_probe`,
`dump_dir`), `port/gen/*.c` (generated — do not hand-edit),
`lifted/*.c` (generated).

## Naming pass (evidence-driven, applied)

`tools/names.map` now carries ~500 AR.EXE renames + ~90 `var`-tagged
variable renames; `tools/rename.py` gained a `var` path (whole-word,
case-sensitive — asm comments keep `word_XXXX` uppercase by design).

Coverage: **0 unnamed `sub_*` remain** in seg000. Evidence sources:
jump-table contents decoded from `port/memimg.c` `img[]` +
`port/data_syms.h` aliases (`jpt_*`/`funcs_*` resolve to owner locs).

Major decoded families:
- adapter order everywhere: 0=CGA 1=Tandy 2=EGA 3=MCGA 4=Hercules
- `funcs_11d52` (15) = sprrow_*_[abc] x adapters; `funcs_11cb8` =
  tile_row_* x adapters; `jpt_1171c` same set inlined
- `funcs_13bf8..141ce` = 8 scroll directions x 5 adapters = the
  `scroll_[a-h]_<adapter>` family; `jpt_16ab9` (action_dispatch) cases
  1,2,4,5,6,8,9,A -> `scroll_go_[a-h]` locs -> `scroll_edge_*` fixups
- `jpt_126d5` = clip_go_<adapter> -> clip_blit/clip_rest pairs
- `funcs_1892f` = ~36 objtype_NN_tick handlers indexed by obj type
- `funcs_1b3e2`/`funcs_1b41f` = mission6/7/8_setup + _populate
- `jpt_126d5`-adjacent `sub_12250/123a6/123ec/124fe` = joystick port
  0x201 readers; `sub_1148c/114b7/1152f/12543-12628` = IVT install/
  restore (int8/int9/int0/int1c)
- `cam_pan_detect`/`cam_pan_apply` = the stale-cmp camera chain;
  `player_input_tick` = combat input (word_26de4 -> move_intent);
  `route_map_screen` = the planning map; `mapgen_*` = mission populate

Weak-evidence symbols intentionally left as word_/byte_/loc_ names.
Pipeline verified: rename.py -> make clean build, 342/342, e2e PASS
(292 frames, 203 distinct).

## Build targets (portability)

`port/` is now multi-platform (Linux was already working):

- **Android TV APK** — `android/` Gradle + ndk-build project. SDL2 is staged
  by `android/fetch-sdl.sh` (sources into `app/jni/SDL`, `org.libsdl.app` java
  into `app/src/main/java`); all game data files are bundled in the APK via
  an assets `srcDir` pointing at the repo root and extracted to filesDir by
  `MainActivity` (`main.c` chdir()s there). TV remote works out of the box
  (dpad=arrows, OK=Enter, BACK→`SDL_SCANCODE_AC_BACK`→Esc); gamepads map in
  `input.c` (A=Enter/fire, B=Esc, stick+dpad=arrows, LB/RB=-/+).
- **Windows x64** — `tools/build_windows.sh` cross-compiles with mingw-w64
  (`gcc-mingw-w64-x86-64-posix` + SDL2 mingw devel package). POSIX signal IRQ
  park is `#ifdef`'d to `SuspendThread`/`ResumeThread` on `_WIN32`;
  `SDL_MAIN_HANDLED` keeps a plain `main()` so no SDL2main is needed.
- **CI** — `.github/workflows/`: `ci.yml` (build + make check + e2e),
  `android.yml`, `windows.yml` (artifacts), `release.yml` (v* tag → GitHub
  release with APK + zip), `wasm.yml` (borrowed skeleton — compile-smoke
  only; the IRQ0 park model has no wasm equivalent yet).

Local Linux build/test unchanged: `make -C port`, `make -C port check`,
`sh port/tests/e2e.sh`.
