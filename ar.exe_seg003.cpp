/* THIS IS GENERATED FILE */

        
#include "ar.exe.h"

                

 static bool seg003_0_proc(m2c::_offsets _i, struct m2c::_STATE* _state){
    X86_REGREF
    __disp = _i;

    if (__disp == 0) goto _begin;
    else goto __dispatch_call;
    seg003_0_proc:
    _begin:
	// 0 ;~ 1CF3:0000
	// 0 ;~ 1CF3:0000
	// 0 ;~ 1CF3:0000
	// 0 ; DATA XREF: seg002:01E0↑o ;~ 1CF3:0000
	// 0 ; seg002:01F8↑o ... ;~ 1CF3:0000
	// 0 ;~ 1CF3:C8C0
	// 0 ;~ 1CF3:C8CB
	// 0 ;~ 1CF3:C8D6
	// 0 ;~ 1CF3:C8E1
	// 0 ;~ 1CF3:C8EC
	// 0 ;~ 1CF3:C8F7
	// 0 ;~ 1CF3:C902
	// 0 ;~ 1CF3:C90D
	// 0 ;~ 1CF3:C918
	// 0 ;~ 1CF3:C923
	// 0 ;~ 1CF3:C92E
	// 0 ;~ 1CF3:C939
	// 0 ;~ 1CF3:C944
	// 0 ;~ 1CF3:C94F
	// 0 ;~ 1CF3:C95A
	// 0 ;~ 1CF3:C965
	// 0 ;~ 1CF3:C970
	// 0 ;~ 1CF3:C97B
	// 0 ;~ 1CF3:C986
	// 0 ;~ 1CF3:C991
	// 0 ;~ 1CF3:C99C
	// 0 ;~ 1CF3:C9A7
	// 0 ;~ 1CF3:C9B2
	// 0 ;~ 1CF3:C9BD
	// 0 ;~ 1CF3:C9C8
	// 0 ;~ 1CF3:C9D3
	// 0 ;~ 1CF3:C9DE
	// 0 ;~ 1CF3:C9E9
	// 0 ;~ 1CF3:C9F4
	// 0 ;~ 1CF3:C9FF
	// 0 ;~ 1CF3:CA0A
	// 0 ;~ 1CF3:CA15
	// 0 ;~ 1CF3:CA20
	// 0 ;~ 1CF3:CA2B
	// 0 ;~ 1CF3:CA36
	// 0 ;~ 1CF3:CA41
	// 0 ;~ 1CF3:CA4C
	// 0 ;~ 1CF3:CA57
	// 0 ;~ 1CF3:CA62
	// 0 ;~ 1CF3:CA6D
	// 0 ;~ 1CF3:CA78
	// 0 ;~ 1CF3:CA83
	// 0 ;~ 1CF3:CA8E
	// 0 ;~ 1CF3:CA99
	// 0 ;~ 1CF3:CAA4
	// 0 ;~ 1CF3:CAAF
	// 0 ;~ 1CF3:CABA
	// 0 ;~ 1CF3:CAC5
	// 0 ;~ 1CF3:CAD0
	// 0 ;~ 1CF3:CADB
	// 0 ;~ 1CF3:CAE6
	// 0 ;~ 1CF3:CAF1
	// 0 ;~ 1CF3:CAFC
	// 0 ;~ 1CF3:CB07

            assert(0);
            __dispatch_call_ext:
            { bool handled = false;
              bool ok = m2c::dispatch_external_code(__disp, _state, &handled);
              if (handled) return ok; }
            __dispatch_call:
        #ifdef DOSBOX_CUSTOM
            if ((__disp >> 16) == 0xf000)
            {cs=0xf000;eip=__disp&0xffff;m2c::fix_segs();return false;}  // Jumping to BIOS
        #endif
            switch (__disp) {
                case m2c::kseg003_0_proc: 	goto seg003_0_proc;
        default: { bool handled = false; if (!m2c::dispatch_external_code(__disp, _state, &handled)) return false; if (handled) break; m2c::log_error("Don't know how to jump to 0x%x. See " __FILE__ " line %d\n", __disp, __LINE__);m2c::stackDump(_state); abort(); }
    };
}

