MCC=3cc
MLD=bin/mld
LIB=../lib/j
JERQ=/usr/jerq
CFILES=$(LIB)/put.c $(LIB)/printf.c $(LIB)/comm.c $(LIB)/fopen.c\
	$(LIB)/rdwr.c $(LIB)/data.c $(LIB)/get.c $(LIB)/popen.c\
	$(LIB)/exit.c

all: jx sysint

jx: jx.c
	cc -o jx -I/usr/include -I$(JERQ)/include jx.c

.c.o:
	$(MCC) $(CFLAGS) -c $<

sysint: sysint.c comm.h
	cc -o sysint sysint.c

comm.h: $(CFILES)
	grep "define._SYS" $(CFILES) | sed "s/.*://" > comm.h

print:
	@pr $(JERQ)/include/jerqio.h comm.h $(CFILES)

install:	jx sysint
	cp jx $(JERQ)/bin
	cp sysint $(JERQ)/lib

clean:
	rm -f core *.o hello sysint jx
