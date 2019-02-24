ramtek: libram.a driver.o
	cc driver.o libram.a -lm -o ramtek
libram.a: ramtek.c.a
	mkdir xplot
	cd xplot;ar x ../ramtek.c.a
	cd xplot;cc -c -O *.c
	cd xplot;ar rc libram.a *.o
	mv xplot/libram.a libram.a
	ranlib libram.a
	rm xplot/*
	rmdir xplot
install:
	cp libram.a /usr/lib/libram.a
	cp ramtek /usr/lib/ramtek
	cp ramtek.sh /usr/bin/ramtek
clean:
	rm libram.a ramtek
