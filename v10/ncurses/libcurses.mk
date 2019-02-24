#	@(#) libcurses.mk: 1.1 10/15/83
#
#	Curses Library High Level Makefile.
#	To compile libcurses & the tic database compiler normally, type:
#		"make all tic"
#	To compile a particular file with normal compilation type:
#		"make FILES='<particular .o files>"
#	If debugging is desired then type:
#		"make O=debug FILES='<particular .o files>"
#	If tracing is desired then type:
#		"make O=trace FILES='<particular .o files>"
#	If profiling is desired then type:
#		"make O=profile FILES='<particular .o files>"
#	To compile the show program type:
#		"make show"
#	To compile only the tic compiler type:
#		"make tic"
#	To compile the test program ti4 that uses MINICURSES type:
#		"make ti4"
#
SHELL=/bin/sh
all:
	cd screen ; make all
	@echo "The libcurses.a & tic have been made."

libcurses.a:
	cd screen ; make libcurses.a
	@echo "Libcurses.a has been made."

ti4:
	cd screen ; make ti4
	@echo "The ti4 program has been made."
	
tic:
	cd screen ; make tic
	@echo "The tic compiler has been made."
	
show:
	cd screen ; make show
	@echo "The show program has been made."
	
install:
	cd screen ; make install
	-mkdir /usr/lib/terminfo
	@echo It will take 20 minutes or more to generate /usr/lib/terminfo/*/*.
	(cd terminfo ; make all 2>&1 > errs ; echo "tic done" )

clean:
	cd screen ; make clean
	cd terminfo ; make clean

clobber:
	cd screen ; make clobber
	cd terminfo ; make clean
