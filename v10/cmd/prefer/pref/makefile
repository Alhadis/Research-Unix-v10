PLIB = /usr/lib/prefer
PBIN = /usr/bin
PHOME = /usr/lib/prefer

CFLAGS = -O
#CFLAGS = "-O -DEXPTOOLS"
#CFLAGS = "-O -DBSD"
LDFLAGS = -s
#LDFLAGS = "-s $(PLIB)/libux3.a"
LINT = cyntax

LFLAGS =-Verrexit:2 -Vfprintf:2

FRC =


PREFER	= prefer.o locate.o alpha.seek.o streams.o makekey.o libpath.o
PINV	= invert.o streams.o  makekey.o  libpath.o
PLOOK	= lookup.o locate.o alpha.seek.o streams.o makekey.o libpath.o

PPROGS = prefer pinvert plook

all:	$(PPROGS)

prefer:	$(PREFER)
	$(CC) $(CFLAGS) $(PREFER) $(LDFLAGS)  -o prefer

plook: $(PLOOK)
	$(CC) $(CFLAGS) $(PLOOK) $(LDFLAGS) -o plook

pinvert:  $(PINV)
	$(CC) $(CFLAGS) $(PINV) $(LDFLAGS) -o pinvert


lint:	lintprefer lintplook lintpinvert

lintprefer: 
	@$(LINT) $(LFLAGS) -DPLIB=\"$(PHOME)\" `echo $(PREFER) | sed -e 's/\.o/.c/g'`

lintplook:
	@$(LINT) $(LFLAGS) -DPLIB=\"$(PHOME)\" `echo $(PLOOK) | sed -e 's/\.o/.c/g'`

lintpinvert:
	@$(LINT) $(LFLAGS) -DPLIB=\"$(PHOME)\" `echo $(PINV) | sed -e 's/\.o/.c/g'`






lcomppinvert:
	lcomp -DPLIB=\"$(PHOME)\" `echo $(PINV) | sed -e 's/\.o/.c/g'`
lcompprefer:
	lcomp -DPLIB=\"$(PHOME)\" `echo $(PREFER) | sed -e 's/\.o/.c/g'`
lcompplook:
	lcomp -DPLIB=\"$(PHOME)\" `echo $(PLOOK) | sed -e 's/\.o/.c/g'`

profprefer:	$(PREFER)
	$(CC) -p $(CFLAGS) -o pprefer $(PREFER)

profplook: $(PLOOK)
	$(CC) -p $(CFLAGS) -o pplook $(PLOOK)  

profpinvert:  $(PINV)
	$(CC) -p $(CFLAGS) -o ppinvert $(PINV)

install: all
	cp $(PPROGS) $(PBIN)

lint: lintprefer lintplook lintpinvert

clean:
	-rm -f *.o

clobber: clean
	-rm -f prefer plook pinvert


print:
	prican makefile *.h *.c



prefer.o:	streams.h bib.h
	$(CC) $(CFLAGS) -c -DPLIB=\"$(PHOME)\" prefer.c

phunt.o:	streams.h bib.h
	$(CC) $(CFLAGS) -c phunt.c

invert.o:       streams.h bib.h
	$(CC) $(CFLAGS) -c -DPLIB=\"$(PHOME)\" invert.c

lookup.o:       streams.h bib.h
	$(CC) $(CFLAGS) -c -DPLIB=\"$(PHOME)\" lookup.c

locate.o:       streams.h bib.h

makekey.o:	bib.h

alpha.seek.o:   streams.h

streams.o:      streams.h
