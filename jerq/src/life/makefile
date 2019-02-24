all:	hostlife life.m

hostlife:	hostlife.c
	cc -o hostlife hostlife.c

life.m:	mlife.c
	3cc -o life.m mlife.c

install:	all
	cp life.m /usr/jerq/mbin
	cp hostlife /usr/jerq/lib
	cp life.sh /usr/jerq/bin/life
	strip /usr/jerq/lib/hostlife

clean:
	rm -f hostlife life.m *.o
