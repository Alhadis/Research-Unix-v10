CFLAGS	= -D$(ARCH) -DTS $(FLEX) -I. -I$(INC) -I$(SDPINC)
ARCH	= AR32WR
FLEX	= -DFLEXNAMES
INC	= ../inc
SDPINC	= ../libsdp
LIBSDP	= ../libsdp/libsdp.a
LIBLD	= ../libld/libld.a
OBJECTS= tstc.o pass1.o mid.o pass2.o copy.o  

cprs:	$(OBJECTS) $(LIBLD) $(LIBSDP)
	$(CC) -o cprs $(OBJECTS) $(LIBLD) $(LIBSDP)
$(OBJECTS):	$(SDPINC)/sdp.h $(INC)/ldfcn.h
tstc.o:	$(INC)/filehdr.h $(INC)/storclass.h $(INC)/syms.h \
	sdp1.h
pass1.o:	$(INC)/filehdr.h $(INC)/storclass.h $(INC)/syms.h \
		tagitem.h pass1.c sdp1.h
mid.o:	$(INC)/storclass.h $(INC)/syms.h tagitem.h sdp1.h
pass2.o:	$(INC)/filehdr.h $(INC)/storclass.h $(INC)/syms.h \
		$(INC)/scnhdr.h $(INC)/linenum.h $(INC)/reloc.h \
		tagitem.h
copy.o:	$(INC)/filehdr.h $(INC)/scnhdr.h $(INC)/reloc.h \
	$(INC)/storclass.h $(INC)/syms.h $(INC)/linenum.h

install:	cprs
	cp cprs /usr/jerq/bin/cprs
	strip /usr/jerq/bin/cprs

clean:
	rm -f *.o core cprs
