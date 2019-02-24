CFLAGS	= -I. -I$(INC) \
	  -D$(ARCH) -D$(NPROC) -D$(SDP) -D$(ARFORMAT) -D$(DBO) $(FLEX)
YFLAGS	= -d
ARCH	= AR32WR
NPROC	= ONEPROC
SDP	= NOSDP
ARFORMAT= PORTAR
DBO	= FBO
FLEX	= -DFLEXNAMES
INC	= ../inc
LIBLD	= ../libld/libld.a
OBJECTS=expr0.o globs.o sglobs.o ld00.o lex.yy.o lists.o \
	util.o version.o y.tab.o special1.o special2.o \
	alloc.o specsyms.o dump.o expr1.o ld01.o ld1.o ld2.o maps.o \
	output.o sdp.o slotvec.o syms.o tv.o instr.o

ld:	$(OBJECTS) $(LIBLD)
	$(CC) -o ld $(OBJECTS) -ll -ly $(LIBLD)

y.tab.c y.tab.h:	ld.yac
	@echo Expect to see 6 shift/reduce conflicts
	yacc $(YFLAGS) ld.yac

y.tab.o:	attributes.h list.h structs.h \
		ldmacros.h extrns.h ldtv.h system.h \
		$(INC)/filehdr.h $(INC)/scnhdr.h $(INC)/syms.h \
		$(INC)/storclass.h $(INC)/bool.h $(INC)/sgsmacros.h \
		$(INC)/sgs.h $(INC)/tv.h params.h
lex.yy.c:	ld.lex
		time lex ld.lex
lex.yy.o:	structs.h extrns.h system.h ldtv.h instr.h $(INC)/aouthdr.h \
		$(INC)/filehdr.h $(INC)/scnhdr.h $(INC)/syms.h \
		$(INC)/storclass.h $(INC)/bool.h \
		$(INC)/paths.h $(INC)/sgs.h $(INC)/tv.h y.tab.h
expr0.o:	extrns.h structs.h system.h ldmacros.h \
		$(INC)/filehdr.h $(INC)/scnhdr.h $(INC)/syms.h \
		$(INC)/storclass.h $(INC)/sgsmacros.h y.tab.h
expr1.o:	extrns.h structs.h system.h ldmacros.h \
		$(INC)/filehdr.h $(INC)/scnhdr.h $(INC)/syms.h \
		$(INC)/storclass.h $(INC)/sgsmacros.h y.tab.h
ld00.o:		extrns.h list.h structs.h \
		system.h $(INC)/ldfcn.h \
		$(INC)/filehdr.h $(INC)/scnhdr.h $(INC)/syms.h \
		$(INC)/storclass.h $(INC)/sgsmacros.h \
		$(INC)/paths.h $(INC)/sgs.h params.h y.tab.h
ld01.o:		extrns.h structs.h list.h \
		ldmacros.h system.h ldtv.h instr.h \
		$(INC)/filehdr.h $(INC)/scnhdr.h $(INC)/syms.h \
		$(INC)/storclass.h $(INC)/sgsmacros.h $(INC)/tv.h
ld1.o:		extrns.h list.h ldtv.h ldmacros.h \
		structs.h system.h $(INC)/aouthdr.h \
		$(INC)/filehdr.h $(INC)/scnhdr.h $(INC)/syms.h \
		$(INC)/storclass.h $(INC)/ldfcn.h $(INC)/reloc.h \
		$(INC)/sgsmacros.h $(INC)/sdpsrc/hd/define2.h \
		$(INC)/sgs.h $(INC)/tv.h $(INC)/magic.h \
		params.h
ld2.o:		ldtv.h extrns.h list.h structs.h system.h \
		$(INC)/filehdr.h $(INC)/scnhdr.h $(INC)/syms.h \
		$(INC)/sgsmacros.h $(INC)/storclass.h \
		$(INC)/reloc.h $(INC)/linenum.h $(INC)/patch.h \
		$(INC)/tv.h y.tab.h params.h
alloc.o:	extrns.h list.h structs.h ldmacros.h system.h ldtv.h \
		$(INC)/filehdr.h $(INC)/scnhdr.h $(INC)/syms.h \
		$(INC)/storclass.h $(INC)/sgsmacros.h \
		$(INC)/sdpsrc/hd/define2.h $(INC)/tv.h params.h
output.o:	structs.h extrns.h system.h \
		ldtv.h slotvec.h $(INC)/aouthdr.h ldmacros.h instr.h \
		$(INC)/filehdr.h $(INC)/scnhdr.h $(INC)/syms.h \
		$(INC)/sgsmacros.h $(INC)/storclass.h \
		$(INC)/reloc.h $(INC)/linenum.h $(INC)/ldfcn.h \
		$(INC)/patch.h $(INC)/sgs.h $(INC)/tv.h params.h 
maps.o:		extrns.h structs.h attributes.h system.h \
		$(INC)/filehdr.h $(INC)/scnhdr.h $(INC)/syms.h \
		$(INC)/storclass.h $(INC)/sgsmacros.h
util.o:		structs.h extrns.h system.h \
		$(INC)/filehdr.h $(INC)/scnhdr.h $(INC)/syms.h \
		$(INC)/storclass.h $(INC)/sgsmacros.h \
		$(INC)/sgs.h
lists.o: 	list.h structs.h system.h \
		$(INC)/filehdr.h $(INC)/scnhdr.h $(INC)/syms.h \
		$(INC)/sgsmacros.h $(INC)/storclass.h
dump.o:		structs.h extrns.h system.h \
		$(INC)/filehdr.h $(INC)/scnhdr.h $(INC)/syms.h \
		$(INC)/storclass.h
syms.o:		structs.h extrns.h ldtv.h system.h sdp1.h instr.h \
		ldmacros.h $(INC)/ldfcn.h \
		$(INC)/filehdr.h $(INC)/scnhdr.h $(INC)/syms.h \
		$(INC)/storclass.h $(INC)/sgsmacros.h \
		$(INC)/sdpsrc/hd/define2.h $(INC)/sdpsrc/hd/define4.h \
		$(INC)/sdpsrc/hd/struct.h $(INC)/paths.h $(INC)/tv.h
sdp.o:		sdp1.h system.h structs.h \
		instr.h extrns.h \
		$(INC)/filehdr.h $(INC)/scnhdr.h $(INC)/syms.h \
		$(INC)/storclass.h \
		$(INC)/sdpsrc/hd/define2.h $(INC)/sdpsrc/hd/define3.h \
		$(INC)/sdpsrc/hd/define4.h $(INC)/sdpsrc/hd/struct.h \
		$(INC)/sdpsrc/hd/globals.h $(INC)/sdpsrc/lo/delist.c \
		$(INC)/sdpsrc/lo/enlist.c $(INC)/sdpsrc/lo/fatal.c \
		$(INC)/sdpsrc/lo/getnxtpage.c $(INC)/sdpsrc/lo/readpage.c \
		$(INC)/sdpsrc/lo/writeframe.c $(INC)/sdpsrc/lo/sojourn.c \
		$(INC)/sdpsrc/lo/rpartition.c \
		$(INC)/sdpsrc/hi/allocate.c $(INC)/sdpsrc/hi/flush.c \
		$(INC)/sdpsrc/hi/init.c $(INC)/sdpsrc/hi/lock.c \
		$(INC)/sdpsrc/hi/unlock.c $(INC)/paths.h
tv.o:		extrns.h structs.h ldtv.h ldmacros.h system.h list.h \
		$(INC)/filehdr.h $(INC)/scnhdr.h $(INC)/syms.h \
		$(INC)/sdpsrc/hd/define2.h $(INC)/bool.h $(INC)/storclass.h \
		$(INC)/reloc.h $(INC)/ldfcn.h $(INC)/sgsmacros.h $(INC)/tv.h 
globs.o:	structs.h ldtv.h system.h \
		$(INC)/filehdr.h $(INC)/scnhdr.h $(INC)/syms.h \
		$(INC)/storclass.h $(INC)/paths.h $(INC)/tv.h params.h
slotvec.o:	slotvec.h structs.h extrns.h sdp1.h system.h \
		$(INC)/filehdr.h $(INC)/scnhdr.h $(INC)/syms.h \
		$(INC)/storclass.h $(INC)/sdpsrc/hd/define2.h \
		$(INC)/sdpsrc/hd/define4.h $(INC)/paths.h
sglobs.o:	structs.h $(INC)/filehdr.h $(INC)/scnhdr.h $(INC)/syms.h \
		$(INC)/storclass.h $(INC)/sgs.h $(INC)/tv.h ldtv.h system.h
special1.o:	system.h structs.h extrns.h \
		$(INC)/filehdr.h $(INC)/scnhdr.h $(INC)/syms.h $(INC)/storclass.h
special2.o:	$(INC)/aouthdr.h structs.h $(INC)/filehdr.h \
		$(INC)/scnhdr.h $(INC)/syms.h $(INC)/storclass.h $(INC)/ldfcn.h \
		$(INC)/tv.h ldtv.h slotvec.h $(INC)/reloc.h extrns.h list.h \
		$(INC)/sgs.h $(INC)/magic.h  $(INC)/sgsmacros.h system.h
specsyms.o:	system.h extrns.h $(INC)/sgsmacros.h \
		structs.h $(INC)/filehdr.h $(INC)/scnhdr.h $(INC)/syms.h \
		$(INC)/storclass.h $(INC)/tv.h ldtv.h

install:	ld
		cp ld /usr/jerq/bin/3ld
		strip /usr/jerq/bin/3ld

clean:
		rm -f $(OBJECTS) y.tab.c lex.yy.c y.tab.h ld
