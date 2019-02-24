CC = cc     # the usual situation
CFLAGS =    # the usual situation

CC = lcc        # you will probably want to remove this 
CFLAGS = -g -A -N -I/usr/include/lcc -I/usr/include       # and this

YFLAGS = -d

SOURCE = eqn.y e.h main.c tuning.c diacrit.c eqnbox.c font.c fromto.c funny.c glob.c integral.c \
 input.c lex.c lookup.c mark.c matrix.c move.c over.c paren.c \
 pile.c shift.c size.c sqrt.c text.c

FILES =  main.o tuning.o diacrit.o eqnbox.o font.o fromto.o funny.o glob.o integral.o \
 input.o lex.o lookup.o mark.o matrix.o move.o over.o paren.o \
 pile.o shift.o size.o sqrt.o text.o eqn.o

a.out:	$(FILES)
	$(CC) $(CFLAGS) $(FILES) 

y.tab.h:	eqn.o

prevy.tab.h:	y.tab.h
	-cmp -s y.tab.h prevy.tab.h || cp y.tab.h prevy.tab.h

$(FILES):	e.h prevy.tab.h

bundle:
	@bundle $(SOURCE) makefile README

bowell:	README $(SOURCE) makefile eqntest.a
	push bowell $? /usr/src/cmd/eqn
	@touch bowell

install:
	cp a.out /usr/bin/eqn
	strip /usr/bin/eqn

clean:
	rm -f a.out *.o make.out *y.tab.h foo* core
