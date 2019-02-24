CFLAGS=-g

test: coloriz
	coloriz -n254,50 <wedge.i >wedge.rgb
	od -c wedge.rgb | sed 15q
coloriz: coloriz.o cvmap.o sys.o
	$(CC) -o coloriz coloriz.o cvmap.o sys.o

clean:
	rm -f core *.o coloriz
