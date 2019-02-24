CFLAGS	=-D$(ARCH) -D$(ARFORMAT) -DUNIX $(FLEX) -I$(INC) -I.
ARCH	=AR32WR
#	Set ARFORMAT to "PORTAR" for portable archive format (post 5.0)
#	Set ARFORMAT to OLDAR for pre-5.0 archive format
ARFORMAT=PORTAR
FLEX	= -DFLEXNAMES
INC	= ../inc
LIBLD	= ../libld/libld.a
OBJECTS= main.o setflags.o process.o checkflags.o buildtmp.o buildfcns.o \
	copytmp.o fcns.o

strip:	$(OBJECTS) $(LIBLD)
	$(CC) -o strip $(OBJECTS) $(LIBLD)

$(OBJECTS):	$(INC)/filehdr.h $(INC)/ldfcn.h $(INC)/sgs.h defs.h
main.o process.o:  $(INC)/paths.h
buildfcns.o:  $(INC)/scnhdr.h \
	      $(INC)/reloc.h $(INC)/syms.h \
	      $(INC)/storclass.h $(INC)/linenum.h

install:	strip
	cp strip /usr/jerq/bin/3strip
	mv strip xxx
	strip /usr/jerq/bin/3strip
	mv xxx strip

clean:
	rm -f *.o core strip xxx
