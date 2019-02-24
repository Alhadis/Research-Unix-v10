CC =	cc
CFLAGS =	-O -DNODELAY
CFILES =	pacman.c monster.c util.c
OFILES =	pacman.o monster.o util.o

pacr:	$(OFILES)
	$(CC) -o pacr $(OFILES) -ltermlib -lcurses

pacman.o:	pacman.c pacdefs.h
	$(CC) -c $(CFLAGS) $(DFLAGS) pacman.c

monster.o:	monster.c pacdefs.h
	$(CC) -c $(CFLAGS)  $(DFLAGS) monster.c

util.o:	util.c pacdefs.h
	$(CC) -c $(CFLAGS)  $(DFLAGS) util.c


install:	pacr
	strip pacr
	mv pacr /usr/games/pacman


clean:
	rm -f *.o pacr

lint:	$(CFILES)
	lint -pc $(CFILES)

list:	$(CFILES) pacdefs.h makefile
	oprl -x makefile $(CFILES) pacdefs.h
	oprl -x -C $(CFILES)
	touch list
