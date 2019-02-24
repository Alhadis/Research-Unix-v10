#ident	"@(#)ctrans:src/makefile	1.3.7.20"
###############################################################################
#			Copyright (c) 1984 AT&T
#	  		  All Rights Reserved  	
#
#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	
#	The copyright notice above does not evidence any   	
#	actual or intended publication of such source code.
#
###############################################################################
CC	=	nCC

#The ANSI_OUT option has been replaced by the +a1 and +a0 compilation options

#for systems which only recognize identifiers of up to 31 characters
#setting DENSE to -DDENSE causes cfront to generate internal names
#of 31 characters or less 
DENSE=

# The use of RETBUG is nolonger needed with version 2.0 and has been removed.
# RETBUG was used in 1.* Translator versions to circumvent a C compiler bug.

CCFLAGS=

# top level makefile modifies the following
MPMACRO	= MUNCH
MPFILE	= munch

OFILES	=	alloc.o dcl.o dcl2.o dcl3.o dcl4.o del.o error.o expand.o \
		expr.o expr2.o expr3.o find.o lalex.o lex.o main.o \
		norm.o norm2.o print.o print2.o repr.o simpl.o simpl2.o \
		size.o table.o typ.o typ2.o y.tab.o malloc.o

cfront:	$(OFILES)
	$(CC)	-o cfront $(CCFLAGS) $(OFILES)

$(OFILES):	cfront.h token.h typedef.h

y.tab.c:	gram.y 
	yacc gram.y

alloc.o:	alloc.c
	$(CC) $(CCFLAGS) -c alloc.c 

dcl.o:	dcl.c size.h
	$(CC) $(CCFLAGS) -c dcl.c 

dcl2.o:	dcl2.c size.h
	$(CC) $(CCFLAGS) -c dcl2.c 

dcl3.o:	dcl3.c size.h
	$(CC) $(CCFLAGS) -c dcl3.c 

dcl4.o:	dcl4.c size.h
	$(CC) $(CCFLAGS) -c dcl4.c 

del.o:	del.c
	$(CC) $(CCFLAGS) -c del.c 

error.o:	error.c size.h
	$(CC) $(CCFLAGS) -c error.c 

expand.o:	expand.c
	$(CC) $(CCFLAGS) -c expand.c 

expr.o:	expr.c size.h
	$(CC) $(CCFLAGS) -c expr.c 

expr2.o:	expr2.c size.h
	$(CC) $(CCFLAGS) -c expr2.c 

expr3.o:	expr3.c size.h
	$(CC) $(CCFLAGS) -c expr3.c 

find.o:	find.c 
	$(CC) $(CCFLAGS) -c find.c 

lalex.o:lalex.c yystype.h tqueue.h 
	$(CC) $(CCFLAGS) -c lalex.c

lex.o:	lex.c size.h tqueue.h yystype.h
	$(CC) $(CCFLAGS) -c lex.c 

# main depends on this makefile to force recompilation
# when going from patch to munch
main.o:	main.c makefile
	$(CC) -D$(MPMACRO) $(DENSE) $(CCFLAGS) -c main.c 

norm.o:	norm.c size.h
	$(CC) $(CCFLAGS) -c norm.c 

norm2.o:	norm2.c size.h
	$(CC) $(CCFLAGS) -c norm2.c 

print.o:	print.c
	$(CC) $(CCFLAGS) -c print.c 

print2.o:	print2.c
	$(CC) $(DENSE) $(CCFLAGS) -c print2.c 

repr.o:	repr.c
	$(CC) $(CCFLAGS) -c repr.c 

simpl.o:	simpl.c size.h
	$(CC) $(CCFLAGS) -c simpl.c 

simpl2.o:	simpl2.c size.h
	$(CC) $(CCFLAGS) -c simpl2.c 

size.o:	size.c size.h
	$(CC) $(CCFLAGS) -c size.c 

table.o:	table.c
	$(CC) $(CCFLAGS) -c table.c 

typ.o:	typ.c size.h
	$(CC) $(CCFLAGS) -c typ.c 

typ2.o:	typ2.c size.h
	$(CC) $(CCFLAGS) -c typ2.c 

y.tab.o:	y.tab.c size.h 
	$(CC) -DGRAM $(CCFLAGS) -c y.tab.c 


malloc.o:	malloc.c
	cc $(CCFLAGS) -c malloc.c

cpio:	
	ls alloc.c dcl.c dcl2.c dcl3.c dcl4.c del.c error.c expand.c \
		expr.c expr2.c expr3.c find.c lalex.c lex.c main.c \
		norm.c norm2.c print.c print2.c repr.c simpl.c simpl2.c \
		size.c table.c typ.c typ2.c \
		gram.y \
		cfront.h token.h size.h tqueue.h typedef.h yystype.h \
		makefile | cpio -oc > cfront.cpio

