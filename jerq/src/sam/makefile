CFILES=sam.c address.c alloc.c buffer.c cmd.c disc.c error.c file.c gcalloc.c \
	io.c list.c mesg.c misc.c moveto.c multi.c rasp.c regexp.c string.c \
	sys.c xec.c unix.c malloc.c
FILES=sam.o address.o alloc.o buffer.o cmd.o disc.o error.o file.o gcalloc.o \
	io.o list.o mesg.o misc.o moveto.o multi.o rasp.o regexp.o string.o \
	sys.o xec.o unix.o malloc.o
HFILES=sam.h errors.h parse.h
CFLAGS=-g -N -I/usr/include -DDIST
CC=lcc

sam:&	$(FILES)
	$(CC) -o sam $(CFLAGS) $(FILES) -lipc
	cp sam sam+g
	strip -g sam

unix.o:	unix.c
	$(CC) $(CFLAGS) -c -DV8 unix.c
$(FILES):	sam.h errors.h mesg.h
address.o cmd.o parse.o xec.o unix.o:	parse.h

clean:
	rm -f *.[o] core *.out

pr:
	prcan $(HFILES) $(CFILES)

lcomp:
	lcomp $(CFILES) -lnstdio
