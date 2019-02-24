MAKE=/bin/make
MAKEFILE=devLatin1.mk

SYSTEM=V9
VERSION=3.3.1

GROUP=bin
OWNER=bin

FONTDIR=/usr/lib/font
FONTFILES=DESC ? ?? [A-Z]??* shell.lib

all :

install : all
	@if [ ! -d $(FONTDIR) ]; then \
	    mkdir $(FONTDIR); \
	    chmod 755 $(FONTDIR); \
	    chgrp $(GROUP) $(FONTDIR); \
	    chown $(OWNER) $(FONTDIR); \
	fi
	@if [ ! -d $(FONTDIR)/devLatin1 ]; then \
	    mkdir $(FONTDIR)/devLatin1; \
	    chmod 755 $(FONTDIR)/devLatin1; \
	    chgrp $(GROUP) $(FONTDIR)/devLatin1; \
	    chown $(OWNER) $(FONTDIR)/devLatin1; \
	fi
	@if [ ! -d $(FONTDIR)/devLatin1/charlib ]; then \
	    mkdir $(FONTDIR)/devLatin1/charlib; \
	    chmod 755 $(FONTDIR)/devLatin1/charlib; \
	    chgrp $(GROUP) $(FONTDIR)/devLatin1/charlib; \
	    chown $(OWNER) $(FONTDIR)/devLatin1/charlib; \
	fi
	cp $(FONTFILES) $(FONTDIR)/devLatin1
	@for i in $(FONTFILES); do \
	    chmod 644 $(FONTDIR)/devLatin1/$$i; \
	    chgrp $(GROUP) $(FONTDIR)/devLatin1/$$i; \
	    chown $(OWNER) $(FONTDIR)/devLatin1/$$i; \
	done
	cp charlib/* $(FONTDIR)/devLatin1/charlib
	@for i in charlib/*; do \
	    chmod 644 $(FONTDIR)/devLatin1/$$i; \
	    chgrp $(GROUP) $(FONTDIR)/devLatin1/$$i; \
	    chown $(OWNER) $(FONTDIR)/devLatin1/$$i; \
	done

clean :

clobber : clean

changes :
	@trap "" 1 2 3 15; \
	sed \
	    -e "s'^SYSTEM=.*'SYSTEM=$(SYSTEM)'" \
	    -e "s'^VERSION=.*'VERSION=$(VERSION)'" \
	    -e "s'^GROUP=.*'GROUP=$(GROUP)'" \
	    -e "s'^OWNER=.*'OWNER=$(OWNER)'" \
	    -e "s'^FONTDIR=.*'FONTDIR=$(FONTDIR)'" \
	$(MAKEFILE) >XXX.mk; \
	mv XXX.mk $(MAKEFILE)

