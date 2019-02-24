#	Makefile	1.11	83/08/19
#
# Sail - Wooden Ships And Iron Men
#
CC=	/bin/cc
CFLAGS=	-O
I=	/usr/include
S=	/usr/include/sys
CTAGS=	ctags -w

DFILES=	driver1.c driver2.c driver3.c grap.c
DOBJS=	driver1.o driver2.o driver3.o grap.o

PFILES=	version.c player1.c player2.c
POBJS=	version.o player1.o player2.o

LFILES=	sail.log.c
LOBJS=	sail.log.o

IFILES=	info.c
IOBJS=	info.o

COMMONFILES= assorted.c boarders.c game.c parties.c screen.c main.c globals.c
COMMONOBJS= assorted.o boarders.o game.o parties.o screen.o main.o globals.o

HDRS=	externs.h player.h machdep.h

OTHERFILES= Makefile sail.6

CTAGFILES= ${DFILES} ${PFILES} ${LFILES} ${IFILES} ${COMMONFILES}

JUNKFILES= sail driver sail.log info junk arch tags core

PLIBS= 	-lcurses -ltermlib -ljobs
DLIBS=	-ljobs
LLIBS=
SAILMASTER= daemon
SAIL=	sail
DRIVER=	lib/saildriver
SAIL.LOG= sail.log
SHIPSLOG= lib/saillog
DESTDIR=
TARGET=	${DESTDIR}/usr/games

..c.o:; ${CC} ${CFLAGS} -c $<

all: sail driver sail.log info 

sail: ${COMMONOBJS} ${POBJS}
	${CC} -O ${COMMONOBJS} ${POBJS} ${PLIBS} -o sail

driver: ${COMMONOBJS} ${DOBJS}
	${CC} -O ${COMMONOBJS} ${DOBJS} ${DLIBS} -o driver

sail.log: ${LOBJS} globals.o
	${CC} -O ${LOBJS} globals.o -o sail.log

info: ${IOBJS} globals.o
	${CC} -O ${IOBJS} globals.o -o info


tags: ${CTAGFILES}
	${CTAGS} ${CTAGFILES}

externs.h:	$(I)/stdio.h $(I)/signal.h $(I)/ctype.h machdep.h
		touch externs.h

player.h:	$(I)/curses.h externs.h
		touch player.h

assorted.o:	assorted.c externs.h
boarders.o:	boarders.c externs.h
driver1.o:	driver1.c externs.h $(S)/types.h
driver2.o:	driver2.c externs.h
driver3.o:	driver3.c externs.h
game.o:		game.c externs.h
globals.o:	globals.c externs.h
grap.o:		grap.c externs.h
info.o:		info.c externs.h
main.o:		main.c externs.h
parties.o:	parties.c externs.h
player1.o:	player1.c player.h $(S)/types.h $(I)/wait.h
player2.o:	player2.c player.h
sail.log.o:	sail.log.c externs.h $(I)/pwd.h
screen.o:	screen.c externs.h $(S)/types.h $(S)/stat.h

#${HDRS} ${CTAGFILES}:
#	sccs get $@;

arch:	sail.ar
	ar uv sail.ar ${PFILES} ${COMMONFILES} ${OTHERFILES} \
	${DFILES} ${LFILES} ${HDRS}

driveprofile: ${COMMONOBJS} ${DOBJS}
	${CC} -p ${COMMONOBJS} ${DOBJS} ${CFLAGS} ${DLIBS} -o DRIVE.pro

profile: ${COMMONOBJS} ${POBJS} ${DOBJS}
	${CC} -p ${COMMONOBJS} ${POBJS} ${CFLAGS} ${PLIBS} -o SAIL.pro
	${CC} -p ${COMMONOBJS} ${DOBJS} ${CFLAGS} ${DLIBS} -o DRIVE.pro

clean: 
	rm -f ${DOBJS} ${POBJS} ${LOBJS} ${IOBJS} ${COMMONOBJS} ${JUNKFILES}

install: sail driver sail.log ${TARGET}/${SHIPSLOG}
	strip sail
	/etc/chown ${SAILMASTER} sail
	chmod 4711 sail
	rm -f ${TARGET}/${SAIL}
	mv sail ${TARGET}/${SAIL}
	strip driver
	cp driver ${TARGET}/${DRIVER}
	chown ${SAILMASTER} ${TARGET}/${DRIVER}
	chmod 4711 ${TARGET}/${DRIVER}
	strip sail.log
	cp sail.log ${TARGET}/${SAIL.LOG}
	chmod 711 ${TARGET}/${SAIL.LOG}

${TARGET}/${SHIPSLOG}:
	cp /dev/null ${TARGET}/${SHIPSLOG}
	chown ${SAILMASTER} ${TARGET}/${SHIPSLOG}
	chmod 644 ${TARGET}/${SHIPSLOG}
