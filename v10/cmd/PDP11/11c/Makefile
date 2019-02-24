#	Makefile for Ritchie c compiler, portable version
#
LIBDIR = /usr/lib
BINDIR = /usr/bin

C0 = 11c0
C1 = 11c1
C2 = 11c2

CFLAGS = -O

C0FILES = c00.o c01.o c02.o c03.o c04.o c05.o
C1FILES = c10.o c11.o c12.o c13.o
C2FILES = c20.o c21.o

all:	${C0} ${C1} ${C2}
	@echo \`all\' is up to date: $?

${C0}: ${C0FILES}
	cc ${CFLAGS} -o ${C0} ${C0FILES}

${C0FILES}: c0.h

${C1}: ${C1FILES} table.o
	cc ${CFLAGS} -o ${C1} ${C1FILES} table.o

${C1FILES}: c1.h

table.o: optable cvopt
	./cvopt <optable >junk.c
	cc -S junk.c
	sed 's/\.data/\.text/' <junk.s >junk.i
	as -o table.o junk.i
	rm junk.i junk.c junk.s

${C2}: ${C2FILES}
	cc ${CFLAGS} -o ${C2} ${C2FILES}

${C2FILES}: c2.h

cvopt:	cvopt.c
	cc ${CFLAGS} -s -o cvopt cvopt.c

inst${C0}: ${C0}
	strip $^
	cp $^ ${LIBDIR}

inst${C1}: ${C1}
	strip $^
	cp $^ ${LIBDIR}

inst${C2}: ${C2}
	strip $^
	cp $^ ${LIBDIR}

install: inst${C0} inst${C1} inst${C2}
	@echo The c compiler is installed

clean:
	rm -f *.o make.out
	rm -f *.o make.out ${C0} ${C1} ${C2} cvopt

print:
	-@pr -r README Makefile c0.h c0*.c c1.h c1*.c optable
