OBJDKCC =	dkcc.o dkcanon.o dkxenviron.o dkxstdio.o dkxwrite.o maphost.o
CFLAGS =	-O
LIBS =		-lipc

dkcc:	$(OBJDKCC)
	cc $(CFLAGS) $(OBJDKCC) $(LIBS) -o dkcc

clean:
	rm -f *.o dkcc

dkcc.o:	sysexits.h
dkxstdio.o:	remfio.h
dkxstdio.o:	sysexits.h
dkxstdio.o:	Vtermio.h
dkxwrite.o:	remfio.h
dkxwrite.o:	sysexits.h
