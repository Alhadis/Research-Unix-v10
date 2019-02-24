TARGET=
OS=
CFLAGS=
LDFLAGS=-s
D=$(HOST)-$(HOSTOS)

all:	$(D)/rcc

OBJS=$(D)/bind.o $(D)/gen.o $(D)/decode.o \
	$(D)/vax-sel.o $(D)/mips-sel.o $(D)/mc-sel.o $(D)/sparc-sel.o

INCLUDES=-I$(HOST) -I. -I../c
include ../c/makefile

$(COBJS):	config.h
$(OBJS):	config.h
$(OBJS):	gen.h

$(D)/bind.o:	$(D)/bind.c;		$(CC) -c -o $@ $(CCOPT) $(D)/bind.c
$(D)/decode.o:	$(HOST)/decode.c;	$(CC) -c -o $@ $(CCOPT) $(HOST)/decode.c
$(D)/gen.o:	gen.c;			$(CC) -c -o $@ $(CCOPT) gen.c

$(D)/vax-sel.o:		vax/sel.c vax/pseudos.c stabbsd.c
			$(CC) -c -o $@ -Ivax $(CCOPT) vax/sel.c
$(D)/mips-sel.o:	mips/config.h mips/sel.c mips/pseudos.c
			$(CC) -c -o $@ -Imips $(CCOPT) mips/sel.c
$(D)/mc-sel.o:		mc/sel.c mc/pseudos.c stabbsd.c
			$(CC) -c -o $@ -Imc $(CCOPT) mc/sel.c
$(D)/sparc-sel.o:	sparc/config.h sparc/sel.c sparc/pseudos.c stabbsd.c
			$(CC) -c -o $@ -Isparc $(CCOPT) sparc/sel.c

RUN=run
V=$(TARGET)-$(OS)
include ../tst/makefile
