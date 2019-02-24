#	makefile for vis
#
#	A. F. Gettier
#	S. Coffin
#	Bell Laboratories
#	March, 1982
#
YFLAGS= -d
CFLAGS= -O 
SRC= lexio.c init.c queues.c lex.l parse.y vis.c nodes.c screen.c hashing.c macros.c
LSRC= lexio.c init.c queues.c vis.c nodes.c screen.c hashing.c lex.yy.c y.tab.c macros.c
OBJ= vis.o init.o parse.o lex.o lexio.o queues.o nodes.o screen.o hashing.o macros.o
all:	visi


visi:	${OBJ}
	cc ${OBJ} -o visi -lm -ly -ll -lcurses -ltermlib

${OBJ}:	vis.h

hashing.o lex.o:	vis.h y.tab.h

print:
	pr -n vis.h ${SRC} makefile

lint:
	lex lex.l
	yacc parse.y
	lint ${LSRC}

install:
	strip visi
	mv visi /usr/bin
	cp helpfile /usr/lib/vis.help

clean:
	rm -f visi *.o
