CC ?= gcc
CFLAGS ?= -O2 -g -fcommon -fsigned-char -Wno-implicit-function-declaration -fno-strict-aliasing
SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LIBS := $(shell sdl2-config --libs) -lm

OBJS = rt.o dos.o video.o input.o snd.o main.o memimg.o data_defs.o rewrite.o \
       gen/ar.exe.o gen/ar.exe_seg000.o gen/ar.exe_seg002.o \
       gen/ar.exe_seg001.o gen/ar.exe_seg003.o gen/ar.exe_default_seg.o \
       gen/tandysnd_seg001.o

ar_port: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(SDL_LIBS)

%.o: %.c rt.h data_syms.h procs.h
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -I. -c -o $@ $<

GAME_OBJS = $(filter-out main.o,$(OBJS))

tests/t_rt: tests/test_rt.c rt.o rt.h
	$(CC) $(CFLAGS) -I. -o $@ $< rt.o
tests/t_dos: tests/test_dos.c dos.c rt.o rt.h data_syms.h
	$(CC) $(CFLAGS) -I. -o $@ $< rt.o
tests/t_input: tests/test_input.c input.c rt.o rt.h data_syms.h
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -I. -o $@ $< rt.o $(SDL_LIBS)
tests/t_snd: tests/test_snd.c snd.c rt.o rt.h data_syms.h
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -I. -o $@ $< rt.o $(SDL_LIBS)
tests/t_video: tests/test_video.c video.c rt.o rt.h data_syms.h
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -I. -o $@ $< rt.o $(SDL_LIBS) -lpthread
tests/t_game: tests/test_game.c main.c $(GAME_OBJS) rt.h data_syms.h procs.h
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -I. -o $@ $< $(GAME_OBJS) $(SDL_LIBS) -lpthread

check: tests/t_rt tests/t_dos tests/t_input tests/t_snd tests/t_video tests/t_game
	@fail=0; for t in tests/t_rt tests/t_dos tests/t_input tests/t_snd tests/t_video tests/t_game; do \
		$$t || fail=1; done; \
	[ $$fail = 0 ] && echo "ALL TESTS PASSED" || { echo "TESTS FAILED"; exit 1; }

OPT_CFLAGS ?= -O3 -g -march=native -fcommon -Wno-implicit-function-declaration -fno-strict-aliasing
opt: clean
	$(MAKE) CFLAGS="$(OPT_CFLAGS)" ar_port

clean:
	rm -f $(OBJS) ar_port tests/t_rt tests/t_dos tests/t_input tests/t_snd tests/t_video tests/t_game
.PHONY: clean check opt
