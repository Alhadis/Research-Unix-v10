CFLAGS = -Od2

pp:	pp.o scan.o
	cc -o pp pp.o scan.o -ll

install: pp
	strip pp
	cp pp /usr/bin

clean:
	rm -f pp pp.o scan.o

pp.o: dev.h
