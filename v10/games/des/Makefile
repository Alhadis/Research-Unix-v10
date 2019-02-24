CFLAGS = -Od2

OBJ = BOXES.o compkeys.o des.o encrypt.o

encrypt: $(OBJ)
	cc -o encrypt $(OBJ)

install:	encrypt
	rm -f /usr/games/encrypt /usr/games/decrypt
	strip encrypt
	mv encrypt /usr/games
	ln /usr/games/encrypt /usr/games/decrypt

clean:
	rm -f $(OBJ) encrypt
