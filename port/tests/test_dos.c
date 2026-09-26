/* unit tests for dos.c: file ops, find patterns, memory alloc, int vectors.
 * includes dos.c directly to reach static helpers (asciz, match_pat). */
#include "../rt.h"
#include <stdio.h>
#include <unistd.h>

void text_putc(db c){ (void)c; }         /* video.c stubs */
dw rt_in(dw p){ (void)p; return 0; }
void rt_out(dw p, dw v){ (void)p; (void)v; }
vfn func_at(dd a){ (void)a; return 0; }

#include "../dos.c"

static int fails, checks;
#define CHECK(cond) do{ checks++; if(!(cond)){ fails++; \
    fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); } }while(0)

static void put_asciz(dw seg, dw off, const char *s){
    strcpy((char*)raddr_(seg,off), s);
}

int main(void){
    /* --- asciz --- */
    char buf[64];
    put_asciz(0x100, 0x20, "FOO.DTX");
    CHECK(!strcmp(asciz(0x100,0x20,buf,sizeof buf), "FOO.DTX"));

    /* --- open/read/seek/close on a temp fixture --- */
    const char *tp = "/tmp/ar_t_dos.bin";
    FILE *fx = fopen(tp, "wb");
    for (int i=0;i<64;i++) fputc(i, fx);
    fclose(fx);

    ds = 0x100; dx = 0x20;
    put_asciz(ds, dx, tp);
    dos_open();
    CHECK(CF == 0); CHECK(ax > 0 && ax < MAXH);
    dw h = ax;

    ds = 0x200; dx = 0x100; cx = 16; bx = h;
    dos_read();
    CHECK(CF == 0); CHECK(ax == 16);
    CHECK(mem_b(0x2100) == 0 && mem_b(0x210f) == 15);   /* ds:dx = 0x200:0x100 */

    al = 0; cx = 0; dx = 32; bx = h;      /* SEEK_SET 32 */
    dos_seek();
    CHECK(CF == 0); CHECK(ax == 32 && dx == 0);
    ds = 0x200; dx = 0x100; cx = 4;
    dos_read();
    CHECK(ax == 4); CHECK(mem_b(0x2100) == 32);

    al = 2; cx = 0xffff; dx = 0xfff0;     /* SEEK_END -16 */
    dos_seek();
    CHECK(CF == 0); CHECK(ax == 48);
    bx = h; dos_close();
    CHECK(CF == 0);

    /* open failure: CF + ax=2 */
    ds = 0x100; put_asciz(ds, dx, "/tmp/definitely_not_here.xyz");
    dos_open();
    CHECK(CF == 1); CHECK(ax == 2);

    /* --- match_pat (DOS wildcards) --- */
    CHECK(match_pat("FOO.DTX", "FOO.DTX"));
    CHECK(match_pat("FOO.DTX", "*.DTX"));
    CHECK(match_pat("FOO.DTX", "FOO.*"));
    CHECK(match_pat("FOO.DTX", "F?O.DTX"));
    CHECK(!match_pat("FOO.DTX", "*.DAT"));
    CHECK(match_pat("FOO.DTX", "*.*"));
    CHECK(!match_pat("FOODTX", "FOO.DTX"));
    CHECK(match_pat("a.dtx", "*.DTX"));            /* case-insensitive */
    CHECK(!match_pat("FOO.DTXX", "*.DTX"));

    /* --- find_first/find_next against a fixture dir --- */
    mkdir("/tmp/ar_ff", 0755);
    fclose(fopen("/tmp/ar_ff/ONE.DTX","w"));
    fclose(fopen("/tmp/ar_ff/TWO.DTX","w"));
    fclose(fopen("/tmp/ar_ff/THREE.DAT","w"));
    ds = 0x100; dx = 0x20;
    put_asciz(ds, dx, "/tmp/ar_ff/*.DTX");
    dx = 0x800; dos_set_dta();                     /* DTA at 0x100:0x800 */
    dx = 0x20;
    dos_find_first();
    CHECK(CF == 0);
    const char *n1 = (char*)&mem[dta + 30];
    CHECK(match_pat(n1, "*.DTX"));
    int found = 1;
    while (1){
        dos_find_next();
        if (CF) break;
        found++;
        CHECK(match_pat((char*)&mem[dta + 30], "*.DTX"));
    }
    CHECK(found == 2);

    /* --- int vectors --- */
    al = 0x1c; dx = 0x14d6; ds = 0x1a2;
    dos_set_int_vector();
    CHECK(*(dw*)&mem[0x1c*4] == 0x14d6 && *(dw*)&mem[0x1c*4+2] == 0x1a2);
    dos_get_int_vector();
    CHECK(es == 0x1a2 && bx == 0x14d6);

    /* --- memory allocation --- */
    nablk = nfblk = 0; alloc_next = 0x80000;
    bx = 0x100; dos_alloc();                       /* 0x100 paras */
    CHECK(CF == 0); CHECK(ax == 0x8000);
    dw seg1 = ax;
    bx = 0x100; dos_alloc();
    CHECK(ax == 0x8100);
    bx = 0x2000; dos_alloc();                      /* too big: 0x1e00 avail */
    CHECK(CF == 1); CHECK(bx == 0x1e00);
    es = seg1; dos_free();                          /* free first -> free list */
    bx = 0x80; dos_alloc();                         /* reuses freed block */
    CHECK(CF == 0); CHECK(ax == 0x8000);
    bx = 0x80; dos_alloc();                         /* leftover half */
    CHECK(ax == 0x8080);

    /* resize shrink returns tail to free list / frontier */
    bx = 0x40; es = 0x8080; dos_resize();
    CHECK(CF == 0);
    bx = 0xc0; dos_alloc();                         /* 0x40c0 fits freed space */
    CHECK(CF == 0);

    /* --- misc services --- */
    dos_get_version(); CHECK(ax == 0x0500);
    dos_get_drive();   CHECK(al == 2);
    dos_get_psp();     CHECK(bx == 0x1a2);

    fprintf(stderr, "test_dos: %d checks, %d failures\n", checks, fails);
    return fails ? 1 : 0;
}
