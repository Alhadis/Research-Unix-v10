DESTDIR=/usr/lib
CFLAGS=
OBJS= pow.o gcd.o msqrt.o mdiv.o mout.o mult.o madd.o util.o halloc.o 

libmp.a: $(OBJS)
	ar cr libmp.a $(OBJS)

install: libmp.a
	cp libmp.a ${DESTDIR}
	ranlib ${DESTDIR}/libmp.a

clean:
	rm -f *.o libmp.a
