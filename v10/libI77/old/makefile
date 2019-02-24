CFLAGS =-I. -g
INS = install
INSDIR = $(ROOT)/usr/lib
FFLAGS=-Ns500

OBJ =	Version.o backspace.o dfe.o due.o iio.o inquire.o rewind.o rsfe.o \
	rdfmt.o sue.o uio.o wsfe.o sfe.o fmt.o nio.o lio.o lread.o open.o \
	close.o util.o endfile.o wrtfmt.o err.o fmtlib.o ecvt.o ltostr.o

libI77.a:	$(OBJ)
		ar r libI77.a $?
		ranlib libI77.a
install:	libI77.a
	cp libI77.a /usr/lib/libI77.a
	ranlib /usr/lib/libI77.a

lio.o:	lio.h
SRC=	lio.h fio.h fmt.h backspace.c dfe.c due.c iio.c inquire.c rewind.c \
	rsfe.c rdfmt.c sue.c uio.c wsfe.c sfe.c fmt.c nio.c lio.c lread.c open.c \
	close.c util.c endfile.c wrtfmt.c err.c fmtlib.c ecvt.c ltostr.c
list:	$(SRC)
	pr $(SRC)

lread.o: lio.h

.c.o:
	$(CC) -c $(CFLAGS) $<

clean:
	-rm -f $(OBJ) libI77.a

clobber:	clean
	-rm -f libI77.a

backspace.o:  fio.h
close.o:  fio.h
dfe.o:  fio.h
dfe.o:  fmt.h
due.o:  fio.h
endfile.o:  fio.h
err.o:  fio.h
fmt.o:  fio.h
fmt.o:  fmt.h
ftest.o:  fio.h
iio.o:  fio.h
iio.o:  fmt.h
inquire.o:  fio.h
lib.o:  fio.h
lio.o:  fio.h
lio.o:  fmt.h
lio.o:  lio.h
lread.o:  fio.h
lread.o:  fmt.h
lread.o:  lio.h
nio.o:  fio.h
nio.o:  fmt.h
nio.o:  lio.h
open.o:  fio.h
rdfmt.o:  fio.h
rdfmt.o:  fmt.h
rewind.o:  fio.h
rsfe.o:  fio.h
rsfe.o:  fmt.h
sfe.o:  fio.h
stest.o:  fio.h
sue.o:  fio.h
uio.o:  fio.h
util.o:  fio.h
wrtfmt.o:  fio.h
wrtfmt.o:  fmt.h
wsfe.o:  fio.h
wsfe.o:  fmt.h
