OBJS=	getwd.o killpg.o setpgrp.o signal.o sigset.o wait3.o

libjobs.a: ${OBJS}
	ar cr libjobs.a ${OBJS}

install:	libjobs.a
	cp libjobs.a $(DESTDIR)/usr/lib
	ranlib $(DESTDIR)/usr/lib/libjobs.a

clean:
	rm libjobs.a *.o
