CFLAGS=-O

libdbm.a: dbm.o
	mv dbm.o libdbm.a

dbm.o : dbm.h

install:	libdbm.a
	cp libdbm.a $(DESTDIR)/usr/lib
	cp dbm.h $(DESTDIR)/usr/include

clean:
	rm libdbm.a
