TERM = blit
# you may want to change this to TERM = x11 or TERM = 630

FILES = README LONG_DESC movie.1 man.page script.def stills.def int.def \
	isort.gen test.i test.s \
	movie stills stills.awk develop \
	makefile fdev.make blit.make 630.make x11.make \
	fdevelop.h fdevelop.c fdsymbol.c fdutil.c \
	anim.h xjerq.h host.c term.c libj.c \
	hsupport.c protocol.c \
	view.clicks show.clicks newer.c \
	sun.make sunhost.c sunterm.c

all:
	make -f fdev.make
	make -f $(TERM).make
	cc -o newer newer.c
	chmod +x movie develop stills show.clicks view.clicks

blit:
	make -f blit.make

x11:
	make -f x11.make

630:
	make -f 630.make

install:
	echo change LIB=... in movie, stills, develop, blit.make, x11.make
	echo then cp develop fdevelop stills.awk anim animterm newer  LIB
	echo then cp stills movie /usr/bin

clean:
	rm *.o

realclean:
	rm *.o fdevelop anim animterm newer

bundle:
	@bundle $(FILES)

bowell:	$(FILES)
	push bowell $? /usr/src/cmd/movie
	touch bowell

BIN = /usr/bin
LIB = /usr/lib/movie

ship:
	ship $(BIN)/movie $(BIN)/stills $(LIB)/develop $(LIB)/fdevelop \
		$(LIB)/stills.awk $(LIB)/anim $(LIB)/animterm $(LIB)/newer
