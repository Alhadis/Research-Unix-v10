CFLAGS = -O

all: atc sread

install:	all
	if [ ! -d /usr/games/lib/atc ] ;\
	then	mkdir /usr/games/lib/atc ;\
		chown bin /usr/games/lib/atc ;\
		chgrp bin /usr/games/lib/atc ;\
		chmod o-w,g+w /usr/games/lib/atc ;\
	fi
	strip atc
	mv atc /usr/games
	cp airspaces /usr/games/lib/atc

atc.o aread.o: ahdr.h

atc: atc.o aread.o
	cc -o atc atc.o aread.o -ltermlib

sread: sread.o
	cc -o sread sread.o

clean:
	rm -f atc *.o
