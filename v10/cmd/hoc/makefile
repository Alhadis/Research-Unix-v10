YFLAGS = -d
CFLAGS = -g

SRC = hoc.y hoc.h code.c init.c math.c symbol.c
OBJS = hoc.o code.o init.o math.o symbol.o

hoc:	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lm -o hoc

hoc.o code.o init.o symbol.o:	hoc.h

code.o init.o symbol.o:	x.tab.h

x.tab.h:	y.tab.h
	-cmp -s x.tab.h y.tab.h || cp y.tab.h x.tab.h

pr:	$(SRC)
	@prcan $?
	@touch pr

install:	hoc
	cp hoc /usr/bin
	strip /usr/bin/hoc

clean:
	rm -f $(OBJS) [xy].tab.[ch]  hoc

bundle:
	@bundle $(SRC) makefile README
