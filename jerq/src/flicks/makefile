ALL=flicks fflicks preflicks fflicks.m flicks.m
CC=/usr/jerq/bin/3cc
CFLAGS=-g

all:	$(ALL)

flicks:	flicks.c piclib.o
	cc -g -o flicks flicks.c piclib.o -lfb

piclib.o:	piclib.c piclib.h
	cc -g -c piclib.c

preflicks.o:	preflicks.c
	cc -g -c preflicks.c

fflicks:	fflicks.c
	cc -g -o fflicks fflicks.c

preflicks:	preflicks.o piclib.o
	cc -g -o preflicks preflicks.o piclib.o -lfb

fflicks.m:	fjerq.o fcommon.o
	/usr/jerq/bin/3cc -g -o fflicks.m fjerq.o fcommon.o -ljj

flicks.m:	jerq.o fcommon.o
	/usr/jerq/bin/3cc -g -o flicks.m jerq.o fcommon.o -ljj

udither: udither.c
	cc -g udither.c -o udither

ldither: ldither.c
	cc -g ldither.c -o ldither

udither.data: udither.c
	make udither
	udither >udither.data

ldither.data: ldither.c
	make ldither
	ldither >ldither.data

install:	all
	cp flicks fflicks preflicks /usr/jerq/bin
	cp flicks.m fflicks.m /usr/jerq/mbin

ship:	install
	ship /usr/jerq/bin/preflicks /usr/jerq/bin/flicks /usr/jerq/bin/fflicks /usr/jerq/mbin/fflicks.m /usr/jerq/mbin/flicks.m

clean:
	rm -f *.o $(ALL)
