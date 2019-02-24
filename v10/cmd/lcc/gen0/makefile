TARGET=vax
OS=bsd
CFLAGS=-g
LDFLAGS=-g
INCLUDES=-I. -I../c

D=$(TARGET)-$(OS)

all:	$(D)/rcc

OBJS=$(D)/gen.o

$(D)/gen.o:	gen.c
		$(CC) -c -o $@ $(CFLAGS) $(INCLUDES) gen.c

include ../c/makefile

$(COBJS) $(OBJS):	config.h

RUN=run
V=$(TARGET)-$(OS)
include ../tst/makefile
