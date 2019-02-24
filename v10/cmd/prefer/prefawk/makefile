PLIB = /usr/lib/prefer
PBIN = /usr/bin

YFLAGS = -d
CFLAGS = -O

OFILES = b.o main.o parse.o proctab.o tran.o lib.o run.o 

SOURCE = awk.h awk.g.y awk.lx.l b.c main.c maketab.c parse.c lib.c run.c tran.c

prefawk:	awk.g.o awk.lx.o $(OFILES) $(ALLOC) awk.h
	cc $(CFLAGS) awk.g.o awk.lx.o $(OFILES) $(ALLOC)   -lm -o prefawk

$(OFILES) awk.lx.o:	awk.h prevy.tab.h 

awk.g.o:	awk.h

y.tab.h:	awk.g.o

prevy.tab.h:	y.tab.h
	-cmp -s y.tab.h prevy.tab.h || (cp y.tab.h prevy.tab.h; echo change maketab)

proctab.c:	maketab
	./maketab >proctab.c

maketab:	prevy.tab.h maketab.c
	cc maketab.c -o maketab

profile:
	make 'CFLAGS=-p'


lcomp:
	make 'CC=lcomp' 'CFLAGS='

lprint:
	echo 1>&2 "piping into can"
	@lprint -P "pr -l90 -w120" | lp -fCW.8 -l90


clean:
	rm -f a.out *.o t.* *temp* *.out *junk* y.tab.* prevy.tab.h maketab proctab.c

clobber: clean
	-rm -f prefawk

run.o: awk.h y.tab.h
	$(CC) $(CFLAGS) -c -DFLUSH run.c

install: prefawk
	cp prefawk $(PLIB)
	strip $(PLIB)/prefawk

