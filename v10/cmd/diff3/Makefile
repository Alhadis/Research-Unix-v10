CFLAGS = -Od2

diff3: diff3.o
	cc diff3.o -o diff3

install: diff3 diff3.sh
	strip diff3
	mv diff3 /usr/lib
	cp diff3.sh /usr/bin/diff3

clean:
	rm -f *.o diff3
