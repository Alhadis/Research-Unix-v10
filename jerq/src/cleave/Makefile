install: cleave
	cp cleave /usr/jerq/bin
	chmod 775 /usr/jerq/bin/cleave
	strip /usr/jerq/bin/cleave

cleave:	cleave.c
	cc -g -o cleave `pwd`/cleave.c

clean:
	rm -f *junk* dead.letter jim.recover cleave

