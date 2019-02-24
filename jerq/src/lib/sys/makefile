L=libsys.a
INCL=/usr/jerq/include
CC=3cc
AS=3as
AR=3ar
CFLAGS=-J

OBJ=acia.o bram.o buttons.o cursblt.o cursor.o kbd.o queue.o setup.o
CFILES=acia.c bram.c buttons.c cursblt.s cursor.c kbd.c queue.c setup.c

$L:	$(OBJ)
	$(AR) uv $L $^

install: $L
	cp $L /usr/jerq/lib

clean:
	rm -f *.[oa]

print:
	@pr $(CFILES)

pp:
	pp -tSystem -kjerq $(CFILES) | dcan

lint:
	lint -h -c -u -n -I/usr/jerq/include $(CFILES)
