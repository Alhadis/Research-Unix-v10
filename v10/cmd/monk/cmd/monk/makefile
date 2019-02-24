MHOME = /usr/lib/monk

CFLAGS = -O
LDFLAGS = -s

LINT = /usr/frodo/bin/lint
LFLAGS =

FRC =

MONKFILES = \
	db_manage.o \
	envir.o \
	hash.o \
	malloc.o \
	pr_struct.o \
	read_attrib.o \
	read_cmpress.o \
	read_db.o \
	read_define.o \
	read_user.o \
	regcmp.o \
	regex.o \
	run_monk.o \
	support.o \
	textput.o \
	tokenize.o \
	translate.o \
	warn.o

COMPRESSFILES = \
	cdb_manage.o \
	compress.o \
	hash.o \
	malloc.o \
	pr_struct.o \
	read_attrib.o \
	read_db.o \
	read_define.o \
	regcmp.o \
	regex.o \
	support.o \
	tokenize.o \
	translate.o \
	warn.o

all:	monk compress

monk:	$(MONKFILES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o monk $(MONKFILES)

compress:	$(COMPRESSFILES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o compress $(COMPRESSFILES)

lintmonk:
	@$(LINT) $(LFLAGS) `echo $(MONKFILES) | sed -e 's/\.o/.c/g'`

lintcompress:
	@$(LINT) $(LFLAGS) -DCMONK `echo $(COMPRESSFILES) | sed -e 's/\.o/.c/g'`

clean:
	-rm -f *.o cdb_manage.c

clobber: clean
	-rm -f monk compress

install: all
	cp monk $(MHOME)
	cp compress $(MHOME)

cdb_manage.c: db_manage.c
	cp db_manage.c cdb_manage.c

cdb_manage.o: \
	dbcompress.h \
	rd.h \
	search.h \
	warn.h \
	$(FRC)
	$(CC) $(CFLAGS) -c -DCMONK cdb_manage.c

compress.o: \
	dbcompress.h \
	rd.h \
	search.h \
	space.h \
	warn.h \
	$(FRC)

db_manage.o: \
	dbcompress.h \
	rd.h \
	search.h \
	warn.h \
	$(FRC)

envir.o: \
	file_info.h \
	rd.h \
	search.h \
	warn.h \
	$(FRC)

hash.o: \
	hash.h \
	search.h \
	$(FRC)

malloc.o: \
	$(FRC)

pr_struct.o: \
	rd.h \
	search.h \
	$(FRC)

read_attrib.o: \
	dbcompress.h \
	file_info.h \
	rd.h \
	search.h \
	warn.h \
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

read_define.o: \
	dbcompress.h \
	rd.h \
	search.h \
	warn.h \
	$(FRC)

read_user.o: \
	rd.h \
	search.h \
	warn.h \
	$(FRC)

regcmp.o: \
	$(FRC)

regex.o: \
	$(FRC)

run_monk.o: \
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

textput.o: \
	rd.h \
	search.h \
	warn.h \
	$(FRC)

tokenize.o: \
	rd.h \
	search.h \
	$(FRC)

translate.o: \
	dbcompress.h \
	rd.h \
	search.h \
	warn.h \
	$(FRC)

warn.o: \
	file_info.h \
	warn.h \
	$(FRC)

FRC:
