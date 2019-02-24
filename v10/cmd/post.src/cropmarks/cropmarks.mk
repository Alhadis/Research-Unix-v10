MAKE=/bin/make
MAKEFILE=cropmarks.mk

OWNER=bin
GROUP=bin

MAN1DIR=/tmp
MAN5DIR=/usr/man/p_man/man5
POSTLIB=/usr/lib/postscript
POSTBIN=/usr/bin/postscript

all : cropmarks

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
	cp cropmarks $(POSTBIN)/cropmarks
	@chmod 755 $(POSTBIN)/cropmarks
	@chgrp $(GROUP) $(POSTBIN)/cropmarks
	@chown $(OWNER) $(POSTBIN)/cropmarks
	cp cropmarks.ps $(POSTLIB)/cropmarks.ps
	@chmod 644 $(POSTLIB)/cropmarks.ps
	@chgrp $(GROUP) $(POSTLIB)/cropmarks.ps
	@chown $(OWNER) $(POSTLIB)/cropmarks.ps
	cp cropmarks.1 $(MAN1DIR)/cropmarks.1
	@chmod 644 $(MAN1DIR)/cropmarks.1
	@chgrp $(GROUP) $(MAN1DIR)/cropmarks.1
	@chown $(OWNER) $(MAN1DIR)/cropmarks.1

clean :

clobber : clean
	rm -f cropmarks

cropmarks : cropmarks.sh
	sed "s'^POSTLIB=.*'POSTLIB=$(POSTLIB)'" cropmarks.sh >cropmarks
	@chmod 755 cropmarks

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
	cropmarks.1 >XXX.1; \
	mv XXX.1 cropmarks.1

