#	@(#)cflow.mk	1.7	83/08/19
#	3.0 SID #	1.2
FLG=
TESTDIR = .
CFLAGS =-O -DFLEXNAMES -DUNIX5
FILES = Makefile README cflow.sh dag.c lpfx.c nmf.c flip.c
ALL = dag lpfx nmf flip
LINT = ../lint
MIP = $(ROOT)/usr/src/cmd/cc/pcc/mip
BIN = $(ROOT)/usr/bin
LIB = $(ROOT)/usr/lib

all:	chk_pdp $(ALL)

chk_pdp:
	if pdp11 && [ "$(FLG)" != "x" ]; then \
		$(MAKE) -$(MAKEFLAGS) FLG=x ALL="$(ALL)" LINT="$(LINT)" \
			MIP="$(MIP)" CFLAGS="$(CFLAGS) -UFLEXNAME" \
			-f cflow.mk all; \
	fi

dag:	dag.c
	$(CC) $(CFLAGS) $(LDFLAGS) -i dag.c -o $(TESTDIR)/dag

lpfx:	lpfx.c $(LINT)/lerror.h $(LINT)/lmanifest $(LINT)/lpass2.h \
		$(MIP)/manifest
	$(CC) $(CFLAGS) -I$(LINT) -I$(MIP) $(LDFLAGS) -i lpfx.c -o $(TESTDIR)/lpfx

nmf:	nmf.c
	$(CC) $(CFLAGS) nmf.c $(LDFLAGS) -o $(TESTDIR)/nmf

flip:	flip.c
	$(CC) $(CFLAGS) flip.c $(LDFLAGS) -o $(TESTDIR)/flip

install:	chk_pdp $(ALL)
	cp cflow.sh $(BIN)/cflow
	cp dag $(LIB)/dag
	cp lpfx $(LIB)/lpfx
	cp nmf $(LIB)/nmf
	cp flip $(LIB)/flip
	chgrp bin $(BIN)/cflow $(LIB)/dag $(LIB)/lpfx $(LIB)/nmf $(LIB)/flip
#	chown bin $(BIN)/cflow $(LIB)/dag $(LIB)/lpfx $(LIB)/nmf $(LIB)/flip
#	chmod 755 $(BIN)/cflow $(LIB)/dag $(LIB)/lpfx $(LIB)/nmf $(LIB)/flip

clean:
	-rm -f *.o a.out make.out core

clobber:	clean
	-rm -f $(ALL)
