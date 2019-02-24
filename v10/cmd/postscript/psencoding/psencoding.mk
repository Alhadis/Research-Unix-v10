MAKE=/bin/make
MAKEFILE=psencoding.mk

OWNER=bin
GROUP=bin

MAN1DIR=/tmp
MAN5DIR=/usr/man/p_man/man5
POSTLIB=/usr/lib/postscript
POSTBIN=/usr/bin/postscript

all : psencoding

install : all
	@if [ ! -d "$(POSTBIN)" ]; then \
	    mkdir $(POSTBIN); \
	    chmod 755 $(POSTBIN); \
	    chgrp $(GROUP) $(POSTBIN); \
	    chown $(OWNER) $(POSTBIN); \
	fi
	@if [ ! -d "$(POSTLIB)" ]; then \
	    mkdir $(POSTLIB); \
	    chmod 755 $(POSTLIB); \
	    chgrp $(GROUP) $(POSTLIB); \
	    chown $(OWNER) $(POSTLIB); \
	fi
	cp psencoding $(POSTBIN)/psencoding
	@chmod 755 $(POSTBIN)/psencoding
	@chgrp $(GROUP) $(POSTBIN)/psencoding
	@chown $(OWNER) $(POSTBIN)/psencoding
	cp Latin1.enc $(POSTLIB)/Latin1.enc
	@chmod 644 $(POSTLIB)/Latin1.enc
	@chgrp $(GROUP) $(POSTLIB)/Latin1.enc
	@chown $(OWNER) $(POSTLIB)/Latin1.enc
	cp psencoding.1 $(MAN1DIR)/psencoding.1
	@chmod 644 $(MAN1DIR)/psencoding.1
	@chgrp $(GROUP) $(MAN1DIR)/psencoding.1
	@chown $(OWNER) $(MAN1DIR)/psencoding.1

clean :

clobber : clean
	rm -f psencoding

psencoding : psencoding.sh
	sed "s'^POSTLIB=.*'POSTLIB=$(POSTLIB)'" psencoding.sh >psencoding
	@chmod 755 psencoding

changes :
	@trap "" 1 2 3 15; \
	sed \
	    -e "s'^OWNER=.*'OWNER=$(OWNER)'" \
	    -e "s'^GROUP=.*'GROUP=$(GROUP)'" \
	    -e "s'^MAN1DIR=.*'MAN1DIR=$(MAN1DIR)'" \
	    -e "s'^MAN5DIR=.*'MAN5DIR=$(MAN5DIR)'" \
	    -e "s'^POSTBIN=.*'POSTBIN=$(POSTBIN)'" \
	    -e "s'^POSTLIB=.*'POSTLIB=$(POSTLIB)'" \
	$(MAKEFILE) >XXX.mk; \
	mv XXX.mk $(MAKEFILE); \
	sed \
	    -e "s'^.ds dQ.*'.ds dQ $(POSTLIB)'" \
	psencoding.1 >XXX.1; \
	mv XXX.1 psencoding.1

