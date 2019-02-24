DEBUG= .
PCC2COM= ../common

CC=	cc
LINT=	lint
YACC=	/usr/bin/yacc
LFLAGS=	-p
CFLAGS=-g
YFLAGS=-d -D
INCLIST=	-I. -I$(PCC2COM)
DEFLIST=
LDFLAGS=
CCOMNAME=comp
TARGET=-DVAX
CC_CMD=	$(CC) -c $(CFLAGS) $(INCLIST) $(DEFLIST) $(TARGET)
YACC_CMD=	$(YACC) $(YFLAGS)

# use M as a short-hand for the awkward PCC2COM
# (M for Machine independent part)
# similarly, use D for DEBUG

M=	$(PCC2COM)
D=	$(DEBUG)

OFILES= cgram.o xdefs.o scan.o pftn.o trees.o optim.o local.o reader.o\
	debug.o common1.o \
	pjw.o gencode.o genaux.o printx.o lookup.o lcatch2.o catch2.o \
	genmore.o
CFILES=	cgram.c $M/xdefs.c $M/scan.c $M/pftn.c \
	$M/trees.c $M/optim.c local.c $M/reader.c \
	$D/debug.c local2.c $M/allo.c $M/pjw.c \
	gencode.c genaux.c genmore.c
POFILES= cgram.o xdefs.o scan.o pftn.o trees.o optim.o local.o reader.o\
	local2.o debug.o common1.o \
	memcpy.o pjw.o newcode.o printx.o lookup.o lcatch2.o catch2.o \
	t2print.o
build:	$(CCOMNAME)

install:
	cp /lib/ccom comp.sv
	cp comp /lib/ccom
	strip /lib/ccom

vcc:	vcc.c
	$(CC) -o Nvcc vcc.c
	mv Nvcc vcc
#-------------------------

$(CCOMNAME):	$(OFILES)
		time $(CC) $(CFLAGS) $(LDFLAGS) -o $(CCOMNAME) $(OFILES)

pjw.o:		$M/pjw.c
		$(CC_CMD) $M/pjw.c
gencode.o:	$M/mfile2.h gencode.c gencode.h
		$(CC_CMD) gencode.c
genaux.o:	$M/mfile2.h genaux.c gencode.h
		$(CC_CMD) genaux.c
genmore.o:	$M/mfile2.h genmore.c gencode.h
		$(CC_CMD) genmore.c
catch2.o:	$M/mfile2.h $M/catch2.c
		$(CC_CMD) $M/catch2.c
lcatch2.o:	$M/mfile2.h lcatch2.c
		$(CC_CMD) lcatch2.c
t2print.o:	$M/mfile2.h $M/t2print.c
		$(CC_CMD) $M/t2print.c
printx.o:	printx.c
		$(CC_CMD) printx.c
trees.o:	$M/manifest.h macdefs.h $M/mfile1.h $M/trees.c
		$(CC_CMD) $M/trees.c
lookup.o:	$M/manifest.h macdefs.h $M/mfile1.h $M/lookup.c
		$(CC_CMD) $M/lookup.c

optim.o:	$M/manifest.h macdefs.h $M/mfile1.h $M/optim.c
		$(CC_CMD) $M/optim.c

pftn.o:		$M/manifest.h macdefs.h $M/mfile1.h $M/pftn.c
		$(CC_CMD) $M/pftn.c

local.o:	$M/manifest.h macdefs.h $M/mfile1.h 
		$(CC_CMD) local.c

scan.o:		$M/manifest.h macdefs.h $M/mfile1.h $M/scan.c
		$(CC_CMD) $M/scan.c

common1.o:	$M/manifest.h $M/mfile1.h macdefs.h dope.h $M/common1.c
		$(CC_CMD) $M/common1.c

xdefs.o:	$M/manifest.h $M/mfile1.h macdefs.h $M/xdefs.c
		$(CC_CMD) $M/xdefs.c

cgram.o:	$M/manifest.h $M/mfile1.h macdefs.h cgram.c
		cp y.debug.sv y.debug
		$(CC_CMD) -DYYDEBUG -DYYMAXDEPTH=300 cgram.c

cgram.c:	$M/cgram.y
		-@echo "Expect 6 shift/reduce conflicts"
		$(YACC_CMD) $M/cgram.y
		sed 's_^# line .*_/* & */_' y.tab.c >cgram.c
		rm y.tab.c

cdgen.o:	$M/manifest.h $M/mfile2.h macdefs.h $M/cdgen.c
		$(CC_CMD) $M/cdgen.c

reader.o:	$M/manifest.h $M/mfile2.h macdefs.h reader.c
		$(CC_CMD) reader.c

local2.o:	$M/manifest.h $M/mfile2.h macdefs.h
		$(CC_CMD) local2.c

debug.o:	$M/manifest.h $M/mfile1.h macdefs.h $D/debug.c $D/debug.h 
		$(CC_CMD) $D/debug.c

allo.o:		$M/manifest.h $M/mfile2.h macdefs.h $M/allo.c
		$(CC_CMD) $M/allo.c

#--------------------------

lint: $(CFILES)
	$(LINT) $(INCLIST) $(LFLAGS) $(CFILES)

#--------------------------

clean:
	-rm -f $(OFILES) cgram.c core nohup.out stout mout lout

#--------------------------

clobber:	clean
		-rm -f $(CCOMNAME) table.c sty
