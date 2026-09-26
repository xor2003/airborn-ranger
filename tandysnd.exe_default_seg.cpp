/* THIS IS GENERATED FILE */

        
#include "tandysnd.exe.h"

                

 static bool mainproc(m2c::_offsets _i, struct m2c::_STATE* _state){
    X86_REGREF
    __disp = _i;

    if (__disp == 0) goto _begin;
    else goto __dispatch_call;
    mainproc:
    _begin:
	// 0 ;~ None:0000
	// 0 ;~ None:0000
	// 0 ;~ None:0000
	// 0 ;~ None:0000
	// 0 ;~ None:0001
	// 0 ;~ None:0002
	// 0 ;~ None:0003
	// 0 ;~ None:0004
	// 0 ;~ None:0005
	// 0 ;~ None:0006
	// 0 ;~ None:0007
	// 0 ;~ None:0008
	// 0 ; E ;~ None:0009
	// 0 ;~ None:000A
	// 0 ;~ None:000B
	// 0 ;~ None:000C
	// 0 ; 9 ;~ None:000D
	// 0 ;~ None:000E
	// 0 ; 0 ;~ None:000F
	// 0 ; 1 ;~ None:0010
	// 0 ;~ None:0011
	// 0 ; 8 ;~ None:0012
	// 0 ; 8 ;~ None:0013
	// 0 ;~ None:0014
	// 0 ;~ None:0018
	// 0 ;~ None:001A
	// 0 ;~ None:001C
	// 0 ;~ None:002A
	// 0 ;~ None:0038
	// 0 ;~ None:0044
	// 0 ;~ None:004F
	// 0 ;~ None:005A
	// 0 ;~ None:0064
	// 0 ;~ None:006F
	J(CALL(__dispatch_call_ext,start));	J(RETN(0));
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
                case m2c::kmainproc: 	goto mainproc;
        default: { bool handled = false; if (!m2c::dispatch_external_code(__disp, _state, &handled)) return false; if (handled) break; m2c::log_error("Don't know how to jump to 0x%x. See " __FILE__ " line %d\n", __disp, __LINE__);m2c::stackDump(_state); abort(); }
    };
}

