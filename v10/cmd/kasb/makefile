YACCRM=-rm
TESTDIR = .
FRC =
INSDIR =
CFLAGS = -g $(CCFLAGS)

all:	kasb

kasb:	kas0.o kas1.o kas2.o kas3.o
	$(CC) $(CFLAGS)  -o $(TESTDIR)/kasb kas0.o kas1.o kas2.o kas3.o -ll
kas0.o:	kas.h kas0.c kas0.yl $(FRC)
kas1.o:	kas.h kas1.c $(FRC)
kas2.o: kas.h kas2.c kas.yh $(FRC)
kas3.o: kas.h kas3.c $(FRC)
kas0.c:	kas0.y $(FRC)
	yacc -d kas0.y
	mv y.tab.c kas0.c
	mv y.tab.h kas.yh
kas0.yl: kas0.l $(FRC)
	lex kas0.l
	mv lex.yy.c kas0.yl

install:	all
	cp kasb /bin
	strip /bin/kasb

clean:
	-rm kasb *.o kas.yh kas0.c kas0.yl
