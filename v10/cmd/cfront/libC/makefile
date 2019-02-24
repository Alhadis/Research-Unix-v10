CC = nCC

DIRS = bignum complex generic misc new task

libC.a:
	for i in $(DIRS); do (cd $$i; make CC=$(CC)) done
	cd iostream; make -f iostream.mk CC=$(CC); cd ..
	cd obj; ar x ../iostream/iostream.a; cd ..
	rm -f libC.a
	cd obj; ar qc ../libC.a *.o

install: libC.a
	cp libC.a /usr/lib/nlibC.a
	ranlib /usr/lib/nlibC.a

clean:
	cd iostream; make clean -f iostream.mk
	rm -f */*.[oi] libC.a obj/*
