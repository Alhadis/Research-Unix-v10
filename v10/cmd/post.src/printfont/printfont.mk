MAKE=/bin/make
MAKEFILE=printfont.mk

OWNER=bin
GROUP=bin

MAN1DIR=/tmp
MAN5DIR=/usr/man/p_man/man5
POSTLIB=/usr/lib/postscript
POSTBIN=/usr/bin/postscript

all : printfont

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
	cp printfont $(POSTBIN)/printfont
	@chmod 755 $(POSTBIN)/printfont
	@chgrp $(GROUP) $(POSTBIN)/printfont
	@chown $(OWNER) $(POSTBIN)/printfont
	cp printfont.ps $(POSTLIB)/printfont.ps
	@chmod 644 $(POSTLIB)/printfont.ps
	@chgrp $(GROUP) $(POSTLIB)/printfont.ps
	@chown $(OWNER) $(POSTLIB)/printfont.ps
	cp printfont.1 $(MAN1DIR)/printfont.1
	@chmod 644 $(MAN1DIR)/printfont.1
	@chgrp $(GROUP) $(MAN1DIR)/printfont.1
	@chown $(OWNER) $(MAN1DIR)/printfont.1

clean :

clobber : clean
	rm -f printfont

printfont : printfont.sh
	sed "s'^POSTLIB=.*'POSTLIB=$(POSTLIB)'" printfont.sh >printfont
	@chmod 755 printfont

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
	printfont.1 >XXX.1; \
	mv XXX.1 printfont.1

