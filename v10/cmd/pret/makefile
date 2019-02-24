CC = cc
YFLAGS = -dDv
CFLAGS = -g
OBJS = pret.o pret1.o pret2.o pret3.o pret4.o pret6.o pret7.o pret8.o pret9.o pret0.o pretlex.o

#	FILES:
#		pret.y	-	yacc parser
#		pret.h	-	constants and macros
#		pret.d	-	internal representation of the tables
#		pret0.c  -	reduction of the state machines
#		pret1.c	-	qsets
#		pret2.c	-	operations on rows and columns
#		pret3.c	-	procedures
#		pret4.c	-	processes
#		pret6.c	-	queues and messages
#		pret7.c	-	memory allocation
#		pret8.c	-	variables
#		pret9.c	-	parameters
#		pret.expr.c -	expressions
#		pretlex.c	lexical analyzer

pret:	$(OBJS) pret.h pret.d
	$(CC) $(CFLAGS) $(OBJS) -lln -o pret

pret.o:	pret.y pret.expr.c

pretlex.o:	pretlex.l x.tab.h
	lex pretlex.l
	$(CC) $(CFLAGS) -c lex.yy.c
	rm lex.yy.c
	mv lex.yy.o pretlex.o

x.tab.h:	y.tab.h
	-cmp -s x.tab.h y.tab.h || cp y.tab.h x.tab.h

clean:
	rm -f *.o [xy].tab.[ch] y.output pret.out pret.err pret.tmp core a.out

install:
	cp pret /usr/bin
