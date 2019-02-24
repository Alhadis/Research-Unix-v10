CC=3cc
CFLAGS=-g
OBJS=bubble.o ball.o other.o newmenu.o

term:	$(OBJS) nodes.h textures.h
	$(CC) $(CFLAGS) -o bubble.m $(OBJS) -ljj
	cp bubble.m /usr/jerq/mbin

host:	host.c
	cc -g -o bubble host.c
	cp bubble /usr/jerq/bin

test:
	for i in `seq 100 10 200`
	do
		for j in `seq 100 10 200`
		do
			echo "${i}x ${j}y 0zh"
		done
	done > test

cyntax:
	cyntax -I/usr/jerq/include -Dvoid=int bubble.c ball.c other.c newmenu.c -lj
