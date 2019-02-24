# pick one for yourself:
LIB = /usr/lib/movie
LIB = .

# add -Dsafe to CFLAGS if not on a Sun
CFLAGS = -g -DX11
CC = cc

HOBJ = host.o hsupport.o protocol.o
TOBJ = term.o protocol.o libj.o
SRCS = host.c hsupport.c anim.h protocol.c term.c x11.make

here: anim animterm

anim:	$(HOBJ)
	$(CC) $(CFLAGS) $(HOBJ) -o anim

host.o:	anim.h
	$(CC) -c $(CFLAGS) -DANIMTERM=\"$(LIB)/animterm\" host.c

animterm: $(TOBJ)
	$(CC) $(CFLAGS) $(TOBJ) -o animterm -lX

term.o: anim.h xjerq.h

libj.o: xjerq.h
	$(CC) $(CFLAGS) -DBSD -c libj.c
