#	text Development Support Library (DSL) macros make file
#	SCCS: @(#)macros.mk	1.28

INSMAC = /usr/lib/macros
INSTMAC = /usr/lib/tmac
TMACFILES = tmac.an tmac.an6 tmac.m tmac.org tmac.ptx tmac.osd\
	tmac.s tmac.sdisp tmac.scover tmac.skeep tmac.v
OUTFILES = mmn mmt vmca osdd an an6 ptx

all:	ntar $(OUTFILES)

install:	all tmac
	cp ${OUTFILES} ${INSMAC};
	cd ${INSMAC}; chown bin ${OUTFILES};\
	chgrp bin ${OUTFILES}; chmod 664 ${OUTFILES}

ntar:	ntar.c
	${CC} -o ntar ntar.c

mmn:	ntar
	macrunch -d /usr mmn

mmt:	ntar
	macrunch -d /usr mmt

vmca:	ntar
	macrunch -d /usr vmca

org:	ntar
	macrunch -d /usr org

osdd:	ntar
	macrunch -d /usr osdd

an:	ntar
	macrunch -d /usr an

an6:	ntar
	macrunch -d /usr an6

ptx:	ntar
	macrunch -d / ptx

tmac:
	cp ${TMACFILES} ${INSTMAC}
	cd ${INSTMAC}; chmod 644 ${TMACFILES};\
	chown bin ${TMACFILES}; chgrp bin ${TMACFILES}

clean:
	rm -f $(OUTFILES) ntar
