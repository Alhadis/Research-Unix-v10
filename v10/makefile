CC=vcc
CFLAGS=-g
SOURCE=dkstuff.c fserv.h main.c ps.y perm.c start.c sub.c sys.c wait3.s work.c \
	setup.c
OBJ=main.o dkstuff.o ps.o perm.o sys.o wait3.o start.o sub.o work.o
fshare:	$(OBJ) profit.o
	$(CC) -o fshare $(LDFLAGS) $(OBJ) profit.o -ldk -ljobs -lpjw
list:	$(SOURCE) makefile
	-prcan $?
	touch list
$(OBJ):	fserv.h
setup:	setup.c
	cc -g -o setup setup.c -ldk -lpjw
ptest:	ptest.o perm.o ps.o
	cc -o ptest ptest.o perm.o ps.o
