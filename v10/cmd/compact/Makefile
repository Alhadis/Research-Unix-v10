CFLAGS=-Od2
DESTDIR=

all: compact uncompact

compact: compact.o tree.o
	cc compact.o tree.o -o compact

uncompact: uncompact.o tree.o
	cc uncompact.o tree.o -o uncompact

install:	all
	strip compact uncompact
	mv compact uncompact ${DESTDIR}/usr/bin
	cp ccat.sh ${DESTDIR}/usr/bin/ccat

clean:
	rm -f compact.o uncompact.o tree.o
