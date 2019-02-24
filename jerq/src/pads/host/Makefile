OFILES=cache.o pad.o term.o menu.o remote.o sf.o padrcv.o stats.o help.o\
	libC.o trace.o popen.o extern.o

libpads.a: $(OFILES)
	ar ruv libpads.a $(OFILES)
	ranlib libpads.a

trace.o: trace.c
	cc -cg `pwd`/trace.c

%.o:	%.c ../pads.pri
	CC  -I.. -I/usr/include -cg `pwd`/$*.c

install: libpads.a
	cp libpads.a /usr/jerq/lib
	ranlib /usr/jerq/lib/libpads.a

clean:
	rm -f *.i *junk* core jim.recover

a.out:	test.o libpads.a
	cc test.o libpads.a
