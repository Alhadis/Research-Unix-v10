FLEX=	-DFLEXNAMES

LFLAGS=	-p
CFLAGS=	-O -I. -DNODBG $(FLEX)

OFILES= cgram.o xdefs.o scan.o pftn.o trees.o optim.o local.o reader.o\
	local2.o debug.o match.o allo.o comm1.o table.o cost.o cgen.o\
	optdir.o

comp:	$(OFILES)
		$(CC) $(CFLAGS) -o comp $(OFILES)

sty:		sty.y manifest.h macdefs.h mfile2.h dope.h
		-@echo "Expect 3 reduce/reduce conflicts"
		$(YACC) sty.y
		$(CC) $(CFLAGS) -I. -o sty y.tab.c
		rm y.tab.c
$(OFILES):	manifest.h macdefs.h
trees.o optim.o pftn.o local.o scan.o xdefs.o cgram.o:	mfile1.h sdb.h 
table.o cost.o cgen.o reader.o local2.o allo.o:	mfile2.h 
cgram.c:	cgram.y
		-@echo "Expect 6 shift/reduce conflicts"
		$(YACC) $(YFLAGS) cgram.y
		mv y.tab.c cgram.c
comm1.o:	mfile1.h sdb.h common dope.h 
cgram.o:	cgram.c
		$(CC) $(CFLAGS) -DYYMAXDEPTH=600 -c cgram.c
table.c:	sty stin
		-@echo "Expect 2 \"... may be covered by ...\" messages"
		./sty <stin >table.c
debug.o optdir.o:	mfile1.h storclass.h
match.o:	mfile2.h sdb.h

install:	comp
		cp comp /usr/jerq/lib/m32/comp
		strip /usr/jerq/lib/m32/comp

clean:
	-rm -f $(OFILES) comp table.c sty cgram.c
