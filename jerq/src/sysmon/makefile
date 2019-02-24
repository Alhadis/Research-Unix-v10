CFLAGS=-g
MCC=/usr/blit/bin/mcc -DM68000
MOPT=/usr/blit/bin/mopt
MSTRIP=/usr/blit/bin/mstrip
DCC=/usr/jerq/bin/3cc
CC=cc

MONOBJ=sysmon.o calldaemon.o sname.o sampleload.o utmp.o icon.o from.o nlist.o scanmail.o
MONSRC=sysmon.c calldaemon.c sname.c sampleload.c utmp.c icon.c from.c nlist.c scanmail.c
DAEOBJ=sampleload.o sname.o daemon.o ntdkmgr.o generate.o
DISPDEP=jerqmon.c

all: sysmond sysmon sysmon.m
install: install.bin install.mbin install.lib
ship: ship.bin ship.mbin ship.lib

#
#  create the daemon
#
sysmond: ${DAEOBJ}
	${CC} ${CFLAGS} ${DAEOBJ} -o sysmond -ldk

sampleload.o: defs.h load.h
sname.o: defs.h
calldaemon.o: defs.h
daemon.o: defs.h
ntdkmgr.o:
generate.o: defs.h load.h msg.h
scanmail.o: defs.h scanmail.h
sysmon.o: load.h defs.h msg.h

#
#  create the communications window routine
#

#
#  create the host piece (same for 68000 or MAC32)
#
sysmon: ${MONOBJ}
	${CC} ${CFLAGS} -o sysmon ${MONOBJ} -ldk

cyn.bin: ${MONSRC}
	cyntax ${MONSRC}

#
#  create the 68000 version
#
#
# sysmon.m.68: ${DISPDEP}
# 	${MCC} ${CFLAGS} -o sysmon.m.68 jerqmon.c

#
#  create the MAC 32 version
#
sysmon.m: ${DISPDEP}
	-${DCC} ${CFLAGS} -o sysmon.m jerqmon.c -ljj

#
#  install things
#
install.bin:	sysmon
	-rm -f /usr/jerq/bin/sysmon
	cp sysmon /usr/jerq/bin
	/etc/chown bin,sys /usr/jerq/bin/sysmon
	chmod 2755 /usr/jerq/bin/sysmon
	/bin/rm -f /usr/jerq/bin/vismon
	ln /usr/jerq/bin/sysmon /usr/jerq/bin/vismon

ship.bin:
	ship /usr/jerq/bin/sysmon /usr/jerq/bin/vismon

install.mbin: sysmon.m 
	cp sysmon.m /usr/jerq/mbin/sysmon.m

ship.mbin:
	ship /usr/jerq/mbin/sysmon.m

install.lib:
	-mv /usr/jerq/lib/sysmond /usr/jerq/lib/Osysmond
	cp sysmond /usr/jerq/lib
	/etc/chown root,sys /usr/jerq/lib/sysmond
	chmod 4755 /usr/jerq/lib/sysmond

ship.lib:
	ship /usr/jerq/lib/sysmond

#
#  misc
#
clean:
	rm -f core *.o sysmon *.m.68 *.m.32 *.out sysmond vismon 
