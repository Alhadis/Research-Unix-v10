CFLAGS = -Od2

a.out:	=.o
	cc =.o

install:	a.out
	rm -f /usr/bin/= /usr/bin/== /usr/bin/=p /usr/bin/==p
	strip a.out
	cp a.out /usr/bin/=
	ln /usr/bin/= /usr/bin/==
	ln /usr/bin/= /usr/bin/=p
	ln /usr/bin/= /usr/bin/==p

clean:
	rm -f a.out *.o
ship:
	ship /usr/bin/= /usr/bin/== /usr/bin/=p /usr/bin/==p
