package org.airborn;

import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Color;
import android.net.Uri;
import android.os.Bundle;
import android.provider.DocumentsContract;
import android.view.Gravity;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Locale;

/**
 * Asset-request menu shown while the original game files are missing.
 *
 * The original DOS files are copyrighted and are NOT shipped in the APK or
 * the repo — the user must supply their own copy. Three ways in:
 *   1. "Import folder" — SAF tree picker, recursive import of *.DTX/*.DAT/
 *      *.MIJ/*.EXE (Android TV supports this picker).
 *   2. "Import files" — SAF multi-select for devices where a whole tree
 *      can't be granted.
 *   3. adb push <gamedir>/. /sdcard/Android/data/org.airborn/files/
 *      which lands in the same filesDir.
 *
 * The screen is a plain focusable-button layout: fully D-pad navigable
 * on TV with no touch input and no androidx dependencies.
 */
public final class ImportActivity extends Activity {
    private static final int REQ_PICK_DIR = 42;
    private static final int REQ_PICK_FILES = 43;
    private static final String WANT = ".*\\.(DTX|DAT|MIJ|EXE)";
    // representative files — all present means a real import happened
    private static final String[] REQUIRED = {
        "TTLSCR.DTX", "TTLCHR.DTX", "MA_GSCR.DTX", "ROSTER.DAT"
    };

    private LinearLayout statusBox;

    private boolean assetsPresent() {
        for (String name : REQUIRED)
            if (!new File(getFilesDir(), name).isFile()
                    && !new File(getExternalFilesDir(null), name).isFile())
                return false;
        return true;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (assetsPresent()) { launchGame(); return; }

        float d = getResources().getDisplayMetrics().density;
        ScrollView scroll = new ScrollView(this);
        LinearLayout lay = new LinearLayout(this);
        lay.setOrientation(LinearLayout.VERTICAL);
        int pad = (int) (28 * d);
        lay.setPadding(pad, pad, pad, pad);
        lay.setBackgroundColor(Color.rgb(0x10, 0x10, 0x18));
        scroll.addView(lay);

        TextView title = new TextView(this);
        title.setText("AIRBORNE RANGER");
        title.setTextColor(0xFFFFD040);
        title.setTextSize(26);
        title.setGravity(Gravity.CENTER_HORIZONTAL);
        lay.addView(title);

        TextView msg = new TextView(this);
        msg.setText("\nThe original DOS game files are not included — " +
                "they are copyrighted.\nSupply your own copy of Airborne " +
                "Ranger for DOS (the files with .DTX, .DAT, .MIJ and " +
                ".EXE extensions).\n");
        msg.setTextColor(Color.WHITE);
        msg.setTextSize(16);
        lay.addView(msg);

        // live checklist of the required files
        statusBox = new LinearLayout(this);
        statusBox.setOrientation(LinearLayout.VERTICAL);
        lay.addView(statusBox);

        Button folder = addButton(lay, "Import game folder", v -> {
            Intent i = new Intent(Intent.ACTION_OPEN_DOCUMENT_TREE);
            i.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
            launch(i, REQ_PICK_DIR);
        });
        folder.requestFocus();

        addButton(lay, "Import files (multi-select)", v -> {
            Intent i = new Intent(Intent.ACTION_OPEN_DOCUMENT);
            i.addCategory(Intent.CATEGORY_OPENABLE);
            i.setType("*/*");
            i.putExtra(Intent.EXTRA_ALLOW_MULTIPLE, true);
            i.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
            launch(i, REQ_PICK_FILES);
        });

        TextView help = new TextView(this);
        help.setText("\nNo file picker? From a PC, copy the game folder to " +
                "the device and use the buttons above, or push directly:\n\n" +
                "  adb push <game-dir>/. " +
                "/sdcard/Android/data/org.airborn/files/\n\n" +
                "Then restart the app.");
        help.setTextColor(0xFF9AA0A6);
        help.setTextSize(14);
        lay.addView(help);

        setContentView(scroll);
        refreshStatus();
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (isFinishing()) return;
        // returning from the picker or after an adb push: auto-launch
        if (statusBox != null) refreshStatus();
        if (assetsPresent()) launchGame();
    }

    private Button addButton(LinearLayout lay, String text,
                             android.view.View.OnClickListener l) {
        Button b = new Button(this);
        b.setText(text);
        b.setTextSize(16);
        b.setFocusable(true);
        b.setOnClickListener(l);
        LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.MATCH_PARENT,
                LinearLayout.LayoutParams.WRAP_CONTENT);
        lp.topMargin = (int) (12 * getResources().getDisplayMetrics().density);
        lay.addView(b, lp);
        return b;
    }

    private void launch(Intent i, int req) {
        try {
            startActivityForResult(i, req);
        } catch (ActivityNotFoundException e) {
            Toast.makeText(this,
                    "No file manager on this device — use the adb method below",
                    Toast.LENGTH_LONG).show();
        }
    }

    private void refreshStatus() {
        statusBox.removeAllViews();
        int found = 0;
        for (String name : REQUIRED) {
            boolean ok = new File(getFilesDir(), name).isFile()
                      || new File(getExternalFilesDir(null), name).isFile();
            if (ok) found++;
            TextView row = new TextView(this);
            row.setText(String.format(Locale.US, "%s  %s",
                    ok ? "[OK]" : "[  ]", name));
            row.setTextColor(ok ? 0xFF7CFC90 : 0xFFFF7C7C);
            row.setTextSize(14);
            statusBox.addView(row);
        }
        TextView sum = new TextView(this);
        int total = countGameFiles();
        sum.setText(String.format(Locale.US,
                "\n%d game file%s in storage, %d/%d required present\n",
                total, total == 1 ? "" : "s", found, REQUIRED.length));
        sum.setTextColor(Color.WHITE);
        sum.setTextSize(15);
        statusBox.addView(sum);
    }

    private int countGameFiles() {
        int n = 0;
        File[] dirs = { getFilesDir(), getExternalFilesDir(null) };
        for (File dir : dirs) {
            File[] files = dir == null ? null : dir.listFiles();
            if (files == null) continue;
            for (File f : files)
                if (f.isFile() && f.getName().toUpperCase(Locale.US).matches(WANT)) n++;
        }
        return n;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (resultCode != RESULT_OK || data == null || data.getData() == null) {
            if (requestCode == REQ_PICK_DIR || requestCode == REQ_PICK_FILES)
                Toast.makeText(this, "Nothing selected", Toast.LENGTH_LONG).show();
            return;
        }
        try {
            if (requestCode == REQ_PICK_DIR) {
                Uri tree = data.getData();
                try {
                    getContentResolver().takePersistableUriPermission(
                            tree, Intent.FLAG_GRANT_READ_URI_PERMISSION);
                } catch (SecurityException ignored) { }
                int copied = copyDir(tree, DocumentsContract.getTreeDocumentId(tree));
                finishImport(copied);
            } else if (requestCode == REQ_PICK_FILES) {
                int copied = copyClipOrSingle(data);
                finishImport(copied);
            }
        } catch (IOException e) {
            Toast.makeText(this, "Import failed: " + e.getMessage(),
                    Toast.LENGTH_LONG).show();
        }
        refreshStatus();
    }

    private void finishImport(int copied) {
        if (assetsPresent()) {
            Toast.makeText(this, copied + " files imported", Toast.LENGTH_SHORT).show();
            launchGame();
        } else {
            Toast.makeText(this,
                    copied + " files imported — required files still missing " +
                            "(need the full DOS game folder)",
                    Toast.LENGTH_LONG).show();
        }
    }

    /* Copy every picked document (or the single one) into filesDir. */
    private int copyClipOrSingle(Intent data) throws IOException {
        int copied = 0;
        if (data.getClipData() != null) {
            for (int i = 0; i < data.getClipData().getItemCount(); i++) {
                Uri u = data.getClipData().getItemAt(i).getUri();
                String name = displayName(u);
                if (name != null && name.toUpperCase(Locale.US).matches(WANT)) {
                    copyOne(u, name.toUpperCase(Locale.US));
                    copied++;
                }
            }
        } else if (data.getData() != null) {
            Uri u = data.getData();
            String name = displayName(u);
            if (name != null && name.toUpperCase(Locale.US).matches(WANT)) {
                copyOne(u, name.toUpperCase(Locale.US));
                copied++;
            }
        }
        return copied;
    }

    private String displayName(Uri doc) {
        try (Cursor c = getContentResolver().query(doc,
                new String[]{DocumentsContract.Document.COLUMN_DISPLAY_NAME},
                null, null, null)) {
            if (c != null && c.moveToFirst()) return c.getString(0);
        } catch (Exception ignored) { }
        return null;
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
                } else if (name != null &&
                        name.toUpperCase(Locale.US).matches(WANT)) {
                    Uri doc = DocumentsContract.buildDocumentUriUsingTree(treeUri, id);
                    copyOne(doc, name.toUpperCase(Locale.US));
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
