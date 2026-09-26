/* TANDYSND.EXE overlay module, bound to the runtime-loaded image in m2c::m.
   EXEC loads the real overlay bytes into a host-chosen segment; the data
   labels the translated code touches are redirected into that image via
   the defines below, so all accesses share the single emulated memory. */
#include "asm.h"
namespace m2c { dw tnd_seg = 0; dw tnd_code_seg = 0; bool tnd_present = true; }
#include "tandysnd.exe.h"

namespace m2c {
/* Airborne Ranger INT 9 hardware-keyboard ISR replica. The original handler at
   seg000:0x25a1 survives only as data bytes (not a callable proc), so we
   reproduce it here: when word_1D957 (ds:0xad7) == 1 the game owns the
   keyboard; it reads the scancode, looks up a held-key bitmask in the ds:0x950
   table, and ORs (make) / ANDs (break) it into word_1D959 (ds:0xad9). */
void host_int9_update(int scan_code, bool pressed) {
	db* ds = (db*)m2c::raddr_(0xe8a, 0);
	dw* flag = (dw*)(ds + 0xad7);              /* word_1D957: int9 enable */
	if (*flag != 1) return;
	dw bit = *(dw*)(ds + 0x950 + (scan_code & 0x7f) * 2);
	if (!bit) return;
	dw* held = (dw*)(ds + 0xad9);              /* word_1D959: held-key bitmask */
	if (pressed) *held = (dw)(*held | bit);
	else         *held = (dw)(*held & ~bit);
}

/* Mirror of the original ISR's routing: a scancode is diverted into word_1D959
   only while the ISR is enabled (word_1D957 == 1) AND it has a nonzero entry in
   the ds:0x950 bitmask table. Every other key (including keys pressed while the
   ISR is disabled) chains to the BIOS handler, so it still reaches the INT 16h
   buffer -- that is what lets text-entry / arrow menus keep working. */
bool host_int9_diverts_key(int scan_code) {
	db* ds = (db*)m2c::raddr_(0xe8a, 0);
	if (*(dw*)(ds + 0xad7) != 1) return false;
	return *(dw*)(ds + 0x950 + (scan_code & 0x7f) * 2) != 0;
}
}

/* overlay data labels -> emulated memory at the loaded image (base 0x10000) */
#define seg001      (*(db*)m2c::raddr_(m2c::tnd_seg,0x0070))
#define unk_1009f   (*(db*)m2c::raddr_(m2c::tnd_seg,0x009f))
#define byte_100d7  (*(db*)m2c::raddr_(m2c::tnd_seg,0x00d7))
#define byte_100d9  (*(db*)m2c::raddr_(m2c::tnd_seg,0x00d9))
#define off_10380   (*(dw*)m2c::raddr_(m2c::tnd_seg,0x0380))
#define dword_103de (*(dd*)m2c::raddr_(m2c::tnd_seg,0x03de))
#define byte_103e2  (*(db*)m2c::raddr_(m2c::tnd_seg,0x03e2))

 static bool seg001_33e_proc(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kseg001_33e_proc, _state);}

 static bool seg001_4_proc(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kseg001_4_proc, _state);}

 static bool sub_1040b(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_1040b, _state);}

 static bool sub_104a4(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_104a4, _state);}

 static bool sub_104c5(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_104c5, _state);}

 static bool sub_1051f(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_1051f, _state);}

 static bool sub_1059b(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_1059b, _state);}

 static bool sub_105b0(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_105b0, _state);}

 static bool sub_105c4(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_105c4, _state);}

 __attribute__((weak)) bool sub_10652(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_10652, _state);}

 __attribute__((weak)) bool sub_10661(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_10661, _state);}

 __attribute__((weak)) bool sub_10677(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_10677, _state);}

 __attribute__((weak)) bool sub_1067e(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_1067e, _state);}

 __attribute__((weak)) bool sub_1069e(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_1069e, _state);}

 __attribute__((weak)) bool sub_106a5(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_106a5, _state);}

 __attribute__((weak)) bool sub_106b0(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_106b0, _state);}

 __attribute__((weak)) bool sub_106c3(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_106c3, _state);}

 __attribute__((weak)) bool sub_106d0(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_106d0, _state);}

 __attribute__((weak)) bool sub_106d7(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_106d7, _state);}

 __attribute__((weak)) bool sub_106e2(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_106e2, _state);}

 __attribute__((weak)) bool sub_106e8(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_106e8, _state);}

 __attribute__((weak)) bool sub_106fe(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_106fe, _state);}

 __attribute__((weak)) bool sub_1070d(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_1070d, _state);}

 __attribute__((weak)) bool sub_1071c(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_1071c, _state);}

 __attribute__((weak)) bool sub_1072a(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_1072a, _state);}

 __attribute__((weak)) bool sub_10731(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_10731, _state);}

 __attribute__((weak)) bool sub_1073f(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_1073f, _state);}

 __attribute__((weak)) bool sub_10746(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_10746, _state);}

 __attribute__((weak)) bool sub_1075c(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_1075c, _state);}

 __attribute__((weak)) bool sub_1076b(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_1076b, _state);}

 __attribute__((weak)) bool sub_10781(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_10781, _state);}

 __attribute__((weak)) bool sub_10790(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_10790, _state);}

 __attribute__((weak)) bool sub_107a6(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_107a6, _state);}

 static bool sub_107b2(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_107b2, _state);}

 static bool sub_107b5(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_107b5, _state);}

 __attribute__((weak)) bool sub_107bb(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_107bb, _state);}

 __attribute__((weak)) bool sub_107c9(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_107c9, _state);}

 __attribute__((weak)) bool sub_107d7(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_107d7, _state);}

 __attribute__((weak)) bool sub_107e5(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_107e5, _state);}

 __attribute__((weak)) bool sub_107fa(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_107fa, _state);}

 __attribute__((weak)) bool sub_10801(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_10801, _state);}

 __attribute__((weak)) bool sub_1080e(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_1080e, _state);}

 __attribute__((weak)) bool sub_10814(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_10814, _state);}

 __attribute__((weak)) bool sub_1081a(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_1081a, _state);}

 __attribute__((weak)) bool sub_10820(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_10820, _state);}

 __attribute__((weak)) bool sub_10826(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_10826, _state);}

 __attribute__((weak)) bool sub_10833(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_10833, _state);}

 __attribute__((weak)) bool sub_10851(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_10851, _state);}

 __attribute__((weak)) bool sub_10867(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_10867, _state);}

 static bool sub_10889(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::ksub_10889, _state);}

 static bool loc_1046b(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_1046b, _state);}

 static bool loc_104bb(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_104bb, _state);}

 static bool loc_104bf(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_104bf, _state);}

 static bool loc_104e1(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_104e1, _state);}

 static bool loc_10502(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_10502, _state);}

 static bool loc_10506(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_10506, _state);}

 static bool loc_10566(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_10566, _state);}

 static bool loc_1058c(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_1058c, _state);}

 static bool loc_10592(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_10592, _state);}

 static bool loc_105a8(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_105a8, _state);}

 static bool loc_105d2(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_105d2, _state);}

 static bool loc_105f6(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_105f6, _state);}

 static bool loc_105fa(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_105fa, _state);}

 static bool loc_105fd(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_105fd, _state);}

 static bool loc_10605(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_10605, _state);}

 static bool loc_10615(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_10615, _state);}

 static bool loc_1061f(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_1061f, _state);}

 static bool loc_10629(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_10629, _state);}

 static bool loc_1062d(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_1062d, _state);}

 static bool loc_10630(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_10630, _state);}

 static bool loc_10642(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_10642, _state);}

 static bool loc_1064a(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_1064a, _state);}

 static bool loc_1064d(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_1064d, _state);}

 static bool loc_10694(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_10694, _state);}

 static bool loc_106a9(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_106a9, _state);}

 static bool loc_106f4(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_106f4, _state);}

 static bool loc_10735(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_10735, _state);}

 static bool loc_1074a(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_1074a, _state);}

 static bool loc_1079c(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_1079c, _state);}

 static bool loc_10805(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_10805, _state);}

 static bool loc_108bd(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::kloc_108bd, _state);}

 static bool locret_1008c(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::klocret_1008c, _state);}

 static bool locret_103dd(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::klocret_103dd, _state);}

 static bool locret_1040a(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::klocret_1040a, _state);}

 static bool locret_1046a(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::klocret_1046a, _state);}

 __attribute__((weak)) bool locret_104c4(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::klocret_104c4, _state);}

 static bool locret_10604(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::klocret_10604, _state);}

 static bool locret_1069d(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::klocret_1069d, _state);}

 static bool locret_106c2(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::klocret_106c2, _state);}

 static bool locret_106e7(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::klocret_106e7, _state);}

 static bool locret_107ba(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::klocret_107ba, _state);}

 static bool locret_108bc(m2c::_offsets _i, struct m2c::_STATE* _state){return _group1(_i ? _i : m2c::klocret_108bc, _state);}


  static bool __dispatch_call(m2c::_offsets __i, struct m2c::_STATE* _state){
  X86_REGREF
     __disp=__i;
     switch (__i) {
        case m2c::kedummylabel51: 	if (!_group1(__disp, _state)) return false; break;
        default: { bool handled = false; if (!m2c::dispatch_external_code(__disp, _state, &handled)) return false; if (handled) break; m2c::log_error("Don't know how to call to 0x%x. See " __FILE__ " line %d\n", __disp, __LINE__);m2c::stackDump(_state); abort(); }
     };
     return true;
}

  static bool __dispatch_call_ext(m2c::_offsets __disp, struct m2c::_STATE* _state){
     // Indirect call/jump targets hold aggregate (linked) code offsets
     // (OFFSET emits m2c::kglobal_* and code tables store the same space),
     // so resolve them through the aggregate dispatcher before falling back
     // to this module's local offset switch.
     bool handled = false;
     bool ok = m2c::dispatch_external_code(__disp, _state, &handled);
     if (handled) return ok;
     return __dispatch_call(__disp, _state);
}

#include "tandysnd.exe_seg001.cpp"

namespace m2c {
/* seg001 offset -> k-id, for external far calls / ISR entry / near returns. */
static dd tnd_off2kid(dd off) {
    dd kid = 0;
    switch (off) {
      case 0x0004: kid=m2c::kseg001_4_proc; break;
      case 0x0007: kid=m2c::kedummylabel1; break;
      case 0x0008: kid=m2c::kedummylabel2; break;
      case 0x0009: kid=m2c::kedummylabel3; break;
      case 0x000a: kid=m2c::kedummylabel3; break;
      case 0x000b: kid=m2c::kedummylabel3; break;
      case 0x000e: kid=m2c::kedummylabel3; break;
      case 0x000f: kid=m2c::kedummylabel4; break;
      case 0x0010: kid=m2c::kedummylabel4; break;
      case 0x0011: kid=m2c::kedummylabel4; break;
      case 0x0016: kid=m2c::kedummylabel4; break;
      case 0x0018: kid=m2c::kedummylabel4; break;
      case 0x001c: kid=m2c::klocret_1008c; break;
      case 0x001d: kid=m2c::kedummylabel5; break;
      case 0x001e: kid=m2c::kedummylabel5; break;
      case 0x001f: kid=m2c::kedummylabel5; break;
      case 0x0024: kid=m2c::kedummylabel5; break;
      case 0x0025: kid=m2c::kedummylabel5; break;
      case 0x0026: kid=m2c::kedummylabel6; break;
      case 0x0027: kid=m2c::kedummylabel6; break;
      case 0x0028: kid=m2c::kedummylabel6; break;
      case 0x002d: kid=m2c::kedummylabel6; break;
      case 0x002e: kid=m2c::kedummylabel6; break;
      case 0x0364: kid=m2c::kedummylabel7; break;
      case 0x0367: kid=m2c::kedummylabel7; break;
      case 0x0369: kid=m2c::kedummylabel7; break;
      case 0x036d: kid=m2c::klocret_103dd; break;
      case 0x0373: kid=m2c::kedummylabel8; break;
      case 0x0379: kid=m2c::kedummylabel8; break;
      case 0x037b: kid=m2c::kedummylabel8; break;
      case 0x037c: kid=m2c::kedummylabel8; break;
      case 0x037d: kid=m2c::kedummylabel8; break;
      case 0x037e: kid=m2c::kedummylabel8; break;
      case 0x037f: kid=m2c::kedummylabel8; break;
      case 0x0380: kid=m2c::kedummylabel8; break;
      case 0x0381: kid=m2c::kedummylabel8; break;
      case 0x0382: kid=m2c::kedummylabel8; break;
      case 0x0385: kid=m2c::kedummylabel8; break;
      case 0x0386: kid=m2c::kedummylabel8; break;
      case 0x0387: kid=m2c::kedummylabel8; break;
      case 0x0388: kid=m2c::kedummylabel8; break;
      case 0x038c: kid=m2c::kedummylabel8; break;
      case 0x038e: kid=m2c::kedummylabel8; break;
      case 0x038f: kid=m2c::kedummylabel8; break;
      case 0x0391: kid=m2c::kedummylabel8; break;
      case 0x0393: kid=m2c::kedummylabel8; break;
      case 0x0394: kid=m2c::kedummylabel8; break;
      case 0x039a: kid=m2c::klocret_1040a; break;
      case 0x039b: kid=m2c::ksub_1040b; break;
      case 0x03a0: kid=m2c::kedummylabel9; break;
      case 0x03a5: kid=m2c::kedummylabel9; break;
      case 0x03ab: kid=m2c::kedummylabel9; break;
      case 0x03b1: kid=m2c::kedummylabel9; break;
      case 0x03b2: kid=m2c::kedummylabel9; break;
      case 0x03b7: kid=m2c::kedummylabel9; break;
      case 0x03b9: kid=m2c::kedummylabel9; break;
      case 0x03bd: kid=m2c::kedummylabel9; break;
      case 0x03be: kid=m2c::kedummylabel9; break;
      case 0x03c0: kid=m2c::kedummylabel9; break;
      case 0x03c2: kid=m2c::kedummylabel9; break;
      case 0x03c6: kid=m2c::kedummylabel9; break;
      case 0x03ca: kid=m2c::kedummylabel9; break;
      case 0x03cf: kid=m2c::kedummylabel9; break;
      case 0x03d4: kid=m2c::kedummylabel9; break;
      case 0x03d5: kid=m2c::kedummylabel9; break;
      case 0x03d6: kid=m2c::kedummylabel9; break;
      case 0x03db: kid=m2c::kedummylabel9; break;
      case 0x03dc: kid=m2c::kedummylabel9; break;
      case 0x03de: kid=m2c::kedummylabel9; break;
      case 0x03e0: kid=m2c::kedummylabel9; break;
      case 0x03e4: kid=m2c::kedummylabel9; break;
      case 0x03e7: kid=m2c::kedummylabel9; break;
      case 0x03eb: kid=m2c::kedummylabel9; break;
      case 0x03ec: kid=m2c::kedummylabel9; break;
      case 0x03ee: kid=m2c::kedummylabel9; break;
      case 0x03f0: kid=m2c::kedummylabel9; break;
      case 0x03f3: kid=m2c::kedummylabel9; break;
      case 0x03f5: kid=m2c::kedummylabel9; break;
      case 0x03f7: kid=m2c::kedummylabel9; break;
      case 0x03f9: kid=m2c::kedummylabel9; break;
      case 0x03fa: kid=m2c::klocret_1046a; break;
      case 0x03fb: kid=m2c::kloc_1046b; break;
      case 0x0400: kid=m2c::kloc_1046b; break;
      case 0x0406: kid=m2c::kloc_1046b; break;
      case 0x0407: kid=m2c::kloc_1046b; break;
      case 0x0408: kid=m2c::kloc_1046b; break;
      case 0x040d: kid=m2c::kedummylabel10; break;
      case 0x040e: kid=m2c::kedummylabel10; break;
      case 0x040f: kid=m2c::kedummylabel10; break;
      case 0x0414: kid=m2c::kedummylabel10; break;
      case 0x0419: kid=m2c::kedummylabel10; break;
      case 0x041b: kid=m2c::kedummylabel10; break;
      case 0x041d: kid=m2c::kedummylabel10; break;
      case 0x0421: kid=m2c::kedummylabel10; break;
      case 0x0425: kid=m2c::kedummylabel10; break;
      case 0x0426: kid=m2c::kedummylabel10; break;
      case 0x0428: kid=m2c::kedummylabel10; break;
      case 0x042a: kid=m2c::kedummylabel10; break;
      case 0x042c: kid=m2c::kedummylabel10; break;
      case 0x042e: kid=m2c::kedummylabel10; break;
      case 0x0430: kid=m2c::kedummylabel10; break;
      case 0x0432: kid=m2c::kedummylabel10; break;
      case 0x0433: kid=m2c::kedummylabel10; break;
      case 0x0434: kid=m2c::ksub_104a4; break;
      case 0x0436: kid=m2c::kedummylabel11; break;
      case 0x0438: kid=m2c::kedummylabel11; break;
      case 0x043a: kid=m2c::kedummylabel11; break;
      case 0x043c: kid=m2c::kedummylabel11; break;
      case 0x043e: kid=m2c::kedummylabel11; break;
      case 0x0440: kid=m2c::kedummylabel11; break;
      case 0x0442: kid=m2c::kedummylabel11; break;
      case 0x0444: kid=m2c::kedummylabel11; break;
      case 0x0447: kid=m2c::kedummylabel11; break;
      case 0x044b: kid=m2c::kloc_104bb; break;
      case 0x044d: kid=m2c::kloc_104bb; break;
      case 0x044f: kid=m2c::kloc_104bf; break;
      case 0x0450: kid=m2c::kloc_104bf; break;
      case 0x0452: kid=m2c::kloc_104bf; break;
      case 0x0454: kid=m2c::klocret_104c4; break;
      case 0x0455: kid=m2c::ksub_104c5; break;
      case 0x045a: kid=m2c::kedummylabel12; break;
      case 0x045c: kid=m2c::kedummylabel12; break;
      case 0x0461: kid=m2c::kedummylabel12; break;
      case 0x0463: kid=m2c::kedummylabel12; break;
      case 0x0468: kid=m2c::kedummylabel12; break;
      case 0x046a: kid=m2c::kedummylabel12; break;
      case 0x046e: kid=m2c::kedummylabel12; break;
      case 0x0471: kid=m2c::kloc_104e1; break;
      case 0x0476: kid=m2c::kloc_104e1; break;
      case 0x0478: kid=m2c::kloc_104e1; break;
      case 0x047d: kid=m2c::kloc_104e1; break;
      case 0x047f: kid=m2c::kloc_104e1; break;
      case 0x0484: kid=m2c::kloc_104e1; break;
      case 0x0489: kid=m2c::kloc_104e1; break;
      case 0x048b: kid=m2c::kloc_104e1; break;
      case 0x048f: kid=m2c::kloc_104e1; break;
      case 0x0492: kid=m2c::kloc_10502; break;
      case 0x0496: kid=m2c::kloc_10506; break;
      case 0x049a: kid=m2c::kloc_10506; break;
      case 0x049d: kid=m2c::kloc_10506; break;
      case 0x04a1: kid=m2c::kloc_10506; break;
      case 0x04a4: kid=m2c::kloc_10506; break;
      case 0x04a8: kid=m2c::kloc_10506; break;
      case 0x04ab: kid=m2c::kloc_10506; break;
      case 0x04af: kid=m2c::ksub_1051f; break;
      case 0x04b2: kid=m2c::ksub_1051f; break;
      case 0x04b4: kid=m2c::ksub_1051f; break;
      case 0x04b6: kid=m2c::ksub_1051f; break;
      case 0x04b8: kid=m2c::ksub_1051f; break;
      case 0x04bc: kid=m2c::ksub_1051f; break;
      case 0x04be: kid=m2c::ksub_1051f; break;
      case 0x04c1: kid=m2c::ksub_1051f; break;
      case 0x04c3: kid=m2c::ksub_1051f; break;
      case 0x04c5: kid=m2c::ksub_1051f; break;
      case 0x04c7: kid=m2c::ksub_1051f; break;
      case 0x04c9: kid=m2c::ksub_1051f; break;
      case 0x04cb: kid=m2c::ksub_1051f; break;
      case 0x04ce: kid=m2c::ksub_1051f; break;
      case 0x04d0: kid=m2c::ksub_1051f; break;
      case 0x04d3: kid=m2c::ksub_1051f; break;
      case 0x04d5: kid=m2c::ksub_1051f; break;
      case 0x04d6: kid=m2c::ksub_1051f; break;
      case 0x04d9: kid=m2c::ksub_1051f; break;
      case 0x04db: kid=m2c::ksub_1051f; break;
      case 0x04de: kid=m2c::ksub_1051f; break;
      case 0x04e2: kid=m2c::ksub_1051f; break;
      case 0x04e4: kid=m2c::ksub_1051f; break;
      case 0x04e7: kid=m2c::ksub_1051f; break;
      case 0x04e9: kid=m2c::ksub_1051f; break;
      case 0x04eb: kid=m2c::ksub_1051f; break;
      case 0x04ed: kid=m2c::ksub_1051f; break;
      case 0x04ef: kid=m2c::ksub_1051f; break;
      case 0x04f1: kid=m2c::ksub_1051f; break;
      case 0x04f3: kid=m2c::ksub_1051f; break;
      case 0x04f6: kid=m2c::kloc_10566; break;
      case 0x04f8: kid=m2c::kloc_10566; break;
      case 0x04fa: kid=m2c::kloc_10566; break;
      case 0x04fc: kid=m2c::kloc_10566; break;
      case 0x04ff: kid=m2c::kloc_10566; break;
      case 0x0501: kid=m2c::kloc_10566; break;
      case 0x0504: kid=m2c::kloc_10566; break;
      case 0x0506: kid=m2c::kloc_10566; break;
      case 0x0509: kid=m2c::kloc_10566; break;
      case 0x050b: kid=m2c::kloc_10566; break;
      case 0x050e: kid=m2c::kloc_10566; break;
      case 0x0511: kid=m2c::kloc_10566; break;
      case 0x0513: kid=m2c::kloc_10566; break;
      case 0x0516: kid=m2c::kloc_10566; break;
      case 0x0519: kid=m2c::kloc_10566; break;
      case 0x051c: kid=m2c::kloc_1058c; break;
      case 0x051f: kid=m2c::kloc_1058c; break;
      case 0x0522: kid=m2c::kloc_10592; break;
      case 0x0525: kid=m2c::kloc_10592; break;
      case 0x0527: kid=m2c::kloc_10592; break;
      case 0x0529: kid=m2c::kloc_10592; break;
      case 0x052b: kid=m2c::ksub_1059b; break;
      case 0x052e: kid=m2c::ksub_1059b; break;
      case 0x0530: kid=m2c::ksub_1059b; break;
      case 0x0532: kid=m2c::ksub_1059b; break;
      case 0x0535: kid=m2c::ksub_1059b; break;
      case 0x0538: kid=m2c::kloc_105a8; break;
      case 0x053a: kid=m2c::kloc_105a8; break;
      case 0x053d: kid=m2c::kloc_105a8; break;
      case 0x0540: kid=m2c::kedummylabel13; break;
      case 0x0542: kid=m2c::kedummylabel13; break;
      case 0x0544: kid=m2c::kedummylabel13; break;
      case 0x0546: kid=m2c::kedummylabel13; break;
      case 0x0548: kid=m2c::kedummylabel13; break;
      case 0x0549: kid=m2c::kedummylabel13; break;
      case 0x054c: kid=m2c::kedummylabel13; break;
      case 0x054e: kid=m2c::kedummylabel13; break;
      case 0x0552: kid=m2c::kedummylabel13; break;
      case 0x0554: kid=m2c::ksub_105c4; break;
      case 0x0556: kid=m2c::ksub_105c4; break;
      case 0x0557: kid=m2c::ksub_105c4; break;
      case 0x0559: kid=m2c::ksub_105c4; break;
      case 0x055c: kid=m2c::ksub_105c4; break;
      case 0x055e: kid=m2c::ksub_105c4; break;
      case 0x055f: kid=m2c::ksub_105c4; break;
      case 0x0562: kid=m2c::kloc_105d2; break;
      case 0x0564: kid=m2c::kloc_105d2; break;
      case 0x0565: kid=m2c::kloc_105d2; break;
      case 0x0568: kid=m2c::kloc_105d2; break;
      case 0x056a: kid=m2c::kloc_105d2; break;
      case 0x056c: kid=m2c::kloc_105d2; break;
      case 0x056d: kid=m2c::kloc_105d2; break;
      case 0x056e: kid=m2c::kloc_105d2; break;
      case 0x0571: kid=m2c::kloc_105d2; break;
      case 0x0572: kid=m2c::kloc_105d2; break;
      case 0x0575: kid=m2c::kloc_105d2; break;
      case 0x0576: kid=m2c::kloc_105d2; break;
      case 0x0579: kid=m2c::kloc_105d2; break;
      case 0x057a: kid=m2c::kloc_105d2; break;
      case 0x057d: kid=m2c::kloc_105d2; break;
      case 0x057e: kid=m2c::kloc_105d2; break;
      case 0x0581: kid=m2c::kloc_105d2; break;
      case 0x0582: kid=m2c::kloc_105d2; break;
      case 0x0583: kid=m2c::kloc_105d2; break;
      case 0x0586: kid=m2c::kloc_105f6; break;
      case 0x0588: kid=m2c::kloc_105f6; break;
      case 0x058a: kid=m2c::kloc_105fa; break;
      case 0x058c: kid=m2c::kloc_105fa; break;
      case 0x058d: kid=m2c::kloc_105fd; break;
      case 0x0590: kid=m2c::kloc_105fd; break;
      case 0x0592: kid=m2c::kloc_105fd; break;
      case 0x0594: kid=m2c::klocret_10604; break;
      case 0x0595: kid=m2c::kloc_10605; break;
      case 0x0597: kid=m2c::kloc_10605; break;
      case 0x0598: kid=m2c::kloc_10605; break;
      case 0x0599: kid=m2c::kloc_10605; break;
      case 0x059c: kid=m2c::kloc_10605; break;
      case 0x059e: kid=m2c::kloc_10605; break;
      case 0x05a0: kid=m2c::kloc_10605; break;
      case 0x05a2: kid=m2c::kloc_10605; break;
      case 0x05a5: kid=m2c::kloc_10615; break;
      case 0x05a7: kid=m2c::kloc_10615; break;
      case 0x05a9: kid=m2c::kloc_10615; break;
      case 0x05ab: kid=m2c::kloc_10615; break;
      case 0x05ac: kid=m2c::kloc_10615; break;
      case 0x05af: kid=m2c::kloc_1061f; break;
      case 0x05b1: kid=m2c::kloc_1061f; break;
      case 0x05b3: kid=m2c::kloc_1061f; break;
      case 0x05b5: kid=m2c::kloc_1061f; break;
      case 0x05b6: kid=m2c::kloc_1061f; break;
      case 0x05b9: kid=m2c::kloc_10629; break;
      case 0x05bb: kid=m2c::kloc_10629; break;
      case 0x05bc: kid=m2c::kloc_10629; break;
      case 0x05bd: kid=m2c::kloc_1062d; break;
      case 0x05c0: kid=m2c::kloc_10630; break;
      case 0x05c2: kid=m2c::kloc_10630; break;
      case 0x05c4: kid=m2c::kloc_10630; break;
      case 0x05c6: kid=m2c::kloc_10630; break;
      case 0x05c7: kid=m2c::kloc_10630; break;
      case 0x05cb: kid=m2c::kloc_10630; break;
      case 0x05cd: kid=m2c::kloc_10630; break;
      case 0x05d0: kid=m2c::kloc_10630; break;
      case 0x05d2: kid=m2c::kloc_10642; break;
      case 0x05d4: kid=m2c::kloc_10642; break;
      case 0x05d5: kid=m2c::kloc_10642; break;
      case 0x05d8: kid=m2c::kloc_10642; break;
      case 0x05da: kid=m2c::kloc_1064a; break;
      case 0x05dd: kid=m2c::kloc_1064d; break;
      case 0x05e0: kid=m2c::kloc_1064d; break;
      case 0x05e2: kid=m2c::kedummylabel14; break;
      case 0x05e4: kid=m2c::kedummylabel14; break;
      case 0x05e8: kid=m2c::kedummylabel14; break;
      case 0x05ea: kid=m2c::kedummylabel14; break;
      case 0x05ee: kid=m2c::kedummylabel14; break;
      case 0x05f1: kid=m2c::kedummylabel15; break;
      case 0x05f3: kid=m2c::kedummylabel15; break;
      case 0x05f7: kid=m2c::kedummylabel15; break;
      case 0x05f9: kid=m2c::kedummylabel15; break;
      case 0x05fd: kid=m2c::kedummylabel15; break;
      case 0x0600: kid=m2c::kedummylabel15; break;
      case 0x0604: kid=m2c::kedummylabel15; break;
      case 0x0607: kid=m2c::kedummylabel16; break;
      case 0x060b: kid=m2c::kedummylabel16; break;
      case 0x060e: kid=m2c::kedummylabel17; break;
      case 0x0610: kid=m2c::kedummylabel17; break;
      case 0x0614: kid=m2c::kedummylabel17; break;
      case 0x0618: kid=m2c::kedummylabel17; break;
      case 0x061a: kid=m2c::kedummylabel17; break;
      case 0x061d: kid=m2c::kedummylabel17; break;
      case 0x0621: kid=m2c::kedummylabel17; break;
      case 0x0624: kid=m2c::kloc_10694; break;
      case 0x0625: kid=m2c::kloc_10694; break;
      case 0x0629: kid=m2c::kloc_10694; break;
      case 0x062c: kid=m2c::kloc_10694; break;
      case 0x062d: kid=m2c::klocret_1069d; break;
      case 0x062e: kid=m2c::kedummylabel18; break;
      case 0x0632: kid=m2c::kedummylabel18; break;
      case 0x0635: kid=m2c::kedummylabel19; break;
      case 0x0639: kid=m2c::kloc_106a9; break;
      case 0x063d: kid=m2c::kloc_106a9; break;
      case 0x0640: kid=m2c::kedummylabel20; break;
      case 0x0646: kid=m2c::kedummylabel20; break;
      case 0x064b: kid=m2c::kedummylabel20; break;
      case 0x064d: kid=m2c::kedummylabel20; break;
      case 0x0652: kid=m2c::klocret_106c2; break;
      case 0x0653: kid=m2c::kedummylabel21; break;
      case 0x0659: kid=m2c::kedummylabel21; break;
      case 0x065d: kid=m2c::kedummylabel21; break;
      case 0x0660: kid=m2c::kedummylabel22; break;
      case 0x0664: kid=m2c::kedummylabel22; break;
      case 0x0667: kid=m2c::kedummylabel23; break;
      case 0x066c: kid=m2c::kedummylabel23; break;
      case 0x0671: kid=m2c::kedummylabel23; break;
      case 0x0672: kid=m2c::kedummylabel24; break;
      case 0x0677: kid=m2c::klocret_106e7; break;
      case 0x0678: kid=m2c::kedummylabel25; break;
      case 0x067a: kid=m2c::kedummylabel25; break;
      case 0x067e: kid=m2c::kedummylabel25; break;
      case 0x0680: kid=m2c::kedummylabel25; break;
      case 0x0684: kid=m2c::kloc_106f4; break;
      case 0x0687: kid=m2c::kloc_106f4; break;
      case 0x068b: kid=m2c::kloc_106f4; break;
      case 0x068e: kid=m2c::kedummylabel26; break;
      case 0x0690: kid=m2c::kedummylabel26; break;
      case 0x0694: kid=m2c::kedummylabel26; break;
      case 0x0696: kid=m2c::kedummylabel26; break;
      case 0x069a: kid=m2c::kedummylabel26; break;
      case 0x069d: kid=m2c::kedummylabel27; break;
      case 0x069f: kid=m2c::kedummylabel27; break;
      case 0x06a3: kid=m2c::kedummylabel27; break;
      case 0x06a5: kid=m2c::kedummylabel27; break;
      case 0x06a9: kid=m2c::kedummylabel27; break;
      case 0x06ac: kid=m2c::kedummylabel28; break;
      case 0x06ae: kid=m2c::kedummylabel28; break;
      case 0x06b2: kid=m2c::kedummylabel28; break;
      case 0x06b4: kid=m2c::kedummylabel28; break;
      case 0x06b8: kid=m2c::kedummylabel28; break;
      case 0x06ba: kid=m2c::kedummylabel29; break;
      case 0x06be: kid=m2c::kedummylabel29; break;
      case 0x06c1: kid=m2c::kedummylabel30; break;
      case 0x06c5: kid=m2c::kloc_10735; break;
      case 0x06c8: kid=m2c::kloc_10735; break;
      case 0x06cc: kid=m2c::kloc_10735; break;
      case 0x06cf: kid=m2c::kedummylabel31; break;
      case 0x06d3: kid=m2c::kedummylabel31; break;
      case 0x06d6: kid=m2c::kedummylabel32; break;
      case 0x06da: kid=m2c::kloc_1074a; break;
      case 0x06dc: kid=m2c::kloc_1074a; break;
      case 0x06e0: kid=m2c::kloc_1074a; break;
      case 0x06e2: kid=m2c::kloc_1074a; break;
      case 0x06e5: kid=m2c::kloc_1074a; break;
      case 0x06e9: kid=m2c::kloc_1074a; break;
      case 0x06ec: kid=m2c::kedummylabel33; break;
      case 0x06ee: kid=m2c::kedummylabel33; break;
      case 0x06f2: kid=m2c::kedummylabel33; break;
      case 0x06f4: kid=m2c::kedummylabel33; break;
      case 0x06f8: kid=m2c::kedummylabel33; break;
      case 0x06fb: kid=m2c::kedummylabel34; break;
      case 0x06fd: kid=m2c::kedummylabel34; break;
      case 0x0701: kid=m2c::kedummylabel34; break;
      case 0x0703: kid=m2c::kedummylabel34; break;
      case 0x0707: kid=m2c::kedummylabel34; break;
      case 0x070a: kid=m2c::kedummylabel34; break;
      case 0x070e: kid=m2c::kedummylabel34; break;
      case 0x0711: kid=m2c::kedummylabel35; break;
      case 0x0713: kid=m2c::kedummylabel35; break;
      case 0x0717: kid=m2c::kedummylabel35; break;
      case 0x0719: kid=m2c::kedummylabel35; break;
      case 0x071d: kid=m2c::kedummylabel35; break;
      case 0x0720: kid=m2c::kedummylabel36; break;
      case 0x0722: kid=m2c::kedummylabel36; break;
      case 0x0726: kid=m2c::kedummylabel36; break;
      case 0x0728: kid=m2c::kedummylabel36; break;
      case 0x072c: kid=m2c::kloc_1079c; break;
      case 0x072f: kid=m2c::kloc_1079c; break;
      case 0x0733: kid=m2c::kloc_1079c; break;
      case 0x0736: kid=m2c::kedummylabel37; break;
      case 0x0738: kid=m2c::kedummylabel37; break;
      case 0x073c: kid=m2c::kedummylabel37; break;
      case 0x073e: kid=m2c::kedummylabel37; break;
      case 0x0742: kid=m2c::ksub_107b2; break;
      case 0x0745: kid=m2c::ksub_107b5; break;
      case 0x0746: kid=m2c::ksub_107b5; break;
      case 0x0749: kid=m2c::ksub_107b5; break;
      case 0x074a: kid=m2c::klocret_107ba; break;
      case 0x074b: kid=m2c::kedummylabel38; break;
      case 0x074d: kid=m2c::kedummylabel38; break;
      case 0x0751: kid=m2c::kedummylabel38; break;
      case 0x0753: kid=m2c::kedummylabel38; break;
      case 0x0757: kid=m2c::kedummylabel38; break;
      case 0x0759: kid=m2c::kedummylabel39; break;
      case 0x075b: kid=m2c::kedummylabel39; break;
      case 0x075f: kid=m2c::kedummylabel39; break;
      case 0x0761: kid=m2c::kedummylabel39; break;
      case 0x0765: kid=m2c::kedummylabel39; break;
      case 0x0767: kid=m2c::kedummylabel40; break;
      case 0x0769: kid=m2c::kedummylabel40; break;
      case 0x076d: kid=m2c::kedummylabel40; break;
      case 0x076f: kid=m2c::kedummylabel40; break;
      case 0x0773: kid=m2c::kedummylabel40; break;
      case 0x0775: kid=m2c::kedummylabel41; break;
      case 0x0777: kid=m2c::kedummylabel41; break;
      case 0x077b: kid=m2c::kedummylabel41; break;
      case 0x077d: kid=m2c::kedummylabel41; break;
      case 0x0781: kid=m2c::kedummylabel41; break;
      case 0x0784: kid=m2c::kedummylabel41; break;
      case 0x0788: kid=m2c::kedummylabel41; break;
      case 0x078a: kid=m2c::kedummylabel42; break;
      case 0x078e: kid=m2c::kedummylabel42; break;
      case 0x0791: kid=m2c::kedummylabel43; break;
      case 0x0795: kid=m2c::kloc_10805; break;
      case 0x0798: kid=m2c::kloc_10805; break;
      case 0x079c: kid=m2c::kloc_10805; break;
      case 0x079e: kid=m2c::kedummylabel44; break;
      case 0x07a2: kid=m2c::kedummylabel44; break;
      case 0x07a4: kid=m2c::kedummylabel45; break;
      case 0x07a8: kid=m2c::kedummylabel45; break;
      case 0x07aa: kid=m2c::kedummylabel46; break;
      case 0x07ae: kid=m2c::kedummylabel46; break;
      case 0x07b0: kid=m2c::kedummylabel47; break;
      case 0x07b4: kid=m2c::kedummylabel47; break;
      case 0x07b6: kid=m2c::kedummylabel48; break;
      case 0x07ba: kid=m2c::kedummylabel48; break;
      case 0x07bd: kid=m2c::kedummylabel48; break;
      case 0x07c1: kid=m2c::kedummylabel48; break;
      case 0x07c3: kid=m2c::kedummylabel49; break;
      case 0x07c7: kid=m2c::kedummylabel49; break;
      case 0x07ca: kid=m2c::kedummylabel49; break;
      case 0x07ce: kid=m2c::kedummylabel49; break;
      case 0x07d2: kid=m2c::kedummylabel49; break;
      case 0x07d6: kid=m2c::kedummylabel49; break;
      case 0x07d9: kid=m2c::kedummylabel49; break;
      case 0x07dd: kid=m2c::kedummylabel49; break;
      case 0x07e1: kid=m2c::ksub_10851; break;
      case 0x07e5: kid=m2c::ksub_10851; break;
      case 0x07e8: kid=m2c::ksub_10851; break;
      case 0x07ec: kid=m2c::ksub_10851; break;
      case 0x07f0: kid=m2c::ksub_10851; break;
      case 0x07f4: kid=m2c::ksub_10851; break;
      case 0x07f7: kid=m2c::kedummylabel50; break;
      case 0x07fa: kid=m2c::kedummylabel50; break;
      case 0x07fe: kid=m2c::kedummylabel50; break;
      case 0x0802: kid=m2c::kedummylabel50; break;
      case 0x0806: kid=m2c::kedummylabel50; break;
      case 0x080a: kid=m2c::kedummylabel50; break;
      case 0x080e: kid=m2c::kedummylabel50; break;
      case 0x0812: kid=m2c::kedummylabel50; break;
      case 0x0814: kid=m2c::kedummylabel50; break;
      case 0x0818: kid=m2c::kedummylabel50; break;
      case 0x0819: kid=m2c::kedummylabel51; break;
      case 0x081e: kid=m2c::kedummylabel51; break;
      case 0x0820: kid=m2c::kedummylabel51; break;
      case 0x0822: kid=m2c::kedummylabel51; break;
      case 0x0825: kid=m2c::kedummylabel51; break;
      case 0x0827: kid=m2c::kedummylabel51; break;
      case 0x0829: kid=m2c::kedummylabel51; break;
      case 0x082c: kid=m2c::kedummylabel51; break;
      case 0x082f: kid=m2c::kedummylabel51; break;
      case 0x0831: kid=m2c::kedummylabel51; break;
      case 0x0833: kid=m2c::kedummylabel51; break;
      case 0x0835: kid=m2c::kedummylabel51; break;
      case 0x0838: kid=m2c::kedummylabel51; break;
      case 0x083a: kid=m2c::kedummylabel51; break;
      case 0x083c: kid=m2c::kedummylabel51; break;
      case 0x083e: kid=m2c::kedummylabel51; break;
      case 0x0840: kid=m2c::kedummylabel51; break;
      case 0x0842: kid=m2c::kedummylabel51; break;
      case 0x0844: kid=m2c::kedummylabel51; break;
      case 0x0846: kid=m2c::kedummylabel51; break;
      case 0x0848: kid=m2c::kedummylabel51; break;
      case 0x084a: kid=m2c::kedummylabel51; break;
      case 0x084c: kid=m2c::klocret_108bc; break;
      case 0x084d: kid=m2c::kloc_108bd; break;
      case 0x084f: kid=m2c::kloc_108bd; break;
      case 0x0851: kid=m2c::kloc_108bd; break;
      case 0x0853: kid=m2c::kloc_108bd; break;
      case 0x0855: kid=m2c::kloc_108bd; break;
      case 0x0857: kid=m2c::kloc_108bd; break;
      case 0x0859: kid=m2c::kloc_108bd; break;
      default: return 0;
    }
    return kid;
}
/* Route a call target into the translated overlay.
   disp is either a seg001 code offset (external entry / stored table) or an
   already-resolved k-id (the generated jump table may hold k-ids). */
bool tnd_overlay_call(dd disp, _STATE* _state) {
    {   db* im = (db*)raddr_(tnd_seg, 0);
        m2c::log_debug2("tnd enter disp=%x img=%02x%02x%02x%02x\n", disp, im[0],im[1],im[2],im[3]); }
    /* The overlay executes at its code segment (tnd_code_seg). Entries do
       'push cs; pop ds' to load ds=cs, and the ISR uses cs: addressing, so cs
       must reflect the driver segment before dispatch. */
    _state->cs = tnd_code_seg;

    /* The ISR tails off with 'jmp cs:dword_103DE', chaining to the int8 vector
       that was installed before the driver. When no previous handler exists the
       saved vector is 0:0 -> disp==0. There is nothing to chain to (the host
       IRQ thread already advances the BIOS tick), so a null target is a no-op. */
    if (disp == 0) return true;

    dd kid = 0;
    if (disp >= 0x1000 && disp <= 0xffff) {
        kid = disp;                       // already a k-id (near/indirect dispatch)
    } else {
        dd off = disp & 0xffff;           // seg001 code offset
        if (off == 0x0002) {              // 'push cs; pop ds' + fall into install
            _state->ds = _state->cs;
            kid = m2c::kseg001_4_proc;
        } else {
            kid = tnd_off2kid(off);
            if (!kid) {
                db* img = (db*)raddr_(tnd_seg, 0);
                m2c::log_error("tnd miss disp=%x bx=%x ds=%x tnd_seg=%x hdr=%02x%02x%02x%02x tbl0=%04x flag=%02x &m=%p imgp=%p\n",
                    disp, (_state->ebx & 0xffff), _state->ds, tnd_seg, img[0], img[1], img[2], img[3],
                    *(dw*)raddr_(tnd_seg, 0x380), *(db*)raddr_(tnd_seg, 0xd7),
                    (void*)&m2c::m, (void*)img);
                return false;
            }
        }
    }
    bool r = _group1((m2c::_offsets)kid, _state);
    {   db* im = (db*)raddr_(tnd_seg, 0);
        m2c::log_debug2("tnd exit disp=%x r=%d img=%02x%02x%02x%02x\n", disp, r, im[0],im[1],im[2],im[3]); }
    return r;
}
}
