MHOME = /usr/lib/monk
MBIN = /usr/bin

MAKE = make

LDFLAGS = -s
FRC =

CFLAGS = -O -I.
INSDIR = $(MHOME)

PROGS = \
	crunch \
	permute \
	sequal \
	subindex

SEDS = \
	doclean \
	dofont \
	undofont

all:	$(PROGS)

crunch: crunch.o
	$(CC) crunch.o $(LDFLAGS) -o crunch

permute: permute.o
	$(CC) permute.o $(LDFLAGS) -o permute

sequal: sequal.o
	$(CC) sequal.o $(LDFLAGS) -o sequal

subindex: subindex.o
	$(CC) subindex.o $(LDFLAGS) -o subindex

clean:
	-rm -f *.o

clobber: clean
	-rm -f $(PROGS)

install: all
	cp $(PROGS) $(MHOME)
	cp $(SEDS) $(MHOME)
	cp dosort.sh $(MHOME)/dosort
	cp doindex.sh $(MHOME)/doindex
