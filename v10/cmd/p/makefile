CFLAGS = -Od2 # -DSPNAME

p:	p.o spname.o pad.o
	cc -o p p.o spname.o pad.o

pad.o:	pad.h

install:	p
	strip p
	cp p /usr/bin

clean:
	rm -f p *.o
