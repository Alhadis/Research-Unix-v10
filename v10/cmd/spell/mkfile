BUILTINS=
NPROC=1

HFILES=code.h

all:V:	sprog brspell amspell

bin:	spell.sh
	cp spell.sh /usr/bin/spell

lib:	sprog amspell brspell
	cp sprog /usr/lib/spell/sprog
	strip /usr/lib/spell/sprog
	cp amspell /usr/lib/spell/amspell
	cp brspell /usr/lib/spell/brspell
	rm -f sprog amspell brspell
	
brspell: pcode  list british local stop
	./pcode list british local stop >brspell

amspell: pcode  list american local stop
	./pcode list american local stop >amspell

pcode:	pcode.o
	lcc -o pcode pcode.o

sprog:	sprog.o
	lcc -o sprog sprog.o

%.o:	%.c $HFILES
	lcc -A -c $stem.c

install: sprog amspell brspell
	strip sprog
	cp sprog amspell brspell /usr/lib/spell
clean:V:
	rm -f *.[v2o] amspell brspell pcode sprog
