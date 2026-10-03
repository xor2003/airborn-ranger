package org.airborn;

import android.content.res.AssetManager;
import android.os.Bundle;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import org.libsdl.app.SDLActivity;

/**
 * Bundles the DOS game data inside the APK (see assets srcDir in
 * app/build.gradle). SDL opens them with plain fopen() relative to the
 * working directory, so they must exist as real files: extracted once to the
 * app's private internal storage, which the native side chdir()s into.
 */
public final class MainActivity extends SDLActivity {

    @Override
    protected String[] getLibraries() {
        return new String[] { "SDL2", "main" };
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        installGameAssets();
        super.onCreate(savedInstanceState);
    }

    private void installGameAssets() {
        AssetManager am = getAssets();
        String[] names;
        try {
            names = am.list("");
        } catch (IOException e) {
            throw new IllegalStateException("asset list failed", e);
        }
        File dest = getFilesDir();
        for (String name : names) {
            if (!name.matches("[A-Za-z0-9_]+\\.(DTX|DAT|MIJ|EXE)")) continue;
            File out = new File(dest, name);
            try (InputStream in = am.open(name)) {
                // skip a byte-identical previous extraction
                if (out.isFile() && out.length() == in.available()) continue;
                try (OutputStream os = new FileOutputStream(out)) {
                    byte[] buf = new byte[16384];
                    int n;
                    while ((n = in.read(buf)) > 0) os.write(buf, 0, n);
                }
            } catch (IOException e) {
                throw new IllegalStateException("cannot install " + name, e);
            }
        }
    }
}
