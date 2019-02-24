CFLAGS	=-d2 -D$(ARCH) -D$(ARFORMAT) -DUNIX $(FLEX) -I$(INC) -I.
ARCH	=AR32WR
ARFORMAT=PORTAR
FLEX	= -DFLEXNAMES
INC	= ../inc
LIBLD	= ../libld/libld.a
OBJECTS = main.o process.o fcns.o
#
size:	$(OBJECTS) $(LIBLD)
	$(CC) -o size $(OBJECTS) $(LIBLD)
$(OBJECTS):	$(INC)/filehdr.h $(INC)/ldfcn.h $(INC)/sgs.h defs.h
process.o: $(INC)/scnhdr.h process.h
fcns.o:	$(INC)/scnhdr.h
install:	size
	cp size /usr/jerq/bin/3size
	strip /usr/jerq/bin/3size
clean:
	rm -f *.o core size
