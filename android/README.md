# Airborne Ranger — Android (TV) build

Gradle + ndk-build project producing an APK. **The original game files are
not bundled and not in the repo** (copyright): on first launch the
asset-request menu (`ImportActivity`) shows a checklist of the required
files and offers three ways to supply your own DOS copy
(`*.DTX`/`*.DAT`/`*.MIJ`/`*.EXE`):

1. **Import game folder** — SAF tree picker, recursive copy (works on
   Android TV; point it at a USB stick or shared folder).
2. **Import files** — SAF multi-select, for devices where a whole tree
   can't be granted.
3. **`adb push <gamedir>/. /sdcard/Android/data/org.airborn/files/`** —
   lands directly in the app's private storage.

Once all required files are present the menu launches the game
automatically. If a device has no system file picker, the menu detects
that and points to the adb method.

## Build

```sh
sh android/fetch-sdl.sh          # stage SDL2 (sources + org.libsdl.app java)
gradle -p android assembleDebug  # needs Android SDK + NDK 26.3
```

`fetch-sdl.sh` fetches `SDL2-2.32.10.tar.gz` from libsdl-org releases and
stages three things into `app/` (all gitignored): the source tree
(`jni/SDL`, picked up by `include $(call all-subdir-makefiles)` in
`jni/Android.mk`), the
`org.libsdl.app` Java glue (`src/main/java/org/libsdl`), and an include shim
(`jni/include/SDL2` → SDL headers) so the port's `<SDL2/SDL.h>` resolves.
Override the version with `SDL_VER=x.y.z`.

In CI this is done by `.github/workflows/android.yml`; `release.yml` attaches
the APK to a GitHub release on `v*` tags. Set `AR_ANDROID_KEYSTORE` +
`AR_ANDROID_KEYSTORE_PASSWORD` (env or secrets) for a persistent signing key —
without them the default debug key is used, which still sideloads fine.

## TV / gamepad key mapping

D-pad on the stock remote already arrives as arrow keys; OK/Center is Enter,
Back is Esc (`SDL_SCANCODE_AC_BACK` → XT `0x01`).

Gamepads (`SDL_CONTROLLER*` → `synth_key` in `port/input.c`):

| Pad | Key |
|---|---|
| D-pad / left stick | arrows |
| A / Start | Enter (select, fire) |
| B / Back | Esc (back, menu) |
| X | Space |
| Y | KP5 (alt fire) |
| LB / RB | `-` / `+` |

Weapon selection still needs number keys — pair a keyboard or use the
on-screen remote app for that screen.
