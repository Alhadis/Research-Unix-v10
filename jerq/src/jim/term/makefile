CFLAGS=-g
CC=3cc
F=../frame
CFILES=main.c frame.c menu.c msgs.c string.c $F/deltext.c $F/fralloc.c $F/frameop.c $F/instext.c $F/ptofchar.c
FILES=main.o frame.o menu.o msgs.o string.o $F/frame.a

jim.m:&	$(FILES) $F/frame.a
	$(CC) -o jim.m $(FILES)
$F/frame.a:
	cd $F; make
print:
	@prcan $F/frame.h $(CFILES)

pp:
	@pp -t"Terminal Jim" jim.h menu.h msgs.h $F/frame.h $(CFILES) | dcan

lint:
	lint -Dvoid=int -I/usr/jerq/include -DMUX $(CFILES) -lj 2>&1 | /usr/rob/bin/buffer

cyntax:
	cyntax -m $(CFILES)

$(FILES): $F/frame.h /usr/jerq/include/jerq.h /usr/jerq/include/jerqproc.h
main.o frame.o menu.o:	jim.h
jim.h:	$F/frame.h menu.h msgs.h
.PRECIOUS:	frame.a

install:	jim.m
	cprs jim.m /usr/jerq/mbin/jim.m

clean:
	rm -f *.o jim.m core
	cd $F; make clean
