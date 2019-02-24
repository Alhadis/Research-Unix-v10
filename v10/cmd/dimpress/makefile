MAKE=make
#
# If your system doesn't have hardware floating point make sure you include
# the -f option in the definition of CFLAGS. It may already be there, but
# I would suggest checking anyway - if you need it.
#
# Either of the following commands should take care of building and
# installing everything you'll need from this package,
#
#	make clobber install
#	make INS=mv clobber install
#
# The only real difference between these two is how the raster files are
# handled. The first one will just install a copy of the files from directory
# $(TABLEDIR)/rast*, while the second command will actually mv the raster
# files from $(TABLEDIR)/rast*. By default the raster files are copied but
# they take up about 8000 blocks and if you're short on disk space you
# may not want to keep two identical copies around.
#
# If you just want to build everything you'll need to type,
#
#	make all
#
# That will compile everything you need (dimpress and makedev) and will also
# build all the binary font files. Nothing will be installed, but the stuff
# should be easy to test because troff and dimpress both accept the -F option
# to set a new font directory. You'll also need to use the -B option to
# tell dimpress that the new raster file directory is ./TABLES.
#
# I've also included a few other programs that aren't compiled when you
# just ask for the default stuff. The target name 'allprogs' will compile
# everything I've given you. Most of these programs are really only useful
# if you're going to do some work on the raster files I've supplied.
#
# Target names tables, rastbuild, PDQ, and rastmaps should only be used
# if you've got to rebuild some of the files I've supplied. Most will
# only work properly under certain conditions. For example rastbuild
# needs to have a set of raster files that available that can be used
# with the RASTi300 file in this directory. Many of the source raster
# files come from the standard Imagen software tape for the 8/300, but
# not all of them. I strongly suggest you don't try and rebuild any of
# the tables I've supplied using these targets. It's unlikely things
# will work out properly.
#
# With that in mind I'll just mention that I built all the tables using
# a command line that looked something like,
#
# make DEVICES=i300 RASTSRC=/tmp/raster FONTSRC=/usr/src/cmd/text/troff.d tables
#
#
# Some important directories. They really need to agree with the definitions
# used by dimpress (file init.h). If you don't want to keep things in the
# places that I've picked change the definitions here and in init.h.
#


FONTDIR=/usr/lib/font
BITDIR=/usr/lib/raster


#
# A few other definitions we'll need later on. These guys really don't have
# anything to do with the code, but you may want to change them anyway. If
# you've got PDQs on your system make sure PDQDIR is correct, otherwise none
# of the PDQ file I've given you will be installed. BINDIR is where we'll
# put dimpress if we install it.
#

THISDIR=/usr/src/cmd/dimpress
BINDIR=/usr/bin
PDQDIR=$(FONTDIR)/PDQ
PUBDIR=/usr/pub

TABLEDIR=TABLES

OWNER=bin
GROUP=bin


#
# The next few definitions are only used if we need to build a new set of
# PDQ binary files.
#


PDQFRONT=/usr/m/bin/front
PDQMAKEDEV=/usr/M/bin/makedev


#
# The value assigned to INS controls whether we install stuff or just build
# it. As long as INS is : nothing will ever be installed, but assigning it
# any other value will force us to try and do the installation. ARGS controls
# what target names are built and installed. By default it's set to 'all'
# but that can be changed on the command line.
#


INS=:
ARGS=all


CFLAGS=-g -DV9

CFILES=glob.c misc.c rast.c readrast.c printrast.c buildrast.c editrast.c \
	dimpress.c impdraw.c oldrast.c draw.c res.c rotate.c bimp.c
OFILES=glob.o misc.o rast.o readrast.o printrast.o buildrast.o editrast.o \
	dimpress.o impdraw.o oldrast.o res.o rotate.o bimp.o
HFILES=ext.h gen.h init.h rast.h impcodes.h buildrast.h editrast.h \
	dimpress.h glyph.h dev.h spectab.h rotate.h imPRESS.h
XFILES=makefile RASTi300 README dimpress.1
ALLFILES=$(CFILES) $(HFILES) $(XFILES)


#
# Programs you'll find in this directory. In most cases you'll only need
# dimpress and makedev.
#


PROGS=readrast printrast buildrast dimpress makedev makemap

READRAST=glob.o misc.o readrast.o rast.o rotate.o
PRINTRAST=glob.o misc.o printrast.o rast.o rotate.o
BUILDRAST=glob.o misc.o buildrast.o rast.o editrast.o rotate.o
DIMPRESS=dimpress.o impdraw.o glob.o misc.o rast.o oldrast.o res.o rotate.o bimp.o
MAKEDEV=makedev.c
MAKEMAP=makemap.c


help :
	@echo
	@echo "Using this file make understands the following target names:"
	@echo
	@echo " help       : just prints this stuff out again"
	@echo " all        : does most everything you'll need"
	@echo " allprogs   : compiles all the programs in this directory"
	@echo " fonts      : builds all the binary font files in this package"
	@echo " raster     : installs all the raster files if INS isn't set to :"
	@echo "              if INS is mv the files will be moved rather than copied"
	@echo " pub        : installs special eqn character building files"
	@echo " clean      : removes all the .o files from this directory"
	@echo " clobber    : gets rid of everything but source"
	@echo " backup     : copies all the important files into directory BACKUP"
	@echo
	@echo


all : dimpress makedev fonts raster pub
#
#
# It's really not everything but it's most of the stuff you'll need. If
# you want to do some work on the raster files you'll probably also need
# to compile buildrast, printrast, and readrast.
#

install :
	@if [ "$(INS)" = ":" ]; \
	    then $(MAKE) INS=cp $(ARGS); \
	    else $(MAKE) INS=$(INS) $(ARGS); \
	fi

allprogs : $(PROGS)

readrast : $(READRAST)
	cc $(CFLAGS) -o readrast $(READRAST)

printrast : $(PRINTRAST)
	cc $(CFLAGS) -o printrast $(PRINTRAST)

buildrast : $(BUILDRAST)
	cc $(CFLAGS) -o buildrast $(BUILDRAST)

dimpress : $(DIMPRESS)
	cc $(CFLAGS) -o dimpress $(DIMPRESS) -lm
	if [ "$(INS)" != ":" ]; then \
	    cp dimpress $(BINDIR); \
	    chmod 755 $(BINDIR)/dimpress; \
	    chgrp $(GROUP) $(BINDIR)/dimpress; \
	    chown $(OWNER) $(BINDIR)/dimpress; \
	fi

makedev : $(MAKEDEV)
	cc $(CFLAGS) -o makedev $(MAKEDEV)

makemap : $(MAKEMAP)
	cc $(CFLAGS) -o makemap $(MAKEMAP)


glob.o : gen.h init.h
misc.o : ext.h gen.h
rast.o : ext.h gen.h rast.h
readrast.o : ext.h gen.h rast.h
printrast.o : ext.h gen.h rast.h impcodes.h
buildrast.o : ext.h gen.h init.h rast.h buildrast.h
editrast.o : gen.h rast.h buildrast.h editrast.h
dimpress.o : ext.h gen.h init.h rast.h dev.h impcodes.h dimpress.h spectab.h
oldrast.o : ext.h gen.h glyph.h impcodes.h dimpress.h
res.o : ext.h gen.h init.h rast.h impcodes.h
rotate.o : gen.h rast.h rotate.h


fonts : makedev
#
#
# Builds all the binary font files and installs them if INS isn't set to :.
# If you've got PDQs on your system make sure PDQDIR is properly defined.
# I've set it to /usr/lib/font/PDQ, which is the place that PDQ troff
# normally looks. If PDQDIR exists then the all the PDQ binary files that
# I've supplied will be installed.
#
# There's a lot of stuff to do here and I'm sure I could have written things
# better, but it all seems to work so I'm not going to change anything.
#
#
	@if [ "$(INS)" != ":"  -a  ! -d $(FONTDIR) ]; then \
	    echo "Directory $(FONTDIR) doesn't exist"; \
	    exit 1; \
	fi
	@cd $(TABLEDIR); \
	for i in dev*; do \
	    if [ ! -d "$$i" ]; then \
		continue; \
	    fi; \
	    cd $$i; \
	    echo " Building binary files in directory $$i"; \
	    $(THISDIR)/makedev DESC ? ??; \
	    if [ -r LINKFILE ]; then \
		sh LINKFILE; \
	    fi; \
	    if [ "$(INS)" != ":" ]; then \
		if [ ! -d $(FONTDIR)/$$i ]; then \
		    rm -f $(FONTDIR)/$$i; \
		    mkdir $(FONTDIR)/$$i; \
		fi; \
		echo " Installing binary files in directory $(FONTDIR)/$$i"; \
		mv *.out $(FONTDIR)/$$i; \
		chmod 644 $(FONTDIR)/$$i/*; \
		chgrp $(GROUP) $(FONTDIR)/$$i/*; \
		chown $(OWNER) $(FONTDIR)/$$i/*; \
	    fi; \
	    cd ..; \
	done
	@if [ "$(INS)" != ":"  -a  ! -d $(BITDIR) ]; then \
	    echo "Raster file directory $(BITDIR) doesn't exist"; \
	    exit 1; \
	fi
	@cd $(TABLEDIR); \
	for j in rast*; do \
	    if [ ! -d "$$j" ]; then \
		continue; \
	    fi; \
	    cd $$j; \
	    for i in dev*; do \
		if [ ! -d "$$i" ]; then \
		    continue; \
		fi; \
		cd $$i; \
		echo " Building binary files in directory $$j/$$i"; \
		$(THISDIR)/makedev DESC ? ??; \
		if [ -r LINKFILE ]; then \
		    sh LINKFILE; \
		fi; \
		if [ "$(INS)" != ":" ]; then \
		    if [ ! -d $(BITDIR)/$$j ]; then \
			rm -f $(BITDIR)/$$j; \
			mkdir $(BITDIR)/$$j; \
		    fi; \
		    if [ ! -d $(BITDIR)/$$j/$$i ]; then \
			rm -f $(BITDIR)/$$j/$$i; \
			mkdir $(BITDIR)/$$j/$$i; \
		    fi; \
		    echo " Installing binary files in $(BITDIR)/$$j/$$i"; \
		    mv *.out $(BITDIR)/$$j/$$i; \
		    chmod 644 $(BITDIR)/$$j/$$i/*; \
		    chgrp $(GROUP) $(BITDIR)/$$j/$$i/*; \
		    chown $(OWNER) $(BITDIR)/$$j/$$i/*; \
		fi; \
		cd ..; \
	    done; \
	    cd ..; \
	done
	@if [ "$(INS)" != ":"  -a  -d $(PDQDIR) ]; then \
	    cd $(TABLEDIR)/PDQ; \
	    for i in dev*; do \
		if [ ! -d "$$i" ]; then \
		    continue; \
		fi; \
		cd $$i; \
		if [ -r LINKFILE ]; then \
		    sh LINKFILE; \
		fi; \
		if [ ! -d $(PDQDIR)/$$i ]; then \
		    rm -f $(PDQDIR)/$$i; \
		    mkdir $(PDQDIR)/$$i; \
		fi; \
		echo " Installing PDQ files in directory $(PDQDIR)/$$i"; \
		cp *.out $(PDQDIR)/$$i; \
		chmod 644 $(PDQDIR)/$$i/*; \
		chgrp $(GROUP) $(PDQDIR)/$$i/*; \
		chown $(OWNER) $(PDQDIR)/$$i/*; \
		cd ..; \
	    done; \
	fi


raster :
#
#
# Handles the installation of all the raster files. The source directories
# are in $(TABLEDIR)/rast* and they files are copied from there to the
# appropriate directory in $(BITDIR).
#
# The raster files take up quite a bit of disk space and you probably don't
# want to keep two copies of them around. Normally they're copied from the
# source directories $(TABLEDIR)/rast* to $(BITDIR)/rast*, but if you set
# INS to mv they'll be moved instead.
#
#
	@if [ "$(INS)" != ":"  -a  ! -d $(BITDIR) ]; then \
	    echo "Raster file directory $(BITDIR) doesn't exist"; \
	    exit 1; \
	fi
	@if [ "$(INS)" != ":" ]; then \
	    cd $(TABLEDIR); \
	    HERE=`pwd`; \
	    for i in rast*; do \
		if [ ! -d "$$i" ]; then \
		    continue; \
		fi; \
		cd $$i; \
		if [ ! -d $(BITDIR)/$$i ]; then \
		    rm -f $(BITDIR)/$$i; \
		    mkdir $(BITDIR)/$$i; \
		fi; \
		echo " Installing raster files in $(BITDIR)/$$i"; \
		if [ "$(INS)" = "cp"  -o  "$(INS)" = "mv" ]; \
		    then $(INS) *.* RAST* $(BITDIR)/$$i; \
		    else cp *.* RAST* $(BITDIR)/$$i; \
		fi; \
		cd $(BITDIR)/$$i; \
		chmod 644 *.* RAST*; \
		chgrp $(GROUP) *.* RAST*; \
		chown $(OWNER) *.* RAST*; \
		cd $$HERE; \
	    done; \
	    if [ -d RESIDENT ]; then \
		if [ ! -d $(BITDIR)/RESIDENT ]; then \
		    rm -f $(BITDIR)/RESIDENT; \
		    mkdir $(BITDIR)/RESIDENT; \
		fi; \
		echo " Installing resident font files in $(BITDIR)/RESIDENT"; \
		cp RESIDENT/* $(BITDIR)/RESIDENT; \
		cd $(BITDIR)/RESIDENT; \
		chmod 644 *; \
		chgrp $(GROUP) *; \
		chown $(OWNER) *; \
	    fi; \
	fi


pub :
#
#
# Character building files for eqn are included in $(TABLEDIR)/pub. If
# INS isn't ":" and $(PUBDIR) exists all the files in this directory will
# be copied to $(PUBDIR). Some locations may already have their own versions
# of these files, so if any one of them already exists in directory
# $(PUBDIR) it won't be installed.
#
#
	@if [ "$(INS)" != ":"  -a  -d $(TABLEDIR)/pub ]; then \
	    cd $(TABLEDIR)/pub; \
	    if [ ! -d $(PUBDIR) ]; then \
		echo "Missing directory $(PUBDIR)"; \
		exit 1; \
	    fi; \
	    for i in *; do \
		if [ -f $(PUBDIR)/$$i ]; then \
		    echo " $(PUBDIR)/$$i already exists - $$i not installed"; \
		    continue; \
		fi; \
		echo " Installing $$i in $(PUBDIR)"; \
		cp $$i $(PUBDIR)/$$i; \
		chmod 644 $(PUBDIR)/$$i; \
		chgrp $(GROUP) $(PUBDIR)/$$i; \
		chown $(OWNER) $(PUBDIR)/$$i; \
	    done; \
	fi


tables : rastbuild PDQ rastmaps
#
# You probably will never need this target, but it's how I built most of
# the tables I've supplied. You'll need to define DEVICES, RASTSRC, and
# FONTSRC on the command line before things work. DEVICES probably should
# just be i300, RASTSRC is the directory where the source raster files
# for $(DEVICES) can be found (omitting the device name), and FONTSRC
# is where you've got the ASCII font files for the aps and i10.
#


rastbuild : buildrast
#
#
# Builds the raster files and associated fonts for $(DEVICES) using
# source raster files found in $(RASTSRC) and appropriately named control
# files found in this directory. You'll need to define both of these guys
# on the command line before things work right. In particular that means
# you'll need the source raster files that I used with RASTi300 to build
# the the rasti300 tables. I doubt you have the complete set so don't
# try using this target unless you sure you know what's happening.
#
#
	@if [ ! "$(DEVICES)" ]; then \
	    echo "No device names given"; \
	    exit 1; \
	fi
	@if [ ! -d "$(RASTSRC)" ]; then \
	    echo "Bad raster file source directory"; \
	    exit 1; \
	fi
	@echo "Building new font and raster files for devices $(DEVICES)"
	@SRCDIR=`pwd`; \
	cd $(TABLEDIR); \
	for i in $(DEVICES); do \
	    if [ ! -r $$SRCDIR/RAST$$i ]; then \
		echo " Can't read $$SRCDIR/RAST$$i - device skipped"; \
		continue; \
	    fi; \
	    if [ ! -d $(RASTSRC)/$$i ]; then \
		echo " Missing source raster files for device $$i"; \
		exit 1; \
	    fi; \
	    if [ ! -d rast$$i ]; then \
		rm -f rast$$i; \
		mkdir rast$$i; \
	    fi; \
	    if [ ! -d dev$$i ]; then \
		rm -f dev$$i; \
		mkdir dev$$i; \
	    fi; \
	    echo " Device $$i"; \
	    $$SRCDIR/buildrast -S $(RASTSRC)/$$i $$SRCDIR/RAST$$i; \
	    echo " Fixing CW spacewidth command in dev$$i"; \
	    WIDTH="spacewidth `grep '^Z' dev$$i/CW | awk '{ print $$2 }'`"; \
	    echo $$WIDTH | cat - dev$$i/CW >CW.tmp; \
	    mv CW.tmp dev$$i/CW; \
	done


PDQ :
#
#
# Builds all the binary PDQ files for devices that have directories in
# $(TABLEDIR) and moves the binary files to an appropriately named
# directory in $(TABLEDIR)/PDQ. Obviously you'll need $(PDQFRONT) and
# $(PDQMAKEDEV) if you expect to use this target.
#
#
	@echo "Building PDQ binary files"
	@cd $(TABLEDIR); \
	for i in dev*; do \
	    if [ ! -d "$$i" ]; then \
		echo " $$i not a device directory"; \
		continue; \
	    fi; \
	    if [ ! -d PDQ/$$i ]; then \
		rm -f PDQ/$$i; \
		mkdir PDQ/$$i; \
	    fi; \
	    cd $$i; \
	    echo " Device $$i"; \
	    $(PDQFRONT) $(PDQMAKEDEV) DESC ? ?? 2>/dev/null; \
	    if [ -r LINKFILE ]; then \
		sh LINKFILE; \
	    fi; \
	    echo " Moving binary PDQ files to $(TABLEDIR)/PDQ/$$i"; \
	    mv *.out ../PDQ/$$i; \
	    cd ..; \
	done


rastmaps : makedev makemap
#
#
# Builds the special font mapping files for devices that we expect will
# be using some of the supplied raster files. For example you'll find
# a devaps directory in $(TABLEDIR)/rasti300. The ASCII files are built
# by makemap using the binary i300 files, the ASCII aps files, and the
# FONTMAP file found in that directory. If there's no FONTMAP file in
# that directory we won't do anything. You'll need to define FONTSRC
# on the command line, otherwise nothing will be done.
#
#
	@if [ ! "$(FONTSRC)" ]; then \
	    echo "No font source directory given"; \
	    exit 1; \
	fi
	@echo "Building special font mapping files"
	@SRCDIR=`pwd`; \
	cd $(TABLEDIR); \
	for i in rast*; do \
	    if [ ! -d "$$i" ]; then \
		echo " Skipping $$i - not a directory"; \
		continue; \
	    fi; \
	    MASTER=`echo $$i | sed 's/rast//'`; \
	    if [ ! -d dev$$MASTER ]; then \
		echo " Can't find master font directory $(TABLEDIR)/dev$$MASTER"; \
		continue; \
	    fi; \
	    echo " Building binary font files for device $$MASTER"; \
	    cd dev$$MASTER; \
	    $$SRCDIR/makedev DESC ? ?? 2>/dev/null; \
	    if [ -r LINKFILE ]; then \
		sh LINKFILE; \
	    fi; \
	    cd ../$$i; \
	    for j in dev*; do \
		if [ ! -d "$$j" ]; then \
		    echo " No device directories in $(TABLEDIR)/$$i"; \
		    break; \
		fi; \
		if [ ! -r $$j/FONTMAP ]; then \
		    echo " No FONTMAP file in $(TABLEDIR)/$$i/$$j"; \
		    continue; \
		fi; \
		echo " Building ASCII font mapping files in $(TABLEDIR)/$$i/$$j"; \
		TARGET=`echo $$j | sed 's/dev//'`; \
		$$SRCDIR/makemap -m$$MASTER -S$(FONTSRC) -F.. $$TARGET; \
	    done; \
	    cd ..; \
	    rm -f dev$$MASTER/*.out; \
	done


clean :
	rm -f *.o

clobber :
	rm -f *.o $(PROGS)

backup :
	@if [ ! -d BACKUP ]; then \
	    mkdir BACKUP; \
	fi
	cp $(ALLFILES) BACKUP

list :
	pr $(ALLFILES) | i10send -lpr -pland -von -o5

