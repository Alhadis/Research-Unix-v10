libplot.a:	plot.c.a
	ar x plot.c.a
	cc -cO *.c
	ar rc libplot.a *.o
	ranlib libplot.a

install:	libplot.a
	cp iplot.h /usr/include
	cp pplot.h /usr/include
	cp plot.h /usr/include
	cp libplot.a /usr/lib
	ranlib /usr/lib/libplot.a

clean:
	rm *.o libplot.a *.c
