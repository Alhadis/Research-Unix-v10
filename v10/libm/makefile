#
# libm is now a dummy for those who still want to say -lm
#
libm.a: dummy.o
	rm -f libm.a
	ar rc libm.a dummy.o

install: libm.a
	cp libm.a /usr/lib
	ranlib /usr/lib/libm.a

clean:
	rm -f libm.a
