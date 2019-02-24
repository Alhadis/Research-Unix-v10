OBJ=a.o batcomp.o bomb.o event.o explode.o image.o rock.o ship.o
FILES=a.c batcomp.c bomb.c event.c explode.c image.c rock.c ship.c
BIN=/usr/jerq/bin
CC=3cc
CFLAGS=

a:	$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o a 
lint:
	lint -p -I/usr/jerq/include $(FILES)
pp:
	pp -kjerq -keywords -t"Asteroids" event.h rock.h ship.h $(FILES) | dcan

install:	a
	mopt a
	mstrip m.out
	cp m.out /usr/jerq/demo/mpx/asteroids
clean:
	rm -f *.o core a m.out
