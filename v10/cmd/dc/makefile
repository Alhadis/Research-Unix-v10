CFLAGS = -Od2

dc:	dc.o
	cc -o dc dc.o

dc.o:	dc.c dc.h

install:	dc
	strip dc
	mv dc /usr/bin

clean:
	rm -f *.o dc
