D=/usr/bin/primes
CFLAGS = -Od2

primes:	primes.o
	cc -o primes primes.o -lm

install:	primes
	cp primes $D
	strip $D

clean:
	rm -f primes *.o core a.out
