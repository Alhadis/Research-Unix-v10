all:	apserver apnews remotestamp

apserver: apserver.c
	cc -g -o apserver apserver.c

remotestamp:	apserver
	cp apserver /usr/jerq/lib
	chmod 775 /usr/jerq/lib/apserver
	dest='alice research grigg' ship /usr/jerq/lib/apserver
	touch remotestamp

apnews:	apnews.o
	cc apnews.o -g -o apnews /usr/jerq/lib/libpads.a -lipc
	strip -s apnews

apnews.o: apnews.c
	CC -I/usr/jerq/include -c `pwd`/apnews.c
clean:
	rm -f *junk* *.i *.cppd core jim.recover apserver apnews remotestamp *.o

install: apnews
	cp apnews /usr/jerq/bin
	chmod 775 /usr/jerq/bin/apnews

