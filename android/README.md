# Airborne Ranger — Android (TV) build

Gradle + ndk-build project producing an APK. **The original game files are
not bundled and not in the repo** (copyright): on first launch,
`ImportActivity` asks for a folder containing your own DOS copy
(`*.DTX`/`*.DAT`/`*.MIJ`/`*.EXE`, picked via SAF or `adb push` into
`/sdcard/Android/data/org.airborn/files/`) and imports it into private
storage.

## Build

```sh
sh android/fetch-sdl.sh          # stage SDL2 (sources + org.libsdl.app java)
gradle -p android assembleDebug  # needs Android SDK + NDK 26.3
```

`fetch-sdl.sh` fetches `SDL2-2.32.10.tar.gz` from libsdl-org releases and
stages three things into `app/` (all gitignored): the source tree
(`jni/SDL`, built by `include $(LOCAL_PATH)/SDL/Android.mk`), the
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
