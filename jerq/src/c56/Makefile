LMODS = c56 c56.m
J = /usr/jerq

CFLAGS = -O

all: $(LMODS)

c56: c56.o subr.o struct.h
	cc $(CFLAGS) -o c56 c56.o subr.o

c56.m: c56term.c struct.h 3subr.o
	3cc -o c56.m c56term.c 3subr.o

subr.o: subr.c struct.h

3subr.o: subr.c struct.h
	rm -f 3subr.c
	ln subr.c 3subr.c
	3cc -c 3subr.c
	rm -f 3subr.c

install: all
	cp c56.m $J/mbin
	cp c56 $J/bin

clean:
	rm -f *.o $(LMODS)
