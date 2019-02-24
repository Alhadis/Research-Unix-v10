#ident	"@(#)ctrans:src/makefile	1.8"
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
YACC	=	yacc

#The ANSI_OUT option has been replaced by the +a1 and +a0 compilation options

#for systems which only recognize identifiers of up to 31 characters
#setting DENSE to -DDENSE causes cfront to generate internal names
#of 31 characters or less 
DENSE=

# The use of RETBUG is nolonger needed with version 2.0 and has been removed.
# RETBUG was used in 1.* Translator versions to circumvent a C compiler bug.

CCFLAGS=-O -I. -DFDOTRIGHT

# top level makefile modifies the following
MPMACRO	= PATCH
MPFILE	= patch

OFILES	=	alloc.o Bits.o block.o dcl.o dcl2.o dcl3.o dcl4.o \
		del.o discrim.o doprint.o error.o expand.o \
		expr.o expr2.o expr3.o find.o hash.o lalex.o lex.o main.o \
		norm.o norm2.o print.o print2.o print_self.o repr.o simpl.o \
		simpl2.o size.o table.o template.o tree_copy.o tree_dump.o \
		tree_walk.o typ.o typ2.o y.tab.o 

cfront:	$(OFILES)
	$(CC)	-o cfront $(CCFLAGS) $(OFILES)

$(OFILES):	cfront.h token.h typedef.h

y.tab.c:	gram.y 
	$(YACC) gram.y

alloc.o:	alloc.c
	$(CC) $(CCFLAGS) -c alloc.c 

Bits.o:	Bits.c Bits.h
	$(CC) $(CCFLAGS) -c Bits.c 

block.o:	block.c Block.h
	$(CC) $(CCFLAGS) -c block.c 

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

discrim.o:	discrim.c cfront.h
	$(CC) $(CCFLAGS) -c discrim.c

doprint.o:	doprint.c
	$(CC) $(CCFLAGS) -c doprint.c

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

hash.o: hash.c hash.h
	$(CC) $(CCFLAGS) -c hash.c 

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

print_self.o:   print_self.c
	$(CC) $(CCFLAGS) -c print_self.c 

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

template.o: template.c
	$(CC) $(CCFLAGS) -c template.c 

tree_copy.o: tree_copy.c
	$(CC) $(CCFLAGS) -c tree_copy.c 

tree_dump.o:    tree_dump.c cfront.h
	$(CC) $(CCFLAGS) -c tree_dump.c

tree_walk.o: tree_walk.c
	$(CC) $(CCFLAGS) -c tree_walk.c 

typ.o:	typ.c size.h
	$(CC) $(CCFLAGS) -c typ.c 

typ2.o:	typ2.c size.h
	$(CC) $(CCFLAGS) -c typ2.c 

y.tab.o:	y.tab.c size.h 
	$(CC) -DGRAM $(CCFLAGS) -c y.tab.c 

clean:
	rm -f *.o y.tab.c

clobber: clean
	rm -f cfront

cpio:	
	ls alloc.c Bits.c block.c dcl.c dcl2.c dcl3.c dcl4.c del.c discrim.c doprint.c \
		error.c expand.c expr.c expr2.c expr3.c find.c hash.c \
		lalex.c lex.c main.c norm.c norm2.c print.c print2.c \
		print_self.c repr.c simpl.c simpl2.c size.c table.c \
		template.c tree_copy.c tree_dump.c tree_walk.c typ.c typ2.c \
		gram.y \
		Bits.h Block.h \
		cfront.h token.h size.h tqueue.h typedef.h yystype.h \
		hash.h ios_printf.h node_classes.h print_self.h \
		template.h token_names.h tree_copy.h tree_dump.h tree_walk.h \
		makefile | cpio -oc > cfront.cpio

