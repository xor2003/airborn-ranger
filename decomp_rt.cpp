/* Decompile-artifact runtime for the M2CDEBUG=-1 build.
 *
 * This file only exists so the generated game code links into a 32-bit ELF
 * that IDA / Hex-Rays-32 can open and decompile.  It is never executed:
 * correctness of the helpers does not matter, only that every symbol the
 * translated code references has a definition so the linker is happy and the
 * real game functions keep their shape in the decompiled output.
 */

#include "asm.h"

// ---------------------------------------------------------------------------
// Global-scope register / flag storage used by the generated procs
// (asm_regs_decomp.h declares these extern at file scope).
// ---------------------------------------------------------------------------
uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip;
m2c::dw cs, ds, es, fs, gs, ss;
bool CF, PF, AF, ZF, SF, DF, OF, IF, TF;
dd other_flags;

// ---------------------------------------------------------------------------
// Namespace-scoped twins referenced by the runtime helpers (functions compiled
// inside `namespace m2c` resolve the same extern names to m2c::*).
// ---------------------------------------------------------------------------
namespace m2c {
uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip;
dw cs, ds, es, fs, gs, ss;
bool CF, PF, AF, ZF, SF, DF, OF, IF, TF;
dd other_flags;
}

// loc_14e9c is a mid-function label inside _group40 that the generator did not
// emit a wrapper for; the linker only needs it to resolve for the artifact.
bool loc_14e9c(m2c::_offsets _i, struct m2c::_STATE* _state) {
    (void)_i; (void)_state;
    return false;
}
