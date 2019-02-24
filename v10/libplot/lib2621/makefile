lib2621.a: hp.c.a
	mkdir xplot
	cd xplot;ar x ../hp.c.a
	cd xplot;cc -c -O *.c
	cd xplot;ar rc ../lib2621.a *.o
	rm -r xplot

install:	lib2621.a
	cp lib2621.a /usr/lib
	ranlib /usr/lib/lib2621.a
clean:
	rm -f lib2621.a
