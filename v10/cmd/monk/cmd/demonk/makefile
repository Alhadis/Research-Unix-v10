MHOME = /usr/lib/monk

CFLAGS = -O
LDFLAGS = -s

LINT = /usr/frodo/bin/lint
LFLAGS =

FRC =

DEMONKFILES = \
	demonk.o \
	hash.o \
	malloc.o \
	read_cmpress.o \
	read_db.o \
	support.o \
	tokenize.o

all:	demonk 

demonk:	$(DEMONKFILES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o demonk $(DEMONKFILES)

lintdemonk:
	@$(LINT) $(LFLAGS) `echo $(DEMONKFILES) | sed -e 's/\.o/.c/g'`

clean:
	-rm -f *.o

clobber: clean
	-rm -f demonk 

install: all
	cp demonk $(MHOME)

demonk.o: \
	demonk.h \
	search.h \
	rd.h \
	$(FRC)

hash.o: \
	hash.h \
	search.h \
	$(FRC)

malloc.o: \
	$(FRC)

read_cmpress.o: \
	dbcompress.h \
	rd.h \
	search.h \
	warn.h \
	$(FRC)

read_db.o: \
	dbcompress.h \
	rd.h \
	search.h \
	warn.h \
	$(FRC)

support.o: \
	dbcompress.h \
	rd.h \
	search.h \
	warn.h \
	$(FRC)

tokenize.o: \
	rd.h \
	search.h \
	$(FRC)

FRC:
