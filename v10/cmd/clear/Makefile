CFLAGS = -Od2

clear:	clear.o
	cc -o clear clear.o -ltermlib

install:	clear
	strip clear
	mv clear /usr/bin

clean:
	rm -f clear *.o
