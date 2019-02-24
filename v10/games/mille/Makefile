#	@(#)Makefile	4.1	12/24/82
#
HEADERS=mille.h
CFILES=	comp.c end.c extern.c init.c mille.c misc.c move.c print.c \
	roll.c save.c types.c varpush.c
OBJS=	comp.o end.o extern.o init.o mille.o misc.o move.o print.o \
	roll.o save.o types.o varpush.o
LIBS=	-lcurses -ltermlib
CFLAGS=	-O -DSTANDOUT
.SUFFIXES: .po .i

mille: ${OBJS}
	${CC} ${CFLAGS} -o mille ${OBJS} ${LIBS}

install: mille
	mv mille /usr/games/mille

clean:
	rm -f ${OBJS} mille a.out

table: table.o extern.o
	${CC} ${CFLAGS} -i -o table table.o extern.o

readdump: readdump.o extern.o varpush.o
	${CC} ${CFLAGS} -i -o readdump readdump.o extern.o varpush.o

