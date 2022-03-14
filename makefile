OBJDIR = obj
SRCDIR = c2048
OUTDIR = bin

_OUT = c2048

_OBJS	= c2048.o getch_raw.o
_SOURCE = c2048.c getch_raw.c
_HEADER	= c2048.h getch_raw.h


OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))
SOURE = $(patsubst %,$(SRCDIR)/%,$(_SOURCE))
HEADER = $(patsubst %,$(SRCDIR)/%,$(_HEADER))


OUT	= $(patsubst %,$(OUTDIR)/%,$(_OUT))
CC	 = gcc
FLAGS	 = -g -c -Wall -Wno-char-subscripts -Wno-switch -std=c89
LFLAGS	 = 

all: $(OBJS)
	mkdir -p $(OBJDIR)
	mkdir -p $(OUTDIR)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

obj/c2048.o: c2048/c2048.c
	$(CC) $(FLAGS) c2048/c2048.c -o obj/c2048.o

obj/getch_raw.o: c2048/getch_raw.c
	$(CC) $(FLAGS) c2048/getch_raw.c -o obj/getch_raw.o


clean:
	rm -f $(OBJS) $(OUT)