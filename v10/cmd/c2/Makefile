CFLAGS= -O -DCOPYCODE
FILES=c20.o c21.o c22.o

c2:	$(FILES)
	$(CC) $(LDFLAGS) -o c2 c2?.o

c20.o:	c20.c c2.h
c21.o:	c21.c c2.h
c22.o:  c22.c c2.h
	$(CC) $(CFLAGS) -cR c22.c

clean:
	rm -f $(FILES) c2

install:	c2
	strip c2
	cp c2 $(DESTDIR)/lib

test:
	rm -fr testdir
	mkdir testdir
	cp /lib/cpp /lib/ccom testdir
	make clean
	make c2
	cp c2 testdir/c2
	make clean
	make CFLAGS="${CFLAGS} -Btestdir/" c2
	cp c2 testdir/c2
	make clean
	make CFLAGS="${CFLAGS} -Btestdir/" c2
	strip c2 testdir/c2
	cmp c2 testdir/c2 && rm -fr c2 testdir
