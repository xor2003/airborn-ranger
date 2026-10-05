// data-segment layout overlaid on mem[] (generated)
#pragma once
#include <stddef.h>

#pragma pack(push, 1)
struct ar_dseg {
    db _pad_0[0x1a20];
    dw v_seg_10000; /* 0x1a20  seg_data */
    dw v_seg_10002; /* 0x1a22 */
    dw v_seg_10004; /* 0x1a24  seg_resbuf */
    db _pad_1a26[0x8];
    dw v_seg_1000e; /* 0x1a2e */
    dw v_seg_10010; /* 0x1a30  seg_flip */
    dw v_seg_10012; /* 0x1a32  seg_draw */
    dw v_seg_10014; /* 0x1a34  seg_aux */
    dw v_seg_10016; /* 0x1a36 */
    dw v_seg_10018; /* 0x1a38 */
    db _pad_1a3a[0xa9];
    dw v_word_100c3; /* 0x1ae3 */
    db _pad_1ae5[0x2c8];
    dd v_jpt_1039d; /* 0x1dad  pal_jt */
    db _pad_1db1[0x3fc];
    dd v_jpt_107b6; /* 0x21ad */
    db _pad_21b1[0x2a];
    dd v_funcs_1083a; /* 0x21db  modecall_tbl */
    db _pad_21df[0x2f6];
    dd v_jpt_10ad3; /* 0x24d5  clrflip_jt */
    db _pad_24d9[0x5d];
    dd v_jpt_10b34; /* 0x2536  clrdraw_jt */
    db _pad_253a[0x1f];
    dd v_jpt_10b57; /* 0x2559  clrback_jt */
    db _pad_255d[0x8f];
    dd v_jpt_10bef; /* 0x25ec */
    db _pad_25f0[0x5c];
    dd v_jpt_10c4f; /* 0x264c */
    db _pad_2650[0x792];
    dd v_jpt_113e3; /* 0x2de2  vidmode_jt */
    db _pad_2de6[0x1f7];
    dd v_funcs_115e7; /* 0x2fdd */
    db _pad_2fe1[0x78];
    dd v_jpt_1164f; /* 0x3059 */
    db _pad_305d[0xc4];
    dd v_jpt_1171c; /* 0x3121 */
    db _pad_3125[0x26e];
    dd v_jpt_11999; /* 0x3393  present_jt */
    db _pad_3397[0x98];
    dd v_jpt_11a25; /* 0x342f */
    db _pad_3433[0x19f];
    dd v_jpt_11bc9; /* 0x35d2 */
    db _pad_35d6[0x82];
    dd v_jpt_11c4e; /* 0x3658 */
    db _pad_365c[0x24];
    dd v_funcs_11cb8; /* 0x3680 */
    db _pad_3684[0x8f];
    db v_byte_11cf3; /* 0x3713 */
    dd v_funcs_11d52; /* 0x3714 */
    db _pad_3718[0x9b7];
    dd v_jpt_126d5; /* 0x40cf */
    db _pad_40d3[0x27];
    dw v_word_126da; /* 0x40fa */
    dw v_word_126dc; /* 0x40fc */
    db _pad_40fe[0xf89];
    dd v_jpt_1367d; /* 0x5087 */
    db _pad_508b[0x425];
    dw v_word_13a90; /* 0x54b0 */
    dw v_word_13a92; /* 0x54b2 */
    db _pad_54b4[0x13c];
    dw v_word_13bd0; /* 0x55f0 */
    db _pad_55f2[0xe];
    dd v_funcs_13bf8; /* 0x5600 */
    db _pad_5604[0xcd];
    dd v_funcs_13cc9; /* 0x56d1 */
    db _pad_56d5[0xd4];
    dd v_funcs_13da1; /* 0x57a9 */
    db _pad_57ad[0xcd];
    dd v_funcs_13e72; /* 0x587a */
    db _pad_587e[0xd2];
    dd v_funcs_13f48; /* 0x5950 */
    db _pad_5954[0xd5];
    dd v_funcs_14021; /* 0x5a29  scroll_tbl_f */
    db _pad_5a2d[0xd5];
    dd v_funcs_140fa; /* 0x5b02 */
    db _pad_5b06[0xd0];
    dd v_funcs_141ce; /* 0x5bd6 */
    db _pad_5bda[0x17d6];
    dd v_funcs_159e9; /* 0x73b0  podhit_tbl */
    db _pad_73b4[0x10fd];
    dd v_jpt_16ab9; /* 0x84b1 */
    db _pad_84b5[0x1188];
    dd v_jpt_17c64; /* 0x963d  bar_jt */
    db _pad_9641[0xcaf];
    dd v_funcs_1892f; /* 0xa2f0 */
    db _pad_a2f4[0x20dc];
    dd v_funcs_1a9c8; /* 0xc3d0 */
    db _pad_c3d4[0x6d2];
    dd v_jpt_1b098; /* 0xcaa6 */
    db _pad_caaa[0x346];
    dd v_funcs_1b3e2; /* 0xcdf0 */
    db _pad_cdf4[0x26];
    dd v_funcs_1b41f; /* 0xce1a */
    db _pad_ce1e[0x59];
    dd v_jpt_1b475; /* 0xce77 */
    db _pad_ce7b[0x1a25];
    dw v_word_1ce80; /* 0xe8a0 */
    db _pad_e8a2[0xe];
    db v_byte_1ce90; /* 0xe8b0 */
    db _pad_e8b1[0x1];
    dw v_word_1ce92; /* 0xe8b2 */
    dw v_word_1ce94; /* 0xe8b4 */
    dw v_word_1ce96; /* 0xe8b6 */
    dw v_word_1ce98; /* 0xe8b8 */
    db _pad_e8ba[0x2];
    dw v_word_1ce9c; /* 0xe8bc */
    db _pad_e8be[0x2];
    dw v_word_1cea0; /* 0xe8c0 */
    db v_byte_1cea2; /* 0xe8c2 */
    dw v_word_1cea3; /* 0xe8c3 */
    dw v_word_1cea5; /* 0xe8c5 */
    db _pad_e8c7[0xb];
    dw v_word_1ceb2; /* 0xe8d2 */
    dw v_word_1ceb4; /* 0xe8d4 */
    dw v_word_1ceb6; /* 0xe8d6 */
    db _pad_e8d8[0x13a];
    dw v_word_1cff2; /* 0xea12 */
    dw v_word_1cff4; /* 0xea14 */
    dw v_word_1cff6; /* 0xea16 */
    db _pad_ea18[0xc];
    dw v_word_1d004; /* 0xea24 */
    dw v_word_1d006; /* 0xea26 */
    dw v_word_1d008; /* 0xea28 */
    dw v_word_1d00a; /* 0xea2a */
    dw v_word_1d00c; /* 0xea2c */
    dw v_word_1d00e; /* 0xea2e */
    dw v_word_1d010; /* 0xea30 */
    dw v_word_1d012; /* 0xea32 */
    dw v_word_1d014; /* 0xea34 */
    dw v_word_1d016; /* 0xea36 */
    dw v_word_1d018; /* 0xea38 */
    dw v_word_1d01a; /* 0xea3a */
    db _pad_ea3c[0x258];
    db v_afarmchrdtx; /* 0xec94 */
    db _pad_ec95[0x2f];
    db v_aarcchrdtx; /* 0xecc4 */
    db _pad_ecc5[0x2f];
    db v_amagchrdtx; /* 0xecf4 */
    db _pad_ecf5[0x77];
    db v_apcoltandtx; /* 0xed6c */
    db _pad_ed6d[0x17];
    db v_apcolegadtx; /* 0xed84 */
    db _pad_ed85[0x17];
    db v_apcolmcgdtx; /* 0xed9c */
    db _pad_ed9d[0x17];
    db v_apcolhrcdtx; /* 0xedb4 */
    db _pad_edb5[0x17];
    db v_apribbondtx; /* 0xedcc */
    db _pad_edcd[0x107];
    db v_acfnegchrdtx; /* 0xeed4 */
    db _pad_eed5[0x2f];
    db v_acanegchrdtx; /* 0xef04 */
    db _pad_ef05[0x2f];
    db v_acidadat; /* 0xef34 */
    db _pad_ef35[0x1c3];
    dw v_word_1d6d8; /* 0xf0f8 */
    dw v_word_1d6da; /* 0xf0fa */
    db v_byte_1d6dc; /* 0xf0fc */
    db v_byte_1d6dd; /* 0xf0fd */
    dw v_word_1d6de; /* 0xf0fe */
    db _pad_f100[0x2];
    dw v_word_1d6e2; /* 0xf102 */
    dw v_word_1d6e4; /* 0xf104 */
    db _pad_f106[0xda];
    dw v_word_1d7c0; /* 0xf1e0 */
    db _pad_f1e2[0x10e];
    dw v_word_1d8d0; /* 0xf2f0 */
    dw v_word_1d8d2; /* 0xf2f2 */
    dw v_word_1d8d4; /* 0xf2f4 */
    dw v_word_1d8d6; /* 0xf2f6 */
    dw v_word_1d8d8; /* 0xf2f8 */
    db _pad_f2fa[0x10];
    dw v_word_1d8ea; /* 0xf30a */
    dw v_word_1d8ec; /* 0xf30c */
    db _pad_f30e[0x8];
    dw v_word_1d8f6; /* 0xf316 */
    dw v_word_1d8f8; /* 0xf318 */
    db _pad_f31a[0x6];
    dw v_word_1d900; /* 0xf320 */
    dw v_word_1d902; /* 0xf322 */
    db _pad_f324[0x12];
    dw v_word_1d916; /* 0xf336 */
    dw v_word_1d918; /* 0xf338 */
    db _pad_f33a[0x2];
    dw v_word_1d91c; /* 0xf33c */
    dw v_word_1d91e; /* 0xf33e */
    dw v_word_1d920; /* 0xf340 */
    db _pad_f342[0xa];
    dw v_word_1d92c; /* 0xf34c */
    dw v_word_1d92e; /* 0xf34e */
    dw v_word_1d930; /* 0xf350 */
    db _pad_f352[0x2];
    dw v_word_1d934; /* 0xf354 */
    db v_byte_1d936; /* 0xf356 */
    dw v_word_1d937; /* 0xf357 */
    db v_byte_1d939; /* 0xf359 */
    db v_byte_1d93a; /* 0xf35a */
    db v_byte_1d93b; /* 0xf35b */
    db _pad_f35c[0x2];
    db v_byte_1d93e; /* 0xf35e */
    dw v_word_1d93f; /* 0xf35f */
    db _pad_f361[0x2];
    dw v_word_1d943; /* 0xf363 */
    dw v_word_1d945; /* 0xf365 */
    db _pad_f367[0x2];
    dw v_word_1d949; /* 0xf369 */
    dw v_word_1d94b; /* 0xf36b */
    dw v_word_1d94d; /* 0xf36d */
    dw v_word_1d94f; /* 0xf36f */
    dw v_word_1d951; /* 0xf371 */
    dw v_word_1d953; /* 0xf373 */
    dw v_word_1d955; /* 0xf375 */
    dw v_word_1d957; /* 0xf377 */
    dw v_word_1d959; /* 0xf379 */
    db _pad_f37b[0x2];
    dw v_word_1d95d; /* 0xf37d */
    dw v_word_1d95f; /* 0xf37f */
    dw v_word_1d961; /* 0xf381 */
    db _pad_f383[0x2];
    dw v_word_1d965; /* 0xf385 */
    db _pad_f387[0x2];
    dw v_word_1d969; /* 0xf389 */
    db _pad_f38b[0x272];
    dw v_word_1dbdd; /* 0xf5fd */
    db _pad_f5ff[0x86];
    db v_byte_1dc65; /* 0xf685 */
    db _pad_f686[0x1];
    dw v_word_1dc67; /* 0xf687 */
    dw v_word_1dc69; /* 0xf689 */
    dw v_word_1dc6b; /* 0xf68b */
    dw v_word_1dc6d; /* 0xf68d */
    dw v_word_1dc6f; /* 0xf68f */
    dw v_word_1dc71; /* 0xf691 */
    dw v_word_1dc73; /* 0xf693 */
    dw v_word_1dc75; /* 0xf695 */
    dw v_word_1dc77; /* 0xf697 */
    db v_byte_1dc79; /* 0xf699 */
    db v_byte_1dc7a; /* 0xf69a */
    db _pad_f69b[0x2];
    db v_byte_1dc7d; /* 0xf69d */
    db _pad_f69e[0x2];
    dw v_word_1dc80; /* 0xf6a0 */
    dw v_word_1dc82; /* 0xf6a2 */
    db _pad_f6a4[0x2];
    dw v_word_1dc86; /* 0xf6a6 */
    dw v_word_1dc88; /* 0xf6a8 */
    db _pad_f6aa[0x2];
    dw v_word_1dc8c; /* 0xf6ac */
    dw v_word_1dc8e; /* 0xf6ae */
    db _pad_f6b0[0x2];
    dw v_word_1dc92; /* 0xf6b2 */
    dw v_word_1dc94; /* 0xf6b4 */
    db _pad_f6b6[0x2];
    dw v_word_1dc98; /* 0xf6b8 */
    dw v_word_1dc9a; /* 0xf6ba */
    db _pad_f6bc[0x2];
    dw v_word_1dc9e; /* 0xf6be */
    dw v_word_1dca0; /* 0xf6c0 */
    dw v_word_1dca2; /* 0xf6c2 */
    dw v_word_1dca4; /* 0xf6c4 */
    db _pad_f6c6[0x4];
    db v_byte_1dcaa; /* 0xf6ca */
    db v_byte_1dcab; /* 0xf6cb */
    dw v_word_1dcac; /* 0xf6cc */
    dw v_word_1dcae; /* 0xf6ce */
    db v_byte_1dcb0; /* 0xf6d0 */
    dw v_word_1dcb1; /* 0xf6d1 */
    dw v_word_1dcb3; /* 0xf6d3 */
    db _pad_f6d5[0xa];
    dw v_word_1dcbf; /* 0xf6df */
    db v_byte_1dcc1; /* 0xf6e1 */
    db _pad_f6e2[0x1b];
    db v_byte_1dcdd; /* 0xf6fd */
    db _pad_f6fe[0x1];
    dw v_word_1dcdf; /* 0xf6ff */
    db v_byte_1dce1; /* 0xf701 */
    db _pad_f702[0x1d];
    dw v_word_1dcff; /* 0xf71f */
    db v_byte_1dd01; /* 0xf721 */
    db _pad_f722[0x3];
    db v_byte_1dd05; /* 0xf725 */
    db _pad_f726[0x19];
    dw v_word_1dd1f; /* 0xf73f */
    db v_byte_1dd21; /* 0xf741 */
    db _pad_f742[0x3];
    db v_byte_1dd25; /* 0xf745 */
    db _pad_f746[0x19];
    dw v_word_1dd3f; /* 0xf75f */
    db v_byte_1dd41; /* 0xf761 */
    db _pad_f762[0x3];
    db v_byte_1dd45; /* 0xf765 */
    db _pad_f766[0x19];
    dw v_word_1dd5f; /* 0xf77f */
    db _pad_f781[0x1e];
    db v_byte_1dd7f; /* 0xf79f */
    db v_byte_1dd80; /* 0xf7a0 */
    db _pad_f7a1[0x1e];
    dw v_word_1dd9f; /* 0xf7bf */
    dw v_word_1dda1; /* 0xf7c1 */
    dw v_word_1dda3; /* 0xf7c3 */
    dw v_word_1dda5; /* 0xf7c5 */
    db _pad_f7c7[0x9];
    dw v_word_1ddb0; /* 0xf7d0 */
    dw v_word_1ddb2; /* 0xf7d2 */
    dw v_word_1ddb4; /* 0xf7d4 */
    db _pad_f7d6[0x3];
    dw v_word_1ddb9; /* 0xf7d9 */
    db _pad_f7db[0x12];
    dw v_word_1ddcd; /* 0xf7ed */
    db _pad_f7ef[0x4];
    dw v_word_1ddd3; /* 0xf7f3 */
    db _pad_f7f5[0x4];
    db v_byte_1ddd9; /* 0xf7f9 */
    db _pad_f7fa[0x2c9];
    db v_byte_1e0a3; /* 0xfac3 */
    db _pad_fac4[0x8b];
    db v_byte_1e12f; /* 0xfb4f */
    db _pad_fb50[0x8d];
    dw v_word_1e1bd; /* 0xfbdd */
    dw v_word_1e1bf; /* 0xfbdf */
    dw v_word_1e1c1; /* 0xfbe1 */
    dw v_word_1e1c3; /* 0xfbe3 */
    db v_byte_1e1c5; /* 0xfbe5 */
    db _pad_fbe6[0x3];
    db v_byte_1e1c9; /* 0xfbe9 */
    db v_byte_1e1ca; /* 0xfbea */
    dw v_word_1e1cb; /* 0xfbeb */
    db v_byte_1e1cd; /* 0xfbed */
    dw v_word_1e1ce; /* 0xfbee */
    dw v_word_1e1d0; /* 0xfbf0 */
    dw v_word_1e1d2; /* 0xfbf2 */
    dw v_word_1e1d4; /* 0xfbf4 */
    db _pad_fbf6[0x66a];
    dw v_word_1e840; /* 0x10260 */
    dw v_word_1e842; /* 0x10262 */
    dw v_word_1e844; /* 0x10264 */
    dw v_word_1e846; /* 0x10266 */
    db _pad_10268[0x1e];
    db v_aallocated1mbof; /* 0x10286 */
    db _pad_10287[0x2179];
    dw v_word_209e0; /* 0x12400 */
    dw v_word_209e2; /* 0x12402 */
    db _pad_12404[0x354];
    dw v_word_20d38; /* 0x12758 */
    db _pad_1275a[0x10];
    dw v_word_20d4a; /* 0x1276a */
    db v_byte_20d4c; /* 0x1276c */
    db _pad_1276d[0x1];
    dw v_word_20d4e; /* 0x1276e */
    db v_byte_20d50; /* 0x12770 */
    db _pad_12771[0x1];
    dw v_word_20d52; /* 0x12772 */
    db v_byte_20d54; /* 0x12774 */
    db _pad_12775[0x1];
    dw v_word_20d56; /* 0x12776 */
    dw v_word_20d58; /* 0x12778 */
    db _pad_1277a[0x580a];
    dw v_word_26564; /* 0x17f84  intro_6564 */
    dw v_word_26566; /* 0x17f86  tmframe_6566 */
    db _pad_17f88[0x2];
    dw v_word_2656a; /* 0x17f8a */
    dw v_word_2656c; /* 0x17f8c */
    dw v_word_2656e; /* 0x17f8e */
    dw v_word_26570; /* 0x17f90  tmframe_6570 */
    db _pad_17f92[0x2];
    dw v_word_26574; /* 0x17f94  intro_6574 */
    db _pad_17f96[0xc];
    dw v_word_26582; /* 0x17fa2  speed_sel */
    db _pad_17fa4[0x24];
    dw v_word_265a8; /* 0x17fc8 */
    dw v_word_265aa; /* 0x17fca */
    dw v_word_265ac; /* 0x17fcc */
    dw v_word_265ae; /* 0x17fce */
    dw v_word_265b0; /* 0x17fd0 */
    dw v_word_265b2; /* 0x17fd2 */
    dw v_word_265b4; /* 0x17fd4 */
    db _pad_17fd6[0x80a];
    dw v_word_26dc0; /* 0x187e0 */
    db _pad_187e2[0xb];
    db v_byte_26dcd; /* 0x187ed */
    db _pad_187ee[0x5];
    dw v_word_26dd3; /* 0x187f3 */
    db v_byte_26dd5; /* 0x187f5 */
    db v_byte_26dd6; /* 0x187f6 */
    db v_byte_26dd7; /* 0x187f7 */
    dw v_word_26dd8; /* 0x187f8 */
    dw v_word_26dda; /* 0x187fa */
    dw v_word_26ddc; /* 0x187fc */
    dw v_word_26dde; /* 0x187fe */
    dw v_word_26de0; /* 0x18800 */
    dw v_word_26de2; /* 0x18802 */
    dw v_word_26de4; /* 0x18804 */
    db v_byte_26de6; /* 0x18806 */
    db v_byte_26de7; /* 0x18807 */
    db v_byte_26de8; /* 0x18808 */
    db v_byte_26de9; /* 0x18809 */
    db v_byte_26dea; /* 0x1880a */
    dw v_word_26deb; /* 0x1880b */
    dw v_word_26ded; /* 0x1880d */
    db v_byte_26def; /* 0x1880f */
    db v_byte_26df0; /* 0x18810 */
    db v_byte_26df1; /* 0x18811 */
    db v_byte_26df2; /* 0x18812 */
    db v_byte_26df3; /* 0x18813 */
    dw v_word_26df4; /* 0x18814 */
    dw v_word_26df6; /* 0x18816 */
    dw v_word_26df8; /* 0x18818 */
    db v_byte_26dfa; /* 0x1881a */
    db v_byte_26dfb; /* 0x1881b */
    db v_byte_26dfc; /* 0x1881c */
    db _pad_1881d[0x34c];
    db v_byte_27149; /* 0x18b69  member_idx */
    db _pad_18b6a[0x6];
    dw v_word_27150; /* 0x18b70  mission_idx */
    db v_byte_27152; /* 0x18b72  color_idx */
    db v_byte_27153; /* 0x18b73  alarm_flag */
    dw v_word_27154; /* 0x18b74  sum_acc */
    db v_byte_27156; /* 0x18b76  sum_ovf_b */
    db v_byte_27157; /* 0x18b77  mst_7157 */
    db v_byte_27158; /* 0x18b78  sum_ovf_a */
    db _pad_18b79[0xd];
    db v_byte_27166; /* 0x18b86  color_a */
    db v_byte_27167; /* 0x18b87  color_b */
    db _pad_18b88[0x1];
    dw v_word_27169; /* 0x18b89  mselprep_7169 */
    db _pad_18b8b[0x6a];
    db v_byte_271d5; /* 0x18bf5 */
    db _pad_18bf6[0x148];
    db v_byte_2731e; /* 0x18d3e */
    db _pad_18d3f[0x10];
    dw v_word_2732f; /* 0x18d4f */
    db _pad_18d51[0xd2];
    dw v_word_27403; /* 0x18e23  main_men_7403 */
    db _pad_18e25[0x22a];
    dw v_word_2762f; /* 0x1904f */
    db _pad_19051[0x3];
    dw v_word_27634; /* 0x19054  brief_sc_7634 */
    db _pad_19056[0xbb7];
    db v_byte_281ed; /* 0x19c0d */
    db v_byte_281ee; /* 0x19c0e */
    db _pad_19c0f[0x5c];
    dw v_word_2824b; /* 0x19c6b */
    db _pad_19c6d[0xf4];
    dw v_word_28341; /* 0x19d61  mseloop_8341 */
    dw v_word_28343; /* 0x19d63  menu_sel */
    dw v_word_28345; /* 0x19d65  menu_nav_8345 */
    dw v_word_28347; /* 0x19d67  mseloop_8347 */
    dw v_word_28349; /* 0x19d69  mseloop_8349 */
    db _pad_19d6b[0x2bf];
    dw v_word_2860a; /* 0x1a02a */
    db _pad_1a02c[0x6b];
    db v_byte_28677; /* 0x1a097  inmenu_8677 */
    db _pad_1a098[0xb8];
    dw v_word_28730; /* 0x1a150  menu_scr_8730 */
    db _pad_1a152[0x2b0];
    db v_byte_289e2; /* 0x1a402 */
    db v_byte_289e3; /* 0x1a403 */
    db _pad_1a404[0x42];
    db v_acom1com2bstrss; /* 0x1a446 */
    db _pad_1a447[0x29];
    db v_byte_28a50; /* 0x1a470 */
    db _pad_1a471[0x7d];
    dw v_word_28ace; /* 0x1a4ee */
    dw v_word_28ad0; /* 0x1a4f0 */
    db v_byte_28ad2; /* 0x1a4f2 */
    db _pad_1a4f3[0x1b0];
    dw v_word_28c83; /* 0x1a6a3 */
    db _pad_1a6a5[0xb];
    dw v_word_28c90; /* 0x1a6b0 */
    dw v_word_28c92; /* 0x1a6b2 */
    db v_byte_28c94; /* 0x1a6b4 */
    db v_byte_28c95; /* 0x1a6b5 */
    db v_byte_28c96; /* 0x1a6b6 */
    db v_byte_28c97; /* 0x1a6b7 */
    db _pad_1a6b8[0x8];
    db v_byte_28ca0; /* 0x1a6c0 */
    db v_byte_28ca1; /* 0x1a6c1 */
    db v_byte_28ca2; /* 0x1a6c2 */
    db v_byte_28ca3; /* 0x1a6c3 */
    db _pad_1a6c4[0x8];
    dw v_word_28cac; /* 0x1a6cc */
    db _pad_1a6ce[0x3];
    db v_byte_28cb1; /* 0x1a6d1 */
    db _pad_1a6d2[0x1];
    db v_byte_28cb3; /* 0x1a6d3 */
    db v_byte_28cb4; /* 0x1a6d4 */
    db _pad_1a6d5[0xb];
    db v_byte_28cc0; /* 0x1a6e0 */
    db v_byte_28cc1; /* 0x1a6e1 */
    db v_byte_28cc2; /* 0x1a6e2 */
    db v_byte_28cc3; /* 0x1a6e3 */
    db v_byte_28cc4; /* 0x1a6e4 */
    db v_byte_28cc5; /* 0x1a6e5 */
    db _pad_1a6e6[0x2];
    db v_byte_28cc8; /* 0x1a6e8 */
    db _pad_1a6e9[0x1];
    db v_byte_28cca; /* 0x1a6ea */
    db v_byte_28ccb; /* 0x1a6eb */
    db _pad_1a6ec[0x5];
    db v_byte_28cd1; /* 0x1a6f1 */
    db v_byte_28cd2; /* 0x1a6f2 */
    db v_byte_28cd3; /* 0x1a6f3 */
    db v_byte_28cd4; /* 0x1a6f4 */
    dw v_word_28cd5; /* 0x1a6f5 */
    db _pad_1a6f7[0x1];
    db v_byte_28cd8; /* 0x1a6f8 */
    db v_byte_28cd9; /* 0x1a6f9 */
    db _pad_1a6fa[0x1];
    db v_byte_28cdb; /* 0x1a6fb */
    db v_byte_28cdc; /* 0x1a6fc */
    db _pad_1a6fd[0x30];
    db v_byte_28d0d; /* 0x1a72d */
    db v_byte_28d0e; /* 0x1a72e */
    db _pad_1a72f[0x42];
    db v_byte_28d51; /* 0x1a771 */
    db _pad_1a772[0xa9];
    db v_byte_28dfb; /* 0x1a81b */
    db _pad_1a81c[0x21];
    db v_byte_28e1d; /* 0x1a83d */
    db _pad_1a83e[0x21];
    db v_byte_28e3f; /* 0x1a85f */
    db _pad_1a860[0x21];
    db v_byte_28e61; /* 0x1a881 */
    db _pad_1a882[0x21];
    db v_byte_28e83; /* 0x1a8a3 */
    db _pad_1a8a4[0xa9];
    db v_byte_28f2d; /* 0x1a94d */
    db _pad_1a94e[0xed];
    db v_byte_2901b; /* 0x1aa3b */
    db _pad_1aa3c[0x21];
    db v_byte_2903d; /* 0x1aa5d */
    db _pad_1aa5e[0x65];
    db v_byte_290a3; /* 0x1aac3 */
    db _pad_1aac4[0xa9];
    db v_byte_2914d; /* 0x1ab6d */
    db v_byte_2914e; /* 0x1ab6e */
    db _pad_1ab6f[0x20];
    db v_byte_2916f; /* 0x1ab8f */
    db v_byte_29170; /* 0x1ab90  csetup_9170 */
    db _pad_1ab91[0x42];
    db v_byte_291b3; /* 0x1abd3 */
    db _pad_1abd4[0x21];
    db v_byte_291d5; /* 0x1abf5 */
    db _pad_1abf6[0x43];
    db v_byte_29219; /* 0x1ac39  obj_9219 */
    db _pad_1ac3a[0x21];
    db v_byte_2923b; /* 0x1ac5b */
    db _pad_1ac5c[0xcb];
    db v_byte_29307; /* 0x1ad27  ot23_tick_9307 */
    db _pad_1ad28[0x22f];
    db v_byte_29537; /* 0x1af57  aux7_9537 */
    db v_byte_29538; /* 0x1af58  aux7_9538 */
    db v_byte_29539; /* 0x1af59  aux7_9539 */
    db _pad_1af5a[0x2d];
    db v_byte_29567; /* 0x1af87  rstate_9567 */
    db _pad_1af88[0x82];
    db v_byte_295ea; /* 0x1b00a */
    dw v_word_295eb; /* 0x1b00b */
    db _pad_1b00d[0x2];
    db v_byte_295ef; /* 0x1b00f */
    db _pad_1b010[0x78];
    dw v_word_29668; /* 0x1b088  scan_id */
    dw v_word_2966a; /* 0x1b08a */
    dw v_word_2966c; /* 0x1b08c */
    dw v_word_2966e; /* 0x1b08e */
    dw v_word_29670; /* 0x1b090  apply_sc_9670 */
    db _pad_1b092[0x4];
    dw v_word_29676; /* 0x1b096  cam_9676 */
    dw v_word_29678; /* 0x1b098  cam_9678 */
    db v_byte_2967a; /* 0x1b09a */
    dw v_word_2967b; /* 0x1b09b */
    dw v_word_2967d; /* 0x1b09d */
    db _pad_1b09f[0x2];
    dw v_word_29681; /* 0x1b0a1  cam_9681 */
    dw v_word_29683; /* 0x1b0a3  cam_org_x */
    db _pad_1b0a5[0x88];
    db v_byte_2970d; /* 0x1b12d */
    db v_byte_2970e; /* 0x1b12e */
    db v_byte_2970f; /* 0x1b12f */
    db v_byte_29710; /* 0x1b130  mst_9710 */
    db v_byte_29711; /* 0x1b131  weapon_sel */
    db v_byte_29712; /* 0x1b132  wounds */
    db _pad_1b133[0x1];
    db v_byte_29714; /* 0x1b134  flash_period */
    db v_byte_29715; /* 0x1b135  hit_flash */
    db v_byte_29716; /* 0x1b136  mst_9716 */
    db v_byte_29717; /* 0x1b137  mst_9717 */
    db v_byte_29718; /* 0x1b138  fade_cnt */
    db _pad_1b139[0x1];
    db v_byte_2971a; /* 0x1b13a */
    db _pad_1b13b[0x6];
    db v_byte_29721; /* 0x1b141  ot13_tick_9721 */
    db v_byte_29722; /* 0x1b142  mst_9722 */
    db v_byte_29723; /* 0x1b143  ot13_tick_9723 */
    db v_byte_29724; /* 0x1b144  ot14_tick_9724 */
    db _pad_1b145[0xe];
    db v_byte_29733; /* 0x1b153  roster_9733 */
    db v_byte_29734; /* 0x1b154  roster_9734 */
    db v_byte_29735; /* 0x1b155  roster_9735 */
    db v_byte_29736; /* 0x1b156  roster_9736 */
    db _pad_1b157[0x1];
    db v_byte_29738; /* 0x1b158  roster_9738 */
    db _pad_1b159[0x16];
    db v_byte_2974f; /* 0x1b16f */
    db v_byte_29750; /* 0x1b170  scan_tgt */
    db _pad_1b171[0x10];
    db v_byte_29761; /* 0x1b181  ai_dir */
    db v_byte_29762; /* 0x1b182  ai_seek__9762 */
    db v_byte_29763; /* 0x1b183  pri_best */
    db v_byte_29764; /* 0x1b184  pri_a */
    db v_byte_29765; /* 0x1b185  pri_b */
    db v_byte_29766; /* 0x1b186  pri_c */
    db v_byte_29767; /* 0x1b187  pri_fiel_9767 */
    db v_byte_29768; /* 0x1b188  ai_drive_9768 */
    db v_byte_29769; /* 0x1b189  trk_flag_b */
    db v_byte_2976a; /* 0x1b18a */
    db v_byte_2976b; /* 0x1b18b */
    db v_byte_2976c; /* 0x1b18c */
    db _pad_1b18d[0x74];
    dw v_word_297e1; /* 0x1b201 */
    db _pad_1b203[0x10];
    dw v_word_297f3; /* 0x1b213 */
    db _pad_1b215[0x1d];
    dw v_word_29812; /* 0x1b232  tmap_9812 */
    dw v_word_29814; /* 0x1b234  cam_px_x */
    dw v_word_29816; /* 0x1b236  cam_px_y */
    dw v_word_29818; /* 0x1b238  burst_dir */
    db _pad_1b23a[0x2b];
    dw v_word_29845; /* 0x1b265  udraw_9845 */
    db v_byte_29847; /* 0x1b267  udraw_9847 */
    db v_byte_29848; /* 0x1b268  udraw_9848 */
    db _pad_1b269[0x2];
    db v_byte_2984b; /* 0x1b26b */
    db v_byte_2984c; /* 0x1b26c */
    db v_byte_2984d; /* 0x1b26d */
    db v_byte_2984e; /* 0x1b26e */
    db v_byte_2984f; /* 0x1b26f */
    db _pad_1b270[0x52];
    dw v_word_298a2; /* 0x1b2c2 */
    dw v_word_298a4; /* 0x1b2c4 */
    db v_byte_298a6; /* 0x1b2c6 */
    db _pad_1b2c7[0x5];
    dw v_word_298ac; /* 0x1b2cc */
    dw v_word_298ae; /* 0x1b2ce */
    db _pad_1b2d0[0x2];
    dw v_word_298b2; /* 0x1b2d2 */
    db _pad_1b2d4[0xc];
    dw v_word_298c0; /* 0x1b2e0 */
    dw v_word_298c2; /* 0x1b2e2 */
    db v_byte_298c4; /* 0x1b2e4 */
    db _pad_1b2e5[0x90];
    dw v_word_29955; /* 0x1b375  facing_c_9955 */
    db _pad_1b377[0x38];
    db v_byte_2998f; /* 0x1b3af */
    db _pad_1b3b0[0x9];
    db v_byte_29999; /* 0x1b3b9  self_id */
    db v_byte_2999a; /* 0x1b3ba */
    db _pad_1b3bb[0x14];
    db v_byte_299af; /* 0x1b3cf */
    db _pad_1b3d0[0x192];
    dw v_word_29b42; /* 0x1b562 */
    dw v_word_29b44; /* 0x1b564 */
    db _pad_1b566[0x9];
    db v_byte_29b4f; /* 0x1b56f */
    db v_byte_29b50; /* 0x1b570 */
    db v_byte_29b51; /* 0x1b571 */
    dw v_word_29b52; /* 0x1b572 */
    dw v_word_29b54; /* 0x1b574 */
    dw v_word_29b56; /* 0x1b576 */
    dw v_word_29b58; /* 0x1b578 */
    db v_byte_29b5a; /* 0x1b57a */
    db v_byte_29b5b; /* 0x1b57b */
    db v_byte_29b5c; /* 0x1b57c */
    db v_byte_29b5d; /* 0x1b57d */
    db v_byte_29b5e; /* 0x1b57e */
    db v_byte_29b5f; /* 0x1b57f */
    db v_byte_29b60; /* 0x1b580 */
    db v_byte_29b61; /* 0x1b581 */
    db v_byte_29b62; /* 0x1b582 */
    dw v_word_29b63; /* 0x1b583 */
    db _pad_1b585[0x30];
    dw v_word_29b95; /* 0x1b5b5 */
    dw v_word_29b97; /* 0x1b5b7 */
    db _pad_1b5b9[0x29d];
    dw v_word_29e36; /* 0x1b856 */
    dw v_word_29e38; /* 0x1b858 */
    dw v_word_29e3a; /* 0x1b85a */
    dw v_word_29e3c; /* 0x1b85c */
    dw v_word_29e3e; /* 0x1b85e */
    dw v_word_29e40; /* 0x1b860 */
    db _pad_1b862[0x7a];
    dw v_word_29ebc; /* 0x1b8dc */
    db _pad_1b8de[0xa0];
    db v_byte_29f5e; /* 0x1b97e */
    db _pad_1b97f[0x1];
    dw v_word_29f60; /* 0x1b980 */
    db _pad_1b982[0x2];
    dw v_word_29f64; /* 0x1b984 */
    dw v_word_29f66; /* 0x1b986 */
    db _pad_1b988[0xe];
    db v_byte_29f76; /* 0x1b996 */
    db v_byte_29f77; /* 0x1b997 */
    dw v_word_29f78; /* 0x1b998 */
    db _pad_1b99a[0x163];
    dw v_word_2a0dd; /* 0x1bafd */
    db _pad_1baff[0xb];
    dw v_word_2a0ea; /* 0x1bb0a */
    db _pad_1bb0c[0x21];
    dw v_word_2a10d; /* 0x1bb2d */
    db _pad_1bb2f[0x1b];
    dw v_word_2a12a; /* 0x1bb4a */
    db _pad_1bb4c[0x1b];
    dw v_word_2a147; /* 0x1bb67 */
    db _pad_1bb69[0x1c4];
    db v_byte_2a30d; /* 0x1bd2d */
    db _pad_1bd2e[0x31];
    db v_byte_2a33f; /* 0x1bd5f */
    db _pad_1bd60[0x70];
    db v_byte_2a3b0; /* 0x1bdd0 */
    db _pad_1bdd1[0x4f];
    db v_byte_2a400; /* 0x1be20 */
    db _pad_1be21[0x40];
    db v_byte_2a441; /* 0x1be61 */
    db _pad_1be62[0x88];
    db v_byte_2a4ca; /* 0x1beea */
    db _pad_1beeb[0x3b];
    db v_byte_2a506; /* 0x1bf26 */
    dw v_word_2a507; /* 0x1bf27 */
    dw v_word_2a509; /* 0x1bf29 */
    db _pad_1bf2b[0x98];
    dw v_word_2a5a3; /* 0x1bfc3 */
    dw v_word_2a5a5; /* 0x1bfc5 */
    dw v_word_2a5a7; /* 0x1bfc7 */
    db _pad_1bfc9[0x7];
    db v_byte_2a5b0; /* 0x1bfd0 */
    db v_byte_2a5b1; /* 0x1bfd1 */
    dw v_word_2a5b2; /* 0x1bfd2 */
    dw v_word_2a5b4; /* 0x1bfd4 */
    dw v_word_2a5b6; /* 0x1bfd6 */
    db _pad_1bfd8[0x36];
    db v_byte_2a5ee; /* 0x1c00e */
    db v_byte_2a5ef; /* 0x1c00f */
    db v_byte_2a5f0; /* 0x1c010 */
    db v_byte_2a5f1; /* 0x1c011 */
    db v_byte_2a5f2; /* 0x1c012 */
    db v_byte_2a5f3; /* 0x1c013 */
    db v_byte_2a5f4; /* 0x1c014 */
    db v_byte_2a5f5; /* 0x1c015 */
    db v_byte_2a5f6; /* 0x1c016 */
    db v_byte_2a5f7; /* 0x1c017 */
    db v_byte_2a5f8; /* 0x1c018 */
    db v_byte_2a5f9; /* 0x1c019 */
    db v_byte_2a5fa; /* 0x1c01a */
    db v_byte_2a5fb; /* 0x1c01b */
    db v_byte_2a5fc; /* 0x1c01c */
    dw v_word_2a5fd; /* 0x1c01d */
    dw v_word_2a5ff; /* 0x1c01f */
    db v_byte_2a601; /* 0x1c021 */
    db v_byte_2a602; /* 0x1c022 */
    db v_byte_2a603; /* 0x1c023 */
    db v_byte_2a604; /* 0x1c024 */
    db v_byte_2a605; /* 0x1c025 */
    db v_byte_2a606; /* 0x1c026 */
    db _pad_1c027[0x8];
    db v_byte_2a60f; /* 0x1c02f */
    dw v_word_2a610; /* 0x1c030 */
    db v_byte_2a612; /* 0x1c032 */
    db v_byte_2a613; /* 0x1c033 */
    db v_byte_2a614; /* 0x1c034 */
    db v_byte_2a615; /* 0x1c035 */
    db _pad_1c036[0x99];
    db v_byte_2a6af; /* 0x1c0cf */
    db v_byte_2a6b0; /* 0x1c0d0 */
    db v_byte_2a6b1; /* 0x1c0d1 */
    db v_byte_2a6b2; /* 0x1c0d2 */
    db v_byte_2a6b3; /* 0x1c0d3 */
    db v_byte_2a6b4; /* 0x1c0d4 */
    db v_byte_2a6b5; /* 0x1c0d5 */
    db _pad_1c0d6[0x8];
    db v_byte_2a6be; /* 0x1c0de */
    dw v_word_2a6bf; /* 0x1c0df */
    db v_byte_2a6c1; /* 0x1c0e1 */
    db v_byte_2a6c2; /* 0x1c0e2 */
    db v_byte_2a6c3; /* 0x1c0e3 */
    db v_byte_2a6c4; /* 0x1c0e4 */
    db v_byte_2a6c5; /* 0x1c0e5 */
    db v_byte_2a6c6; /* 0x1c0e6 */
    db _pad_1c0e7[0x84];
    db v_byte_2a74b; /* 0x1c16b */
    db _pad_1c16c[0x5c];
    dw v_word_2a7a8; /* 0x1c1c8 */
    db v_byte_2a7aa; /* 0x1c1ca */
    db v_byte_2a7ab; /* 0x1c1cb */
    db v_byte_2a7ac; /* 0x1c1cc */
    db v_byte_2a7ad; /* 0x1c1cd */
    db v_byte_2a7ae; /* 0x1c1ce */
    db v_byte_2a7af; /* 0x1c1cf */
    dw v_word_2a7b0; /* 0x1c1d0 */
    db _pad_1c1d2[0xb9];
    db v_byte_2a86b; /* 0x1c28b */
    db v_byte_2a86c; /* 0x1c28c */
    db _pad_1c28d[0x2f];
    dw v_word_2a89c; /* 0x1c2bc */
    dw v_word_2a89e; /* 0x1c2be */
    dw v_word_2a8a0; /* 0x1c2c0 */
    dw v_word_2a8a2; /* 0x1c2c2 */
    db _pad_1c2c4[0xc];
    db v_byte_2a8b0; /* 0x1c2d0 */
    db _pad_1c2d1[0x59];
    db v_byte_2a90a; /* 0x1c32a */
    db v_a0; /* 0x1c32b */
    db _pad_1c32c[0x27];
    dw v_word_2a933; /* 0x1c353 */
    db _pad_1c355[0x2b];
    db v_byte_2a960; /* 0x1c380 */
    db v_byte_2a961; /* 0x1c381 */
    dw v_word_2a962; /* 0x1c382 */
    db _pad_1c384[0x4];
    dw v_word_2a968; /* 0x1c388 */
    dw v_word_2a96a; /* 0x1c38a */
    db v_byte_2a96c; /* 0x1c38c */
    db _pad_1c38d[0x30];
    dw v_word_2a99d; /* 0x1c3bd */
    db v_byte_2a99f; /* 0x1c3bf */
    db _pad_1c3c0[0x2a];
    db v_byte_2a9ca; /* 0x1c3ea */
    db v_byte_2a9cb; /* 0x1c3eb */
    db _pad_1c3ec[0x1];
    db v_byte_2a9cd; /* 0x1c3ed */
    dw v_word_2a9ce; /* 0x1c3ee */
    dw v_word_2a9d0; /* 0x1c3f0 */
    db v_byte_2a9d2; /* 0x1c3f2 */
    db v_byte_2a9d3; /* 0x1c3f3 */
    dw v_word_2a9d4; /* 0x1c3f4 */
    dw v_word_2a9d6; /* 0x1c3f6 */
    db v_byte_2a9d8; /* 0x1c3f8 */
    db _pad_1c3f9[0x6];
    db v_byte_2a9df; /* 0x1c3ff */
    db v_byte_2a9e0; /* 0x1c400 */
    db v_byte_2a9e1; /* 0x1c401 */
    db v_byte_2a9e2; /* 0x1c402 */
    db v_byte_2a9e3; /* 0x1c403 */
    db v_byte_2a9e4; /* 0x1c404 */
    db _pad_1c405[0x14];
    db v_byte_2a9f9; /* 0x1c419 */
    db v_byte_2a9fa; /* 0x1c41a */
    dw v_word_2a9fb; /* 0x1c41b */
    db _pad_1c41d[0x10];
    dw v_word_2aa0d; /* 0x1c42d */
    db _pad_1c42f[0x31];
    dw v_word_2aa40; /* 0x1c460 */
    dw v_word_2aa42; /* 0x1c462 */
    dw v_word_2aa44; /* 0x1c464 */
    dw v_word_2aa46; /* 0x1c466 */
    dw v_word_2aa48; /* 0x1c468 */
    dw v_word_2aa4a; /* 0x1c46a */
    db _pad_1c46c[0xa];
    dw v_word_2aa56; /* 0x1c476 */
    dw v_word_2aa58; /* 0x1c478 */
    db _pad_1c47a[0x2];
    db v_byte_2aa5c; /* 0x1c47c */
    db v_byte_2aa5d; /* 0x1c47d */
    db v_byte_2aa5e; /* 0x1c47e */
    db v_byte_2aa5f; /* 0x1c47f */
    db v_byte_2aa60; /* 0x1c480 */
    db v_byte_2aa61; /* 0x1c481 */
    db v_byte_2aa62; /* 0x1c482 */
    db v_byte_2aa63; /* 0x1c483 */
    db v_byte_2aa64; /* 0x1c484 */
    dw v_word_2aa65; /* 0x1c485 */
    dw v_word_2aa67; /* 0x1c487 */
    dw v_word_2aa69; /* 0x1c489 */
    dw v_word_2aa6b; /* 0x1c48b */
    db v_byte_2aa6d; /* 0x1c48d */
    db v_byte_2aa6e; /* 0x1c48e */
    db v_byte_2aa6f; /* 0x1c48f */
    db _pad_1c490[0x600];
    dw v_word_2b070; /* 0x1ca90 */
    db _pad_1ca92[0x2];
    dw v_word_2b074; /* 0x1ca94 */
    db v_byte_2b076; /* 0x1ca96 */
    db _pad_1ca97[0x4c];
    db v_byte_2b0c3; /* 0x1cae3 */
    dw v_word_2b0c4; /* 0x1cae4 */
    db v_byte_2b0c6; /* 0x1cae6 */
    db v_byte_2b0c7; /* 0x1cae7 */
    dw v_word_2b0c8; /* 0x1cae8 */
    db v_byte_2b0ca; /* 0x1caea */
    dw v_word_2b0cb; /* 0x1caeb */
    dw v_word_2b0cd; /* 0x1caed */
    db v_byte_2b0cf; /* 0x1caef */
    db v_byte_2b0d0; /* 0x1caf0 */
    db v_byte_2b0d1; /* 0x1caf1 */
    db v_byte_2b0d2; /* 0x1caf2 */
    db v_byte_2b0d3; /* 0x1caf3 */
    db v_byte_2b0d4; /* 0x1caf4 */
    db _pad_1caf5[0x40];
    db v_byte_2b115; /* 0x1cb35 */
    db v_byte_2b116; /* 0x1cb36 */
    db v_byte_2b117; /* 0x1cb37 */
    db v_byte_2b118; /* 0x1cb38 */
    db v_byte_2b119; /* 0x1cb39 */
    db v_byte_2b11a; /* 0x1cb3a */
    dw v_word_2b11b; /* 0x1cb3b */
    db v_byte_2b11d; /* 0x1cb3d */
    db v_byte_2b11e; /* 0x1cb3e */
    db v_byte_2b11f; /* 0x1cb3f */
    db v_byte_2b120; /* 0x1cb40 */
    dw v_word_2b121; /* 0x1cb41 */
    db v_byte_2b123; /* 0x1cb43 */
    db _pad_1cb44[0x2];
    db v_byte_2b126; /* 0x1cb46 */
    db v_byte_2b127; /* 0x1cb47 */
    db v_byte_2b128; /* 0x1cb48 */
    db v_byte_2b129; /* 0x1cb49 */
    db v_byte_2b12a; /* 0x1cb4a */
    db v_byte_2b12b; /* 0x1cb4b */
    db v_byte_2b12c; /* 0x1cb4c */
    db v_byte_2b12d; /* 0x1cb4d */
    db v_byte_2b12e; /* 0x1cb4e */
    db v_byte_2b12f; /* 0x1cb4f */
    db v_byte_2b130; /* 0x1cb50 */
    db v_byte_2b131; /* 0x1cb51 */
    db v_byte_2b132; /* 0x1cb52 */
    db _pad_1cb53[0x109];
    db v_byte_2b23c; /* 0x1cc5c */
    db v_byte_2b23d; /* 0x1cc5d */
    dw v_word_2b23e; /* 0x1cc5e */
    dw v_word_2b240; /* 0x1cc60 */
    db v_byte_2b242; /* 0x1cc62 */
    db v_byte_2b243; /* 0x1cc63 */
    db v_byte_2b244; /* 0x1cc64 */
    dw v_word_2b245; /* 0x1cc65 */
    dw v_word_2b247; /* 0x1cc67 */
    db _pad_1cc69[0x4];
    db v_byte_2b24d; /* 0x1cc6d */
    db _pad_1cc6e[0x1];
    dw v_word_2b24f; /* 0x1cc6f */
    dw v_word_2b251; /* 0x1cc71 */
    dw v_word_2b253; /* 0x1cc73 */
    db _pad_1cc75[0x79];
    db v_byte_2b2ce; /* 0x1ccee */
    db _pad_1ccef[0x127];
    db v_byte_2b3f6; /* 0x1ce16 */
    db v_byte_2b3f7; /* 0x1ce17 */
    db _pad_1ce18[0x118];
    db v_byte_2b510; /* 0x1cf30 */
};

/* decompress_res scratch at ds:7FF0 — ds-relative (see
 * gen_port.py DSREL note), so it gets its own overlay. */
struct dcomp_state {
    union { db lim; db v_byte_24e70; }; /* ds:0x7ff0 */
    union { db maxw; db v_byte_24e71; }; /* ds:0x7ff1 */
    union { dw mask; dw v_word_24e72; }; /* ds:0x7ff2 */
    union { dw acc; dw v_word_24e74; }; /* ds:0x7ff4 */
    union { db npending; db v_byte_24e76; }; /* ds:0x7ff6 */
    union { dw end; dw v_word_24e77; }; /* ds:0x7ff7 */
    union { dw prev; dw v_word_24e79; }; /* ds:0x7ff9 */
    union { db first; db v_byte_24e7b; }; /* ds:0x7ffb */
};
#pragma pack(pop)

#define DSEG  ((volatile struct ar_dseg *)mem)
#define DCOMP ((volatile struct dcomp_state*)raddr_(ds,0x7ff0))

_Static_assert(offsetof(struct ar_dseg, v_seg_10000) == 0x1a20, "seg_10000");
_Static_assert(offsetof(struct ar_dseg, v_seg_10002) == 0x1a22, "seg_10002");
_Static_assert(offsetof(struct ar_dseg, v_seg_10004) == 0x1a24, "seg_10004");
_Static_assert(offsetof(struct ar_dseg, v_seg_1000e) == 0x1a2e, "seg_1000E");
_Static_assert(offsetof(struct ar_dseg, v_seg_10010) == 0x1a30, "seg_10010");
_Static_assert(offsetof(struct ar_dseg, v_seg_10012) == 0x1a32, "seg_10012");
_Static_assert(offsetof(struct ar_dseg, v_seg_10014) == 0x1a34, "seg_10014");
_Static_assert(offsetof(struct ar_dseg, v_seg_10016) == 0x1a36, "seg_10016");
_Static_assert(offsetof(struct ar_dseg, v_seg_10018) == 0x1a38, "seg_10018");
_Static_assert(offsetof(struct ar_dseg, v_word_100c3) == 0x1ae3, "word_100C3");
_Static_assert(offsetof(struct ar_dseg, v_jpt_1039d) == 0x1dad, "jpt_1039D");
_Static_assert(offsetof(struct ar_dseg, v_jpt_107b6) == 0x21ad, "jpt_107B6");
_Static_assert(offsetof(struct ar_dseg, v_funcs_1083a) == 0x21db, "funcs_1083A");
_Static_assert(offsetof(struct ar_dseg, v_jpt_10ad3) == 0x24d5, "jpt_10AD3");
_Static_assert(offsetof(struct ar_dseg, v_jpt_10b34) == 0x2536, "jpt_10B34");
_Static_assert(offsetof(struct ar_dseg, v_jpt_10b57) == 0x2559, "jpt_10B57");
_Static_assert(offsetof(struct ar_dseg, v_jpt_10bef) == 0x25ec, "jpt_10BEF");
_Static_assert(offsetof(struct ar_dseg, v_jpt_10c4f) == 0x264c, "jpt_10C4F");
_Static_assert(offsetof(struct ar_dseg, v_jpt_113e3) == 0x2de2, "jpt_113E3");
_Static_assert(offsetof(struct ar_dseg, v_funcs_115e7) == 0x2fdd, "funcs_115E7");
_Static_assert(offsetof(struct ar_dseg, v_jpt_1164f) == 0x3059, "jpt_1164F");
_Static_assert(offsetof(struct ar_dseg, v_jpt_1171c) == 0x3121, "jpt_1171C");
_Static_assert(offsetof(struct ar_dseg, v_jpt_11999) == 0x3393, "jpt_11999");
_Static_assert(offsetof(struct ar_dseg, v_jpt_11a25) == 0x342f, "jpt_11A25");
_Static_assert(offsetof(struct ar_dseg, v_jpt_11bc9) == 0x35d2, "jpt_11BC9");
_Static_assert(offsetof(struct ar_dseg, v_jpt_11c4e) == 0x3658, "jpt_11C4E");
_Static_assert(offsetof(struct ar_dseg, v_funcs_11cb8) == 0x3680, "funcs_11CB8");
_Static_assert(offsetof(struct ar_dseg, v_byte_11cf3) == 0x3713, "byte_11CF3");
_Static_assert(offsetof(struct ar_dseg, v_funcs_11d52) == 0x3714, "funcs_11D52");
_Static_assert(offsetof(struct ar_dseg, v_jpt_126d5) == 0x40cf, "jpt_126D5");
_Static_assert(offsetof(struct ar_dseg, v_word_126da) == 0x40fa, "word_126DA");
_Static_assert(offsetof(struct ar_dseg, v_word_126dc) == 0x40fc, "word_126DC");
_Static_assert(offsetof(struct ar_dseg, v_jpt_1367d) == 0x5087, "jpt_1367D");
_Static_assert(offsetof(struct ar_dseg, v_word_13a90) == 0x54b0, "word_13A90");
_Static_assert(offsetof(struct ar_dseg, v_word_13a92) == 0x54b2, "word_13A92");
_Static_assert(offsetof(struct ar_dseg, v_word_13bd0) == 0x55f0, "word_13BD0");
_Static_assert(offsetof(struct ar_dseg, v_funcs_13bf8) == 0x5600, "funcs_13BF8");
_Static_assert(offsetof(struct ar_dseg, v_funcs_13cc9) == 0x56d1, "funcs_13CC9");
_Static_assert(offsetof(struct ar_dseg, v_funcs_13da1) == 0x57a9, "funcs_13DA1");
_Static_assert(offsetof(struct ar_dseg, v_funcs_13e72) == 0x587a, "funcs_13E72");
_Static_assert(offsetof(struct ar_dseg, v_funcs_13f48) == 0x5950, "funcs_13F48");
_Static_assert(offsetof(struct ar_dseg, v_funcs_14021) == 0x5a29, "funcs_14021");
_Static_assert(offsetof(struct ar_dseg, v_funcs_140fa) == 0x5b02, "funcs_140FA");
_Static_assert(offsetof(struct ar_dseg, v_funcs_141ce) == 0x5bd6, "funcs_141CE");
_Static_assert(offsetof(struct ar_dseg, v_funcs_159e9) == 0x73b0, "funcs_159E9");
_Static_assert(offsetof(struct ar_dseg, v_jpt_16ab9) == 0x84b1, "jpt_16AB9");
_Static_assert(offsetof(struct ar_dseg, v_jpt_17c64) == 0x963d, "jpt_17C64");
_Static_assert(offsetof(struct ar_dseg, v_funcs_1892f) == 0xa2f0, "funcs_1892F");
_Static_assert(offsetof(struct ar_dseg, v_funcs_1a9c8) == 0xc3d0, "funcs_1A9C8");
_Static_assert(offsetof(struct ar_dseg, v_jpt_1b098) == 0xcaa6, "jpt_1B098");
_Static_assert(offsetof(struct ar_dseg, v_funcs_1b3e2) == 0xcdf0, "funcs_1B3E2");
_Static_assert(offsetof(struct ar_dseg, v_funcs_1b41f) == 0xce1a, "funcs_1B41F");
_Static_assert(offsetof(struct ar_dseg, v_jpt_1b475) == 0xce77, "jpt_1B475");
_Static_assert(offsetof(struct ar_dseg, v_word_1ce80) == 0xe8a0, "word_1CE80");
_Static_assert(offsetof(struct ar_dseg, v_byte_1ce90) == 0xe8b0, "byte_1CE90");
_Static_assert(offsetof(struct ar_dseg, v_word_1ce92) == 0xe8b2, "word_1CE92");
_Static_assert(offsetof(struct ar_dseg, v_word_1ce94) == 0xe8b4, "word_1CE94");
_Static_assert(offsetof(struct ar_dseg, v_word_1ce96) == 0xe8b6, "word_1CE96");
_Static_assert(offsetof(struct ar_dseg, v_word_1ce98) == 0xe8b8, "word_1CE98");
_Static_assert(offsetof(struct ar_dseg, v_word_1ce9c) == 0xe8bc, "word_1CE9C");
_Static_assert(offsetof(struct ar_dseg, v_word_1cea0) == 0xe8c0, "word_1CEA0");
_Static_assert(offsetof(struct ar_dseg, v_byte_1cea2) == 0xe8c2, "byte_1CEA2");
_Static_assert(offsetof(struct ar_dseg, v_word_1cea3) == 0xe8c3, "word_1CEA3");
_Static_assert(offsetof(struct ar_dseg, v_word_1cea5) == 0xe8c5, "word_1CEA5");
_Static_assert(offsetof(struct ar_dseg, v_word_1ceb2) == 0xe8d2, "word_1CEB2");
_Static_assert(offsetof(struct ar_dseg, v_word_1ceb4) == 0xe8d4, "word_1CEB4");
_Static_assert(offsetof(struct ar_dseg, v_word_1ceb6) == 0xe8d6, "word_1CEB6");
_Static_assert(offsetof(struct ar_dseg, v_word_1cff2) == 0xea12, "word_1CFF2");
_Static_assert(offsetof(struct ar_dseg, v_word_1cff4) == 0xea14, "word_1CFF4");
_Static_assert(offsetof(struct ar_dseg, v_word_1cff6) == 0xea16, "word_1CFF6");
_Static_assert(offsetof(struct ar_dseg, v_word_1d004) == 0xea24, "word_1D004");
_Static_assert(offsetof(struct ar_dseg, v_word_1d006) == 0xea26, "word_1D006");
_Static_assert(offsetof(struct ar_dseg, v_word_1d008) == 0xea28, "word_1D008");
_Static_assert(offsetof(struct ar_dseg, v_word_1d00a) == 0xea2a, "word_1D00A");
_Static_assert(offsetof(struct ar_dseg, v_word_1d00c) == 0xea2c, "word_1D00C");
_Static_assert(offsetof(struct ar_dseg, v_word_1d00e) == 0xea2e, "word_1D00E");
_Static_assert(offsetof(struct ar_dseg, v_word_1d010) == 0xea30, "word_1D010");
_Static_assert(offsetof(struct ar_dseg, v_word_1d012) == 0xea32, "word_1D012");
_Static_assert(offsetof(struct ar_dseg, v_word_1d014) == 0xea34, "word_1D014");
_Static_assert(offsetof(struct ar_dseg, v_word_1d016) == 0xea36, "word_1D016");
_Static_assert(offsetof(struct ar_dseg, v_word_1d018) == 0xea38, "word_1D018");
_Static_assert(offsetof(struct ar_dseg, v_word_1d01a) == 0xea3a, "word_1D01A");
_Static_assert(offsetof(struct ar_dseg, v_afarmchrdtx) == 0xec94, "aFarmchrDtx");
_Static_assert(offsetof(struct ar_dseg, v_aarcchrdtx) == 0xecc4, "aArcchrDtx");
_Static_assert(offsetof(struct ar_dseg, v_amagchrdtx) == 0xecf4, "aMaGchrDtx");
_Static_assert(offsetof(struct ar_dseg, v_apcoltandtx) == 0xed6c, "aPColtanDtx");
_Static_assert(offsetof(struct ar_dseg, v_apcolegadtx) == 0xed84, "aPColegaDtx");
_Static_assert(offsetof(struct ar_dseg, v_apcolmcgdtx) == 0xed9c, "aPColmcgDtx");
_Static_assert(offsetof(struct ar_dseg, v_apcolhrcdtx) == 0xedb4, "aPColhrcDtx");
_Static_assert(offsetof(struct ar_dseg, v_apribbondtx) == 0xedcc, "aPRibbonDtx");
_Static_assert(offsetof(struct ar_dseg, v_acfnegchrdtx) == 0xeed4, "aCfnegchrDtx");
_Static_assert(offsetof(struct ar_dseg, v_acanegchrdtx) == 0xef04, "aCanegchrDtx");
_Static_assert(offsetof(struct ar_dseg, v_acidadat) == 0xef34, "aCidADat");
_Static_assert(offsetof(struct ar_dseg, v_word_1d6d8) == 0xf0f8, "word_1D6D8");
_Static_assert(offsetof(struct ar_dseg, v_word_1d6da) == 0xf0fa, "word_1D6DA");
_Static_assert(offsetof(struct ar_dseg, v_byte_1d6dc) == 0xf0fc, "byte_1D6DC");
_Static_assert(offsetof(struct ar_dseg, v_byte_1d6dd) == 0xf0fd, "byte_1D6DD");
_Static_assert(offsetof(struct ar_dseg, v_word_1d6de) == 0xf0fe, "word_1D6DE");
_Static_assert(offsetof(struct ar_dseg, v_word_1d6e2) == 0xf102, "word_1D6E2");
_Static_assert(offsetof(struct ar_dseg, v_word_1d6e4) == 0xf104, "word_1D6E4");
_Static_assert(offsetof(struct ar_dseg, v_word_1d7c0) == 0xf1e0, "word_1D7C0");
_Static_assert(offsetof(struct ar_dseg, v_word_1d8d0) == 0xf2f0, "word_1D8D0");
_Static_assert(offsetof(struct ar_dseg, v_word_1d8d2) == 0xf2f2, "word_1D8D2");
_Static_assert(offsetof(struct ar_dseg, v_word_1d8d4) == 0xf2f4, "word_1D8D4");
_Static_assert(offsetof(struct ar_dseg, v_word_1d8d6) == 0xf2f6, "word_1D8D6");
_Static_assert(offsetof(struct ar_dseg, v_word_1d8d8) == 0xf2f8, "word_1D8D8");
_Static_assert(offsetof(struct ar_dseg, v_word_1d8ea) == 0xf30a, "word_1D8EA");
_Static_assert(offsetof(struct ar_dseg, v_word_1d8ec) == 0xf30c, "word_1D8EC");
_Static_assert(offsetof(struct ar_dseg, v_word_1d8f6) == 0xf316, "word_1D8F6");
_Static_assert(offsetof(struct ar_dseg, v_word_1d8f8) == 0xf318, "word_1D8F8");
_Static_assert(offsetof(struct ar_dseg, v_word_1d900) == 0xf320, "word_1D900");
_Static_assert(offsetof(struct ar_dseg, v_word_1d902) == 0xf322, "word_1D902");
_Static_assert(offsetof(struct ar_dseg, v_word_1d916) == 0xf336, "word_1D916");
_Static_assert(offsetof(struct ar_dseg, v_word_1d918) == 0xf338, "word_1D918");
_Static_assert(offsetof(struct ar_dseg, v_word_1d91c) == 0xf33c, "word_1D91C");
_Static_assert(offsetof(struct ar_dseg, v_word_1d91e) == 0xf33e, "word_1D91E");
_Static_assert(offsetof(struct ar_dseg, v_word_1d920) == 0xf340, "word_1D920");
_Static_assert(offsetof(struct ar_dseg, v_word_1d92c) == 0xf34c, "word_1D92C");
_Static_assert(offsetof(struct ar_dseg, v_word_1d92e) == 0xf34e, "word_1D92E");
_Static_assert(offsetof(struct ar_dseg, v_word_1d930) == 0xf350, "word_1D930");
_Static_assert(offsetof(struct ar_dseg, v_word_1d934) == 0xf354, "word_1D934");
_Static_assert(offsetof(struct ar_dseg, v_byte_1d936) == 0xf356, "byte_1D936");
_Static_assert(offsetof(struct ar_dseg, v_word_1d937) == 0xf357, "word_1D937");
_Static_assert(offsetof(struct ar_dseg, v_byte_1d939) == 0xf359, "byte_1D939");
_Static_assert(offsetof(struct ar_dseg, v_byte_1d93a) == 0xf35a, "byte_1D93A");
_Static_assert(offsetof(struct ar_dseg, v_byte_1d93b) == 0xf35b, "byte_1D93B");
_Static_assert(offsetof(struct ar_dseg, v_byte_1d93e) == 0xf35e, "byte_1D93E");
_Static_assert(offsetof(struct ar_dseg, v_word_1d93f) == 0xf35f, "word_1D93F");
_Static_assert(offsetof(struct ar_dseg, v_word_1d943) == 0xf363, "word_1D943");
_Static_assert(offsetof(struct ar_dseg, v_word_1d945) == 0xf365, "word_1D945");
_Static_assert(offsetof(struct ar_dseg, v_word_1d949) == 0xf369, "word_1D949");
_Static_assert(offsetof(struct ar_dseg, v_word_1d94b) == 0xf36b, "word_1D94B");
_Static_assert(offsetof(struct ar_dseg, v_word_1d94d) == 0xf36d, "word_1D94D");
_Static_assert(offsetof(struct ar_dseg, v_word_1d94f) == 0xf36f, "word_1D94F");
_Static_assert(offsetof(struct ar_dseg, v_word_1d951) == 0xf371, "word_1D951");
_Static_assert(offsetof(struct ar_dseg, v_word_1d953) == 0xf373, "word_1D953");
_Static_assert(offsetof(struct ar_dseg, v_word_1d955) == 0xf375, "word_1D955");
_Static_assert(offsetof(struct ar_dseg, v_word_1d957) == 0xf377, "word_1D957");
_Static_assert(offsetof(struct ar_dseg, v_word_1d959) == 0xf379, "word_1D959");
_Static_assert(offsetof(struct ar_dseg, v_word_1d95d) == 0xf37d, "word_1D95D");
_Static_assert(offsetof(struct ar_dseg, v_word_1d95f) == 0xf37f, "word_1D95F");
_Static_assert(offsetof(struct ar_dseg, v_word_1d961) == 0xf381, "word_1D961");
_Static_assert(offsetof(struct ar_dseg, v_word_1d965) == 0xf385, "word_1D965");
_Static_assert(offsetof(struct ar_dseg, v_word_1d969) == 0xf389, "word_1D969");
_Static_assert(offsetof(struct ar_dseg, v_word_1dbdd) == 0xf5fd, "word_1DBDD");
_Static_assert(offsetof(struct ar_dseg, v_byte_1dc65) == 0xf685, "byte_1DC65");
_Static_assert(offsetof(struct ar_dseg, v_word_1dc67) == 0xf687, "word_1DC67");
_Static_assert(offsetof(struct ar_dseg, v_word_1dc69) == 0xf689, "word_1DC69");
_Static_assert(offsetof(struct ar_dseg, v_word_1dc6b) == 0xf68b, "word_1DC6B");
_Static_assert(offsetof(struct ar_dseg, v_word_1dc6d) == 0xf68d, "word_1DC6D");
_Static_assert(offsetof(struct ar_dseg, v_word_1dc6f) == 0xf68f, "word_1DC6F");
_Static_assert(offsetof(struct ar_dseg, v_word_1dc71) == 0xf691, "word_1DC71");
_Static_assert(offsetof(struct ar_dseg, v_word_1dc73) == 0xf693, "word_1DC73");
_Static_assert(offsetof(struct ar_dseg, v_word_1dc75) == 0xf695, "word_1DC75");
_Static_assert(offsetof(struct ar_dseg, v_word_1dc77) == 0xf697, "word_1DC77");
_Static_assert(offsetof(struct ar_dseg, v_byte_1dc79) == 0xf699, "byte_1DC79");
_Static_assert(offsetof(struct ar_dseg, v_byte_1dc7a) == 0xf69a, "byte_1DC7A");
_Static_assert(offsetof(struct ar_dseg, v_byte_1dc7d) == 0xf69d, "byte_1DC7D");
_Static_assert(offsetof(struct ar_dseg, v_word_1dc80) == 0xf6a0, "word_1DC80");
_Static_assert(offsetof(struct ar_dseg, v_word_1dc82) == 0xf6a2, "word_1DC82");
_Static_assert(offsetof(struct ar_dseg, v_word_1dc86) == 0xf6a6, "word_1DC86");
_Static_assert(offsetof(struct ar_dseg, v_word_1dc88) == 0xf6a8, "word_1DC88");
_Static_assert(offsetof(struct ar_dseg, v_word_1dc8c) == 0xf6ac, "word_1DC8C");
_Static_assert(offsetof(struct ar_dseg, v_word_1dc8e) == 0xf6ae, "word_1DC8E");
_Static_assert(offsetof(struct ar_dseg, v_word_1dc92) == 0xf6b2, "word_1DC92");
_Static_assert(offsetof(struct ar_dseg, v_word_1dc94) == 0xf6b4, "word_1DC94");
_Static_assert(offsetof(struct ar_dseg, v_word_1dc98) == 0xf6b8, "word_1DC98");
_Static_assert(offsetof(struct ar_dseg, v_word_1dc9a) == 0xf6ba, "word_1DC9A");
_Static_assert(offsetof(struct ar_dseg, v_word_1dc9e) == 0xf6be, "word_1DC9E");
_Static_assert(offsetof(struct ar_dseg, v_word_1dca0) == 0xf6c0, "word_1DCA0");
_Static_assert(offsetof(struct ar_dseg, v_word_1dca2) == 0xf6c2, "word_1DCA2");
_Static_assert(offsetof(struct ar_dseg, v_word_1dca4) == 0xf6c4, "word_1DCA4");
_Static_assert(offsetof(struct ar_dseg, v_byte_1dcaa) == 0xf6ca, "byte_1DCAA");
_Static_assert(offsetof(struct ar_dseg, v_byte_1dcab) == 0xf6cb, "byte_1DCAB");
_Static_assert(offsetof(struct ar_dseg, v_word_1dcac) == 0xf6cc, "word_1DCAC");
_Static_assert(offsetof(struct ar_dseg, v_word_1dcae) == 0xf6ce, "word_1DCAE");
_Static_assert(offsetof(struct ar_dseg, v_byte_1dcb0) == 0xf6d0, "byte_1DCB0");
_Static_assert(offsetof(struct ar_dseg, v_word_1dcb1) == 0xf6d1, "word_1DCB1");
_Static_assert(offsetof(struct ar_dseg, v_word_1dcb3) == 0xf6d3, "word_1DCB3");
_Static_assert(offsetof(struct ar_dseg, v_word_1dcbf) == 0xf6df, "word_1DCBF");
_Static_assert(offsetof(struct ar_dseg, v_byte_1dcc1) == 0xf6e1, "byte_1DCC1");
_Static_assert(offsetof(struct ar_dseg, v_byte_1dcdd) == 0xf6fd, "byte_1DCDD");
_Static_assert(offsetof(struct ar_dseg, v_word_1dcdf) == 0xf6ff, "word_1DCDF");
_Static_assert(offsetof(struct ar_dseg, v_byte_1dce1) == 0xf701, "byte_1DCE1");
_Static_assert(offsetof(struct ar_dseg, v_word_1dcff) == 0xf71f, "word_1DCFF");
_Static_assert(offsetof(struct ar_dseg, v_byte_1dd01) == 0xf721, "byte_1DD01");
_Static_assert(offsetof(struct ar_dseg, v_byte_1dd05) == 0xf725, "byte_1DD05");
_Static_assert(offsetof(struct ar_dseg, v_word_1dd1f) == 0xf73f, "word_1DD1F");
_Static_assert(offsetof(struct ar_dseg, v_byte_1dd21) == 0xf741, "byte_1DD21");
_Static_assert(offsetof(struct ar_dseg, v_byte_1dd25) == 0xf745, "byte_1DD25");
_Static_assert(offsetof(struct ar_dseg, v_word_1dd3f) == 0xf75f, "word_1DD3F");
_Static_assert(offsetof(struct ar_dseg, v_byte_1dd41) == 0xf761, "byte_1DD41");
_Static_assert(offsetof(struct ar_dseg, v_byte_1dd45) == 0xf765, "byte_1DD45");
_Static_assert(offsetof(struct ar_dseg, v_word_1dd5f) == 0xf77f, "word_1DD5F");
_Static_assert(offsetof(struct ar_dseg, v_byte_1dd7f) == 0xf79f, "byte_1DD7F");
_Static_assert(offsetof(struct ar_dseg, v_byte_1dd80) == 0xf7a0, "byte_1DD80");
_Static_assert(offsetof(struct ar_dseg, v_word_1dd9f) == 0xf7bf, "word_1DD9F");
_Static_assert(offsetof(struct ar_dseg, v_word_1dda1) == 0xf7c1, "word_1DDA1");
_Static_assert(offsetof(struct ar_dseg, v_word_1dda3) == 0xf7c3, "word_1DDA3");
_Static_assert(offsetof(struct ar_dseg, v_word_1dda5) == 0xf7c5, "word_1DDA5");
_Static_assert(offsetof(struct ar_dseg, v_word_1ddb0) == 0xf7d0, "word_1DDB0");
_Static_assert(offsetof(struct ar_dseg, v_word_1ddb2) == 0xf7d2, "word_1DDB2");
_Static_assert(offsetof(struct ar_dseg, v_word_1ddb4) == 0xf7d4, "word_1DDB4");
_Static_assert(offsetof(struct ar_dseg, v_word_1ddb9) == 0xf7d9, "word_1DDB9");
_Static_assert(offsetof(struct ar_dseg, v_word_1ddcd) == 0xf7ed, "word_1DDCD");
_Static_assert(offsetof(struct ar_dseg, v_word_1ddd3) == 0xf7f3, "word_1DDD3");
_Static_assert(offsetof(struct ar_dseg, v_byte_1ddd9) == 0xf7f9, "byte_1DDD9");
_Static_assert(offsetof(struct ar_dseg, v_byte_1e0a3) == 0xfac3, "byte_1E0A3");
_Static_assert(offsetof(struct ar_dseg, v_byte_1e12f) == 0xfb4f, "byte_1E12F");
_Static_assert(offsetof(struct ar_dseg, v_word_1e1bd) == 0xfbdd, "word_1E1BD");
_Static_assert(offsetof(struct ar_dseg, v_word_1e1bf) == 0xfbdf, "word_1E1BF");
_Static_assert(offsetof(struct ar_dseg, v_word_1e1c1) == 0xfbe1, "word_1E1C1");
_Static_assert(offsetof(struct ar_dseg, v_word_1e1c3) == 0xfbe3, "word_1E1C3");
_Static_assert(offsetof(struct ar_dseg, v_byte_1e1c5) == 0xfbe5, "byte_1E1C5");
_Static_assert(offsetof(struct ar_dseg, v_byte_1e1c9) == 0xfbe9, "byte_1E1C9");
_Static_assert(offsetof(struct ar_dseg, v_byte_1e1ca) == 0xfbea, "byte_1E1CA");
_Static_assert(offsetof(struct ar_dseg, v_word_1e1cb) == 0xfbeb, "word_1E1CB");
_Static_assert(offsetof(struct ar_dseg, v_byte_1e1cd) == 0xfbed, "byte_1E1CD");
_Static_assert(offsetof(struct ar_dseg, v_word_1e1ce) == 0xfbee, "word_1E1CE");
_Static_assert(offsetof(struct ar_dseg, v_word_1e1d0) == 0xfbf0, "word_1E1D0");
_Static_assert(offsetof(struct ar_dseg, v_word_1e1d2) == 0xfbf2, "word_1E1D2");
_Static_assert(offsetof(struct ar_dseg, v_word_1e1d4) == 0xfbf4, "word_1E1D4");
_Static_assert(offsetof(struct ar_dseg, v_word_1e840) == 0x10260, "word_1E840");
_Static_assert(offsetof(struct ar_dseg, v_word_1e842) == 0x10262, "word_1E842");
_Static_assert(offsetof(struct ar_dseg, v_word_1e844) == 0x10264, "word_1E844");
_Static_assert(offsetof(struct ar_dseg, v_word_1e846) == 0x10266, "word_1E846");
_Static_assert(offsetof(struct ar_dseg, v_aallocated1mbof) == 0x10286, "aAllocated1mbOf");
_Static_assert(offsetof(struct ar_dseg, v_word_209e0) == 0x12400, "word_209E0");
_Static_assert(offsetof(struct ar_dseg, v_word_209e2) == 0x12402, "word_209E2");
_Static_assert(offsetof(struct ar_dseg, v_word_20d38) == 0x12758, "word_20D38");
_Static_assert(offsetof(struct ar_dseg, v_word_20d4a) == 0x1276a, "word_20D4A");
_Static_assert(offsetof(struct ar_dseg, v_byte_20d4c) == 0x1276c, "byte_20D4C");
_Static_assert(offsetof(struct ar_dseg, v_word_20d4e) == 0x1276e, "word_20D4E");
_Static_assert(offsetof(struct ar_dseg, v_byte_20d50) == 0x12770, "byte_20D50");
_Static_assert(offsetof(struct ar_dseg, v_word_20d52) == 0x12772, "word_20D52");
_Static_assert(offsetof(struct ar_dseg, v_byte_20d54) == 0x12774, "byte_20D54");
_Static_assert(offsetof(struct ar_dseg, v_word_20d56) == 0x12776, "word_20D56");
_Static_assert(offsetof(struct ar_dseg, v_word_20d58) == 0x12778, "word_20D58");
_Static_assert(offsetof(struct ar_dseg, v_word_26564) == 0x17f84, "word_26564");
_Static_assert(offsetof(struct ar_dseg, v_word_26566) == 0x17f86, "word_26566");
_Static_assert(offsetof(struct ar_dseg, v_word_2656a) == 0x17f8a, "word_2656A");
_Static_assert(offsetof(struct ar_dseg, v_word_2656c) == 0x17f8c, "word_2656C");
_Static_assert(offsetof(struct ar_dseg, v_word_2656e) == 0x17f8e, "word_2656E");
_Static_assert(offsetof(struct ar_dseg, v_word_26570) == 0x17f90, "word_26570");
_Static_assert(offsetof(struct ar_dseg, v_word_26574) == 0x17f94, "word_26574");
_Static_assert(offsetof(struct ar_dseg, v_word_26582) == 0x17fa2, "word_26582");
_Static_assert(offsetof(struct ar_dseg, v_word_265a8) == 0x17fc8, "word_265A8");
_Static_assert(offsetof(struct ar_dseg, v_word_265aa) == 0x17fca, "word_265AA");
_Static_assert(offsetof(struct ar_dseg, v_word_265ac) == 0x17fcc, "word_265AC");
_Static_assert(offsetof(struct ar_dseg, v_word_265ae) == 0x17fce, "word_265AE");
_Static_assert(offsetof(struct ar_dseg, v_word_265b0) == 0x17fd0, "word_265B0");
_Static_assert(offsetof(struct ar_dseg, v_word_265b2) == 0x17fd2, "word_265B2");
_Static_assert(offsetof(struct ar_dseg, v_word_265b4) == 0x17fd4, "word_265B4");
_Static_assert(offsetof(struct ar_dseg, v_word_26dc0) == 0x187e0, "word_26DC0");
_Static_assert(offsetof(struct ar_dseg, v_byte_26dcd) == 0x187ed, "byte_26DCD");
_Static_assert(offsetof(struct ar_dseg, v_word_26dd3) == 0x187f3, "word_26DD3");
_Static_assert(offsetof(struct ar_dseg, v_byte_26dd5) == 0x187f5, "byte_26DD5");
_Static_assert(offsetof(struct ar_dseg, v_byte_26dd6) == 0x187f6, "byte_26DD6");
_Static_assert(offsetof(struct ar_dseg, v_byte_26dd7) == 0x187f7, "byte_26DD7");
_Static_assert(offsetof(struct ar_dseg, v_word_26dd8) == 0x187f8, "word_26DD8");
_Static_assert(offsetof(struct ar_dseg, v_word_26dda) == 0x187fa, "word_26DDA");
_Static_assert(offsetof(struct ar_dseg, v_word_26ddc) == 0x187fc, "word_26DDC");
_Static_assert(offsetof(struct ar_dseg, v_word_26dde) == 0x187fe, "word_26DDE");
_Static_assert(offsetof(struct ar_dseg, v_word_26de0) == 0x18800, "word_26DE0");
_Static_assert(offsetof(struct ar_dseg, v_word_26de2) == 0x18802, "word_26DE2");
_Static_assert(offsetof(struct ar_dseg, v_word_26de4) == 0x18804, "word_26DE4");
_Static_assert(offsetof(struct ar_dseg, v_byte_26de6) == 0x18806, "byte_26DE6");
_Static_assert(offsetof(struct ar_dseg, v_byte_26de7) == 0x18807, "byte_26DE7");
_Static_assert(offsetof(struct ar_dseg, v_byte_26de8) == 0x18808, "byte_26DE8");
_Static_assert(offsetof(struct ar_dseg, v_byte_26de9) == 0x18809, "byte_26DE9");
_Static_assert(offsetof(struct ar_dseg, v_byte_26dea) == 0x1880a, "byte_26DEA");
_Static_assert(offsetof(struct ar_dseg, v_word_26deb) == 0x1880b, "word_26DEB");
_Static_assert(offsetof(struct ar_dseg, v_word_26ded) == 0x1880d, "word_26DED");
_Static_assert(offsetof(struct ar_dseg, v_byte_26def) == 0x1880f, "byte_26DEF");
_Static_assert(offsetof(struct ar_dseg, v_byte_26df0) == 0x18810, "byte_26DF0");
_Static_assert(offsetof(struct ar_dseg, v_byte_26df1) == 0x18811, "byte_26DF1");
_Static_assert(offsetof(struct ar_dseg, v_byte_26df2) == 0x18812, "byte_26DF2");
_Static_assert(offsetof(struct ar_dseg, v_byte_26df3) == 0x18813, "byte_26DF3");
_Static_assert(offsetof(struct ar_dseg, v_word_26df4) == 0x18814, "word_26DF4");
_Static_assert(offsetof(struct ar_dseg, v_word_26df6) == 0x18816, "word_26DF6");
_Static_assert(offsetof(struct ar_dseg, v_word_26df8) == 0x18818, "word_26DF8");
_Static_assert(offsetof(struct ar_dseg, v_byte_26dfa) == 0x1881a, "byte_26DFA");
_Static_assert(offsetof(struct ar_dseg, v_byte_26dfb) == 0x1881b, "byte_26DFB");
_Static_assert(offsetof(struct ar_dseg, v_byte_26dfc) == 0x1881c, "byte_26DFC");
_Static_assert(offsetof(struct ar_dseg, v_byte_27149) == 0x18b69, "byte_27149");
_Static_assert(offsetof(struct ar_dseg, v_word_27150) == 0x18b70, "word_27150");
_Static_assert(offsetof(struct ar_dseg, v_byte_27152) == 0x18b72, "byte_27152");
_Static_assert(offsetof(struct ar_dseg, v_byte_27153) == 0x18b73, "byte_27153");
_Static_assert(offsetof(struct ar_dseg, v_word_27154) == 0x18b74, "word_27154");
_Static_assert(offsetof(struct ar_dseg, v_byte_27156) == 0x18b76, "byte_27156");
_Static_assert(offsetof(struct ar_dseg, v_byte_27157) == 0x18b77, "byte_27157");
_Static_assert(offsetof(struct ar_dseg, v_byte_27158) == 0x18b78, "byte_27158");
_Static_assert(offsetof(struct ar_dseg, v_byte_27166) == 0x18b86, "byte_27166");
_Static_assert(offsetof(struct ar_dseg, v_byte_27167) == 0x18b87, "byte_27167");
_Static_assert(offsetof(struct ar_dseg, v_word_27169) == 0x18b89, "word_27169");
_Static_assert(offsetof(struct ar_dseg, v_byte_271d5) == 0x18bf5, "byte_271D5");
_Static_assert(offsetof(struct ar_dseg, v_byte_2731e) == 0x18d3e, "byte_2731E");
_Static_assert(offsetof(struct ar_dseg, v_word_2732f) == 0x18d4f, "word_2732F");
_Static_assert(offsetof(struct ar_dseg, v_word_27403) == 0x18e23, "word_27403");
_Static_assert(offsetof(struct ar_dseg, v_word_2762f) == 0x1904f, "word_2762F");
_Static_assert(offsetof(struct ar_dseg, v_word_27634) == 0x19054, "word_27634");
_Static_assert(offsetof(struct ar_dseg, v_byte_281ed) == 0x19c0d, "byte_281ED");
_Static_assert(offsetof(struct ar_dseg, v_byte_281ee) == 0x19c0e, "byte_281EE");
_Static_assert(offsetof(struct ar_dseg, v_word_2824b) == 0x19c6b, "word_2824B");
_Static_assert(offsetof(struct ar_dseg, v_word_28341) == 0x19d61, "word_28341");
_Static_assert(offsetof(struct ar_dseg, v_word_28343) == 0x19d63, "word_28343");
_Static_assert(offsetof(struct ar_dseg, v_word_28345) == 0x19d65, "word_28345");
_Static_assert(offsetof(struct ar_dseg, v_word_28347) == 0x19d67, "word_28347");
_Static_assert(offsetof(struct ar_dseg, v_word_28349) == 0x19d69, "word_28349");
_Static_assert(offsetof(struct ar_dseg, v_word_2860a) == 0x1a02a, "word_2860A");
_Static_assert(offsetof(struct ar_dseg, v_byte_28677) == 0x1a097, "byte_28677");
_Static_assert(offsetof(struct ar_dseg, v_word_28730) == 0x1a150, "word_28730");
_Static_assert(offsetof(struct ar_dseg, v_byte_289e2) == 0x1a402, "byte_289E2");
_Static_assert(offsetof(struct ar_dseg, v_byte_289e3) == 0x1a403, "byte_289E3");
_Static_assert(offsetof(struct ar_dseg, v_acom1com2bstrss) == 0x1a446, "aCom1Com2BstrSs");
_Static_assert(offsetof(struct ar_dseg, v_byte_28a50) == 0x1a470, "byte_28A50");
_Static_assert(offsetof(struct ar_dseg, v_word_28ace) == 0x1a4ee, "word_28ACE");
_Static_assert(offsetof(struct ar_dseg, v_word_28ad0) == 0x1a4f0, "word_28AD0");
_Static_assert(offsetof(struct ar_dseg, v_byte_28ad2) == 0x1a4f2, "byte_28AD2");
_Static_assert(offsetof(struct ar_dseg, v_word_28c83) == 0x1a6a3, "word_28C83");
_Static_assert(offsetof(struct ar_dseg, v_word_28c90) == 0x1a6b0, "word_28C90");
_Static_assert(offsetof(struct ar_dseg, v_word_28c92) == 0x1a6b2, "word_28C92");
_Static_assert(offsetof(struct ar_dseg, v_byte_28c94) == 0x1a6b4, "byte_28C94");
_Static_assert(offsetof(struct ar_dseg, v_byte_28c95) == 0x1a6b5, "byte_28C95");
_Static_assert(offsetof(struct ar_dseg, v_byte_28c96) == 0x1a6b6, "byte_28C96");
_Static_assert(offsetof(struct ar_dseg, v_byte_28c97) == 0x1a6b7, "byte_28C97");
_Static_assert(offsetof(struct ar_dseg, v_byte_28ca0) == 0x1a6c0, "byte_28CA0");
_Static_assert(offsetof(struct ar_dseg, v_byte_28ca1) == 0x1a6c1, "byte_28CA1");
_Static_assert(offsetof(struct ar_dseg, v_byte_28ca2) == 0x1a6c2, "byte_28CA2");
_Static_assert(offsetof(struct ar_dseg, v_byte_28ca3) == 0x1a6c3, "byte_28CA3");
_Static_assert(offsetof(struct ar_dseg, v_word_28cac) == 0x1a6cc, "word_28CAC");
_Static_assert(offsetof(struct ar_dseg, v_byte_28cb1) == 0x1a6d1, "byte_28CB1");
_Static_assert(offsetof(struct ar_dseg, v_byte_28cb3) == 0x1a6d3, "byte_28CB3");
_Static_assert(offsetof(struct ar_dseg, v_byte_28cb4) == 0x1a6d4, "byte_28CB4");
_Static_assert(offsetof(struct ar_dseg, v_byte_28cc0) == 0x1a6e0, "byte_28CC0");
_Static_assert(offsetof(struct ar_dseg, v_byte_28cc1) == 0x1a6e1, "byte_28CC1");
_Static_assert(offsetof(struct ar_dseg, v_byte_28cc2) == 0x1a6e2, "byte_28CC2");
_Static_assert(offsetof(struct ar_dseg, v_byte_28cc3) == 0x1a6e3, "byte_28CC3");
_Static_assert(offsetof(struct ar_dseg, v_byte_28cc4) == 0x1a6e4, "byte_28CC4");
_Static_assert(offsetof(struct ar_dseg, v_byte_28cc5) == 0x1a6e5, "byte_28CC5");
_Static_assert(offsetof(struct ar_dseg, v_byte_28cc8) == 0x1a6e8, "byte_28CC8");
_Static_assert(offsetof(struct ar_dseg, v_byte_28cca) == 0x1a6ea, "byte_28CCA");
_Static_assert(offsetof(struct ar_dseg, v_byte_28ccb) == 0x1a6eb, "byte_28CCB");
_Static_assert(offsetof(struct ar_dseg, v_byte_28cd1) == 0x1a6f1, "byte_28CD1");
_Static_assert(offsetof(struct ar_dseg, v_byte_28cd2) == 0x1a6f2, "byte_28CD2");
_Static_assert(offsetof(struct ar_dseg, v_byte_28cd3) == 0x1a6f3, "byte_28CD3");
_Static_assert(offsetof(struct ar_dseg, v_byte_28cd4) == 0x1a6f4, "byte_28CD4");
_Static_assert(offsetof(struct ar_dseg, v_word_28cd5) == 0x1a6f5, "word_28CD5");
_Static_assert(offsetof(struct ar_dseg, v_byte_28cd8) == 0x1a6f8, "byte_28CD8");
_Static_assert(offsetof(struct ar_dseg, v_byte_28cd9) == 0x1a6f9, "byte_28CD9");
_Static_assert(offsetof(struct ar_dseg, v_byte_28cdb) == 0x1a6fb, "byte_28CDB");
_Static_assert(offsetof(struct ar_dseg, v_byte_28cdc) == 0x1a6fc, "byte_28CDC");
_Static_assert(offsetof(struct ar_dseg, v_byte_28d0d) == 0x1a72d, "byte_28D0D");
_Static_assert(offsetof(struct ar_dseg, v_byte_28d0e) == 0x1a72e, "byte_28D0E");
_Static_assert(offsetof(struct ar_dseg, v_byte_28d51) == 0x1a771, "byte_28D51");
_Static_assert(offsetof(struct ar_dseg, v_byte_28dfb) == 0x1a81b, "byte_28DFB");
_Static_assert(offsetof(struct ar_dseg, v_byte_28e1d) == 0x1a83d, "byte_28E1D");
_Static_assert(offsetof(struct ar_dseg, v_byte_28e3f) == 0x1a85f, "byte_28E3F");
_Static_assert(offsetof(struct ar_dseg, v_byte_28e61) == 0x1a881, "byte_28E61");
_Static_assert(offsetof(struct ar_dseg, v_byte_28e83) == 0x1a8a3, "byte_28E83");
_Static_assert(offsetof(struct ar_dseg, v_byte_28f2d) == 0x1a94d, "byte_28F2D");
_Static_assert(offsetof(struct ar_dseg, v_byte_2901b) == 0x1aa3b, "byte_2901B");
_Static_assert(offsetof(struct ar_dseg, v_byte_2903d) == 0x1aa5d, "byte_2903D");
_Static_assert(offsetof(struct ar_dseg, v_byte_290a3) == 0x1aac3, "byte_290A3");
_Static_assert(offsetof(struct ar_dseg, v_byte_2914d) == 0x1ab6d, "byte_2914D");
_Static_assert(offsetof(struct ar_dseg, v_byte_2914e) == 0x1ab6e, "byte_2914E");
_Static_assert(offsetof(struct ar_dseg, v_byte_2916f) == 0x1ab8f, "byte_2916F");
_Static_assert(offsetof(struct ar_dseg, v_byte_29170) == 0x1ab90, "byte_29170");
_Static_assert(offsetof(struct ar_dseg, v_byte_291b3) == 0x1abd3, "byte_291B3");
_Static_assert(offsetof(struct ar_dseg, v_byte_291d5) == 0x1abf5, "byte_291D5");
_Static_assert(offsetof(struct ar_dseg, v_byte_29219) == 0x1ac39, "byte_29219");
_Static_assert(offsetof(struct ar_dseg, v_byte_2923b) == 0x1ac5b, "byte_2923B");
_Static_assert(offsetof(struct ar_dseg, v_byte_29307) == 0x1ad27, "byte_29307");
_Static_assert(offsetof(struct ar_dseg, v_byte_29537) == 0x1af57, "byte_29537");
_Static_assert(offsetof(struct ar_dseg, v_byte_29538) == 0x1af58, "byte_29538");
_Static_assert(offsetof(struct ar_dseg, v_byte_29539) == 0x1af59, "byte_29539");
_Static_assert(offsetof(struct ar_dseg, v_byte_29567) == 0x1af87, "byte_29567");
_Static_assert(offsetof(struct ar_dseg, v_byte_295ea) == 0x1b00a, "byte_295EA");
_Static_assert(offsetof(struct ar_dseg, v_word_295eb) == 0x1b00b, "word_295EB");
_Static_assert(offsetof(struct ar_dseg, v_byte_295ef) == 0x1b00f, "byte_295EF");
_Static_assert(offsetof(struct ar_dseg, v_word_29668) == 0x1b088, "word_29668");
_Static_assert(offsetof(struct ar_dseg, v_word_2966a) == 0x1b08a, "word_2966A");
_Static_assert(offsetof(struct ar_dseg, v_word_2966c) == 0x1b08c, "word_2966C");
_Static_assert(offsetof(struct ar_dseg, v_word_2966e) == 0x1b08e, "word_2966E");
_Static_assert(offsetof(struct ar_dseg, v_word_29670) == 0x1b090, "word_29670");
_Static_assert(offsetof(struct ar_dseg, v_word_29676) == 0x1b096, "word_29676");
_Static_assert(offsetof(struct ar_dseg, v_word_29678) == 0x1b098, "word_29678");
_Static_assert(offsetof(struct ar_dseg, v_byte_2967a) == 0x1b09a, "byte_2967A");
_Static_assert(offsetof(struct ar_dseg, v_word_2967b) == 0x1b09b, "word_2967B");
_Static_assert(offsetof(struct ar_dseg, v_word_2967d) == 0x1b09d, "word_2967D");
_Static_assert(offsetof(struct ar_dseg, v_word_29681) == 0x1b0a1, "word_29681");
_Static_assert(offsetof(struct ar_dseg, v_word_29683) == 0x1b0a3, "word_29683");
_Static_assert(offsetof(struct ar_dseg, v_byte_2970d) == 0x1b12d, "byte_2970D");
_Static_assert(offsetof(struct ar_dseg, v_byte_2970e) == 0x1b12e, "byte_2970E");
_Static_assert(offsetof(struct ar_dseg, v_byte_2970f) == 0x1b12f, "byte_2970F");
_Static_assert(offsetof(struct ar_dseg, v_byte_29710) == 0x1b130, "byte_29710");
_Static_assert(offsetof(struct ar_dseg, v_byte_29711) == 0x1b131, "byte_29711");
_Static_assert(offsetof(struct ar_dseg, v_byte_29712) == 0x1b132, "byte_29712");
_Static_assert(offsetof(struct ar_dseg, v_byte_29714) == 0x1b134, "byte_29714");
_Static_assert(offsetof(struct ar_dseg, v_byte_29715) == 0x1b135, "byte_29715");
_Static_assert(offsetof(struct ar_dseg, v_byte_29716) == 0x1b136, "byte_29716");
_Static_assert(offsetof(struct ar_dseg, v_byte_29717) == 0x1b137, "byte_29717");
_Static_assert(offsetof(struct ar_dseg, v_byte_29718) == 0x1b138, "byte_29718");
_Static_assert(offsetof(struct ar_dseg, v_byte_2971a) == 0x1b13a, "byte_2971A");
_Static_assert(offsetof(struct ar_dseg, v_byte_29721) == 0x1b141, "byte_29721");
_Static_assert(offsetof(struct ar_dseg, v_byte_29722) == 0x1b142, "byte_29722");
_Static_assert(offsetof(struct ar_dseg, v_byte_29723) == 0x1b143, "byte_29723");
_Static_assert(offsetof(struct ar_dseg, v_byte_29724) == 0x1b144, "byte_29724");
_Static_assert(offsetof(struct ar_dseg, v_byte_29733) == 0x1b153, "byte_29733");
_Static_assert(offsetof(struct ar_dseg, v_byte_29734) == 0x1b154, "byte_29734");
_Static_assert(offsetof(struct ar_dseg, v_byte_29735) == 0x1b155, "byte_29735");
_Static_assert(offsetof(struct ar_dseg, v_byte_29736) == 0x1b156, "byte_29736");
_Static_assert(offsetof(struct ar_dseg, v_byte_29738) == 0x1b158, "byte_29738");
_Static_assert(offsetof(struct ar_dseg, v_byte_2974f) == 0x1b16f, "byte_2974F");
_Static_assert(offsetof(struct ar_dseg, v_byte_29750) == 0x1b170, "byte_29750");
_Static_assert(offsetof(struct ar_dseg, v_byte_29761) == 0x1b181, "byte_29761");
_Static_assert(offsetof(struct ar_dseg, v_byte_29762) == 0x1b182, "byte_29762");
_Static_assert(offsetof(struct ar_dseg, v_byte_29763) == 0x1b183, "byte_29763");
_Static_assert(offsetof(struct ar_dseg, v_byte_29764) == 0x1b184, "byte_29764");
_Static_assert(offsetof(struct ar_dseg, v_byte_29765) == 0x1b185, "byte_29765");
_Static_assert(offsetof(struct ar_dseg, v_byte_29766) == 0x1b186, "byte_29766");
_Static_assert(offsetof(struct ar_dseg, v_byte_29767) == 0x1b187, "byte_29767");
_Static_assert(offsetof(struct ar_dseg, v_byte_29768) == 0x1b188, "byte_29768");
_Static_assert(offsetof(struct ar_dseg, v_byte_29769) == 0x1b189, "byte_29769");
_Static_assert(offsetof(struct ar_dseg, v_byte_2976a) == 0x1b18a, "byte_2976A");
_Static_assert(offsetof(struct ar_dseg, v_byte_2976b) == 0x1b18b, "byte_2976B");
_Static_assert(offsetof(struct ar_dseg, v_byte_2976c) == 0x1b18c, "byte_2976C");
_Static_assert(offsetof(struct ar_dseg, v_word_297e1) == 0x1b201, "word_297E1");
_Static_assert(offsetof(struct ar_dseg, v_word_297f3) == 0x1b213, "word_297F3");
_Static_assert(offsetof(struct ar_dseg, v_word_29812) == 0x1b232, "word_29812");
_Static_assert(offsetof(struct ar_dseg, v_word_29814) == 0x1b234, "word_29814");
_Static_assert(offsetof(struct ar_dseg, v_word_29816) == 0x1b236, "word_29816");
_Static_assert(offsetof(struct ar_dseg, v_word_29818) == 0x1b238, "word_29818");
_Static_assert(offsetof(struct ar_dseg, v_word_29845) == 0x1b265, "word_29845");
_Static_assert(offsetof(struct ar_dseg, v_byte_29847) == 0x1b267, "byte_29847");
_Static_assert(offsetof(struct ar_dseg, v_byte_29848) == 0x1b268, "byte_29848");
_Static_assert(offsetof(struct ar_dseg, v_byte_2984b) == 0x1b26b, "byte_2984B");
_Static_assert(offsetof(struct ar_dseg, v_byte_2984c) == 0x1b26c, "byte_2984C");
_Static_assert(offsetof(struct ar_dseg, v_byte_2984d) == 0x1b26d, "byte_2984D");
_Static_assert(offsetof(struct ar_dseg, v_byte_2984e) == 0x1b26e, "byte_2984E");
_Static_assert(offsetof(struct ar_dseg, v_byte_2984f) == 0x1b26f, "byte_2984F");
_Static_assert(offsetof(struct ar_dseg, v_word_298a2) == 0x1b2c2, "word_298A2");
_Static_assert(offsetof(struct ar_dseg, v_word_298a4) == 0x1b2c4, "word_298A4");
_Static_assert(offsetof(struct ar_dseg, v_byte_298a6) == 0x1b2c6, "byte_298A6");
_Static_assert(offsetof(struct ar_dseg, v_word_298ac) == 0x1b2cc, "word_298AC");
_Static_assert(offsetof(struct ar_dseg, v_word_298ae) == 0x1b2ce, "word_298AE");
_Static_assert(offsetof(struct ar_dseg, v_word_298b2) == 0x1b2d2, "word_298B2");
_Static_assert(offsetof(struct ar_dseg, v_word_298c0) == 0x1b2e0, "word_298C0");
_Static_assert(offsetof(struct ar_dseg, v_word_298c2) == 0x1b2e2, "word_298C2");
_Static_assert(offsetof(struct ar_dseg, v_byte_298c4) == 0x1b2e4, "byte_298C4");
_Static_assert(offsetof(struct ar_dseg, v_word_29955) == 0x1b375, "word_29955");
_Static_assert(offsetof(struct ar_dseg, v_byte_2998f) == 0x1b3af, "byte_2998F");
_Static_assert(offsetof(struct ar_dseg, v_byte_29999) == 0x1b3b9, "byte_29999");
_Static_assert(offsetof(struct ar_dseg, v_byte_2999a) == 0x1b3ba, "byte_2999A");
_Static_assert(offsetof(struct ar_dseg, v_byte_299af) == 0x1b3cf, "byte_299AF");
_Static_assert(offsetof(struct ar_dseg, v_word_29b42) == 0x1b562, "word_29B42");
_Static_assert(offsetof(struct ar_dseg, v_word_29b44) == 0x1b564, "word_29B44");
_Static_assert(offsetof(struct ar_dseg, v_byte_29b4f) == 0x1b56f, "byte_29B4F");
_Static_assert(offsetof(struct ar_dseg, v_byte_29b50) == 0x1b570, "byte_29B50");
_Static_assert(offsetof(struct ar_dseg, v_byte_29b51) == 0x1b571, "byte_29B51");
_Static_assert(offsetof(struct ar_dseg, v_word_29b52) == 0x1b572, "word_29B52");
_Static_assert(offsetof(struct ar_dseg, v_word_29b54) == 0x1b574, "word_29B54");
_Static_assert(offsetof(struct ar_dseg, v_word_29b56) == 0x1b576, "word_29B56");
_Static_assert(offsetof(struct ar_dseg, v_word_29b58) == 0x1b578, "word_29B58");
_Static_assert(offsetof(struct ar_dseg, v_byte_29b5a) == 0x1b57a, "byte_29B5A");
_Static_assert(offsetof(struct ar_dseg, v_byte_29b5b) == 0x1b57b, "byte_29B5B");
_Static_assert(offsetof(struct ar_dseg, v_byte_29b5c) == 0x1b57c, "byte_29B5C");
_Static_assert(offsetof(struct ar_dseg, v_byte_29b5d) == 0x1b57d, "byte_29B5D");
_Static_assert(offsetof(struct ar_dseg, v_byte_29b5e) == 0x1b57e, "byte_29B5E");
_Static_assert(offsetof(struct ar_dseg, v_byte_29b5f) == 0x1b57f, "byte_29B5F");
_Static_assert(offsetof(struct ar_dseg, v_byte_29b60) == 0x1b580, "byte_29B60");
_Static_assert(offsetof(struct ar_dseg, v_byte_29b61) == 0x1b581, "byte_29B61");
_Static_assert(offsetof(struct ar_dseg, v_byte_29b62) == 0x1b582, "byte_29B62");
_Static_assert(offsetof(struct ar_dseg, v_word_29b63) == 0x1b583, "word_29B63");
_Static_assert(offsetof(struct ar_dseg, v_word_29b95) == 0x1b5b5, "word_29B95");
_Static_assert(offsetof(struct ar_dseg, v_word_29b97) == 0x1b5b7, "word_29B97");
_Static_assert(offsetof(struct ar_dseg, v_word_29e36) == 0x1b856, "word_29E36");
_Static_assert(offsetof(struct ar_dseg, v_word_29e38) == 0x1b858, "word_29E38");
_Static_assert(offsetof(struct ar_dseg, v_word_29e3a) == 0x1b85a, "word_29E3A");
_Static_assert(offsetof(struct ar_dseg, v_word_29e3c) == 0x1b85c, "word_29E3C");
_Static_assert(offsetof(struct ar_dseg, v_word_29e3e) == 0x1b85e, "word_29E3E");
_Static_assert(offsetof(struct ar_dseg, v_word_29e40) == 0x1b860, "word_29E40");
_Static_assert(offsetof(struct ar_dseg, v_word_29ebc) == 0x1b8dc, "word_29EBC");
_Static_assert(offsetof(struct ar_dseg, v_byte_29f5e) == 0x1b97e, "byte_29F5E");
_Static_assert(offsetof(struct ar_dseg, v_word_29f60) == 0x1b980, "word_29F60");
_Static_assert(offsetof(struct ar_dseg, v_word_29f64) == 0x1b984, "word_29F64");
_Static_assert(offsetof(struct ar_dseg, v_word_29f66) == 0x1b986, "word_29F66");
_Static_assert(offsetof(struct ar_dseg, v_byte_29f76) == 0x1b996, "byte_29F76");
_Static_assert(offsetof(struct ar_dseg, v_byte_29f77) == 0x1b997, "byte_29F77");
_Static_assert(offsetof(struct ar_dseg, v_word_29f78) == 0x1b998, "word_29F78");
_Static_assert(offsetof(struct ar_dseg, v_word_2a0dd) == 0x1bafd, "word_2A0DD");
_Static_assert(offsetof(struct ar_dseg, v_word_2a0ea) == 0x1bb0a, "word_2A0EA");
_Static_assert(offsetof(struct ar_dseg, v_word_2a10d) == 0x1bb2d, "word_2A10D");
_Static_assert(offsetof(struct ar_dseg, v_word_2a12a) == 0x1bb4a, "word_2A12A");
_Static_assert(offsetof(struct ar_dseg, v_word_2a147) == 0x1bb67, "word_2A147");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a30d) == 0x1bd2d, "byte_2A30D");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a33f) == 0x1bd5f, "byte_2A33F");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a3b0) == 0x1bdd0, "byte_2A3B0");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a400) == 0x1be20, "byte_2A400");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a441) == 0x1be61, "byte_2A441");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a4ca) == 0x1beea, "byte_2A4CA");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a506) == 0x1bf26, "byte_2A506");
_Static_assert(offsetof(struct ar_dseg, v_word_2a507) == 0x1bf27, "word_2A507");
_Static_assert(offsetof(struct ar_dseg, v_word_2a509) == 0x1bf29, "word_2A509");
_Static_assert(offsetof(struct ar_dseg, v_word_2a5a3) == 0x1bfc3, "word_2A5A3");
_Static_assert(offsetof(struct ar_dseg, v_word_2a5a5) == 0x1bfc5, "word_2A5A5");
_Static_assert(offsetof(struct ar_dseg, v_word_2a5a7) == 0x1bfc7, "word_2A5A7");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a5b0) == 0x1bfd0, "byte_2A5B0");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a5b1) == 0x1bfd1, "byte_2A5B1");
_Static_assert(offsetof(struct ar_dseg, v_word_2a5b2) == 0x1bfd2, "word_2A5B2");
_Static_assert(offsetof(struct ar_dseg, v_word_2a5b4) == 0x1bfd4, "word_2A5B4");
_Static_assert(offsetof(struct ar_dseg, v_word_2a5b6) == 0x1bfd6, "word_2A5B6");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a5ee) == 0x1c00e, "byte_2A5EE");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a5ef) == 0x1c00f, "byte_2A5EF");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a5f0) == 0x1c010, "byte_2A5F0");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a5f1) == 0x1c011, "byte_2A5F1");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a5f2) == 0x1c012, "byte_2A5F2");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a5f3) == 0x1c013, "byte_2A5F3");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a5f4) == 0x1c014, "byte_2A5F4");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a5f5) == 0x1c015, "byte_2A5F5");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a5f6) == 0x1c016, "byte_2A5F6");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a5f7) == 0x1c017, "byte_2A5F7");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a5f8) == 0x1c018, "byte_2A5F8");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a5f9) == 0x1c019, "byte_2A5F9");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a5fa) == 0x1c01a, "byte_2A5FA");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a5fb) == 0x1c01b, "byte_2A5FB");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a5fc) == 0x1c01c, "byte_2A5FC");
_Static_assert(offsetof(struct ar_dseg, v_word_2a5fd) == 0x1c01d, "word_2A5FD");
_Static_assert(offsetof(struct ar_dseg, v_word_2a5ff) == 0x1c01f, "word_2A5FF");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a601) == 0x1c021, "byte_2A601");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a602) == 0x1c022, "byte_2A602");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a603) == 0x1c023, "byte_2A603");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a604) == 0x1c024, "byte_2A604");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a605) == 0x1c025, "byte_2A605");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a606) == 0x1c026, "byte_2A606");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a60f) == 0x1c02f, "byte_2A60F");
_Static_assert(offsetof(struct ar_dseg, v_word_2a610) == 0x1c030, "word_2A610");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a612) == 0x1c032, "byte_2A612");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a613) == 0x1c033, "byte_2A613");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a614) == 0x1c034, "byte_2A614");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a615) == 0x1c035, "byte_2A615");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a6af) == 0x1c0cf, "byte_2A6AF");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a6b0) == 0x1c0d0, "byte_2A6B0");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a6b1) == 0x1c0d1, "byte_2A6B1");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a6b2) == 0x1c0d2, "byte_2A6B2");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a6b3) == 0x1c0d3, "byte_2A6B3");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a6b4) == 0x1c0d4, "byte_2A6B4");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a6b5) == 0x1c0d5, "byte_2A6B5");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a6be) == 0x1c0de, "byte_2A6BE");
_Static_assert(offsetof(struct ar_dseg, v_word_2a6bf) == 0x1c0df, "word_2A6BF");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a6c1) == 0x1c0e1, "byte_2A6C1");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a6c2) == 0x1c0e2, "byte_2A6C2");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a6c3) == 0x1c0e3, "byte_2A6C3");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a6c4) == 0x1c0e4, "byte_2A6C4");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a6c5) == 0x1c0e5, "byte_2A6C5");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a6c6) == 0x1c0e6, "byte_2A6C6");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a74b) == 0x1c16b, "byte_2A74B");
_Static_assert(offsetof(struct ar_dseg, v_word_2a7a8) == 0x1c1c8, "word_2A7A8");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a7aa) == 0x1c1ca, "byte_2A7AA");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a7ab) == 0x1c1cb, "byte_2A7AB");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a7ac) == 0x1c1cc, "byte_2A7AC");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a7ad) == 0x1c1cd, "byte_2A7AD");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a7ae) == 0x1c1ce, "byte_2A7AE");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a7af) == 0x1c1cf, "byte_2A7AF");
_Static_assert(offsetof(struct ar_dseg, v_word_2a7b0) == 0x1c1d0, "word_2A7B0");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a86b) == 0x1c28b, "byte_2A86B");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a86c) == 0x1c28c, "byte_2A86C");
_Static_assert(offsetof(struct ar_dseg, v_word_2a89c) == 0x1c2bc, "word_2A89C");
_Static_assert(offsetof(struct ar_dseg, v_word_2a89e) == 0x1c2be, "word_2A89E");
_Static_assert(offsetof(struct ar_dseg, v_word_2a8a0) == 0x1c2c0, "word_2A8A0");
_Static_assert(offsetof(struct ar_dseg, v_word_2a8a2) == 0x1c2c2, "word_2A8A2");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a8b0) == 0x1c2d0, "byte_2A8B0");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a90a) == 0x1c32a, "byte_2A90A");
_Static_assert(offsetof(struct ar_dseg, v_a0) == 0x1c32b, "a0");
_Static_assert(offsetof(struct ar_dseg, v_word_2a933) == 0x1c353, "word_2A933");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a960) == 0x1c380, "byte_2A960");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a961) == 0x1c381, "byte_2A961");
_Static_assert(offsetof(struct ar_dseg, v_word_2a962) == 0x1c382, "word_2A962");
_Static_assert(offsetof(struct ar_dseg, v_word_2a968) == 0x1c388, "word_2A968");
_Static_assert(offsetof(struct ar_dseg, v_word_2a96a) == 0x1c38a, "word_2A96A");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a96c) == 0x1c38c, "byte_2A96C");
_Static_assert(offsetof(struct ar_dseg, v_word_2a99d) == 0x1c3bd, "word_2A99D");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a99f) == 0x1c3bf, "byte_2A99F");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a9ca) == 0x1c3ea, "byte_2A9CA");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a9cb) == 0x1c3eb, "byte_2A9CB");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a9cd) == 0x1c3ed, "byte_2A9CD");
_Static_assert(offsetof(struct ar_dseg, v_word_2a9ce) == 0x1c3ee, "word_2A9CE");
_Static_assert(offsetof(struct ar_dseg, v_word_2a9d0) == 0x1c3f0, "word_2A9D0");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a9d2) == 0x1c3f2, "byte_2A9D2");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a9d3) == 0x1c3f3, "byte_2A9D3");
_Static_assert(offsetof(struct ar_dseg, v_word_2a9d4) == 0x1c3f4, "word_2A9D4");
_Static_assert(offsetof(struct ar_dseg, v_word_2a9d6) == 0x1c3f6, "word_2A9D6");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a9d8) == 0x1c3f8, "byte_2A9D8");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a9df) == 0x1c3ff, "byte_2A9DF");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a9e0) == 0x1c400, "byte_2A9E0");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a9e1) == 0x1c401, "byte_2A9E1");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a9e2) == 0x1c402, "byte_2A9E2");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a9e3) == 0x1c403, "byte_2A9E3");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a9e4) == 0x1c404, "byte_2A9E4");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a9f9) == 0x1c419, "byte_2A9F9");
_Static_assert(offsetof(struct ar_dseg, v_byte_2a9fa) == 0x1c41a, "byte_2A9FA");
_Static_assert(offsetof(struct ar_dseg, v_word_2a9fb) == 0x1c41b, "word_2A9FB");
_Static_assert(offsetof(struct ar_dseg, v_word_2aa0d) == 0x1c42d, "word_2AA0D");
_Static_assert(offsetof(struct ar_dseg, v_word_2aa40) == 0x1c460, "word_2AA40");
_Static_assert(offsetof(struct ar_dseg, v_word_2aa42) == 0x1c462, "word_2AA42");
_Static_assert(offsetof(struct ar_dseg, v_word_2aa44) == 0x1c464, "word_2AA44");
_Static_assert(offsetof(struct ar_dseg, v_word_2aa46) == 0x1c466, "word_2AA46");
_Static_assert(offsetof(struct ar_dseg, v_word_2aa48) == 0x1c468, "word_2AA48");
_Static_assert(offsetof(struct ar_dseg, v_word_2aa4a) == 0x1c46a, "word_2AA4A");
_Static_assert(offsetof(struct ar_dseg, v_word_2aa56) == 0x1c476, "word_2AA56");
_Static_assert(offsetof(struct ar_dseg, v_word_2aa58) == 0x1c478, "word_2AA58");
_Static_assert(offsetof(struct ar_dseg, v_byte_2aa5c) == 0x1c47c, "byte_2AA5C");
_Static_assert(offsetof(struct ar_dseg, v_byte_2aa5d) == 0x1c47d, "byte_2AA5D");
_Static_assert(offsetof(struct ar_dseg, v_byte_2aa5e) == 0x1c47e, "byte_2AA5E");
_Static_assert(offsetof(struct ar_dseg, v_byte_2aa5f) == 0x1c47f, "byte_2AA5F");
_Static_assert(offsetof(struct ar_dseg, v_byte_2aa60) == 0x1c480, "byte_2AA60");
_Static_assert(offsetof(struct ar_dseg, v_byte_2aa61) == 0x1c481, "byte_2AA61");
_Static_assert(offsetof(struct ar_dseg, v_byte_2aa62) == 0x1c482, "byte_2AA62");
_Static_assert(offsetof(struct ar_dseg, v_byte_2aa63) == 0x1c483, "byte_2AA63");
_Static_assert(offsetof(struct ar_dseg, v_byte_2aa64) == 0x1c484, "byte_2AA64");
_Static_assert(offsetof(struct ar_dseg, v_word_2aa65) == 0x1c485, "word_2AA65");
_Static_assert(offsetof(struct ar_dseg, v_word_2aa67) == 0x1c487, "word_2AA67");
_Static_assert(offsetof(struct ar_dseg, v_word_2aa69) == 0x1c489, "word_2AA69");
_Static_assert(offsetof(struct ar_dseg, v_word_2aa6b) == 0x1c48b, "word_2AA6B");
_Static_assert(offsetof(struct ar_dseg, v_byte_2aa6d) == 0x1c48d, "byte_2AA6D");
_Static_assert(offsetof(struct ar_dseg, v_byte_2aa6e) == 0x1c48e, "byte_2AA6E");
_Static_assert(offsetof(struct ar_dseg, v_byte_2aa6f) == 0x1c48f, "byte_2AA6F");
_Static_assert(offsetof(struct ar_dseg, v_word_2b070) == 0x1ca90, "word_2B070");
_Static_assert(offsetof(struct ar_dseg, v_word_2b074) == 0x1ca94, "word_2B074");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b076) == 0x1ca96, "byte_2B076");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b0c3) == 0x1cae3, "byte_2B0C3");
_Static_assert(offsetof(struct ar_dseg, v_word_2b0c4) == 0x1cae4, "word_2B0C4");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b0c6) == 0x1cae6, "byte_2B0C6");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b0c7) == 0x1cae7, "byte_2B0C7");
_Static_assert(offsetof(struct ar_dseg, v_word_2b0c8) == 0x1cae8, "word_2B0C8");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b0ca) == 0x1caea, "byte_2B0CA");
_Static_assert(offsetof(struct ar_dseg, v_word_2b0cb) == 0x1caeb, "word_2B0CB");
_Static_assert(offsetof(struct ar_dseg, v_word_2b0cd) == 0x1caed, "word_2B0CD");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b0cf) == 0x1caef, "byte_2B0CF");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b0d0) == 0x1caf0, "byte_2B0D0");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b0d1) == 0x1caf1, "byte_2B0D1");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b0d2) == 0x1caf2, "byte_2B0D2");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b0d3) == 0x1caf3, "byte_2B0D3");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b0d4) == 0x1caf4, "byte_2B0D4");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b115) == 0x1cb35, "byte_2B115");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b116) == 0x1cb36, "byte_2B116");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b117) == 0x1cb37, "byte_2B117");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b118) == 0x1cb38, "byte_2B118");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b119) == 0x1cb39, "byte_2B119");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b11a) == 0x1cb3a, "byte_2B11A");
_Static_assert(offsetof(struct ar_dseg, v_word_2b11b) == 0x1cb3b, "word_2B11B");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b11d) == 0x1cb3d, "byte_2B11D");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b11e) == 0x1cb3e, "byte_2B11E");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b11f) == 0x1cb3f, "byte_2B11F");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b120) == 0x1cb40, "byte_2B120");
_Static_assert(offsetof(struct ar_dseg, v_word_2b121) == 0x1cb41, "word_2B121");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b123) == 0x1cb43, "byte_2B123");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b126) == 0x1cb46, "byte_2B126");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b127) == 0x1cb47, "byte_2B127");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b128) == 0x1cb48, "byte_2B128");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b129) == 0x1cb49, "byte_2B129");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b12a) == 0x1cb4a, "byte_2B12A");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b12b) == 0x1cb4b, "byte_2B12B");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b12c) == 0x1cb4c, "byte_2B12C");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b12d) == 0x1cb4d, "byte_2B12D");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b12e) == 0x1cb4e, "byte_2B12E");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b12f) == 0x1cb4f, "byte_2B12F");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b130) == 0x1cb50, "byte_2B130");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b131) == 0x1cb51, "byte_2B131");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b132) == 0x1cb52, "byte_2B132");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b23c) == 0x1cc5c, "byte_2B23C");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b23d) == 0x1cc5d, "byte_2B23D");
_Static_assert(offsetof(struct ar_dseg, v_word_2b23e) == 0x1cc5e, "word_2B23E");
_Static_assert(offsetof(struct ar_dseg, v_word_2b240) == 0x1cc60, "word_2B240");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b242) == 0x1cc62, "byte_2B242");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b243) == 0x1cc63, "byte_2B243");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b244) == 0x1cc64, "byte_2B244");
_Static_assert(offsetof(struct ar_dseg, v_word_2b245) == 0x1cc65, "word_2B245");
_Static_assert(offsetof(struct ar_dseg, v_word_2b247) == 0x1cc67, "word_2B247");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b24d) == 0x1cc6d, "byte_2B24D");
_Static_assert(offsetof(struct ar_dseg, v_word_2b24f) == 0x1cc6f, "word_2B24F");
_Static_assert(offsetof(struct ar_dseg, v_word_2b251) == 0x1cc71, "word_2B251");
_Static_assert(offsetof(struct ar_dseg, v_word_2b253) == 0x1cc73, "word_2B253");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b2ce) == 0x1ccee, "byte_2B2CE");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b3f6) == 0x1ce16, "byte_2B3F6");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b3f7) == 0x1ce17, "byte_2B3F7");
_Static_assert(offsetof(struct ar_dseg, v_byte_2b510) == 0x1cf30, "byte_2B510");
_Static_assert(offsetof(struct dcomp_state, lim) == 0x0, "byte_24e70");
_Static_assert(offsetof(struct dcomp_state, maxw) == 0x1, "byte_24e71");
_Static_assert(offsetof(struct dcomp_state, mask) == 0x2, "word_24e72");
_Static_assert(offsetof(struct dcomp_state, acc) == 0x4, "word_24e74");
_Static_assert(offsetof(struct dcomp_state, npending) == 0x6, "byte_24e76");
_Static_assert(offsetof(struct dcomp_state, end) == 0x7, "word_24e77");
_Static_assert(offsetof(struct dcomp_state, prev) == 0x9, "word_24e79");
_Static_assert(offsetof(struct dcomp_state, first) == 0xb, "byte_24e7b");
