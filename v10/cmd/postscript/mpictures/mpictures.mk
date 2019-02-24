#
# Not installing the man page.
#

MAKE=/bin/make
MAKEFILE=mpictures.mk

SYSTEM=V9
VERSION=3.3.1

OWNER=bin
GROUP=bin

TMACDIR=/usr/lib/tmac

all :
	@cp pictures.sr tmac.pictures;

install : all
	@if [ ! -d $(TMACDIR) ]; then \
	    mkdir $(TMACDIR); \
	    chmod 755 $(TMACDIR); \
	    chgrp $(GROUP) $(TMACDIR); \
	    chown $(OWNER) $(TMACDIR); \
	fi
	cp tmac.pictures $(TMACDIR)/tmac.pictures
	@chmod 644 $(TMACDIR)/tmac.pictures
	@chgrp $(GROUP) $(TMACDIR)/tmac.pictures
	@chown $(OWNER) $(TMACDIR)/tmac.pictures

clean :
	rm -f tmac.pictures

clobber : clean

changes :
	@trap "" 1 2 3 15; \
	sed \
	    -e "s'^SYSTEM=.*'SYSTEM=$(SYSTEM)'" \
	    -e "s'^VERSION=.*'VERSION=$(VERSION)'" \
	    -e "s'^GROUP=.*'GROUP=$(GROUP)'" \
	    -e "s'^OWNER=.*'OWNER=$(OWNER)'" \
	    -e "s'^TMACDIR=.*'TMACDIR=$(TMACDIR)'" \
	$(MAKEFILE) >X$(MAKEFILE); \
	mv X$(MAKEFILE) $(MAKEFILE); \
	sed \
	    -e "s'^.ds dT.*'.ds dT $(TMACDIR)'" \
	mpictures.5 >Xmpictures.5; \
	mv Xmpictures.5 mpictures.5

