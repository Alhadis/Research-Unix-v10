D=/usr/bin/load
CFLAGS = -O

load:	load.o
	cc -o load load.o

install:	load
	cp load $D
	strip $D
	/etc/chown ,sys $D
	chmod o-w,g+ws $D

clean:
	rm -f load *.o
