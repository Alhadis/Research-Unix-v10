CFLAGS	= -O
LFLAGS	=
OBJS	= pcheck.o pinit.o precv.o psend.o ptimeout.o
CFILES	= pcheck.c pinit.c precv.c psend.c ptimeout.c
HEADERS = pconfig.h proto.h packets.h pstats.h

Plib.a:		$(OBJS)
		ar rv Plib.a $(OBJS)
		ranlib Plib.a

pinit.o:	$(HEADERS)
precv.o:	$(HEADERS)
psend.o:	$(HEADERS)
ptimeout.o:	$(HEADERS)

lint:		$(HEADERS) $(CFILES)
		lint $(LFLAGS) $(CFILES)

print:		man
		@(cat man; pr makefile *.h *.c)

man:		proto.3c
		nroff -man -Tlpr proto.3c >man

clean:	
		rm -f *
pp:
		pp -kjerq -tproto makefile *.h *.c | dcan
