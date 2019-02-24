MAKE=/bin/make
MAKEFILE=postreverse.mk

SYSTEM=V9
VERSION=3.3.1

GROUP=bin
OWNER=bin

MAN1DIR=/tmp
POSTBIN=/usr/bin/postscript

COMMONDIR=../common

CFLGS=-O
LDFLGS=-s

CFLAGS=$(CFLGS) -I$(COMMONDIR)
LDFLAGS=$(LDFLGS)

HFILES=postreverse.h\
       $(COMMONDIR)/comments.h\
       $(COMMONDIR)/ext.h\
       $(COMMONDIR)/gen.h\
       $(COMMONDIR)/path.h

OFILES=postreverse.o\
       $(COMMONDIR)/glob.o\
       $(COMMONDIR)/misc.o\
       $(COMMONDIR)/tempnam.o

all : postreverse

install : all
	@if [ ! -d "$(POSTBIN)" ]; then \
	    mkdir $(POSTBIN); \
	    chmod 755 $(POSTBIN); \
	    chgrp $(GROUP) $(POSTBIN); \
	    chown $(OWNER) $(POSTBIN); \
	fi
	cp postreverse $(POSTBIN)/postreverse
	@chmod 755 $(POSTBIN)/postreverse
	@chgrp $(GROUP) $(POSTBIN)/postreverse
	@chown $(OWNER) $(POSTBIN)/postreverse
	cp postreverse.1 $(MAN1DIR)/postreverse.1
	@chmod 644 $(MAN1DIR)/postreverse.1
	@chgrp $(GROUP) $(MAN1DIR)/postreverse.1
	@chown $(OWNER) $(MAN1DIR)/postreverse.1

clean :
	rm -f *.o

clobber : clean
	rm -f postreverse

postreverse : $(OFILES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o postreverse $(OFILES)

postreverse.o : $(HFILES)

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
	    -e "s'^MAN1DIR=.*'MAN1DIR=$(MAN1DIR)'" \
	    -e "s'^POSTBIN=.*'POSTBIN=$(POSTBIN)'" \
	$(MAKEFILE) >XXX.mk; \
	mv XXX.mk $(MAKEFILE)

