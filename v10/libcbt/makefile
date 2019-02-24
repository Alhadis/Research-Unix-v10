.SUFFIXES:	.x .o .c
.c.o:
	$(CC) $(CFLAGS) -c $<
	-ld -r -X $@
	mv a.out $@
.c.x:
	$(CC) $(CFLAGS) -c $<
	-ld -r -X $*.o
	mv a.out $*.o
	ar u libcbt.a $*.o
	rm $*.o
	touch $*.x
all:	libcbt.a 
bwrite.x bdelete.x diskrd.x diskwrt.x bt.x seek.x tran.x:	cbt.h
libcbt.a:	bt.x seek.x tran.x diskrd.x diskwrt.x bwrite.x bdelete.x lib.x
	ranlib libcbt.a
cyntax:
	cyntax bttest.c bt.c seek.c tran.c diskrd.c diskwrt.c bwrite.c bdelete.c lib.c
install:
	-cp cbt.h /usr/include
	-cp libcbt.a /usr/lib
	-ranlib /usr/lib/libcbt.a
clean:
	rm -f *.o *.x libcbt.a nohup.out
