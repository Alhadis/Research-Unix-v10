
cb:	cb.o
	cc -o cb cb.o

cb.o:	cb.c cbtype.c cbtype.h

install:	cb
	strip cb
	mv cb /usr/bin

clean:
	rm -f cb *.o
