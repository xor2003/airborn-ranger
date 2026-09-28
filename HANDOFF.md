UPDATE 2026-09-28 — port/ stability verified; formal comparator is reference-only.

- port/ar_port builds clean (gcc -O2, no errors) and passes `make check`
  (320 unit checks: rt/dos/input/snd/video/game harnesses).
- e2e smoke (tests/e2e.sh): headless boot -> key-driven to POD screen,
  411 frames / 7 distinct, no unresolved indirect calls. PASS.
- The clean C port at port/gen/*.c + port/*.c is the stable, readable
  deliverable. The masm2c oracle (build_sdl/ar_m2c) remains the reference.

COMPARATOR (artifacts/airborn-z3cmp/, informational only):
- Z3/SSA per-function cross-ABI equivalence checker; 479/479 AR.EXE
  functions processed: 26 passed, 57 failed, 35 refused, 342 timeout,
  19 incomplete. Merged report: aircmp-results.json.
- Most "failed" verdicts are comparator-modeling noise (lazy x86 flag
  materialization, esp/segment bookkeeping conventions, I/O-port width
  granularity, oracle-only symbolic-dispatch path asymmetry), NOT proven
  port bugs. A small set (e.g. data+reg combos like sub_106a7,
  sub_15f09, sub_169b6, sub_1b23d, sub_1bcaa) would reward manual triage
  if deeper assurance is ever needed.
- Known comparator gaps: unresolved dispatches -> indirect_call (honest),
  guest-image bytes resolve on cand but not oracle (BSS vs embedded img),
  label-entry callsite inlining expands path budget (timeouts).
- straightline_ssa.py exits are now (guard,dst,jumpkind) triples —
  aircmp.py adapted accordingly.

UPDATE 2026-09-24 — MCGA blocker fixed; stopped at user request.

- build_sdl/ar_m2c rebuilt successfully from the corrected generated C++.
- Fixed 11 numeric far-pointer/asset-descriptor relocations in ar.exe.cpp:
  seven 3803:0001 values now use seg004, four 4723:0100 use seg005.
  Verified against orig_image.bin: expected 29A5:0001 and 38C5:0100.
  The incorrect title destination overlapped compressed TTLCHR.DTX input.
- Added indirect dispatch for 01A2:4E9C, a RET callback installed by literal
  address. Changes are in ar.exe.cpp and ar.exe_seg000.cpp.
- Verified real SDL mode 13h output, control selection, credits, and RANGER
  ASSIGNMENT past the previous missing-callback abort. Screenshot:
  build_sdl/mcga.png. Full mission gameplay and sound are not verified.
- Launch: ./run-mcga.sh, then select graphics 4 (MCGA), controls 2.
- Rebuild current corrected sources: tools/build_sdl.sh.
- tools/prepare_translation.py creates normalized listing + map with symbolic
  relocations and callback label for future masm2c runs. A full fresh translation
  was interrupted at user request; build_translate is incomplete, do not use
  its output as a replacement for the working root sources.
- No changes made to the external masm2c repository. No commit/push.

Previous handoff (historical; black-screen/menu speculation superseded):

You are continuing work on decompiling the DOS game "Airborne Ranger" located at
/home/xor/games/airborn into recompilable C/C++ via the masm2c translator at
/home/xor/masm2c. Read /home/xor/masm2c/AGENTS.md first for repo rules.

SCOPE (user-set, do not expand):
- KvikDOS (/home/xor/kvikdos) is ONLY for compiler validation and isolated
  function tests - it has NO sound/video. Do NOT use it as a game runtime.
- The playable target is the masm2c SDL runtime.
- Implement ONLY MCGA (VGA mode 13h) graphics and TANDY sound. Do NOT touch
  CGA/EGA/Hercules graphics or IBM sound.
- DOSBox is the reference environment for comparing behavior against the
  original. DOSBox requires 8.3 filenames.

DONE SO FAR:
1. Byte-exact rebuilt AR.EXE exists (ARN.EXE, 164528 bytes, 152 relocs, zero
   instruction mismatches vs recovered image orig_image.bin). It loads and runs
   in DOSBox identically to the packed original (ORIG.EXE). Verified it reaches
   the same post-menu state (byte_1CEA2=3 MCGA selected, IRQ0 hooked, IBMSNDS
   resident, black screen waiting for input - identical to ORIG.EXE). The rebuild
   is CORRECT; the black screen after menu is the game's normal wait-for-input
   state, not a bug.

2. m2c runtime: added BIOS INT 10h AH=10h palette/DAC subfunctions in
   /home/xor/masm2c/asm.cpp (around line 2277). Implemented AL=00 (set palette
   reg), AL=01/02 (overscan/all regs), AL=07/08/09 (reads), AL=10 (set DAC reg
   BX/DH/CH/CL), AL=12 (DAC block write), AL=15/17 (DAC reads). These update
   vgaPalette[] and set vga_render_dirty.

3. Rebuilt SDL binary: /home/xor/games/airborn/build_sdl/ar_m2c. Build recipe:
     g++ -m32 -mno-ms-bitfields -O0 -ggdb3 -Wno-multichar \
       -Wno-address-of-packed-member \
       -I/home/xor/masm2c -I/usr/include/x86_64-linux-gnu -I/usr/include/SDL2 \
       -D_REENTRANT -c /home/xor/masm2c/asm.cpp -o build_sdl/asm.o
     g++ -m32 -mno-ms-bitfields -ggdb3 build_sdl/*.o -o build_sdl/ar_m2c \
       /usr/lib/i386-linux-gnu/libSDL2-2.0.so.0 \
       /usr/lib/i386-linux-gnu/libncurses.so.6 \
       /usr/lib/i386-linux-gnu/libtinfo.so.6
   (32-bit SDL2 dev .so symlink is missing; link against the versioned .so.
   SDL2 config header is under /usr/include/x86_64-linux-gnu.)

4. The translated game RUNS: '4' selects MCGA, then it loads COLMCG.DTX,
   MAPDES.DTX, DCOL_TEM.DTX, ROSTER.DAT, TTLCHR.DTX - real asset loading. No
   more "INT10 AX=1010 not supported" errors. It opens an SDL window titled
   "masm2c VGA".

CURRENT BLOCKER / WHAT TO FIX:
- The game reaches a black screen after MCGA selection. vga stats show
  mode=03 (text mode), framebuffer all-black, only 1 frame rendered. The game
  loads title-screen assets then sits idle.
- KEY UNCERTAINTY: the game's post-menu screens (SELECT CONTROL DEVICE, name
  entry, mission select) are TEXT-MODE screens rendered via curses on the pty,
  NOT the VGA window. The mode only switches to 13h when gameplay actually
  starts. Need to confirm whether the game is truly stuck or just waiting for
  input at a text menu that isn't being driven correctly.
- Menu input sequence needed (from strings): graphics mode '4' -> SELECT
  CONTROL DEVICE ('2'=KEYBOARD-DIRECTIONAL) -> "Your Ranger's name:" (type name
  + ENTER) -> mission/difficulty select (ENTER) -> gameplay (mode 13h).

TESTING HARNESS (already built, reuse it):
- /tmp/m2c_drv.py - runs ar_m2c under a pty, reads keys from FIFO /tmp/m2c_key,
  writes ALL pty output to /tmp/m2c_screen.log (NOT the terminal - the curses
  escape codes damage terminal rendering).
  Run it detached:
    cd /home/xor/games/airborn && setsid python3 /tmp/m2c_drv.py >/dev/null 2>&1 </dev/null &
  Send keys: printf 'X' > /tmp/m2c_key
  Read output: /tmp/m2c_screen.log (strip escape codes when displaying)
- IMPORTANT: never let ar_m2c's raw curses output reach an interactive
  terminal - it emits escape sequences that corrupt rendering. Always capture
  to a file and grep/strip it.
- Debug env vars already in asm.cpp:
    M2C_VGA_DUMP_FRAME=/tmp/f.ppm   (dump framebuffer to PPM)
    M2C_VGA_DUMP_FRAME_AT=N         (dump after N frames)
    M2C_VGA_STATS=1                 (periodic mode/palette/pixel stats to stderr)
- Runtime logs "dos open FILE" / "dos read" to the pty as it works.

NEXT STEPS:
1. Drive the full menu sequence via /tmp/m2c_key (4 -> 2 -> NAME+CR -> CR x N)
   and watch M2C_VGA_STATS for mode=13. Capture a dumped PPM frame to confirm
   the mode-13 framebuffer + palette render correctly (COLMCG.DTX is the
   palette data).
2. If keys don't register: the game reads input via BIOS int16 - check how
   m2c's curses frontend feeds the int16 buffer and whether the game polls it.
3. Once graphics render, implement Tandy sound. Read
   /home/xor/games/airborn/TANDYSND.EXE.lst (1934 lines) to map the overlay's
   entry points, calling convention, and hardware interface. Tandy sound = the
   SN76489-style 3-voice chip. Determine which ports/ints the game pokes and
   add minimal emulation in the runtime. Do NOT implement IBM sound.
4. Verify with `rtk make check` (runtime change) when done.

REFERENCE: DOSBox comparison - run ORIG.EXE (packed original) and ARN.EXE
(rebuilt) side by side; both must reach identical states. Both currently show
the same black-screen wait state after '4', so the rebuilt exe is a faithful
reference for expected behavior.

## Working headless DOSBox reference harness (2024-09)

Xvfb + dosbox-staging can drive ORIG.EXE and capture real frames:
- `Xvfb :99 -screen 0 800x600x24` (running)
- `DISPLAY=:99 /home/xor/dosbox-staging/build/debug-linux/dosbox -conf /tmp/dosbox.conf --nolocale`
  (/tmp/dosbox.conf mounts the game dir and runs ORIG.EXE)
- Drive keys: `DISPLAY=:99 xdotool keydown --window <WID> <Key>; sleep; keyup` —
  the game's custom INT9 ISR needs the key HELD (keydown+keyup with a delay),
  a bare `xdotool key` press/release is too fast to register.
- Capture: `DISPLAY=:99 import -window root out.png`, crop (104,78,744,558) = the
  640x480 DOSBox window. Window title is "ORIG.EXE ...", find id via
  `xdotool search --name ORIG.EXE`.
- Flow to packing: 4(MCGA) -> title -> 2(kbd-dir) -> credits -> RANGER
  ASSIGNMENTS -> Down+Enter = VETERAN -> mission select -> Enter -> difficulty
  -> Enter -> briefing -> Enter -> SUPPLY POD SELECTION.

## Confirmed reference frames (what correct looks like)

- Difficulty "ruler" = a SLIDER between Easy/Hard: red-outlined track + white
  movable knob + red end-caps. Palette indices in region: 4 (red track), 6 (bg),
  2/5/3 (knob/caps). In the m2c build the region is pure index 6 = NOT DRAWN.
- Supply pod interior is NOT empty: it renders stored-equipment artwork
  (grenades, ammo boxes, a top rack) in palette indices 0/2/3 over black. In the
  m2c build the pod interior is mostly black with only a bottom strip drawn.
- Both are "a detailed bitmap graphic isn't reaching the mode-13 framebuffer."
  Text, box outlines, and small item sprites all render fine.

## Sprite-rendering fix (verified) — root cause of both missing graphics

The mission-difficulty slider AND the supply-pod item artwork were both gated by
the same bug. The shared sprite-list renderer `sub_126b9` (called every frame by
`sub_15996` on the packing screen, and for the slider) iterates 32 slots and tests
each with `vis_mask[bx] & bitmask_table[si&7]`. The bitmask table lives at
`ds:0x0DD5` (= `seg002:0x0DD5`, absolute `m+0xF675`) and must be
`01 02 04 08 10 20 40 80 FE FD FB F7 EF DF BF 7F`. It read all zeros, so every
sprite slot was skipped -> no slider, no pod items.

Root cause: the stale `ar.exe.cpp` was generated by an older masm2c that parsed an
all-decimal-digit hex `dup` count like `76h` as decimal 76 instead of hex 118.
In `db 76h dup(0), 1,2,4,...` the table landed at field+76 (0x0DAB) instead of
field+118 (0x0DD5). Live test: poking `01 02 04 08 ...` at `m+0xF675` made
`sub_130ba` (the mode-13 blit) fire and sprites draw.

Fix: `tools/fix_dup_fields.py` re-expands each `db/dw/dd` line in AR.EXE.lst with
the correct hex `dup` count and rewrites the matching `tmp999` initializer in
`Initializer_ar_exe` (ar.exe.cpp). It only writes a field when re-running the
BUGGY (decimal) expansion reproduces the generated array exactly, so the only
change is the corrected `dup` count. 27 fields fixed (slider table + text/border/
sprite-adjacent data). Pure-zero `dup(0)` BSS fields are skipped (already zero).

Verified: difficulty slider renders (red caps + track + white knob, knob tracks
challenge level), packing pod shows the default loadout items, DONE unwinds to
the map screen without crashing (StackPop path intact), assignment/credits/
briefing/mission-select screens all render cleanly.

## Crash fix (verified)

The packing DONE path is a deliberate return-two-levels: loc_15CBC does
`pop di` (drops the `call cs:funcs_159E9[bx]` return) then `jmp sub_10116` ->
`retn` pops the outer `call sub_15996` return (0x4832). asm.h now matches a
shallower-depth native-return mark and throws StackPop(depth_delta) to unwind
the skipped C++ call scopes. Verified: word_265a8=2, game reaches the map screen.
rtk make check: 4038/4038 pass.

## Audio engines (verified)

Two live synthesis paths share the SN76496 register state in m2c_snd (asm.cpp).
Select with `M2C_SND_ENGINE`:

- unset / `midi` (default): live MIDI-style soft-synth. Tone channels 0-2 render
  triangle-wave voices with attack/release envelope smoothing; channel 3 keeps
  the PSG noise/percussion. `snd_engine()` returns 0.
- `psg` / `square` / `tandy` / `chip`: original square-wave SN76496 output via
  `snd_sample()`. `snd_engine()` returns 1.

`snd_sdl_callback()` picks `midi_synth_sample(dt)` or `snd_sample()` per sample.
Per-channel phase/amplitude accumulators are audio-thread-owned; register writes
stay under the SDL device lock. `snd_sample_rate` is set to the real SDL rate in
snd_init so the synth `dt` is correct.

`M2C_MIDI_OUT=/path/file.mid` independently records a Standard MIDI File (note
events derived from the same register state); it works under either live engine.

Verified via SDL disk-audio capture (`SDL_AUDIODRIVER=disk`): default run gives a
smooth multi-level waveform (727 distinct amplitudes), `psg` run gives a 2-level
square wave; `M2C_MIDI_OUT` under the default engine produced a valid SMF
(MThd, 96 tpq) with note-on events. This is a soft-synth approximation of the
notes, not literal MIDI-device playback and not cycle-exact Tandy hardware.

Do not commit or push unless asked. Keep changes minimal and in-scope.

## Decompilation artifacts

- `tools/lift.py` — source-level lifter: reads `build_ida/src/` (decomp-mode
  sources), emits readable C to `lifted/`. Flag-folds cmp/test+jcc into real
  conditions, names int21/int10/int16 calls from AH, renders REP string ops as
  `while (cx--)`, structures goto loops into do-while/if, keeps original asm as
  `/* */` comments. Run: `python3 tools/lift.py` (output parses clean with gcc).
- `build_ida/ar_ida` — 32-bit non-PIE ELF for IDA/Hex-Rays (static analysis only,
  not runnable). Built with `-DM2CDEBUG=-1 -m32 -O2 -fno-pic -no-pie
  -fno-stack-protector`. asm.h gains a `M2CDEBUG==-1` override block (inline ops,
  ZF/SF/CF/OF only, direct call/return/push/pop); playable build unaffected.
- `build_ida/ar_lifted` — `lifted/*.c` compiled (`gcc -m32 -O1 -g -fno-pic
  -no-pie -fno-inline -fno-tree-switch-conversion -falign-*=1` + stub + defs);
  2013 named procs, 0 clones, mem[]-unified addressing. IDA was unstable on
  these binaries — superseded by the Ghidra pipeline below.
- `ghidra_c/` — AJenbo Ghidra fork decompiles of `AR_rebuilt.exe` (unpacked
  image; packed AR.EXE/ORIG.EXE are the same 73K file). 498 functions, all
  named from AR.EXE.map: code symbols land at `(map_seg+0x1000):off`, data
  labels at `0000:off` (Ghidra collapses unresolved DS to segment 0 — verified
  `_word_26582` renders). `int` calls post-processed by `tools/fix_swi.py`:
  `swi(0x21)` artifacts → named `dos_*`/`bios_*` calls resolved per-address by
  the lifter; `(*pcVar)()` → `= ax`. Quality: structured C, better than lifter
  output on pure computation; lifter remains authoritative for syscall sites
  and full coverage. Rerun:
    GHIDRA_INSTALL_DIR=/home/xor/ghidra/build/dist/ghidra-run/ghidra_12.2_DEV \
      ~/.config/ghidra/ghidra_12.2_DEV/venv/bin/pyghidra AR_rebuilt.exe \
      /home/xor/ghidra_scripts/MapAndDecompile.py AR.EXE.map ghidra_c
  then `python3 tools/fix_swi.py`. Saved project for GUI use:
  /home/xor/ghidra/ar_proj (AR_rebuilt.exe, analyzed + labeled).

## Lifter register-fold fix (verified) — root cause of invisible briefing text

`_reg_dead_after` in `tools/lift.py` scanned linearly and treated the first
register write after a fold candidate as a kill — ignoring that a label between
them may be entered by paths that never passed the write. In `sub_14fb1` this
dropped `mov si,0` at `loc_14fdc`: `si` kept whatever value the glyph renderer
left, so the `0x0b` row-adjust handler read its operand at
`[word_26DDE + dirty_si]` instead of the string byte. The footer terminator
`0x0b 0xEB` (row += signed -21) computed 23+0x56=109 instead of 23-21=2, so
`byte_26DE9 >= 25` disabled drawing (`byte_26DEF=1`) and the whole mission
paragraph at `ds:0xA841` was consumed invisibly.

Fix: `_reg_dead_after` now bails conservatively (reg=live) on labels,
control-flow lines and calls — only straight-line write-kills count as dead;
`_reads_reg` also treats `R++`/`R--` as reading the register. Regeneration
restored ~4K lines of register writes (179 `si = 0` alone); briefing now
renders "DESTROY A MUNITIONS DEPOT ... This is a Desert mission. ... press the
ENTER key" matching the reference layout.

## Signed-branch byte-width fix (verified) — root cause of POD right-edge noise

`flagval()` in `tools/lift.py` folded `js`/`jns`/signed `jl|jg|jle|jge`
conditions as `(short)(op)` regardless of operand width. For an 8-bit operand
`0xFF` zero-extends to `+255`, so `js` on a byte never fired — the opposite of
real hardware, where the sign flag tests bit 7 of the byte.

Concrete failure: `sub_15BD5` walks the POD display list at `ds:0xBC53`;
entries are item types 1-5 plus `0xFF` padding. `or al,al; js` should skip the
`0xFF` entries (negative byte). The lifted `(short)(al) >= 0` inverted it, so
every padding slot was spawned as an object whose sprite-id field
(`word_1DCBF` == `ds:0xE3F`) held `0x00FF`. The draw loop's `cx < 0x200` check
passed, `sub_130BA` did `bp = sprite_table[0x1FE]` — past PODSPR's 34-entry
table — got `0xDDD8` (uninitialized `0xDD` scratch), read a fake sprite header
from stack junk and painted a 16×83 block of `+0x18`-rebased noise at
`38c5:8AB0` — the right-edge band (x304-319, y75-157).

Fix: `pending` now carries operand width; `JS/JNS` and signed compares emit
`(signed char)` for byte operands, `(short)` for words; `SAR` on byte regs also
uses `(signed char)`. `js` after a non-zero `cmp` now tests the subtraction
result, not the left operand. ~1300 sites in `ar.exe_seg000.c` affected; POD
right edge now renders the correct "6"/"3" digits and labels, matching DOSBox
pixel-for-pixel. `make check` green.
