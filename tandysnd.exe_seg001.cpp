/* THIS IS GENERATED FILE */

        
#include "tandysnd.exe.h"

                

 static bool _group1(m2c::_offsets _i, struct m2c::_STATE* _state){
    X86_REGREF
    __disp = _i;

    if (__disp == 0) goto _begin;
    else goto __dispatch_call;
    _group1:
    _begin:
seg001_4_proc:
	// 74 
	J(CALL(sub_1040b,0));	// 74                  call    sub_1040B ;~ None:0004
edummylabel1:
	// 4369 
	J(RETF(0));	// 75                  retf ;~ None:0007
	// 0 ; --------------------------------------------------------------------------- ;~ None:0008
edummylabel2:
	// 4371 
	J(RETF(0));	// 77                  retf ;~ None:0008
	// 0 ; --------------------------------------------------------------------------- ;~ None:0009
edummylabel3:
	// 4373 
	R(PUSH(cs));	// 79                  push    cs ;~ None:0009
	R(POP(ds));	// 80                  pop     ds ;~ None:000A
	// 0 ;~ None:000B
	J(CALL(sub_104a4,0));	// 82                  call    sub_104A4 ;~ None:000B
	J(RETF(0));	// 83                  retf ;~ None:000E
	// 0 ; --------------------------------------------------------------------------- ;~ None:000F
edummylabel4:
	// 4375 
	R(PUSH(cs));	// 85                  push    cs ;~ None:000F
	R(POP(ds));	// 86                  pop     ds ;~ None:0010
	R(CMP(byte_100d7, 0x0FF));	// 87                  cmp     byte_100D7, 0FFh ;~ None:0011
	J(JZ(locret_1008c));	// 88                  jz      short locret_1008C ;~ None:0016
	J(CALL(__dispatch_call_ext,*(dw*)(((db*)&off_10380)+bx)));	// 89                  call    off_10380[bx] ;~ None:0018
	// 0 ;~ None:001C
locret_1008c:
	// 4377 
	// 0 ; CODE XREF: seg001:0016↑j ;~ None:001C
	J(RETF(0));	// 92                  retf ;~ None:001C
	// 0 ; --------------------------------------------------------------------------- ;~ None:001D
edummylabel5:
	// 4379 
	R(PUSH(cs));	// 94                  push    cs ;~ None:001D
	R(POP(ds));	// 95                  pop     ds ;~ None:001E
	R(byte_100d7 = 0;);	// 96                  mov     byte_100D7, 0 ;~ None:001F
	R(NOP);	// 97                  nop ;~ None:0024
	J(RETF(0));	// 98                  retf ;~ None:0025
	// 0 ; --------------------------------------------------------------------------- ;~ None:0026
edummylabel6:
	// 4381 
	R(PUSH(cs));	// 100                  push    cs ;~ None:0026
	R(POP(ds));	// 101                  pop     ds ;~ None:0027
	R(byte_100d7 = 0x0FF;);	// 102                  mov     byte_100D7, 0FFh ;~ None:0028
	R(NOP);	// 103                  nop ;~ None:002D
	J(RETF(0));	// 104                  retf ;~ None:002E
	// 0 ; --------------------------------------------------------------------------- ;~ None:002E
	// 0 ; DATA XREF: sub_104A4+13↓o ;~ None:002F
	// 0 ;~ None:0030
	// 0 ;~ None:0031
	// 0 ;~ None:0032
	// 0 ;~ None:0033
	// 0 ;~ None:0034
	// 0 ;~ None:0035
	// 0 ;~ None:0036
	// 0 ;~ None:0037
	// 0 ;~ None:0038
	// 0 ;~ None:0039
	// 0 ;~ None:003A
	// 0 ;~ None:003B
	// 0 ;~ None:003C
	// 0 ;~ None:003D
	// 0 ;~ None:003E
	// 0 ;~ None:003F
	// 0 ;~ None:0040
	// 0 ;~ None:0041
	// 0 ;~ None:0042
	// 0 ;~ None:0043
	// 0 ;~ None:0044
	// 0 ;~ None:0045
	// 0 ;~ None:0046
	// 0 ;~ None:0047
	// 0 ;~ None:0048
	// 0 ;~ None:0049
	// 0 ;~ None:004A
	// 0 ;~ None:004B
	// 0 ;~ None:004C
	// 0 ;~ None:004D
	// 0 ;~ None:004E
	// 0 ;~ None:004F
	// 0 ;~ None:0050
	// 0 ;~ None:0051
	// 0 ;~ None:0052
	// 0 ;~ None:0053
	// 0 ;~ None:0054
	// 0 ;~ None:0055
	// 0 ;~ None:0056
	// 0 ;~ None:0057
	// 0 ;~ None:0058
	// 0 ;~ None:0059
	// 0 ;~ None:005A
	// 0 ;~ None:005B
	// 0 ;~ None:005C
	// 0 ;~ None:005D
	// 0 ;~ None:005E
	// 0 ;~ None:005F
	// 0 ;~ None:0060
	// 0 ;~ None:0061
	// 0 ;~ None:0062
	// 0 ;~ None:0063
	// 0 ;~ None:0065
	// 0 ;~ None:0066
	// 0 ; DATA XREF: seg001:0011↑r ;~ None:0067
	// 0 ; seg001:001F↑w ... ;~ None:0067
	// 0 ;~ None:0068
	// 0 ; DATA XREF: seg001:0388↓w ;~ None:0069
	// 0 ;~ None:006A
	// 0 ;~ None:006B
	// 0 ;~ None:006C
	// 0 ;~ None:006D
	// 0 ;~ None:006E
	// 0 ;~ None:006F
	// 0 ; , ;~ None:0070
	// 0 ;~ None:0071
	// 0 ;~ None:0072
	// 0 ; Z ;~ None:0073
	// 0 ;~ None:0074
	// 0 ;~ None:0075
	// 0 ;~ None:0076
	// 0 ;~ None:0077
	// 0 ;~ None:0078
	// 0 ; @ ;~ None:0079
	// 0 ;~ None:007A
	// 0 ;~ None:007B
	// 0 ; M ;~ None:007C
	// 0 ;~ None:007D
	// 0 ;~ None:007E
	// 0 ; - ;~ None:007F
	// 0 ;~ None:0080
	// 0 ; / ;~ None:0081
	// 0 ; C ;~ None:0082
	// 0 ;~ None:0083
	// 0 ;~ None:0084
	// 0 ; I ;~ None:0085
	// 0 ;~ None:0086
	// 0 ;~ None:0087
	// 0 ;~ None:0088
	// 0 ;~ None:0089
	// 0 ; & ;~ None:008A
	// 0 ; < ;~ None:008B
	// 0 ;~ None:008C
	// 0 ;~ None:008D
	// 0 ;~ None:008E
	// 0 ;~ None:008F
	// 0 ;~ None:0090
	// 0 ;~ None:0091
	// 0 ;~ None:0092
	// 0 ;~ None:0093
	// 0 ;~ None:0094
	// 0 ;~ None:0095
	// 0 ;~ None:0096
	// 0 ;~ None:0097
	// 0 ;~ None:0098
	// 0 ;~ None:0099
	// 0 ;~ None:009A
	// 0 ;~ None:009B
	// 0 ;~ None:009C
	// 0 ;~ None:009D
	// 0 ;~ None:009E
	// 0 ;~ None:009F
	// 0 ;~ None:00A0
	// 0 ;~ None:00A1
	// 0 ;~ None:00A2
	// 0 ;~ None:00A3
	// 0 ; < ;~ None:00A4
	// 0 ;~ None:00A5
	// 0 ;~ None:00A6
	// 0 ;~ None:00A7
	// 0 ;~ None:00A8
	// 0 ;~ None:00A9
	// 0 ;~ None:00AA
	// 0 ;~ None:00AB
	// 0 ; $ ;~ None:00AC
	// 0 ;~ None:00AD
	// 0 ;~ None:00AE
	// 0 ; R ;~ None:00AF
	// 0 ;~ None:00B0
	// 0 ;~ None:00B1
	// 0 ;~ None:00B2
	// 0 ;~ None:00B3
	// 0 ;~ None:00B4
	// 0 ;~ None:00B5
	// 0 ;~ None:00B6
	// 0 ;~ None:00B7
	// 0 ;~ None:00B8
	// 0 ;~ None:00B9
	// 0 ;~ None:00BA
	// 0 ;~ None:00BB
	// 0 ;~ None:00BC
	// 0 ; g ;~ None:00BD
	// 0 ;~ None:00BE
	// 0 ;~ None:00BF
	// 0 ;~ None:00C0
	// 0 ;~ None:00C1
	// 0 ;~ None:00C2
	// 0 ;~ None:00C3
	// 0 ;~ None:00C4
	// 0 ;~ None:00C5
	// 0 ;~ None:00C6
	// 0 ;~ None:00C7
	// 0 ;~ None:00C8
	// 0 ;~ None:00C9
	// 0 ;~ None:00CA
	// 0 ;~ None:00CB
	// 0 ; 4 ;~ None:00CC
	// 0 ;~ None:00CD
	// 0 ;~ None:00CE
	// 0 ;~ None:00CF
	// 0 ;~ None:00D0
	// 0 ;~ None:00D1
	// 0 ;~ None:00D2
	// 0 ; W ;~ None:00D3
	// 0 ;~ None:00D4
	// 0 ;~ None:00D5
	// 0 ;~ None:00D6
	// 0 ;~ None:00D7
	// 0 ;~ None:00D8
	// 0 ;~ None:00D9
	// 0 ;~ None:00DA
	// 0 ;~ None:00DB
	// 0 ; D ;~ None:00DC
	// 0 ;~ None:00DD
	// 0 ;~ None:00DE
	// 0 ;~ None:00DF
	// 0 ;~ None:00E0
	// 0 ;~ None:00E1
	// 0 ;~ None:00E2
	// 0 ; E ;~ None:00E3
	// 0 ;~ None:00E4
	// 0 ;~ None:00E5
	// 0 ;~ None:00E6
	// 0 ;~ None:00E7
	// 0 ;~ None:00E8
	// 0 ; G ;~ None:00E9
	// 0 ; E ;~ None:00EA
	// 0 ;~ None:00EB
	// 0 ;~ None:00EC
	// 0 ;~ None:00ED
	// 0 ;~ None:00EE
	// 0 ;~ None:00EF
	// 0 ;~ None:00F0
	// 0 ;~ None:00F1
	// 0 ;~ None:00F2
	// 0 ;~ None:00F3
	// 0 ;~ None:00F4
	// 0 ;~ None:00F5
	// 0 ; $ ;~ None:00F6
	// 0 ;~ None:00F7
	// 0 ;~ None:00F8
	// 0 ;~ None:00F9
	// 0 ;~ None:00FA
	// 0 ;~ None:00FB
	// 0 ;~ None:00FC
	// 0 ; 0 ;~ None:00FD
	// 0 ; < ;~ None:00FE
	// 0 ;~ None:00FF
	// 0 ;~ None:0100
	// 0 ;~ None:0101
	// 0 ;~ None:0102
	// 0 ;~ None:0103
	// 0 ;~ None:0104
	// 0 ;~ None:0105
	// 0 ; = ;~ None:0106
	// 0 ;~ None:0107
	// 0 ;~ None:0108
	// 0 ;~ None:0109
	// 0 ;~ None:010A
	// 0 ;~ None:010B
	// 0 ;~ None:010C
	// 0 ;~ None:010D
	// 0 ; = ;~ None:010E
	// 0 ;~ None:010F
	// 0 ;~ None:0110
	// 0 ;~ None:0111
	// 0 ;~ None:0112
	// 0 ;~ None:0113
	// 0 ;~ None:0114
	// 0 ;~ None:0115
	// 0 ; 4 ;~ None:0116
	// 0 ;~ None:0117
	// 0 ;~ None:0118
	// 0 ;~ None:0119
	// 0 ;~ None:011A
	// 0 ;~ None:011B
	// 0 ;~ None:011C
	// 0 ;~ None:011D
	// 0 ;~ None:011E
	// 0 ;~ None:011F
	// 0 ;~ None:0120
	// 0 ;~ None:0121
	// 0 ;~ None:0122
	// 0 ;~ None:0123
	// 0 ;~ None:0124
	// 0 ;~ None:0125
	// 0 ;~ None:0126
	// 0 ;~ None:0127
	// 0 ;~ None:0128
	// 0 ;~ None:0129
	// 0 ;~ None:012A
	// 0 ;~ None:012B
	// 0 ;~ None:012C
	// 0 ;~ None:012D
	// 0 ;~ None:012E
	// 0 ;~ None:012F
	// 0 ;~ None:0130
	// 0 ;~ None:0131
	// 0 ;~ None:0132
	// 0 ;~ None:0133
	// 0 ;~ None:0134
	// 0 ;~ None:0135
	// 0 ;~ None:0136
	// 0 ;~ None:0137
	// 0 ;~ None:0138
	// 0 ; < ;~ None:0139
	// 0 ;~ None:013A
	// 0 ;~ None:013B
	// 0 ;~ None:013C
	// 0 ;~ None:013D
	// 0 ;~ None:013E
	// 0 ;~ None:013F
	// 0 ; 0 ;~ None:0140
	// 0 ; < ;~ None:0141
	// 0 ;~ None:0142
	// 0 ;~ None:0143
	// 0 ;~ None:0144
	// 0 ;~ None:0145
	// 0 ;~ None:0146
	// 0 ;~ None:0147
	// 0 ;~ None:0148
	// 0 ;~ None:0149
	// 0 ;~ None:014A
	// 0 ;~ None:014B
	// 0 ;~ None:014C
	// 0 ;~ None:014D
	// 0 ;~ None:014E
	// 0 ;~ None:014F
	// 0 ;~ None:0150
	// 0 ; % ;~ None:0151
	// 0 ;~ None:0152
	// 0 ;~ None:0153
	// 0 ;~ None:0154
	// 0 ;~ None:0155
	// 0 ;~ None:0156
	// 0 ; @ ;~ None:0157
	// 0 ;~ None:0158
	// 0 ;~ None:0159
	// 0 ;~ None:015A
	// 0 ;~ None:015B
	// 0 ;~ None:015C
	// 0 ;~ None:015D
	// 0 ;~ None:015E
	// 0 ; @ ;~ None:015F
	// 0 ;~ None:0160
	// 0 ;~ None:0161
	// 0 ;~ None:0162
	// 0 ;~ None:0163
	// 0 ; Z ;~ None:0164
	// 0 ;~ None:0165
	// 0 ;~ None:0166
	// 0 ;~ None:0167
	// 0 ;~ None:0168
	// 0 ;~ None:0169
	// 0 ;~ None:016A
	// 0 ; Z ;~ None:016B
	// 0 ;~ None:016C
	// 0 ;~ None:016D
	// 0 ;~ None:016E
	// 0 ;~ None:016F
	// 0 ;~ None:0170
	// 0 ;~ None:0171
	// 0 ;~ None:0172
	// 0 ;~ None:0173
	// 0 ;~ None:0174
	// 0 ; Z ;~ None:0175
	// 0 ;~ None:0176
	// 0 ;~ None:0177
	// 0 ;~ None:0178
	// 0 ;~ None:0179
	// 0 ; 0 ;~ None:017A
	// 0 ;~ None:017B
	// 0 ;~ None:017C
	// 0 ;~ None:017D
	// 0 ;~ None:017E
	// 0 ;~ None:017F
	// 0 ;~ None:0180
	// 0 ;~ None:0181
	// 0 ;~ None:0182
	// 0 ;~ None:0183
	// 0 ;~ None:0184
	// 0 ;~ None:0185
	// 0 ;~ None:0186
	// 0 ;~ None:0187
	// 0 ;~ None:0188
	// 0 ;~ None:0189
	// 0 ;~ None:018A
	// 0 ;~ None:018B
	// 0 ;~ None:018C
	// 0 ;~ None:018D
	// 0 ;~ None:018E
	// 0 ;~ None:018F
	// 0 ;~ None:0190
	// 0 ;~ None:0191
	// 0 ;~ None:0192
	// 0 ;~ None:0193
	// 0 ;~ None:0194
	// 0 ;~ None:0195
	// 0 ;~ None:0196
	// 0 ;~ None:0197
	// 0 ;~ None:0198
	// 0 ;~ None:0199
	// 0 ;~ None:019A
	// 0 ;~ None:019B
	// 0 ;~ None:019C
	// 0 ;~ None:019D
	// 0 ; i ;~ None:019E
	// 0 ;~ None:019F
	// 0 ;~ None:01A0
	// 0 ;~ None:01A1
	// 0 ;~ None:01A2
	// 0 ;~ None:01A3
	// 0 ;~ None:01A4
	// 0 ;~ None:01A5
	// 0 ;~ None:01A6
	// 0 ;~ None:01A7
	// 0 ;~ None:01A8
	// 0 ;~ None:01A9
	// 0 ;~ None:01AA
	// 0 ;~ None:01AB
	// 0 ; P ;~ None:01AC
	// 0 ;~ None:01AD
	// 0 ;~ None:01AE
	// 0 ; } ;~ None:01AF
	// 0 ;~ None:01B0
	// 0 ; % ;~ None:01B1
	// 0 ;~ None:01B2
	// 0 ;~ None:01B3
	// 0 ; C ;~ None:01B4
	// 0 ;~ None:01B5
	// 0 ;~ None:01B6
	// 0 ;~ None:01B7
	// 0 ;~ None:01B8
	// 0 ;~ None:01B9
	// 0 ;~ None:01BA
	// 0 ;~ None:01BB
	// 0 ;~ None:01BC
	// 0 ;  ;~ None:01BD
	// 0 ;~ None:01BE
	// 0 ;~ None:01BF
	// 0 ;~ None:01C0
	// 0 ;~ None:01C1
	// 0 ;~ None:01C2
	// 0 ;~ None:01C3
	// 0 ;~ None:01C4
	// 0 ;  ;~ None:01C5
	// 0 ;~ None:01C6
	// 0 ;~ None:01C7
	// 0 ;~ None:01C8
	// 0 ;~ None:01C9
	// 0 ;~ None:01CA
	// 0 ;~ None:01CB
	// 0 ;~ None:01CC
	// 0 ;~ None:01CD
	// 0 ;~ None:01CE
	// 0 ;~ None:01CF
	// 0 ;~ None:01D0
	// 0 ;~ None:01D1
	// 0 ;~ None:01D2
	// 0 ;~ None:01D3
	// 0 ;~ None:01D4
	// 0 ;~ None:01D5
	// 0 ;~ None:01D6
	// 0 ;~ None:01D7
	// 0 ; t ;~ None:01D8
	// 0 ;~ None:01D9
	// 0 ;~ None:01DA
	// 0 ;~ None:01DB
	// 0 ;~ None:01DC
	// 0 ;~ None:01DD
	// 0 ;~ None:01DE
	// 0 ;~ None:01DF
	// 0 ;~ None:01E0
	// 0 ;~ None:01E1
	// 0 ;~ None:01E2
	// 0 ;~ None:01E3
	// 0 ; m ;~ None:01E4
	// 0 ;~ None:01E5
	// 0 ;~ None:01E6
	// 0 ;~ None:01E7
	// 0 ; } ;~ None:01E8
	// 0 ;~ None:01E9
	// 0 ;~ None:01EA
	// 0 ;~ None:01EB
	// 0 ;~ None:01EC
	// 0 ;~ None:01ED
	// 0 ;~ None:01EE
	// 0 ;~ None:01EF
	// 0 ;~ None:01F0
	// 0 ;~ None:01F1
	// 0 ;~ None:01F2
	// 0 ;~ None:01F3
	// 0 ; ] ;~ None:01F4
	// 0 ;~ None:01F5
	// 0 ;~ None:01F6
	// 0 ;~ None:01F7
	// 0 ;~ None:01F8
	// 0 ;~ None:01F9
	// 0 ;~ None:01FA
	// 0 ;~ None:01FB
	// 0 ; } ;~ None:01FC
	// 0 ;~ None:01FD
	// 0 ;~ None:01FE
	// 0 ;~ None:01FF
	// 0 ;~ None:0200
	// 0 ;~ None:0201
	// 0 ;~ None:0202
	// 0 ;~ None:0203
	// 0 ;~ None:0204
	// 0 ;~ None:0205
	// 0 ;~ None:0206
	// 0 ;~ None:0207
	// 0 ; ] ;~ None:0208
	// 0 ;~ None:0209
	// 0 ;~ None:020A
	// 0 ;~ None:020B
	// 0 ;~ None:020C
	// 0 ;~ None:020D
	// 0 ;~ None:020E
	// 0 ;~ None:020F
	// 0 ;~ None:0210
	// 0 ;~ None:0211
	// 0 ;~ None:0212
	// 0 ;~ None:0213
	// 0 ;~ None:0214
	// 0 ; - ;~ None:0215
	// 0 ;~ None:0216
	// 0 ;~ None:0217
	// 0 ;~ None:0218
	// 0 ;~ None:0219
	// 0 ;~ None:021A
	// 0 ;~ None:021B
	// 0 ;~ None:021C
	// 0 ;~ None:021D
	// 0 ;~ None:021E
	// 0 ;~ None:021F
	// 0 ;~ None:0220
	// 0 ; } ;~ None:0221
	// 0 ;~ None:0222
	// 0 ;~ None:0223
	// 0 ;~ None:0224
	// 0 ;~ None:0225
	// 0 ;~ None:0226
	// 0 ;~ None:0227
	// 0 ;~ None:0228
	// 0 ; m ;~ None:0229
	// 0 ;~ None:022A
	// 0 ;~ None:022B
	// 0 ;~ None:022C
	// 0 ;~ None:022D
	// 0 ;~ None:022E
	// 0 ;~ None:022F
	// 0 ;~ None:0230
	// 0 ; } ;~ None:0231
	// 0 ;~ None:0232
	// 0 ;~ None:0233
	// 0 ;~ None:0234
	// 0 ;~ None:0235
	// 0 ;~ None:0236
	// 0 ; { ;~ None:0237
	// 0 ;~ None:0238
	// 0 ;~ None:0239
	// 0 ;~ None:023A
	// 0 ; h ;~ None:023B
	// 0 ;~ None:023C
	// 0 ;~ None:023D
	// 0 ;~ None:023E
	// 0 ; { ;~ None:023F
	// 0 ;~ None:0240
	// 0 ;~ None:0241
	// 0 ;~ None:0242
	// 0 ; T ;~ None:0243
	// 0 ;~ None:0244
	// 0 ;~ None:0245
	// 0 ;~ None:0246
	// 0 ;~ None:0247
	// 0 ; e ;~ None:0248
	// 0 ;~ None:0249
	// 0 ;~ None:024A
	// 0 ;~ None:024B
	// 0 ; h ;~ None:024C
	// 0 ;~ None:024D
	// 0 ;~ None:024E
	// 0 ; w ;~ None:024F
	// 0 ; j ;~ None:0250
	// 0 ;~ None:0251
	// 0 ;~ None:0252
	// 0 ;~ None:0253
	// 0 ; l ;~ None:0254
	// 0 ;~ None:0255
	// 0 ;~ None:0256
	// 0 ;~ None:0257
	// 0 ; o ;~ None:0258
	// 0 ;~ None:0259
	// 0 ;~ None:025A
	// 0 ; v ;~ None:025B
	// 0 ; r ;~ None:025C
	// 0 ;~ None:025D
	// 0 ;~ None:025E
	// 0 ; u ;~ None:025F
	// 0 ; u ;~ None:0260
	// 0 ;~ None:0261
	// 0 ;~ None:0262
	// 0 ;~ None:0263
	// 0 ; x ;~ None:0264
	// 0 ;~ None:0265
	// 0 ;~ None:0266
	// 0 ;~ None:0267
	// 0 ; { ;~ None:0268
	// 0 ;~ None:0269
	// 0 ;~ None:026A
	// 0 ;~ None:026B
	// 0 ;~ None:026C
	// 0 ;~ None:026D
	// 0 ;~ None:026E
	// 0 ;~ None:026F
	// 0 ;~ None:0270
	// 0 ;~ None:0271
	// 0 ;~ None:0272
	// 0 ;~ None:0273
	// 0 ;~ None:0274
	// 0 ;~ None:0275
	// 0 ;~ None:0276
	// 0 ;~ None:0277
	// 0 ;~ None:0278
	// 0 ;~ None:0279
	// 0 ;~ None:027A
	// 0 ;~ None:027B
	// 0 ;~ None:027C
	// 0 ;~ None:027D
	// 0 ;~ None:027E
	// 0 ;~ None:027F
	// 0 ;~ None:0280
	// 0 ;~ None:0281
	// 0 ;~ None:0282
	// 0 ;~ None:0283
	// 0 ; n ;~ None:0284
	// 0 ;~ None:0285
	// 0 ;~ None:0286
	// 0 ;~ None:0287
	// 0 ; n ;~ None:0288
	// 0 ;~ None:0289
	// 0 ;~ None:028A
	// 0 ;~ None:028B
	// 0 ;~ None:028C
	// 0 ;~ None:028D
	// 0 ;~ None:028E
	// 0 ;~ None:028F
	// 0 ;~ None:0290
	// 0 ;~ None:0291
	// 0 ;~ None:0292
	// 0 ;~ None:0293
	// 0 ; n ;~ None:0294
	// 0 ;~ None:0295
	// 0 ;~ None:0296
	// 0 ;~ None:0297
	// 0 ;~ None:0298
	// 0 ;~ None:0299
	// 0 ;~ None:029A
	// 0 ;~ None:029B
	// 0 ;~ None:029C
	// 0 ;~ None:029D
	// 0 ;~ None:029E
	// 0 ;~ None:029F
	// 0 ; n ;~ None:02A0
	// 0 ;~ None:02A1
	// 0 ;~ None:02A2
	// 0 ;~ None:02A3
	// 0 ;~ None:02A4
	// 0 ;~ None:02A5
	// 0 ;~ None:02A6
	// 0 ;~ None:02A7
	// 0 ; n ;~ None:02A8
	// 0 ;~ None:02A9
	// 0 ;~ None:02AA
	// 0 ;~ None:02AB
	// 0 ;~ None:02AC
	// 0 ; / ;~ None:02AD
	// 0 ;~ None:02AE
	// 0 ;~ None:02AF
	// 0 ;~ None:02B0
	// 0 ; / ;~ None:02B1
	// 0 ;~ None:02B2
	// 0 ;~ None:02B3
	// 0 ;~ None:02B4
	// 0 ;~ None:02B5
	// 0 ;~ None:02B6
	// 0 ;~ None:02B7
	// 0 ;~ None:02B8
	// 0 ;~ None:02B9
	// 0 ;~ None:02BA
	// 0 ;~ None:02BB
	// 0 ;~ None:02BC
	// 0 ;~ None:02BD
	// 0 ;~ None:02BE
	// 0 ;~ None:02BF
	// 0 ;~ None:02C0
	// 0 ;~ None:02C1
	// 0 ;~ None:02C2
	// 0 ;~ None:02C3
	// 0 ;~ None:02C4
	// 0 ;~ None:02C5
	// 0 ;~ None:02C6
	// 0 ;~ None:02C7
	// 0 ;~ None:02C8
	// 0 ;~ None:02C9
	// 0 ;~ None:02CA
	// 0 ;~ None:02CB
	// 0 ;~ None:02CC
	// 0 ;~ None:02CD
	// 0 ;~ None:02CE
	// 0 ;~ None:02CF
	// 0 ;~ None:02D0
	// 0 ;~ None:02D1
	// 0 ;~ None:02D2
	// 0 ;~ None:02D3
	// 0 ;~ None:02D4
	// 0 ; o ;~ None:02D5
	// 0 ;~ None:02D6
	// 0 ;~ None:02D7
	// 0 ;~ None:02D8
	// 0 ; o ;~ None:02D9
	// 0 ;~ None:02DA
	// 0 ;~ None:02DB
	// 0 ;~ None:02DC
	// 0 ; O ;~ None:02DD
	// 0 ;~ None:02DE
	// 0 ;~ None:02DF
	// 0 ;~ None:02E0
	// 0 ; O ;~ None:02E1
	// 0 ;~ None:02E2
	// 0 ;~ None:02E3
	// 0 ;~ None:02E4
	// 0 ; / ;~ None:02E5
	// 0 ;~ None:02E6
	// 0 ;~ None:02E7
	// 0 ;~ None:02E8
	// 0 ; / ;~ None:02E9
	// 0 ;~ None:02EA
	// 0 ;~ None:02EB
	// 0 ;~ None:02EC
	// 0 ;~ None:02ED
	// 0 ;~ None:02EE
	// 0 ;~ None:02EF
	// 0 ;~ None:02F0
	// 0 ;~ None:02F1
	// 0 ;~ None:02F2
	// 0 ;~ None:02F3
	// 0 ;~ None:02F4
	// 0 ;~ None:02F5
	// 0 ;~ None:02F6
	// 0 ;~ None:02F7
	// 0 ;~ None:02F8
	// 0 ;~ None:02F9
	// 0 ;~ None:02FA
	// 0 ;~ None:02FB
	// 0 ;~ None:02FC
	// 0 ;~ None:02FD
	// 0 ;~ None:02FE
	// 0 ;~ None:02FF
	// 0 ;~ None:0300
	// 0 ;~ None:0301
	// 0 ;~ None:0302
	// 0 ; 8 ;~ None:0303
	// 0 ;~ None:0304
	// 0 ;~ None:0305
	// 0 ;~ None:0306
	// 0 ;~ None:0307
	// 0 ;~ None:0308
	// 0 ;~ None:0309
	// 0 ;~ None:030A
	// 0 ;~ None:030B
	// 0 ;~ None:030C
	// 0 ;~ None:030D
	// 0 ;~ None:030E
	// 0 ;~ None:030F
	// 0 ; DATA XREF: seg001:0018↑r ;~ None:0310
	// 0 ; seg001:0369↓r ;~ None:0310
	// 0 ;~ None:0312
	// 0 ;~ None:0314
	// 0 ;~ None:0316
	// 0 ;~ None:0318
	// 0 ;~ None:031A
	// 0 ;~ None:031C
	// 0 ;~ None:031E
	// 0 ;~ None:0320
	// 0 ;~ None:0322
	// 0 ;~ None:0324
	// 0 ;~ None:0326
	// 0 ;~ None:0328
	// 0 ;~ None:032A
	// 0 ;~ None:032C
	// 0 ;~ None:032E
	// 0 ;~ None:0330
	// 0 ;~ None:0332
	// 0 ;~ None:0334
	// 0 ;~ None:0336
	// 0 ;~ None:0338
	// 0 ;~ None:033A
	// 0 ;~ None:033C
	// 0 ;~ None:033E
	// 0 ;~ None:0340
	// 0 ;~ None:0342
	// 0 ;~ None:0344
	// 0 ;~ None:0346
	// 0 ;~ None:0348
	// 0 ;~ None:034A
	// 0 ;~ None:034C
	// 0 ;~ None:034E
	// 0 ;~ None:0350
	// 0 ;~ None:0352
	// 0 ;~ None:0354
	// 0 ;~ None:0356
	// 0 ;~ None:0358
	// 0 ;~ None:035A
	// 0 ;~ None:035C
	// 0 ;~ None:035E
	// 0 ;~ None:0360
	// 0 ;~ None:0362
	// 0 ; --------------------------------------------------------------------------- ;~ None:0364
edummylabel7:
	// 4383 
	R(CMP(bx, 0x52));	// 887                  cmp     bx, 52h ; 'R' ;~ None:0364
	J(JA(locret_103dd));	// 888                  ja      short locret_103DD ;~ None:0367
	J(CALL(__dispatch_call_ext,*(dw*)(((db*)&off_10380)+bx)));	// 889                  call    off_10380[bx] ;~ None:0369
	// 0 ;~ None:036D
locret_103dd:
	// 4385 
	// 0 ; CODE XREF: seg001:0367↑j ;~ None:036D
	J(RETN(0));	// 892                  retn ;~ None:036D
	// 0 ; --------------------------------------------------------------------------- ;~ None:036D
	// 0 ; DATA XREF: sub_1040B+2F↓w ;~ None:036E
	// 0 ; seg001:0408↓r ... ;~ None:036E
	// 0 ; DATA XREF: seg001:0373↓r ;~ None:0372
	// 0 ; seg001:0394↓w ... ;~ None:0372
	// 0 ; --------------------------------------------------------------------------- ;~ None:0373
edummylabel8:
	// 4387 
	R(CMP(byte_103e2, 0x0FF));	// 899                  cmp     cs:byte_103E2, 0FFh ;~ None:0373
	J(JZ(locret_1040a));	// 900                  jz      short locret_1040A ;~ None:0379
	R(PUSH(ax));	// 901                  push    ax ;~ None:037B
	R(PUSH(ds));	// 902                  push    ds ;~ None:037C
	R(PUSH(cs));	// 903                  push    cs ;~ None:037D
	R(POP(ds));	// 904                  pop     ds ;~ None:037E
	R(PUSH(di));	// 905                  push    di ;~ None:037F
	R(PUSH(cx));	// 906                  push    cx ;~ None:0380
	R(PUSH(bx));	// 907                  push    bx ;~ None:0381
	J(CALL(sub_104c5,0));	// 908                  call    sub_104C5 ;~ None:0382
	R(POP(bx));	// 909                  pop     bx ;~ None:0385
	R(POP(cx));	// 910                  pop     cx ;~ None:0386
	R(POP(di));	// 911                  pop     di ;~ None:0387
	R(DEC(byte_100d9));	// 912                  dec     byte_100D9 ;~ None:0388
	J(JZ(loc_1046b));	// 913                  jz      short loc_1046B ;~ None:038C
	R(POP(ds));	// 914                  pop     ds ;~ None:038E
	// 0 ;~ None:038F
	R(al = 0x20;);	// 916                  mov     al, 20h ; ' ' ;~ None:038F
	R(OUT(0x20, al));	// 917                  out     20h, al         ; Interrupt controller, 8259A. ;~ None:0391
	R(POP(ax));	// 918                  pop     ax ;~ None:0393
	R(byte_103e2 = 0;);	// 919                  mov     cs:byte_103E2, 0 ;~ None:0394
	// 0 ;~ None:039A
locret_1040a:
	// 4389 
	// 0 ; CODE XREF: seg001:0379↑j ;~ None:039A
	J(IRET);	// 922                  iret ;~ None:039A
	// 0 ;~ None:039B
	// 0 ; =============== S U B R O U T I N E ======================================= ;~ None:039B
	// 0 ;~ None:039B
	// 0 ;~ None:039B
sub_1040b:
	// 927 
	R(MOV(*(raddr(ds,0x67)), 0));	// 928                  mov     byte ptr ds:67h, 0 ;~ None:039B
edummylabel9:
	// 4391 
	R(MOV(*(raddr(ds,0x65)), 0));	// 929                  mov     byte ptr ds:65h, 0 ;~ None:03A0
	R(MOV(*(dw*)(raddr(ds,0x63)), 0));	// 930                  mov     word ptr ds:63h, 0 ;~ None:03A5
	R(byte_103e2 = 0;);	// 931                  mov     cs:byte_103E2, 0 ;~ None:03AB
	R(CLI);	// 932                  cli ;~ None:03B1
	R(CMP(*(raddr(ds,0x68)), 0));	// 933                  cmp     byte ptr ds:68h, 0 ;~ None:03B2
	J(JNZ(locret_1046a));	// 934                  jnz     short locret_1046A ;~ None:03B7
	R(NOT(*(raddr(ds,0x68))));	// 935                  not     byte ptr ds:68h ;~ None:03B9
	R(PUSH(ds));	// 936                  push    ds ;~ None:03BD
	R(SUB(ax, ax));	// 937                  sub     ax, ax ;~ None:03BE
	R(m2c::set_segment_register(ds, ax););	// 938                  mov     ds, ax ;~ None:03C0
	// 0 ;~ None:03C2
	R(MOV(bx, *(dw*)(raddr(ds,0x20))));	// 940                  mov     bx, ds:20h ;~ None:03C2
	R(MOV(cx, *(dw*)(raddr(ds,0x22))));	// 941                  mov     cx, ds:22h ;~ None:03C6
	R(*(dw*)(&dword_103de) = bx;);	// 942                  mov     word ptr cs:dword_103DE, bx ;~ None:03CA
	R(*(dw*)(((db*)&dword_103de)+2) = cx;);	// 943                  mov     word ptr cs:dword_103DE+2, cx ;~ None:03CF
	R(POP(ds));	// 944                  pop     ds ;~ None:03D4
	// 0 ;~ None:03D5
	R(CLI);	// 946                  cli ;~ None:03D5
	R(MOV(*(raddr(ds,0x69)), 3));	// 947                  mov     byte ptr ds:69h, 3 ;~ None:03D6
	R(PUSH(ds));	// 948                  push    ds ;~ None:03DB
	R(SUB(ax, ax));	// 949                  sub     ax, ax ;~ None:03DC
	R(m2c::set_segment_register(ds, ax););	// 950                  mov     ds, ax ;~ None:03DE
	// 0 ;~ None:03E0
	R(ax = 0x373);	// 952                  lea     ax, ds:373h ;~ None:03E0
	R(MOV(*(dw*)(raddr(ds,0x20)), ax));	// 953                  mov     ds:20h, ax ;~ None:03E4
	R(MOV(*(dw*)(raddr(ds,0x22)), cs));	// 954                  mov     word ptr ds:22h, cs ;~ None:03E7
	R(POP(ds));	// 955                  pop     ds ;~ None:03EB
	// 0 ;~ None:03EC
	R(al = 0x36;);	// 957                  mov     al, 36h ; '6' ;~ None:03EC
	R(OUT(0x43, al));	// 958                  out     43h, al         ; Timer 8253-5 (AT: 8254.2). ;~ None:03EE
	R(ax = 0x4DAE;);	// 959                  mov     ax, 4DAEh ;~ None:03F0
	R(OUT(0x40, al));	// 960                  out     40h, al         ; Timer 8253-5 (AT: 8254.2). ;~ None:03F3
	R(al = ah;);	// 961                  mov     al, ah ;~ None:03F5
	R(OUT(0x40, al));	// 962                  out     40h, al         ; Timer 8253-5 (AT: 8254.2). ;~ None:03F7
	R(STI);	// 963                  sti ;~ None:03F9
	// 0 ;~ None:03FA
locret_1046a:
	// 4393 
	// 0 ; CODE XREF: sub_1040B+1C↑j ;~ None:03FA
	J(RETN(0));	// 966                  retn ;~ None:03FA
seg001_33e_proc:
	// 971 
loc_1046b:
	// 4395 
	// 0 ; CODE XREF: seg001:038C↑j ;~ None:03FB
	R(MOV(*(raddr(ds,0x69)), 3));	// 972                  mov     byte ptr ds:69h, 3 ;~ None:03FB
	R(byte_103e2 = 0;);	// 973                  mov     cs:byte_103E2, 0 ;~ None:0400
	R(POP(ds));	// 974                  pop     ds ;~ None:0406
	R(POP(ax));	// 975                  pop     ax ;~ None:0407
__disp=dword_103de;
	J(return __dispatch_call(__disp, _state););	// 976                  jmp     cs:dword_103DE ;~ None:0408
	// 0 ; --------------------------------------------------------------------------- ;~ None:040D
edummylabel10:
	// 4397 
	R(CLI);	// 978                  cli ;~ None:040D
	R(PUSH(ds));	// 979                  push    ds ;~ None:040E
	R(bx = *(dw*)(&dword_103de););	// 980                  mov     bx, word ptr cs:dword_103DE ;~ None:040F
	R(cx = *(dw*)(((db*)&dword_103de)+2););	// 981                  mov     cx, word ptr cs:dword_103DE+2 ;~ None:0414
	R(SUB(ax, ax));	// 982                  sub     ax, ax ;~ None:0419
	R(m2c::set_segment_register(ds, ax););	// 983                  mov     ds, ax ;~ None:041B
	// 0 ;~ None:041D
	R(MOV(*(dw*)(raddr(ds,0x20)), bx));	// 985                  mov     ds:20h, bx ;~ None:041D
	R(MOV(*(dw*)(raddr(ds,0x22)), cx));	// 986                  mov     ds:22h, cx ;~ None:0421
	R(POP(ds));	// 987                  pop     ds ;~ None:0425
	// 0 ;~ None:0426
	R(al = 0x36;);	// 989                  mov     al, 36h ; '6' ;~ None:0426
	R(OUT(0x43, al));	// 990                  out     43h, al         ; Timer 8253-5 (AT: 8254.2). ;~ None:0428
	R(XOR(ax, ax));	// 991                  xor     ax, ax ;~ None:042A
	R(OUT(0x40, al));	// 992                  out     40h, al         ; Timer 8253-5 (AT: 8254.2). ;~ None:042C
	R(al = ah;);	// 993                  mov     al, ah ;~ None:042E
	R(OUT(0x40, al));	// 994                  out     40h, al         ; Timer 8253-5 (AT: 8254.2). ;~ None:0430
	R(STI);	// 995                  sti ;~ None:0432
	J(RETN(0));	// 996                  retn ;~ None:0433
	// 0 ;~ None:0434
	// 0 ; =============== S U B R O U T I N E ======================================= ;~ None:0434
	// 0 ;~ None:0434
	// 0 ;~ None:0434
sub_104a4:
	// 1001 
	R(al = 0x9F;);	// 1002                  mov     al, 9Fh ;~ None:0434
edummylabel11:
	// 4399 
	R(OUT(0x0C0, al));	// 1003                  out     0C0h, al        ; DMA controller, 8237A-5. ;~ None:0436
	// 0 ; channel 0 base address ;~ None:0436
	// 0 ; (also sets current address) ;~ None:0436
	R(al = 0x0BF;);	// 1006                  mov     al, 0BFh ;~ None:0438
	R(OUT(0x0C0, al));	// 1007                  out     0C0h, al        ; DMA controller, 8237A-5. ;~ None:043A
	// 0 ; channel 0 base address ;~ None:043A
	// 0 ; (also sets current address) ;~ None:043A
	R(al = 0x0DF;);	// 1010                  mov     al, 0DFh ;~ None:043C
	R(OUT(0x0C0, al));	// 1011                  out     0C0h, al        ; DMA controller, 8237A-5. ;~ None:043E
	// 0 ; channel 0 base address ;~ None:043E
	// 0 ; (also sets current address) ;~ None:043E
	R(al = 0x0FF;);	// 1014                  mov     al, 0FFh ;~ None:0440
	R(OUT(0x0C0, al));	// 1015                  out     0C0h, al        ; DMA controller, 8237A-5. ;~ None:0442
	// 0 ; channel 0 base address ;~ None:0442
	// 0 ; (also sets current address) ;~ None:0442
	R(cx = 0x38;);	// 1018                  mov     cx, 38h ; '8' ;~ None:0444
	R(bx = offset(seg001,unk_1009f));	// 1019                  lea     bx, unk_1009F ;~ None:0447
	// 0 ;~ None:044B
loc_104bb:
	// 4401 
	// 0 ; CODE XREF: sub_1059B+12↓j ;~ None:044B
	R(XOR(ax, ax));	// 1022                  xor     ax, ax ;~ None:044B
	R(ADD(bx, cx));	// 1023                  add     bx, cx ;~ None:044D
	// 0 ;~ None:044F
loc_104bf:
	// 4403 
	// 0 ; CODE XREF: sub_104A4+1E↓j ;~ None:044F
	R(DEC(bx));	// 1026                  dec     bx ;~ None:044F
	R(MOV(*(raddr(ds,bx)), al));	// 1027                  mov     [bx], al ;~ None:0450
	J(LOOP(loc_104bf));	// 1028                  loop    loc_104BF ;~ None:0452
	// 0 ;~ None:0454
locret_104c4:
	// 4405 
	// 0 ; CODE XREF: seg001:0018↑p ;~ None:0454
	// 0 ; seg001:0369↑p ... ;~ None:0454
	J(RETN(0));	// 1032                  retn ;~ None:0454
sub_104c5:
	// 1039 
	R(CMP(*(raddr(ds,0x69)), 1));	// 1040                  cmp     byte ptr ds:69h, 1 ;~ None:0455
edummylabel12:
	// 4407 
	J(JZ(loc_10506));	// 1041                  jz      short loc_10506 ;~ None:045A
	R(CMP(*(dw*)(raddr(ds,0x63)), 0));	// 1042                  cmp     word ptr ds:63h, 0 ;~ None:045C
	J(JZ(loc_104e1));	// 1043                  jz      short loc_104E1 ;~ None:0461
	R(CMP(*(raddr(ds,0x49)), 0));	// 1044                  cmp     byte ptr ds:49h, 0 ;~ None:0463
	J(JNZ(loc_104e1));	// 1045                  jnz     short loc_104E1 ;~ None:0468
	R(MOV(bx, *(dw*)(raddr(ds,0x63))));	// 1046                  mov     bx, ds:63h ;~ None:046A
	J(JMP(sub_105b0));	// 1047                  jmp     sub_105B0 ;~ None:046E
	// 0 ; --------------------------------------------------------------------------- ;~ None:0471
	// 0 ;~ None:0471
loc_104e1:
	// 4409 
	// 0 ; CODE XREF: sub_104C5+C↑j ;~ None:0471
	// 0 ; sub_104C5+13↑j ;~ None:0471
	R(CMP(*(raddr(ds,0x65)), 0));	// 1052                  cmp     byte ptr ds:65h, 0 ;~ None:0471
	J(JZ(loc_10506));	// 1053                  jz      short loc_10506 ;~ None:0476
	R(CMP(*(raddr(ds,0x66)), 0));	// 1054                  cmp     byte ptr ds:66h, 0 ;~ None:0478
	J(JG(loc_10502));	// 1055                  jg      short loc_10502 ;~ None:047D
	R(MOV(*(raddr(ds,0x66)), 0x1E));	// 1056                  mov     byte ptr ds:66h, 1Eh ;~ None:047F
	R(CMP(*(raddr(ds,0x2F)), 0));	// 1057                  cmp     byte ptr ds:2Fh, 0 ;~ None:0484
	J(JNZ(loc_10502));	// 1058                  jnz     short loc_10502 ;~ None:0489
	R(bx = 0x1B6);	// 1059                  lea     bx, ds:1B6h ;~ None:048B
	J(JMP(sub_105b0));	// 1060                  jmp     sub_105B0 ;~ None:048F
	// 0 ; --------------------------------------------------------------------------- ;~ None:0492
	// 0 ;~ None:0492
loc_10502:
	// 4411 
	// 0 ; CODE XREF: sub_104C5+28↑j ;~ None:0492
	// 0 ; sub_104C5+34↑j ;~ None:0492
	R(DEC(*(raddr(ds,0x66))));	// 1065                  dec     byte ptr ds:66h ;~ None:0492
	// 0 ;~ None:0496
loc_10506:
	// 4413 
	// 0 ; CODE XREF: sub_104C5+5↑j ;~ None:0496
	// 0 ; sub_104C5+21↑j ;~ None:0496
	R(di = 0x2F);	// 1069                  lea     di, ds:2Fh ;~ None:0496
	J(CALL(sub_1051f,0));	// 1070                  call    sub_1051F ;~ None:049A
	R(di = 0x3C);	// 1071                  lea     di, ds:3Ch ;~ None:049D
	J(CALL(sub_1051f,0));	// 1072                  call    sub_1051F ;~ None:04A1
	R(di = 0x49);	// 1073                  lea     di, ds:49h ;~ None:04A4
	J(CALL(sub_1051f,0));	// 1074                  call    sub_1051F ;~ None:04A8
	R(di = 0x56);	// 1075                  lea     di, ds:56h ;~ None:04AB
sub_1051f:
	// 1082 
	// 0 ; sub_104C5+4C↑p ... ;~ None:04AF
	R(CMP(*(raddr(ds,di)), 0));	// 1084                  cmp     byte ptr [di], 0 ;~ None:04AF
	J(JZ(locret_104c4));	// 1085                  jz      short locret_104C4 ;~ None:04B2
	R(DEC(*(raddr(ds,di))));	// 1086                  dec     byte ptr [di] ;~ None:04B4
	J(JZ(loc_10592));	// 1087                  jz      short loc_10592 ;~ None:04B6
	R(CMP(*(dw*)(raddr(ds,di+2)), 0));	// 1088                  cmp     word ptr [di+2], 0 ;~ None:04B8
	J(JZ(locret_104c4));	// 1089                  jz      short locret_104C4 ;~ None:04BC
	R(MOV(al, *(raddr(ds,di+7))));	// 1090                  mov     al, [di+7] ;~ None:04BE
	R(OR(al, al));	// 1091                  or      al, al ;~ None:04C1
	J(JZ(loc_1058c));	// 1092                  jz      short loc_1058C ;~ None:04C3
	R(bl = al;);	// 1093                  mov     bl, al ;~ None:04C5
	R(AND(al, 0x0F0));	// 1094                  and     al, 0F0h ;~ None:04C7
	J(JZ(loc_10566));	// 1095                  jz      short loc_10566 ;~ None:04C9
	R(DEC(*(raddr(ds,di+4))));	// 1096                  dec     byte ptr [di+4] ;~ None:04CB
	J(JNZ(loc_10566));	// 1097                  jnz     short loc_10566 ;~ None:04CE
	R(MOV(al, *(raddr(ds,di+6))));	// 1098                  mov     al, [di+6] ;~ None:04D0
	R(AND(al, 0x0F0));	// 1099                  and     al, 0F0h ;~ None:04D3
	R(CBW);	// 1100                  cbw ;~ None:04D5
	R(MOV(cx, *(dw*)(raddr(ds,di+2))));	// 1101                  mov     cx, [di+2] ;~ None:04D6
	R(ADD(ax, cx));	// 1102                  add     ax, cx ;~ None:04D9
	R(AND(ax, 0x3FF0));	// 1103                  and     ax, 3FF0h ;~ None:04DB
	R(AND(cx, 0x0C00F));	// 1104                  and     cx, 0C00Fh ;~ None:04DE
	R(OR(ax, cx));	// 1105                  or      ax, cx ;~ None:04E2
	R(MOV(*(dw*)(raddr(ds,di+2)), ax));	// 1106                  mov     [di+2], ax ;~ None:04E4
	R(al = bl;);	// 1107                  mov     al, bl ;~ None:04E7
	R(AND(al, 0x0F0));	// 1108                  and     al, 0F0h ;~ None:04E9
	R(SHR(al, 1));	// 1109                  shr     al, 1 ;~ None:04EB
	R(SHR(al, 1));	// 1110                  shr     al, 1 ;~ None:04ED
	R(SHR(al, 1));	// 1111                  shr     al, 1 ;~ None:04EF
	R(SHR(al, 1));	// 1112                  shr     al, 1 ;~ None:04F1
	R(MOV(*(raddr(ds,di+4)), al));	// 1113                  mov     [di+4], al ;~ None:04F3
	// 0 ;~ None:04F6
loc_10566:
	// 4415 
	// 0 ; CODE XREF: sub_1051F+1A↑j ;~ None:04F6
	// 0 ; sub_1051F+1F↑j ;~ None:04F6
	R(al = bl;);	// 1117                  mov     al, bl ;~ None:04F6
	R(AND(al, 0x0F));	// 1118                  and     al, 0Fh ;~ None:04F8
	J(JZ(loc_1058c));	// 1119                  jz      short loc_1058C ;~ None:04FA
	R(DEC(*(raddr(ds,di+5))));	// 1120                  dec     byte ptr [di+5] ;~ None:04FC
	J(JNZ(loc_1058c));	// 1121                  jnz     short loc_1058C ;~ None:04FF
	R(MOV(al, *(raddr(ds,di+6))));	// 1122                  mov     al, [di+6] ;~ None:0501
	R(AND(al, 0x0F));	// 1123                  and     al, 0Fh ;~ None:0504
	R(MOV(cx, *(dw*)(raddr(ds,di+2))));	// 1124                  mov     cx, [di+2] ;~ None:0506
	R(ADD(ax, cx));	// 1125                  add     ax, cx ;~ None:0509
	R(AND(ax, 0x0F));	// 1126                  and     ax, 0Fh ;~ None:050B
	R(AND(cx, 0x0FFF0));	// 1127                  and     cx, 0FFF0h ;~ None:050E
	R(OR(ax, cx));	// 1128                  or      ax, cx ;~ None:0511
	R(MOV(*(dw*)(raddr(ds,di+2)), ax));	// 1129                  mov     [di+2], ax ;~ None:0513
	R(AND(bl, 0x0F));	// 1130                  and     bl, 0Fh ;~ None:0516
	R(MOV(*(raddr(ds,di+5)), bl));	// 1131                  mov     [di+5], bl ;~ None:0519
	// 0 ;~ None:051C
loc_1058c:
	// 4417 
	// 0 ; CODE XREF: sub_1051F+14↑j ;~ None:051C
	// 0 ; sub_1051F+4B↑j ... ;~ None:051C
	R(MOV(bx, *(dw*)(raddr(ds,di+2))));	// 1135                  mov     bx, [di+2] ;~ None:051C
	J(JMP(sub_10889));	// 1136                  jmp     sub_10889 ;~ None:051F
	// 0 ; --------------------------------------------------------------------------- ;~ None:0522
	// 0 ;~ None:0522
loc_10592:
	// 4419 
	// 0 ; CODE XREF: sub_1051F+7↑j ;~ None:0522
	R(MOV(bx, *(dw*)(raddr(ds,di+0x0B))));	// 1140                  mov     bx, [di+0Bh] ;~ None:0522
	R(MOV(al, *(raddr(ds,bx))));	// 1141                  mov     al, [bx] ;~ None:0525
	R(OR(al, al));	// 1142                  or      al, al ;~ None:0527
	J(JNZ(loc_105d2));	// 1143                  jnz     short loc_105D2 ;~ None:0529
sub_1059b:
	// 1150 
	// 0 ; sub_105C4+22↓p ;~ None:052B
	R(MOV(bx, *(dw*)(raddr(ds,di+2))));	// 1152                  mov     bx, [di+2] ;~ None:052B
	R(OR(bx, bx));	// 1153                  or      bx, bx ;~ None:052E
	J(JZ(loc_105a8));	// 1154                  jz      short loc_105A8 ;~ None:0530
	R(OR(bl, 0x0F));	// 1155                  or      bl, 0Fh ;~ None:0532
	J(CALL(sub_10889,0));	// 1156                  call    sub_10889 ;~ None:0535
	// 0 ;~ None:0538
loc_105a8:
	// 4421 
	// 0 ; CODE XREF: sub_1059B+5↑j ;~ None:0538
	R(bx = di;);	// 1159                  mov     bx, di ;~ None:0538
	R(cx = 0x0D;);	// 1160                  mov     cx, 0Dh ;~ None:053A
	J(JMP(loc_104bb));	// 1161                  jmp     loc_104BB ;~ None:053D
sub_105b0:
	// 1168 
	// 0 ; sub_104C5+3A↑j ... ;~ None:0540
edummylabel13:
	// 4423 
	R(MOV(al, *(raddr(ds,bx))));	// 1170                  mov     al, [bx] ;~ None:0540
	R(OR(al, al));	// 1171                  or      al, al ;~ None:0542
	J(JZ(locret_10604));	// 1172                  jz      short locret_10604 ;~ None:0544
	R(AND(al, 3));	// 1173                  and     al, 3 ;~ None:0546
	R(CBW);	// 1174                  cbw ;~ None:0548
	R(cx = 0x0D;);	// 1175                  mov     cx, 0Dh ;~ None:0549
	R(MUL1_2(cx));	// 1176                  mul     cx ;~ None:054C
	R(cx = 0x2F);	// 1177                  lea     cx, ds:2Fh ;~ None:054E
	R(ADD(ax, cx));	// 1178                  add     ax, cx ;~ None:0552
sub_105c4:
	// 1185 
	R(di = ax;);	// 1186                  mov     di, ax ;~ None:0554
	R(PUSH(bx));	// 1187                  push    bx ;~ None:0556
	R(PUSH(*(dw*)(raddr(ds,di))));	// 1188                  push    word ptr [di] ;~ None:0557
	J(CALL(sub_1059b,0));	// 1189                  call    sub_1059B ;~ None:0559
	R(POP(*(dw*)(raddr(ds,di))));	// 1190                  pop     word ptr [di] ;~ None:055C
	R(POP(bx));	// 1191                  pop     bx ;~ None:055E
	R(MOV(*(dw*)(raddr(ds,di+9)), bx));	// 1192                  mov     [di+9], bx ;~ None:055F
	// 0 ;~ None:0562
loc_105d2:
	// 4425 
	// 0 ; CODE XREF: sub_1051F+7A↑j ;~ None:0562
	R(MOV(al, *(raddr(ds,bx))));	// 1195                  mov     al, [bx] ;~ None:0562
	R(INC(bx));	// 1196                  inc     bx ;~ None:0564
	R(AND(ax, 0x7F));	// 1197                  and     ax, 7Fh ;~ None:0565
	R(TEST(al, 0x88));	// 1198                  test    al, 88h ;~ None:0568
	J(JZ(loc_10605));	// 1199                  jz      short loc_10605 ;~ None:056A
	R(PUSH(bx));	// 1200                  push    bx ;~ None:056C
	R(PUSH(ax));	// 1201                  push    ax ;~ None:056D
	R(MOV(ax, *(dw*)(raddr(ds,di+9))));	// 1202                  mov     ax, [di+9] ;~ None:056E
	R(PUSH(ax));	// 1203                  push    ax ;~ None:0571
	R(MOV(al, *(raddr(ds,di+8))));	// 1204                  mov     al, [di+8] ;~ None:0572
	R(PUSH(ax));	// 1205                  push    ax ;~ None:0575
	J(CALL(sub_1059b,0));	// 1206                  call    sub_1059B ;~ None:0576
	R(POP(ax));	// 1207                  pop     ax ;~ None:0579
	R(MOV(*(raddr(ds,di+8)), al));	// 1208                  mov     [di+8], al ;~ None:057A
	R(POP(ax));	// 1209                  pop     ax ;~ None:057D
	R(MOV(*(dw*)(raddr(ds,di+9)), ax));	// 1210                  mov     [di+9], ax ;~ None:057E
	R(POP(ax));	// 1211                  pop     ax ;~ None:0581
	R(POP(bx));	// 1212                  pop     bx ;~ None:0582
	J(JMP(loc_10630));	// 1213                  jmp     short loc_10630 ;~ None:0583
	// 0 ; --------------------------------------------------------------------------- ;~ None:0583
	// 0 ;~ None:0585
	// 0 ; --------------------------------------------------------------------------- ;~ None:0586
	// 0 ;~ None:0586
loc_105f6:
	// 4427 
	// 0 ; CODE XREF: sub_105C4+6E↓j ;~ None:0586
	R(TEST(al, 0x84));	// 1219                  test    al, 84h ;~ None:0586
	J(JNZ(loc_10642));	// 1220                  jnz     short loc_10642 ;~ None:0588
	// 0 ;~ None:058A
loc_105fa:
	// 4429 
	// 0 ; CODE XREF: sub_105C4+84↓j ;~ None:058A
	R(cx = bx;);	// 1223                  mov     cx, bx ;~ None:058A
	R(INC(cx));	// 1224                  inc     cx ;~ None:058C
	// 0 ;~ None:058D
loc_105fd:
	// 4431 
	// 0 ; CODE XREF: sub_105C4+8C↓j ;~ None:058D
	R(MOV(*(dw*)(raddr(ds,di+0x0B)), cx));	// 1227                  mov     [di+0Bh], cx ;~ None:058D
	R(MOV(cl, *(raddr(ds,bx))));	// 1228                  mov     cl, [bx] ;~ None:0590
	R(MOV(*(raddr(ds,di)), cl));	// 1229                  mov     [di], cl ;~ None:0592
	// 0 ;~ None:0594
locret_10604:
	// 4433 
	// 0 ; CODE XREF: sub_105B0+4↑j ;~ None:0594
	J(RETN(0));	// 1232                  retn ;~ None:0594
	// 0 ; --------------------------------------------------------------------------- ;~ None:0595
	// 0 ;~ None:0595
loc_10605:
	// 4435 
	// 0 ; CODE XREF: sub_105C4+16↑j ;~ None:0595
	R(MOV(cx, *(dw*)(raddr(ds,bx))));	// 1236                  mov     cx, [bx] ;~ None:0595
	R(INC(bx));	// 1237                  inc     bx ;~ None:0597
	R(INC(bx));	// 1238                  inc     bx ;~ None:0598
	R(MOV(*(dw*)(raddr(ds,di+2)), cx));	// 1239                  mov     [di+2], cx ;~ None:0599
	R(TEST(al, 0x0B0));	// 1240                  test    al, 0B0h ;~ None:059C
	J(JNZ(loc_10615));	// 1241                  jnz     short loc_10615 ;~ None:059E
	R(XOR(cx, cx));	// 1242                  xor     cx, cx ;~ None:05A0
	J(JMP(loc_1062d));	// 1243                  jmp     short loc_1062D ;~ None:05A2
	// 0 ; --------------------------------------------------------------------------- ;~ None:05A2
	// 0 ;~ None:05A4
	// 0 ; --------------------------------------------------------------------------- ;~ None:05A5
	// 0 ;~ None:05A5
loc_10615:
	// 4437 
	// 0 ; CODE XREF: sub_105C4+4A↑j ;~ None:05A5
	R(TEST(al, 0x0A0));	// 1249                  test    al, 0A0h ;~ None:05A5
	J(JZ(loc_1061f));	// 1250                  jz      short loc_1061F ;~ None:05A7
	R(MOV(cl, *(raddr(ds,bx))));	// 1251                  mov     cl, [bx] ;~ None:05A9
	R(INC(bx));	// 1252                  inc     bx ;~ None:05AB
	R(MOV(*(raddr(ds,di+4)), cl));	// 1253                  mov     [di+4], cl ;~ None:05AC
	// 0 ;~ None:05AF
loc_1061f:
	// 4439 
	// 0 ; CODE XREF: sub_105C4+53↑j ;~ None:05AF
	R(TEST(al, 0x90));	// 1256                  test    al, 90h ;~ None:05AF
	J(JZ(loc_10629));	// 1257                  jz      short loc_10629 ;~ None:05B1
	R(MOV(cl, *(raddr(ds,bx))));	// 1258                  mov     cl, [bx] ;~ None:05B3
	R(INC(bx));	// 1259                  inc     bx ;~ None:05B5
	R(MOV(*(raddr(ds,di+5)), cl));	// 1260                  mov     [di+5], cl ;~ None:05B6
	// 0 ;~ None:05B9
loc_10629:
	// 4441 
	// 0 ; CODE XREF: sub_105C4+5D↑j ;~ None:05B9
	R(MOV(cx, *(dw*)(raddr(ds,bx))));	// 1263                  mov     cx, [bx] ;~ None:05B9
	R(INC(bx));	// 1264                  inc     bx ;~ None:05BB
	R(INC(bx));	// 1265                  inc     bx ;~ None:05BC
	// 0 ;~ None:05BD
loc_1062d:
	// 4443 
	// 0 ; CODE XREF: sub_105C4+4E↑j ;~ None:05BD
	R(MOV(*(dw*)(raddr(ds,di+6)), cx));	// 1268                  mov     [di+6], cx ;~ None:05BD
	// 0 ;~ None:05C0
loc_10630:
	// 4445 
	// 0 ; CODE XREF: sub_105C4+2F↑j ;~ None:05C0
	R(TEST(al, 0x0C0));	// 1271                  test    al, 0C0h ;~ None:05C0
	J(JZ(loc_105f6));	// 1272                  jz      short loc_105F6 ;~ None:05C2
	R(MOV(cl, *(raddr(ds,bx))));	// 1273                  mov     cl, [bx] ;~ None:05C4
	R(INC(bx));	// 1274                  inc     bx ;~ None:05C6
	R(CMP(*(raddr(ds,di+8)), 0));	// 1275                  cmp     byte ptr [di+8], 0 ;~ None:05C7
	J(JZ(loc_1064a));	// 1276                  jz      short loc_1064A ;~ None:05CB
	R(DEC(*(raddr(ds,di+8))));	// 1277                  dec     byte ptr [di+8] ;~ None:05CD
	J(JNZ(loc_1064d));	// 1278                  jnz     short loc_1064D ;~ None:05D0
	// 0 ;~ None:05D2
loc_10642:
	// 4447 
	// 0 ; CODE XREF: sub_105C4+34↑j ;~ None:05D2
	R(cx = bx;);	// 1281                  mov     cx, bx ;~ None:05D2
	R(INC(cx));	// 1282                  inc     cx ;~ None:05D4
	R(MOV(*(dw*)(raddr(ds,di+9)), cx));	// 1283                  mov     [di+9], cx ;~ None:05D5
	J(JMP(loc_105fa));	// 1284                  jmp     short loc_105FA ;~ None:05D8
	// 0 ; --------------------------------------------------------------------------- ;~ None:05DA
	// 0 ;~ None:05DA
loc_1064a:
	// 4449 
	// 0 ; CODE XREF: sub_105C4+77↑j ;~ None:05DA
	R(MOV(*(raddr(ds,di+8)), cl));	// 1288                  mov     [di+8], cl ;~ None:05DA
	// 0 ;~ None:05DD
loc_1064d:
	// 4451 
	// 0 ; CODE XREF: sub_105C4+7C↑j ;~ None:05DD
	R(MOV(cx, *(dw*)(raddr(ds,di+9))));	// 1291                  mov     cx, [di+9] ;~ None:05DD
	J(JMP(loc_105fd));	// 1292                  jmp     short loc_105FD ;~ None:05E0
sub_10652:
	// 1299 
	// 0 ; seg001:0369↑p ;~ None:05E2
	// 0 ; DATA XREF: ... ;~ None:05E2
edummylabel14:
	// 4453 
	R(al = 0x32;);	// 1302                  mov     al, 32h ; '2' ;~ None:05E2
	R(CMP(al, *(raddr(ds,0x30))));	// 1303                  cmp     al, ds:30h ;~ None:05E4
	J(JC(locret_1069d));	// 1304                  jb      short locret_1069D ;~ None:05E8
	R(bx = 0x0F0);	// 1305                  lea     bx, ds:0F0h ;~ None:05EA
	J(JMP(sub_107b2));	// 1306                  jmp     sub_107B2 ;~ None:05EE
sub_10661:
	// 1313 
	// 0 ; seg001:0369↑p ;~ None:05F1
	// 0 ; DATA XREF: ... ;~ None:05F1
edummylabel15:
	// 4455 
	R(al = 0x1E;);	// 1316                  mov     al, 1Eh ;~ None:05F1
	R(CMP(al, *(raddr(ds,0x30))));	// 1317                  cmp     al, ds:30h ;~ None:05F3
	J(JC(locret_1069d));	// 1318                  jb      short locret_1069D ;~ None:05F7
	R(bx = 0x108);	// 1319                  lea     bx, ds:108h ;~ None:05F9
	J(CALL(sub_107b2,0));	// 1320                  call    sub_107B2 ;~ None:05FD
	R(bx = 0x100);	// 1321                  lea     bx, ds:100h ;~ None:0600
	J(JMP(sub_107b5));	// 1322                  jmp     sub_107B5 ;~ None:0604
sub_10677:
	// 1329 
	// 0 ; seg001:0369↑p ;~ None:0607
	// 0 ; DATA XREF: ... ;~ None:0607
edummylabel16:
	// 4457 
	R(bx = 0x9D);	// 1332                  lea     bx, ds:9Dh ;~ None:0607
	J(JMP(sub_107b5));	// 1333                  jmp     sub_107B5 ;~ None:060B
sub_1067e:
	// 1340 
	// 0 ; seg001:0369↑p ;~ None:060E
	// 0 ; DATA XREF: ... ;~ None:060E
edummylabel17:
	// 4459 
	R(al = 0x64;);	// 1343                  mov     al, 64h ; 'd' ;~ None:060E
	R(CMP(al, *(raddr(ds,0x30))));	// 1344                  cmp     al, ds:30h ;~ None:0610
	R(bx = 0x198);	// 1345                  lea     bx, ds:198h ;~ None:0614
	J(JC(loc_10694));	// 1346                  jb      short loc_10694 ;~ None:0618
	J(CALL(sub_107b2,0));	// 1347                  call    sub_107B2 ;~ None:061A
	R(bx = 0x1A7);	// 1348                  lea     bx, ds:1A7h ;~ None:061D
	J(JMP(sub_107b5));	// 1349                  jmp     sub_107B5 ;~ None:0621
	// 0 ; --------------------------------------------------------------------------- ;~ None:0624
	// 0 ;~ None:0624
loc_10694:
	// 4461 
	// 0 ; CODE XREF: sub_1067E+A↑j ;~ None:0624
	R(CLI);	// 1353                  cli ;~ None:0624
	R(ax = 0x3C);	// 1354                  lea     ax, ds:3Ch ;~ None:0625
	J(CALL(sub_105c4,0));	// 1355                  call    sub_105C4 ;~ None:0629
	R(STI);	// 1356                  sti ;~ None:062C
	// 0 ;~ None:062D
locret_1069d:
	// 4463 
	// 0 ; CODE XREF: sub_10652+6↑j ;~ None:062D
	// 0 ; sub_10661+6↑j ;~ None:062D
	J(RETN(0));	// 1360                  retn ;~ None:062D
sub_1069e:
	// 1367 
	// 0 ; seg001:0369↑p ;~ None:062E
	// 0 ; DATA XREF: ... ;~ None:062E
edummylabel18:
	// 4465 
	R(bx = 0x1BB);	// 1370                  lea     bx, ds:1BBh ;~ None:062E
	J(JMP(loc_106a9));	// 1371                  jmp     short loc_106A9 ;~ None:0632
sub_106a5:
	// 1380 
	// 0 ; seg001:0369↑p ;~ None:0635
	// 0 ; DATA XREF: ... ;~ None:0635
edummylabel19:
	// 4467 
	R(bx = 0x1C3);	// 1383                  lea     bx, ds:1C3h ;~ None:0635
	// 0 ;~ None:0639
loc_106a9:
	// 4469 
	// 0 ; CODE XREF: sub_1069E+4↑j ;~ None:0639
	R(MOV(*(dw*)(raddr(ds,0x63)), bx));	// 1386                  mov     ds:63h, bx ;~ None:0639
	J(JMP(sub_107b5));	// 1387                  jmp     sub_107B5 ;~ None:063D
sub_106b0:
	// 1394 
	// 0 ; seg001:0369↑p ;~ None:0640
	// 0 ; DATA XREF: ... ;~ None:0640
edummylabel20:
	// 4471 
	R(MOV(*(dw*)(raddr(ds,0x63)), 0));	// 1397                  mov     word ptr ds:63h, 0 ;~ None:0640
	R(CMP(*(raddr(ds,0x49)), 0));	// 1398                  cmp     byte ptr ds:49h, 0 ;~ None:0646
	J(JZ(locret_106c2));	// 1399                  jz      short locret_106C2 ;~ None:064B
	R(MOV(*(raddr(ds,0x49)), 1));	// 1400                  mov     byte ptr ds:49h, 1 ;~ None:064D
	// 0 ;~ None:0652
locret_106c2:
	// 4473 
	// 0 ; CODE XREF: sub_106B0+B↑j ;~ None:0652
	J(RETN(0));	// 1403                  retn ;~ None:0652
sub_106c3:
	// 1410 
	// 0 ; seg001:0369↑p ;~ None:0653
	// 0 ; DATA XREF: ... ;~ None:0653
edummylabel21:
	// 4475 
	R(MOV(*(dw*)(raddr(ds,0x63)), 0));	// 1413                  mov     word ptr ds:63h, 0 ;~ None:0653
	R(bx = 0x74);	// 1414                  lea     bx, ds:74h ;~ None:0659
	J(JMP(sub_107b5));	// 1415                  jmp     sub_107B5 ;~ None:065D
sub_106d0:
	// 1422 
	// 0 ; seg001:0369↑p ;~ None:0660
	// 0 ; DATA XREF: ... ;~ None:0660
edummylabel22:
	// 4477 
	R(bx = 0x72);	// 1425                  lea     bx, ds:72h ;~ None:0660
	J(JMP(sub_107b5));	// 1426                  jmp     sub_107B5 ;~ None:0664
sub_106d7:
	// 1433 
	// 0 ; seg001:0369↑p ;~ None:0667
	// 0 ; DATA XREF: ... ;~ None:0667
edummylabel23:
	// 4479 
	R(MOV(*(raddr(ds,0x66)), 0x1E));	// 1436                  mov     byte ptr ds:66h, 1Eh ;~ None:0667
	R(MOV(*(raddr(ds,0x65)), 0x0FF));	// 1437                  mov     byte ptr ds:65h, 0FFh ;~ None:066C
	J(RETN(0));	// 1438                  retn ;~ None:0671
sub_106e2:
	// 1445 
	// 0 ; seg001:0369↑p ;~ None:0672
	// 0 ; DATA XREF: ... ;~ None:0672
edummylabel24:
	// 4481 
	R(MOV(*(raddr(ds,0x65)), 0));	// 1448                  mov     byte ptr ds:65h, 0 ;~ None:0672
	// 0 ;~ None:0677
locret_106e7:
	// 4483 
	// 0 ; CODE XREF: sub_106E8+6↓j ;~ None:0677
	// 0 ; sub_106FE+6↓j ... ;~ None:0677
	J(RETN(0));	// 1452                  retn ;~ None:0677
sub_106e8:
	// 1459 
	// 0 ; seg001:0369↑p ;~ None:0678
	// 0 ; DATA XREF: ... ;~ None:0678
edummylabel25:
	// 4485 
	R(al = 0x4B;);	// 1462                  mov     al, 4Bh ; 'K' ;~ None:0678
	R(CMP(al, *(raddr(ds,0x30))));	// 1463                  cmp     al, ds:30h ;~ None:067A
	J(JC(locret_106e7));	// 1464                  jb      short locret_106E7 ;~ None:067E
	R(bx = 0x153);	// 1465                  lea     bx, ds:153h ;~ None:0680
	// 0 ;~ None:0684
loc_106f4:
	// 4487 
	// 0 ; CODE XREF: sub_1071C+C↓j ;~ None:0684
	J(CALL(sub_107b2,0));	// 1468                  call    sub_107B2 ;~ None:0684
	R(bx = 0x143);	// 1469                  lea     bx, ds:143h ;~ None:0687
	J(JMP(sub_107b5));	// 1470                  jmp     sub_107B5 ;~ None:068B
sub_106fe:
	// 1477 
	// 0 ; seg001:0369↑p ;~ None:068E
	// 0 ; DATA XREF: ... ;~ None:068E
edummylabel26:
	// 4489 
	R(al = 0x32;);	// 1480                  mov     al, 32h ; '2' ;~ None:068E
	R(CMP(al, *(raddr(ds,0x30))));	// 1481                  cmp     al, ds:30h ;~ None:0690
	J(JC(locret_106e7));	// 1482                  jb      short locret_106E7 ;~ None:0694
	R(bx = 0x8D);	// 1483                  lea     bx, ds:8Dh ;~ None:0696
	J(JMP(sub_107b2));	// 1484                  jmp     sub_107B2 ;~ None:069A
sub_1070d:
	// 1491 
	// 0 ; seg001:0369↑p ;~ None:069D
	// 0 ; DATA XREF: ... ;~ None:069D
edummylabel27:
	// 4491 
	R(al = 0x32;);	// 1494                  mov     al, 32h ; '2' ;~ None:069D
	R(CMP(al, *(raddr(ds,0x30))));	// 1495                  cmp     al, ds:30h ;~ None:069F
	J(JC(locret_106e7));	// 1496                  jb      short locret_106E7 ;~ None:06A3
	R(bx = 0x11F);	// 1497                  lea     bx, ds:11Fh ;~ None:06A5
	J(JMP(sub_107b2));	// 1498                  jmp     sub_107B2 ;~ None:06A9
sub_1071c:
	// 1505 
	// 0 ; seg001:0369↑p ;~ None:06AC
	// 0 ; DATA XREF: ... ;~ None:06AC
edummylabel28:
	// 4493 
	R(al = 0x41;);	// 1508                  mov     al, 41h ; 'A' ;~ None:06AC
	R(CMP(al, *(raddr(ds,0x30))));	// 1509                  cmp     al, ds:30h ;~ None:06AE
	J(JC(locret_106e7));	// 1510                  jb      short locret_106E7 ;~ None:06B2
	R(bx = 0x15B);	// 1511                  lea     bx, ds:15Bh ;~ None:06B4
	J(JMP(loc_106f4));	// 1512                  jmp     short loc_106F4 ;~ None:06B8
sub_1072a:
	// 1519 
	// 0 ; seg001:0369↑p ;~ None:06BA
	// 0 ; DATA XREF: ... ;~ None:06BA
edummylabel29:
	// 4495 
	R(bx = 0x16A);	// 1522                  lea     bx, ds:16Ah ;~ None:06BA
	J(JMP(loc_10735));	// 1523                  jmp     short loc_10735 ;~ None:06BE
sub_10731:
	// 1532 
	// 0 ; seg001:0369↑p ;~ None:06C1
	// 0 ; DATA XREF: ... ;~ None:06C1
edummylabel30:
	// 4497 
	R(bx = 0x174);	// 1535                  lea     bx, ds:174h ;~ None:06C1
	// 0 ;~ None:06C5
loc_10735:
	// 4499 
	// 0 ; CODE XREF: sub_1072A+4↑j ;~ None:06C5
	J(CALL(sub_107b5,0));	// 1538                  call    sub_107B5 ;~ None:06C5
	R(bx = 0x163);	// 1539                  lea     bx, ds:163h ;~ None:06C8
	J(JMP(sub_107b5));	// 1540                  jmp     short sub_107B5 ;~ None:06CC
sub_1073f:
	// 1549 
	// 0 ; seg001:0369↑p ;~ None:06CF
	// 0 ; DATA XREF: ... ;~ None:06CF
edummylabel31:
	// 4501 
	R(bx = 0x16C);	// 1552                  lea     bx, ds:16Ch ;~ None:06CF
	J(JMP(loc_1074a));	// 1553                  jmp     short loc_1074A ;~ None:06D3
sub_10746:
	// 1562 
	// 0 ; seg001:0369↑p ;~ None:06D6
	// 0 ; DATA XREF: ... ;~ None:06D6
edummylabel32:
	// 4503 
	R(bx = 0x176);	// 1565                  lea     bx, ds:176h ;~ None:06D6
	// 0 ;~ None:06DA
loc_1074a:
	// 4505 
	// 0 ; CODE XREF: sub_1073F+4↑j ;~ None:06DA
	R(al = 0x28;);	// 1568                  mov     al, 28h ; '(' ;~ None:06DA
	R(CMP(al, *(raddr(ds,0x30))));	// 1569                  cmp     al, ds:30h ;~ None:06DC
	J(JC(locret_107ba));	// 1570                  jb      short locret_107BA ;~ None:06E0
	J(CALL(sub_107b2,0));	// 1571                  call    sub_107B2 ;~ None:06E2
	R(bx = 0x165);	// 1572                  lea     bx, ds:165h ;~ None:06E5
	J(JMP(sub_107b5));	// 1573                  jmp     short sub_107B5 ;~ None:06E9
sub_1075c:
	// 1582 
	// 0 ; seg001:0369↑p ;~ None:06EC
	// 0 ; DATA XREF: ... ;~ None:06EC
edummylabel33:
	// 4507 
	R(al = 0x3C;);	// 1585                  mov     al, 3Ch ; '<' ;~ None:06EC
	R(CMP(al, *(raddr(ds,0x30))));	// 1586                  cmp     al, ds:30h ;~ None:06EE
	J(JC(locret_107ba));	// 1587                  jb      short locret_107BA ;~ None:06F2
	R(bx = 0x0A6);	// 1588                  lea     bx, ds:0A6h ;~ None:06F4
	J(JMP(sub_107b2));	// 1589                  jmp     short sub_107B2 ;~ None:06F8
sub_1076b:
	// 1598 
	// 0 ; seg001:0369↑p ;~ None:06FB
	// 0 ; DATA XREF: ... ;~ None:06FB
edummylabel34:
	// 4509 
	R(al = 0x63;);	// 1601                  mov     al, 63h ; 'c' ;~ None:06FB
	R(CMP(al, *(raddr(ds,0x30))));	// 1602                  cmp     al, ds:30h ;~ None:06FD
	J(JC(locret_107ba));	// 1603                  jb      short locret_107BA ;~ None:0701
	R(bx = 0x187);	// 1604                  lea     bx, ds:187h ;~ None:0703
	J(CALL(sub_107b2,0));	// 1605                  call    sub_107B2 ;~ None:0707
	R(bx = 0x17E);	// 1606                  lea     bx, ds:17Eh ;~ None:070A
	J(JMP(sub_107b5));	// 1607                  jmp     short sub_107B5 ;~ None:070E
sub_10781:
	// 1616 
	// 0 ; seg001:0369↑p ;~ None:0711
	// 0 ; DATA XREF: ... ;~ None:0711
edummylabel35:
	// 4511 
	R(al = 0x46;);	// 1619                  mov     al, 46h ; 'F' ;~ None:0711
	R(CMP(al, *(raddr(ds,0x30))));	// 1620                  cmp     al, ds:30h ;~ None:0713
	J(JC(locret_107ba));	// 1621                  jb      short locret_107BA ;~ None:0717
	R(bx = 0x0F8);	// 1622                  lea     bx, ds:0F8h ;~ None:0719
	J(JMP(loc_1079c));	// 1623                  jmp     short loc_1079C ;~ None:071D
sub_10790:
	// 1632 
	// 0 ; seg001:0369↑p ;~ None:0720
	// 0 ; DATA XREF: ... ;~ None:0720
edummylabel36:
	// 4513 
	R(al = 0x5F;);	// 1635                  mov     al, 5Fh ; '_' ;~ None:0720
	R(CMP(al, *(raddr(ds,0x30))));	// 1636                  cmp     al, ds:30h ;~ None:0722
	J(JC(locret_107ba));	// 1637                  jb      short locret_107BA ;~ None:0726
	R(bx = 0x13B);	// 1638                  lea     bx, ds:13Bh ;~ None:0728
	// 0 ;~ None:072C
loc_1079c:
	// 4515 
	// 0 ; CODE XREF: sub_10781+C↑j ;~ None:072C
	J(CALL(sub_107b2,0));	// 1641                  call    sub_107B2 ;~ None:072C
	R(bx = 0x133);	// 1642                  lea     bx, ds:133h ;~ None:072F
	J(JMP(sub_107b5));	// 1643                  jmp     short sub_107B5 ;~ None:0733
sub_107a6:
	// 1652 
	// 0 ; seg001:0369↑p ;~ None:0736
	// 0 ; DATA XREF: ... ;~ None:0736
edummylabel37:
	// 4517 
	R(al = 0x2E;);	// 1655                  mov     al, 2Eh ; '.' ;~ None:0736
	R(CMP(al, *(raddr(ds,0x30))));	// 1656                  cmp     al, ds:30h ;~ None:0738
	J(JC(locret_107ba));	// 1657                  jb      short locret_107BA ;~ None:073C
	R(bx = 0x126);	// 1658                  lea     bx, ds:126h ;~ None:073E
sub_107b2:
	// 1665 
	// 0 ; sub_10661+C↑p ... ;~ None:0742
	R(MOV(*(raddr(ds,0x30)), al));	// 1667                  mov     ds:30h, al ;~ None:0742
sub_107b5:
	// 1674 
	// 0 ; sub_10677+4↑j ... ;~ None:0745
	R(CLI);	// 1676                  cli ;~ None:0745
	J(CALL(sub_105b0,0));	// 1677                  call    sub_105B0 ;~ None:0746
	R(STI);	// 1678                  sti ;~ None:0749
	// 0 ;~ None:074A
locret_107ba:
	// 4519 
	// 0 ; CODE XREF: sub_10746+A↑j ;~ None:074A
	// 0 ; sub_1075C+6↑j ... ;~ None:074A
	J(RETN(0));	// 1682                  retn ;~ None:074A
sub_107bb:
	// 1689 
	// 0 ; seg001:0369↑p ;~ None:074B
	// 0 ; DATA XREF: ... ;~ None:074B
edummylabel38:
	// 4521 
	R(al = 0x2D;);	// 1692                  mov     al, 2Dh ; '-' ;~ None:074B
	R(CMP(al, *(raddr(ds,0x30))));	// 1693                  cmp     al, ds:30h ;~ None:074D
	J(JC(locret_107ba));	// 1694                  jb      short locret_107BA ;~ None:0751
	R(bx = 0x12E);	// 1695                  lea     bx, ds:12Eh ;~ None:0753
	J(JMP(sub_107b2));	// 1696                  jmp     short sub_107B2 ;~ None:0757
sub_107c9:
	// 1703 
	// 0 ; seg001:0369↑p ;~ None:0759
	// 0 ; DATA XREF: ... ;~ None:0759
edummylabel39:
	// 4523 
	R(al = 0x55;);	// 1706                  mov     al, 55h ; 'U' ;~ None:0759
	R(CMP(al, *(raddr(ds,0x30))));	// 1707                  cmp     al, ds:30h ;~ None:075B
	J(JC(locret_107ba));	// 1708                  jb      short locret_107BA ;~ None:075F
	R(bx = 0x190);	// 1709                  lea     bx, ds:190h ;~ None:0761
	J(JMP(sub_107b2));	// 1710                  jmp     short sub_107B2 ;~ None:0765
sub_107d7:
	// 1717 
	// 0 ; seg001:0369↑p ;~ None:0767
	// 0 ; DATA XREF: ... ;~ None:0767
edummylabel40:
	// 4525 
	R(al = 0x32;);	// 1720                  mov     al, 32h ; '2' ;~ None:0767
	R(CMP(al, *(raddr(ds,0x30))));	// 1721                  cmp     al, ds:30h ;~ None:0769
	J(JC(locret_107ba));	// 1722                  jb      short locret_107BA ;~ None:076D
	R(bx = 0x117);	// 1723                  lea     bx, ds:117h ;~ None:076F
	J(JMP(sub_107b2));	// 1724                  jmp     short sub_107B2 ;~ None:0773
sub_107e5:
	// 1731 
	// 0 ; seg001:0369↑p ;~ None:0775
	// 0 ; DATA XREF: ... ;~ None:0775
edummylabel41:
	// 4527 
	R(al = 0x50;);	// 1734                  mov     al, 50h ; 'P' ;~ None:0775
	R(CMP(al, *(raddr(ds,0x30))));	// 1735                  cmp     al, ds:30h ;~ None:0777
	J(JC(locret_107ba));	// 1736                  jb      short locret_107BA ;~ None:077B
	R(bx = 0x14B);	// 1737                  lea     bx, ds:14Bh ;~ None:077D
	J(CALL(sub_107b2,0));	// 1738                  call    sub_107B2 ;~ None:0781
	R(bx = 0x153);	// 1739                  lea     bx, ds:153h ;~ None:0784
	J(JMP(sub_107b5));	// 1740                  jmp     short sub_107B5 ;~ None:0788
sub_107fa:
	// 1747 
	// 0 ; seg001:0369↑p ;~ None:078A
	// 0 ; DATA XREF: ... ;~ None:078A
edummylabel42:
	// 4529 
	R(bx = 0x0AE);	// 1750                  lea     bx, ds:0AEh ;~ None:078A
	J(JMP(loc_10805));	// 1751                  jmp     short loc_10805 ;~ None:078E
sub_10801:
	// 1760 
	// 0 ; seg001:0369↑p ;~ None:0791
	// 0 ; DATA XREF: ... ;~ None:0791
edummylabel43:
	// 4531 
	R(bx = 0x0C4);	// 1763                  lea     bx, ds:0C4h ;~ None:0791
	// 0 ;~ None:0795
loc_10805:
	// 4533 
	// 0 ; CODE XREF: sub_107FA+4↑j ;~ None:0795
	J(CALL(sub_107b5,0));	// 1766                  call    sub_107B5 ;~ None:0795
	R(bx = 0x0DA);	// 1767                  lea     bx, ds:0DAh ;~ None:0798
	J(JMP(sub_107b5));	// 1768                  jmp     short sub_107B5 ;~ None:079C
sub_1080e:
	// 1775 
	// 0 ; seg001:0369↑p ;~ None:079E
	// 0 ; DATA XREF: ... ;~ None:079E
edummylabel44:
	// 4535 
	R(bx = 0x0E1);	// 1778                  lea     bx, ds:0E1h ;~ None:079E
	J(JMP(sub_107b5));	// 1779                  jmp     short sub_107B5 ;~ None:07A2
sub_10814:
	// 1786 
	// 0 ; seg001:0369↑p ;~ None:07A4
	// 0 ; DATA XREF: ... ;~ None:07A4
edummylabel45:
	// 4537 
	R(bx = 0x1D0);	// 1789                  lea     bx, ds:1D0h ;~ None:07A4
	J(JMP(sub_107b5));	// 1790                  jmp     short sub_107B5 ;~ None:07A8
sub_1081a:
	// 1797 
	// 0 ; seg001:0369↑p ;~ None:07AA
	// 0 ; DATA XREF: ... ;~ None:07AA
edummylabel46:
	// 4539 
	R(bx = 0x1CB);	// 1800                  lea     bx, ds:1CBh ;~ None:07AA
	J(JMP(sub_107b5));	// 1801                  jmp     short sub_107B5 ;~ None:07AE
sub_10820:
	// 1808 
	// 0 ; seg001:0369↑p ;~ None:07B0
	// 0 ; DATA XREF: ... ;~ None:07B0
edummylabel47:
	// 4541 
	R(bx = 0x6A);	// 1811                  lea     bx, ds:6Ah ;~ None:07B0
	J(JMP(sub_107b5));	// 1812                  jmp     short sub_107B5 ;~ None:07B4
sub_10826:
	// 1819 
	// 0 ; seg001:0369↑p ;~ None:07B6
	// 0 ; DATA XREF: ... ;~ None:07B6
edummylabel48:
	// 4543 
	R(bx = 0x110);	// 1822                  lea     bx, ds:110h ;~ None:07B6
	J(CALL(sub_107b5,0));	// 1823                  call    sub_107B5 ;~ None:07BA
	R(bx = 0x84);	// 1824                  lea     bx, ds:84h ;~ None:07BD
	J(JMP(sub_107b5));	// 1825                  jmp     short sub_107B5 ;~ None:07C1
sub_10833:
	// 1832 
	// 0 ; seg001:0369↑p ... ;~ None:07C3
edummylabel49:
	// 4545 
	R(bx = 0x1EF);	// 1834                  lea     bx, ds:1EFh ;~ None:07C3
	J(CALL(sub_107b5,0));	// 1835                  call    sub_107B5 ;~ None:07C7
	R(bx = 0x1DF);	// 1836                  lea     bx, ds:1DFh ;~ None:07CA
	R(MOV(*(dw*)(raddr(ds,0x38)), bx));	// 1837                  mov     ds:38h, bx ;~ None:07CE
	R(bx = 0x23D);	// 1838                  lea     bx, ds:23Dh ;~ None:07D2
	J(CALL(sub_107b5,0));	// 1839                  call    sub_107B5 ;~ None:07D6
	R(bx = 0x235);	// 1840                  lea     bx, ds:235h ;~ None:07D9
	R(MOV(*(dw*)(raddr(ds,0x45)), bx));	// 1841                  mov     ds:45h, bx ;~ None:07DD
sub_10851:
	// 1848 
	// 0 ; seg001:0369↑p ;~ None:07E1
	// 0 ; DATA XREF: ... ;~ None:07E1
	R(bx = 0x28B);	// 1851                  lea     bx, ds:28Bh ;~ None:07E1
	J(CALL(sub_107b5,0));	// 1852                  call    sub_107B5 ;~ None:07E5
	R(bx = 0x26B);	// 1853                  lea     bx, ds:26Bh ;~ None:07E8
	R(MOV(*(dw*)(raddr(ds,0x52)), bx));	// 1854                  mov     ds:52h, bx ;~ None:07EC
	R(bx = 0x2FD);	// 1855                  lea     bx, ds:2FDh ;~ None:07F0
	J(JMP(sub_107b5));	// 1856                  jmp     sub_107B5 ;~ None:07F4
sub_10867:
	// 1863 
	// 0 ; seg001:0369↑p ;~ None:07F7
	// 0 ; DATA XREF: ... ;~ None:07F7
edummylabel50:
	// 4547 
	J(CALL(sub_10833,0));	// 1866                  call    sub_10833 ;~ None:07F7
	R(bx = 0x210);	// 1867                  lea     bx, ds:210h ;~ None:07FA
	R(MOV(*(dw*)(raddr(ds,0x38)), bx));	// 1868                  mov     ds:38h, bx ;~ None:07FE
	R(bx = 0x246);	// 1869                  lea     bx, ds:246h ;~ None:0802
	R(MOV(*(dw*)(raddr(ds,0x45)), bx));	// 1870                  mov     ds:45h, bx ;~ None:0806
	R(bx = 0x2AC);	// 1871                  lea     bx, ds:2ACh ;~ None:080A
	R(MOV(*(dw*)(raddr(ds,0x52)), bx));	// 1872                  mov     ds:52h, bx ;~ None:080E
	R(bl = 0x18;);	// 1873                  mov     bl, 18h ;~ None:0812
	R(MOV(*(raddr(ds,0x5E)), bl));	// 1874                  mov     ds:5Eh, bl ;~ None:0814
	J(RETN(0));	// 1875                  retn ;~ None:0818
sub_10889:
	// 1882 
	// 0 ; sub_1059B+A↑p ;~ None:0819
edummylabel51:
	// 4549 
	R(CMP(*(raddr(ds,0x67)), 0x0FF));	// 1884                  cmp     byte ptr ds:67h, 0FFh ;~ None:0819
	J(JZ(locret_108bc));	// 1885                  jz      short locret_108BC ;~ None:081E
	R(ax = bx;);	// 1886                  mov     ax, bx ;~ None:0820
	R(CMP(ah, 0x0BF));	// 1887                  cmp     ah, 0BFh ;~ None:0822
	J(JA(loc_108bd));	// 1888                  ja      short loc_108BD ;~ None:0825
	R(SHR(ah, 1));	// 1889                  shr     ah, 1 ;~ None:0827
	R(OR(ah, 0x90));	// 1890                  or      ah, 90h ;~ None:0829
	R(AND(ah, 0x0F0));	// 1891                  and     ah, 0F0h ;~ None:082C
	R(AND(al, 0x0F));	// 1892                  and     al, 0Fh ;~ None:082F
	R(OR(al, ah));	// 1893                  or      al, ah ;~ None:0831
	R(OUT(0x0C0, al));	// 1894                  out     0C0h, al        ; DMA controller, 8237A-5. ;~ None:0833
	// 0 ; channel 0 base address ;~ None:0833
	// 0 ; (also sets current address) ;~ None:0833
	R(AND(ah, 0x0E0));	// 1897                  and     ah, 0E0h ;~ None:0835
	R(al = bl;);	// 1898                  mov     al, bl ;~ None:0838
	R(SHR(al, 1));	// 1899                  shr     al, 1 ;~ None:083A
	R(SHR(al, 1));	// 1900                  shr     al, 1 ;~ None:083C
	R(SHR(al, 1));	// 1901                  shr     al, 1 ;~ None:083E
	R(SHR(al, 1));	// 1902                  shr     al, 1 ;~ None:0840
	R(OR(al, ah));	// 1903                  or      al, ah ;~ None:0842
	R(OUT(0x0C0, al));	// 1904                  out     0C0h, al        ; DMA controller, 8237A-5. ;~ None:0844
	// 0 ; channel 0 base address ;~ None:0844
	// 0 ; (also sets current address) ;~ None:0844
	R(al = bh;);	// 1907                  mov     al, bh ;~ None:0846
	R(AND(al, 0x3F));	// 1908                  and     al, 3Fh ;~ None:0848
	R(OUT(0x0C0, al));	// 1909                  out     0C0h, al        ; DMA controller, 8237A-5. ;~ None:084A
	// 0 ; channel 0 base address ;~ None:084A
	// 0 ; (also sets current address) ;~ None:084A
	// 0 ;~ None:084C
locret_108bc:
	// 4551 
	// 0 ; CODE XREF: sub_10889+5↑j ;~ None:084C
	J(RETN(0));	// 1914                  retn ;~ None:084C
	// 0 ; --------------------------------------------------------------------------- ;~ None:084D
	// 0 ;~ None:084D
loc_108bd:
	// 4553 
	// 0 ; CODE XREF: sub_10889+C↑j ;~ None:084D
	R(al = bh;);	// 1918                  mov     al, bh ;~ None:084D
	R(OR(al, 0x0E0));	// 1919                  or      al, 0E0h ;~ None:084F
	R(OUT(0x0C0, al));	// 1920                  out     0C0h, al        ; DMA controller, 8237A-5. ;~ None:0851
	// 0 ; channel 0 base address ;~ None:0851
	// 0 ; (also sets current address) ;~ None:0851
	R(al = bl;);	// 1923                  mov     al, bl ;~ None:0853
	R(OR(al, 0x0F0));	// 1924                  or      al, 0F0h ;~ None:0855
	R(OUT(0x0C0, al));	// 1925                  out     0C0h, al        ; DMA controller, 8237A-5. ;~ None:0857
	// 0 ; channel 0 base address ;~ None:0857
	// 0 ; (also sets current address) ;~ None:0857
	J(RETN(0));	// 1928                  retn ;~ None:0859

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
                case m2c::kedummylabel1: 	goto edummylabel1;
        case m2c::kedummylabel10: 	goto edummylabel10;
        case m2c::kedummylabel11: 	goto edummylabel11;
        case m2c::kedummylabel12: 	goto edummylabel12;
        case m2c::kedummylabel13: 	goto edummylabel13;
        case m2c::kedummylabel14: 	goto edummylabel14;
        case m2c::kedummylabel15: 	goto edummylabel15;
        case m2c::kedummylabel16: 	goto edummylabel16;
        case m2c::kedummylabel17: 	goto edummylabel17;
        case m2c::kedummylabel18: 	goto edummylabel18;
        case m2c::kedummylabel19: 	goto edummylabel19;
        case m2c::kedummylabel2: 	goto edummylabel2;
        case m2c::kedummylabel20: 	goto edummylabel20;
        case m2c::kedummylabel21: 	goto edummylabel21;
        case m2c::kedummylabel22: 	goto edummylabel22;
        case m2c::kedummylabel23: 	goto edummylabel23;
        case m2c::kedummylabel24: 	goto edummylabel24;
        case m2c::kedummylabel25: 	goto edummylabel25;
        case m2c::kedummylabel26: 	goto edummylabel26;
        case m2c::kedummylabel27: 	goto edummylabel27;
        case m2c::kedummylabel28: 	goto edummylabel28;
        case m2c::kedummylabel29: 	goto edummylabel29;
        case m2c::kedummylabel3: 	goto edummylabel3;
        case m2c::kedummylabel30: 	goto edummylabel30;
        case m2c::kedummylabel31: 	goto edummylabel31;
        case m2c::kedummylabel32: 	goto edummylabel32;
        case m2c::kedummylabel33: 	goto edummylabel33;
        case m2c::kedummylabel34: 	goto edummylabel34;
        case m2c::kedummylabel35: 	goto edummylabel35;
        case m2c::kedummylabel36: 	goto edummylabel36;
        case m2c::kedummylabel37: 	goto edummylabel37;
        case m2c::kedummylabel38: 	goto edummylabel38;
        case m2c::kedummylabel39: 	goto edummylabel39;
        case m2c::kedummylabel4: 	goto edummylabel4;
        case m2c::kedummylabel40: 	goto edummylabel40;
        case m2c::kedummylabel41: 	goto edummylabel41;
        case m2c::kedummylabel42: 	goto edummylabel42;
        case m2c::kedummylabel43: 	goto edummylabel43;
        case m2c::kedummylabel44: 	goto edummylabel44;
        case m2c::kedummylabel45: 	goto edummylabel45;
        case m2c::kedummylabel46: 	goto edummylabel46;
        case m2c::kedummylabel47: 	goto edummylabel47;
        case m2c::kedummylabel48: 	goto edummylabel48;
        case m2c::kedummylabel49: 	goto edummylabel49;
        case m2c::kedummylabel5: 	goto edummylabel5;
        case m2c::kedummylabel50: 	goto edummylabel50;
        case m2c::kedummylabel51: 	goto edummylabel51;
        case m2c::kedummylabel6: 	goto edummylabel6;
        case m2c::kedummylabel7: 	goto edummylabel7;
        case m2c::kedummylabel8: 	goto edummylabel8;
        case m2c::kedummylabel9: 	goto edummylabel9;
        case m2c::kloc_1046b: 	goto loc_1046b;
        case m2c::kloc_104bb: 	goto loc_104bb;
        case m2c::kloc_104bf: 	goto loc_104bf;
        case m2c::kloc_104e1: 	goto loc_104e1;
        case m2c::kloc_10502: 	goto loc_10502;
        case m2c::kloc_10506: 	goto loc_10506;
        case m2c::kloc_10566: 	goto loc_10566;
        case m2c::kloc_1058c: 	goto loc_1058c;
        case m2c::kloc_10592: 	goto loc_10592;
        case m2c::kloc_105a8: 	goto loc_105a8;
        case m2c::kloc_105d2: 	goto loc_105d2;
        case m2c::kloc_105f6: 	goto loc_105f6;
        case m2c::kloc_105fa: 	goto loc_105fa;
        case m2c::kloc_105fd: 	goto loc_105fd;
        case m2c::kloc_10605: 	goto loc_10605;
        case m2c::kloc_10615: 	goto loc_10615;
        case m2c::kloc_1061f: 	goto loc_1061f;
        case m2c::kloc_10629: 	goto loc_10629;
        case m2c::kloc_1062d: 	goto loc_1062d;
        case m2c::kloc_10630: 	goto loc_10630;
        case m2c::kloc_10642: 	goto loc_10642;
        case m2c::kloc_1064a: 	goto loc_1064a;
        case m2c::kloc_1064d: 	goto loc_1064d;
        case m2c::kloc_10694: 	goto loc_10694;
        case m2c::kloc_106a9: 	goto loc_106a9;
        case m2c::kloc_106f4: 	goto loc_106f4;
        case m2c::kloc_10735: 	goto loc_10735;
        case m2c::kloc_1074a: 	goto loc_1074a;
        case m2c::kloc_1079c: 	goto loc_1079c;
        case m2c::kloc_10805: 	goto loc_10805;
        case m2c::kloc_108bd: 	goto loc_108bd;
        case m2c::klocret_1008c: 	goto locret_1008c;
        case m2c::klocret_103dd: 	goto locret_103dd;
        case m2c::klocret_1040a: 	goto locret_1040a;
        case m2c::klocret_1046a: 	goto locret_1046a;
        case m2c::klocret_104c4: 	goto locret_104c4;
        case m2c::klocret_10604: 	goto locret_10604;
        case m2c::klocret_1069d: 	goto locret_1069d;
        case m2c::klocret_106c2: 	goto locret_106c2;
        case m2c::klocret_106e7: 	goto locret_106e7;
        case m2c::klocret_107ba: 	goto locret_107ba;
        case m2c::klocret_108bc: 	goto locret_108bc;
        case m2c::kseg001_33e_proc: 	goto seg001_33e_proc;
        case m2c::kseg001_4_proc: 	goto seg001_4_proc;
        case m2c::ksub_1040b: 	goto sub_1040b;
        case m2c::ksub_104a4: 	goto sub_104a4;
        case m2c::ksub_104c5: 	goto sub_104c5;
        case m2c::ksub_1051f: 	goto sub_1051f;
        case m2c::ksub_1059b: 	goto sub_1059b;
        case m2c::ksub_105b0: 	goto sub_105b0;
        case m2c::ksub_105c4: 	goto sub_105c4;
        case m2c::ksub_10652: 	goto sub_10652;
        case m2c::ksub_10661: 	goto sub_10661;
        case m2c::ksub_10677: 	goto sub_10677;
        case m2c::ksub_1067e: 	goto sub_1067e;
        case m2c::ksub_1069e: 	goto sub_1069e;
        case m2c::ksub_106a5: 	goto sub_106a5;
        case m2c::ksub_106b0: 	goto sub_106b0;
        case m2c::ksub_106c3: 	goto sub_106c3;
        case m2c::ksub_106d0: 	goto sub_106d0;
        case m2c::ksub_106d7: 	goto sub_106d7;
        case m2c::ksub_106e2: 	goto sub_106e2;
        case m2c::ksub_106e8: 	goto sub_106e8;
        case m2c::ksub_106fe: 	goto sub_106fe;
        case m2c::ksub_1070d: 	goto sub_1070d;
        case m2c::ksub_1071c: 	goto sub_1071c;
        case m2c::ksub_1072a: 	goto sub_1072a;
        case m2c::ksub_10731: 	goto sub_10731;
        case m2c::ksub_1073f: 	goto sub_1073f;
        case m2c::ksub_10746: 	goto sub_10746;
        case m2c::ksub_1075c: 	goto sub_1075c;
        case m2c::ksub_1076b: 	goto sub_1076b;
        case m2c::ksub_10781: 	goto sub_10781;
        case m2c::ksub_10790: 	goto sub_10790;
        case m2c::ksub_107a6: 	goto sub_107a6;
        case m2c::ksub_107b2: 	goto sub_107b2;
        case m2c::ksub_107b5: 	goto sub_107b5;
        case m2c::ksub_107bb: 	goto sub_107bb;
        case m2c::ksub_107c9: 	goto sub_107c9;
        case m2c::ksub_107d7: 	goto sub_107d7;
        case m2c::ksub_107e5: 	goto sub_107e5;
        case m2c::ksub_107fa: 	goto sub_107fa;
        case m2c::ksub_10801: 	goto sub_10801;
        case m2c::ksub_1080e: 	goto sub_1080e;
        case m2c::ksub_10814: 	goto sub_10814;
        case m2c::ksub_1081a: 	goto sub_1081a;
        case m2c::ksub_10820: 	goto sub_10820;
        case m2c::ksub_10826: 	goto sub_10826;
        case m2c::ksub_10833: 	goto sub_10833;
        case m2c::ksub_10851: 	goto sub_10851;
        case m2c::ksub_10867: 	goto sub_10867;
        case m2c::ksub_10889: 	goto sub_10889;
        default: return __dispatch_call(__disp, _state);
    };
}

