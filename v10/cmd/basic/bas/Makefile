#CFLAGS = -O
OBJECTS = cmds.o def.o expr.o fns.o io.o main.o misc.o\
		plot.o renum.o stack.o sym.o sys.o\
		tokens.o wlib.o enter.o

bas:	$(OBJECTS)
	cc $(OBJECTS) -lm -o bas

$(OBJECTS):	basic.h defines.h tokens.h typedef.h

tokens.h:	tokens.c
	grep '^	"' tokens.c | grep -n "^" | \
	    sed "s;[^0-9]*\([0-9]*\).*/\* \([^ ]*\).*;#define	\2	-\1;" \
	    > tokens.h
