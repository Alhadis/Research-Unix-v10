CFLAGS=-O

all:	calendar1 calendar2 calendar4

calendar1: calendar1.o
	cc calendar1.o -o calendar1

calendar2: calendar2.o
	cc calendar2.o -o calendar2

calendar4: calendar4.o
	cc calendar4.o -o calendar4

install:	all
	strip calendar1 calendar2 calendar4
	cp calendar /usr/bin
	cp calendar3 /usr/lib
	mv calendar1 calendar2 calendar4 /usr/lib

clean:
	rm -f calendar[124] *.o
