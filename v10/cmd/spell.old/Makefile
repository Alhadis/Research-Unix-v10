CFLAGS = -O

all: spellprog spellin hashmake hashcheck parsecode amspell brspell

spellprog: spellprog.o hash.o hashlook.o huff.o typeprint.o
	cc $(CFLAGS) spellprog.o hash.o hashlook.o huff.o \
		typeprint.o -o spellprog

spellin: spellin.o huff.o hash.h
	cc $(CFLAGS) spellin.o huff.o -o spellin

hashcheck: hashcheck.o hash.o huff.o hash.h
	cc $(CFLAGS) hashcheck.o hash.o huff.o -o hashcheck

hashmake: hashmake.o hash.o 
	cc $(CFLAGS) hashmake.o hash.o -o hashmake

parsecode: parsecode.o typeprint.o typecode.o code.h
	cc $(CFLAGS) parsecode.o typeprint.o typecode.o -o parsecode
	 	
spellprog.o: spellprog.c code.h
	cc $(CFLAGS) -c spellprog.c

spellin.o: spellin.c hash.h
	cc $(CFLAGS) -c spellin.c

hashcheck.o: hashcheck.c hash.h
	cc $(CFLAGS) -c hashcheck.c

hashmake.o: hashmake.c hash.h
	cc $(CFLAGS) -c hashmake.c

parsecode.o: parsecode.c hash.h
	cc $(CFLAGS) -c parsecode.c

hash.o: hash.c hash.h
	cc $(CFLAGS) -c hash.c

hashlook.o: hashlook.c hash.h huff.h
	cc $(CFLAGS) -c hashlook.c

typecode.o: typecode.c code.h
	cc $(CFLAGS) -c typecode.c

typeprint.o: typeprint.c code.h
	cc $(CFLAGS) -c typeprint.c

huff.o: huff.c huff.h
	cc $(CFLAGS) -c huff.c

lists: amspell brspell

amspell: list american local hashmake spellin stop parsecode
	./parsecode list american local stop | ./spellprep >amspell

	
brspell: list british local hashmake spellin stop parsecode
	./parsecode list british local  stop | ./spellprep >brspell


install: spellprog amspell brspell spell.sh
	strip spellprog
	cp spellprog /usr/dict/spellprog
	cp amspell brspell  /usr/dict
	cp spell.sh /usr/bin/spell

clean:
	rm -f *.o spellprog amspell brspell spellin hashmake \
		hashcheck parsecode nohup.out core squeeze unsqueeze

squeeze: squeeze.c
	cc squeeze.c -o squeeze

list.sq: squeeze list
	squeeze <list >list.sq

american.sq: squeeze american
	squeeze <american >american.sq

british.sq: squeeze british
	squeeze <british >british.sq

local.sq: squeeze local
	squeeze <local >local.sq

stop.sq: squeeze stop
	squeeze <stop >stop.sq

spell.a.z:  list.sq american.sq british.sq local.sq stop.sq
	ar cr spell.a Makefile code.h hash.h huff.h hash.c hashcheck.c hashlook.c \
		hashmake.c huff.c parsecode.c spellin.c spellprog.c typecode.c \
		typeprint.c squeeze.c unsqueeze.c list.sq american.sq  \
		british.sq local.sq stop.sq spell.sh spellprep
	pack spell.a

unsqueeze: unsqueeze.c
	cc unsqueeze.c -o unsqeeze

expand: unsqueeze list.sq american.sq british.sq stop.sq local.sq
	for i in list american british stop local; do unsqeeze <$$i.sq >$$i; done
