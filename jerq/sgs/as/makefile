YFLAGS	= -d
CFLAGS	= -g -I. -I$(INC) -I$(USRINC) \
		 -D$(ARCH) -D$(DBO) -DONEPROC -D$(PCCALL) $(FLEX) -D$(MAC)
MAC	= ABWORMAC
ARCH	= AR32WR
DBO	= FBO
PCCALL	= NOCALLPCREL
FLEX	= -DFLEXNAMES
INC	= ../inc
OFILES	= pass0.o parse.o code.o errors.o pass1.o strings.o instab.o gencode.o \
	  swagen.o expand1.o expand2.o float.o addr1.o addr2.o codeout.o \
	  getstab.o pass2.o obj.o symlist.o symbols.o

as:	$(OFILES)
	$(CC) -o as $(OFILES) -lm

parse.c parse.h: parse.y
		yacc $(YFLAGS) parse.y
		mv y.tab.c parse.c
		mv y.tab.h parse.h
pass0.o:	pass0.h $(INC)/paths.h $(INC)/sgs.h systems.h
parse.o:	symbols.h $(INC)/filehdr.h instab.h systems.h\
		gendefs.h $(INC)/storclass.h $(INC)/sgs.h
code.o:		symbols.h codeout.h gendefs.h systems.h
errors.o:	gendefs.h systems.h
pass1.o:	pass1.c $(INC)/paths.h symbols.h gendefs.h systems.h
strings.o:	gendefs.h systems.h
instab.o:	instab.h ops.out symbols.h parse.h systems.h
gencode.o:	symbols.h instab.h systems.h parse.h gendefs.h expand.h expand2.h
swagen.o:	symbols.h instab.h systems.h parse.h gendefs.h expand.h expand2.h
expand1.o:	expand.h symbols.h gendefs.h systems.h
expand2.o:	expand.h expand2.h symbols.h systems.h
float.o:	instab.h symbols.h systems.h 
addr1.o:	$(INC)/reloc.h $(INC)/syms.h \
		  $(INC)/storclass.h $(INC)/linenum.h $(INC)/filehdr.h \
		  gendefs.h symbols.h codeout.h systems.h
addr2.o:	$(INC)/reloc.h $(INC)/storclass.h systems.h \
		  $(INC)/syms.h gendefs.h symbols.h \
		  codeout.h instab.h
codeout.o:	symbols.h codeout.h gendefs.h systems.h
getstab.o:	gendefs.h symbols.h systems.h
pass2.o:	gendefs.h symbols.h systems.h
obj.o:		$(INC)/filehdr.h $(INC)/linenum.h instab.h \
		$(INC)/reloc.h $(INC)/scnhdr.h $(INC)/syms.h $(INC)/storclass.h \
		symbols.h codeout.h gendefs.h $(INC)/sgs.h systems.h
symlist.o:	symbols.h $(INC)/syms.h $(INC)/storclass.h gendefs.h systems.h
symbols.o:	symbols.c symbols.h symbols2.h systems.h

install:	as
		cp as /usr/jerq/bin/3as
		strip /usr/jerq/bin/3as
# I dunno where these go; they aren't documented for the 32000, only the 32100 -rob
$(LIBDIR)/cm4defs:	cm4defs
		-rm -f $(LIBDIR)/cm4defs
		cp cm4defs $(LIBDIR)/cm4defs
$(LIBDIR)/cm4tvdefs:	cm4tvdefs
		-rm -f $(LIBDIR)/cm4tvdefs
		cp cm4tvdefs $(LIBDIR)/cm4tvdefs

clean:
	-rm -f $(OFILES) parse.c parse.h y.output core as
