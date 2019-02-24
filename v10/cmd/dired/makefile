CFLAGS = -O

dired:	dired.o
	cc -o dired dired.o -ltermcap

clean:
	rm -f dired.o dired

install:	dired
	strip dired
	mv dired /usr/bin
	cp dired.help /usr/lib/dired
