// TANDYSND overlay data symbols -> mem[tnd_base + image offset]
#pragma once
#define unk_1009f (*(volatile db*)&mem[tnd_base + 0x9f])
#define byte_100d7 (*(volatile db*)&mem[tnd_base + 0xd7])
#define byte_100d9 (*(volatile db*)&mem[tnd_base + 0xd9])
#define off_10380 (*(volatile dw*)&mem[tnd_base + 0x380])
#define dword_103de (*(volatile dd*)&mem[tnd_base + 0x3de])
#define byte_103e2 (*(volatile db*)&mem[tnd_base + 0x3e2])
