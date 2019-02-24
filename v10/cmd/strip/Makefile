CFLAGS = -Od2

HDR= strip.h hash.h malloc.h stab.h
SRC= strip.c stab.c rdout.c shrink.c symwrite.c hash.c prtsym.c fcopy.c
STRIP= strip.o rdout.o shrink.o symwrite.o hash.o fcopy.o
STAB= stab.o rdout.o prtsym.o

all: strip

strip: $(STRIP)
	$(CC) $(CFLAGS) -o strip $(STRIP)

stab: $(STAB)
	$(CC) $(CFLAGS) -o stab $(STAB)

strip.o stab.o rdout.o shrink.o symwrite.o: strip.h
strip.o shrink.o symwrite.o hash.o: hash.h
strip.o stab.o rdout.o shrink.o symwrite.o hash.o: malloc.h
shrink.o prtsym.o: stab.h

install: all
	cp strip strip.out
	strip strip.out
	cp strip.out /usr/bin/strip

clean:
	rm -f strip stab *.o

pp:	Makefile $(HDR) $(SRC)
	@pp -fPO $?
	@touch pp

list:	Makefile $(HDR) $(SRC)
	4prtiny $?
	touch list
