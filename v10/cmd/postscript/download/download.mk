MAKE=/bin/make
MAKEFILE=download.mk

SYSTEM=V9
VERSION=3.3.1

GROUP=bin
OWNER=bin

HOSTDIR=/usr/lib/font/postscript
MAN1DIR=/tmp
POSTBIN=/usr/bin/postscript
POSTLIB=/usr/lib/postscript

COMMONDIR=../common

CFLGS=-O
LDFLGS=-s

CFLAGS=$(CFLGS) -I$(COMMONDIR)
LDFLAGS=$(LDFLGS)

HFILES=download.h\
       $(COMMONDIR)/comments.h\
       $(COMMONDIR)/ext.h\
       $(COMMONDIR)/gen.h\
       $(COMMONDIR)/path.h

OFILES=download.o\
       $(COMMONDIR)/glob.o\
       $(COMMONDIR)/misc.o\
       $(COMMONDIR)/tempnam.o

all : download

install : all
	@if [ ! -d "$(POSTBIN)" ]; then \
	    mkdir $(POSTBIN); \
	    chmod 755 $(POSTBIN); \
	    chgrp $(GROUP) $(POSTBIN); \
	    chown $(OWNER) $(POSTBIN); \
	fi
	cp download $(POSTBIN)/download
	@chmod 755 $(POSTBIN)/download
	@chgrp $(GROUP) $(POSTBIN)/download
	@chown $(OWNER) $(POSTBIN)/download
	cp download.1 $(MAN1DIR)/download.1
	@chmod 644 $(MAN1DIR)/download.1
	@chgrp $(GROUP) $(MAN1DIR)/download.1
	@chown $(OWNER) $(MAN1DIR)/download.1

clean :
	rm -f *.o

clobber : clean
	rm -f download

download : $(OFILES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o download $(OFILES)

download.o : $(HFILES)

$(COMMONDIR)/glob.o\
$(COMMONDIR)/misc.o\
$(COMMONDIR)/tempnam.o :
	@cd $(COMMONDIR); $(MAKE) -f common.mk SYSTEM=$(SYSTEM) `basename $@`

changes :
	@trap "" 1 2 3 15; \
	sed \
	    -e "s'^SYSTEM=.*'SYSTEM=$(SYSTEM)'" \
	    -e "s'^VERSION=.*'VERSION=$(VERSION)'" \
	    -e "s'^GROUP=.*'GROUP=$(GROUP)'" \
	    -e "s'^OWNER=.*'OWNER=$(OWNER)'" \
	    -e "s'^HOSTDIR=.*'HOSTDIR=$(HOSTDIR)'" \
	    -e "s'^MAN1DIR=.*'MAN1DIR=$(MAN1DIR)'" \
	    -e "s'^POSTBIN=.*'POSTBIN=$(POSTBIN)'" \
	    -e "s'^POSTLIB=.*'POSTLIB=$(POSTLIB)'" \
	$(MAKEFILE) >XXX.mk; \
	mv XXX.mk $(MAKEFILE); \
	sed \
	    -e "s'^.ds dH.*'.ds dH $(HOSTDIR)'" \
	download.1 >XXX.1; \
	mv XXX.1 download.1

