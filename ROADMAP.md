# Airborne Ranger — Enhancement Ideas

I would **not "remaster" the actual game art or mechanics yet**. *Airborne Ranger* has a very specific 1980s MicroProse look, and replacing sprites, portraits, terrain, etc. could easily make it less charming.

The best direction is: **keep a pixel-exact Classic mode and build a modern shell around it.** Then optionally add an "Enhanced" mode later.

## What I would do next

| Priority | Feature | Value | Effort |
|---|---|---:|---:|
| 1 | Proper **4:3 aspect correction** | ★★★★★ | Low |
| 2 | Pause/settings overlay | ★★★★★ | Low |
| 3 | Master/music/SFX volume | ★★★★★ | Low |
| 4 | Better gamepad defaults + weapon next/previous | ★★★★★ | Low |
| 5 | Quick save / quick load | ★★★★★ | Medium |
| 6 | Mission briefing/objectives available from pause menu | ★★★★☆ | Low–Medium |
| 7 | CRT / sharp-pixel display options | ★★★★☆ | Medium |
| 8 | Better SN76489 emulation | ★★★★☆ | Medium |
| 9 | Android touch controls | ★★★★☆ | Medium |
| 10 | Smooth presentation/interpolated scrolling | ★★★★☆ | Medium–High |
| 11 | PNG → DTX mod pipeline | ★★★☆☆ | Medium |
| 12 | Rewind | ★★★☆☆ | Medium–High |

## 1. Fix the aspect ratio first

This will probably make the game look surprisingly better without changing **one pixel of artwork**.

320×200 was intended for a roughly 4:3 CRT display. Showing it as 640×400 makes everything too wide.

I would expose three display modes:

- **Original CRT 4:3** — recommended/default.
- **Square pixels 16:10** — exact 320×200 geometry for purists/development.
- **Stretch to screen** — optional, but not default.

And scaling options:

- Sharp / nearest
- Sharp + aspect correction
- CRT
- CRT + subtle curvature/glow

Do not use generic blurry bilinear filtering.

A nice modern fullscreen presentation could therefore be:

```text
                 ┌──────────────────────┐
                 │                      │
                 │    AIRBORNE RANGER   │
                 │       4:3 GAME       │
                 │                      │
                 └──────────────────────┘
            black/patterned side borders
```

On a 16:9 TV this would immediately look much more intentional.

## 2. Add a real pause/settings overlay

F8 is already effectively your frontend entry point. Expand it rather than modifying the original menus.

Something like:

```text
AIRBORNE RANGER

 Resume
 Save Game
 Load Game
 Mission Briefing
 Controls
 Audio
 Video
 Restart Mission
 Quit
```

This solves many usability problems while leaving `AR.EXE` behaviour intact.

You can even make the UI visually consistent with MicroProse: 320×200-style fonts, borders and palette, rather than an obviously modern SDL menu.

## 3. Quick-save is much more valuable than rewind

I would implement save states before rewind.

For an old game, this is transformative:

```text
F5        Quick Save
F7        Quick Load
Gamepad   hold Select → Save/Load menu
```

But distinguish them clearly from the game's own saves:

> Port Save State

instead of pretending it is an original feature.

Save:

- emulated DOS/game memory
- translated global state
- object tables
- RNG state
- audio sequencer state
- whatever timing counters affect deterministic execution

Since your port is already deterministic and has memory/frame dumping infrastructure, you're unusually well positioned to do this.

Then save-state support also gives you a foundation for **rewind later**.

## 4. Modernize controller use without changing game input

Your current abstraction is excellent because you can implement modern controls by generating the original keys.

The biggest thing missing is that **1–9 is bad on a gamepad**.

Add virtual actions:

```text
NEXT WEAPON
PREVIOUS WEAPON
QUICK MAP
QUICK SAVE
QUICK LOAD
PAUSE
```

They don't need corresponding DOS keys. For example:

```text
NEXT WEAPON
    current_weapon++
    inject corresponding '1'...'9'
```

Suggested controller default:

```text
Left stick / D-pad      Move
A                       Fire
B                       Back
X                       Center
Y                       Map

LB / RB                 Previous / Next weapon
LT / RT                 Walk / Run or useful secondary actions

Start                   Pause
Select                  Mission/map
```

This is much better than exposing historical keyboard implementation details to controller users.

And retain the existing ability to override everything.

## 5. Improve the binding screen

The architecture is already there, so polish it.

Instead of:

> Re-binding steals the old key

silently, display:

> A is currently FIRE.
> Reassign A to MAP?

And distinguish physical inputs:

```text
Keyboard: Space
Xbox: A
DualShock: Cross
Android TV: OK
```

Profiles would also be useful:

```text
Keyboard
Xbox-style controller
PlayStation-style controller
Android TV remote
Custom
```

The configuration file can still ultimately map them to the same actions.

## 6. Make the game understandable without reading the 1988 manual

This may be the biggest improvement to actual enjoyment.

Old games commonly assume that you've read the manual.

Add a **Help / Mission Briefing** screen accessible at any time containing the information the player has already been given:

```text
MISSION

Objective:
Destroy the communications center.

Extraction:
Northwest extraction zone.

Equipment:
M16
LAW × 3
Grenades × 5
Medkits × 2
```

Don't add information that the original game deliberately hides.

Similarly, an optional first-run overlay:

```text
D-PAD        MOVE
A            FIRE
RB/LB        CHANGE WEAPON
Y            MAP
START        PAUSE
```

Display it for 5–10 seconds and never again after the player dismisses it.

That makes the Android-TV build vastly more approachable.

## 7. Improve video presentation, not the artwork

I would avoid HD-redrawing the ranger/enemies/backgrounds.

Instead add a few tasteful display modes:

**Sharp**
- corrected 4:3
- no smoothing

**CRT**
- scanlines
- very slight phosphor glow
- optional shadow mask
- maybe tiny persistence

**Raw**
- exact 320×200 square-pixel framebuffer

One particularly useful technique would be a **sharp-bilinear/aspect-correct shader**: maintain crisp pixel boundaries while correcting the non-square pixels.

That should look much better on a 4K TV than simply scaling 640×400.

## 8. Smooth scrolling could make it feel dramatically newer

This is one visual enhancement I think could be worth experimenting with.

Don't change game physics.

Suppose internally the ranger goes:

```text
x=100
x=102
x=104
```

Instead of presenting those positions directly, render intermediate visual states:

```text
100.0
100.5
101.0
101.5
102.0
...
```

Same for the camera.

So:

**simulation remains completely original and deterministic**

but

**renderer interpolates between simulation frames.**

You could potentially present at 60/120 Hz while the original game runs at exactly its historic rate.

Make it:

```text
Motion smoothing:
 [Off - Original]
 [On]
```

Some old games benefit enormously from this.

## 9. Audio deserves more attention than new graphics

Your Tandy decompilation is one of the coolest parts of the project.

I'd make audio selectable:

```text
Audio emulation

○ Authentic SN76489
○ Softened PSG
○ Raw square-wave debug

Master     ━━━━━━━━
Music      ━━━━━━
Effects    ━━━━━━━
```

A high-quality/cycle-accurate SN76489 implementation would give you authentic tone/noise behaviour while avoiding artifacts from a simplistic synthesized implementation.

And separate volume controls are basic modern usability.

## 10. Add instant retry

This tiny feature could make the game much more enjoyable.

On death:

```text
MISSION FAILED

Retry mission
Load save
Return to menu
```

rather than forcing the player through unnecessary historical menu flows.

Again, the underlying game can remain unchanged: your frontend just restores a state captured at mission start.

That also gives you:

> Restart Mission

from Pause.

## 11. Rewind later

Once save states work, rewind becomes fairly natural:

```text
state every 500 ms
        ↓
ring buffer
        ↓
hold LT+LB / keyboard R
```

Maybe 30–60 seconds.

However, I would make it an **accessibility/convenience feature**, disabled by default.

Otherwise it changes the character of the game substantially.

## 12. A complete graphics mod pipeline would be excellent

You have:

```text
DTX → PNG
```

Finish:

```text
DTX ⇄ PNG
```

And ideally:

```text
ar_port/
  mods/
    hd-ish-art/
       ...
    yugoslavia/
       ...
```

Then other people can remake graphics without you distributing MicroProse assets.

That is much more interesting long-term than hardcoding replacement graphics into the engine.

You could even support an external override:

```text
mods/foo/SPR/...
```

If replacement exists → load it.
Otherwise → use original DTX.

This allows completely new campaigns/art packs later.

---

There are also several **small QoL features** I would add because they're cheap:

- remember fullscreen/window state
- remember display/filter/audio settings
- `Alt+Enter` fullscreen
- controller hot-plugging
- controller deadzone adjustment
- screenshots
- pause automatically when window loses focus on desktop
- optional confirmation before quitting
- show game version / port version separately
- display FPS only in developer mode
- disable screen saver while playing
- selectable integer window sizes: 1× / 2× / 3× / 4×
- controller button glyphs in the port menus

### What I would *not* do yet

I would avoid:

- AI-upscaled sprites
- completely redrawn portraits
- new particle effects
- dynamic lighting
- replacing terrain with high-resolution textures
- changing enemy AI
- adding health bars above enemies
- waypoint arrows
- auto-aim
- radically changing the HUD

Those start turning *Airborne Ranger* into another game.

There is a better separation:

```text
             AIRBORNE RANGER PORT

          Original simulation/game
                    │
          ┌─────────┴──────────┐
          │                    │
      Classic mode        Enhanced mode
          │                    │
     exact visuals         4:3 correction
     exact timing          smooth renderer
     exact controls        controller QoL
     original saves        save states
                          improved audio
```

The **original game should remain reproducible pixel-for-pixel**, which is especially valuable given how unusually rigorous your decompilation and tests already are. Then enhancements become an SDL/frontend layer rather than contaminating the recovered game logic.

If I were choosing the next **five commits**, I would do:

1. **4:3 aspect-correct fullscreen + Sharp/Raw modes**
2. **Pause overlay with Audio/Video/Controls**
3. **Master/Music/SFX volume**
4. **Next/Previous Weapon + improved gamepad defaults**
5. **Save-state + Restart Mission**

That would move it from "very impressive decompilation port" to something I'd actually expect people to comfortably play on a TV today.

---

## Feasibility notes (from the codebase)

- **Aspect correction** is cheap: `rt_present` already renders the 320×200
  framebuffer to a texture — a 4:3 destination rect or
  `SDL_RenderSetLogicalSize` is a small change in `port/video.c`.
- **Save state** is very feasible: the whole guest state is `mem[]` + CPU
  registers + a few port globals — snapshot/load is essentially a memcpy of
  memory plus the `tnd_*` sequencer fields. The existing `M2C_DUMP_*`
  memory/frame dump infrastructure is most of the plumbing already.
- **Next/prev weapon** needs no game-code changes: inject digit scancodes
  `1..9` from `port/input.c`, tracking the current weapon in port state.
- **Pause overlay** can reuse the same interception mechanism as the keymap
  screen — input is already captured before the guest sees it.
- **PNG→DTX mod pipeline**: the only missing piece is an LZW *encoder*
  matching the decompressor in `tools/dtx2png.py` (the decode side is proven
  pixel-exact); plus a resource loader override point in `port/dos.c`.
- **Classic/Enhanced split** maps cleanly onto the codebase: everything above
  lives in `port/*.c` (SDL shell); `port/gen/` stays the untouched,
  test-locked decompilation core.
