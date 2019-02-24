OBJ = spitv35.o spitv35sub.o \
	arg2scb.o cpys2sc.o doexec.o doset.o dosys.o fakexit.o flush.o \
        getdate.o gethost.o lenfnm.o openerr.o osclose.o osopen.o \
	osint.o ospipe.o osread.o oswait.o oswrite.o \
	rdenv.o sigs.o sigtrap.o sioarg.o swcinp.o swcoup.o testty.o wrtaout.o

COBJ = arg2scb.o doexec.o dosys.o fakexit.o flush.o getdate.o lenfnm.o \
	openerr.o osclose.o osopen.o \
	osread.o oswrite.o rdenv.o sigs.o sigtrap.o sioarg.o swcinp.o swcoup.o testty.o \
	wrtaout.o

CFLAGS = -O


FILES = /usr/bin/spitbol /usr/lib/vaxspitv35.err

newspit: $(OBJ)
	cc -o newspit $(OBJ)

install: newspit
	cp newspit /usr/bin/spitbol

ship:
	./distribute $(FILES)

spitv35.o: spitv35.s header.s
	as -o spitv35.o header.s spitv35.s

spitv35sub.o: spitv35sub.s regmasks.s
	as -o spitv35sub.o spitv35sub.s regmasks.s

spitv35.ppmin: spitv35.min
	spitbol -i100k minpp.spt

spitv35.tok: spitv35.ppmin
	spitbol -i100k mintok.spt

spitv35.src: spitv35.tok spitv35.plb
	spitbol -i100k tokunx.spt

spitv35.serr: spitv35.src
	spitbol -i100k vaxpo1.spt

spitv35.s: spitv35.serr labels.sed
	sed -f labels.sed <spitv35.serr >spitv35.s

$(COBJ): spitblks.h spitio.h

clean:
	rm -f spitbol newspit $(OBJ)

clobber:	clean
	rm -f spitv35.ppmin spitv35.tok spitv35.src spitv35.serr spitv35.s
