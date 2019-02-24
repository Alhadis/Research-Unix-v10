MCC=3cc
all:	jplot jplot.m
jplot: lib5620.a driver.c blit.c.a(jcom.h)
	ar x blit.c.a jcom.h
	cc -DT5620 driver.c lib5620.a -lm -o jplot
	rm jcom.h
lib5620.a: blit.c.a
	mkdir xplot
	cd xplot;ar x ../blit.c.a
	cd xplot;cc -c -O *.c
	cd xplot;ar rc lib5620.a *.o
	mv xplot/lib5620.a lib5620.a
	ranlib lib5620.a
	rm xplot/*
	rmdir xplot
jplot.m:	inblit.a
	mkdir yplot
	cd yplot;ar x ../inblit.a
	cd yplot;ar x ../blit.c.a jcom.h
	cd yplot;$(MCC) *.c -lmj -o jplot.m
	mv yplot/jplot.m jplot.m
	rm yplot/*
	rmdir yplot
install: jplot.m jplot
	cp jplot.m /usr/jerq/mbin/jplot.m
	cp jplot /usr/jerq/bin/jplot
clean:
	rm -fr *.o jplot.m jplot core xplot yplot
