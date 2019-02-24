TESTDIR = .
FRC =
CFLAGS = -DASSTRINGS -DSTABDOT -DLCOMM -DBUFSTDERR -DFLEXNAMES -O
M = ../mip

all: fort # testcomp

comp: rodata.o cgram.o xdefs.o scan.o pftn.o trees.o optim.o code.o local.o \
	reader.o local2.o order.o match.o allo.o comm1.o table.o BITOOR.o
	$(CC) $(LDFLAGS) -z rodata.o cgram.o xdefs.o scan.o pftn.o trees.o \
		optim.o code.o local.o reader.o local2.o order.o match.o \
		allo.o comm1.o table.o BITOOR.o -o $(TESTDIR)/comp

trees.o: $M/manifest macdefs $M/mfile1 $M/trees.c
	$(CC) -c $(CFLAGS) -I$M -I.  $M/trees.c

optim.o: $M/manifest macdefs $M/mfile1 $M/optim.c
	$(CC) -c $(CFLAGS) -I$M -I. $M/optim.c

pftn.o: $M/manifest macdefs $M/mfile1 $M/pftn.c
	$(CC) -c $(CFLAGS) -I$M -I. $M/pftn.c

code.o: $M/manifest macdefs $M/mfile1
	$(CC) -c $(CFLAGS) -I$M -I. code.c

local.o: $M/manifest macdefs $M/mfile1
	$(CC) -c $(CFLAGS) -I$M -I. local.c

scan.o: $M/manifest macdefs $M/mfile1 $M/scan.c
	$(CC) -c $(CFLAGS) -I$M -I. $M/scan.c

xdefs.o: $M/manifest $M/mfile1 macdefs $M/xdefs.c
	$(CC) -c $(CFLAGS) -I$M -I. $M/xdefs.c

cgram.o: $M/manifest $M/mfile1 macdefs cgram.c
	$(CC) -c $(CFLAGS) -I$M -I. cgram.c

rodata.o: rodata.c
	$(CC) $(CFLAGS) -S rodata.c
	./:rofix rodata.s
	$(AS) -o rodata.o rodata.s
	rm -f rodata.s

rodata.c cgram.c: $M/cgram.y
	$(YACC) $M/cgram.y
	rm -f rodata.c
	./:yyfix yyexca yyact yypact yypgo yyr1 yyr2 yychk yydef
	mv y.tab.c cgram.c

comm1.o: $M/manifest $M/mfile1 $M/common macdefs $M/comm1.c
	$(CC) -c $(CFLAGS) -I$M -I. $M/comm1.c

table.o: $M/manifest $M/mfile2 mac2defs macdefs table.c
	$(CC) $(CFLAGS) -S -I$M -I. table.c
	./:rofix table.s
	$(AS) -o table.o table.s
	rm -f table.s

reader.o: $M/manifest $M/mfile2 mac2defs macdefs $M/reader.c
	$(CC) -c $(CFLAGS) -I$M -I. $M/reader.c

local2.o: $M/manifest $M/mfile2 mac2defs macdefs
	$(CC) -c $(CFLAGS) -I$M -I. local2.c

order.o: $M/manifest $M/mfile2 mac2defs macdefs
	$(CC) -c $(CFLAGS) -I$M -I. order.c

match.o: $M/manifest $M/mfile2 mac2defs macdefs $M/match.c
	$(CC) -c $(CFLAGS) -I$M -I. $M/match.c

allo.o: $M/manifest $M/mfile2 mac2defs macdefs $M/allo.c
	$(CC) -c $(CFLAGS) -I$M -I. $M/allo.c

shrink:
	rm *.o comp

clean:
	rm -f *.o comp fort cgram.c rodata.c

lintall:
	lint -hpv -I. -I$M  $M/cgram.c $M/xdefs.c $M/scan.c $M/pftn.c \
		$M/trees.c $M/optim.c code.c local.c $M/reader \
		local2.c order.c $M/match.c $M/allo.c $M/comm1.c table.c

fort: fort.o freader.o fallo.o fmatch.o ftable.o forder.o flocal2.o \
	fcomm2.o BITOOR.o
	$(CC) -z $(CFLAGS) fort.o freader.o fallo.o fmatch.o ftable.o \
		forder.o flocal2.o fcomm2.o BITOOR.o -o $(TESTDIR)/fort

fort.o: fort.h $M/fort.c
	$(CC) -c $(CFLAGS) -I$M -I. $M/fort.c

freader.o: $M/manifest $M/mfile2 mac2defs macdefs $M/reader.c
	$(CC) -c $(CFLAGS) -I$M -I. $M/freader.c

fallo.o: $M/fallo.c
	$(CC) -c $(CFLAGS) -I$M -I. $M/fallo.c

fmatch.o: $M/fmatch.c
	$(CC) -c $(CFLAGS) -I$M -I. $M/fmatch.c

ftable.o: UCtable.c
	$(CC) -c -R $(CFLAGS) -DFORT -I$M -I. UCtable.c
	mv UCtable.o ftable.o

forder.o: $M/forder.c
	$(CC) -c $(CFLAGS) -I$M -I. $M/forder.c

flocal2.o: UClocal2.c
	$(CC) -c $(CFLAGS) -DFORT -I$M -I. UClocal2.c
	mv UClocal2.o flocal2.o

fcomm2.o: $M/common
	$(CC) -c $(CFLAGS) -I$M -I. $M/fcomm2.c

fort.o freader.o fallo.o fmatch.o ftable.o forder.o flocal2.o fcomm2.o: \
	$M/mfile2 $M/manifest macdefs mac2defs

install:	fort
	strip fort
#	mv comp ${DESTDIR}/lib/ccom
	mv fort ${DESTDIR}/lib/f1

testcomp:
	rm -fr testdir
	mkdir testdir
	cp /lib/cpp /lib/c2 testdir
	make clean
	make comp
	cp comp testdir/ccom
	make clean
	make CFLAGS="${CFLAGS} -Btestdir/" comp
	cp comp testdir/ccom
	make clean
	make CFLAGS="${CFLAGS} -Btestdir/" comp
	cmp comp testdir/ccom
	rm -fr testdir
