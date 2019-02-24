CC=3cc
CFLAGS=-J
AS=3as
AR=3ar

L=libc.a

OBJ=abs.o atoi.o atol.o chrtab.o ctype.o index.o qsort.o rand.o rindex.o strcat.o strchr.o strrchr.o strcmp.o strcpy.o strlen.o strncat.o strncmp.o strncpy.o swab.o

$L: $(OBJ)
	$(AR) uv $L $(OBJ)

install: $L
	cp $L /usr/jerq/lib

clean:
	rm -f *.[oa]
