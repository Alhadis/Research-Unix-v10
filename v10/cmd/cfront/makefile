all:
install:
	cd cfront; make install
	CC -O -o nmunch munch.c
	strip nmunch
	mv nmunch /usr/lib/munch
	cd libC; make install

clean:
	rm -f munch.[oi] nmunch
	cd cfront; make clean
	cd libC; make clean
