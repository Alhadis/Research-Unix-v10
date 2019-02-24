D=/usr/bin/factor
CFLAGS = -O

factor:	factor.o
	cc -o factor factor.o -lm

install:	factor
	cp factor $D
	strip $D

clean:
	rm -f factor *.o core a.out
