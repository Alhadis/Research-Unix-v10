CC=3cc
CFLAGS=

lens:	lens.o magn.o mag2.o mag3.o mageven.o magodd.o magslow.o
	3cc -o lens lens.o magn.o mag2.o mag3.o mageven.o magodd.o magslow.o

clean:
	rm -f *.o m.out core

install:	lens
	cp lens /usr/jerq/mbin/lens.m
