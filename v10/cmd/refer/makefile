CFLAGS=-O -g
all: mkey inv hunt deliv refpart refer
install: refer-install lib-install
clean:
	rm -f *.o mkey inv hunt deliv refpart refer *junk*
mkey: mkey1.o mkey2.o mkey3.o deliv2.o
	cc -s mkey?.o deliv2.o -o mkey
inv: inv1.o inv2.o inv3.o inv5.o inv6.o deliv2.o
	cc -s inv?.o deliv2.o -o inv
hunt: hunt1.o hunt2.o hunt3.o hunt5.o hunt6.o hunt7.o glue5.o
hunt: refer3.o hunt9.o shell.o deliv2.o hunt8.o glue4.o tick.o iread.o
	cc -s hunt?.o refer3.o glue5.o glue4.o shell.o deliv2.o tick.o iread.o -o hunt
deliv: deliv1.o deliv2.o
	cc -s deliv?.o  -o deliv
refpart: refer0.o refer1.o refer2.o refer3.o refer4.o refer5.o
refpart: refer6.o refer7.o refer8.o deliv2.o glue4.o
refpart: glue5.o mkey3.o iread.o
	cc -s refer?.o deliv2.o glue4.o glue5.o mkey3.o iread.o -o refpart
refer0.o: refer..c
refer1.o: refer..c
refer2.o: refer..c
refer3.o: refer..c
refer4.o: refer..c
refer6.o: refer..c
refer: glue1.o refer1.o refer2.o refer4.o refer5.o refer6.o mkey3.o
refer: refer7.o refer8.o hunt2.o hunt3.o deliv2.o hunt5.o hunt6.o hunt8.o
refer: glue3.o hunt7.o hunt9.o glue2.o glue4.o glue5.o refer0.o shell.o
	cc glue?.o refer[01245678].o hunt[2356789].o mkey3.o shell.o deliv2.o -o refer
drefer: glue1.o refer1.o refer2.o refer4.o refer5.o refer6.o mkey3.o
drefer: refer7.o refer8.o hunt2.o hunt3.o deliv2.o hunt5.o hunt6.o hunt8.o
drefer: glue3.o hunt7.o hunt9.o glue2.o glue4.o glue5.o refer0.o shell.o
	cc  -i glue?.o refer[01245678].o hunt[2356789].o mkey3.o shell.o deliv2.o -o refer
refer-test:
	/usr/bin/refer test >junk1a
	refer test >junk1b
	if cmp -s junk1a junk1b; then echo ok; else echo bad; fi
	/usr/bin/refer -e test >junk1a
	refer -e test >junk1b
	if cmp -s junk1a junk1b; then echo ok; else echo bad; fi
	/usr/bin/refer -s test >junk1a
	refer -s test >junk1b
	if cmp -s junk1a junk1b; then echo ok; else echo bad; fi
	/usr/bin/refer -s -l test >junk1a
	refer -s -l test >junk1b
	if cmp -s junk1a junk1b; then echo ok; else echo bad; fi
	rm junk1a junk1b
refer-install:
	strip refer
	cp /usr/bin/refer orefer
	cp refer /usr/bin/refer
whatabout: what1.o what2.o what3.o what4.o shell.o mkey3.o
	cc what?.o shell.o mkey3.o  -o whatabout
lib-install:
#	strip mkey hunt inv
	cp mkey /usr/lib/refer/mkey
	cp hunt /usr/lib/refer/hunt
	cp inv /usr/lib/refer/inv

bundle:
	bundle makefile *.c update/makefile update/*.c pubindex sample test lookbib
