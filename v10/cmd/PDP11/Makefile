BINDIR = /usr/bin
LIBDIR = /usr/lib

AR = ar
A_DOT_OUT = 11a.out

CC = cc
CFLAGS = -O -DA_DOT_OUT=\"${A_DOT_OUT}\"

PATHFLAGS = BINDIR=${BINDIR} LIBDIR=${LIBDIR} \
		AR=${AR} A_DOT_OUT=${A_DOT_OUT}
C11FLAGS  = C0=11c0 C1=11c1 C2=11c2
AS11FLAGS = AS1=11as AS2=11as2

all: 11as 11c \
      11crt0.o 11cc 11ld 11nm 11ranlib \
      11reloc 11size 11strip
	@echo \`all\' is up to date: $?

init:		# Assumes that c compiler tape already installed
	mv c 11c
	mv as 11as
	mv ld.c 11ld.c
	mv ranlib.c 11ranlib.c
	mv nm.c 11nm.c


rmfpp:
	grep -v fpp Makefile > nMakefile
	rm -r fpp; mv nMakefile Makefile

.force1:
	@touch .force1

11as:	.force1
	cd 11as; make all ${PATHFLAGS} ${AS11FLAGS}
	@touch .force1

11c:	.force1
	cd 11c; make all ${PATHFLAGS} ${C11FLAGS}
	@touch .force1

fpp:	inst11as inst11c inst11cc		# must be there before fpp
fpp:	.force1
	cd fpp; make all ${PATHFLAGS}
	@touch .force1  ; echo weird way to tag fpp > /dev/null

11crt0.o: 11crt0.s
	11as -o 11crt0.o 11crt0.s

11cc.c:	11cc.h

11cc.h: 11cc.h.skel
	sed -e '/BINPATH/s;~;${BINDIR};' \
	    -e '/LIBPATH/s;~;${LIBDIR};'  < 11cc.h.skel > 11cc.h

11nm.c 11ranlib.c 11size.c 11strip.c: a.out.h	# 11ld and 11reloc should, too

11cc:	  11cc.c
11ld:	  11ld.c
	${CC} -o 11ld ${CFLAGS} -DLIBPATH=\"${LIBDIR}\" \
				-DLIBPREFIX=\"/11lib\" 11ld.c
11nm:	  11nm.c
11ranlib: 11ranlib.c
11reloc:  11reloc.c
11size:   11size.c
11strip:  11strip.c


11ranlib: 
	${CC} -o $@ ${CFLAGS} -DAR=\"${AR}\" $@.c

11cc 11nm 11reloc 11size 11strip:
	${CC} ${CFLAGS} -o $@ $@.c

${LIBDIR}/11crt0.o:	11crt0.o
	cp 11crt0.o ${LIBDIR}

.force2:
	@touch .force2

inst11as: .force2
	cd 11as; make install ${PATHFLAGS} ${AS11FLAGS}
	@touch .force2

inst11c: .force2
	cd 11c; make install ${PATHFLAGS} ${C11FLAGS}
	@touch .force2

instfpp: inst11as inst11c inst11cc inst11ranlib	# must be there before fpp
instfpp: .force2
	cd fpp; make install ${PATHFLAGS}
	@touch .force2  ;  echo weird way to tag fpp > /dev/null

inst11crt0.o:	11crt0.o
	cp 11crt0.o ${LIBDIR}
inst11cc:	11cc
	strip 11cc
	cp 11cc ${BINDIR}
inst11size:	11size
	strip 11size
	cp 11size ${BINDIR}
inst11ld:	11ld
	strip 11ld
	cp 11ld ${BINDIR}
inst11strip:	11strip
	strip 11strip
	cp 11strip ${BINDIR}
inst11nm:	11nm
	strip 11nm
	cp 11nm ${BINDIR}
inst11ranlib:	11ranlib
	strip 11ranlib
	cp 11ranlib ${BINDIR}
inst11reloc:	11reloc
	strip 11reloc
	cp 11reloc ${BINDIR}

install: inst11as inst11c \
	 inst11crt0.o inst11cc inst11size inst11ld \
	 inst11strip inst11ranlib inst11reloc \
	 inst11nm
	 @echo The PDP11 package is installed

clean:
	cd 11as; make clean ${AS11FLAGS}
	cd 11c; make clean ${C11FLAGS}
	cd fpp; make clean
	-rm -f 11cc.h *.o make.out 11cc 11size 11ld 11strip 11nm 11ranlib \
		11reloc .force1 .force2

print:
	@cd 11as; make print
	@cd 11c; make print
	@cd fpp; make print
	@pr Makefile *.h *.s *.c *.sh
