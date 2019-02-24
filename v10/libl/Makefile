CFLAGS = -O

all: libl.a

libl.a:
	cc -c -O allprint.c main.c reject.c yyless.c
	cc -c -O yywrap.c
	rm -f libl.a
	ar rvc libl.a allprint.o main.o reject.o yyless.o yywrap.o
	rm allprint.o main.o reject.o yyless.o yywrap.o

install: all
	cp libl.a $(DESTDIR)/usr/lib
	ranlib $(DESTDIR)/usr/lib/libl.a

clean:
	rm -f *.o libl.a
