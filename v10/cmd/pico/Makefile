CC = cc
YFLAGS = -d -D -v
CFLAGS = -g

OBJS0 = x.o
OBJS1 = builtins.o loops.o files.o gen.o opt.o pmet.o pfb.o \
	munge.o other.o windows.o main.o psin.o ptan.o fsqrt.o tobit.o
OBJS2 = lex.o sym.o
OBJS3 = nodes.o

pico:	$(OBJS0) $(OBJS1) $(OBJS3) $(OBJS2)
	$(CC) $(OBJS0) $(OBJS1) $(OBJS3) $(OBJS2) -o pico -lfb
	strip -g pico

$(OBJS1):	pico.h
$(OBJS0):	pico.h pico2.h
$(OBJS2):	pico.h x.tab.h
$(OBJS3):	pico.h pico2.h x.tab.h

x.tab.h:	y.tab.h
	-cmp -s x.tab.h y.tab.h || cp y.tab.h x.tab.h

clean:
	rm -f *.o core a.out pico

install:	pico
	cp pico /usr/bin
