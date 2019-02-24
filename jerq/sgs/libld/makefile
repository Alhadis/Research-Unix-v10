#		OLDAR	= old format
#		PORT5AR	= 5.0 random access format
#		PORTAR	= 6.0 portable ascii format
ARFORMAT= PORTAR
ARCH	= AR32WR
FLEX	= -DFLEXNAMES
#
#	Compilation Parameters
#
INC	= ../inc
CFLAGS	= -D$(ARFORMAT) -D$(ARCH) $(FLEX) -I. -I$(INC)
#
#	Internal Directories
#
OBJECTS = ldaclose.o ldaopen.o ldclose.o ldahread.o ldlseek.o ldnlseek.o \
	ldnrseek.o ldnsseek.o ldnshread.o ldopen.o ldrseek.o ldlread.o \
	ldsseek.o ldshread.o ldfhread.o ldtbseek.o ldohseek.o ldtbread.o \
	ldtbindex.o ldgetname.o allocldptr.o freeldptr.o vldldptr.o \
	sgetl.o sputl.o

libld.a: $(OBJECTS)
	rm -f libld.a
	$(AR) r libld.a $(OBJECTS)
	ranlib libld.a
#
$(OBJECTS):	$(INC)/filehdr.h $(INC)/ldfcn.h
allocldptr.o freeldptr.o vldldptr.o:	lddef.h
ldlseek.o ldnlseek.o ldnrseek.o ldnsseek.o ldnshread.o ldrseek.o:  $(INC)/scnhdr.h
ldlread.o ldsseek.o ldshread.o:	$(INC)/scnhdr.h
ldgetname.o ldtbread.o ldtbindex.o ldlread.o:	$(INC)/syms.h $(INC)/storclass.h
ldlread.o:	$(INC)/linenum.h

install:	libld.a

clean:
	rm -f $(OBJECTS)

clobber:	clean
	rm -f libld.a

llib-lld.ln:	llib-lld.c
	$(LINT) llib-lld.c -c
