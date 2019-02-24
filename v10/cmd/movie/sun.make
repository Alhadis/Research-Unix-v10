TERM = sun

# pick one for yourself:
LIB = /usr/lib/movie
LIB = /usr/jlb/anim/Export

CFLAGS = -g
CC = cc

HOBJ = host.o hsupport.o protocol.o
TOBJ = sunterm.o tprotocol.o
SRCS = host.c hsupport.c anim.h protocol.c sunterm.c sun.make

here: anim animterm

anim:	$(HOBJ)
	$(CC) $(CFLAGS) $(HOBJ) -o anim

host.o:	anim.h
	$(CC) -c $(CFLAGS) -DSUN -DANIMTERM=\"$(LIB)/animterm\" host.c

animterm: $(TOBJ)
	$(CC) $(CFLAGS) $(TOBJ) -o animterm -lsuntool -lsunwindow -lpixrect

sunterm.o: anim.h
