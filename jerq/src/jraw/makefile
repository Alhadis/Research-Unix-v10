.SUFFIXES: .c .e .i .s .o
CC = 3CC
OFILES = io.o inst.o thing.o util.o menu.o text.o wire.o box.o list.o main.o tiny.o
CFILES = thing.c list.c inst.c text.c wire.c box.c menu.c util.c io.c main.c
HFILES = thing.h menu.h list.h inst.h text.h wire.h box.h
CFLAGS = -DMUX -g

njraw: $(OFILES)
	$(CC) -o njraw $(OFILES) tiny.o -ljj

huge: huge.o
	$(CC) -o huge huge.o tiny.o -ljj

huge.c: $(HFILES) $(CFILES)
	cat $(CFILES) > huge.c

tiny.o:
	3cc -c tiny.c

list.o: list.h

wire.o: list.h wire.h

box.o: list.h box.h

dots.o: list.h dots.h

text.o: list.h text.h

util.o: list.h

menu.o: menu.h thing.h

inst.o: list.h inst.h thing.h

thing.o: thing.h

main.o: menu.h thing.h inst.h list.h text.h wire.h box.h dots.h

io.o: menu.h thing.h inst.h list.h text.h wire.h box.h dots.h

print:
	cat $(HFILES) > junk
	CANDEST=u print jraw.h junk $(CFILES)

clean:
	rm njraw $(OFILES) huge.c

wc:
	wc $(HFILES) $(CFILES)

$(OFILES): jraw.h

.c.s:
	$(CC) -S $(CFLAGS) $<

.c.i:
	$(CC) -Fc -.i $(CFLAGS) $<

.c.e:
	$(CC) -Fc -.e -E $(CFLAGS) $<

files:
	@echo $(CFILES)

hfiles:
	@echo $(HFILES)

ship:
	@bundle makefile $(HFILES) $(CFILES) tiny.c
