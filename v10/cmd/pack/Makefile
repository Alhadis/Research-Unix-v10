CFLAGS = -Od2

all: pack unpack
pack: pack.o
	cc -o pack pack.o
unpack:	unpack.o
	cc -o unpack unpack.o

install: all
	strip pack unpack
	cp pack unpack /usr/bin
	rm -f /usr/bin/pcat
	ln /usr/bin/unpack /usr/bin/pcat

clean:
	rm -f *.o pack unpack
