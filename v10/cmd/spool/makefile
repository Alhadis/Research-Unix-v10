CFLAGS = -O

all:	lab lp1

lp0:	lpr.h lp0.o spool.c
	cc -o lp0 lp0.o

lp1:	lpr.h lp1.o spool.c
	cc -o lp1 lp1.o

lab:	lpr.h lab.o spool.c
	cc -o lab lab.o

install:	lp0 lp1
	strip lp0 lp1
	mv lp0 lp1 $(DESTDIR)/usr/bin
	rm -f /usr/bin/lpr
	ln /usr/bin/lp1 /usr/bin/lpr

clean:
	rm -f *.o lp[01]
