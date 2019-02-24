CC = cc     # the usual situation
CFLAGS =    # the usual situation

CC = lcc        # you will probably want to remove this 
CFLAGS = -g -A -N -I/usr/include/lcc -I/usr/include       # and this

YFLAGS = -d

# ALLOC = malloc.o

OFILES = main.o input.o print.o frame.o for.o coord.o ticks.o plot.o label.o misc.o $(ALLOC)
CFILES = main.c input.c print.c frame.c for.c coord.c ticks.c plot.c label.c misc.c
SRCFILES = grap.y grapl.l grap.h $(CFILES)

a.out:	grap.o grapl.o $(OFILES) grap.h 
	$(CC) $(CFLAGS) grap.o grapl.o $(OFILES) -lm

$(OFILES) grapl.o:	grap.h prevy.tab.h

grap.o:	grap.h

y.tab.h:	grap.o

prevy.tab.h:	y.tab.h
	-cmp -s y.tab.h prevy.tab.h || cp y.tab.h prevy.tab.h

prcan:	README WISH $(SRCFILES) makefile
	@lprcan $(CAN) $? $(OTHERS)
	@touch prcan

bin:
	cp /usr/bin/grap /usr/bin/ograp
	cp a.out /usr/bin/grap

clean:
	rm -f *.o a.out *y.tab.[ch] lex.yy.c foo* *.[gd]

bowell:	$(SRCFILES) makefile grap.1 grap.defines graptest.a WISH FIXES
	push bowell $? /usr/src/cmd/grap
	touch bowell

bundle:
	@bundle FIXES README $(SRCFILES) makefile grap.1 grap.defines
