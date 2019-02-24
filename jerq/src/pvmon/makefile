CC=/usr/jerq/bin/3cc
CFLAGS=-g

all:	pv pvmon pvmon.m

pv:	pvdither.c piclib.o
	cc -g -o pv pvdither.c piclib.o -lfb

piclib.o:	piclib.c piclib.h
		cc -g -c piclib.c

pvmon:	pvhost.c
	cc -g -o pvmon pvhost.c

pvmon.m:	pvterm.c
	/usr/jerq/bin/3cc -g -o pvmon.m pvterm.c

install:	all
	cp pv /usr/jerq/bin
	cp pvmon.m /usr/jerq/mbin
	cp pvmon /usr/jerq/bin

clean:
	rm -f *.o pvmon.m pvmon pv core a.out
