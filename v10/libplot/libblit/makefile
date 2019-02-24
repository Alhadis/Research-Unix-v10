libblit.a: blit.c.a
	mkdir xplot
	cd xplot;ar x ../blit.c.a
	cd xplot;cc -c -O *.c
	cd xplot;ar rc ../libblit.a *.o
	rm -r xplot/*

install:	libblit.a
	cp libblit.a /usr/lib
	ranlib /usr/lib/libblit.a

clean:
	rm -rf xplot libblit.a
