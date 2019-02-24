L=liblayer.a
CC=3cc
INCL=/usr/dud/include
DCAN=dcan
SOURCE=dellayer.c downback.c layerop.c lbitblt.c lblt.c lpoint.c lrectf.c \
	lsegment.c ltexture.c newlayer.c upfront.c lbox.c
OBJ=   dellayer.o downback.o layerop.o lbitblt.o lblt.o lpoint.o lrectf.o \
	lsegment.o ltexture.o newlayer.o upfront.o lbox.o
CFLAGS=-J

$L: $(OBJ)
	3ar uv $L $^

lint:
	lint $(SOURCE)

print:
	@pr layer.h $(SOURCE)

pp:
	pp -tLayer -kjerq layer.h $(SOURCE) | $(DCAN)

newlayer.a: $(OBJ)
	3ar cr layer.a `mlorder $(OBJ) | tsort`

clean:
	rm -f core *.o

