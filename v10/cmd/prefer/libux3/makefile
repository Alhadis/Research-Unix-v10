LIBNAME = libux3.a
CFLAGS = -O
INCRT = /usr/include
FRC =

FILES = \
	$(LIBNAME)(strchr.o) \
	$(LIBNAME)(strpbrk.o) \
	$(LIBNAME)(strspn.o) \
	$(LIBNAME)(strtok.o)

all:	$(LIBNAME) 

$(LIBNAME):	$(FILES)
	ranlib $(LIBNAME)

install:	all

clean:
	-rm -f *.o

clobber:	clean
	-rm -f $(LIBNAME)

$(LIBNAME)(%.o):	%.c
	$(CC) -c -x $(CFLAGS) $%.c
	$(AR) r $(LIBNAME) $%.o
	rm -f $%.o

$(LIBNAME)(strchr.o): \
	$(FRC)

$(LIBNAME)(strpbrk.o): \
	$(FRC)

$(LIBNAME)(strspn.o): \
	$(FRC)

$(LIBNAME)(strtok.o): \
	$(FRC)

FRC:
