D=/bin/oops
CFLAGS = -O

oops:	oops.o
	cc -o oops oops.o

install:	oops
	cp oops $D
	strip $D
	/etc/chown ,sys $D
	chmod o-w,g+ws $D

clean:
	rm -f oops *.o
