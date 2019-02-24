CFLAGS=-g -DUSE_STDIO -I. #-p -DPROFILING #
OBJ=main.o dofgrep.o dogre.o fns.o buffer.o cw.o bm.o eg.o egcomp.o eglit.o egpos.o egstate.o egcw.o egbr.o egmatch.o egcanon.o
HOBJ=htest.o eg.o egcomp.o eglit.o egpos.o egstate.o egcw.o egbr.o egmatch.o re.o cw.o refile.o egerror.o #dofgrep.o dogre.o fns.o buffer.o cw.o bm.o egcanon.o

gre:	$(OBJ)
	$(CC) $(CFLAGS) -o gre $(OBJ)

htest:	$(HOBJ)
	$(CC) $(CFLAGS) -o htest $(HOBJ)

regress:
	rm -f buffer.o; make CC=$(CC) CFLAGS="$(CFLAGS) -DBUFSIZE=500" gre
	cd regress.d; make GRE=../gre
	rm -f buffer.o gre
