---
name: clean-code
description: Readability and clean-code checklist for writing or reviewing code. Triggers on: rewrite functions, replace lifted/machine-shaped code with readable C, write clean code, readability review, code style review, refactor for clarity, make this readable.
---

# Readable Code — working checklist

Source: "The Art of Readable Code" (Boswell & Foucher, O'Reilly 2012),
summary archived at https://github.com/soodoku/tldr/blob/master/summaries/23_art_of_readable_code.md

## The one rule

**Minimize the time it takes someone else to understand the code.**
"Understand" means: able to modify it, spot bugs, and reason about how it
interacts with the rest of the system. This beats compactness, cleverness,
and even small performance wins when they conflict.

## Naming

- Be specific: `n_nodes` not `size`; `mem_bytes` not `len`. A name is a tiny
  comment.
- No generic placeholders (`tmp`, `retval`, `foo`, `df1`) — except loop
  indices in short loops, and prefer `user_i` over `i` when nesting.
- Attach units/attributes: `delay_ms`, `plaintext_password`, `src_off`.
- Booleans name the predicate: `has_space`, `is_done`, `can_write`,
  `should_retry` — never bare `read_password`/`flag` for a bool.
- `min`/`max` = inclusive; `first`/`last` for inclusive ranges;
  `begin`/`end` for exclusive ranges. Ambiguity here causes off-by-ones.
- Drop unneeded words (`ToString` not `ConvertToString`); don't strip so
  hard the name loses meaning.
- Don't violate expectations: if it looks cheap (`size()`, `get_x`) it must
  be cheap.
- Match conventions for entity kinds: `MACRO_CONST`, `TypeName`,
  `lower_snake` variables — different formats act as syntax highlighting.

## Layout

- Consistent formatting; align columns when it makes mistakes visible.
- Group related lines into blocks (decls, setup, work, teardown).
- Consistent order of handling (e.g. always dst-then-src, or alphabetical).

## Comments

- Never comment what the code already says; fix the name instead.
- Do comment **why** (director's commentary), known flaws, invariants,
  surprising constraints, how NOT to use something, and block summaries.
- Use typology consistently: `TODO`, `FIXME`, `HACK`, `NOTE`.
- State intent precisely — no ambiguous pronouns ("count newline bytes",
  not "count them").

## Control flow

- Conditions: put the varying value left, the constant-like value right
  (`bytes_received < expected`).
- Prefer early returns over nesting; keep the guarded code at lowest depth.
- Avoid `do/while` (condition reads *after* the body it guards) and `goto` —
  **except** when faithful equivalence to source control flow requires it;
  then say so in a comment.
- Break giant expressions into named intermediates; use De Morgan's laws to
  flatten `!(a && !b)` → `!a || b`.
- Don't hide work in short-circuit operands.

## Variables

- Fewer live variables at once beats fewer lines of code.
- Shrink scope; declare at first use.
- Prefer write-once locals.

## Functions

- One high-level goal per function; lines serving an unrelated sub-problem
  get extracted.
- Describe the function in a sentence first — if you can't, it does too much.
- Wrap ugly interfaces; don't propagate ugliness.
- Remove unused code.

## Tests

- Readable error messages (prefer a CHECK macro with context over bare assert).
- Simplest input values that do the job (`-10000`, not `-9997`).
- Name tests `test_<unit>_<situation>`.

## Applying this in the airborn port

Rewrites live in `port/rewrite.c`; generated trampolines keep the public
name and preserve the original as `<name>_lifted` (see `REWRITES` in
`tools/gen_port.py`). For this codebase specifically:

- **Equivalence first, readability second.** The lifted body is the oracle.
  Readable form must produce identical `mem[]`/register effects — proven by
  the A/B differential test, not by eye.
- Name register-shaped values by *role*, not by register: `code`, `width`,
  `dict_slot`, `pending_bits` — not `bx`, `cl`. Keep the DOS register name
  only in a comment where the mapping is non-obvious.
- Magic addresses/values that are pure implementation detail of the DOS
  layout (e.g. `0x67f0` dictionary base) may stay literal when the alias
  (`dcomp_*`) already names them; introduce `#define`/`enum` only where a
  literal is a *semantic* constant (code widths, dict size).
- Where the lifted code's structure is the bug-compatible behavior
  (e.g. a do-while that runs once on degenerate input), keep the shape and
  comment it — do not "clean up" into a different loop.
- Helpers (`lzw_read`, `lzw_emit`) earn their existence by being named,
  single-purpose, and side-effect-explicit.
