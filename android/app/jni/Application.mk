APP_ABI := arm64-v8a armeabi-v7a x86_64
APP_PLATFORM := android-21
# SDL's hidapi android sources use operator new/delete — needs a C++
# runtime. c++_static links LLVM libc++ into each .so, nothing to ship.
APP_STL := c++_static
APP_MODULES := SDL2 main
