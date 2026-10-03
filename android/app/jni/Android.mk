# SDL2 source tree staged by android/fetch-sdl.sh (jni/SDL) plus our
# module (jni/src). all-subdir-makefiles is the canonical SDL template
# entry; an explicit include breaks my-dir after SDL's import-module.
include $(call all-subdir-makefiles)
