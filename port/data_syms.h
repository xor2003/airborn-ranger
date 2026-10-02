// data symbol -> mem[] aliases (lst-linear addressing)
#pragma once
#define seg_data (*(volatile dw*)&mem[0x1a20])
#define seg_10002 (*(volatile dw*)&mem[0x1a22])
#define seg_resbuf (*(volatile dw*)&mem[0x1a24])
#define seg_1000E (*(volatile dw*)&mem[0x1a2e])
#define seg_flip (*(volatile dw*)&mem[0x1a30])
#define seg_draw (*(volatile dw*)&mem[0x1a32])
#define seg_aux (*(volatile dw*)&mem[0x1a34])
#define seg_10016 (*(volatile dw*)&mem[0x1a36])
#define seg_10018 (*(volatile dw*)&mem[0x1a38])
#define word_100C3 (*(volatile dw*)&mem[0x1ae3])
#define pal_jt (*(volatile dd*)&mem[0x1dad])
#define jpt_107B6 (*(volatile dd*)&mem[0x21ad])
#define modecall_tbl (*(volatile dd*)&mem[0x21db])
#define clrflip_jt (*(volatile dd*)&mem[0x24d5])
#define clrdraw_jt (*(volatile dd*)&mem[0x2536])
#define clrback_jt (*(volatile dd*)&mem[0x2559])
#define jpt_10BEF (*(volatile dd*)&mem[0x25ec])
#define jpt_10C4F (*(volatile dd*)&mem[0x264c])
#define vidmode_jt (*(volatile dd*)&mem[0x2de2])
#define funcs_115E7 (*(volatile dd*)&mem[0x2fdd])
#define jpt_1164F (*(volatile dd*)&mem[0x3059])
#define jpt_1171C (*(volatile dd*)&mem[0x3121])
#define present_jt (*(volatile dw*)&mem[0x3393])
#define jpt_11A25 (*(volatile dd*)&mem[0x342f])
#define jpt_11BC9 (*(volatile dd*)&mem[0x35d2])
#define jpt_11C4E (*(volatile dd*)&mem[0x3658])
#define funcs_11CB8 (*(volatile dd*)&mem[0x3680])
#define byte_11CF3 (*(volatile db*)&mem[0x3713])
#define funcs_11D52 (*(volatile dd*)&mem[0x3714])
#define jpt_126D5 (*(volatile dd*)&mem[0x40cf])
#define word_126DA (*(volatile dw*)&mem[0x40fa])
#define word_126DC (*(volatile dw*)&mem[0x40fc])
#define jpt_1367D (*(volatile dd*)&mem[0x5087])
#define word_13A90 (*(volatile dw*)&mem[0x54b0])
#define word_13A92 (*(volatile dw*)&mem[0x54b2])
#define word_13BD0 (*(volatile dw*)&mem[0x55f0])
#define funcs_13BF8 (*(volatile dd*)&mem[0x5600])
#define funcs_13CC9 (*(volatile dd*)&mem[0x56d1])
#define funcs_13DA1 (*(volatile dd*)&mem[0x57a9])
#define funcs_13E72 (*(volatile dd*)&mem[0x587a])
#define funcs_13F48 (*(volatile dd*)&mem[0x5950])
#define scroll_tbl_f (*(volatile dw*)&mem[0x5a29])
#define funcs_140FA (*(volatile dd*)&mem[0x5b02])
#define funcs_141CE (*(volatile dd*)&mem[0x5bd6])
#define podhit_tbl (*(volatile dd*)&mem[0x73b0])
#define jpt_16AB9 (*(volatile dd*)&mem[0x84b1])
#define bar_jt (*(volatile dd*)&mem[0x963d])
#define funcs_1892F (*(volatile dd*)&mem[0xa2f0])
#define funcs_1A9C8 (*(volatile dd*)&mem[0xc3d0])
#define jpt_1B098 (*(volatile dd*)&mem[0xcaa6])
#define funcs_1B3E2 (*(volatile dd*)&mem[0xcdf0])
#define funcs_1B41F (*(volatile dd*)&mem[0xce1a])
#define jpt_1B475 (*(volatile dd*)&mem[0xce77])
#define word_1CE80 (*(volatile dw*)&mem[0xe8a0])
#define byte_1CE90 (*(volatile db*)&mem[0xe8b0])
#define word_1CE92 (*(volatile dw*)&mem[0xe8b2])
#define word_1CE94 (*(volatile dw*)&mem[0xe8b4])
#define word_1CE96 (*(volatile dw*)&mem[0xe8b6])
#define word_1CE98 (*(volatile dw*)&mem[0xe8b8])
#define word_1CE9C (*(volatile dw*)&mem[0xe8bc])
#define word_1CEA0 (*(volatile dw*)&mem[0xe8c0])
#define byte_1CEA2 (*(volatile db*)&mem[0xe8c2])
#define word_1CEA3 (*(volatile dw*)&mem[0xe8c3])
#define word_1CEA5 (*(volatile dw*)&mem[0xe8c5])
#define word_1CEB2 (*(volatile dw*)&mem[0xe8d2])
#define word_1CEB4 (*(volatile dw*)&mem[0xe8d4])
#define word_1CEB6 (*(volatile dw*)&mem[0xe8d6])
#define word_1CFF2 (*(volatile dw*)&mem[0xea12])
#define word_1CFF4 (*(volatile dw*)&mem[0xea14])
#define word_1CFF6 (*(volatile dw*)&mem[0xea16])
#define word_1D004 (*(volatile dw*)&mem[0xea24])
#define word_1D006 (*(volatile dw*)&mem[0xea26])
#define word_1D008 (*(volatile dw*)&mem[0xea28])
#define word_1D00A (*(volatile dw*)&mem[0xea2a])
#define word_1D00C (*(volatile dw*)&mem[0xea2c])
#define word_1D00E (*(volatile dw*)&mem[0xea2e])
#define word_1D010 (*(volatile dw*)&mem[0xea30])
#define word_1D012 (*(volatile dw*)&mem[0xea32])
#define word_1D014 (*(volatile dw*)&mem[0xea34])
#define word_1D016 (*(volatile dw*)&mem[0xea36])
#define word_1D018 (*(volatile dw*)&mem[0xea38])
#define word_1D01A (*(volatile dw*)&mem[0xea3a])
#define aFarmchrDtx (*(volatile db*)&mem[0xec94])
#define aArcchrDtx (*(volatile db*)&mem[0xecc4])
#define aMaGchrDtx (*(volatile db*)&mem[0xecf4])
#define aPColtanDtx (*(volatile db*)&mem[0xed6c])
#define aPColegaDtx (*(volatile db*)&mem[0xed84])
#define aPColmcgDtx (*(volatile db*)&mem[0xed9c])
#define aPColhrcDtx (*(volatile db*)&mem[0xedb4])
#define aPRibbonDtx (*(volatile db*)&mem[0xedcc])
#define aCfnegchrDtx (*(volatile db*)&mem[0xeed4])
#define aCanegchrDtx (*(volatile db*)&mem[0xef04])
#define aCidADat (*(volatile db*)&mem[0xef34])
#define word_1D6D8 (*(volatile dw*)&mem[0xf0f8])
#define word_1D6DA (*(volatile dw*)&mem[0xf0fa])
#define byte_1D6DC (*(volatile db*)&mem[0xf0fc])
#define byte_1D6DD (*(volatile db*)&mem[0xf0fd])
#define word_1D6DE (*(volatile dw*)&mem[0xf0fe])
#define word_1D6E2 (*(volatile dw*)&mem[0xf102])
#define word_1D6E4 (*(volatile dw*)&mem[0xf104])
#define word_1D7C0 (*(volatile dw*)&mem[0xf1e0])
#define word_1D8D0 (*(volatile dw*)&mem[0xf2f0])
#define word_1D8D2 (*(volatile dw*)&mem[0xf2f2])
#define word_1D8D4 (*(volatile dw*)&mem[0xf2f4])
#define word_1D8D6 (*(volatile dw*)&mem[0xf2f6])
#define word_1D8D8 (*(volatile dw*)&mem[0xf2f8])
#define word_1D8EA (*(volatile dw*)&mem[0xf30a])
#define word_1D8EC (*(volatile dw*)&mem[0xf30c])
#define word_1D8F6 (*(volatile dw*)&mem[0xf316])
#define word_1D8F8 (*(volatile dw*)&mem[0xf318])
#define word_1D900 (*(volatile dw*)&mem[0xf320])
#define word_1D902 (*(volatile dw*)&mem[0xf322])
#define word_1D916 (*(volatile dw*)&mem[0xf336])
#define word_1D918 (*(volatile dw*)&mem[0xf338])
#define word_1D91C (*(volatile dw*)&mem[0xf33c])
#define word_1D91E (*(volatile dw*)&mem[0xf33e])
#define word_1D920 (*(volatile dw*)&mem[0xf340])
#define word_1D92C (*(volatile dw*)&mem[0xf34c])
#define word_1D92E (*(volatile dw*)&mem[0xf34e])
#define word_1D930 (*(volatile dw*)&mem[0xf350])
#define word_1D934 (*(volatile dw*)&mem[0xf354])
#define byte_1D936 (*(volatile db*)&mem[0xf356])
#define word_1D937 (*(volatile dw*)&mem[0xf357])
#define byte_1D939 (*(volatile db*)&mem[0xf359])
#define byte_1D93A (*(volatile db*)&mem[0xf35a])
#define byte_1D93B (*(volatile db*)&mem[0xf35b])
#define byte_1D93E (*(volatile db*)&mem[0xf35e])
#define word_1D93F (*(volatile dw*)&mem[0xf35f])
#define word_1D943 (*(volatile dw*)&mem[0xf363])
#define word_1D945 (*(volatile dw*)&mem[0xf365])
#define word_1D949 (*(volatile dw*)&mem[0xf369])
#define word_1D94B (*(volatile dw*)&mem[0xf36b])
#define word_1D94D (*(volatile dw*)&mem[0xf36d])
#define word_1D94F (*(volatile dw*)&mem[0xf36f])
#define word_1D951 (*(volatile dw*)&mem[0xf371])
#define word_1D953 (*(volatile dw*)&mem[0xf373])
#define word_1D955 (*(volatile dw*)&mem[0xf375])
#define word_1D957 (*(volatile dw*)&mem[0xf377])
#define word_1D959 (*(volatile dw*)&mem[0xf379])
#define word_1D95D (*(volatile dw*)&mem[0xf37d])
#define word_1D95F (*(volatile dw*)&mem[0xf37f])
#define word_1D961 (*(volatile dw*)&mem[0xf381])
#define word_1D965 (*(volatile dw*)&mem[0xf385])
#define word_1D969 (*(volatile dw*)&mem[0xf389])
#define word_1DBDD (*(volatile dw*)&mem[0xf5fd])
#define byte_1DC65 (*(volatile db*)&mem[0xf685])
#define word_1DC67 (*(volatile dw*)&mem[0xf687])
#define word_1DC69 (*(volatile dw*)&mem[0xf689])
#define word_1DC6B (*(volatile dw*)&mem[0xf68b])
#define word_1DC6D (*(volatile dw*)&mem[0xf68d])
#define word_1DC6F (*(volatile dw*)&mem[0xf68f])
#define word_1DC71 (*(volatile dw*)&mem[0xf691])
#define word_1DC73 (*(volatile dw*)&mem[0xf693])
#define word_1DC75 (*(volatile dw*)&mem[0xf695])
#define word_1DC77 (*(volatile dw*)&mem[0xf697])
#define byte_1DC79 (*(volatile db*)&mem[0xf699])
#define byte_1DC7A (*(volatile db*)&mem[0xf69a])
#define byte_1DC7D (*(volatile db*)&mem[0xf69d])
#define word_1DC80 (*(volatile dw*)&mem[0xf6a0])
#define word_1DC82 (*(volatile dw*)&mem[0xf6a2])
#define word_1DC86 (*(volatile dw*)&mem[0xf6a6])
#define word_1DC88 (*(volatile dw*)&mem[0xf6a8])
#define word_1DC8C (*(volatile dw*)&mem[0xf6ac])
#define word_1DC8E (*(volatile dw*)&mem[0xf6ae])
#define word_1DC92 (*(volatile dw*)&mem[0xf6b2])
#define word_1DC94 (*(volatile dw*)&mem[0xf6b4])
#define word_1DC98 (*(volatile dw*)&mem[0xf6b8])
#define word_1DC9A (*(volatile dw*)&mem[0xf6ba])
#define word_1DC9E (*(volatile dw*)&mem[0xf6be])
#define word_1DCA0 (*(volatile dw*)&mem[0xf6c0])
#define word_1DCA2 (*(volatile dw*)&mem[0xf6c2])
#define word_1DCA4 (*(volatile dw*)&mem[0xf6c4])
#define byte_1DCAA (*(volatile db*)&mem[0xf6ca])
#define byte_1DCAB (*(volatile db*)&mem[0xf6cb])
#define word_1DCAC (*(volatile dw*)&mem[0xf6cc])
#define word_1DCAE (*(volatile dw*)&mem[0xf6ce])
#define byte_1DCB0 (*(volatile db*)&mem[0xf6d0])
#define word_1DCB1 (*(volatile dw*)&mem[0xf6d1])
#define word_1DCB3 (*(volatile dw*)&mem[0xf6d3])
#define word_1DCBF (*(volatile dw*)&mem[0xf6df])
#define byte_1DCC1 (*(volatile db*)&mem[0xf6e1])
#define byte_1DCDD (*(volatile db*)&mem[0xf6fd])
#define word_1DCDF (*(volatile dw*)&mem[0xf6ff])
#define byte_1DCE1 (*(volatile db*)&mem[0xf701])
#define word_1DCFF (*(volatile dw*)&mem[0xf71f])
#define byte_1DD01 (*(volatile db*)&mem[0xf721])
#define byte_1DD05 (*(volatile db*)&mem[0xf725])
#define word_1DD1F (*(volatile dw*)&mem[0xf73f])
#define byte_1DD21 (*(volatile db*)&mem[0xf741])
#define byte_1DD25 (*(volatile db*)&mem[0xf745])
#define word_1DD3F (*(volatile dw*)&mem[0xf75f])
#define byte_1DD41 (*(volatile db*)&mem[0xf761])
#define byte_1DD45 (*(volatile db*)&mem[0xf765])
#define word_1DD5F (*(volatile dw*)&mem[0xf77f])
#define byte_1DD7F (*(volatile db*)&mem[0xf79f])
#define byte_1DD80 (*(volatile db*)&mem[0xf7a0])
#define word_1DD9F (*(volatile dw*)&mem[0xf7bf])
#define word_1DDA1 (*(volatile dw*)&mem[0xf7c1])
#define word_1DDA3 (*(volatile dw*)&mem[0xf7c3])
#define word_1DDA5 (*(volatile dw*)&mem[0xf7c5])
#define word_1DDB0 (*(volatile dw*)&mem[0xf7d0])
#define word_1DDB2 (*(volatile dw*)&mem[0xf7d2])
#define word_1DDB4 (*(volatile dw*)&mem[0xf7d4])
#define word_1DDB9 (*(volatile dw*)&mem[0xf7d9])
#define word_1DDCD (*(volatile dw*)&mem[0xf7ed])
#define word_1DDD3 (*(volatile dw*)&mem[0xf7f3])
#define byte_1DDD9 (*(volatile db*)&mem[0xf7f9])
#define byte_1E0A3 (*(volatile db*)&mem[0xfac3])
#define byte_1E12F (*(volatile db*)&mem[0xfb4f])
#define word_1E1BD (*(volatile dw*)&mem[0xfbdd])
#define word_1E1BF (*(volatile dw*)&mem[0xfbdf])
#define word_1E1C1 (*(volatile dw*)&mem[0xfbe1])
#define word_1E1C3 (*(volatile dw*)&mem[0xfbe3])
#define byte_1E1C5 (*(volatile db*)&mem[0xfbe5])
#define byte_1E1C9 (*(volatile db*)&mem[0xfbe9])
#define byte_1E1CA (*(volatile db*)&mem[0xfbea])
#define word_1E1CB (*(volatile dw*)&mem[0xfbeb])
#define byte_1E1CD (*(volatile db*)&mem[0xfbed])
#define word_1E1CE (*(volatile dw*)&mem[0xfbee])
#define word_1E1D0 (*(volatile dw*)&mem[0xfbf0])
#define word_1E1D2 (*(volatile dw*)&mem[0xfbf2])
#define word_1E1D4 (*(volatile dw*)&mem[0xfbf4])
#define word_1E840 (*(volatile dw*)&mem[0x10260])
#define word_1E842 (*(volatile dw*)&mem[0x10262])
#define word_1E844 (*(volatile dw*)&mem[0x10264])
#define word_1E846 (*(volatile dw*)&mem[0x10266])
#define aAllocated1mbOf (*(volatile db*)&mem[0x10286])
#define word_209E0 (*(volatile dw*)&mem[0x12400])
#define word_209E2 (*(volatile dw*)&mem[0x12402])
#define word_20D38 (*(volatile dw*)&mem[0x12758])
#define word_20D4A (*(volatile dw*)&mem[0x1276a])
#define byte_20D4C (*(volatile db*)&mem[0x1276c])
#define word_20D4E (*(volatile dw*)&mem[0x1276e])
#define byte_20D50 (*(volatile db*)&mem[0x12770])
#define word_20D52 (*(volatile dw*)&mem[0x12772])
#define byte_20D54 (*(volatile db*)&mem[0x12774])
#define word_20D56 (*(volatile dw*)&mem[0x12776])
#define word_20D58 (*(volatile dw*)&mem[0x12778])
#define byte_24E70 (*(volatile db*)raddr_(ds,0x7ff0))
#define byte_24E71 (*(volatile db*)raddr_(ds,0x7ff1))
#define word_24E72 (*(volatile dw*)raddr_(ds,0x7ff2))
#define word_24E74 (*(volatile dw*)raddr_(ds,0x7ff4))
#define byte_24E76 (*(volatile db*)raddr_(ds,0x7ff6))
#define word_24E77 (*(volatile dw*)raddr_(ds,0x7ff7))
#define word_24E79 (*(volatile dw*)raddr_(ds,0x7ff9))
#define byte_24E7B (*(volatile db*)raddr_(ds,0x7ffb))
#define intro_6564 (*(volatile dw*)&mem[0x17f84])
#define tmframe_6566 (*(volatile dw*)&mem[0x17f86])
#define word_2656A (*(volatile dw*)&mem[0x17f8a])
#define word_2656C (*(volatile dw*)&mem[0x17f8c])
#define word_2656E (*(volatile dw*)&mem[0x17f8e])
#define tmframe_6570 (*(volatile dw*)&mem[0x17f90])
#define intro_6574 (*(volatile dw*)&mem[0x17f94])
#define speed_sel (*(volatile dw*)&mem[0x17fa2])
#define word_265A8 (*(volatile dw*)&mem[0x17fc8])
#define word_265AA (*(volatile dw*)&mem[0x17fca])
#define word_265AC (*(volatile dw*)&mem[0x17fcc])
#define word_265AE (*(volatile dw*)&mem[0x17fce])
#define word_265B0 (*(volatile dw*)&mem[0x17fd0])
#define word_265B2 (*(volatile dw*)&mem[0x17fd2])
#define word_265B4 (*(volatile dw*)&mem[0x17fd4])
#define word_26DC0 (*(volatile dw*)&mem[0x187e0])
#define byte_26DCD (*(volatile db*)&mem[0x187ed])
#define word_26DD3 (*(volatile dw*)&mem[0x187f3])
#define byte_26DD5 (*(volatile db*)&mem[0x187f5])
#define byte_26DD6 (*(volatile db*)&mem[0x187f6])
#define byte_26DD7 (*(volatile db*)&mem[0x187f7])
#define word_26DD8 (*(volatile dw*)&mem[0x187f8])
#define word_26DDA (*(volatile dw*)&mem[0x187fa])
#define word_26DDC (*(volatile dw*)&mem[0x187fc])
#define word_26DDE (*(volatile dw*)&mem[0x187fe])
#define word_26DE0 (*(volatile dw*)&mem[0x18800])
#define word_26DE2 (*(volatile dw*)&mem[0x18802])
#define word_26DE4 (*(volatile dw*)&mem[0x18804])
#define byte_26DE6 (*(volatile db*)&mem[0x18806])
#define byte_26DE7 (*(volatile db*)&mem[0x18807])
#define byte_26DE8 (*(volatile db*)&mem[0x18808])
#define byte_26DE9 (*(volatile db*)&mem[0x18809])
#define byte_26DEA (*(volatile db*)&mem[0x1880a])
#define word_26DEB (*(volatile dw*)&mem[0x1880b])
#define word_26DED (*(volatile dw*)&mem[0x1880d])
#define byte_26DEF (*(volatile db*)&mem[0x1880f])
#define byte_26DF0 (*(volatile db*)&mem[0x18810])
#define byte_26DF1 (*(volatile db*)&mem[0x18811])
#define byte_26DF2 (*(volatile db*)&mem[0x18812])
#define byte_26DF3 (*(volatile db*)&mem[0x18813])
#define word_26DF4 (*(volatile dw*)&mem[0x18814])
#define word_26DF6 (*(volatile dw*)&mem[0x18816])
#define word_26DF8 (*(volatile dw*)&mem[0x18818])
#define byte_26DFA (*(volatile db*)&mem[0x1881a])
#define byte_26DFB (*(volatile db*)&mem[0x1881b])
#define byte_26DFC (*(volatile db*)&mem[0x1881c])
#define member_idx (*(volatile db*)&mem[0x18b69])
#define mission_idx (*(volatile dw*)&mem[0x18b70])
#define byte_27152 (*(volatile db*)&mem[0x18b72])
#define alarm_flag (*(volatile db*)&mem[0x18b73])
#define sum_acc (*(volatile dw*)&mem[0x18b74])
#define sum_ovf_b (*(volatile db*)&mem[0x18b76])
#define byte_27157 (*(volatile db*)&mem[0x18b77])
#define sum_ovf_a (*(volatile db*)&mem[0x18b78])
#define byte_27166 (*(volatile db*)&mem[0x18b86])
#define byte_27167 (*(volatile db*)&mem[0x18b87])
#define mselprep_7169 (*(volatile dw*)&mem[0x18b89])
#define byte_271D5 (*(volatile db*)&mem[0x18bf5])
#define byte_2731E (*(volatile db*)&mem[0x18d3e])
#define word_2732F (*(volatile dw*)&mem[0x18d4f])
#define main_men_7403 (*(volatile dw*)&mem[0x18e23])
#define word_2762F (*(volatile dw*)&mem[0x1904f])
#define brief_sc_7634 (*(volatile dw*)&mem[0x19054])
#define byte_281ED (*(volatile db*)&mem[0x19c0d])
#define byte_281EE (*(volatile db*)&mem[0x19c0e])
#define word_2824B (*(volatile dw*)&mem[0x19c6b])
#define mseloop_8341 (*(volatile dw*)&mem[0x19d61])
#define menu_sel (*(volatile dw*)&mem[0x19d63])
#define menu_nav_8345 (*(volatile dw*)&mem[0x19d65])
#define mseloop_8347 (*(volatile dw*)&mem[0x19d67])
#define mseloop_8349 (*(volatile dw*)&mem[0x19d69])
#define word_2860A (*(volatile dw*)&mem[0x1a02a])
#define inmenu_8677 (*(volatile db*)&mem[0x1a097])
#define menu_scr_8730 (*(volatile dw*)&mem[0x1a150])
#define byte_289E2 (*(volatile db*)&mem[0x1a402])
#define byte_289E3 (*(volatile db*)&mem[0x1a403])
#define aCom1Com2BstrSs (*(volatile db*)&mem[0x1a446])
#define byte_28A50 (*(volatile db*)&mem[0x1a470])
#define word_28ACE (*(volatile dw*)&mem[0x1a4ee])
#define word_28AD0 (*(volatile dw*)&mem[0x1a4f0])
#define byte_28AD2 (*(volatile db*)&mem[0x1a4f2])
#define word_28C83 (*(volatile dw*)&mem[0x1a6a3])
#define word_28C90 (*(volatile dw*)&mem[0x1a6b0])
#define word_28C92 (*(volatile dw*)&mem[0x1a6b2])
#define byte_28C94 (*(volatile db*)&mem[0x1a6b4])
#define byte_28C95 (*(volatile db*)&mem[0x1a6b5])
#define byte_28C96 (*(volatile db*)&mem[0x1a6b6])
#define byte_28C97 (*(volatile db*)&mem[0x1a6b7])
#define byte_28CA0 (*(volatile db*)&mem[0x1a6c0])
#define byte_28CA1 (*(volatile db*)&mem[0x1a6c1])
#define byte_28CA2 (*(volatile db*)&mem[0x1a6c2])
#define byte_28CA3 (*(volatile db*)&mem[0x1a6c3])
#define word_28CAC (*(volatile dw*)&mem[0x1a6cc])
#define byte_28CB1 (*(volatile db*)&mem[0x1a6d1])
#define byte_28CB3 (*(volatile db*)&mem[0x1a6d3])
#define byte_28CB4 (*(volatile db*)&mem[0x1a6d4])
#define byte_28CC0 (*(volatile db*)&mem[0x1a6e0])
#define byte_28CC1 (*(volatile db*)&mem[0x1a6e1])
#define byte_28CC2 (*(volatile db*)&mem[0x1a6e2])
#define byte_28CC3 (*(volatile db*)&mem[0x1a6e3])
#define byte_28CC4 (*(volatile db*)&mem[0x1a6e4])
#define byte_28CC5 (*(volatile db*)&mem[0x1a6e5])
#define byte_28CC8 (*(volatile db*)&mem[0x1a6e8])
#define byte_28CCA (*(volatile db*)&mem[0x1a6ea])
#define byte_28CCB (*(volatile db*)&mem[0x1a6eb])
#define byte_28CD1 (*(volatile db*)&mem[0x1a6f1])
#define byte_28CD2 (*(volatile db*)&mem[0x1a6f2])
#define byte_28CD3 (*(volatile db*)&mem[0x1a6f3])
#define byte_28CD4 (*(volatile db*)&mem[0x1a6f4])
#define word_28CD5 (*(volatile dw*)&mem[0x1a6f5])
#define byte_28CD8 (*(volatile db*)&mem[0x1a6f8])
#define byte_28CD9 (*(volatile db*)&mem[0x1a6f9])
#define byte_28CDB (*(volatile db*)&mem[0x1a6fb])
#define byte_28CDC (*(volatile db*)&mem[0x1a6fc])
#define byte_28D0D (*(volatile db*)&mem[0x1a72d])
#define byte_28D0E (*(volatile db*)&mem[0x1a72e])
#define byte_28D51 (*(volatile db*)&mem[0x1a771])
#define byte_28DFB (*(volatile db*)&mem[0x1a81b])
#define byte_28E1D (*(volatile db*)&mem[0x1a83d])
#define byte_28E3F (*(volatile db*)&mem[0x1a85f])
#define byte_28E61 (*(volatile db*)&mem[0x1a881])
#define byte_28E83 (*(volatile db*)&mem[0x1a8a3])
#define byte_28F2D (*(volatile db*)&mem[0x1a94d])
#define byte_2901B (*(volatile db*)&mem[0x1aa3b])
#define byte_2903D (*(volatile db*)&mem[0x1aa5d])
#define byte_290A3 (*(volatile db*)&mem[0x1aac3])
#define byte_2914D (*(volatile db*)&mem[0x1ab6d])
#define byte_2914E (*(volatile db*)&mem[0x1ab6e])
#define byte_2916F (*(volatile db*)&mem[0x1ab8f])
#define csetup_9170 (*(volatile db*)&mem[0x1ab90])
#define byte_291B3 (*(volatile db*)&mem[0x1abd3])
#define byte_291D5 (*(volatile db*)&mem[0x1abf5])
#define byte_29219 (*(volatile db*)&mem[0x1ac39])
#define byte_2923B (*(volatile db*)&mem[0x1ac5b])
#define ot23_tick_9307 (*(volatile db*)&mem[0x1ad27])
#define aux7_9537 (*(volatile db*)&mem[0x1af57])
#define aux7_9538 (*(volatile db*)&mem[0x1af58])
#define aux7_9539 (*(volatile db*)&mem[0x1af59])
#define rstate_9567 (*(volatile db*)&mem[0x1af87])
#define byte_295EA (*(volatile db*)&mem[0x1b00a])
#define word_295EB (*(volatile dw*)&mem[0x1b00b])
#define byte_295EF (*(volatile db*)&mem[0x1b00f])
#define scan_id (*(volatile dw*)&mem[0x1b088])
#define word_2966A (*(volatile dw*)&mem[0x1b08a])
#define word_2966C (*(volatile dw*)&mem[0x1b08c])
#define word_2966E (*(volatile dw*)&mem[0x1b08e])
#define apply_sc_9670 (*(volatile dw*)&mem[0x1b090])
#define word_29676 (*(volatile dw*)&mem[0x1b096])
#define word_29678 (*(volatile dw*)&mem[0x1b098])
#define byte_2967A (*(volatile db*)&mem[0x1b09a])
#define word_2967B (*(volatile dw*)&mem[0x1b09b])
#define word_2967D (*(volatile dw*)&mem[0x1b09d])
#define word_29681 (*(volatile dw*)&mem[0x1b0a1])
#define cam_org_x (*(volatile dw*)&mem[0x1b0a3])
#define byte_2970D (*(volatile db*)&mem[0x1b12d])
#define byte_2970E (*(volatile db*)&mem[0x1b12e])
#define byte_2970F (*(volatile db*)&mem[0x1b12f])
#define byte_29710 (*(volatile db*)&mem[0x1b130])
#define weapon_sel (*(volatile db*)&mem[0x1b131])
#define byte_29712 (*(volatile db*)&mem[0x1b132])
#define flash_period (*(volatile db*)&mem[0x1b134])
#define hit_flash (*(volatile db*)&mem[0x1b135])
#define byte_29716 (*(volatile db*)&mem[0x1b136])
#define byte_29717 (*(volatile db*)&mem[0x1b137])
#define fade_cnt (*(volatile db*)&mem[0x1b138])
#define byte_2971A (*(volatile db*)&mem[0x1b13a])
#define ot13_tick_9721 (*(volatile db*)&mem[0x1b141])
#define byte_29722 (*(volatile db*)&mem[0x1b142])
#define ot13_tick_9723 (*(volatile db*)&mem[0x1b143])
#define ot14_tick_9724 (*(volatile db*)&mem[0x1b144])
#define roster_9733 (*(volatile db*)&mem[0x1b153])
#define roster_9734 (*(volatile db*)&mem[0x1b154])
#define roster_9735 (*(volatile db*)&mem[0x1b155])
#define roster_9736 (*(volatile db*)&mem[0x1b156])
#define roster_9738 (*(volatile db*)&mem[0x1b158])
#define byte_2974F (*(volatile db*)&mem[0x1b16f])
#define scan_tgt (*(volatile db*)&mem[0x1b170])
#define byte_29761 (*(volatile db*)&mem[0x1b181])
#define ai_seek__9762 (*(volatile db*)&mem[0x1b182])
#define pri_best (*(volatile db*)&mem[0x1b183])
#define pri_a (*(volatile db*)&mem[0x1b184])
#define pri_b (*(volatile db*)&mem[0x1b185])
#define pri_c (*(volatile db*)&mem[0x1b186])
#define pri_fiel_9767 (*(volatile db*)&mem[0x1b187])
#define ai_drive_9768 (*(volatile db*)&mem[0x1b188])
#define trk_flag_b (*(volatile db*)&mem[0x1b189])
#define byte_2976A (*(volatile db*)&mem[0x1b18a])
#define byte_2976B (*(volatile db*)&mem[0x1b18b])
#define byte_2976C (*(volatile db*)&mem[0x1b18c])
#define word_297E1 (*(volatile dw*)&mem[0x1b201])
#define word_297F3 (*(volatile dw*)&mem[0x1b213])
#define word_29812 (*(volatile dw*)&mem[0x1b232])
#define cam_px_x (*(volatile dw*)&mem[0x1b234])
#define cam_px_y (*(volatile dw*)&mem[0x1b236])
#define burst_dir (*(volatile dw*)&mem[0x1b238])
#define udraw_9845 (*(volatile dw*)&mem[0x1b265])
#define udraw_9847 (*(volatile db*)&mem[0x1b267])
#define udraw_9848 (*(volatile db*)&mem[0x1b268])
#define byte_2984B (*(volatile db*)&mem[0x1b26b])
#define byte_2984C (*(volatile db*)&mem[0x1b26c])
#define byte_2984D (*(volatile db*)&mem[0x1b26d])
#define byte_2984E (*(volatile db*)&mem[0x1b26e])
#define byte_2984F (*(volatile db*)&mem[0x1b26f])
#define word_298A2 (*(volatile dw*)&mem[0x1b2c2])
#define word_298A4 (*(volatile dw*)&mem[0x1b2c4])
#define byte_298A6 (*(volatile db*)&mem[0x1b2c6])
#define word_298AC (*(volatile dw*)&mem[0x1b2cc])
#define word_298AE (*(volatile dw*)&mem[0x1b2ce])
#define word_298B2 (*(volatile dw*)&mem[0x1b2d2])
#define word_298C0 (*(volatile dw*)&mem[0x1b2e0])
#define word_298C2 (*(volatile dw*)&mem[0x1b2e2])
#define byte_298C4 (*(volatile db*)&mem[0x1b2e4])
#define facing_c_9955 (*(volatile dw*)&mem[0x1b375])
#define byte_2998F (*(volatile db*)&mem[0x1b3af])
#define self_id (*(volatile db*)&mem[0x1b3b9])
#define byte_2999A (*(volatile db*)&mem[0x1b3ba])
#define byte_299AF (*(volatile db*)&mem[0x1b3cf])
#define word_29B42 (*(volatile dw*)&mem[0x1b562])
#define word_29B44 (*(volatile dw*)&mem[0x1b564])
#define byte_29B4F (*(volatile db*)&mem[0x1b56f])
#define byte_29B50 (*(volatile db*)&mem[0x1b570])
#define byte_29B51 (*(volatile db*)&mem[0x1b571])
#define word_29B52 (*(volatile dw*)&mem[0x1b572])
#define word_29B54 (*(volatile dw*)&mem[0x1b574])
#define word_29B56 (*(volatile dw*)&mem[0x1b576])
#define word_29B58 (*(volatile dw*)&mem[0x1b578])
#define byte_29B5A (*(volatile db*)&mem[0x1b57a])
#define byte_29B5B (*(volatile db*)&mem[0x1b57b])
#define byte_29B5C (*(volatile db*)&mem[0x1b57c])
#define byte_29B5D (*(volatile db*)&mem[0x1b57d])
#define byte_29B5E (*(volatile db*)&mem[0x1b57e])
#define byte_29B5F (*(volatile db*)&mem[0x1b57f])
#define byte_29B60 (*(volatile db*)&mem[0x1b580])
#define byte_29B61 (*(volatile db*)&mem[0x1b581])
#define byte_29B62 (*(volatile db*)&mem[0x1b582])
#define word_29B63 (*(volatile dw*)&mem[0x1b583])
#define word_29B95 (*(volatile dw*)&mem[0x1b5b5])
#define word_29B97 (*(volatile dw*)&mem[0x1b5b7])
#define word_29E36 (*(volatile dw*)&mem[0x1b856])
#define word_29E38 (*(volatile dw*)&mem[0x1b858])
#define word_29E3A (*(volatile dw*)&mem[0x1b85a])
#define word_29E3C (*(volatile dw*)&mem[0x1b85c])
#define word_29E3E (*(volatile dw*)&mem[0x1b85e])
#define word_29E40 (*(volatile dw*)&mem[0x1b860])
#define word_29EBC (*(volatile dw*)&mem[0x1b8dc])
#define byte_29F5E (*(volatile db*)&mem[0x1b97e])
#define word_29F60 (*(volatile dw*)&mem[0x1b980])
#define word_29F64 (*(volatile dw*)&mem[0x1b984])
#define word_29F66 (*(volatile dw*)&mem[0x1b986])
#define byte_29F76 (*(volatile db*)&mem[0x1b996])
#define byte_29F77 (*(volatile db*)&mem[0x1b997])
#define word_29F78 (*(volatile dw*)&mem[0x1b998])
#define word_2A0DD (*(volatile dw*)&mem[0x1bafd])
#define word_2A0EA (*(volatile dw*)&mem[0x1bb0a])
#define word_2A10D (*(volatile dw*)&mem[0x1bb2d])
#define word_2A12A (*(volatile dw*)&mem[0x1bb4a])
#define word_2A147 (*(volatile dw*)&mem[0x1bb67])
#define byte_2A30D (*(volatile db*)&mem[0x1bd2d])
#define byte_2A33F (*(volatile db*)&mem[0x1bd5f])
#define byte_2A3B0 (*(volatile db*)&mem[0x1bdd0])
#define byte_2A400 (*(volatile db*)&mem[0x1be20])
#define byte_2A441 (*(volatile db*)&mem[0x1be61])
#define byte_2A4CA (*(volatile db*)&mem[0x1beea])
#define byte_2A506 (*(volatile db*)&mem[0x1bf26])
#define word_2A507 (*(volatile dw*)&mem[0x1bf27])
#define word_2A509 (*(volatile dw*)&mem[0x1bf29])
#define word_2A5A3 (*(volatile dw*)&mem[0x1bfc3])
#define word_2A5A5 (*(volatile dw*)&mem[0x1bfc5])
#define word_2A5A7 (*(volatile dw*)&mem[0x1bfc7])
#define byte_2A5B0 (*(volatile db*)&mem[0x1bfd0])
#define byte_2A5B1 (*(volatile db*)&mem[0x1bfd1])
#define word_2A5B2 (*(volatile dw*)&mem[0x1bfd2])
#define word_2A5B4 (*(volatile dw*)&mem[0x1bfd4])
#define word_2A5B6 (*(volatile dw*)&mem[0x1bfd6])
#define byte_2A5EE (*(volatile db*)&mem[0x1c00e])
#define byte_2A5EF (*(volatile db*)&mem[0x1c00f])
#define byte_2A5F0 (*(volatile db*)&mem[0x1c010])
#define byte_2A5F1 (*(volatile db*)&mem[0x1c011])
#define byte_2A5F2 (*(volatile db*)&mem[0x1c012])
#define byte_2A5F3 (*(volatile db*)&mem[0x1c013])
#define byte_2A5F4 (*(volatile db*)&mem[0x1c014])
#define byte_2A5F5 (*(volatile db*)&mem[0x1c015])
#define byte_2A5F6 (*(volatile db*)&mem[0x1c016])
#define byte_2A5F7 (*(volatile db*)&mem[0x1c017])
#define byte_2A5F8 (*(volatile db*)&mem[0x1c018])
#define byte_2A5F9 (*(volatile db*)&mem[0x1c019])
#define byte_2A5FA (*(volatile db*)&mem[0x1c01a])
#define byte_2A5FB (*(volatile db*)&mem[0x1c01b])
#define byte_2A5FC (*(volatile db*)&mem[0x1c01c])
#define word_2A5FD (*(volatile dw*)&mem[0x1c01d])
#define word_2A5FF (*(volatile dw*)&mem[0x1c01f])
#define byte_2A601 (*(volatile db*)&mem[0x1c021])
#define byte_2A602 (*(volatile db*)&mem[0x1c022])
#define byte_2A603 (*(volatile db*)&mem[0x1c023])
#define byte_2A604 (*(volatile db*)&mem[0x1c024])
#define byte_2A605 (*(volatile db*)&mem[0x1c025])
#define byte_2A606 (*(volatile db*)&mem[0x1c026])
#define byte_2A60F (*(volatile db*)&mem[0x1c02f])
#define word_2A610 (*(volatile dw*)&mem[0x1c030])
#define byte_2A612 (*(volatile db*)&mem[0x1c032])
#define byte_2A613 (*(volatile db*)&mem[0x1c033])
#define byte_2A614 (*(volatile db*)&mem[0x1c034])
#define byte_2A615 (*(volatile db*)&mem[0x1c035])
#define byte_2A6AF (*(volatile db*)&mem[0x1c0cf])
#define byte_2A6B0 (*(volatile db*)&mem[0x1c0d0])
#define byte_2A6B1 (*(volatile db*)&mem[0x1c0d1])
#define byte_2A6B2 (*(volatile db*)&mem[0x1c0d2])
#define byte_2A6B3 (*(volatile db*)&mem[0x1c0d3])
#define byte_2A6B4 (*(volatile db*)&mem[0x1c0d4])
#define byte_2A6B5 (*(volatile db*)&mem[0x1c0d5])
#define byte_2A6BE (*(volatile db*)&mem[0x1c0de])
#define word_2A6BF (*(volatile dw*)&mem[0x1c0df])
#define byte_2A6C1 (*(volatile db*)&mem[0x1c0e1])
#define byte_2A6C2 (*(volatile db*)&mem[0x1c0e2])
#define byte_2A6C3 (*(volatile db*)&mem[0x1c0e3])
#define byte_2A6C4 (*(volatile db*)&mem[0x1c0e4])
#define byte_2A6C5 (*(volatile db*)&mem[0x1c0e5])
#define byte_2A6C6 (*(volatile db*)&mem[0x1c0e6])
#define byte_2A74B (*(volatile db*)&mem[0x1c16b])
#define word_2A7A8 (*(volatile dw*)&mem[0x1c1c8])
#define byte_2A7AA (*(volatile db*)&mem[0x1c1ca])
#define byte_2A7AB (*(volatile db*)&mem[0x1c1cb])
#define byte_2A7AC (*(volatile db*)&mem[0x1c1cc])
#define byte_2A7AD (*(volatile db*)&mem[0x1c1cd])
#define byte_2A7AE (*(volatile db*)&mem[0x1c1ce])
#define byte_2A7AF (*(volatile db*)&mem[0x1c1cf])
#define word_2A7B0 (*(volatile dw*)&mem[0x1c1d0])
#define byte_2A86B (*(volatile db*)&mem[0x1c28b])
#define byte_2A86C (*(volatile db*)&mem[0x1c28c])
#define word_2A89C (*(volatile dw*)&mem[0x1c2bc])
#define word_2A89E (*(volatile dw*)&mem[0x1c2be])
#define word_2A8A0 (*(volatile dw*)&mem[0x1c2c0])
#define word_2A8A2 (*(volatile dw*)&mem[0x1c2c2])
#define byte_2A8B0 (*(volatile db*)&mem[0x1c2d0])
#define byte_2A90A (*(volatile db*)&mem[0x1c32a])
#define a0 (*(volatile db*)&mem[0x1c32b])
#define word_2A933 (*(volatile dw*)&mem[0x1c353])
#define byte_2A960 (*(volatile db*)&mem[0x1c380])
#define byte_2A961 (*(volatile db*)&mem[0x1c381])
#define word_2A962 (*(volatile dw*)&mem[0x1c382])
#define word_2A968 (*(volatile dw*)&mem[0x1c388])
#define word_2A96A (*(volatile dw*)&mem[0x1c38a])
#define byte_2A96C (*(volatile db*)&mem[0x1c38c])
#define word_2A99D (*(volatile dw*)&mem[0x1c3bd])
#define byte_2A99F (*(volatile db*)&mem[0x1c3bf])
#define byte_2A9CA (*(volatile db*)&mem[0x1c3ea])
#define byte_2A9CB (*(volatile db*)&mem[0x1c3eb])
#define byte_2A9CD (*(volatile db*)&mem[0x1c3ed])
#define word_2A9CE (*(volatile dw*)&mem[0x1c3ee])
#define word_2A9D0 (*(volatile dw*)&mem[0x1c3f0])
#define byte_2A9D2 (*(volatile db*)&mem[0x1c3f2])
#define byte_2A9D3 (*(volatile db*)&mem[0x1c3f3])
#define word_2A9D4 (*(volatile dw*)&mem[0x1c3f4])
#define word_2A9D6 (*(volatile dw*)&mem[0x1c3f6])
#define byte_2A9D8 (*(volatile db*)&mem[0x1c3f8])
#define byte_2A9DF (*(volatile db*)&mem[0x1c3ff])
#define byte_2A9E0 (*(volatile db*)&mem[0x1c400])
#define byte_2A9E1 (*(volatile db*)&mem[0x1c401])
#define byte_2A9E2 (*(volatile db*)&mem[0x1c402])
#define byte_2A9E3 (*(volatile db*)&mem[0x1c403])
#define byte_2A9E4 (*(volatile db*)&mem[0x1c404])
#define byte_2A9F9 (*(volatile db*)&mem[0x1c419])
#define byte_2A9FA (*(volatile db*)&mem[0x1c41a])
#define word_2A9FB (*(volatile dw*)&mem[0x1c41b])
#define word_2AA0D (*(volatile dw*)&mem[0x1c42d])
#define word_2AA40 (*(volatile dw*)&mem[0x1c460])
#define word_2AA42 (*(volatile dw*)&mem[0x1c462])
#define word_2AA44 (*(volatile dw*)&mem[0x1c464])
#define word_2AA46 (*(volatile dw*)&mem[0x1c466])
#define word_2AA48 (*(volatile dw*)&mem[0x1c468])
#define word_2AA4A (*(volatile dw*)&mem[0x1c46a])
#define word_2AA56 (*(volatile dw*)&mem[0x1c476])
#define word_2AA58 (*(volatile dw*)&mem[0x1c478])
#define byte_2AA5C (*(volatile db*)&mem[0x1c47c])
#define byte_2AA5D (*(volatile db*)&mem[0x1c47d])
#define byte_2AA5E (*(volatile db*)&mem[0x1c47e])
#define byte_2AA5F (*(volatile db*)&mem[0x1c47f])
#define byte_2AA60 (*(volatile db*)&mem[0x1c480])
#define byte_2AA61 (*(volatile db*)&mem[0x1c481])
#define byte_2AA62 (*(volatile db*)&mem[0x1c482])
#define byte_2AA63 (*(volatile db*)&mem[0x1c483])
#define byte_2AA64 (*(volatile db*)&mem[0x1c484])
#define word_2AA65 (*(volatile dw*)&mem[0x1c485])
#define word_2AA67 (*(volatile dw*)&mem[0x1c487])
#define word_2AA69 (*(volatile dw*)&mem[0x1c489])
#define word_2AA6B (*(volatile dw*)&mem[0x1c48b])
#define byte_2AA6D (*(volatile db*)&mem[0x1c48d])
#define byte_2AA6E (*(volatile db*)&mem[0x1c48e])
#define byte_2AA6F (*(volatile db*)&mem[0x1c48f])
#define word_2B070 (*(volatile dw*)&mem[0x1ca90])
#define word_2B074 (*(volatile dw*)&mem[0x1ca94])
#define byte_2B076 (*(volatile db*)&mem[0x1ca96])
#define byte_2B0C3 (*(volatile db*)&mem[0x1cae3])
#define word_2B0C4 (*(volatile dw*)&mem[0x1cae4])
#define byte_2B0C6 (*(volatile db*)&mem[0x1cae6])
#define byte_2B0C7 (*(volatile db*)&mem[0x1cae7])
#define word_2B0C8 (*(volatile dw*)&mem[0x1cae8])
#define byte_2B0CA (*(volatile db*)&mem[0x1caea])
#define word_2B0CB (*(volatile dw*)&mem[0x1caeb])
#define word_2B0CD (*(volatile dw*)&mem[0x1caed])
#define byte_2B0CF (*(volatile db*)&mem[0x1caef])
#define byte_2B0D0 (*(volatile db*)&mem[0x1caf0])
#define byte_2B0D1 (*(volatile db*)&mem[0x1caf1])
#define byte_2B0D2 (*(volatile db*)&mem[0x1caf2])
#define byte_2B0D3 (*(volatile db*)&mem[0x1caf3])
#define byte_2B0D4 (*(volatile db*)&mem[0x1caf4])
#define byte_2B115 (*(volatile db*)&mem[0x1cb35])
#define byte_2B116 (*(volatile db*)&mem[0x1cb36])
#define byte_2B117 (*(volatile db*)&mem[0x1cb37])
#define byte_2B118 (*(volatile db*)&mem[0x1cb38])
#define byte_2B119 (*(volatile db*)&mem[0x1cb39])
#define byte_2B11A (*(volatile db*)&mem[0x1cb3a])
#define word_2B11B (*(volatile dw*)&mem[0x1cb3b])
#define byte_2B11D (*(volatile db*)&mem[0x1cb3d])
#define byte_2B11E (*(volatile db*)&mem[0x1cb3e])
#define byte_2B11F (*(volatile db*)&mem[0x1cb3f])
#define byte_2B120 (*(volatile db*)&mem[0x1cb40])
#define word_2B121 (*(volatile dw*)&mem[0x1cb41])
#define byte_2B123 (*(volatile db*)&mem[0x1cb43])
#define byte_2B126 (*(volatile db*)&mem[0x1cb46])
#define byte_2B127 (*(volatile db*)&mem[0x1cb47])
#define byte_2B128 (*(volatile db*)&mem[0x1cb48])
#define byte_2B129 (*(volatile db*)&mem[0x1cb49])
#define byte_2B12A (*(volatile db*)&mem[0x1cb4a])
#define byte_2B12B (*(volatile db*)&mem[0x1cb4b])
#define byte_2B12C (*(volatile db*)&mem[0x1cb4c])
#define byte_2B12D (*(volatile db*)&mem[0x1cb4d])
#define byte_2B12E (*(volatile db*)&mem[0x1cb4e])
#define byte_2B12F (*(volatile db*)&mem[0x1cb4f])
#define byte_2B130 (*(volatile db*)&mem[0x1cb50])
#define byte_2B131 (*(volatile db*)&mem[0x1cb51])
#define byte_2B132 (*(volatile db*)&mem[0x1cb52])
#define byte_2B23C (*(volatile db*)&mem[0x1cc5c])
#define byte_2B23D (*(volatile db*)&mem[0x1cc5d])
#define word_2B23E (*(volatile dw*)&mem[0x1cc5e])
#define word_2B240 (*(volatile dw*)&mem[0x1cc60])
#define byte_2B242 (*(volatile db*)&mem[0x1cc62])
#define byte_2B243 (*(volatile db*)&mem[0x1cc63])
#define byte_2B244 (*(volatile db*)&mem[0x1cc64])
#define word_2B245 (*(volatile dw*)&mem[0x1cc65])
#define word_2B247 (*(volatile dw*)&mem[0x1cc67])
#define byte_2B24D (*(volatile db*)&mem[0x1cc6d])
#define word_2B24F (*(volatile dw*)&mem[0x1cc6f])
#define word_2B251 (*(volatile dw*)&mem[0x1cc71])
#define word_2B253 (*(volatile dw*)&mem[0x1cc73])
#define byte_2B2CE (*(volatile db*)&mem[0x1ccee])
#define byte_2B3F6 (*(volatile db*)&mem[0x1ce16])
#define byte_2B3F7 (*(volatile db*)&mem[0x1ce17])
#define byte_2B510 (*(volatile db*)&mem[0x1cf30])
extern db default_seg;
extern db seg000;
#define seg_screen (*(volatile dw*)&mem[0x1a2e]) /* alias seg_1000E */
extern dw dummy50a3c745_seg000_1a3a_48;
extern dw dummy50a3c745_seg000_1a3c_49;
#define word_100c3 (*(volatile dw*)&mem[0x1ae3]) /* alias word_100C3 */
extern db dummy50a3c745_seg000_1b1a_170;
extern db dummy50a3c745_seg000_1d8e_565;
extern db dummy50a3c745_seg000_1dac_589;
#define jpt_1039d (*(volatile dw*)&mem[0x1dad]) /* alias pal_jt */
extern dw dummy50a3c745_seg000_1daf_591;
extern dw dummy50a3c745_seg000_1db1_592;
extern dw dummy50a3c745_seg000_1db3_593;
extern dw dummy50a3c745_seg000_1db5_594;
#define glyphconv_jt (*(volatile dw*)&mem[0x21ad]) /* alias jpt_107B6 */
extern dw dummy50a3c745_seg000_21af_1227;
extern dw dummy50a3c745_seg000_21b1_1228;
extern dw dummy50a3c745_seg000_21b3_1229;
extern dw dummy50a3c745_seg000_21b5_1230;
#define funcs_1083a (*(volatile dw*)&mem[0x21db]) /* alias modecall_tbl */
extern dw dummy50a3c745_seg000_21dd_1256;
extern dw dummy50a3c745_seg000_21df_1257;
extern dw dummy50a3c745_seg000_21e1_1258;
extern db dummy50a3c745_seg000_24d4_1729;
#define jpt_10ad3 (*(volatile dw*)&mem[0x24d5]) /* alias clrflip_jt */
extern dw dummy50a3c745_seg000_24d7_1731;
extern dw dummy50a3c745_seg000_24d9_1732;
extern dw dummy50a3c745_seg000_24db_1733;
extern dw dummy50a3c745_seg000_24dd_1734;
#define jpt_10b34 (*(volatile dw*)&mem[0x2536]) /* alias clrdraw_jt */
extern dw dummy50a3c745_seg000_2538_1818;
extern dw dummy50a3c745_seg000_253a_1819;
extern dw dummy50a3c745_seg000_253c_1820;
extern dw dummy50a3c745_seg000_253e_1821;
#define jpt_10b57 (*(volatile dw*)&mem[0x2559]) /* alias clrback_jt */
extern dw dummy50a3c745_seg000_255b_1842;
extern dw dummy50a3c745_seg000_255d_1843;
extern dw dummy50a3c745_seg000_255f_1844;
extern dw dummy50a3c745_seg000_2561_1845;
#define glyphblit_jt (*(volatile dw*)&mem[0x25ec]) /* alias jpt_10BEF */
extern dw dummy50a3c745_seg000_25ee_1998;
extern dw dummy50a3c745_seg000_25f0_1999;
extern dw dummy50a3c745_seg000_25f2_2000;
extern dw dummy50a3c745_seg000_25f4_2001;
#define glyphblit2_jt (*(volatile dw*)&mem[0x264c]) /* alias jpt_10C4F */
extern dw dummy50a3c745_seg000_264e_2073;
extern dw dummy50a3c745_seg000_2650_2074;
extern dw dummy50a3c745_seg000_2652_2075;
extern dw dummy50a3c745_seg000_2654_2076;
extern db dummy50a3c745_seg000_2cd6_2971;
#define jpt_113e3 (*(volatile dw*)&mem[0x2de2]) /* alias vidmode_jt */
extern dw dummy50a3c745_seg000_2de4_3158;
extern dw dummy50a3c745_seg000_2de6_3159;
extern dw dummy50a3c745_seg000_2de8_3160;
extern dw dummy50a3c745_seg000_2dea_3161;
#define glyph_put_tbl (*(volatile dw*)&mem[0x2fdd]) /* alias funcs_115E7 */
extern dw dummy50a3c745_seg000_2fdf_3485;
extern dw dummy50a3c745_seg000_2fe1_3486;
extern dw dummy50a3c745_seg000_2fe3_3487;
extern dw dummy50a3c745_seg000_2fe5_3488;
extern dw dummy50a3c745_seg000_2fe7_3489;
extern dw dummy50a3c745_seg000_2fe9_3490;
extern dw dummy50a3c745_seg000_2feb_3491;
extern dw dummy50a3c745_seg000_2fed_3492;
extern dw dummy50a3c745_seg000_2fef_3493;
#define bufsel_jt (*(volatile dw*)&mem[0x3059]) /* alias jpt_1164F */
extern dw dummy50a3c745_seg000_305b_3561;
extern dw dummy50a3c745_seg000_305d_3562;
extern dw dummy50a3c745_seg000_305f_3563;
extern dw dummy50a3c745_seg000_3061_3564;
extern db dummy50a3c745_seg000_310a_3654;
extern db dummy50a3c745_seg000_3120_3670;
#define tileblit_jt (*(volatile dw*)&mem[0x3121]) /* alias jpt_1171C */
extern dw dummy50a3c745_seg000_3123_3672;
extern dw dummy50a3c745_seg000_3125_3673;
extern dw dummy50a3c745_seg000_3127_3674;
extern dw dummy50a3c745_seg000_3129_3675;
extern db dummy50a3c745_seg000_3146_3695;
extern dw dummy50a3c745_seg000_3395_4001;
extern dw dummy50a3c745_seg000_3397_4002;
extern dw dummy50a3c745_seg000_3399_4003;
extern dw dummy50a3c745_seg000_339b_4004;
#define flip_jt (*(volatile dw*)&mem[0x342f]) /* alias jpt_11A25 */
extern dw dummy50a3c745_seg000_3431_4099;
extern dw dummy50a3c745_seg000_3433_4100;
extern dw dummy50a3c745_seg000_3435_4101;
extern dw dummy50a3c745_seg000_3437_4102;
extern db dummy50a3c745_seg000_3548_4222;
#define compose_jt (*(volatile dw*)&mem[0x35d2]) /* alias jpt_11BC9 */
extern dw dummy50a3c745_seg000_35d4_4305;
extern dw dummy50a3c745_seg000_35d6_4306;
extern dw dummy50a3c745_seg000_35d8_4307;
extern dw dummy50a3c745_seg000_35da_4308;
#define compose2_jt (*(volatile dw*)&mem[0x3658]) /* alias jpt_11C4E */
extern dw dummy50a3c745_seg000_365a_4413;
extern dw dummy50a3c745_seg000_365c_4414;
extern dw dummy50a3c745_seg000_365e_4415;
extern dw dummy50a3c745_seg000_3660_4416;
#define tilerow_tbl (*(volatile dw*)&mem[0x3680]) /* alias funcs_11CB8 */
extern dw dummy50a3c745_seg000_3682_4446;
extern dw dummy50a3c745_seg000_3684_4447;
extern dw dummy50a3c745_seg000_3686_4448;
extern dw dummy50a3c745_seg000_3688_4449;
#define byte_11cf3 (*(volatile db*)&mem[0x3713]) /* alias byte_11CF3 */
#define sprrow_tbl (*(volatile dw*)&mem[0x3714]) /* alias funcs_11D52 */
extern dw dummy50a3c745_seg000_3716_4504;
extern dw dummy50a3c745_seg000_3718_4505;
extern dw dummy50a3c745_seg000_371a_4506;
extern dw dummy50a3c745_seg000_371c_4507;
extern dw dummy50a3c745_seg000_371e_4508;
extern dw dummy50a3c745_seg000_3720_4509;
extern dw dummy50a3c745_seg000_3722_4510;
extern dw dummy50a3c745_seg000_3724_4511;
extern dw dummy50a3c745_seg000_3726_4512;
extern dw dummy50a3c745_seg000_3728_4513;
extern dw dummy50a3c745_seg000_372a_4514;
extern dw dummy50a3c745_seg000_372c_4515;
extern dw dummy50a3c745_seg000_372e_4516;
extern dw dummy50a3c745_seg000_3730_4517;
extern db dummy50a3c745_seg000_3c82_5189;
extern db dummy50a3c745_seg000_3d6e_5349;
extern db dummy50a3c745_seg000_3da2_5381;
extern db dummy50a3c745_seg000_3f20_5635;
extern db dummy50a3c745_seg000_40ce_5856;
#define clip_jt (*(volatile dw*)&mem[0x40cf]) /* alias jpt_126D5 */
extern dw dummy50a3c745_seg000_40d1_5858;
extern dw dummy50a3c745_seg000_40d3_5859;
extern dw dummy50a3c745_seg000_40d5_5860;
extern dw dummy50a3c745_seg000_40d7_5861;
#define word_126da (*(volatile dw*)&mem[0x40fa]) /* alias word_126DA */
#define word_126dc (*(volatile dw*)&mem[0x40fc]) /* alias word_126DC */
extern db dummy50a3c745_seg000_489c_6805;
#define blitflag_jt (*(volatile dw*)&mem[0x5087]) /* alias jpt_1367D */
extern dw dummy50a3c745_seg000_5089_7772;
extern dw dummy50a3c745_seg000_508b_7773;
extern dw dummy50a3c745_seg000_508d_7774;
extern dw dummy50a3c745_seg000_508f_7775;
#define ovl_3a90 (*(volatile dw*)&mem[0x54b0]) /* alias word_13A90 */
#define ovl_3a92 (*(volatile dw*)&mem[0x54b2]) /* alias word_13A92 */
extern db dummy50a3c745_seg000_55a4_8431;
#define word_13bd0 (*(volatile dw*)&mem[0x55f0]) /* alias word_13BD0 */
#define scroll_tbl_a (*(volatile dw*)&mem[0x5600]) /* alias funcs_13BF8 */
extern dw dummy50a3c745_seg000_5602_8498;
extern dw dummy50a3c745_seg000_5604_8499;
extern dw dummy50a3c745_seg000_5606_8500;
extern dw dummy50a3c745_seg000_5608_8501;
#define scroll_tbl_b (*(volatile dw*)&mem[0x56d1]) /* alias funcs_13CC9 */
extern dw dummy50a3c745_seg000_56d3_8636;
extern dw dummy50a3c745_seg000_56d5_8637;
extern dw dummy50a3c745_seg000_56d7_8638;
extern dw dummy50a3c745_seg000_56d9_8639;
#define scroll_tbl_c (*(volatile dw*)&mem[0x57a9]) /* alias funcs_13DA1 */
extern dw dummy50a3c745_seg000_57ab_8779;
extern dw dummy50a3c745_seg000_57ad_8780;
extern dw dummy50a3c745_seg000_57af_8781;
extern dw dummy50a3c745_seg000_57b1_8782;
#define scroll_tbl_d (*(volatile dw*)&mem[0x587a]) /* alias funcs_13E72 */
extern dw dummy50a3c745_seg000_587c_8917;
extern dw dummy50a3c745_seg000_587e_8918;
extern dw dummy50a3c745_seg000_5880_8919;
extern dw dummy50a3c745_seg000_5882_8920;
#define scroll_tbl_e (*(volatile dw*)&mem[0x5950]) /* alias funcs_13F48 */
extern dw dummy50a3c745_seg000_5952_9064;
extern dw dummy50a3c745_seg000_5954_9065;
extern dw dummy50a3c745_seg000_5956_9066;
extern dw dummy50a3c745_seg000_5958_9067;
extern dw dummy50a3c745_seg000_5a2b_9207;
extern dw dummy50a3c745_seg000_5a2d_9208;
extern dw dummy50a3c745_seg000_5a2f_9209;
extern dw dummy50a3c745_seg000_5a31_9210;
#define scroll_tbl_g (*(volatile dw*)&mem[0x5b02]) /* alias funcs_140FA */
extern dw dummy50a3c745_seg000_5b04_9350;
extern dw dummy50a3c745_seg000_5b06_9351;
extern dw dummy50a3c745_seg000_5b08_9352;
extern dw dummy50a3c745_seg000_5b0a_9353;
#define scroll_tbl_h (*(volatile dw*)&mem[0x5bd6]) /* alias funcs_141CE */
extern dw dummy50a3c745_seg000_5bd8_9488;
extern dw dummy50a3c745_seg000_5bda_9489;
extern dw dummy50a3c745_seg000_5bdc_9490;
extern dw dummy50a3c745_seg000_5bde_9491;
extern db dummy50a3c745_seg000_624e_10224;
#define funcs_159e9 (*(volatile dw*)&mem[0x73b0]) /* alias podhit_tbl */
extern dw dummy50a3c745_seg000_73b2_12456;
extern dw dummy50a3c745_seg000_73b4_12457;
extern db dummy50a3c745_seg000_7694_12833;
extern db dummy50a3c745_seg000_7c2a_13609;
extern db dummy50a3c745_seg000_7f88_14053;
#define pan_jt (*(volatile dw*)&mem[0x84b1]) /* alias jpt_16AB9 */
extern dw dummy50a3c745_seg000_84b3_14761;
extern dw dummy50a3c745_seg000_84b5_14762;
extern dw dummy50a3c745_seg000_84b7_14763;
extern dw dummy50a3c745_seg000_84b9_14764;
extern dw dummy50a3c745_seg000_84bb_14765;
extern dw dummy50a3c745_seg000_84bd_14766;
extern dw dummy50a3c745_seg000_84bf_14767;
extern dw dummy50a3c745_seg000_84c1_14768;
extern dw dummy50a3c745_seg000_84c3_14769;
extern dw dummy50a3c745_seg000_84c5_14770;
extern dw dummy50a3c745_seg000_84c7_14771;
extern dw dummy50a3c745_seg000_84c9_14772;
extern dw dummy50a3c745_seg000_84cb_14773;
extern dw dummy50a3c745_seg000_84cd_14774;
extern dw dummy50a3c745_seg000_84cf_14775;
extern db dummy50a3c745_seg000_8934_15480;
extern db dummy50a3c745_seg000_8fae_16375;
extern db dummy50a3c745_seg000_8ff0_16411;
extern db dummy50a3c745_seg000_90f0_16529;
#define jpt_17c64 (*(volatile dw*)&mem[0x963d]) /* alias bar_jt */
extern dw dummy50a3c745_seg000_963f_17221;
extern dw dummy50a3c745_seg000_9641_17222;
extern dw dummy50a3c745_seg000_9643_17223;
extern dw dummy50a3c745_seg000_9645_17224;
extern db dummy50a3c745_seg000_9c9a_17954;
extern db dummy50a3c745_seg000_9e90_18241;
extern db dummy50a3c745_seg000_9f16_18314;
extern db dummy50a3c745_seg000_a04c_18457;
extern db dummy50a3c745_seg000_a1b6_18646;
extern db dummy50a3c745_seg000_a298_18751;
#define objtype_tbl (*(volatile dw*)&mem[0xa2f0]) /* alias funcs_1892F */
extern dw dummy50a3c745_seg000_a2f2_18783;
extern dw dummy50a3c745_seg000_a2f4_18784;
extern dw dummy50a3c745_seg000_a2f6_18785;
extern dw dummy50a3c745_seg000_a2f8_18786;
extern dw dummy50a3c745_seg000_a2fa_18787;
extern dw dummy50a3c745_seg000_a2fc_18788;
extern dw dummy50a3c745_seg000_a2fe_18789;
extern dw dummy50a3c745_seg000_a300_18790;
extern dw dummy50a3c745_seg000_a302_18791;
extern dw dummy50a3c745_seg000_a304_18792;
extern dw dummy50a3c745_seg000_a306_18793;
extern dw dummy50a3c745_seg000_a308_18794;
extern dw dummy50a3c745_seg000_a30a_18795;
extern dw dummy50a3c745_seg000_a30c_18796;
extern dw dummy50a3c745_seg000_a30e_18797;
extern dw dummy50a3c745_seg000_a310_18798;
extern dw dummy50a3c745_seg000_a312_18799;
extern dw dummy50a3c745_seg000_a314_18800;
extern dw dummy50a3c745_seg000_a316_18801;
extern dw dummy50a3c745_seg000_a318_18802;
extern dw dummy50a3c745_seg000_a31a_18803;
extern dw dummy50a3c745_seg000_a31c_18804;
extern dw dummy50a3c745_seg000_a31e_18805;
extern dw dummy50a3c745_seg000_a320_18806;
extern dw dummy50a3c745_seg000_a322_18807;
extern dw dummy50a3c745_seg000_a324_18808;
extern dw dummy50a3c745_seg000_a326_18809;
extern dw dummy50a3c745_seg000_a328_18810;
extern dw dummy50a3c745_seg000_a32a_18811;
extern dw dummy50a3c745_seg000_a32c_18812;
extern dw dummy50a3c745_seg000_a32e_18813;
extern dw dummy50a3c745_seg000_a330_18814;
extern dw dummy50a3c745_seg000_a332_18815;
extern dw dummy50a3c745_seg000_a334_18816;
extern dw dummy50a3c745_seg000_a336_18817;
extern dw dummy50a3c745_seg000_a338_18818;
extern db dummy50a3c745_seg000_a57a_19121;
extern db dummy50a3c745_seg000_a8a8_19532;
extern db dummy50a3c745_seg000_ab84_19873;
extern db dummy50a3c745_seg000_ab8a_19895;
extern db dummy50a3c745_seg000_ad00_20087;
extern db dummy50a3c745_seg000_aeea_20318;
extern db dummy50a3c745_seg000_af04_20333;
extern db dummy50a3c745_seg000_af34_20358;
extern db dummy50a3c745_seg000_b078_20529;
extern db dummy50a3c745_seg000_b418_21028;
extern db dummy50a3c745_seg000_b594_21210;
extern db dummy50a3c745_seg000_b852_21537;
extern db dummy50a3c745_seg000_badc_21887;
extern db dummy50a3c745_seg000_bcf8_22178;
extern db dummy50a3c745_seg000_bd04_22188;
extern db dummy50a3c745_seg000_bf68_22493;
#define rdr_copy_tbl (*(volatile dw*)&mem[0xc3d0]) /* alias funcs_1A9C8 */
extern dw dummy50a3c745_seg000_c3d2_23112;
extern dw dummy50a3c745_seg000_c3d4_23113;
extern dw dummy50a3c745_seg000_c3d6_23114;
extern dw dummy50a3c745_seg000_c3d8_23115;
extern db dummy50a3c745_seg000_c568_23319;
extern db dummy50a3c745_seg000_c588_23335;
extern db dummy50a3c745_seg000_c5da_23381;
extern db dummy50a3c745_seg000_c7f6_23677;
extern db dummy50a3c745_seg000_c822_23697;
extern db dummy50a3c745_seg000_c866_23727;
#define cellgfx_jt (*(volatile dw*)&mem[0xcaa6]) /* alias jpt_1B098 */
extern dw dummy50a3c745_seg000_caa8_24029;
extern dw dummy50a3c745_seg000_caaa_24030;
extern dw dummy50a3c745_seg000_caac_24031;
extern dw dummy50a3c745_seg000_caae_24032;
#define mission_setup_tbl (*(volatile dw*)&mem[0xcdf0]) /* alias funcs_1B3E2 */
extern dw dummy50a3c745_seg000_cdf2_24467;
extern dw dummy50a3c745_seg000_cdf4_24468;
#define mission_pop_tbl (*(volatile dw*)&mem[0xce1a]) /* alias funcs_1B41F */
extern dw dummy50a3c745_seg000_ce1c_24488;
extern dw dummy50a3c745_seg000_ce1e_24489;
#define mission_jt (*(volatile dw*)&mem[0xce77]) /* alias jpt_1B475 */
extern dw dummy50a3c745_seg000_ce79_24539;
extern dw dummy50a3c745_seg000_ce7b_24540;
extern dw dummy50a3c745_seg000_ce7d_24541;
extern dw dummy50a3c745_seg000_ce7f_24542;
extern dw dummy50a3c745_seg000_ce81_24543;
extern dw dummy50a3c745_seg000_ce83_24544;
extern dw dummy50a3c745_seg000_ce85_24545;
extern dw dummy50a3c745_seg000_ce87_24546;
extern dw dummy50a3c745_seg000_ce89_24547;
extern dw dummy50a3c745_seg000_ce8b_24548;
extern dw dummy50a3c745_seg000_ce8d_24549;
extern db dummy50a3c745_seg000_d773_25722;
extern dw dummy50a3c745_seg000_d774_25723;
extern db dummy50a3c745_seg000_d831_25820;
extern dw dummy50a3c745_seg000_d832_25821;
extern db dummy50a3c745_seg000_d8f5_25918;
extern dw dummy50a3c745_seg000_d8f6_25919;
extern db dummy50a3c745_seg000_daaf_26125;
extern dw dummy50a3c745_seg000_dab0_26126;
extern db dummy50a3c745_seg000_dc7b_26340;
extern dw dummy50a3c745_seg000_dc7c_26341;
extern db dummy50a3c745_seg000_dd76_26453;
extern dw dummy50a3c745_seg000_dd77_26454;
extern db dummy50a3c745_seg000_deaa_26586;
extern dw dummy50a3c745_seg000_deab_26587;
extern db dummy50a3c745_seg000_e08e_26812;
extern dw dummy50a3c745_seg000_e08f_26813;
extern db dummy50a3c745_seg000_e11c_26887;
extern dw dummy50a3c745_seg000_e11d_26888;
extern db dummy50a3c745_seg000_e295_27049;
extern dw dummy50a3c745_seg000_e296_27050;
extern db dummy50a3c745_seg000_e39d_27166;
extern dw dummy50a3c745_seg000_e39e_27167;
extern db dummy50a3c745_seg000_e41c_27227;
extern dw dummy50a3c745_seg000_e41d_27228;
extern db seg001;
extern db seg002;
#define resize_p_ce80 (*(volatile dw*)&mem[0xe8a0]) /* alias word_1CE80 */
#define gfx_sel (*(volatile db*)&mem[0xe8b0]) /* alias byte_1CE90 */
#define start_ce92 (*(volatile dw*)&mem[0xe8b2]) /* alias word_1CE92 */
#define start_ce94 (*(volatile dw*)&mem[0xe8b4]) /* alias word_1CE94 */
#define blk_ce96 (*(volatile dw*)&mem[0xe8b6]) /* alias word_1CE96 */
#define blk_ce98 (*(volatile dw*)&mem[0xe8b8]) /* alias word_1CE98 */
#define word_1ce9c (*(volatile dw*)&mem[0xe8bc]) /* alias word_1CE9C */
#define palette_ptr (*(volatile dw*)&mem[0xe8c0]) /* alias word_1CEA0 */
#define byte_1cea2 (*(volatile db*)&mem[0xe8c2]) /* alias byte_1CEA2 */
#define textbuf_seg (*(volatile dw*)&mem[0xe8c3]) /* alias word_1CEA3 */
#define word_1cea5 (*(volatile dw*)&mem[0xe8c5]) /* alias word_1CEA5 */
#define gconv_ceb2 (*(volatile dw*)&mem[0xe8d2]) /* alias word_1CEB2 */
#define gconv_ceb4 (*(volatile dw*)&mem[0xe8d4]) /* alias word_1CEB4 */
#define gconv_ceb6 (*(volatile dw*)&mem[0xe8d6]) /* alias word_1CEB6 */
#define res_parm (*(volatile dw*)&mem[0xea12]) /* alias word_1CFF2 */
#define res_stg_seg (*(volatile dw*)&mem[0xea14]) /* alias word_1CFF4 */
#define res_off (*(volatile dw*)&mem[0xea16]) /* alias word_1CFF6 */
#define load_res_d004 (*(volatile dw*)&mem[0xea24]) /* alias word_1D004 */
#define word_1d006 (*(volatile dw*)&mem[0xea26]) /* alias word_1D006 */
#define res_cache_a (*(volatile dw*)&mem[0xea28]) /* alias word_1D008 */
#define res_cache_b (*(volatile dw*)&mem[0xea2a]) /* alias word_1D00A */
#define res_cache_c (*(volatile dw*)&mem[0xea2c]) /* alias word_1D00C */
#define res_cache_d (*(volatile dw*)&mem[0xea2e]) /* alias word_1D00E */
#define res_ptr (*(volatile dw*)&mem[0xea30]) /* alias word_1D010 */
#define res_seg (*(volatile dw*)&mem[0xea32]) /* alias word_1D012 */
#define res_tmp (*(volatile dw*)&mem[0xea34]) /* alias word_1D014 */
#define res_handle (*(volatile dw*)&mem[0xea36]) /* alias word_1D016 */
#define res_name_ptr (*(volatile dw*)&mem[0xea38]) /* alias word_1D018 */
#define word_1d01a (*(volatile dw*)&mem[0xea3a]) /* alias word_1D01A */
extern dw dummy50a3c745_seg002_ea4c_28452;
extern dd dummy50a3c745_seg002_ea50_28454;
extern dw dummy50a3c745_seg002_ea64_28457;
extern dd dummy50a3c745_seg002_ea68_28459;
extern dw dummy50a3c745_seg002_ea7c_28462;
extern dd dummy50a3c745_seg002_ea80_28464;
extern dw dummy50a3c745_seg002_ea94_28467;
extern dd dummy50a3c745_seg002_ea98_28469;
extern dw dummy50a3c745_seg002_eaac_28472;
extern dd dummy50a3c745_seg002_eab0_28474;
extern dw dummy50a3c745_seg002_eac4_28477;
extern dd dummy50a3c745_seg002_eac8_28479;
extern dw dummy50a3c745_seg002_eadc_28482;
extern dd dummy50a3c745_seg002_eae0_28484;
extern dw dummy50a3c745_seg002_eaf4_28487;
extern dd dummy50a3c745_seg002_eaf8_28489;
extern dw dummy50a3c745_seg002_eb0c_28492;
extern dd dummy50a3c745_seg002_eb10_28494;
extern dw dummy50a3c745_seg002_eb24_28497;
extern dd dummy50a3c745_seg002_eb28_28499;
extern dw dummy50a3c745_seg002_eb3c_28502;
extern dw dummy50a3c745_seg002_eb42_28507;
extern db dummy50a3c745_seg002_eb53_28512;
extern dw dummy50a3c745_seg002_eb54_28513;
extern dd dummy50a3c745_seg002_eb58_28515;
extern dw dummy50a3c745_seg002_eb6c_28518;
extern dd dummy50a3c745_seg002_eb70_28520;
extern dw dummy50a3c745_seg002_eb84_28523;
extern dd dummy50a3c745_seg002_eb88_28525;
extern dw dummy50a3c745_seg002_eb9c_28528;
extern dd dummy50a3c745_seg002_eba0_28530;
extern dw dummy50a3c745_seg002_ebb4_28533;
extern dd dummy50a3c745_seg002_ebb8_28535;
extern dw dummy50a3c745_seg002_ebcc_28538;
extern dd dummy50a3c745_seg002_ebd0_28540;
extern dw dummy50a3c745_seg002_ebe4_28543;
extern dd dummy50a3c745_seg002_ebe8_28545;
extern dw dummy50a3c745_seg002_ebfc_28548;
extern dd dummy50a3c745_seg002_ec00_28550;
extern dw dummy50a3c745_seg002_ec14_28553;
extern dd dummy50a3c745_seg002_ec18_28555;
extern dw dummy50a3c745_seg002_ec2c_28558;
extern dd dummy50a3c745_seg002_ec30_28560;
extern dw dummy50a3c745_seg002_ec44_28563;
extern dd dummy50a3c745_seg002_ec48_28565;
extern dw dummy50a3c745_seg002_ec5c_28568;
extern dd dummy50a3c745_seg002_ec60_28570;
extern dw dummy50a3c745_seg002_ec74_28573;
extern dd dummy50a3c745_seg002_ec78_28575;
extern dw dummy50a3c745_seg002_ec8c_28578;
extern dd dummy50a3c745_seg002_ec90_28580;
extern dd dummy50a3c745_seg002_eca2_28582;
extern dd dummy50a3c745_seg002_eca8_28584;
extern dw dummy50a3c745_seg002_ecbc_28587;
extern dd dummy50a3c745_seg002_ecc0_28589;
extern dw dummy50a3c745_seg002_ecd4_28592;
extern dd dummy50a3c745_seg002_ecd8_28594;
extern dw dummy50a3c745_seg002_ecec_28597;
extern dd dummy50a3c745_seg002_ecf0_28599;
extern dd dummy50a3c745_seg002_ed02_28601;
extern dd dummy50a3c745_seg002_ed08_28603;
extern dw dummy50a3c745_seg002_ed1c_28606;
extern dd dummy50a3c745_seg002_ed20_28608;
extern dw dummy50a3c745_seg002_ed34_28611;
extern dd dummy50a3c745_seg002_ed38_28613;
extern dw dummy50a3c745_seg002_ed4c_28616;
extern dd dummy50a3c745_seg002_ed50_28618;
extern dw dummy50a3c745_seg002_ed64_28621;
extern dd dummy50a3c745_seg002_ed68_28623;
extern dw dummy50a3c745_seg002_ed7c_28626;
extern dd dummy50a3c745_seg002_ed80_28628;
extern dw dummy50a3c745_seg002_ed94_28631;
extern dd dummy50a3c745_seg002_ed98_28633;
extern dw dummy50a3c745_seg002_edac_28636;
extern dd dummy50a3c745_seg002_edb0_28638;
extern dw dummy50a3c745_seg002_edc4_28641;
extern dd dummy50a3c745_seg002_edc8_28643;
extern dw dummy50a3c745_seg002_eddc_28646;
extern dd dummy50a3c745_seg002_ede0_28648;
extern dw dummy50a3c745_seg002_edf4_28651;
extern dd dummy50a3c745_seg002_edf8_28653;
extern dw dummy50a3c745_seg002_ee0c_28656;
extern dd dummy50a3c745_seg002_ee10_28658;
extern dw dummy50a3c745_seg002_ee24_28661;
extern dd dummy50a3c745_seg002_ee28_28663;
extern dw dummy50a3c745_seg002_ee3c_28666;
extern dd dummy50a3c745_seg002_ee40_28668;
extern dw dummy50a3c745_seg002_ee54_28671;
extern dd dummy50a3c745_seg002_ee58_28673;
extern dw dummy50a3c745_seg002_ee6c_28676;
extern dd dummy50a3c745_seg002_ee70_28678;
extern dw dummy50a3c745_seg002_ee84_28681;
extern dd dummy50a3c745_seg002_ee88_28683;
extern dw dummy50a3c745_seg002_ee9c_28686;
extern dd dummy50a3c745_seg002_eea0_28688;
extern dw dummy50a3c745_seg002_eeb4_28691;
extern dd dummy50a3c745_seg002_eeb8_28693;
extern dw dummy50a3c745_seg002_eecc_28696;
extern dd dummy50a3c745_seg002_eed0_28698;
extern dd dummy50a3c745_seg002_eee2_28700;
extern dd dummy50a3c745_seg002_eee8_28702;
extern dw dummy50a3c745_seg002_eefc_28705;
extern dd dummy50a3c745_seg002_ef00_28707;
extern dd dummy50a3c745_seg002_ef12_28709;
extern dd dummy50a3c745_seg002_ef18_28711;
extern dw dummy50a3c745_seg002_ef2c_28714;
extern dd dummy50a3c745_seg002_ef30_28716;
extern dw dummy50a3c745_seg002_ef44_28719;
extern dw dummy50a3c745_seg002_ef4a_28721;
extern dw dummy50a3c745_seg002_ef5c_28724;
extern dd dummy50a3c745_seg002_ef60_28726;
extern dw dummy50a3c745_seg002_ef74_28729;
extern dd dummy50a3c745_seg002_ef78_28731;
extern dw dummy50a3c745_seg002_ef8c_28734;
extern dd dummy50a3c745_seg002_ef90_28736;
extern dw dummy50a3c745_seg002_efa4_28739;
extern dd dummy50a3c745_seg002_efa8_28741;
extern dw dummy50a3c745_seg002_efbc_28744;
extern dd dummy50a3c745_seg002_efc0_28746;
extern dw dummy50a3c745_seg002_efd4_28749;
extern dd dummy50a3c745_seg002_efd8_28751;
extern dw dummy50a3c745_seg002_efec_28754;
extern dd dummy50a3c745_seg002_eff0_28756;
#define video_inited (*(volatile dw*)&mem[0xf0f8]) /* alias word_1D6D8 */
#define keytest_d6da (*(volatile dw*)&mem[0xf0fa]) /* alias word_1D6DA */
#define eblit_d6dc (*(volatile db*)&mem[0xf0fc]) /* alias byte_1D6DC */
#define eblit_d6dd (*(volatile db*)&mem[0xf0fd]) /* alias byte_1D6DD */
#define fmt8_dec_d6de (*(volatile dw*)&mem[0xf0fe]) /* alias word_1D6DE */
#define word_1d6e2 (*(volatile dw*)&mem[0xf102]) /* alias word_1D6E2 */
#define word_1d6e4 (*(volatile dw*)&mem[0xf104]) /* alias word_1D6E4 */
#define tile_bli_d7c0 (*(volatile dw*)&mem[0xf1e0]) /* alias word_1D7C0 */
#define modal_flag (*(volatile dw*)&mem[0xf2f0]) /* alias word_1D8D0 */
#define word_1d8d2 (*(volatile dw*)&mem[0xf2f2]) /* alias word_1D8D2 */
#define fwait_d8d4 (*(volatile dw*)&mem[0xf2f4]) /* alias word_1D8D4 */
#define fwait_d8d6 (*(volatile dw*)&mem[0xf2f6]) /* alias word_1D8D6 */
#define joy_cali_d8d8 (*(volatile dw*)&mem[0xf2f8]) /* alias word_1D8D8 */
#define otick_d8ea (*(volatile dw*)&mem[0xf30a]) /* alias word_1D8EA */
#define otick_d8ec (*(volatile dw*)&mem[0xf30c]) /* alias word_1D8EC */
#define routemap_d8f6 (*(volatile dw*)&mem[0xf316]) /* alias word_1D8F6 */
#define map_base (*(volatile dw*)&mem[0xf318]) /* alias word_1D8F8 */
#define draw_col (*(volatile dw*)&mem[0xf320]) /* alias word_1D900 */
#define draw_row (*(volatile dw*)&mem[0xf322]) /* alias word_1D902 */
#define word_1d916 (*(volatile dw*)&mem[0xf336]) /* alias word_1D916 */
#define word_1d918 (*(volatile dw*)&mem[0xf338]) /* alias word_1D918 */
#define print_width (*(volatile dw*)&mem[0xf33c]) /* alias word_1D91C */
#define screen_state (*(volatile dw*)&mem[0xf33e]) /* alias word_1D91E */
#define word_1d920 (*(volatile dw*)&mem[0xf340]) /* alias word_1D920 */
#define wparm_a (*(volatile dw*)&mem[0xf34c]) /* alias word_1D92C */
#define wparm_b (*(volatile dw*)&mem[0xf34e]) /* alias word_1D92E */
#define extkey_mode (*(volatile dw*)&mem[0xf350]) /* alias word_1D930 */
#define adapter_id (*(volatile dw*)&mem[0xf354]) /* alias word_1D934 */
#define cellgfx__d936 (*(volatile db*)&mem[0xf356]) /* alias byte_1D936 */
#define word_1d937 (*(volatile dw*)&mem[0xf357]) /* alias word_1D937 */
#define byte_1d939 (*(volatile db*)&mem[0xf359]) /* alias byte_1D939 */
#define byte_1d93a (*(volatile db*)&mem[0xf35a]) /* alias byte_1D93A */
#define byte_1d93b (*(volatile db*)&mem[0xf35b]) /* alias byte_1D93B */
#define keytest_d93e (*(volatile db*)&mem[0xf35e]) /* alias byte_1D93E */
#define word_1d93f (*(volatile dw*)&mem[0xf35f]) /* alias word_1D93F */
#define eblit_d943 (*(volatile dw*)&mem[0xf363]) /* alias word_1D943 */
#define exit_flag (*(volatile dw*)&mem[0xf365]) /* alias word_1D945 */
#define stage_parm_a (*(volatile dw*)&mem[0xf369]) /* alias word_1D949 */
#define stage_parm_b (*(volatile dw*)&mem[0xf36b]) /* alias word_1D94B */
#define delay_cnt (*(volatile dw*)&mem[0xf36d]) /* alias word_1D94D */
#define tick_isr_d94f (*(volatile dw*)&mem[0xf36f]) /* alias word_1D94F */
#define word_1d951 (*(volatile dw*)&mem[0xf371]) /* alias word_1D951 */
#define word_1d953 (*(volatile dw*)&mem[0xf373]) /* alias word_1D953 */
#define timer_cnt (*(volatile dw*)&mem[0xf375]) /* alias word_1D955 */
#define word_1d957 (*(volatile dw*)&mem[0xf377]) /* alias word_1D957 */
#define held_keys (*(volatile dw*)&mem[0xf379]) /* alias word_1D959 */
#define print_de_d95d (*(volatile dw*)&mem[0xf37d]) /* alias word_1D95D */
#define rand_s0 (*(volatile dw*)&mem[0xf37f]) /* alias word_1D95F */
#define rand_s1 (*(volatile dw*)&mem[0xf381]) /* alias word_1D961 */
#define seg000_1_d965 (*(volatile dw*)&mem[0xf385]) /* alias word_1D965 */
#define seg000_1_d969 (*(volatile dw*)&mem[0xf389]) /* alias word_1D969 */
#define draw_cel_dbdd (*(volatile dw*)&mem[0xf5fd]) /* alias word_1DBDD */
#define inmenu_dc65 (*(volatile db*)&mem[0xf685]) /* alias byte_1DC65 */
extern db dummy50a3c745_seg002_f686_28925;
#define devmenu_dc67 (*(volatile dw*)&mem[0xf687]) /* alias word_1DC67 */
#define devmenu_dc69 (*(volatile dw*)&mem[0xf689]) /* alias word_1DC69 */
#define devmenu_dc6b (*(volatile dw*)&mem[0xf68b]) /* alias word_1DC6B */
#define devmenu_dc6d (*(volatile dw*)&mem[0xf68d]) /* alias word_1DC6D */
#define devmenu_dc6f (*(volatile dw*)&mem[0xf68f]) /* alias word_1DC6F */
#define devmenu_dc71 (*(volatile dw*)&mem[0xf691]) /* alias word_1DC71 */
#define devmenu_dc73 (*(volatile dw*)&mem[0xf693]) /* alias word_1DC73 */
#define devmenu_dc75 (*(volatile dw*)&mem[0xf695]) /* alias word_1DC75 */
#define inmenu_dc77 (*(volatile dw*)&mem[0xf697]) /* alias word_1DC77 */
#define inmenu_dc79 (*(volatile db*)&mem[0xf699]) /* alias byte_1DC79 */
#define inmenu_dc7a (*(volatile db*)&mem[0xf69a]) /* alias byte_1DC7A */
#define byte_1dc7d (*(volatile db*)&mem[0xf69d]) /* alias byte_1DC7D */
#define joy_ax (*(volatile dw*)&mem[0xf6a0]) /* alias word_1DC80 */
#define joy_ay (*(volatile dw*)&mem[0xf6a2]) /* alias word_1DC82 */
#define joy_bx (*(volatile dw*)&mem[0xf6a6]) /* alias word_1DC86 */
#define joy_by (*(volatile dw*)&mem[0xf6a8]) /* alias word_1DC88 */
#define joya_dc8c (*(volatile dw*)&mem[0xf6ac]) /* alias word_1DC8C */
#define joyb_dc8e (*(volatile dw*)&mem[0xf6ae]) /* alias word_1DC8E */
#define joya_dc92 (*(volatile dw*)&mem[0xf6b2]) /* alias word_1DC92 */
#define joyb_dc94 (*(volatile dw*)&mem[0xf6b4]) /* alias word_1DC94 */
#define joya_dc98 (*(volatile dw*)&mem[0xf6b8]) /* alias word_1DC98 */
#define joyb_dc9a (*(volatile dw*)&mem[0xf6ba]) /* alias word_1DC9A */
#define joya_dc9e (*(volatile dw*)&mem[0xf6be]) /* alias word_1DC9E */
#define joyb_dca0 (*(volatile dw*)&mem[0xf6c0]) /* alias word_1DCA0 */
#define input_a (*(volatile dw*)&mem[0xf6c2]) /* alias word_1DCA2 */
#define input_b (*(volatile dw*)&mem[0xf6c4]) /* alias word_1DCA4 */
#define joy_mask (*(volatile db*)&mem[0xf6ca]) /* alias byte_1DCAA */
#define byte_1dcab (*(volatile db*)&mem[0xf6cb]) /* alias byte_1DCAB */
#define joy_acc_x (*(volatile dw*)&mem[0xf6cc]) /* alias word_1DCAC */
#define joy_acc_y (*(volatile dw*)&mem[0xf6ce]) /* alias word_1DCAE */
#define byte_1dcb0 (*(volatile db*)&mem[0xf6d0]) /* alias byte_1DCB0 */
#define poll_dcb1 (*(volatile dw*)&mem[0xf6d1]) /* alias word_1DCB1 */
#define poll_dcb3 (*(volatile dw*)&mem[0xf6d3]) /* alias word_1DCB3 */
#define word_1dcbf (*(volatile dw*)&mem[0xf6df]) /* alias word_1DCBF */
#define drawadv_dcc1 (*(volatile db*)&mem[0xf6e1]) /* alias byte_1DCC1 */
extern db dummy50a3c745_seg002_f6e2_28995;
#define find_fil_dcdd (*(volatile db*)&mem[0xf6fd]) /* alias byte_1DCDD */
extern db dummy50a3c745_seg002_f6fe_28997;
#define word_1dcdf (*(volatile dw*)&mem[0xf6ff]) /* alias word_1DCDF */
#define drawadv_dce1 (*(volatile db*)&mem[0xf701]) /* alias byte_1DCE1 */
#define word_1dcff (*(volatile dw*)&mem[0xf71f]) /* alias word_1DCFF */
#define mdir_dd01 (*(volatile db*)&mem[0xf721]) /* alias byte_1DD01 */
#define phase_dd05 (*(volatile db*)&mem[0xf725]) /* alias byte_1DD05 */
#define word_1dd1f (*(volatile dw*)&mem[0xf73f]) /* alias word_1DD1F */
#define mdir_dd21 (*(volatile db*)&mem[0xf741]) /* alias byte_1DD21 */
#define phase_dd25 (*(volatile db*)&mem[0xf745]) /* alias byte_1DD25 */
#define word_1dd3f (*(volatile dw*)&mem[0xf75f]) /* alias word_1DD3F */
#define mdir_dd41 (*(volatile db*)&mem[0xf761]) /* alias byte_1DD41 */
#define phase_dd45 (*(volatile db*)&mem[0xf765]) /* alias byte_1DD45 */
#define word_1dd5f (*(volatile dw*)&mem[0xf77f]) /* alias word_1DD5F */
#define byte_1dd7f (*(volatile db*)&mem[0xf79f]) /* alias byte_1DD7F */
#define select_dd80 (*(volatile db*)&mem[0xf7a0]) /* alias byte_1DD80 */
#define clip_mask_a (*(volatile dw*)&mem[0xf7bf]) /* alias word_1DD9F */
#define word_1dda1 (*(volatile dw*)&mem[0xf7c1]) /* alias word_1DDA1 */
#define clip_mask_b (*(volatile dw*)&mem[0xf7c3]) /* alias word_1DDA3 */
#define word_1dda5 (*(volatile dw*)&mem[0xf7c5]) /* alias word_1DDA5 */
#define dirtyrect_ptr (*(volatile dw*)&mem[0xf7d0]) /* alias word_1DDB0 */
#define compose_gate (*(volatile dw*)&mem[0xf7d2]) /* alias word_1DDB2 */
#define word_1ddb4 (*(volatile dw*)&mem[0xf7d4]) /* alias word_1DDB4 */
#define rectreg_base (*(volatile dw*)&mem[0xf7d9]) /* alias word_1DDB9 */
#define word_1ddcd (*(volatile dw*)&mem[0xf7ed]) /* alias word_1DDCD */
#define rectreg_off (*(volatile dw*)&mem[0xf7f3]) /* alias word_1DDD3 */
#define byte_1ddd9 (*(volatile db*)&mem[0xf7f9]) /* alias byte_1DDD9 */
#define cframe_e0a3 (*(volatile db*)&mem[0xfac3]) /* alias byte_1E0A3 */
#define cframe_e12f (*(volatile db*)&mem[0xfb4f]) /* alias byte_1E12F */
#define word_1e1bd (*(volatile dw*)&mem[0xfbdd]) /* alias word_1E1BD */
#define crest_e1bf (*(volatile dw*)&mem[0xfbdf]) /* alias word_1E1BF */
#define crest_e1c1 (*(volatile dw*)&mem[0xfbe1]) /* alias word_1E1C1 */
#define word_1e1c3 (*(volatile dw*)&mem[0xfbe3]) /* alias word_1E1C3 */
#define byte_1e1c5 (*(volatile db*)&mem[0xfbe5]) /* alias byte_1E1C5 */
#define cblit_e1c9 (*(volatile db*)&mem[0xfbe9]) /* alias byte_1E1C9 */
#define cblit_e1ca (*(volatile db*)&mem[0xfbea]) /* alias byte_1E1CA */
#define word_1e1cb (*(volatile dw*)&mem[0xfbeb]) /* alias word_1E1CB */
#define cblit_e1cd (*(volatile db*)&mem[0xfbed]) /* alias byte_1E1CD */
#define word_1e1ce (*(volatile dw*)&mem[0xfbee]) /* alias word_1E1CE */
#define word_1e1d0 (*(volatile dw*)&mem[0xfbf0]) /* alias word_1E1D0 */
#define word_1e1d2 (*(volatile dw*)&mem[0xfbf2]) /* alias word_1E1D2 */
#define word_1e1d4 (*(volatile dw*)&mem[0xfbf4]) /* alias word_1E1D4 */
#define ovl_e840 (*(volatile dw*)&mem[0x10260]) /* alias word_1E840 */
#define ovl_e842 (*(volatile dw*)&mem[0x10262]) /* alias word_1E842 */
#define ovl_e844 (*(volatile dw*)&mem[0x10264]) /* alias word_1E844 */
#define ovl_e846 (*(volatile dw*)&mem[0x10266]) /* alias word_1E846 */
#define start_09e0 (*(volatile dw*)&mem[0x12400]) /* alias word_209E0 */
#define start_09e2 (*(volatile dw*)&mem[0x12402]) /* alias word_209E2 */
#define select_r_0d38 (*(volatile dw*)&mem[0x12758]) /* alias word_20D38 */
#define seg000_1_0d4a (*(volatile dw*)&mem[0x1276a]) /* alias word_20D4A */
#define seg000_1_0d4c (*(volatile db*)&mem[0x1276c]) /* alias byte_20D4C */
#define spcopy_0d4e (*(volatile dw*)&mem[0x1276e]) /* alias word_20D4E */
#define spcopy_0d50 (*(volatile db*)&mem[0x12770]) /* alias byte_20D50 */
#define word_20d52 (*(volatile dw*)&mem[0x12772]) /* alias word_20D52 */
#define byte_20d54 (*(volatile db*)&mem[0x12774]) /* alias byte_20D54 */
#define word_20d56 (*(volatile dw*)&mem[0x12776]) /* alias word_20D56 */
#define legend_0d58 (*(volatile dw*)&mem[0x12778]) /* alias word_20D58 */
extern dw dummy50a3c745_seg002_1277e_29407;
#define byte_24e70 (*(volatile db*)raddr_(ds,0x7ff0)) /* ds-rel alias byte_24E70 */
#define dcomp_4e71 (*(volatile db*)raddr_(ds,0x7ff1)) /* ds-rel alias byte_24E71 */
#define word_24e72 (*(volatile dw*)raddr_(ds,0x7ff2)) /* ds-rel alias word_24E72 */
#define dcomp_4e74 (*(volatile dw*)raddr_(ds,0x7ff4)) /* ds-rel alias word_24E74 */
#define dcomp_4e76 (*(volatile db*)raddr_(ds,0x7ff6)) /* ds-rel alias byte_24E76 */
#define dcomp_4e77 (*(volatile dw*)raddr_(ds,0x7ff7)) /* ds-rel alias word_24E77 */
#define dcomp_4e79 (*(volatile dw*)raddr_(ds,0x7ff9)) /* ds-rel alias word_24E79 */
#define dcomp_4e7b (*(volatile db*)raddr_(ds,0x7ffb)) /* ds-rel alias byte_24E7B */
#define intro_656a (*(volatile dw*)&mem[0x17f8a]) /* alias word_2656A */
#define word_2656c (*(volatile dw*)&mem[0x17f8c]) /* alias word_2656C */
#define word_2656e (*(volatile dw*)&mem[0x17f8e]) /* alias word_2656E */
#define game_mai_65a8 (*(volatile dw*)&mem[0x17fc8]) /* alias word_265A8 */
#define frame_cnt (*(volatile dw*)&mem[0x17fca]) /* alias word_265AA */
#define ot0a_tick_65ac (*(volatile dw*)&mem[0x17fcc]) /* alias word_265AC */
#define render_cnt (*(volatile dw*)&mem[0x17fce]) /* alias word_265AE */
#define rec_ptr_a (*(volatile dw*)&mem[0x17fd0]) /* alias word_265B0 */
#define rec_ptr_b (*(volatile dw*)&mem[0x17fd2]) /* alias word_265B2 */
#define cell_base (*(volatile dw*)&mem[0x17fd4]) /* alias word_265B4 */
#define input_wait (*(volatile dw*)&mem[0x187e0]) /* alias word_26DC0 */
#define find_fil_6dcd (*(volatile db*)&mem[0x187ed]) /* alias byte_26DCD */
#define title_shown (*(volatile dw*)&mem[0x187f3]) /* alias word_26DD3 */
#define alt_control (*(volatile db*)&mem[0x187f5]) /* alias byte_26DD5 */
#define cur_col (*(volatile db*)&mem[0x187f6]) /* alias byte_26DD6 */
#define cur_row (*(volatile db*)&mem[0x187f7]) /* alias byte_26DD7 */
#define cur_idx (*(volatile dw*)&mem[0x187f8]) /* alias word_26DD8 */
#define cur_end (*(volatile dw*)&mem[0x187fa]) /* alias word_26DDA */
#define main_men_6ddc (*(volatile dw*)&mem[0x187fc]) /* alias word_26DDC */
#define word_26dde (*(volatile dw*)&mem[0x187fe]) /* alias word_26DDE */
#define script_ptr (*(volatile dw*)&mem[0x18800]) /* alias word_26DE0 */
#define dlg_buf (*(volatile dw*)&mem[0x18802]) /* alias word_26DE2 */
#define input_mask (*(volatile dw*)&mem[0x18804]) /* alias word_26DE4 */
#define accept_latch (*(volatile db*)&mem[0x18806]) /* alias byte_26DE6 */
#define run_6de7 (*(volatile db*)&mem[0x18807]) /* alias byte_26DE7 */
#define dlg_col (*(volatile db*)&mem[0x18808]) /* alias byte_26DE8 */
#define dlg_row (*(volatile db*)&mem[0x18809]) /* alias byte_26DE9 */
#define byte_26dea (*(volatile db*)&mem[0x1880a]) /* alias byte_26DEA */
#define word_26deb (*(volatile dw*)&mem[0x1880b]) /* alias word_26DEB */
#define rowf_6ded (*(volatile dw*)&mem[0x1880d]) /* alias word_26DED */
#define rowf_6def (*(volatile db*)&mem[0x1880f]) /* alias byte_26DEF */
#define rowf_6df0 (*(volatile db*)&mem[0x18810]) /* alias byte_26DF0 */
#define snap_v (*(volatile db*)&mem[0x18811]) /* alias byte_26DF1 */
#define byte_26df2 (*(volatile db*)&mem[0x18812]) /* alias byte_26DF2 */
#define csnap_6df3 (*(volatile db*)&mem[0x18813]) /* alias byte_26DF3 */
#define csnap_6df4 (*(volatile dw*)&mem[0x18814]) /* alias word_26DF4 */
#define csnap_6df6 (*(volatile dw*)&mem[0x18816]) /* alias word_26DF6 */
#define csnap_6df8 (*(volatile dw*)&mem[0x18818]) /* alias word_26DF8 */
#define csnap_6dfa (*(volatile db*)&mem[0x1881a]) /* alias byte_26DFA */
#define byte_26dfb (*(volatile db*)&mem[0x1881b]) /* alias byte_26DFB */
#define byte_26dfc (*(volatile db*)&mem[0x1881c]) /* alias byte_26DFC */
extern db dummy50a3c745_seg002_18b88_29554;
extern db dummy50a3c745_seg002_18bf4_29564;
#define mselprep_71d5 (*(volatile db*)&mem[0x18bf5]) /* alias byte_271D5 */
#define byte_2731e (*(volatile db*)&mem[0x18d3e]) /* alias byte_2731E */
extern db dummy50a3c745_seg002_18d4e_29601;
#define screen_c_732f (*(volatile dw*)&mem[0x18d4f]) /* alias word_2732F */
#define brief_sc_762f (*(volatile dw*)&mem[0x1904f]) /* alias word_2762F */
#define mode4_81ed (*(volatile db*)&mem[0x19c0d]) /* alias byte_281ED */
#define mode4_81ee (*(volatile db*)&mem[0x19c0e]) /* alias byte_281EE */
#define mode4_824b (*(volatile dw*)&mem[0x19c6b]) /* alias word_2824B */
#define devmenu_860a (*(volatile dw*)&mem[0x1a02a]) /* alias word_2860A */
extern db dummy50a3c745_seg002_1a14f_30088;
#define file_fie_89e2 (*(volatile db*)&mem[0x1a402]) /* alias byte_289E2 */
#define file_fie_89e3 (*(volatile db*)&mem[0x1a403]) /* alias byte_289E3 */
#define byte_28a50 (*(volatile db*)&mem[0x1a470]) /* alias byte_28A50 */
extern db dummy50a3c745_seg002_1a4ed_30177;
#define word_28ace (*(volatile dw*)&mem[0x1a4ee]) /* alias word_28ACE */
#define word_28ad0 (*(volatile dw*)&mem[0x1a4f0]) /* alias word_28AD0 */
#define byte_28ad2 (*(volatile db*)&mem[0x1a4f2]) /* alias byte_28AD2 */
#define pod_hit__8c83 (*(volatile dw*)&mem[0x1a6a3]) /* alias word_28C83 */
#define objblk_i_8c90 (*(volatile dw*)&mem[0x1a6b0]) /* alias word_28C90 */
#define objblk_i_8c92 (*(volatile dw*)&mem[0x1a6b2]) /* alias word_28C92 */
#define objblk_i_8c94 (*(volatile db*)&mem[0x1a6b4]) /* alias byte_28C94 */
#define objblk_i_8c95 (*(volatile db*)&mem[0x1a6b5]) /* alias byte_28C95 */
#define objblk_i_8c96 (*(volatile db*)&mem[0x1a6b6]) /* alias byte_28C96 */
#define objblk_i_8c97 (*(volatile db*)&mem[0x1a6b7]) /* alias byte_28C97 */
#define byte_28ca0 (*(volatile db*)&mem[0x1a6c0]) /* alias byte_28CA0 */
#define post_loo_8ca1 (*(volatile db*)&mem[0x1a6c1]) /* alias byte_28CA1 */
#define ot01_8ca2 (*(volatile db*)&mem[0x1a6c2]) /* alias byte_28CA2 */
#define csetup_8ca3 (*(volatile db*)&mem[0x1a6c3]) /* alias byte_28CA3 */
#define count_ne_8cac (*(volatile dw*)&mem[0x1a6cc]) /* alias word_28CAC */
#define apply_sc_8cb1 (*(volatile db*)&mem[0x1a6d1]) /* alias byte_28CB1 */
extern db dummy50a3c745_seg002_1a6d2_30239;
#define mark_col (*(volatile db*)&mem[0x1a6d3]) /* alias byte_28CB3 */
#define mark_row (*(volatile db*)&mem[0x1a6d4]) /* alias byte_28CB4 */
#define space_flag (*(volatile db*)&mem[0x1a6e0]) /* alias byte_28CC0 */
#define diff_parm (*(volatile db*)&mem[0x1a6e1]) /* alias byte_28CC1 */
#define spawn_level (*(volatile db*)&mem[0x1a6e2]) /* alias byte_28CC2 */
#define byte_28cc3 (*(volatile db*)&mem[0x1a6e3]) /* alias byte_28CC3 */
#define call_fn_8cc4 (*(volatile db*)&mem[0x1a6e4]) /* alias byte_28CC4 */
#define call_fn_8cc5 (*(volatile db*)&mem[0x1a6e5]) /* alias byte_28CC5 */
#define cool_gate (*(volatile db*)&mem[0x1a6e8]) /* alias byte_28CC8 */
#define ot01_8cca (*(volatile db*)&mem[0x1a6ea]) /* alias byte_28CCA */
#define byte_28ccb (*(volatile db*)&mem[0x1a6eb]) /* alias byte_28CCB */
#define sfx_ch (*(volatile db*)&mem[0x1a6f1]) /* alias byte_28CD1 */
#define byte_28cd2 (*(volatile db*)&mem[0x1a6f2]) /* alias byte_28CD2 */
#define byte_28cd3 (*(volatile db*)&mem[0x1a6f3]) /* alias byte_28CD3 */
#define byte_28cd4 (*(volatile db*)&mem[0x1a6f4]) /* alias byte_28CD4 */
#define word_28cd5 (*(volatile dw*)&mem[0x1a6f5]) /* alias word_28CD5 */
extern db dummy50a3c745_seg002_1a6f7_30271;
#define byte_28cd8 (*(volatile db*)&mem[0x1a6f8]) /* alias byte_28CD8 */
#define rstate_8cd9 (*(volatile db*)&mem[0x1a6f9]) /* alias byte_28CD9 */
extern db dummy50a3c745_seg002_1a6fa_30276;
#define bkey_8cdb (*(volatile db*)&mem[0x1a6fb]) /* alias byte_28CDB */
#define carry_blk (*(volatile db*)&mem[0x1a6fc]) /* alias byte_28CDC */
extern db dummy50a3c745_seg002_1a72c_30286;
#define csetup_8d0d (*(volatile db*)&mem[0x1a72d]) /* alias byte_28D0D */
#define csetup_8d0e (*(volatile db*)&mem[0x1a72e]) /* alias byte_28D0E */
#define csetup_8d51 (*(volatile db*)&mem[0x1a771]) /* alias byte_28D51 */
#define byte_28dfb (*(volatile db*)&mem[0x1a81b]) /* alias byte_28DFB */
#define byte_28e1d (*(volatile db*)&mem[0x1a83d]) /* alias byte_28E1D */
#define byte_28e3f (*(volatile db*)&mem[0x1a85f]) /* alias byte_28E3F */
#define byte_28e61 (*(volatile db*)&mem[0x1a881]) /* alias byte_28E61 */
#define owake_8e83 (*(volatile db*)&mem[0x1a8a3]) /* alias byte_28E83 */
#define csetup_8f2d (*(volatile db*)&mem[0x1a94d]) /* alias byte_28F2D */
#define owake_901b (*(volatile db*)&mem[0x1aa3b]) /* alias byte_2901B */
#define owake_903d (*(volatile db*)&mem[0x1aa5d]) /* alias byte_2903D */
#define ot17_tick_90a3 (*(volatile db*)&mem[0x1aac3]) /* alias byte_290A3 */
#define csetup_914d (*(volatile db*)&mem[0x1ab6d]) /* alias byte_2914D */
#define csetup_914e (*(volatile db*)&mem[0x1ab6e]) /* alias byte_2914E */
#define csetup_916f (*(volatile db*)&mem[0x1ab8f]) /* alias byte_2916F */
#define csetup_91b3 (*(volatile db*)&mem[0x1abd3]) /* alias byte_291B3 */
#define csetup_91d5 (*(volatile db*)&mem[0x1abf5]) /* alias byte_291D5 */
#define csetup_923b (*(volatile db*)&mem[0x1ac5b]) /* alias byte_2923B */
#define objblk_i_95ea (*(volatile db*)&mem[0x1b00a]) /* alias byte_295EA */
#define word_295eb (*(volatile dw*)&mem[0x1b00b]) /* alias word_295EB */
#define csetup_95ef (*(volatile db*)&mem[0x1b00f]) /* alias byte_295EF */
#define word_2966a (*(volatile dw*)&mem[0x1b08a]) /* alias word_2966A */
#define dmg_deal_966c (*(volatile dw*)&mem[0x1b08c]) /* alias word_2966C */
#define word_2966e (*(volatile dw*)&mem[0x1b08e]) /* alias word_2966E */
#define pan_flags (*(volatile db*)&mem[0x1b09a]) /* alias byte_2967A */
#define cam_tgt_x (*(volatile dw*)&mem[0x1b09b]) /* alias word_2967B */
#define cam_tgt_y (*(volatile dw*)&mem[0x1b09d]) /* alias word_2967D */
#define ai_flag_out (*(volatile db*)&mem[0x1b12d]) /* alias byte_2970D */
#define bkey_970e (*(volatile db*)&mem[0x1b12e]) /* alias byte_2970E */
#define bkey_970f (*(volatile db*)&mem[0x1b12f]) /* alias byte_2970F */
#define byte_2971a (*(volatile db*)&mem[0x1b13a]) /* alias byte_2971A */
#define enemy_cnt (*(volatile db*)&mem[0x1b16f]) /* alias byte_2974F */
#define hit_flag (*(volatile db*)&mem[0x1b18a]) /* alias byte_2976A */
#define trk_flag (*(volatile db*)&mem[0x1b18b]) /* alias byte_2976B */
#define aux_msg (*(volatile db*)&mem[0x1b18c]) /* alias byte_2976C */
#define state_b (*(volatile dw*)&mem[0x1b201]) /* alias word_297E1 */
#define spawn_budget (*(volatile dw*)&mem[0x1b213]) /* alias word_297F3 */
#define unit_dra_984b (*(volatile db*)&mem[0x1b26b]) /* alias byte_2984B */
#define oscr_0 (*(volatile db*)&mem[0x1b26c]) /* alias byte_2984C */
#define oscr_1 (*(volatile db*)&mem[0x1b26d]) /* alias byte_2984D */
#define oscr_2 (*(volatile db*)&mem[0x1b26e]) /* alias byte_2984E */
#define oscr_3 (*(volatile db*)&mem[0x1b26f]) /* alias byte_2984F */
#define mcell_98a2 (*(volatile dw*)&mem[0x1b2c2]) /* alias word_298A2 */
#define mcell_98a4 (*(volatile dw*)&mem[0x1b2c4]) /* alias word_298A4 */
#define mcell_98a6 (*(volatile db*)&mem[0x1b2c6]) /* alias byte_298A6 */
#define burst_cnt (*(volatile dw*)&mem[0x1b2cc]) /* alias word_298AC */
#define burst_src (*(volatile dw*)&mem[0x1b2ce]) /* alias word_298AE */
#define place_x (*(volatile dw*)&mem[0x1b2d2]) /* alias word_298B2 */
#define word_298c0 (*(volatile dw*)&mem[0x1b2e0]) /* alias word_298C0 */
#define weight_sum (*(volatile dw*)&mem[0x1b2e2]) /* alias word_298C2 */
#define follow_m_98c4 (*(volatile db*)&mem[0x1b2e4]) /* alias byte_298C4 */
#define scan_a (*(volatile db*)&mem[0x1b3af]) /* alias byte_2998F */
#define scan_c (*(volatile db*)&mem[0x1b3ba]) /* alias byte_2999A */
#define ai_dir_p_99af (*(volatile db*)&mem[0x1b3cf]) /* alias byte_299AF */
#define tgt_delt_9b42 (*(volatile dw*)&mem[0x1b562]) /* alias word_29B42 */
#define rel_y (*(volatile dw*)&mem[0x1b564]) /* alias word_29B44 */
#define move_ste_9b4f (*(volatile db*)&mem[0x1b56f]) /* alias byte_29B4F */
#define mv_lo (*(volatile db*)&mem[0x1b570]) /* alias byte_29B50 */
#define mv_hi (*(volatile db*)&mem[0x1b571]) /* alias byte_29B51 */
#define follow_sav_a (*(volatile dw*)&mem[0x1b572]) /* alias word_29B52 */
#define follow_sav_d (*(volatile dw*)&mem[0x1b574]) /* alias word_29B54 */
#define follow_sav_a2 (*(volatile dw*)&mem[0x1b576]) /* alias word_29B56 */
#define follow_sav_d2 (*(volatile dw*)&mem[0x1b578]) /* alias word_29B58 */
#define dmg_kind (*(volatile db*)&mem[0x1b57a]) /* alias byte_29B5A */
#define oscr_a (*(volatile db*)&mem[0x1b57b]) /* alias byte_29B5B */
#define oscr_b (*(volatile db*)&mem[0x1b57c]) /* alias byte_29B5C */
#define oscr_c (*(volatile db*)&mem[0x1b57d]) /* alias byte_29B5D */
#define objpos_t_9b5e (*(volatile db*)&mem[0x1b57e]) /* alias byte_29B5E */
#define oscr_d (*(volatile db*)&mem[0x1b57f]) /* alias byte_29B5F */
#define objpos_t_9b60 (*(volatile db*)&mem[0x1b580]) /* alias byte_29B60 */
#define objpos_t_9b61 (*(volatile db*)&mem[0x1b581]) /* alias byte_29B61 */
#define oscr_e (*(volatile db*)&mem[0x1b582]) /* alias byte_29B62 */
#define tmp_si (*(volatile dw*)&mem[0x1b583]) /* alias word_29B63 */
#define ai_parm_a (*(volatile dw*)&mem[0x1b5b5]) /* alias word_29B95 */
#define ai_parm_b (*(volatile dw*)&mem[0x1b5b7]) /* alias word_29B97 */
#define wpn_9e36 (*(volatile dw*)&mem[0x1b856]) /* alias word_29E36 */
#define wpn_9e38 (*(volatile dw*)&mem[0x1b858]) /* alias word_29E38 */
#define wpn_9e3a (*(volatile dw*)&mem[0x1b85a]) /* alias word_29E3A */
#define wpn_9e3c (*(volatile dw*)&mem[0x1b85c]) /* alias word_29E3C */
#define wpn_9e3e (*(volatile dw*)&mem[0x1b85e]) /* alias word_29E3E */
#define wpn_9e40 (*(volatile dw*)&mem[0x1b860]) /* alias word_29E40 */
#define word_29ebc (*(volatile dw*)&mem[0x1b8dc]) /* alias word_29EBC */
#define fx_overl_9f5e (*(volatile db*)&mem[0x1b97e]) /* alias byte_29F5E */
#define fx_overl_9f60 (*(volatile dw*)&mem[0x1b980]) /* alias word_29F60 */
#define anim2_st_9f64 (*(volatile dw*)&mem[0x1b984]) /* alias word_29F64 */
#define anim2_st_9f66 (*(volatile dw*)&mem[0x1b986]) /* alias word_29F66 */
#define select_9f76 (*(volatile db*)&mem[0x1b996]) /* alias byte_29F76 */
#define select_9f77 (*(volatile db*)&mem[0x1b997]) /* alias byte_29F77 */
#define select_9f78 (*(volatile dw*)&mem[0x1b998]) /* alias word_29F78 */
#define select_a0dd (*(volatile dw*)&mem[0x1bafd]) /* alias word_2A0DD */
#define joytabs_a0ea (*(volatile dw*)&mem[0x1bb0a]) /* alias word_2A0EA */
#define joytabs_a10d (*(volatile dw*)&mem[0x1bb2d]) /* alias word_2A10D */
#define joytabs_a12a (*(volatile dw*)&mem[0x1bb4a]) /* alias word_2A12A */
#define joytabs_a147 (*(volatile dw*)&mem[0x1bb67]) /* alias word_2A147 */
extern db dummy50a3c745_seg002_1bd2c_30852;
#define select_a30d (*(volatile db*)&mem[0x1bd2d]) /* alias byte_2A30D */
#define select_a33f (*(volatile db*)&mem[0x1bd5f]) /* alias byte_2A33F */
#define select_a3b0 (*(volatile db*)&mem[0x1bdd0]) /* alias byte_2A3B0 */
#define select_a400 (*(volatile db*)&mem[0x1be20]) /* alias byte_2A400 */
#define select_a441 (*(volatile db*)&mem[0x1be61]) /* alias byte_2A441 */
#define select_a4ca (*(volatile db*)&mem[0x1beea]) /* alias byte_2A4CA */
#define rec5_cmp_a506 (*(volatile db*)&mem[0x1bf26]) /* alias byte_2A506 */
#define select_a507 (*(volatile dw*)&mem[0x1bf27]) /* alias word_2A507 */
#define select_a509 (*(volatile dw*)&mem[0x1bf29]) /* alias word_2A509 */
extern db dummy50a3c745_seg002_1bfc2_30923;
#define sel_mask (*(volatile dw*)&mem[0x1bfc3]) /* alias word_2A5A3 */
#define accum_a (*(volatile dw*)&mem[0x1bfc5]) /* alias word_2A5A5 */
#define accum_b (*(volatile dw*)&mem[0x1bfc7]) /* alias word_2A5A7 */
#define arr_flag (*(volatile db*)&mem[0x1bfd0]) /* alias byte_2A5B0 */
#define byte_2a5b1 (*(volatile db*)&mem[0x1bfd1]) /* alias byte_2A5B1 */
#define delta_a5b2 (*(volatile dw*)&mem[0x1bfd2]) /* alias word_2A5B2 */
#define arr_bx (*(volatile dw*)&mem[0x1bfd4]) /* alias word_2A5B4 */
#define arr_si (*(volatile dw*)&mem[0x1bfd6]) /* alias word_2A5B6 */
#define byte_2a5ee (*(volatile db*)&mem[0x1c00e]) /* alias byte_2A5EE */
#define byte_2a5ef (*(volatile db*)&mem[0x1c00f]) /* alias byte_2A5EF */
#define byte_2a5f0 (*(volatile db*)&mem[0x1c010]) /* alias byte_2A5F0 */
#define byte_2a5f1 (*(volatile db*)&mem[0x1c011]) /* alias byte_2A5F1 */
#define byte_2a5f2 (*(volatile db*)&mem[0x1c012]) /* alias byte_2A5F2 */
#define byte_2a5f3 (*(volatile db*)&mem[0x1c013]) /* alias byte_2A5F3 */
#define byte_2a5f4 (*(volatile db*)&mem[0x1c014]) /* alias byte_2A5F4 */
#define byte_2a5f5 (*(volatile db*)&mem[0x1c015]) /* alias byte_2A5F5 */
#define rplan_a5f6 (*(volatile db*)&mem[0x1c016]) /* alias byte_2A5F6 */
#define rplan_a5f7 (*(volatile db*)&mem[0x1c017]) /* alias byte_2A5F7 */
#define rplan_a5f8 (*(volatile db*)&mem[0x1c018]) /* alias byte_2A5F8 */
#define rplan_a5f9 (*(volatile db*)&mem[0x1c019]) /* alias byte_2A5F9 */
#define rplan_a5fa (*(volatile db*)&mem[0x1c01a]) /* alias byte_2A5FA */
#define rplan_a5fb (*(volatile db*)&mem[0x1c01b]) /* alias byte_2A5FB */
#define rplan_a5fc (*(volatile db*)&mem[0x1c01c]) /* alias byte_2A5FC */
#define rplan_a5fd (*(volatile dw*)&mem[0x1c01d]) /* alias word_2A5FD */
#define rplan_a5ff (*(volatile dw*)&mem[0x1c01f]) /* alias word_2A5FF */
#define rplan_a601 (*(volatile db*)&mem[0x1c021]) /* alias byte_2A601 */
#define ai_seek__a602 (*(volatile db*)&mem[0x1c022]) /* alias byte_2A602 */
#define rplan_a603 (*(volatile db*)&mem[0x1c023]) /* alias byte_2A603 */
#define rplan_a604 (*(volatile db*)&mem[0x1c024]) /* alias byte_2A604 */
#define byte_2a605 (*(volatile db*)&mem[0x1c025]) /* alias byte_2A605 */
#define byte_2a606 (*(volatile db*)&mem[0x1c026]) /* alias byte_2A606 */
#define byte_2a60f (*(volatile db*)&mem[0x1c02f]) /* alias byte_2A60F */
#define spawn_a610 (*(volatile dw*)&mem[0x1c030]) /* alias word_2A610 */
#define spawn_a612 (*(volatile db*)&mem[0x1c032]) /* alias byte_2A612 */
#define spawn_a613 (*(volatile db*)&mem[0x1c033]) /* alias byte_2A613 */
#define spawn_a614 (*(volatile db*)&mem[0x1c034]) /* alias byte_2A614 */
#define spawn_a615 (*(volatile db*)&mem[0x1c035]) /* alias byte_2A615 */
extern db dummy50a3c745_seg002_1c0ce_31020;
#define expire_flag (*(volatile db*)&mem[0x1c0cf]) /* alias byte_2A6AF */
#define cellpx_a6b0 (*(volatile db*)&mem[0x1c0d0]) /* alias byte_2A6B0 */
#define cellpx_a6b1 (*(volatile db*)&mem[0x1c0d1]) /* alias byte_2A6B1 */
#define cellpx_a6b2 (*(volatile db*)&mem[0x1c0d2]) /* alias byte_2A6B2 */
#define cellpx_a6b3 (*(volatile db*)&mem[0x1c0d3]) /* alias byte_2A6B3 */
#define oalloc_a6b4 (*(volatile db*)&mem[0x1c0d4]) /* alias byte_2A6B4 */
#define oalloc_a6b5 (*(volatile db*)&mem[0x1c0d5]) /* alias byte_2A6B5 */
#define sfx_id (*(volatile db*)&mem[0x1c0de]) /* alias byte_2A6BE */
#define delta_a6bf (*(volatile dw*)&mem[0x1c0df]) /* alias word_2A6BF */
#define delta_a6c1 (*(volatile db*)&mem[0x1c0e1]) /* alias byte_2A6C1 */
#define delta_a6c2 (*(volatile db*)&mem[0x1c0e2]) /* alias byte_2A6C2 */
#define delta_a6c3 (*(volatile db*)&mem[0x1c0e3]) /* alias byte_2A6C3 */
#define delta_a6c4 (*(volatile db*)&mem[0x1c0e4]) /* alias byte_2A6C4 */
#define delta_a6c5 (*(volatile db*)&mem[0x1c0e5]) /* alias byte_2A6C5 */
#define delta_a6c6 (*(volatile db*)&mem[0x1c0e6]) /* alias byte_2A6C6 */
#define dmg_amt (*(volatile db*)&mem[0x1c16b]) /* alias byte_2A74B */
#define otick_a7a8 (*(volatile dw*)&mem[0x1c1c8]) /* alias word_2A7A8 */
#define byte_2a7aa (*(volatile db*)&mem[0x1c1ca]) /* alias byte_2A7AA */
#define ot01_a7ab (*(volatile db*)&mem[0x1c1cb]) /* alias byte_2A7AB */
#define byte_2a7ac (*(volatile db*)&mem[0x1c1cc]) /* alias byte_2A7AC */
#define alert_lvl (*(volatile db*)&mem[0x1c1cd]) /* alias byte_2A7AD */
#define alert_aux (*(volatile db*)&mem[0x1c1ce]) /* alias byte_2A7AE */
#define byte_2a7af (*(volatile db*)&mem[0x1c1cf]) /* alias byte_2A7AF */
#define word_2a7b0 (*(volatile dw*)&mem[0x1c1d0]) /* alias word_2A7B0 */
#define ot0d_a86b (*(volatile db*)&mem[0x1c28b]) /* alias byte_2A86B */
#define ot0d_a86c (*(volatile db*)&mem[0x1c28c]) /* alias byte_2A86C */
#define objpair__a89c (*(volatile dw*)&mem[0x1c2bc]) /* alias word_2A89C */
#define ot0f_tick_a89e (*(volatile dw*)&mem[0x1c2be]) /* alias word_2A89E */
#define move_intent (*(volatile dw*)&mem[0x1c2c0]) /* alias word_2A8A0 */
#define move_phase (*(volatile dw*)&mem[0x1c2c2]) /* alias word_2A8A2 */
#define type12_f_a8b0 (*(volatile db*)&mem[0x1c2d0]) /* alias byte_2A8B0 */
#define aim_delt_a90a (*(volatile db*)&mem[0x1c32a]) /* alias byte_2A90A */
#define ot20_tick_a933 (*(volatile dw*)&mem[0x1c353]) /* alias word_2A933 */
#define cell_x (*(volatile db*)&mem[0x1c380]) /* alias byte_2A960 */
#define cell_y (*(volatile db*)&mem[0x1c381]) /* alias byte_2A961 */
#define run_a962 (*(volatile dw*)&mem[0x1c382]) /* alias word_2A962 */
#define scroll_cnt (*(volatile dw*)&mem[0x1c388]) /* alias word_2A968 */
#define ground_m_a96a (*(volatile dw*)&mem[0x1c38a]) /* alias word_2A96A */
#define descent__a96c (*(volatile db*)&mem[0x1c38c]) /* alias byte_2A96C */
#define maprow_base (*(volatile dw*)&mem[0x1c3bd]) /* alias word_2A99D */
#define descent__a99f (*(volatile db*)&mem[0x1c3bf]) /* alias byte_2A99F */
#define drift_flag (*(volatile db*)&mem[0x1c3ea]) /* alias byte_2A9CA */
#define run_a9cb (*(volatile db*)&mem[0x1c3eb]) /* alias byte_2A9CB */
extern db dummy50a3c745_seg002_1c3ec_31168;
#define run_a9cd (*(volatile db*)&mem[0x1c3ed]) /* alias byte_2A9CD */
#define word_2a9ce (*(volatile dw*)&mem[0x1c3ee]) /* alias word_2A9CE */
#define run_a9d0 (*(volatile dw*)&mem[0x1c3f0]) /* alias word_2A9D0 */
#define phase_cnt (*(volatile db*)&mem[0x1c3f2]) /* alias byte_2A9D2 */
#define run_a9d3 (*(volatile db*)&mem[0x1c3f3]) /* alias byte_2A9D3 */
#define map_mark_a9d4 (*(volatile dw*)&mem[0x1c3f4]) /* alias word_2A9D4 */
#define run_a9d6 (*(volatile dw*)&mem[0x1c3f6]) /* alias word_2A9D6 */
#define view_pan_a9d8 (*(volatile db*)&mem[0x1c3f8]) /* alias byte_2A9D8 */
#define csetup_a9df (*(volatile db*)&mem[0x1c3ff]) /* alias byte_2A9DF */
#define exit_dir (*(volatile db*)&mem[0x1c400]) /* alias byte_2A9E0 */
#define drawadv_a9e1 (*(volatile db*)&mem[0x1c401]) /* alias byte_2A9E1 */
#define drawadv_a9e2 (*(volatile db*)&mem[0x1c402]) /* alias byte_2A9E2 */
#define drawadv_a9e3 (*(volatile db*)&mem[0x1c403]) /* alias byte_2A9E3 */
#define drawadv_a9e4 (*(volatile db*)&mem[0x1c404]) /* alias byte_2A9E4 */
#define drawadv_a9f9 (*(volatile db*)&mem[0x1c419]) /* alias byte_2A9F9 */
#define map_mark_a9fa (*(volatile db*)&mem[0x1c41a]) /* alias byte_2A9FA */
#define map_exit_a9fb (*(volatile dw*)&mem[0x1c41b]) /* alias word_2A9FB */
#define mission_state (*(volatile dw*)&mem[0x1c42d]) /* alias word_2AA0D */
#define row_meta (*(volatile dw*)&mem[0x1c460]) /* alias word_2AA40 */
#define maprow_idx (*(volatile dw*)&mem[0x1c462]) /* alias word_2AA42 */
#define map_rowh_aa44 (*(volatile dw*)&mem[0x1c464]) /* alias word_2AA44 */
#define map_rowh_aa46 (*(volatile dw*)&mem[0x1c466]) /* alias word_2AA46 */
#define probe_save_bx (*(volatile dw*)&mem[0x1c468]) /* alias word_2AA48 */
#define probe_save_si (*(volatile dw*)&mem[0x1c46a]) /* alias word_2AA4A */
#define map_prob_aa56 (*(volatile dw*)&mem[0x1c476]) /* alias word_2AA56 */
#define map_prob_aa58 (*(volatile dw*)&mem[0x1c478]) /* alias word_2AA58 */
#define byte_2aa5c (*(volatile db*)&mem[0x1c47c]) /* alias byte_2AA5C */
#define rplan_aa5d (*(volatile db*)&mem[0x1c47d]) /* alias byte_2AA5D */
#define byte_2aa5e (*(volatile db*)&mem[0x1c47e]) /* alias byte_2AA5E */
#define byte_2aa5f (*(volatile db*)&mem[0x1c47f]) /* alias byte_2AA5F */
#define motion_t_aa60 (*(volatile db*)&mem[0x1c480]) /* alias byte_2AA60 */
#define ot0d_aa61 (*(volatile db*)&mem[0x1c481]) /* alias byte_2AA61 */
#define byte_2aa62 (*(volatile db*)&mem[0x1c482]) /* alias byte_2AA62 */
#define map_kind (*(volatile db*)&mem[0x1c483]) /* alias byte_2AA63 */
#define byte_2aa64 (*(volatile db*)&mem[0x1c484]) /* alias byte_2AA64 */
#define probe_px (*(volatile dw*)&mem[0x1c485]) /* alias word_2AA65 */
#define probe_py (*(volatile dw*)&mem[0x1c487]) /* alias word_2AA67 */
#define word_2aa69 (*(volatile dw*)&mem[0x1c489]) /* alias word_2AA69 */
#define word_2aa6b (*(volatile dw*)&mem[0x1c48b]) /* alias word_2AA6B */
#define byte_2aa6d (*(volatile db*)&mem[0x1c48d]) /* alias byte_2AA6D */
#define byte_2aa6e (*(volatile db*)&mem[0x1c48e]) /* alias byte_2AA6E */
#define byte_2aa6f (*(volatile db*)&mem[0x1c48f]) /* alias byte_2AA6F */
#define map_scroll (*(volatile dw*)&mem[0x1ca90]) /* alias word_2B070 */
#define route_ma_b074 (*(volatile dw*)&mem[0x1ca94]) /* alias word_2B074 */
#define route_ma_b076 (*(volatile db*)&mem[0x1ca96]) /* alias byte_2B076 */
#define ot16_b0c3 (*(volatile db*)&mem[0x1cae3]) /* alias byte_2B0C3 */
#define word_2b0c4 (*(volatile dw*)&mem[0x1cae4]) /* alias word_2B0C4 */
#define fill_a_b0c6 (*(volatile db*)&mem[0x1cae6]) /* alias byte_2B0C6 */
#define fill_b_b0c7 (*(volatile db*)&mem[0x1cae7]) /* alias byte_2B0C7 */
#define legend_b0c8 (*(volatile dw*)&mem[0x1cae8]) /* alias word_2B0C8 */
#define mgen_b0ca (*(volatile db*)&mem[0x1caea]) /* alias byte_2B0CA */
#define mgen_b0cb (*(volatile dw*)&mem[0x1caeb]) /* alias word_2B0CB */
#define mgen_b0cd (*(volatile dw*)&mem[0x1caed]) /* alias word_2B0CD */
#define mgena_b0cf (*(volatile db*)&mem[0x1caef]) /* alias byte_2B0CF */
#define mgena_b0d0 (*(volatile db*)&mem[0x1caf0]) /* alias byte_2B0D0 */
#define mgena_b0d1 (*(volatile db*)&mem[0x1caf1]) /* alias byte_2B0D1 */
#define mgena_b0d2 (*(volatile db*)&mem[0x1caf2]) /* alias byte_2B0D2 */
#define mgen_b0d3 (*(volatile db*)&mem[0x1caf3]) /* alias byte_2B0D3 */
#define mgen_b0d4 (*(volatile db*)&mem[0x1caf4]) /* alias byte_2B0D4 */
#define mgenb_b115 (*(volatile db*)&mem[0x1cb35]) /* alias byte_2B115 */
#define mgenb_b116 (*(volatile db*)&mem[0x1cb36]) /* alias byte_2B116 */
#define mgenb_b117 (*(volatile db*)&mem[0x1cb37]) /* alias byte_2B117 */
#define mgenc_b118 (*(volatile db*)&mem[0x1cb38]) /* alias byte_2B118 */
#define mgenc_b119 (*(volatile db*)&mem[0x1cb39]) /* alias byte_2B119 */
#define mgenc_b11a (*(volatile db*)&mem[0x1cb3a]) /* alias byte_2B11A */
#define mgenc_b11b (*(volatile dw*)&mem[0x1cb3b]) /* alias word_2B11B */
#define oalloc_b11d (*(volatile db*)&mem[0x1cb3d]) /* alias byte_2B11D */
#define mgend_b11e (*(volatile db*)&mem[0x1cb3e]) /* alias byte_2B11E */
#define mgend_b11f (*(volatile db*)&mem[0x1cb3f]) /* alias byte_2B11F */
#define mgend_b120 (*(volatile db*)&mem[0x1cb40]) /* alias byte_2B120 */
#define mgend_b121 (*(volatile dw*)&mem[0x1cb41]) /* alias word_2B121 */
#define pick_e_b123 (*(volatile db*)&mem[0x1cb43]) /* alias byte_2B123 */
extern dw dummy50a3c745_seg002_1cb44_31340;
#define retry_b126 (*(volatile db*)&mem[0x1cb46]) /* alias byte_2B126 */
#define byte_2b127 (*(volatile db*)&mem[0x1cb47]) /* alias byte_2B127 */
#define byte_2b128 (*(volatile db*)&mem[0x1cb48]) /* alias byte_2B128 */
#define retry_b129 (*(volatile db*)&mem[0x1cb49]) /* alias byte_2B129 */
#define byte_2b12a (*(volatile db*)&mem[0x1cb4a]) /* alias byte_2B12A */
#define byte_2b12b (*(volatile db*)&mem[0x1cb4b]) /* alias byte_2B12B */
#define spawn_b12c (*(volatile db*)&mem[0x1cb4c]) /* alias byte_2B12C */
#define mgen6_b12d (*(volatile db*)&mem[0x1cb4d]) /* alias byte_2B12D */
#define mgen6_b12e (*(volatile db*)&mem[0x1cb4e]) /* alias byte_2B12E */
#define mgen6_b12f (*(volatile db*)&mem[0x1cb4f]) /* alias byte_2B12F */
#define mgen6_b130 (*(volatile db*)&mem[0x1cb50]) /* alias byte_2B130 */
#define fill_8_b131 (*(volatile db*)&mem[0x1cb51]) /* alias byte_2B131 */
#define fill_8_b132 (*(volatile db*)&mem[0x1cb52]) /* alias byte_2B132 */
#define mrect_b23c (*(volatile db*)&mem[0x1cc5c]) /* alias byte_2B23C */
#define mrect_b23d (*(volatile db*)&mem[0x1cc5d]) /* alias byte_2B23D */
#define mrect_b23e (*(volatile dw*)&mem[0x1cc5e]) /* alias word_2B23E */
#define mrect_b240 (*(volatile dw*)&mem[0x1cc60]) /* alias word_2B240 */
#define mrect_b242 (*(volatile db*)&mem[0x1cc62]) /* alias byte_2B242 */
#define byte_2b243 (*(volatile db*)&mem[0x1cc63]) /* alias byte_2B243 */
#define byte_2b244 (*(volatile db*)&mem[0x1cc64]) /* alias byte_2B244 */
#define cellpx_b245 (*(volatile dw*)&mem[0x1cc65]) /* alias word_2B245 */
#define cellpx_b247 (*(volatile dw*)&mem[0x1cc67]) /* alias word_2B247 */
#define ot1b_tick_b24d (*(volatile db*)&mem[0x1cc6d]) /* alias byte_2B24D */
extern db dummy50a3c745_seg002_1cc6e_31410;
#define tile_loo_b24f (*(volatile dw*)&mem[0x1cc6f]) /* alias word_2B24F */
#define tile_loo_b251 (*(volatile dw*)&mem[0x1cc71]) /* alias word_2B251 */
#define map_exit_b253 (*(volatile dw*)&mem[0x1cc73]) /* alias word_2B253 */
#define ot1a_tick_b2ce (*(volatile db*)&mem[0x1ccee]) /* alias byte_2B2CE */
#define ot16_b3f6 (*(volatile db*)&mem[0x1ce16]) /* alias byte_2B3F6 */
#define ot16_b3f7 (*(volatile db*)&mem[0x1ce17]) /* alias byte_2B3F7 */
extern db seg003;
extern db seg004;
extern db seg005;
extern db seg006;
extern db seg007;
extern db seg008;
extern db seg009;
