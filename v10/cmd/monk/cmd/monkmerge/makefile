MBIN = /usr/bin

CFLAGS = -O
LDFLAGS = -s

LINT = /usr/frodo/bin/lint
LFLAGS =

FRC =

MONKMERGEFILES = \
	monkmerge.o \
	malloc.o \
	tokenize.o

all:	monkmerge 

monkmerge:	$(MONKMERGEFILES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o monkmerge $(MONKMERGEFILES)

lintmonkmerge:
	@$(LINT) $(LFLAGS) `echo $(MONKMERGEFILES) | sed -e 's/\.o/.c/g'`

clean:
	-rm -f *.o 

clobber: clean
	-rm -f monkmerge 

install: all
	cp monkmerge $(MBIN)

monkmerge.o: \
	monkmerge.h \
	$(FRC)

malloc.o: \
	$(FRC)

tokenize.o: \
	rd.h \
	search.h \
	$(FRC)

FRC:
