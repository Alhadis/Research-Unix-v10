CFLAGS = -O

all:	lp1 lab

lp0:	lp0.o daemon.c daemon0.c banner.o
	cc -o lp0 lp0.o banner.o

lp1:	lp1.o daemon.c daemon0.c banner.o
	cc -o lp1 lp1.o banner.o

lab:	lab.o daemon.c daemon0.c
	cc -o lab lab.o

lp0.o:	lp0.c lpd.h

lp1.o:	lp1.c lpd.h

lab.o:	lab.c lpd.h

install: lp0 lp1
	strip lp0 lp1
	mv lp0 lp1 $(DESTDIR)/usr/lib

clean:
	rm -f lp[01] *.o
