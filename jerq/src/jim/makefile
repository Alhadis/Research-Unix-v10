CFILES=alloc.c file.c gcalloc.c jim.c msgs.c pattern.c string.c unix.c
FILES=alloc.o file.o gcalloc.o jim.o msgs.o pattern.o string.o unix.o
OFILES=alloc.O file.O gcalloc.O jim.O msgs.O pattern.O string.O unix.O
CFLAGS=-g
.SUFFIXES:	.O

%.O:	%.c
	cyntax -c $<

xjim:&	$(FILES)
	$(CC) $(CFLAGS) $(FILES) -o xjim
cyntax:	$(OFILES)
	cyntax $(OFILES) && touch cyntax

jim.O:	jim.c
	cyntax -c -Vdprintf:1 jim.c

ftest:	ftest.o string.o file.o
	$(CC) -o ftest ftest.o string.o file.o

xljim:	$(FILES)
	lcomp $(CFILES) && cp a.out xljim && rm a.out

$(FILES):	jim.h
file.o jim.o pattern.o string.o unix.o:	file.h
jim.o msgs.o:	msgs.h
$(OFILES):	jim.h
file.O jim.O pattern.O string.O unix.O:	file.h
jim.O msgs.O:	msgs.h

lint:
	lint -h -u -x $(CFILES) 2>&1 | buffer

pp:
	@pp -t"Unix Jim" file.h msgs.h $(CFILES) | dcan

print:
	@pr file.h msgs.h $(CFILES)

install:	xjim
	cp xjim /usr/jerq/bin/jim
	strip -g /usr/jerq/bin/jim
	cd term; make install

clean:
	rm -f xjim *.o
	cd term; make clean
