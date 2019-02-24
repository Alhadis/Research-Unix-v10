all:	bb lprint nexit.o

bb:	bb.o

lprint:	lprint.o

install:	all
	cp bb nexit.o /usr/lib
	strip /usr/lib/bb
	cp lprint /usr/bin
	strip /usr/bin/lprint

clean:
	rm -f *.o bb lprint lcomp
