MAKE=/bin/make
MAKEFILE=postprint.mk

SYSTEM=V9
VERSION=3.3.1

GROUP=bin
OWNER=bin

MAN1DIR=/tmp
POSTBIN=/usr/bin/postscript
POSTLIB=/usr/lib/postscript

COMMONDIR=../common

CFLGS=-O
LDFLGS=-s

CFLAGS=$(CFLGS) -I$(COMMONDIR)
LDFLAGS=$(LDFLGS)

HFILES=postprint.h\
       $(COMMONDIR)/comments.h\
       $(COMMONDIR)/ext.h\
       $(COMMONDIR)/gen.h\
       $(COMMONDIR)/path.h

OFILES=postprint.o\
       $(COMMONDIR)/glob.o\
       $(COMMONDIR)/misc.o\
       $(COMMONDIR)/request.o

all : postprint

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
	cp postprint $(POSTBIN)/postprint
	@chmod 755 $(POSTBIN)/postprint
	@chgrp $(GROUP) $(POSTBIN)/postprint
	@chown $(OWNER) $(POSTBIN)/postprint
	cp postprint.ps $(POSTLIB)/postprint.ps
	@chmod 644 $(POSTLIB)/postprint.ps
	@chgrp $(GROUP) $(POSTLIB)/postprint.ps
	@chown $(OWNER) $(POSTLIB)/postprint.ps
	cp postprint.1 $(MAN1DIR)/postprint.1
	@chmod 644 $(MAN1DIR)/postprint.1
	@chgrp $(GROUP) $(MAN1DIR)/postprint.1
	@chown $(OWNER) $(MAN1DIR)/postprint.1

clean :
	rm -f *.o

clobber : clean
	rm -f postprint

postprint : $(OFILES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o postprint $(OFILES)

postprint.o : $(HFILES)

$(COMMONDIR)/glob.o\
$(COMMONDIR)/misc.o\
$(COMMONDIR)/request.o :
	@cd $(COMMONDIR); $(MAKE) -f common.mk `basename $@`

changes :
	@trap "" 1 2 3 15; \
	sed \
	    -e "s'^SYSTEM=.*'SYSTEM=$(SYSTEM)'" \
	    -e "s'^VERSION=.*'VERSION=$(VERSION)'" \
	    -e "s'^GROUP=.*'GROUP=$(GROUP)'" \
	    -e "s'^OWNER=.*'OWNER=$(OWNER)'" \
	    -e "s'^MAN1DIR=.*'MAN1DIR=$(MAN1DIR)'" \
	    -e "s'^POSTBIN=.*'POSTBIN=$(POSTBIN)'" \
	    -e "s'^POSTLIB=.*'POSTLIB=$(POSTLIB)'" \
	$(MAKEFILE) >XXX.mk; \
	mv XXX.mk $(MAKEFILE); \
	sed \
	    -e "s'^.ds dQ.*'.ds dQ $(POSTLIB)'" \
	postprint.1 >XXX.1; \
	mv XXX.1 postprint.1

