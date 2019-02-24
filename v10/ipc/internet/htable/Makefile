CFLAGS=	-g
LIBC=	/lib/libc.a
SRCS=	htable.c parse.c scan.c
OBJS=	htable.o parse.o scan.o
MAN=	htable.0

all: htable

htable: ${OBJS} ${LIBC} y.tab.h
	${CC} -o $@ ${CFLAGS} ${OBJS}

y.tab.h:
	yacc -d parse.y
	rm -f y.tab.c

clean:
	rm -f ${OBJS} core htable parse.c scan.c y.tab.h

