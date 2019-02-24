graph: graph.c
	cc -O graph.c -o graph -lplot -lm

install:	graph
	cp graph /usr/bin
	strip /usr/bin/graph

clean:
	rm -f *.o graph
