#
# Top level makefile. Instructions are included here and in the README file.
#
# First save a copy of this file. Then adjust the following definitions (all
# come immediatedly after the initial block of comments):
#
#     MAKE	where make lives
#
#     MAKEFILE	name of this file - for recursive make calls. Must change
#		if you rename this file.
#
#     SYSTEM	best match for your version of Unix. Current choices for
#		SYSTEM are:
#
#			SYSV	- System V
#			V9	- Ninth Edition
#			BSD4_2	- Berkeley (eg. Sun)
#
#		Controls conditional compilation in a few places.
#
#     VERSION	refers to the Version of the DWB package
#
#     GROUP	group assigned to all installed files
#
#     OWNER	owner of everything that's installed
#
#     BINDIR	dpost and picpack go here. All other programs go in POSTBIN.
#		BINDIR must already exist - it will not created during an
#		install.
#
#     HOSTDIR	hostresident font directory for PostScript printers. Only
#		used in the font download program.
#
#     FONTDIR	width table directory - for troff and dpost
#
#     MAN1DIR	command manpages. A command and its manpage are installed
#		together - there's no easy way to avoid it. Setting MAN1DIR
#		to an existing temporary directory (e.g. /tmp) means an
#		install will work but manpages won't go anywhere permanent.
#		MAN1DIR must already exist - it will not be created during
#		an install.
#
#     POSTBIN	where most PostScript support programs go. dpost and picpack
#		are the exceptions.
#
#     POSTLIB	prologues and miscellaneous PostScript files. Primarily for
#		the programs that live in POSTBIN.
#
#     CFLGS	common compiler options - used to build CFLAGS in the low
#		level makefiles. CFLGS and LDFLGS are best set on the make
#		command line.
#
#     LDFLGS	common link editor options - used to build LDFLAGS in the
#		low level makefiles. LDFLGS and CFLGS are best set on the
#		make command line.
#
#     DKHOST	set it to TRUE to compile the DKHOST Datakit support code
#		in postio. Temporarily resets SYSTEM to SYSV if DKHOST is
#		TRUE and SYSTEM is BSD4_2. Ignored if SYSTEM is not SYSV
#		or BSD4_2.
#
#     DKSTREAMS	set it to TRUE only if you have the streams based DKHOST
#		package. Ignored if SYSTEM is not SYSV or BSD4_2.
#
#     DOROUND	must only be set to TRUE or FALSE. TRUE means translators
#		include code that maps clipping path dimensions into known
#		paper sizes.
#
#     TARGETS	the default list of what's built by make. Each target must
#		be the name of a source directory. A target that names a
#		non-existent source directory is ignored. Setting TARGETS
#		on the make command line overrides the default list.
#
# Source files must be updated whenever this file changes. If you change any
# definitions type,
#
#	make -f postscript.mk changes
#
# to update the source files, man pages, and low level makefiles.
#
# To build (but not install) the default package (i.e. everything named by
# TARGETS) type,
#
#	make -f postscript.mk all
#
# The recommended way to build and install the package is,
#
#	make -f postscript.mk all install
#
# Although you'll likely have to be root for the install to work.
#
# After the package is installed use,
#
#	make -f postscript.mk clobber
#
# to delete binary files and compiled programs from the source directories.
#
# Set TARGETS on the command line to select part of the package. For example,
#
#	make -f postscript.mk TARGETS="dpost devpost" all install
#
# builds and installs dpsot and the PostScript font tables. Quotes hide white
# space from the shell.
#

MAKE=/bin/make
MAKEFILE=postscript.mk

SYSTEM=SYSV
VERSION=3.2

GROUP=bin
OWNER=bin

BINDIR=/usr/bin
FONTDIR=/usr/lib/font
HOSTDIR=/usr/lib/font/postscript
MAN1DIR=/usr/man/u_man/man1
POSTBIN=/usr/lbin/postscript
POSTLIB=/usr/lib/postscript

COMMONDIR=common
CURRENTDIR=.

CFLGS=-O
LDFLGS=-s

DKHOST=FALSE
DKSTREAMS=FALSE
ROUNDPAGE=FALSE

#
# $(TARGETS) is the default list of things built by make.
#

TARGETS=buildtables\
	common\
	cropmarks\
	devLatin1\
	devpost\
	download\
	dpost\
	grabit\
	hardcopy\
	picpack\
        postbgi\
        postdaisy\
        postdmd\
	postgif\
	postio\
	postmd\
	postplot\
        postprint\
	postreverse\
        posttek\
	printfont\
	psencoding\
	psfiles\
	trofftable

ACTION=all

all : $(TARGETS)

clean clobber :
	@$(MAKE) -e -f $(MAKEFILE) MAKE=$(MAKE) ACTION=$@ $(TARGETS)

install :
	@SYSTEM='$(SYSTEM)'; export SYSTEM; \
	VERSION='$(VERSION)'; export VERSION; \
	GROUP='$(GROUP)'; export GROUP; \
	OWNER='$(OWNER)'; export OWNER; \
	BINDIR='$(BINDIR)'; export BINDIR; \
	FONTDIR='$(FONTDIR)'; export FONTDIR; \
	HOSTDIR='$(HOSTDIR)'; export HOSTDIR; \
	MAN1DIR='$(MAN1DIR)'; export MAN1DIR; \
	POSTBIN='$(POSTBIN)'; export POSTBIN; \
	POSTLIB='$(POSTLIB)'; export POSTLIB; \
	$(MAKE) -e -f $(MAKEFILE) MAKE=$(MAKE) ACTION=$@ $(TARGETS)

$(TARGETS) ::
	@TARGETS=; unset TARGETS; \
	HFILES=; unset HFILES; \
	OFILES=; unset OFILES; \
	CFLAGS=; unset CFLAGS; \
	LDFLAGS=; unset LDFLAGS; \
	YFLAGS=; unset YFLAGS; \
	SYSTEM='$(SYSTEM)'; export SYSTEM; \
	VERSION='$(VERSION)'; export VERSION; \
	CFLGS='$(CFLGS)'; export CFLGS; \
	LDFLGS='$(LDFLGS)'; export LDFLGS; \
	COMMONDIR='../$(COMMONDIR)'; export COMMONDIR; \
	if [ $@ = postio ]; then \
	    EXTRA=; unset EXTRA; \
	    DKLIB=" "; export DKLIB; \
	    if [ $(SYSTEM) = SYSV -a $(DKHOST) = TRUE ]; then \
		DKLIB=-ldk; export DKLIB; \
		CFLGS="$(CFLGS) -DDKHOST"; export CFLGS; \
		if [ "$(DKSTREAMS)" = TRUE ]; then \
		    CFLGS="$$CFLGS -DDKSTREAMS"; export CFLGS; \
		fi; \
	    fi; \
	    if [ $(SYSTEM) = BSD4_2 -a $(DKHOST) = TRUE ]; then \
		DKLIB=-ldk; export DKLIB; \
		CFLGS="$(CFLGS) -DDKHOST -DDKSTREAMS"; export CFLGS; \
		SYSTEM=SYSV; export SYSTEM; \
	    fi; \
	    if [ $(SYSTEM) = V9 ]; then \
		DKLIB=-lipc; export DKLIB; \
	    fi; \
	fi; \
	if [ -d $@ -a -f $@/$@.mk ]; then  \
	    cd $@; \
	    echo "---- Making $(ACTION) in directory $(CURRENTDIR)/$@ ----"; \
	    $(MAKE) -e -f $@.mk MAKE=$(MAKE) $(ACTION); \
	    echo; \
	fi

changes :
	@SYSTEM='$(SYSTEM)'; export SYSTEM; \
	VERSION='$(VERSION)'; export VERSION; \
	GROUP='$(GROUP)'; export GROUP; \
	OWNER='$(OWNER)'; export OWNER; \
	BINDIR='$(BINDIR)'; export BINDIR; \
	FONTDIR='$(FONTDIR)'; export FONTDIR; \
	HOSTDIR='$(HOSTDIR)'; export HOSTDIR; \
	MAN1DIR='$(MAN1DIR)'; export MAN1DIR; \
	POSTBIN='$(POSTBIN)'; export POSTBIN; \
	POSTLIB='$(POSTLIB)'; export POSTLIB; \
	DKHOST='$(DKHOST)'; export DKHOST; \
	DKSTREAMS='$(DKSTREAMS)'; export DKSTREAMS; \
	ROUNDPAGE='$(ROUNDPAGE)'; export ROUNDPAGE; \
	$(MAKE) -e -f $(MAKEFILE) MAKE=$(MAKE) ACTION=$@ $(TARGETS)
	@trap "" 1 2 3 15; \
	sed \
	    -e "s'^SYSTEM=.*'SYSTEM=$(SYSTEM)'" \
	    -e "s'^VERSION=.*'VERSION=$(VERSION)'" \
	    -e "s'^GROUP=.*'GROUP=$(GROUP)'" \
	    -e "s'^OWNER=.*'OWNER=$(OWNER)'" \
	    -e "s'^BINDIR=.*'BINDIR=$(BINDIR)'" \
	    -e "s'^FONTDIR=.*'FONTDIR=$(FONTDIR)'" \
	    -e "s'^HOSTDIR=.*'HOSTDIR=$(HOSTDIR)'" \
	    -e "s'^MAN1DIR=.*'MAN1DIR=$(MAN1DIR)'" \
	    -e "s'^POSTBIN=.*'POSTBIN=$(POSTBIN)'" \
	    -e "s'^POSTLIB=.*'POSTLIB=$(POSTLIB)'" \
	    -e "s'^DKHOST=.*'DKHOST=$(DKHOST)'" \
	    -e "s'^DKSTREAMS=.*'DKSTREAMS=$(DKSTREAMS)'" \
	    -e "s'^ROUNDPAGE=.*'ROUNDPAGE=$(ROUNDPAGE)'" \
	$(MAKEFILE) >XXX.mk; \
	mv XXX.mk $(MAKEFILE)

