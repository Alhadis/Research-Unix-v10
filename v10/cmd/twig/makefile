#
CC= cc
INSTALL=	/usr/bin
TEMPLATES = /usr/lib/twig
SRCS=	twig.y sym.c path.c machine.c trees.c lex.c code.c io.c mem.c
OBJS=	y.tab.o sym.o path.o machine.o trees.o lex.o code.o io.o mem.o
HDRS=	common.h code.h sym.h machine.h mem.h
PREFIX=	\"$(TEMPLATES)/walker\"

all: twig

install: twig
	mv twig $(INSTALL)
	mv walker.c1 $(TEMPLATES)

kindling:
	bundle README makefile *.y *.c *.h rawwalker.* >kindling

sym.h:	code.h
machine.h: machcomm.h
	touch machine.h

machine.o: common.h sym.h machine.h machine.c
	$(CC) -g -c machine.c

path.o: common.h sym.h path.c
	$(CC) -g -c path.c

y.tab.h: common.h  sym.h twig.y
	yacc -d twig.y

y.tab.c: y.tab.h common.h sym.h twig.y
y.tab.o: y.tab.c
	$(CC) -DPREFIX_BASE=$(PREFIX) -g -c y.tab.c

sym.o: common.h sym.h y.tab.h sym.c
	$(CC) -g -c sym.c

trees.o: common.h sym.h
	$(CC) -g -c trees.c

lex.o: common.h sym.h y.tab.h lex.c
	$(CC) -g -c lex.c

code.o: common.h code.h
	$(CC) -g -c code.c

io.o:	common.h io.c
	$(CC) -g -c io.c

mem.o:	common.h mem.c
	$(CC) -g -c mem.c

twig:	$(OBJS)
	$(CC) -g -o twig $(OBJS)

# generate walker from templates
walker.c1: machcomm.h walker.h rawwalker.c1
	cat machcomm.h walker.h rawwalker.c1 >walker.c1

walker.ex: machcomm.h walker.h rawwalker.ex
	cat machcomm.h walker.h rawwalker.ex >walker.ex

# at bell labs only
print:
	pr makefile $(HDRS) $(SRCS) | 4can
bell_print:
	pp makefile $(HDRS) $(SRCS) | dcan

# at stanford only
enscript:
	enscript -Pbt -2r makefile $(HDRS) $(SRCS)

