.SUFFIXES:	.x .o .c
.c.o:
	$(CC) $(CFLAGS) -c $<
	-ld -r -X $@
	mv a.out $@
.c.x:
	$(CC) $(CFLAGS) -c $<
	-ld -r -X $*.o
	mv a.out $*.o
	ar u btlib $*.o
	touch $*.x
CFLAGS=-g
debug:	bttest
all:	nodesize
all:	btlib btcat btbuild bttest btcreat btreport btsquash btran btdelete
all:	btadd btdiag btexer
bwrite.x bdelete.x diskrd.x diskwrt.x bt.x seek.x tran.x:	cbt.h
btsquash.o btcreat.o btreport.o bttest.o btcat.o btbuild.o:	cbt.h
btlib:	bt.x seek.x tran.x diskrd.x diskwrt.x bwrite.x bdelete.x lib.x bufs.x
	ranlib btlib
cyntax:
	cyntax bttest.c bt.c seek.c tran.c diskrd.c diskwrt.c bwrite.c bdelete.c lib.c
bttest:	bttest.o btlib
	$(CC) $(CFLAGS) -o bttest bttest.o btlib
btran:	btran.o
	$(CC) $(CFLAGS) -o btran btran.o
btsquash:	btsquash.o btlib
	$(CC) $(CFLAGS) -o btsquash btsquash.o btlib
btadd:	btadd.o btlib
	$(CC) $(CFLAGS) -o btadd btadd.o btlib
btreport:	btreport.o btlib
	$(CC) $(CFLAGS) -o btreport btreport.o btlib
btdelete:	btdelete.o btlib
	$(CC) $(CFLAGS) -o btdelete btdelete.o btlib
btcreat:	btcreat.o cbt.h
	$(CC) $(CFLAGS) -o btcreat btcreat.o btlib
btdiag:	btdiag.o btlib
	$(CC) $(CFLAGS) -o btdiag btdiag.o btlib
btexer:	btexer.o btlib
	$(CC) $(CFLAGS) -o btexer btexer.o btlib
btcat:	btcat.o btlib
	cc -o btcat btcat.o btlib
btbuild:	btbuild.o lib.x
	$(CC) $(CFLAGS) -o btbuild btbuild.o btlib
nodesize:
	@cc -E $(CFLAGS) nodesz.c | grep nodesize
install:
	-cp cbt /usr/bin
	-cp cbt.h /usr/include
	-cp btlib /usr/lib/libcbt.a
	-ranlib /usr/lib/libcbt.a
clean:
	rm *.o *.x btlib
