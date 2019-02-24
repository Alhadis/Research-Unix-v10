#
#	ONE PROCESS BELLMAC-32 COMLD MAKEFILE
#
#	@(#) oneproc.mk: 1.15 9/23/83
#
# Specify the machine architecture on which ld runs:
#
#	AR16WR = DEC 11/45-70
#	AR32WR = VAX 11/780
#	AR32W  = 3B20, IBM370, ...
#
# Specify the version of UN*X beng used:
#
#	TS  :	BTL UN*X TS
#
#
BM=make
SGS=m32
CC=cc
CFLAGS=-O -c
ARCH=AR32WR
SYS=TS
NPROC=ONEPROC
FFLAG=
SDP=NOSDP
ARFORMAT=PORTAR
DBO=FBO
FLEX = -DFLEXNAMES
DEFLIST= -D$(ARCH) -D$(NPROC) -D$(SDP) -D$(ARFORMAT) -D$(DBO) $(FLEX)
INCLIST= -I. -I$(COMLD) -I$(M32INC) -I$(COMINC) -I$(N3BINC) -I$(USRINC)
ENV=
CC_CMD=$(CC) $(CFLAGS) $(DEFLIST) $(INCLIST) $(ENV)
#
LD=$(CC)
LDFLAGS= $(FFLAG)
LD_CMD=$(LD) $(LDFLAGS)
#
YACC=yacc
YFLAGS=-d
YACC_CMD=$(YACC) $(YFLAGS)
#
LEX_CMD=lex
#
STRIP=strip
SPFLAGS=
STRIP_CMD=$(STRIP) $(SPFLAGS)
#
LINT=lint
LINTFLAGS=-p
LINT_CMD=$(LINT) $(LINTFLAGS)
#
#	External Directories
#
ROOT=
VERSION=
USRINC= $(ROOT)/usr/$(VERSION)include
#
#	Internal Directories
#
BASE= ../..
BINDIR= $(BASE)/xenv/m32/bin
LIBDIR= $(BASE)/xenv/m32/lib
M32INC= $(BASE)/inc/m32
COMLD= $(BASE)/ld/common
COMINC= $(BASE)/inc/common
N3BINC= $(BASE)/inc/3b
#
LIBLD	= $(BASE)/libld/m32/libld.a
LIBES= -ll -ly 
LIBS=
#
#
# Specify the *.c files making up ld:
#
CFILES=$(COMLD)/alloc.c $(COMLD)/dump.c $(COMLD)/expr0.c $(COMLD)/expr1.c \
	$(COMLD)/globs.c $(COMLD)/instr.c \
	$(COMLD)/ld00.c $(COMLD)/ld01.c $(COMLD)/ld1.c $(COMLD)/ld2.c \
	$(COMLD)/lists.c $(COMLD)/output.c $(COMLD)/sdp.c $(COMLD)/syms.c \
	$(COMLD)/slotvec.c $(COMLD)/tv.c $(COMLD)/util.c \
	$(COMLD)/version.c \
	lex.yy.c $(COMLD)/maps.c y.tab.c sglobs.c special1.c special2.c specsyms.c
#
#
# Specify the *.h files making up ld:
#
HFILES1=y.tab.h params.h system.h \
	$(COMLD)/ldmacros.h $(COMLD)/attributes.h $(COMLD)/list.h \
	$(COMLD)/structs.h $(COMLD)/extrns.h $(COMLD)/ldtv.h \
	$(COMINC)/aouthdr.h $(COMLD)/slotvec.h $(COMLD)/sdp1.h \
	$(COMLD)/instr.h
HFILES2=$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
	$(COMINC)/storclass.h $(COMINC)/bool.h \
	$(COMINC)/sgsmacros.h $(COMINC)/ldfcn.h $(COMINC)/reloc.h \
	$(COMINC)/linenum.h $(COMINC)/patch.h
HFILES3=$(M32INC)/sgs.h $(M32INC)/paths.h $(M32INC)/tv.h $(N3BINC)/magic.h
#HFILES4=$(COMINC)/sdpsrc/hd/define2.h $(COMINC)/sdpsrc/hd/define3.h \
#	$(COMINC)/sdpsrc/hd/define4.h $(COMINC)/sdpsrc/hd/struct.h \
#	$(COMINC)/sdpsrc/hd/globals.h $(COMINC)/sdpsrc/lo/delist.c \
#	$(COMINC)/sdpsrc/lo/enlist.c $(COMINC)/sdpsrc/lo/fatal.c \
#	$(COMINC)/sdpsrc/lo/getnxtpage.c $(COMINC)/sdpsrc/lo/readpage.c \
#	$(COMINC)/sdpsrc/lo/sojourn.c $(COMINC)/sdpsrc/lo/rpartition.c \
#	$(COMINC)/sdpsrc/hi/flush.c \
#	$(COMINC)/sdpsrc/hi/init.c $(COMINC)/sdpsrc/hi/lock.c \
#	$(COMINC)/sdpsrc/hi/unlock.c
#
#
# Specify the *.o files making up ld:
#
OBJECTS=expr0.o globs.o sglobs.o ld00.o lex.yy.o lists.o \
	util.o version.o y.tab.o special1.o special2.o \
	alloc.o specsyms.o dump.o expr1.o ld01.o ld1.o ld2.o maps.o \
	output.o sdp.o slotvec.o syms.o tv.o instr.o
#
#
# Make ld:
#
all:		ld
#
ld:		$(OBJECTS) $(LIBLD)
	$(LD_CMD) -o ld $(OBJECTS) $(LIBES) $(LIBLD) $(LIBS)
#
#
# Make the *.c:
#
y.tab.c y.tab.h:	$(COMLD)/ld.yac
	@echo "Expect to see message 'conflicts: 6 shift/reduce'";
	$(YACC_CMD) $(COMLD)/ld.yac
#
y.tab.o:	$(COMLD)/attributes.h $(COMLD)/list.h $(COMLD)/structs.h \
		$(COMLD)/ldmacros.h $(COMLD)/extrns.h $(COMLD)/ldtv.h system.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/storclass.h $(COMINC)/bool.h $(COMINC)/sgsmacros.h \
		$(M32INC)/sgs.h $(M32INC)/tv.h \
		params.h \
		y.tab.c
	$(CC_CMD) y.tab.c
#
lex.yy.c:	$(COMLD)/ld.lex
	$(LEX_CMD) $(COMLD)/ld.lex
#
lex.yy.o:	$(COMLD)/structs.h $(COMLD)/extrns.h system.h \
		$(COMLD)/ldtv.h \
		$(COMLD)/instr.h $(COMINC)/aouthdr.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/storclass.h $(COMINC)/bool.h \
		$(M32INC)/paths.h $(M32INC)/sgs.h $(M32INC)/tv.h \
		y.tab.h \
		lex.yy.c
	$(CC_CMD) lex.yy.c
#
expr0.o:	$(COMLD)/extrns.h $(COMLD)/structs.h system.h \
		$(COMLD)/ldmacros.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/storclass.h $(COMINC)/sgsmacros.h \
		y.tab.h \
		$(COMLD)/expr0.c
	$(CC_CMD) $(COMLD)/expr0.c
#
expr1.o:	$(COMLD)/extrns.h $(COMLD)/structs.h system.h \
		$(COMLD)/ldmacros.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/storclass.h $(COMINC)/sgsmacros.h \
		y.tab.h \
		$(COMLD)/expr1.c
	$(CC_CMD) $(COMLD)/expr1.c
#
ld00.o:		$(COMLD)/extrns.h $(COMLD)/list.h $(COMLD)/structs.h \
		system.h $(COMINC)/ldfcn.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/storclass.h $(COMINC)/sgsmacros.h \
		$(M32INC)/paths.h $(M32INC)/sgs.h \
		params.h y.tab.h \
		$(COMLD)/ld00.c
	$(CC_CMD) $(COMLD)/ld00.c
#
ld01.o:		$(COMLD)/extrns.h $(COMLD)/structs.h $(COMLD)/list.h \
		$(COMLD)/ldmacros.h system.h $(COMLD)/ldtv.h $(COMLD)/instr.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/storclass.h $(COMINC)/sgsmacros.h \
		$(M32INC)/tv.h \
		$(COMLD)/ld01.c
	$(CC_CMD) $(COMLD)/ld01.c
#
ld1.o:		$(COMLD)/extrns.h $(COMLD)/list.h $(COMLD)/ldtv.h \
		$(COMLD)/ldmacros.h \
		$(COMLD)/structs.h system.h $(COMINC)/aouthdr.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/storclass.h $(COMINC)/ldfcn.h $(COMINC)/reloc.h \
		$(COMINC)/sgsmacros.h $(COMINC)/sdpsrc/hd/define2.h \
		$(M32INC)/sgs.h $(M32INC)/tv.h $(N3BINC)/magic.h \
		params.h \
		$(COMLD)/ld1.c
	$(CC_CMD) $(COMLD)/ld1.c
#
ld2.o:		$(COMLD)/ldtv.h $(COMLD)/extrns.h $(COMLD)/list.h \
		$(COMLD)/structs.h system.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/sgsmacros.h $(COMINC)/storclass.h \
		$(COMINC)/reloc.h $(COMINC)/linenum.h $(COMINC)/patch.h \
		$(M32INC)/tv.h \
		y.tab.h params.h \
		$(COMLD)/ld2.c
	$(CC_CMD) $(COMLD)/ld2.c
#
alloc.o:	$(COMLD)/extrns.h $(COMLD)/list.h $(COMLD)/structs.h \
		$(COMLD)/ldmacros.h system.h $(COMLD)/ldtv.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/storclass.h $(COMINC)/sgsmacros.h \
		$(COMINC)/sdpsrc/hd/define2.h \
		$(M32INC)/tv.h \
		params.h \
		$(COMLD)/alloc.c
	$(CC_CMD) $(COMLD)/alloc.c
#
output.o:	$(COMLD)/structs.h $(COMLD)/extrns.h system.h \
		$(COMLD)/ldtv.h $(COMLD)/slotvec.h $(COMINC)/aouthdr.h \
		$(COMLD)/ldmacros.h $(COMLD)/instr.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/sgsmacros.h $(COMINC)/storclass.h \
		$(COMINC)/reloc.h $(COMINC)/linenum.h $(COMINC)/ldfcn.h \
		$(COMINC)/patch.h \
		$(M32INC)/sgs.h $(M32INC)/tv.h \
		params.h \
		$(COMLD)/output.c
	$(CC_CMD) $(COMLD)/output.c
#
maps.o:		$(COMLD)/extrns.h $(COMLD)/structs.h $(COMLD)/attributes.h \
		system.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/storclass.h $(COMINC)/sgsmacros.h \
		$(COMLD)/maps.c
	$(CC_CMD) $(COMLD)/maps.c
#
util.o:		$(COMLD)/structs.h $(COMLD)/extrns.h system.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/storclass.h $(COMINC)/sgsmacros.h \
		$(M32INC)/sgs.h \
		$(COMLD)/util.c
	$(CC_CMD) $(COMLD)/util.c
#
lists.o: 	$(COMLD)/list.h $(COMLD)/structs.h system.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/sgsmacros.h $(COMINC)/storclass.h \
		$(COMLD)/lists.c
	$(CC_CMD) $(COMLD)/lists.c
#
dump.o:		$(COMLD)/structs.h $(COMLD)/extrns.h system.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/storclass.h \
		$(COMLD)/dump.c
	$(CC_CMD) $(COMLD)/dump.c
#
syms.o:		$(COMLD)/structs.h $(COMLD)/extrns.h $(COMLD)/ldtv.h \
		system.h $(COMLD)/sdp1.h $(COMLD)/instr.h \
		$(COMLD)/ldmacros.h $(COMINC)/ldfcn.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/storclass.h $(COMINC)/sgsmacros.h \
		$(COMINC)/sdpsrc/hd/define2.h $(COMINC)/sdpsrc/hd/define4.h \
		$(COMINC)/sdpsrc/hd/struct.h $(M32INC)/paths.h $(M32INC)/tv.h \
		$(COMLD)/syms.c
	$(CC_CMD) $(COMLD)/syms.c
#
sdp.o:		$(COMLD)/sdp1.h system.h $(COMLD)/structs.h \
		$(COMLD)/instr.h $(COMLD)/extrns.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/storclass.h \
		$(COMINC)/sdpsrc/hd/define2.h $(COMINC)/sdpsrc/hd/define3.h \
		$(COMINC)/sdpsrc/hd/define4.h $(COMINC)/sdpsrc/hd/struct.h \
		$(COMINC)/sdpsrc/hd/globals.h $(COMINC)/sdpsrc/lo/delist.c \
		$(COMINC)/sdpsrc/lo/enlist.c $(COMINC)/sdpsrc/lo/fatal.c \
		$(COMINC)/sdpsrc/lo/getnxtpage.c \
		$(COMINC)/sdpsrc/lo/readpage.c \
		$(COMINC)/sdpsrc/lo/writeframe.c \
		$(COMINC)/sdpsrc/lo/sojourn.c \
		$(COMINC)/sdpsrc/lo/rpartition.c \
		$(COMINC)/sdpsrc/hi/allocate.c $(COMINC)/sdpsrc/hi/flush.c \
		$(COMINC)/sdpsrc/hi/init.c $(COMINC)/sdpsrc/hi/lock.c \
		$(COMINC)/sdpsrc/hi/unlock.c \
		$(M32INC)/paths.h \
		$(COMLD)/sdp.c
	$(CC_CMD) $(COMLD)/sdp.c
#
tv.o:		$(COMLD)/extrns.h $(COMLD)/structs.h $(COMLD)/ldtv.h \
		$(COMLD)/ldmacros.h system.h $(COMLD)/list.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/sdpsrc/hd/define2.h $(COMINC)/bool.h $(COMINC)/storclass.h \
		$(COMINC)/reloc.h $(COMINC)/ldfcn.h $(COMINC)/sgsmacros.h \
		$(M32INC)/tv.h \
		$(COMLD)/tv.c
	$(CC_CMD) $(COMLD)/tv.c
#
globs.o:	$(COMLD)/structs.h $(COMLD)/ldtv.h system.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/storclass.h \
		$(M32INC)/paths.h $(M32INC)/tv.h \
		params.h \
		$(COMLD)/globs.c
	$(CC_CMD) $(COMLD)/globs.c
#
instr.o:	$(COMLD)/instr.c
	$(CC_CMD) $(COMLD)/instr.c
#
slotvec.o:	$(COMLD)/slotvec.h $(COMLD)/structs.h $(COMLD)/extrns.h \
		$(COMLD)/sdp1.h system.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h \
		$(COMINC)/storclass.h $(COMINC)/sdpsrc/hd/define2.h \
		$(COMINC)/sdpsrc/hd/define4.h \
       		$(M32INC)/paths.h \
		$(COMLD)/slotvec.c
	$(CC_CMD) $(COMLD)/slotvec.c
#
version.o:	$(COMLD)/version.c
	$(CC_CMD) $(COMLD)/version.c
#
sglobs.o:	$(COMLD)/structs.h $(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h $(COMINC)/storclass.h \
		$(M32INC)/sgs.h $(M32INC)/tv.h $(COMLD)/ldtv.h \
		system.h sglobs.c
	$(CC_CMD) sglobs.c
#
special1.o:	system.h special1.c $(COMLD)/structs.h $(COMLD)/extrns.h \
		$(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h $(COMINC)/storclass.h
	$(CC_CMD) special1.c
#
special2.o:	$(COMINC)/aouthdr.h $(COMLD)/structs.h $(COMINC)/filehdr.h \
		$(COMINC)/scnhdr.h $(COMINC)/syms.h $(COMINC)/storclass.h $(COMINC)/ldfcn.h \
		$(M32INC)/tv.h $(COMLD)/ldtv.h $(COMLD)/slotvec.h \
		$(COMINC)/reloc.h $(COMLD)/extrns.h $(COMLD)/list.h \
		$(M32INC)/sgs.h $(N3BINC)/magic.h  $(COMINC)/sgsmacros.h \
		system.h special2.c
	$(CC_CMD) special2.c
#
specsyms.o:	system.h $(COMLD)/extrns.h $(COMINC)/sgsmacros.h \
		$(COMLD)/structs.h $(COMINC)/filehdr.h $(COMINC)/scnhdr.h $(COMINC)/syms.h $(COMINC)/storclass.h \
		$(M32INC)/tv.h $(COMLD)/ldtv.h \
		specsyms.c
	$(CC_CMD) specsyms.c
#
#
#
# File and administrative commands:
#
install:	$(BINDIR)/$(SGS)ld
#
$(BINDIR)/$(SGS)ld:	ld
	-rm -f $(BINDIR)/$(SGS)ld
	cp ld  $(BINDIR)/$(SGS)ld
	-$(STRIP_CMD) $(BINDIR)/$(SGS)ld
#
save:	$(BINDIR)/$(SGS)ld
	-rm -f $(BINDIR)/$(SGS)ld.back
	cp $(BINDIR)/$(SGS)ld   $(BINDIR)/$(SGS)ld.back
#
uninstall:	$(BINDIR)/$(SGS)ld.back
	-rm -f $(BINDIR)/$(SGS)ld
	cp  $(BINDIR)/$(SGS)ld.back    $(BINDIR)/$(SGS)ld
#
shrink:
	-rm -f $(OBJECTS) y.tab.c lex.yy.c y.tab.h ld
#
# Run lint on the *.c files:
#
lint:		$(CFILES) $(HFILES1) $(HFILES2) $(HFILES3) $(HFILES4)
	$(LINT_CMD) $(DEFLIST) $(INCLIST) $(CFILES)
