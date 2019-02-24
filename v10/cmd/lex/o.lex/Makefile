all: lex

lex: lmain.o y.tab.o sub1.o sub2.o header.o
	cc lmain.o y.tab.o sub1.o sub2.o header.o -o lex

smallex:
	cc -DSMALL lmain.c y.tab.c sub1.c sub2.c header.c -o smallex

y.tab.c: parser.y
	yacc parser.y
lmain.o:lmain.c ldefs.c once.c
	cc -c lmain.c
sub1.o: sub1.c ldefs.c
	cc -c sub1.c
sub2.o: sub2.c ldefs.c
	cc -c sub2.c
header.o: header.c ldefs.c
	cc -c header.c

install: all
	strip lex
	mv lex $(DESTDIR)/usr/bin
	cp ncform $(DESTDIR)/usr/lib/lex

clean:
	rm -f *.o lex y.tab.c
