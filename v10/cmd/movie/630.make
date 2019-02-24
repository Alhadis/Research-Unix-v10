# this is only for 630 with system v layers.
# no guarantees about pathnames or files
# DMDCC = dmdcc
DMDCC = /usr/630/bin/dmdcc
# TERMLD = dmdld
TERMLD = /usr/630/bin/dmdld

# pick one for yourself:
LIB = /usr/630/lib/movie
# LIB = .
BIN = /usr/630/bin

# use -DV9 only for version 9 (mux) blit version
CFLAGS = -g -DSYSV -DTTY630
CC = cc

HOBJ = host.o hsupport.o protocol.o
TOBJ = term.o tprotocol.o

here:	anim animterm fdevelop newer

anim:	$(HOBJ)
	$(CC) $(CFLAGS) $(HOBJ) -o anim

host.o:	anim.h
	$(CC) -c $(CFLAGS) -DBLIT \
		-DANIMTERM=\"$(LIB)/animterm\" -DTERMLD=\"$(TERMLD)\" host.c

animterm: term.c tprotocol.c
	$(DMDCC) $(CFLAGS) term.c tprotocol.c -o animterm

tprotocol.c:
	test -r tprotocol.c || ln protocol.c tprotocol.c

term.o: anim.h

newer: newer.c

fdevelop:
	make -f fdev.make

install:
	cp develop fdevelop stills.awk anim animterm newer $(LIB)
	cp stills movie $(BIN)

clean:
	rm *.o

clobber: clean
	rm fdevelop anim animterm newer

bundle:
	@bundle $(FILES)



ship:
	ship $(BIN)/movie $(BIN)/stills $(LIB)/develop $(LIB)/fdevelop \
		$(LIB)/stills.awk $(LIB)/anim $(LIB)/animterm $(LIB)/newer
