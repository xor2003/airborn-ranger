LOCAL_PATH := $(call my-dir)

PORT := $(LOCAL_PATH)/../../../../port

include $(CLEAR_VARS)

LOCAL_MODULE := main

# jni/include/SDL2 -> ../SDL/include symlink (fetch-sdl.sh) so <SDL2/SDL.h> resolves
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include $(PORT)

LOCAL_SRC_FILES := \
    $(PORT)/rt.c \
    $(PORT)/dos.c \
    $(PORT)/video.c \
    $(PORT)/input.c \
    $(PORT)/snd.c \
    $(PORT)/main.c \
    $(PORT)/memimg.c \
    $(PORT)/data_defs.c \
    $(PORT)/gen/ar.exe.c \
    $(PORT)/gen/ar.exe_seg000.c \
    $(PORT)/gen/ar.exe_seg001.c \
    $(PORT)/gen/ar.exe_seg002.c \
    $(PORT)/gen/ar.exe_seg003.c \
    $(PORT)/gen/ar.exe_default_seg.c \
    $(PORT)/gen/tandysnd_seg001.c

LOCAL_CFLAGS := -O2 -fcommon -fsigned-char -Wno-implicit-function-declaration -fno-strict-aliasing

LOCAL_SHARED_LIBRARIES := SDL2

include $(BUILD_SHARED_LIBRARY)
