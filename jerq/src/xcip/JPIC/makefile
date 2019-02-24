########
#
#	Jpic makefile
#
########
#
#	Standard Macros
#
########
AUX_CLEAN = 
#CC = $(SGS)cc -f
CC = $(SGS)cc -g
CFLAGS = -O
MAKE.LO = make.lo
MAKE.ROOT = $(ROOT)/make.root
MKGEN = mkgen
MV = mv
ROOT = ../../..
SGS = $(SGSX)
SGSX = 

INC_LIST	=\
	-I.

CLEAN =\
	.c.o\
	arcgen.o\
	blockgen.o\
	boxgen.o\
	circgen.o\
	linegen.o\
	main.o\
	misc.o\
	movegen.o\
	pic2.o\
	picl.c\
	picl.o\
	picy.c\
	picy.o\
	pltroff.o\
	print.o\
	symtab.o\
	textgen.o\
	troffgen.o

all: Jpic

##########
#
#	make.root
#
##########

DMDBIN=$(ROOT)/bin
DMDLIB=$(ROOT)/lib
DMDSRC=$(ROOT)/src
DMDINCLUDE=$(ROOT)/include
UINC=/usr/include
DMDDEMO=$(ROOT)/demo
#MCC=$(ROOT)/bin/dmdcc -f
MCC=$(ROOT)/bin/dmdcc -g
MLD=m32ld
MAR=ar
MLORDER=m32lorder

########
#
#	make.lo
#
########



TITLE = Jpic makefile
PRODUCTS =  Jpic

CLEAN= *.o Jpic lex.yy.c

MAKEARG= -$(MAKEFLAGS) DMDBIN=$(DMDBIN) DMDLIB=$(DMDLIB) DMDSRC=$(DMDSRC)\
	DMDINCLUDE=$(DMDINCLUDE) DMDSGS=$(DMDSGS) DMDDEMO=$(DMDDEMO)\
	DMDFONT=$(DMDFONT) DMDICON=$(DMDICON)\
	MCC=$(MCC) MCFLAGS=$(MCFLAGS) MLD=$(MLD) MAR=$(MAR)

YFLAGS = -d
LCLCFLAGS=-m

YACC_CMD=yacc $(YFLAGS)
LEX_CMD=lex
#MCC=cc -f
MCC=cc -g

OFILES = main.o print.o misc.o symtab.o blockgen.o boxgen.o circgen.o \
	arcgen.o linegen.o movegen.o troffgen.o textgen.o \
	pic2.o pltroff.o picy.o picl.o

.c.o:
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLFLAGS) -c $<

makefile:
	echo "no makefile built"

makefile_all: makefile


Jpic:	picy.o picl.o $(OFILES) 
	$(CC) $(CFLAGS) -o Jpic picy.o picl.o $(OFILES) -lm

save:
	-cp $(DMDBIN)/Jpic oJpic

install:	$(DMDBIN)/Jpic

$(DMDBIN)/Jpic:	Jpic
	cp Jpic $(DMDBIN)/Jpic

y.tab.h:	picy.o

########
#
#	All dependencies and rules not explicitly stated
#	(including header and nested header dependencies)
#
########

arcgen.o:	arcgen.c
arcgen.o:	pic.h
arcgen.o:	/usr/include/stdio.h
arcgen.o:	y.tab.h
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLFLAGS) -c arcgen.c

blockgen.o:	blockgen.c
blockgen.o:	pic.h
blockgen.o:	/usr/include/stdio.h
blockgen.o:	y.tab.h
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLFLAGS) -c blockgen.c

boxgen.o:	boxgen.c
boxgen.o:	pic.h
boxgen.o:	/usr/include/stdio.h
boxgen.o:	y.tab.h
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLFLAGS) -c boxgen.c

circgen.o:	circgen.c
circgen.o:	pic.h
circgen.o:	/usr/include/stdio.h
circgen.o:	y.tab.h
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLFLAGS) -c circgen.c

linegen.o:	linegen.c
linegen.o:	pic.h
linegen.o:	/usr/include/stdio.h
linegen.o:	y.tab.h
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLFLAGS) -c linegen.c

main.o:	main.c
main.o:	pic.h
main.o:	/usr/include/stdio.h
main.o:	y.tab.h
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLFLAGS) -c main.c

misc.o:	misc.c
misc.o:	pic.h
misc.o:	/usr/include/stdio.h
misc.o:	y.tab.h
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLFLAGS) -c misc.c

movegen.o:	movegen.c
movegen.o:	pic.h
movegen.o:	/usr/include/stdio.h
movegen.o:	y.tab.h
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLFLAGS) -c movegen.c

pic2.o:	/usr/include/ctype.h
pic2.o:	pic.h
pic2.o:	pic2.c
pic2.o:	/usr/include/stdio.h
pic2.o:	y.tab.h
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLFLAGS) -c pic2.c

picl.c:	picl.l
	$(LEX_CMD) -t picl.l > picl.c

picl.o:	/usr/include/ctype.h
picl.o:	pic.h
picl.o:	picl.c
picl.o:	/usr/include/stdio.h
picl.o:	y.tab.h
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLFLAGS) -c picl.c

picy.c:	picy.y
	$(YACC_CMD) picy.y;\
	$(MV) y.tab.c picy.c

picy.o:	picy.c
picy.o:	/usr/include/stdio.h
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLFLAGS) -c picy.c

pltroff.o:	/usr/include/math.h
pltroff.o:	pic.h
pltroff.o:	pltroff.c
pltroff.o:	/usr/include/stdio.h
pltroff.o:	/usr/include/values.h # nested include from math.h
pltroff.o:	y.tab.h
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLFLAGS) -c pltroff.c

print.o:	pic.h
print.o:	print.c
print.o:	/usr/include/stdio.h
print.o:	y.tab.h
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLFLAGS) -c print.c

symtab.o:	/usr/include/ctype.h
symtab.o:	pic.h
symtab.o:	/usr/include/stdio.h
symtab.o:	symtab.c
symtab.o:	y.tab.h
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLFLAGS) -c symtab.c

textgen.o:	pic.h
textgen.o:	/usr/include/stdio.h
textgen.o:	textgen.c
textgen.o:	y.tab.h
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLFLAGS) -c textgen.c

troffgen.o:	pic.h
troffgen.o:	/usr/include/stdio.h
troffgen.o:	troffgen.c
troffgen.o:	y.tab.h
	$(MCC) -I$(DMDINCLUDE) $(MCFLAGS) $(LCLFLAGS) -c troffgen.c

########
#
#	Standard Targets
#
#	all		builds all the products specified by PRODUCTS
#	clean		removes all temporary files (ex. installable object)
#	clobber		"cleans", and then removes $(PRODUCTS)
#	makefile	regenerates makefile
#	install		installs products; user defined in make.lo 
#
########

all:		$(PRODUCTS)

clean:
		rm -f $(CLEAN) $(AUX_CLEAN)

clobber:	clean
		rm -f $(PRODUCTS)

makefile:	$(MAKE.LO) $(MAKE.ROOT)
		$(MKGEN) >make.out
		if [ -s make.out ]; then mv make.out makefile; fi

makefile_all:	makefile

install: 	# rules, if any, specified above

cpio:
	find *.h *.y *.l *.c makefile -type f -print | cpio -oc > jpic.cpio
