CC=/usr/jerq/bin/3cc
CFLAGS=-g
HFLS =  textures.h rebecca.h instruments.h
INST =  gridbutton.o screen.o greyscale.o mover.o \
	resolver.o probe.o zoom.o paint.o doit.o
OBJS =	main.o other.o instruments.o sheet.o

all:		rebecca rebecca.m

rebecca:	rebecca.c piclib.o
		cc -g -o rebecca rebecca.c piclib.o -lfb -lram

piclib.o:	piclib.c piclib.h
		cc -g -c piclib.c

rebecca.m:	$(OBJS) $(INST)
		/usr/jerq/bin/3cc -g -o rebecca.m $(OBJS) $(INST) -ljj

doit.o:		doit.c textures.h
		/usr/jerq/bin/3cc -g -c doit.c

install:	all
		cp rebecca /usr/jerq/bin
		cp rebecca.m /usr/jerq/mbin

%.o:		%.c
		/usr/jerq/bin/3cc $(CFLAGS) -c $%.c

clean:
		rm -f *.o pvmon.m pvmon pv core a.out
