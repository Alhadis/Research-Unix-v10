.SUFFIXES: .c .o
B = /usr/bin
G = -Od2
C = cc
o =  tsort.o subs.o refstore.o
.c.o:
	$C -c $G $*.c
tsort: $o ts.h
	cc -o tsort $o
install: tsort
	strip tsort
	cp tsort $B
clean:
	rm -f tsort $o
