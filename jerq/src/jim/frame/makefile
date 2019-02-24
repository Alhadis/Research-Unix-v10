CFLAGS=-g
CC=3cc
AR=3ar
FILES=deltext.o fralloc.o frameop.o instext.o ptofchar.o
CFILES=deltext.c fralloc.c frameop.c instext.c ptofchar.c 

frame.a:	$(FILES)
	$(AR) uv frame.a $(FILES)

print:
	@pr frame.h $(CFILES)

lint:
	lint -I/usr/jerq/include -DMUX $(CFILES) -lj 2>&1 | /usr/rob/bin/buffer

$(FILES):	frame.h

clean:
	rm -f *.o core
