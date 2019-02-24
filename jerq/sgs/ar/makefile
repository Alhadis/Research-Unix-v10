COMI	= ../inc
FLEX	= -DFLEXNAMES
ARFORMAT= PORTAR
LIBLD	= ../libld/libld.a
HFILES	= $(COMI)/ar.h $(COMI)/filehdr.h $(COMI)/syms.h 
CFLAGS	=  -O $(FLEX) -D$(ARFORMAT) -I$(COMI)

ar:	ar.c $(HFILES) $(LIBLD)
	$(CC) $(CFLAGS) ar.c -o ar $(LIBLD)

install:	ar
	cp ar /usr/jerq/bin/3ar
	strip /usr/jerq/bin/3ar

clean:
	-rm -f ar core
