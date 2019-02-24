UCB=/lusr/ucb
CFLAGS=	-O -I$(UCB)/include -DCM_N -DCM_GT -DCM_B -DCM_D

termcap.a: termcap.o tgoto.o tputs.o
	ar cr termcap.a termcap.o tgoto.o tputs.o

clean:
	-rm -f *.o
	-rm -f termcap.a

install: termcap.a
	cp termcap.a ${DESTDIR}$(UCB)/lib/libtermcap.a
	-rm -f ${DESTDIR}$(UCB)/lib/libtermlib.a
	ln ${DESTDIR}$(UCB)/lib/libtermcap.a ${DESTDIR}$(UCB)/lib/libtermlib.a
	ranlib ${DESTDIR}$(UCB)/lib/libtermcap.a

VGRIND=	csh $(UCB)/ucb/vgrind
vgrind:
	cp /dev/null index
	${VGRIND} -h "Termcap library" termcap.c tputs.c tgoto.c
	${VGRIND} -h "Termcap library" -x index
