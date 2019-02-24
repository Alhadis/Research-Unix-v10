#
#	@(#)Makefile	1.2 (Berkeley) 10/18/80
#
#	error.h			error header file
#	errorinput.c		does all of the input, and canonicalizes errors
#	errorpi.c		canonicalizes pi errors
#	errormain.c		main
#	errorsubr.c		does grotty subroutine work
#	errorfilter.c		"greps" out unwanted messages
#	errortouch.c		touches all mentioned files
#

HDRS = error.h
PHDRS = $(HDRS)
SRCS = errormain.c errorinput.c errorpi.c errorsubr.c errorfilter.c errortouch.c
PSRCS = $(SRCS)
OBJS = errormain.o errorinput.o errorpi.o errorsubr.o errorfilter.o errortouch.o

CFLAGS =  -Od2 $(DFLAGS)

LINTFLAGS = $(DFLAGS)

error:	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o error

$(OBJS): $(HDRS)

install:
	echo "I won't do it"
	: strip error
	: mv error $(DESTDIR)/usr/bin

clean:
	rm -f error $(OBJS)

lint:	
	lint $(LINTFLAGS) $(SRCS)

psrcs:
	echo $(PHDRS) $(PSRCS)

print:
	print $(PHDRS) $(PSRCS)
