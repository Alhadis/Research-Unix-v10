JERQBIN=/usr/jerq/bin
JERQMBIN=/usr/jerq/mbin
JERQINCLUDE=/usr/jerq/include
JERQFONT=/usr/jerq/font
CC=$(JERQBIN)/3cc
CFLAGS=-g
OBJ=main.o tterm.o troff.o pan.o confirm.o pkt.o reader.o sundry.o speed.o
SIZE=3size

all:	proof.m

proof.m:	$(OBJ)
	$(CC) $(CFLAGS) -o proof.m $(OBJ)
	@$(SIZE) proof.m

$(OBJ):	term.h ../comm.h

install:	$(JERQMBIN)/proof.m

$(JERQMBIN)/proof.m:	proof.m
	cp proof.m $(JERQMBIN)/proof.m

pp:
	pp -fPO -t"Term Proof" Makefile ../comm.h term.h main.c confirm.c pan.c pkt.c reader.c speed.c sundry.c tterm.c troff.c | dcan

lint:
	lint -I/usr/jerq/include -DMUX -Dvoid=int -n main.c tterm.c troff.c pan.c speed.c pkt.c confirm.c reader.c sundry.c -lj
clean:
	rm -f *.o proof.m core
