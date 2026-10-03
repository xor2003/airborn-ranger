LOCAL_PATH := $(call my-dir)

# SDL2 source tree staged by android/fetch-sdl.sh
include $(LOCAL_PATH)/SDL/Android.mk

LOCAL_PATH := $(call my-dir)
include $(LOCAL_PATH)/src/Android.mk
