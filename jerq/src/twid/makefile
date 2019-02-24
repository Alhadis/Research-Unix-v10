CFLAGS=
CC=3cc
OBJ=twid.o cursor.o data.o discture.o rotate.o bitfile.o
FILES=twid.c cursor.c data.c discture.c rotate.c bitfile.c

twid.m:	$(OBJ)
	$(CC) $(CFLAGS) -o twid.m $(OBJ)

cyntax:
	cyntax -m $(FILES) 2>&1 | cat

lint:
	@lint -I/usr/jerq/include $(FILES) -lj

clean:
	rm -rf twid $(OBJ) core twid.m
pp:
	pp -kjerq -ttwid $(FILES) | dcan

install:	twid.m
	cp twid.m /usr/jerq/mbin/twid.m

twid.o data.o cursor.o:	twid.h
