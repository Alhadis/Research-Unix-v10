JERQBIN=/usr/jerq/bin
JERQM=/usr/jerq/mbin/proof.m
JERQINCLUDE=/usr/jerq/include
JERQFONT=/usr/jerq/font
JERQMISSING=$(JERQFONT)/MISSING
JLD=$(JERQBIN)/32ld
OBJ=main.o io.o font.o map.o troff.o page.o

LFLAGS=-DJERQM=\"$(JERQM)\" -DJLD=\"$(JLD)\" -DJERQMISSING=\"$(JERQMISSING)\" -DJERQFONT=\"$(JERQFONT)\"
CFLAGS=$(LFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $<

proof: $(OBJ)
	$(CC) $(CFLAGS) -o proof $(OBJ)

main.o:	main.c ../comm.h
font.o:	font.c ../comm.h
io.o:	io.c ../comm.h

clean:
	rm -f *.o proof 

install:	$(JERQBIN)/proof

$(JERQBIN)/proof:	proof
	cp proof $(JERQBIN)/proof
	strip $(JERQBIN)/proof

pp:
	pp -t"Host Proof" Makefile ../comm.h host.h main.c font.c io.c map.c page.c troff.c | dcan

lint:
	lint $(LFLAGS) main.c io.c font.c map.c troff.c page.c
