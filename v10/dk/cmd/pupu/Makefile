ALL:	dkcc pupu

OBJDKCC =	dkcc.o dkcanon.o dkxenviron.o dkxstdio.o dkxwrite.o maphost.o
OBJDIR =	closedir.o fdopendir.o opendir.o readdir.o seekdir.o telldir.o curdir.o
OBJPUPU =	pupu.o pupucommon.o dkeof.o pull.o push.o maphost.o vsprintf.o
CFLAGS =	-O
LIBS =		-ldk
dkcc:	$(OBJDKCC)
	cc $(CFLAGS) $(OBJDKCC) $(LIBS) -o dkcc

pupu:	$(OBJDIR) $(OBJPUPU)
	cc $(CFLAGS) $(OBJPUPU) $(OBJDIR) $(LIBS) -o pupu

pupu.h:	
	touch pupu.h

closedir.o:	ndir.h
dkcc.o:	sysexits.h
dkcc.o:	string.h
dkxenviron.o:	string.h
dkxstdio.o:	remfio.h
dkxstdio.o:	sysexits.h
dkxstdio.o:	Vtermio.h
dkxwrite.o:	remfio.h
dkxwrite.o:	sysexits.h
fdopendir.o:	ndir.h
maphost.o:	string.h
opendir.o:	ndir.h
pull.o:	string.h
pull.o:	pupu.h
pull.o:	ndir.h
pupu.o:	string.h
pupu.o:	tdefs.h
pupu.h:	sysexits.h
pupu.h:	tdefs.h
pupu.h:	udefs.h
pupucommon.o:	pupu.h
pupucommon.o:	string.h
push.o:	string.h
push.o:	pupu.h
push.o:	ndir.h
readdir.o:	ndir.h
seekdir.o:	ndir.h
telldir.o:	ndir.h
