#
#	makefile for J F Reiser's PDP11 Assembler written in C
#	{so it can be used on the VAX}
#
#	BINDIR and LIBDIR must be defined.
#
BINDIR = /usr/bin
LIBDIR = /usr/lib

AS1 = 11as
AS2 = 11as2
A_DOT_OUT = a.out

CC = cc
CFLAGS = -O "-DPASS2PGM=\"${LIBDIR}/${AS2}\"" "-DA_DOT_OUT=\"${A_DOT_OUT}\""

AS1O = as11.o as12.o as13.o as14.o as15.o as16.o as17.o as18.o as19.o
AS2O = as21.o as22.o as23.o as24.o as25.o as26.o as27.o as28.o as29.o

all: ${AS1} ${AS2}

${AS1}: ${AS1O}
	${CC} ${CFLAGS} -o ${AS1} ${AS1O}

${AS1O}: as1.h

${AS2}: ${AS2O}
	${CC} ${CFLAGS} -o ${AS2} ${AS2O}

${AS2O}: as2.h

${BINDIR}/${AS1}: ${AS1}
	strip ${AS1}
	cp ${AS1} ${BINDIR}

inst${AS1}: ${BINDIR}/${AS1}

${LIBDIR}/${AS2}: ${AS2}
	strip ${AS2}
	cp ${AS2} ${LIBDIR}

inst${AS2}: ${LIBDIR}/${AS2}

install: inst${AS1} inst${AS2}
	@echo The assembler is installed


clean:
	rm -f ${AS1} ${AS2}
	rm -f *.o make.out

print:
	@pr -r README makefile *.h *.c
