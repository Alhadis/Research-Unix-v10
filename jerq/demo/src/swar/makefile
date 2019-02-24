CC=3cc
CFLAGS=
swar:	swar.o
	${CC} $(CFLAGS) swar.o -o swar
swar.o:	player0.icon player1.icon accel.h missile.icon deathstar.icon boom.icon
install:	swar
	cp swar /usr/jerq/demo/mux/swar
	mstrip -g /usr/jerq/demo/mux/swar
clean:
	rm -f *.o m.out core swar
