MAKE=/bin/make
MAKEFILE=hardcopy.mk

OWNER=bin
GROUP=bin

MAN1DIR=/tmp
MAN5DIR=/usr/man/p_man/man5
POSTLIB=/usr/lib/postscript
POSTBIN=/usr/bin/postscript

all : hardcopy

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
	cp hardcopy $(POSTBIN)/hardcopy
	@chmod 755 $(POSTBIN)/hardcopy
	@chgrp $(GROUP) $(POSTBIN)/hardcopy
	@chown $(OWNER) $(POSTBIN)/hardcopy
	cp hardcopy.ps $(POSTLIB)/hardcopy.ps
	@chmod 644 $(POSTLIB)/hardcopy.ps
	@chgrp $(GROUP) $(POSTLIB)/hardcopy.ps
	@chown $(OWNER) $(POSTLIB)/hardcopy.ps
	cp hardcopy.1 $(MAN1DIR)/hardcopy.1
	@chmod 644 $(MAN1DIR)/hardcopy.1
	@chgrp $(GROUP) $(MAN1DIR)/hardcopy.1
	@chown $(OWNER) $(MAN1DIR)/hardcopy.1


clean :

clobber : clean
	rm -f hardcopy

hardcopy : hardcopy.sh
	sed "s'^POSTLIB=.*'POSTLIB=$(POSTLIB)'" hardcopy.sh >hardcopy
	@chmod 755 hardcopy

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
	hardcopy.1 >XXX.1; \
	mv XXX.1 hardcopy.1


