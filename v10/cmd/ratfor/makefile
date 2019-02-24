CFLAGS = -g
a.out:	r0.o r1.o r2.o rio.o rlook.o rlex.o y.tab.o
	cc r*.o y.tab.o -ly

r0.o:	r.h y.tab.h r0.c
r1.o:	r.h y.tab.h r1.c
r2.o:	r.h y.tab.h r2.c
rio.o:	r.h y.tab.h rio.c
rlook.o:	r.h y.tab.h rlook.c
rlex.o:	r.h y.tab.h rlex.c
y.tab.c:	r.g
	yacc -d r.g
y.tab.h:	r.g
	yacc -d r.g

list:
	pr r.g r.h r*.c makefile TODO

install:	a.out
	strip a.out
	cp a.out /usr/bin/ratfor

clean:
	rm -f a.out *.o y.tab.* make.out
