CFLAGS = -O
CC=cc
OBJ = map.o index.o symbol.o libmap.a

lplot: maplplot index.o symbol.o libmap.a
	$(CC) $(OBJ) -lplot -lm -o map

v10: mapv10 index.o symbol.o libmap.a
	$(CC) $(OBJ) -lm -o map

ps: maplplot plotPS.o index.o symbol.o libmap.a
	$(CC) $(OBJ) plotPS.o -lm -o map


mapv10: map.c iplot.h
	$(CC) -c $(CFLAGS) -DPLOT='"iplot.h"' map.c

maplplot: map.c plot.h
	$(CC) -c $(CFLAGS) -DPLOT='"plot.h"' map.c


route: route.o libmap.a
	$(CC) $(CFLAGS) route.o libmap.a -lm -o route

libmap.a: 
	cd libmap; make CC=$(CC)  CFLAGS="-I.. $(CFLAGS)"

install: map
	strip map
	test -d /usr/lib/map || mkdir /usr/lib/map
	cp map.sh /usr/bin/map
	cp map mapdata/* /usr/lib/map

clean:
	rm -f map route *.o libmap.a new.results
	cd libmap; make clean

quicktest: v10
	MAPPROG=./map MAPDIR=./mapdata map.sh mercator -l 0 10 0 10 -g -b >new.results
	:
	: If any "diff" output follows, it should show only roundoff differences.
	diff test.results new.results || true
	rm -f map map.o
