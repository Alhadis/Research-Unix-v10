lib5620.a: blit.c.a
	mkdir xplot
	cd xplot;ar x ../blit.c.a
	cd xplot;cc -c -O *.c
	cd xplot;ar rc lib5620.a *.o
	mv xplot/lib5620.a lib5620.a
	rm -r xplot

install:	lib5620.a
	cp lib5620.a /usr/lib
	ranlib /usr/lib/lib5620.a

clean:
	rm -rf xplot lib5620.a
