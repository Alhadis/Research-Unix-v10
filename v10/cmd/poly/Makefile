CFLAGS=-g

all:	polypr polypic

polypr:	polypr.c /usr/include/poly.h
	$(CC) $(CFLAGS) -o polypr polypr.c

polypic:	polypic.c /usr/include/poly.h
	$(CC) $(CFLAGS) -o polypic polypic.c


clean:
	rm polypr polypic

install:	polypr polypic
	cp polypic polypr /usr/bin
