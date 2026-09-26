#!/usr/bin/env python3
"""Normalize IDA numeric far pointers before translating Airborne Ranger.

These 11 DWORDs contain relocated segments at IDA's 1000h load base.
They are pointers/asset descriptors, not segment-independent constants.
The recovered DOS image and its relocation table confirm these sites.
"""
from pathlib import Path
import re
import sys

root = Path(__file__).resolve().parent.parent
out = Path(sys.argv[1]) if len(sys.argv) > 1 else root / "build_translate"
out.mkdir(parents=True, exist_ok=True)
text = (root / "AR.EXE.lst").read_text()
for literal, symbol, expected in [("38030001h", "seg004+1", 7),
                                  ("47230100h", "seg005+100h", 4)]:
    text, count = re.subn(r"(?i)(\bdd\s+)" + literal + r"\b",
                          lambda m: m[1] + symbol, text)
    if count != expected:
        raise SystemExit(f"Expected {expected} relocation sites for {literal}, found {count}")
# The game installs this one-instruction RET callback as a literal address.
# Name it so masm2c includes it in its indirect-call dispatch table.
text, count = re.subn(r"(?i)(mov\s+ax,\s*)4E9Ch\b",
                      lambda m: m[1] + "offset loc_14E9C", text)
assert count == 2, count
text, count = re.subn(r"(?m)^(seg000:4E9C)(\s+retn)$",
                      r"\1 loc_14E9C:\n\1\2", text)
assert count == 1, count
(out / "AR.EXE.lst").write_text(text)
(out / "AR.EXE.map").write_bytes((root / "AR.EXE.map").read_bytes())
print(out / "AR.EXE.lst")
