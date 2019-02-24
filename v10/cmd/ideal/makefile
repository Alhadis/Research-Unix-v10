CFLAGS = -O -DDUMPVARS

SOURCES =\
	ideal.h\
	ideal.c\
	util.c\
	memut.c\
	bldds.c\
	simul.c\
	exprn.c\
	action.c\
	piece.c\
	opaque.c\
	inter.c\
	opqpoly.c\
	idlex.l\
	idyac.y
OBJECTS =\
	y.tab.o\
	lex.yy.o\
	ideal.o\
	util.o\
	memut.o\
	bldds.o\
	simul.o\
	exprn.o\
	action.o\
	piece.o\
	opaque.o\
	opqpoly.o\
	inter.o
ADMIXTURE =\
	y.tab.c\
	lex.yy.c\
	ideal.c\
	util.c\
	memut.c\
	bldds.c\
	simul.c\
	exprn.c\
	action.c\
	piece.c\
	opaque.c\
	opqpoly.c\
	inter.c

a.out:	$(OBJECTS)
	cc $(OBJECTS) -ll -lm

$(OBJECTS):	ideal.h

ideal.h:	stdas.h

lex.yy.c:	idlex.l
	lex idlex.l

y.tab.c:	idyac.y
	yacc -d idyac.y

list:
	pr $(SOURCES)

lint:
	lint $(ADMIXTURE) -lm

backup:
	cp a.out makefile $(SOURCES) precious

working:
	cp a.out makefile $(SOURCES) semiprec

longlist:
	ls -l $(SOURCES)

wc:
	wc $(SOURCES)

diff:
	for i in $(SOURCES);\
	do\
			cmp $$i precious/$$i || diff $$i precious/$$i;\
	done

clean:
	rm $(OBJECTS) y.tab.c lex.yy.c

fgrep:
	fgrep $(WORD) $(SOURCES)

cpio:
	cpio -o <subdirectories >subdirs.cpio

export:	cpio
	uucp README makefile\
	$(SOURCES) stdas.h\
	RAW_FORMAT ideal.cmd\
	manpage\
	subdirectories\
	subdirs.cpio\
	$(WHO)

install:	a.out
	cp ideal.cmd /usr/bin/ideal
	cp a.out /usr/lib/ideal/ideal
	strip /usr/lib/ideal/ideal
	rm -rf /usr/lib/ideal/lib
	mkdir /usr/lib/ideal/lib
	cp lib/* /usr/lib/ideal/lib
	cd idfilt; make install

localexport:
	cp README makefile $(SOURCES) stdas.h \
	RAW_FORMAT ideal.cmd manpage \
	/n/bowell/src/cmd/ideal
	mkdir /n/bowell/src/cmd/ideal/lib
	cp lib/* /n/bowell/src/cmd/ideal/lib
	mkdir /n/bowell/src/cmd/ideal/idfilt
	cp idfilt/* /n/bowell/src/cmd/ideal/idfilt
