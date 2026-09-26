#!/usr/bin/env python3
"""Capture the translated runtime's actual SDL framebuffer via GDB."""
from pathlib import Path
import subprocess
import sys
from PIL import Image
root = Path(__file__).resolve().parent.parent
pid = sys.argv[1]
out = Path(sys.argv[2]) if len(sys.argv) > 2 else root / "build_sdl/mcga.png"
raw = out.with_suffix(".bgra")
subprocess.run(["gdb", "-q", "-batch", "-p", pid,
    "-ex", "set debuginfod enabled off",
    "-ex", f"dump binary memory {raw} vgaSdlFrame vgaSdlFrame+64000"],
    stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, check=True)
Image.frombytes("RGBA", (320, 200), raw.read_bytes(), "raw", "BGRA").convert("RGB").save(out)
raw.unlink()
print(out)
