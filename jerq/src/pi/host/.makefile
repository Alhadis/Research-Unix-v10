OFILES=gram.o batch.o mccsymtab.o snetmaster.o m68kcore.o snetproc.o wd.o kerncore.o kernmaster.o kernproc.o hostcore.o master.o hostmaster.o termmaster.o asm.o audit.o bpts.o coffsymtab.o core.o dtype.o ed8symtab.o expr.o format.o frame.o lib.o main.o memory.o parse.o phrase.o process.o srctext.o symbol.o symtab.o termcore.o termproc.o sigmask.o srcdir.o hostproc.o journal.o m68kasm.o mac32asm.o vaxasm.o snetcore.o werkcore.o werkmaster.o werkproc.o vtbl.o
PRIFILES=dtype.pri asm.pri bpts.pri core.pri format.pri expr.pri frame.pri master.pri memory.pri phrase.pri process.pri srctext.pri symtab.pri journal.pri
HFILES=asm.h bpts.h coff.h core.h dtype.h ed8.h expr.h format.h frame.h gram.h hostcore.h lib.h master.h memory.h mip.h op.h parse.h phrase.h process.h sigmask.h srcdir.h srctext.h symbol.h symtab.h termcore.h univ.h y.tab.h journal.h vaxasm.h mac32asm.h m68kasm.h 
PUBFILES=dtype.pub asm.pub bpts.pub core.pub expr.pub format.pub frame.pub master.pub memory.pub phrase.pub process.pub srctext.pub symtab.pub journal.pub

INC=-I/usr/jerq/include -I/usr/include
CLEAVE=/usr/jerq/bin/cleave
LIB=/usr/jerq/lib/libpads.a -ljobs -ldk

pi:	$(PRIFILES) $(PUBFILES) $(HFILES) $(OFILES)
	rm -f pi 3pi
	cc -o pi $(OFILES) $(LIB)
	strip -s pi
	ln pi 3pi

%.pri:	%.h
	$(CLEAVE) $(INC)  $(CFLAGS) $*
	touch $*.pri

%.o:	%.c
	CC $(INC) -cg +e0 `pwd`/$*.c

vtbl.o:	vtbl.c
	CC $(INC) -cg +e1  vtbl.c

gram.c:	gram.y
	yacc -d gram.y
	mv y.tab.c gram.c

clean:
	rm -f *junk* core jim.*

install: pi
	rm -f /usr/jerq/bin/3pi /usr/jerq/bin/pi /usr/jerq/bin/pi.tables
	cp pi /usr/jerq/bin
	chmod 775 /usr/jerq/bin/pi
	ln /usr/jerq/bin/pi /usr/jerq/bin/3pi
	cp /usr/jerq/bin/pi /usr/jerq/bin/pi.tables
	strip /usr/jerq/bin/pi

diffsrc:
	/usr/tac/bin/diffsrc /n/seki/usr/tac/pi/host /usr/jerq/src/pi/host
