MAKE=/bin/make
MAKEFILE=psfiles.mk

SYSTEM=V9
VERSION=3.3.2

GROUP=bin
OWNER=bin

POSTLIB=/usr/lib/postscript

all :

install :
	@if [ ! -d "$(POSTLIB)" ]; then \
	    mkdir $(POSTLIB); \
	    chmod 755 $(POSTLIB); \
	    chgrp $(GROUP) $(POSTLIB); \
	    chown $(OWNER) $(POSTLIB); \
	fi
	cp *.ps ps.* $(POSTLIB)
	@for i in *.ps ps.*; do \
	    chmod 644 $(POSTLIB)/$$i; \
	    chgrp $(GROUP) $(POSTLIB)/$$i; \
	    chown $(OWNER) $(POSTLIB)/$$i; \
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
	    -e "s'^POSTLIB=.*'POSTLIB=$(POSTLIB)'" \
	$(MAKEFILE) >XXX.mk; \
	mv XXX.mk $(MAKEFILE)

