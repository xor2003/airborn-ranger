// lifted Airborne Ranger — readable decompilation artifact
#include <stdint.h>
typedef uint8_t db; typedef uint16_t dw; typedef uint32_t dd;
extern dd eax,ebx,ecx,edx,esi,edi,esp,ebp;
extern dw ax,bx,cx,dx,si,di,sp,bp,cs,ds,es,fs,gs,ss,ip;
extern db al,ah,bl,bh,cl,ch,dl,dh;
extern int CF,ZF,SF,OF,PF,AF,DF,IF,TF;
extern db mem[];
__attribute__((always_inline)) static inline db *raddr_(int seg,int off){return &mem[((seg&0xffff)<<4)+(off&0xffff)];}
__attribute__((always_inline)) static inline db *raddr(int seg,int off){return raddr_(seg,off);}
void push(dw); dw pop(void); void pushf(void); void popf(void);
dw rol16(dw,int); dw ror16(dw,int); dw rcl16(dw,int); dw rcr16(dw,int);
dw in(dw); void out(dw,dw); void swi(int);
void dos_int21(dw); void bios_video(dw); void bios_kbd(dw); void bios_time(void);
#define swap(a,b) do{__typeof__(a)_t=(a);(a)=(b);(b)=_t;}while(0)
// named int21/int10/int16 wrappers
void dos_exit(void);void dos_read_char_echo(void);void dos_write_char(void);void dos_direct_io(void);
void dos_read_char_noecho(void);void dos_print_string(void);void dos_read_string(void);
void dos_check_stdin(void);void dos_get_drive(void);void dos_set_dta(void);void dos_set_int_vector(void);
void dos_get_date(void);void dos_get_time(void);void dos_get_version(void);void dos_get_int_vector(void);
void dos_open(void);void dos_close(void);void dos_read(void);void dos_write(void);void dos_delete(void);
void dos_seek(void);void dos_get_attr(void);void dos_ioctl(void);void dos_alloc(void);void dos_free(void);
void dos_resize(void);void dos_exec(void);void dos_exit_code(void);void dos_find_first(void);
void dos_find_next(void);void dos_get_verify(void);void dos_rename(void);void dos_file_datetime(void);
void dos_create_temp(void);void dos_create_new(void);void dos_get_psp(void);
void bios_set_mode(void);void bios_set_cursor(void);void bios_scroll(void);void bios_putc(void);
void bios_palette(void);void bios_getch(void);void bios_kbhit(void);
// data symbols live in the segment images (ar.exe.h)
