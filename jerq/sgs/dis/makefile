ARCH	= AR32WR
ARFORMAT= PORTAR
FLEX	= -DFLEXNAMES
DBO	= FBO
CFLAGS	= -D$(ARCH) -DM32 -D$(DBO) -D$(ARFORMAT) $(FLEX) -I$(INC) -I.
INC	= ../inc
LIBLD	= ../libld/libld.a
OBJECTS	= dis_main.o dis_utls.o bits.o tables.o dis_extn.o lists.o

dis:	$(OBJECTS) $(LIBLD)
	$(CC) -o dis $(OBJECTS) $(LIBLD)

$(OBJECTS):	dis.h $(INC)/filehdr.h $(INC)/ldfcn.h $(INC)/scnhdr.h
dis_extn.o:	$(INC)/linenum.h $(INC)/syms.h $(INC)/storclass.h structs.h
dis_main.o:	$(INC)/paths.h $(INC)/sgs.h $(INC)/syms.h\
		$(INC)/storclass.h $(INC)/linenum.h structs.h
dis_utls.o:	$(INC)/syms.h $(INC)/linenum.h $(INC)/storclass.h \
		$(INC)/sgs.h structs.h
lists.o:	structs.h $(INC)/linenum.h \
		$(INC)/syms.h $(INC)/storclass.h $(INC)/paths.h $(INC)/sgs.h
bits.o:		$(INC)/sgs.h

install:	dis
	cp dis /usr/jerq/bin/m32/dis
	strip /usr/jerq/bin/m32/dis
clean:
	-rm -f $(OBJECTS) dis core
