#
# This makefile is rumored to work with 4.1BSD.
#

TESTDIR = .
YACCRM = rm -f
IFLAG =
INSDIR = /usr/bin

#	There is an internal rule (rules.c) which can make
#	and object file "markfile.o" from "markfile" if the
#	"markfile" has a what(1) string in it.
#	Disable the $(MARKFILE) macro if the internal: rule
#	doesn't work on your system.
MARKFILE =

#	Turn off the GETU variable if you system does not have the
#	getu system call as documented in the CB unix manual.
GETU	=

#	RULES is defined to be the Columbus rules. It can be defined to
#	be the pwb rules by typing:
#		make -f make.mk RULES=pwbrules
#
RULES	= rules

#	For PWB systems the PWB define symbol must be in the CFLAGS macro.
#	Thus for pwb systems:
#	    make -f make.mk CFLAGS="-O -DPWB" GETU= RULES=pwbrules LIBES=-lpw
#	will make a version of "make" for PWB systems.


OBJECTS =  \
	main.o \
	doname.o \
	misc.o \
	files.o \
	$(RULES).o \
	dosys.o \
	gram.o \
	dyndep.o \
	prtmem.o \
	$(MARKFILE)

CFLAGS = -O $(GETU)

all:  usgmake

usgmake:  $(OBJECTS)
	$(CC) -o usgmake $(OBJECTS)

gram.c:	gram.y

gram.o: gram.c

$(OBJECTS):  defs

install: all
	mv usgmake $(INSDIR)

clean:
	rm -f *.o usgmake gram.c
