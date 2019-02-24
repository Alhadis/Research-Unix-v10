#
#	Makefile for Floating Point Preprocessor
#	For machines without floating point hardware
#	SEE FIS SELECT OPTION BELOW
#
#	Fpp is called with the -F option to 11cc, using ./fpp
#	Programs which call this must have the F library [11libF.a]
#	linked in with them {included here}
#
#	The Standard C library should be remade with the -F option, too,
#	so as not to use floating point instructions, and called
#	11libFc.a
#
LIBDIR = /lib

FPP = fpp
FLIB = 11libF.a

CC   = cc
AR   = ar
CC11 = 11cc
AS11 = 11as
RANLIB11 = 11ranlib

CFLAGS = -O

#	If you have FIS {floating instruction set}, set RHFLIB = rhflibFIS
#	If you do not, set RHFLIB = rhflibNOFIS
#
RHFLIB = rhflibNOFIS

# 
# LIBFILES = rhmath.o rhprintf.o rhpfloat.o rhftoa1.o mmatof.o \
#		rhflib1.o rhflibu.o rhflib.o format.o ilongops.o

# god: these are (I believe) the minimum basic requirements for
# using fpp.  The other files have junk like math routines
# and use an old printf.  The math routines are
# probably better than the standard libc ones, but it is better
# to use the standard atof and print.c
#
LIBFILES = rhpfloat.o rhflib1.o rhflibu.o rhflib.o ilongops.o

#	Default for making libraries is the PDP11 C compiler and Assembler
%.o:	%.c
	${CC11} -O -tf -B./ -F -c $%.c
%.o:	%.s
	${AS11} -o $%.o $%.s

all:	${FPP} ${FLIB}				# must make fpp first
	@echo \`all\' is up to date: $?

#	Of course, the floating point preprocessor is VAX executable
#
${FPP}:	fpp.o rhccomp.o rhsymtab.o
	${CC} ${CFLAGS} fpp.o rhccomp.o rhsymtab.o -o ${FPP}

fpp.o:	fpp.c
	${CC} ${CFLAGS} -c fpp.c

rhccomp.o: rhccomp.c
	${CC} ${CFLAGS} -c rhccomp.c

rhsymtab.o: rhsymtab.c
	${CC} ${CFLAGS} -c rhsymtab.c

${LIBDIR}/${FPP}: ${FPP}
	cp ${FPP} ${LIBDIR}
	strip ${LIBDIR}/${FPP}

inst${FPP}: ${LIBDIR}/${FPP}

rhflib.o: ${RHFLIB}.s
	cp ${RHFLIB}.s rhflib.s
	${AS11} -o rhflib.o rhflib.s

${FLIB}: ${LIBFILES}

${LIBDIR}/${FLIB}: ${LIBFILES}
	chmod 644 $?
	${AR} ru ${LIBDIR}/${FLIB} $?
	${RANLIB11} ${LIBDIR}/${FLIB}
	chmod 644 ${LIBDIR}/${FLIB}

inst${FLIB}: ${LIBDIR}/${FLIB}

install: inst${FPP} inst${FLIB}
	@echo ${FPP} and ${FLIB} are installed

clean:
	rm -f *.o make.out
	rm -f *.o make.out ${FPP} ${FLIB}

print:
	@-pr -r README NOTE makefile *.c *.s
