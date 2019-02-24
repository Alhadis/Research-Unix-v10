#ident	"@(#)ctrans:demangler/makefile	1.3"
# 
#  C++ Demangler Source Code
#  @(#)master	1.5
#  7/27/88 13:54:37
#
CC = cc
CCFLAGS = -O
TARGETS = dem c++filt errfilt

all: $(TARGETS)

OFILES= nplist.o String.o demangle.o args.o

mangle.a: $(OFILES)
	rm -f mangle.a
	ar cr mangle.a `lorder $(OFILES) | tsort`
	if [ -x /usr/bin/ranlib ]; then ranlib mangle.a; fi

dem: dem.o mangle.a
	$(CC) $(CCFLAGS) -o dem dem.o mangle.a

c++filt: c++filt.o mangle.a
	$(CC) $(CCFLAGS) -o c++filt c++filt.o mangle.a

errfilt: errfilt.c
	$(CC) $(CCFLAGS) -o errfilt errfilt.c

clean:
	rm -f *.o mangle.a

clobber: clean
	rm -f $(TARGETS)

cpio: dem.cpio
	cat files | cpio -ocvu > dem.cpio
