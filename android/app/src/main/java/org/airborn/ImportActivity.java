package org.airborn;

import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.provider.DocumentsContract;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * First-run gate: the original game files are copyrighted and are NOT shipped
 * in the APK or the repo. The user picks a folder containing their own copy
 * (*.DTX / *.DAT / *.MIJ / *.EXE); the files are copied into the app's
 * private storage and the game launches. Also works via
 *   adb push <gamedir>/. /sdcard/Android/data/org.airborn/files/
 * which lands in the same filesDir.
 */
public final class ImportActivity extends Activity {
    private static final int REQ_PICK_DIR = 42;
    private static final String WANT = ".*\\.(DTX|DAT|MIJ|EXE)";
    // representative files — present means a real import happened
    private static final String[] REQUIRED = {
        "TTLSCR.DTX", "TTLCHR.DTX", "MA_GSCR.DTX", "ROSTER.DAT"
    };

    private static boolean assetsPresent(Activity a) {
        for (String name : REQUIRED)
            if (!new File(a.getFilesDir(), name).isFile()) return false;
        return true;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (assetsPresent(this)) { launchGame(); return; }

        LinearLayout lay = new LinearLayout(this);
        lay.setOrientation(LinearLayout.VERTICAL);
        int pad = (int) (24 * getResources().getDisplayMetrics().density);
        lay.setPadding(pad, pad, pad, pad);

        TextView msg = new TextView(this);
        msg.setText("Airborne Ranger needs the original DOS game files.\n\n" +
                "Copy your game folder (*.DTX, *.DAT, *.MIJ, *.EXE) to this device, " +
                "then pick it below. Files are imported once into private storage.\n\n" +
                "Developers: adb push <dir>/. /sdcard/Android/data/org.airborn/files/");
        msg.setTextSize(16);
        lay.addView(msg);

        Button pick = new Button(this);
        pick.setText("Select game folder");
        pick.setOnClickListener(v -> {
            Intent i = new Intent(Intent.ACTION_OPEN_DOCUMENT_TREE);
            i.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
            startActivityForResult(i, REQ_PICK_DIR);
        });
        lay.addView(pick);
        setContentView(lay);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode != REQ_PICK_DIR) return;
        if (resultCode != RESULT_OK || data == null || data.getData() == null) {
            Toast.makeText(this, "No folder selected", Toast.LENGTH_LONG).show();
            return;
        }
        Uri tree = data.getData();
        try {
            getContentResolver().takePersistableUriPermission(
                    tree, Intent.FLAG_GRANT_READ_URI_PERMISSION);
        } catch (SecurityException ignored) { }
        try {
            int copied = copyDir(tree, DocumentsContract.getTreeDocumentId(tree));
            if (assetsPresent(this)) {
                Toast.makeText(this, copied + " files imported", Toast.LENGTH_SHORT).show();
                launchGame();
            } else {
                Toast.makeText(this,
                        "Folder doesn't contain the game files (need *.DTX/ROSTER.DAT)",
                        Toast.LENGTH_LONG).show();
            }
        } catch (IOException e) {
            Toast.makeText(this, "Import failed: " + e.getMessage(), Toast.LENGTH_LONG).show();
        }
    }

    /* Recursive walk over a SAF document tree using raw DocumentsContract —
     * avoids the androidx.documentfile dependency. */
    private int copyDir(Uri treeUri, String docId) throws IOException {
        Uri children = DocumentsContract.buildChildDocumentsUriUsingTree(treeUri, docId);
        int copied = 0;
        try (Cursor c = getContentResolver().query(children, new String[]{
                DocumentsContract.Document.COLUMN_DOCUMENT_ID,
                DocumentsContract.Document.COLUMN_DISPLAY_NAME,
                DocumentsContract.Document.COLUMN_MIME_TYPE}, null, null, null)) {
            while (c != null && c.moveToNext()) {
                String id = c.getString(0), name = c.getString(1), mime = c.getString(2);
                if (DocumentsContract.Document.MIME_TYPE_DIR.equals(mime)) {
                    copied += copyDir(treeUri, id);
                } else if (name != null && name.toUpperCase().matches(WANT)) {
                    Uri doc = DocumentsContract.buildDocumentUriUsingTree(treeUri, id);
                    copyOne(doc, name.toUpperCase());
                    copied++;
                }
            }
        }
        return copied;
    }

    private void copyOne(Uri doc, String name) throws IOException {
        File out = new File(getFilesDir(), name);
        try (InputStream in = getContentResolver().openInputStream(doc);
             OutputStream os = new FileOutputStream(out)) {
            if (in == null) return;
            byte[] buf = new byte[16384];
            int n;
            while ((n = in.read(buf)) > 0) os.write(buf, 0, n);
        }
    }

    private void launchGame() {
        startActivity(new Intent(this, MainActivity.class));
        finish();
    }
}
