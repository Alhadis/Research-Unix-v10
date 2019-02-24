CFLAGS = -Od2

ul:	ul.o
	cc -o ul ul.o -ltermcap

install:	ul
	strip ul
	cp ul /usr/bin

clean:
	rm -f ul *.o
