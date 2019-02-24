CC=3cc
CFLAGS=-g

FOBJS= box.o draw.o frdelete.o frinit.o frinsert.o ptofchar.o select.o str.o util.o
FOOBJS=box.O draw.O frdelete.O frinit.O frinsert.O ptofchar.O select.O str.O util.O
GOBJS= main.o icons.o menu.o mesg.o rasp.o scroll.o flayer.o
GOOBJS=main.O icons.O menu.O mesg.O rasp.O scroll.O flayer.O

OBJS=$(FOBJS) $(GOBJS)
OOBJS=$(FOOBJS) $(GOOBJS)

all:	cyntax a.out
export:	cyntax
	rm -f *.o
	cp defont.ex.h defont.h
	make a.out
	mv a.out export
	rm *.o
	cp /dev/null defont.h
a.out:&	$(OBJS)
	$(CC) $(OBJS)
cyntax:&	$(OOBJS)
	cyntax -jm $(OOBJS) && touch cyntax

flayer.o flayer.O:	flayer.h
$(FOBJS) $(FOOBJS):	frame.h
$(GOBJS) $(GOOBJS):	samterm.h frame.h flayer.h ../mesg.h

%.O:	%.c
	cyntax -jmc -Dlint $%.c

clean:
	rm -f core a.out *.o *.O
