# Master makefile for creating:
#
#		C I P


# DMD 5620 and 630 SGS directories:
DMD5620=/usr/add-on/dmd2.0
DMD630=/usr/630

# C compiler -g flag needed for symbolic debugging but not for final products:
GFLAG=-g
#GFLAG=-O


all:  create dmd630

install: 
	cp cip $(DMD5620)/bin/cip
	cp cip $(DMD630)/bin/cip
	cp dmd5620/cip.m $(DMD5620)/lib/cip.m
	cp dmd630/cip.m $(DMD630)/lib/cip.m
	cd JPIC; make install


# Directory containing lib and include files:
DMDLIB=$(DMD)/lib
DMDINCLUDE=$(DMD)/include


# host programs and commands:
host:	cip Jpic

cip:	cip.sh
	cp cip.sh cip
	chmod +x cip

Jpic:
	cd JPIC; make all


# dmd programs:
dmd630:
	cd DMD630; \
	make -f ../makefile  DMD=$(DMD630) MCFLAGS="$(GFLAG) -DMPXTERM" cip.m

dmd5620:
	cd DMD5620; \
	make -f ../makefile  DMD=$(DMD5620) MCFLAGS="$(GFLAG) -DMPXTERM" cip.m


########
#
#	make objects in lower directories.
#
########

MCC=$(DMD)/bin/dmdcc

LCLCFLAGS=

OBJ=buttons.o cip.o commands.o draw.o edit.o  menus.o readPic.o \
	pic.o space.o things.o track.o transform.o tmenuhit.o

.c.o:	
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLCFLAGS) -c $<


cip.j:	$(OBJ)
	$(MCC) -J -o cip.j $(LCLCFLAGS) $(OBJ) \
	$(DMDLIB)/libsys.a $(DMDLIB)/libc.a

cip.m:	$(OBJ)
	$(MCC) -o cip.m $(LCLCFLAGS) $(OBJ)



########
#
#	CIP logo
#
#   Convert a series of lines in logo.pic to C data in logo.data.
#
########

../logo.data: ../logo.pic
	grep '^line  from' ../logo.pic | \
	sed -e 's/^line  from \([0-9][0-9]*\),\([0-9][0-9]*\) to \([0-9][0-9]*\),\([0-9][0-9]*\)/\1,\2,\3,\4,/' \
	> ../logo.data



########
#
#	dependencies and rules 
#
########

buttons.o:	../buttons.c
buttons.o:	../cip.h
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLCFLAGS) -c ../buttons.c

cip.o:	../cip.c
cip.o:	../cip.h
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLCFLAGS) -c ../cip.c

commands.o:	../commands.c
commands.o:	../cip.h
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLCFLAGS) -c ../commands.c

draw.o:	../draw.c
draw.o:	../cip.h
draw.o: ../logo.data
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLCFLAGS) -c ../draw.c

#dump.o: ../dump.c
#dump.o: ../cip.h
#	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLCFLAGS) -c ../dump.c

edit.o:	../edit.c
edit.o:	../cip.h
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLCFLAGS) -c ../edit.c

menus.o:	../menus.c
menus.o:	../cip.h
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLCFLAGS) -c ../menus.c

pic.o:	../pic.c
pic.o:	../cip.h
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLCFLAGS) -c ../pic.c

readPic.o:	../readPic.c
readPic.o:	../cip.h
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLCFLAGS) -c ../readPic.c

space.o:	../space.c
space.o:	../cip.h
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLCFLAGS) -c ../space.c

things.o:	../things.c
things.o:	../cip.h
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLCFLAGS) -c ../things.c

tmenuhit.o:	../tmenuhit.c
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLCFLAGS) -c ../tmenuhit.c

track.o:	../track.c
track.o:	../cip.h
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLCFLAGS) -c ../track.c

transform.o:	../transform.c
transform.o:	../cip.h
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLCFLAGS) -c ../transform.c





# create lower level directories needed to hold objects.
create:
	-mkdir DMD630
	-mkdir DMD5620


# run lint on source files.
LINTFLAGS=

lint:
	lint -DDMD630 -I$(DMDINCLUDE) $(LINTFLAGS) *.c > lint.out


# print a listing of source files.

print:  
	cpr -x -n -w 92 -l 70 makefile *.sh *.h *.c *.1 \
	| xerox -h 10 -l 70 -s vint -2 | opr -r -txr -2


# Create a cpio file of the source files.

cpio:
	find WISH.LIST makefile *.sh *.h *.c *.1 *.pic *.data \
	JPIC/*.h JPIC/*.y JPIC/*.l JPIC/*.c JPIC/makefile  -type f -print \
	| cpio -oc > cip.cpio
