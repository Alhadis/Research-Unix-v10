CFLAGS = -O 

all: chp chpterm

chp: chp.o subr.o struct.h
	cc $(CFLAGS) -o chp chp.o subr.o

chpterm: chpterm.c struct.h 3subr.o
	3cc -o chpterm chpterm.c 3subr.o

subr.o: subr.c struct.h

3subr.o: subr.c struct.h
	rm -f 3subr.c
	ln subr.c 3subr.c
	3cc -c 3subr.c
	rm -f 3subr.c

clean:
	rm -f *.o chp chpterm

install:
	cp chp /usr/jerq/bin
	cp chpterm /usr/jerq/mbin/chp.m
