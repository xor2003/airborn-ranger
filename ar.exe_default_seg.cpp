/* THIS IS GENERATED FILE */

        
#include "ar.exe.h"

                

 static bool mainproc(m2c::_offsets _i, struct m2c::_STATE* _state){
    X86_REGREF
    __disp = _i;

    if (__disp == 0) goto _begin;
    else goto __dispatch_call;
    mainproc:
    _begin:
	// 0 ;~ 01A2:0000
	// 0 ;~ 01A2:0000
	// 0 ; DATA XREF: start+6↓r ;~ 01A2:0000
	// 0 ; start:loc_10040↓r ... ;~ 01A2:0000
	// 0 ; DATA XREF: start+31↓r ;~ 01A2:0002
	// 0 ; sub_100C5+9↓r ... ;~ 01A2:0002
	// 0 ; DATA XREF: sub_11D12+36↓r ;~ 01A2:0004
	// 0 ; sub_14B0C:loc_14B1A↓r ;~ 01A2:0004
	// 0 ;~ 01A2:0006
	// 0 ; DATA XREF: sub_10B43+8↓r ;~ 01A2:000E
	// 0 ; sub_10BD6+8↓r ... ;~ 01A2:000E
	// 0 ; DATA XREF: sub_10ABF+8↓r ;~ 01A2:0010
	// 0 ; sub_10C36+1↓r ... ;~ 01A2:0010
	// 0 ; DATA XREF: sub_10B20+8↓r ;~ 01A2:0012
	// 0 ; sub_10C7C+1↓r ... ;~ 01A2:0012
	// 0 ; DATA XREF: sub_11643:loc_11671↓r ;~ 01A2:0014
	// 0 ; sub_11643:loc_1168B↓r ;~ 01A2:0014
	// 0 ; DATA XREF: sub_11643+6D↓r ;~ 01A2:0016
	// 0 ; sub_11643+8E↓r ... ;~ 01A2:0016
	// 0 ; DATA XREF: sub_106EE↓r ;~ 01A2:0018
	// 0 ; sub_11643+1F↓w ... ;~ 01A2:0018
	// 0 ;~ 01A2:001A
	// 0 ;~ 01A2:001C
	// 0 ;~ 01A2:001E
	// 0 ;~ 01A2:0020
	// 0 ;~ 01A2:0020
	// 0 ; =============== S U B R O U T I N E ======================================= ;~ 01A2:0020
	// 0 ;~ 01A2:0020
	// 0 ; Attributes: noreturn ;~ 01A2:0020
	// 0 ;~ 01A2:0020
	// 0 ;~ 01A2:0020
	J(CALL(start,0));	J(RETN(0));
            assert(0);
            __dispatch_call_ext:
            { bool handled = false;
              bool ok = m2c::dispatch_external_code(__disp, _state, &handled);
              if (handled) return ok; }
            if (__disp <= 0xffff) { __disp = ((dd)cs << 16) | __disp; }
            __dispatch_call:
        #ifdef DOSBOX_CUSTOM
            if ((__disp >> 16) == 0xf000)
            {cs=0xf000;eip=__disp&0xffff;m2c::fix_segs();return false;}  // Jumping to BIOS
        #endif
            switch (__disp) {
                case m2c::kmainproc: 	goto mainproc;
        default: { bool handled = false; if (!m2c::dispatch_external_code(__disp, _state, &handled)) return false; if (handled) break; m2c::log_error("Don't know how to jump to 0x%x. See " __FILE__ " line %d\n", __disp, __LINE__);m2c::stackDump(_state); abort(); }
    };
}

