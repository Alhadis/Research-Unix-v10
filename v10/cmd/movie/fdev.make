CFLAGS = -g

DEVOBJS =	fdevelop.o fdsymbol.o fdutil.o
DEVSRC =	fdevelop.c fsymbol.c fdutil.c fdevelop.h
DEFS =		int.def script.def stills.def
SRC =		$(DEFS) \
		makefile \
		$(DEVSRC) develop \
		stills stills.awk

fdevelop: $(DEVOBJS)
	cc $(CFLAGS) $(DEVOBJS) -o fdevelop

$(DEVOBJS): fdevelop.h

prcan:	$(SRC)
	prcan $?
	touch prcan
