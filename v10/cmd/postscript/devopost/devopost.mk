MAKE=/bin/make
MAKEFILE=devopost.mk

SYSTEM=V9
VERSION=3.3.1

GROUP=bin
OWNER=bin

FONTDIR=/usr/lib/font
FONTFILES=DESC ? ?? [A-Z]??*

all :

install : all
	@rm -fr $(FONTDIR)/devopost/*.out
	@if [ ! -d $(FONTDIR) ]; then \
	    mkdir $(FONTDIR); \
	    chmod 755 $(FONTDIR); \
	    chgrp $(GROUP) $(FONTDIR); \
	    chown $(OWNER) $(FONTDIR); \
	fi
	@if [ ! -d $(FONTDIR)/devopost ]; then \
	    mkdir $(FONTDIR)/devopost; \
	    chmod 755 $(FONTDIR)/devopost; \
	    chgrp $(GROUP) $(FONTDIR)/devopost; \
	    chown $(OWNER) $(FONTDIR)/devopost; \
	fi
	@if [ ! -d $(FONTDIR)/devopost/charlib ]; then \
	    mkdir $(FONTDIR)/devopost/charlib; \
	    chmod 755 $(FONTDIR)/devopost/charlib; \
	    chgrp $(GROUP) $(FONTDIR)/devopost/charlib; \
	    chown $(OWNER) $(FONTDIR)/devopost/charlib; \
	fi
	cp $(FONTFILES) $(FONTDIR)/devopost
	@for i in $(FONTFILES); do \
	    chmod 644 $(FONTDIR)/devopost/$$i; \
	    chgrp $(GROUP) $(FONTDIR)/devopost/$$i; \
	    chown $(OWNER) $(FONTDIR)/devopost/$$i; \
	done
	cp charlib/* $(FONTDIR)/devopost/charlib
	@for i in charlib/*; do \
	    chmod 644 $(FONTDIR)/devopost/$$i; \
	    chgrp $(GROUP) $(FONTDIR)/devopost/$$i; \
	    chown $(OWNER) $(FONTDIR)/devopost/$$i; \
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

