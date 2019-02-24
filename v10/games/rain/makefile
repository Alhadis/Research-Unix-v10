CFLAGS = -Od2

rain:	rain.o
	cc -o rain rain.o -ltermlib

install:	rain
	strip rain
	mv rain /usr/games

clean:
	rm -f rain *.o
