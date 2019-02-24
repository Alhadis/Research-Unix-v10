PLIB = /usr/lib/prefer
PBIN = /usr/bin
PHOME = /usr/lib/prefer

CFLAGS = -O
LDFLAGS = -s

LFLAGS =
LINT = cyntax

FRC =

BINSTUFF	= penter pconvert
LIBSTUFF	= pdoc pbib ptemplate ignore mypubenter


all:	penter pconvert

penter: mypubenter penter.sh ptemplate
	cp penter.sh penter
	chmod +x penter

mypubenter: mypubenter.o errexit.o
	$(CC) $(CFLAGS) mypubenter.o errexit.o $(LDFLAGS) -o mypubenter

pconvert: pconvert.sh pdoc pbib
	cp pconvert.sh pconvert
	chmod +x pconvert

pdoc:	pdoc.sh
	cp pdoc.sh pdoc
	chmod +x pdoc

pbib:	pbib.sh
	cp pbib.sh pbib
	chmod +x pbib


lint:	lintpubenter

lintpubenter:
	@$(LINT) $(LFLAGS) -DPREFER mypubenter.c


install: all
	cp $(BINSTUFF) $(PBIN)
	cp $(LIBSTUFF) $(PLIB)

clean:
	-rm -f *.o

clobber: clean
	-rm -f pconvert penter mypubenter pdoc pbib


print:
	prican makefile *.c *.sh ptemplate ignore



mypubenter.o:
	$(CC) $(CFLAGS) -c -DPREFER mypubenter.c

errexit.o:
	$(CC) $(CFLAGS) -c errexit.c
