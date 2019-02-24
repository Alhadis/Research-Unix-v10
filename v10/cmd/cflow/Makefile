#	@(#)cflow.mk	1.7	83/08/19
#	3.0 SID #	1.2
FLG=
TESTDIR = .
CFLAGS =-O -DFLEXNAMES
FILES = Makefile README cflow.sh dag.c lpfx.c nmf.c flip.c
ALL = dag lpfx nmf flip
LINT = ../lint
MIP = ../pcc1/mip
BIN = $(ROOT)/usr/bin
LIB = $(ROOT)/usr/lib

all:	$(ALL)

dag:	dag.c
	$(CC) $(CFLAGS) $(LDFLAGS) dag.c -o $(TESTDIR)/dag

lpfx:	lpfx.c $(LINT)/lint.h  $(MIP)/manifest
	$(CC) $(CFLAGS) -I$(LINT) -I$(MIP) $(LDFLAGS) lpfx.c -o $(TESTDIR)/lpfx

nmf:	nmf.c
	$(CC) $(CFLAGS) nmf.c $(LDFLAGS) -o $(TESTDIR)/nmf

flip:	flip.c
	$(CC) $(CFLAGS) flip.c $(LDFLAGS) -o $(TESTDIR)/flip

install:	$(ALL)
	cp cflow.sh $(BIN)/cflow
	cp dag $(LIB)/dag
	cp lpfx $(LIB)/lpfx
	cp nmf $(LIB)/nmf
	cp flip $(LIB)/flip

clean:
	-rm -f *.o a.out make.out core $(ALL)
