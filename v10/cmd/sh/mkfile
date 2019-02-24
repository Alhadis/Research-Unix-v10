TESTDIR = .
FRC =
ROOT=
INSDIR =
CFLAGS = -g #-gd2
LDFLAGS = -g
NPROC = 2

OFILES = shsetbrk.o blok.o stak.o cmd.o fault.o main.o word.o string.o\
	name.o args.o xec.o service.o error.o io.o print.o macro.o expand.o\
	sctype.o msg.o defs.o pathserv.o func.o spname.o
OOFILES = shsetbrk.O blok.O stak.O cmd.O fault.O main.O word.O string.O\
	name.O args.O xec.O service.O error.O io.O print.O macro.O expand.O\
	sctype.O msg.O defs.O pathserv.O func.O spname.O

sh: $OFILES 
	$CC $LDFLAGS $OFILES -o $TESTDIR/sh

$OFILES $OOFILES:	defs.h $FRC

sctype.o:	sctype.h
	CC="$CC" AS="$AS" CFLAGS="$CFLAGS" sh ./:fix sctype

msg.o:	msg.c
	CC="$CC" AS="$AS" CFLAGS="$CFLAGS" sh ./:fix msg

blok.o fault.o main.o stak.o:	brkincr.h
blok.O fault.O main.O stak.O:	brkincr.h
cwd.o:	mac.h
cwd.O:	mac.h
cmd.o macro.o main.o msg.o name.o word.o xec.o:	sym.h
cmd.O macro.O main.O msg.O name.O word.O xec.O:	sym.h
main.o:	timeout.h
main.O:	timeout.h

install:  all
	mv /bin/sh /bin/osh;	cp sh /bin/sh;	strip /bin/sh
	/etc/chown bin,bin /bin/sh
	chmod o-w,g+w /bin/sh

clean:
	rm -f *.o *.O sh

clobber:  clean
	set -e
	rm -f $TESTDIR/sh
	rm -f $ROOT/bin/OLDrsh

cyntax:	$OOFILES
	cyntax $OOFILES > temp && touch cyntax

%.O:	%.c
	cyntax -c $CFLAGS $stem.c
xec.O:	xec.c
	cyntax -c -Vexecute:3 $CFLAGS xec.c

FRC:

pp:
	pp -tsh makefile *.h *.c | dcan
