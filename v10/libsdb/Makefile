libsdb.a:	dbxxx.s
	as dbxxx.s -o libsdb.a

install: libsdb.a
	cp libsdb.a ${DESTDIR}/usr/lib

clean:
	rm -f libsdb.a
