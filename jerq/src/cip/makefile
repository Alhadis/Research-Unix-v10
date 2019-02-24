CC=3cc
CFLAGS=

CFILES=buttons.c cip.c commands.c draw.c dump.c edit.c menus.c near.c pic.c readPic.c space.c things.c track.c transform.c
FILES=buttons.o cip.o commands.o draw.o dump.o edit.o menus.o near.o pic.o readPic.o space.o things.o track.o transform.o

cip.m:	/usr/jerq/lib/libj.a /usr/jerq/lib/libmj.a
cip.m:	$(FILES) 
	$(CC) -o cip.m $(FILES)
$(FILES):	cip.h /usr/jerq/include/jerq.h /usr/jerq/include/mux.h
cyntax:
	cyntax -m $(CFILES)
clean:
	rm -f $(FILES) core cip.m
install:	cip.m
	cprs cip.m /usr/jerq/mbin/cip.m
