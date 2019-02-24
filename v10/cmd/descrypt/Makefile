SRC= SP.c io.c keysetup.c crunch.c des.c permute.c randblock.c setup.c getpass.c alpha.c
OBJS= SP.o io.o keysetup.o crunch.o des.o permute.o randblock.o setup.o getpass.o alpha.o
CFLAGS= -O

encrypt:	$(OBJS) encrypt.o decrypt.o
	cc -o encrypt encrypt.o $(OBJS)
	cc -o decrypt decrypt.o $(OBJS)

$(OBJS) encrypt.o decrypt.o:	crypt.h

install:	encrypt
	cp encrypt decrypt /usr/bin
	strip /usr/bin/encrypt /usr/bin/decrypt

clean:
	rm -f *.o encrypt decrypt
