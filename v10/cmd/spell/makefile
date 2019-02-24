
HFILES=code.h

all:	sprog brspell amspell

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
	$(CC) -o pcode pcode.o

sprog:	sprog.o
	$(CC) -o sprog sprog.o

%.o:	%.c $(HFILES)
	$(CC) $(CFLAGS) -c $*.c

clean:
	rm -f *.[v2o] amspell brspell pcode sprog
