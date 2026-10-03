package org.airborn;

import org.libsdl.app.SDLActivity;

/**
 * Game activity — reached from ImportActivity once the user's own copy of the
 * original files is sitting in filesDir (the native side chdir()s into
 * SDL_AndroidGetInternalStoragePath(), so plain fopen() paths just work).
 */
public final class MainActivity extends SDLActivity {

    @Override
    protected String[] getLibraries() {
        return new String[] { "SDL2", "main" };
    }
}
