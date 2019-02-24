LIBNAME =	libether.a
LIBDIR =	/usr/lib
LIBF =		$(LIBDIR)/$(LIBNAME)

LIBFILES =	order.o findslot.o

INCNAMES =	ethernet.h order.h
INCDIR =	/usr/include/sys
INCF =		$(INCDIR)/ethernet.h $(INCDIR)/order.h

LIBETHER =	$(LIBDIR)/libether.a
LIBCHAOS =	$(LIBDIR)/libchaos.a
LIBIN =		$(LIBDIR)/libin.a

BINDIR =	/usr/ether

CMDS =		netconfig arpconfig ipconfig chaosconfig
OFILES =	netconfig.o arpconfig.o chaosconfig.o ipconfig.o


all:	lib cmds

lib:	$(LIBF)
cmds:	$(CMDS)

clean:
	rm -f core *.BAK .emacs* *.o $(CMDS)


$(LIBF):	$(LIBFILES)
	ar r $(LIBF) $(LIBFILES)
	ranlib $(LIBF)

$(LIBFILES):	$(INCF)
$(OFILES):	$(INCF)

$(INCF):	$(INCNAMES)
	cp $(INCNAMES) $(INCDIR)


netconfig:	netconfig.o $(LIBETHER)
	cc -o netconfig netconfig.o -lether
	cp netconfig $(BINDIR)
	strip $(BINDIR)/netconfig

arpconfig:	arpconfig.o $(LIBETHER)
	cc -o arpconfig arpconfig.o -lether
	cp arpconfig $(BINDIR)
	strip $(BINDIR)/arpconfig

chaosconfig:	chaosconfig.o $(LIBETHER) $(LIBCHAOS)
	cc -o chaosconfig chaosconfig.o -lether -lchaos
	cp chaosconfig $(BINDIR)
	strip $(BINDIR)/chaosconfig

ipconfig:	ipconfig.o $(LIBETHER) $(LIBIN)
	cc -o ipconfig ipconfig.o -lether -lin
	cp ipconfig $(BINDIR)
	strip $(BINDIR)/ipconfig
