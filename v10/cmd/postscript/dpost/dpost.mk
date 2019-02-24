MAKE=/bin/make
MAKEFILE=dpost.mk

SYSTEM=V9
VERSION=3.3.1

GROUP=bin
OWNER=bin

FONTDIR=/usr/lib/font
MAN1DIR=/tmp
POSTBIN=/usr/bin/postscript
POSTLIB=/usr/lib/postscript

COMMONDIR=../common

CFLGS=-O
LDFLGS=-s

CFLAGS=$(CFLGS) -I$(COMMONDIR)
LDFLAGS=$(LDFLGS)

HFILES=dpost.h\
       font.h\
       motion.h\
       ps_include.h\
       $(COMMONDIR)/comments.h\
       $(COMMONDIR)/ext.h\
       $(COMMONDIR)/gen.h\
       $(COMMONDIR)/path.h

OFILES=dpost.o\
       draw.o\
       color.o\
       font.o\
       pictures.o\
       ps_include.o\
       $(COMMONDIR)/bbox.o\
       $(COMMONDIR)/glob.o\
       $(COMMONDIR)/misc.o\
       $(COMMONDIR)/request.o\
       $(COMMONDIR)/tempnam.o

all : dpost

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
	cp dpost $(POSTBIN)/dpost
	@chmod 755 $(POSTBIN)/dpost
	@chgrp $(GROUP) $(POSTBIN)/dpost
	@chown $(OWNER) $(POSTBIN)/dpost
	cp dpost.ps $(POSTLIB)/dpost.ps
	@chmod 644 $(POSTLIB)/dpost.ps
	@chgrp $(GROUP) $(POSTLIB)/dpost.ps
	@chown $(OWNER) $(POSTLIB)/dpost.ps
	cp draw.ps $(POSTLIB)/draw.ps
	@chmod 644 $(POSTLIB)/draw.ps
	@chgrp $(GROUP) $(POSTLIB)/draw.ps
	@chown $(OWNER) $(POSTLIB)/draw.ps
	cp dpost.1 $(MAN1DIR)/dpost.1
	@chmod 644 $(MAN1DIR)/dpost.1
	@chgrp $(GROUP) $(MAN1DIR)/dpost.1
	@chown $(OWNER) $(MAN1DIR)/dpost.1

clean :
	rm -f *.o

clobber : clean
	rm -f dpost

dpost : $(OFILES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o dpost $(OFILES) -lm

dpost.o : $(HFILES)
color.o : $(COMMONDIR)/ext.h $(COMMONDIR)/gen.h
draw.o : motion.h $(COMMONDIR)/ext.h $(COMMONDIR)/gen.h
font.o : font.h $(COMMONDIR)/gen.h
pictures.o : $(COMMONDIR)/comments.h $(COMMONDIR)/gen.h
ps_include.o : ps_include.h

$(COMMONDIR)/bbox.o\
$(COMMONDIR)/glob.o\
$(COMMONDIR)/misc.o\
$(COMMONDIR)/request.o\
$(COMMONDIR)/tempnam.o :
	@cd $(COMMONDIR); $(MAKE) -f common.mk SYSTEM=$(SYSTEM) `basename $@`

changes :
	@trap "" 1 2 3 15; \
	sed \
	    -e "s'^SYSTEM=.*'SYSTEM=$(SYSTEM)'" \
	    -e "s'^VERSION=.*'VERSION=$(VERSION)'" \
	    -e "s'^GROUP=.*'GROUP=$(GROUP)'" \
	    -e "s'^OWNER=.*'OWNER=$(OWNER)'" \
	    -e "s'^FONTDIR=.*'FONTDIR=$(FONTDIR)'" \
	    -e "s'^MAN1DIR=.*'MAN1DIR=$(MAN1DIR)'" \
	    -e "s'^POSTBIN=.*'POSTBIN=$(POSTBIN)'" \
	    -e "s'^POSTLIB=.*'POSTLIB=$(POSTLIB)'" \
	$(MAKEFILE) >XXX.mk; \
	mv XXX.mk $(MAKEFILE); \
	sed \
	    -e "s'^.ds dF.*'.ds dF $(FONTDIR)'" \
	    -e "s'^.ds dQ.*'.ds dQ $(POSTLIB)'" \
	dpost.1 >XXX.1; \
	mv XXX.1 dpost.1

