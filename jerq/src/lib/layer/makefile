L=liblayer.a
CC=3cc
OBJ=   dellayer.o downback.o layerop.o lbitblt.o lblt.o lpoint.o lrectf.o \
	lsegment.o ltexture.o newlayer.o upfront.o lbox.o setvis.o
CFLAGS=-J

$L: $(OBJ)
	3ar uv $L $^

clean:
	rm -f core *.o

install:	$L
	rm -f /usr/jerq/lib/liblayer.a
	ln $L /usr/jerq/lib/liblayer.a
