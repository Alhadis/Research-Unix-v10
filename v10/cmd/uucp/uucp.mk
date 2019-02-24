#	@(#)uucp.mk	1.28
#	replace these words with a descriptive comment

#	/*  11/45, 11/70, and VAX version ('-i' has no effect on VAX)	*/
#	/* for 11/23, 11/34 (without separate I/D), IFLAG= */

CFLAGS = -O		# for normal case
CFLAGS = -O -DSMALL  # use on small machine (3B2) (reduces debugging statements)
CC=cc
IFLAG =
LDFLAGS= -s ${IFLAG}
#	NOTE - when UNETLIB is to be used, define UNET in parms.h
# add eio.o to PROTOCOLS below
# UNETLIB = -lunet
#	NOTE - when TCP is defined in parms.h
# add eio.o to PROTOCOLS below
#	NOTE - when DKLIB is to be used, define DATAKIT in parms.h
# add dio.o to PROTOCOLS below
# DKLIB = -ldk
# 	NOTE - some sites use -lbtl for compatibility
# BTLLIB = -lbtl 

# PROTOCOLS= dio.o	# for datakit
PROTOCOLS =	# if no datakit and no unet

LIBS= ${BTLLIB} ${DKLIB} ${UNETLIB}

# some versions of unix don't have strpbrk in libc
# STRPBRK = strpbrk.o
# STRPBRKSRC = strpbrk.c
# Use my getopt until the argument gobbling is fixed!

# use this on systems that don't have getopt() in libc
# GETOPT = getopt.o
# GETOPTSRC = getopt.c

# BSD can't compile uugetty -- remove next line for BSD
UUGETTY=uugetty

OWNER=uucp
GRP=daemon
BIN=$(ROOT)/usr/bin
INSDIR=$(ROOT)/usr/lib/uucp
UPATH=.
# The a.out files go to ODIR
ODIR=.

INS=cpset
# If you system does not have "cpset"
# use the one in the uucp source directory and use the
#  next INS=../uucp/Cpset line instead of the INS=cpset line above.
# INS=../uucp/Cpset

# OLD=-o will create OLDname when install is executed before copy.

# if you change these directories, change them in uucp.h as well

LOCKS=$(ROOT)/usr/spool/locks
PUBDIR=$(ROOT)/usr/spool/uucppublic
SPOOL=$(ROOT)/usr/spool/uucp
#		things are linked from SPOOL into XQTDIR and CORRUPT
XQTDIR=	$(SPOOL)/.Xqtdir
#		this is optional
CORRUPT=$(SPOOL)/.Corrupt
#		for logfiles
LOGDIR=$(SPOOL)/.Log
#		for sequence number files
SEQDIR=$(SPOOL)/.Sequence
#		for STST files
STATDIR=$(SPOOL)/.Status
LOGUUCP=$(LOGDIR)/uucp
LOGUUX=$(LOGDIR)/uux
LOGUUXQT=$(LOGDIR)/uuxqt
LOGCICO=$(LOGDIR)/uucico
WORKSPACE=$(SPOOL)/.Workspace
ADMIN=$(SPOOL)/.Admin
#		for saving old log files
OLDLOG=$(SPOOL)/.Old
#		maximum number of uuxqts at one file in this file
MAXUUXQTS=$(INSDIR)/Maxuuxqts

#		Maximum number of uuscheds at one time
MAXUUSCHEDS=$(INSDIR)/Maxuuscheds

CLEAN=
LINTOP=
COMMANDS=uucp uux uuxqt uucico uusched
TOOLS=uuname uucheck uustat uucleanup $(UUGETTY)
OFILES=utility.o cpmv.o expfile.o gename.o getpwinfo.o \
	ulockf.o xqt.o logent.o versys.o gnamef.o systat.o \
	$(GETOPT)
LFILES=utility.c cpmv.c expfile.c gename.c getpwinfo.c \
	ulockf.c xqt.c logent.c versys.c gnamef.c systat.c \
	$(GETOPTSRC)
OUUCP=uucpdefs.o uucp.o gwd.o permission.o getargs.o uucpname.o\
	versys.o gtcfile.o $(STRPBRK) chremdir.o 
LUUCP=uucpdefs.c uucp.c gwd.c permission.c getargs.c uucpname.c\
	versys.c gtcfile.c $(STRPBRKSRC) chremdir.c 
OUUX=uucpdefs.o uux.o gwd.o permission.o getargs.o getprm.o\
	uucpname.o versys.o gtcfile.o chremdir.o $(STRPBRK)
LUUX=uucpdefs.c uux.c gwd.c permission.c getargs.c getprm.c\
	uucpname.c versys.c gtcfile.c chremdir.c $(STRPBRKSRC)
OUUXQT=uucpdefs.o uuxqt.o mailst.o getprm.o gnamef.o uucpname.o \
	permission.o getargs.o gtcfile.o $(STRPBRK) \
	shio.o chremdir.o
LUUXQT=uucpdefs.c uuxqt.c mailst.c getprm.c gnamef.c uucpname.c \
	permission.c getargs.c gtcfile.c $(STRPBRKSRC) \
	shio.c chremdir.c
OUUCICO=uucpdefs.o cico.o conn.o callers.o cntrl.o pk0.o pk1.o \
	gio.o xio.o anlwrk.o permission.o getargs.o gnamef.o \
	gnxseq.o pkdefs.o imsg.o gtcfile.o \
	mailst.o uucpname.o line.o chremdir.o $(STRPBRK) $(PROTOCOLS)
LUUCICO=uucpdefs.c cico.c conn.c callers.c cntrl.c pk0.c pk1.c \
	gio.c xio.c anlwrk.c permission.c getargs.c gnamef.c \
	gnxseq.c pkdefs.c imsg.c gtcfile.c \
	mailst.c uucpname.c line.c chremdir.c $(STRPBRKSRC) eio.c dio.c
OUUNAME=uuname.o uucpname.o uucpdefs.o getpwinfo.o
LUUNAME=uuname.c uucpname.c uucpdefs.c getpwinfo.c
OUUSTAT=uustat.o gnamef.o expfile.o uucpdefs.o getpwinfo.o ulockf.o getargs.o \
	utility.o uucpname.o versys.o $(STRPBRK) 
LUUSTAT=uustat.c gnamef.c expfile.c uucpdefs.c getpwinfo.c ulockf.c getargs.c \
	utility.c uucpname.c versys.c $(STRPBRKSRC) 
OUUSCHED=uusched.o gnamef.o expfile.o uucpdefs.o getpwinfo.o ulockf.o \
	systat.o getargs.o utility.o
LUUSCHED=uusched.c gnamef.c expfile.c uucpdefs.c getpwinfo.c ulockf.c \
	systat.c getargs.c utility.c
OUUCLEANUP=uucleanup.o gnamef.o expfile.o uucpdefs.o getpwinfo.o \
	uucpname.o ulockf.o getargs.o cpmv.o utility.o
LUUCLEANUP=uucleanup.c gnamef.c expfile.c uucpdefs.c getpwinfo.c \
	uucpname.c ulockf.c getargs.c cpmv.c utility.c
OUUGETTY=uugetty.o ulockf.o uucpdefs.o
LUUGETTY=uugetty.c ulockf.c uucpdefs.c

INIT=init

all:	$(INIT) $(COMMANDS) $(TOOLS) 

install:	mkdirs all shells cp

shells:
	#	For cross environment, copy default files
	if [ -n "$(CH)" ]; then cp Devices Dialers Systems Permissions Poll Dialcodes $(ROOT)/usr/lib/uucp/; fi
	$(CH)/bin/sh SetUp
	echo 2 > $(MAXUUXQTS)
	echo 2 > $(MAXUUSCHEDS)
	$(CH)-chown $(OWNER) $(MAXUUXQTS)
	$(CH)-chmod 444 $(MAXUUXQTS)
	$(CH)-chown $(OWNER) $(MAXUUSCHEDS)
	$(CH)-chmod 444 $(MAXUUSCHEDS)
	$(INS) $(OLD) uudemon.poll $(INSDIR)/uudemon.poll 555 $(OWNER) $(GRP)
	$(INS) $(OLD) uudemon.cleanup $(INSDIR)/uudemon.cleanup 555 $(OWNER) $(GRP)
	$(INS) $(OLD) uudemon.hour $(INSDIR)/uudemon.hour 555 $(OWNER) $(GRP)
	$(INS) $(OLD) uudemon.admin $(INSDIR)/uudemon.admin 555 $(OWNER) $(GRP)
	$(INS) $(OLD) remote.unknown $(INSDIR)/remote.unknown 555 $(OWNER) $(GRP)
	$(INS) $(OLD) Uutry $(INSDIR)/Uutry 555 $(OWNER) $(GRP)
	$(INS) $(OLD) SetUp $(INSDIR)/SetUp 555 $(OWNER) $(GRP)
	$(INS) $(OLD) uulog $(BIN)/uulog 555 $(OWNER) $(GRP)
	$(INS) $(OLD) uuto $(BIN)/uuto 555 $(OWNER) $(GRP)
	$(INS) $(OLD) uupick $(BIN)/uupick 555 $(OWNER) $(GRP)
	$(CH)./uucheck

cp:	all 
	$(INS) $(OLD) uuname $(BIN)/uuname 4111 $(OWNER) $(GRP)
	$(INS) $(OLD) uucp $(BIN)/uucp 4111 $(OWNER) $(GRP)
	$(INS) $(OLD) uux $(BIN)/uux 4111 $(OWNER) $(GRP)
	$(INS) $(OLD) uustat $(BIN)/uustat 4111 $(OWNER) $(GRP)

	$(INS) $(OLD) uuxqt $(INSDIR)/uuxqt 4111 $(OWNER) $(GRP)
	$(INS) $(OLD) uucico $(INSDIR)/uucico 4111 $(OWNER) $(GRP)
	$(INS) $(OLD) uusched $(INSDIR)/uusched 4111 $(OWNER) $(GRP)
	$(INS) $(OLD) uugetty $(INSDIR)/uugetty 111 $(OWNER) $(GRP)

# uucheck should only be run by root or uucp administrator
# uucleanup should only be run by root or uucp administrator
	$(INS) $(OLD) uucheck $(INSDIR)/uucheck 110 $(OWNER) $(GRP)
	$(INS) $(OLD) uucleanup $(INSDIR)/uucleanup 110 $(OWNER) $(GRP)

restore:
	$(CH)-chmod u+w $(BIN)/uucp
	$(CH)-mv $(BIN)/OLDuucp $(BIN)/uucp
	$(CH)-chown $(OWNER) $(BIN)/uucp
	$(CH)-chgrp $(GRP) $(BIN)/uucp
	$(CH)-chmod 4111 $(BIN)/uucp
	$(CH)-chmod u+w $(BIN)/uux
	$(CH)-mv $(BIN)/OLDuux $(BIN)/uux
	$(CH)-chown $(OWNER) $(BIN)/uux
	$(CH)-chgrp $(GRP) $(BIN)/uux
	$(CH)-chmod 4111 $(BIN)/uux
	$(CH)-chmod u+w $(INSDIR)/uuxqt
	$(CH)-mv $(INSDIR)/OLDuuxqt $(INSDIR)/uuxqt
	$(CH)-chown $(OWNER) $(INSDIR)/uuxqt
	$(CH)-chgrp $(GRP) $(INSDIR)/uuxqt
	$(CH)-chmod 4111 $(INSDIR)/uuxqt
	$(CH)-chmod u+w $(INSDIR)/uucico
	$(CH)-mv $(INSDIR)/OLDuucico $(INSDIR)/uucico
	$(CH)-chown $(OWNER) $(INSDIR)/uucico
	$(CH)-chgrp $(GRP) $(INSDIR)/uucico
	$(CH)-chmod 4111 $(INSDIR)/uucico
	$(CH)-chmod u+w $(BIN)/uuname
	$(CH)-mv $(BIN)/OLDuuname $(BIN)/uuname
	$(CH)-chown $(OWNER) $(BIN)/uuname
	$(CH)-chgrp $(GRP) $(BIN)/uuname
	$(CH)-chmod 4111 $(BIN)/uuname
	$(CH)-chmod u+w $(INSDIR)/uucheck
	$(CH)-mv $(INSDIR)/OLDuucheck $(INSDIR)/uucheck
	$(CH)-chown $(OWNER) $(INSDIR)/uucheck
	$(CH)-chgrp $(GRP) $(INSDIR)/uucheck
	$(CH)-chmod u+w $(INSDIR)/uucleanup
	$(CH)-mv $(INSDIR)/OLDuucleanup $(INSDIR)/uucleanup
	$(CH)-chown $(OWNER) $(INSDIR)/uucleanup
	$(CH)-chgrp $(GRP) $(INSDIR)/uucleanup
	$(CH)-chmod u+w $(BIN)/uustat
	$(CH)-mv $(BIN)/OLDuustat $(BIN)/uustat
	$(CH)-chown $(OWNER) $(BIN)/uustat
	$(CH)-chgrp $(GRP) $(BIN)/uustat
	$(CH)-chmod 4111 $(BIN)/uustat
	$(CH)-chmod u+w $(BIN)/uulog
	$(CH)-mv $(BIN)/OLDuulog $(BIN)/uulog
	$(CH)-chown $(OWNER) $(BIN)/uulog
	$(CH)-chgrp $(GRP) $(BIN)/uulog
	$(CH)-chmod 555 $(BIN)/uulog
	$(CH)-chmod u+w $(INSDIR)/uusched
	$(CH)-mv $(INSDIR)/OLDuusched $(INSDIR)/uusched
	$(CH)-chown $(OWNER) $(INSDIR)/uusched
	$(CH)-chgrp $(GRP) $(INSDIR)/uusched
	$(CH)-chmod 4111 $(INSDIR)/uusched
	$(CH)-mv $(INSDIR)/OLDuugetty $(INSDIR)/uugetty
	$(CH)-chown $(OWNER) $(INSDIR)/uugetty
	$(CH)-chgrp $(GRP) $(INSDIR)/uugetty
	$(CH)-chmod 555 $(INSDIR)/uugetty

clean:
	-rm -f *.o

clobber:	clean
	-rm -f $(COMMANDS) $(TOOLS)

burn:
	-rm -f $(BIN)/OLDuucp
	-rm -f $(BIN)/OLDuux
	-rm -f $(INSDIR)/OLDuuxqt
	-rm -f $(INSDIR)/OLDuucico
	-rm -f $(BIN)/OLDuuname
	-rm -f $(INSDIR)/OLDuucheck
	-rm -f $(INSDIR)/OLDuucleanup
	-rm -f $(BIN)/OLDuustat
	-rm -f $(INSDIR)/OLDuusched
	-rm -f $(INSDIR)/OLDuugetty

cmp:	all
	cmp uucp $(BIN)/uucp
	rm uucp
	cmp uux $(BIN)/uux
	rm uux
	cmp uuxqt $(INSDIR)/uuxqt
	rm uuxqt
	cmp uucico $(INSDIR)/uucico
	rm uucico
	cmp uuname $(BIN)/uuname
	rm uuname
	cmp uucheck $(INSDIR)/uucheck
	rm uucheck
	cmp uucleanup $(INSDIR)/uucleanup
	rm uucleanup
	cmp uustat $(BIN)/uustat
	rm uustat
	cmp uusched $(INSDIR)/uusched
	rm uusched
	-cmp uugetty $(INSDIR)/uugetty
	-rm uugetty
	rm *.o


init:	anlwrk.o permission.o cpmv.o expfile.o gename.o \
	getargs.o getprm.o getpwinfo.o gnamef.o \
	gnxseq.o gwd.o imsg.o logent.o \
	mailst.o shio.o \
	systat.o ulockf.o uucpname.o versys.o xqt.o

uucp:	$(OUUCP) $(OFILES)
	$(CC) $(CFLAGS)  $(LDFLAGS) $(OUUCP) $(OFILES) $(LIBS) \
		-o $(ODIR)/uucp

uux:	$(OUUX) $(OFILES)
	$(CC) $(CFLAGS)  $(LDFLAGS) $(OUUX) $(OFILES) $(LIBS) \
		-o $(ODIR)/uux

uuxqt:	$(OUUXQT) $(OFILES)
	$(CC) $(CFLAGS)  $(LDFLAGS) $(OUUXQT) $(OFILES) $(LIBS) \
		-o $(ODIR)/uuxqt

uucico:	$(OUUCICO) $(OFILES)
	$(CC) $(CFLAGS)  $(LDFLAGS) $(OUUCICO) $(OFILES) $(LIBS) \
		-o $(ODIR)/uucico

uuname:	$(OUUNAME)
	$(CC) $(CFLAGS)  $(LDFLAGS) $(OUUNAME) $(LIBS) \
		-o $(ODIR)/uuname

uustat:	$(OUUSTAT) $(GETOPT)
	$(CC) $(CFLAGS)  $(LDFLAGS) $(OUUSTAT) $(GETOPT) $(LIBS) \
		-o $(ODIR)/uustat
 
uucleanup:	$(OUUCLEANUP) $(GETOPT)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OUUCLEANUP) $(GETOPT) $(LIBS) \
		-o $(ODIR)/uucleanup
 
uusched:	$(OUUSCHED) $(GETOPT)
	$(CC) $(CFLAGS)  $(LDFLAGS) $(OUUSCHED) $(GETOPT) $(LIBS) \
		-o $(ODIR)/uusched
 
uucheck:	uucheck.o uucpname.o $(GETOPT)
	$(CC) $(CFLAGS)  $(LDFLAGS) uucheck.o uucpname.o $(GETOPT) $(LIBS) \
		-o $(ODIR)/uucheck
 
uugetty:	$(OUUGETTY)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OUUGETTY) \
		-o $(ODIR)/uugetty
 
uucheck.o:	permission.c

utility.o permission.o cico.o conn.o callers.o cpmv.o\
	anlwrk.o cntrl.o expfile.o gename.o\
	getpwinfo.o gio.o xio.o gnamef.o gnxseq.o gwd.o imsg.o ioctl.o\
	logent.o mailst.o sdmail.o line.o shio.o\
	systat.o ulockf.o uucpdefs.o uucpname.o uuname.o\
	uucleanup.o uucheck.o uusched.o  uucp.o uustat.o dio.o \
	uux.o uuxqt.o versys.o xqt.o:	uucp.h parms.h

mkdirs:
	-mkdir $(INSDIR)
	$(CH)-chmod 755 $(INSDIR)
	$(CH)-chown $(OWNER) $(INSDIR)
	$(CH)-chgrp $(GRP) $(INSDIR)
	-mkdir $(SPOOL)
	$(CH)-chmod 777 $(SPOOL)
	$(CH)-chown $(OWNER) $(SPOOL)
	$(CH)-chgrp $(GRP) $(SPOOL)
	-mkdir $(PUBDIR)
	$(CH)-chmod 777 $(PUBDIR)
	$(CH)-chown $(OWNER) $(PUBDIR)
	$(CH)-chgrp $(GRP) $(PUBDIR)
	-mkdir $(XQTDIR)
	$(CH)-chmod 777 $(XQTDIR)
	$(CH)-chown $(OWNER) $(XQTDIR)
	$(CH)-chgrp $(GRP) $(XQTDIR)
	-mkdir $(CORRUPT)
	$(CH)-chmod 777 $(CORRUPT)
	$(CH)-chown $(OWNER) $(CORRUPT)
	$(CH)-chgrp $(GRP) $(CORRUPT)
	-mkdir $(LOGDIR)
	$(CH)-chmod 777 $(LOGDIR)
	$(CH)-chown $(OWNER) $(LOGDIR)
	$(CH)-chgrp $(GRP) $(LOGDIR)
	-mkdir $(SEQDIR)
	$(CH)-chmod 777 $(SEQDIR)
	$(CH)-chown $(OWNER) $(SEQDIR)
	$(CH)-chgrp $(GRP) $(SEQDIR)
	-mkdir $(STATDIR)
	$(CH)-chmod 777 $(STATDIR)
	$(CH)-chown $(OWNER) $(STATDIR)
	$(CH)-chgrp $(GRP) $(STATDIR)
	-mkdir $(LOGUUCP)
	$(CH)-chmod 777 $(LOGUUCP)
	$(CH)-chown $(OWNER) $(LOGUUCP)
	$(CH)-chgrp $(GRP) $(LOGUUCP)
	-mkdir $(LOGUUX)
	$(CH)-chmod 777 $(LOGUUX)
	$(CH)-chown $(OWNER) $(LOGUUX)
	$(CH)-chgrp $(GRP) $(LOGUUX)
	-mkdir $(LOGUUXQT)
	$(CH)-chmod 777 $(LOGUUXQT)
	$(CH)-chown $(OWNER) $(LOGUUXQT)
	$(CH)-chgrp $(GRP) $(LOGUUXQT)
	-mkdir $(LOGCICO)
	$(CH)-chmod 777 $(LOGCICO)
	$(CH)-chown $(OWNER) $(LOGCICO)
	$(CH)-chgrp $(GRP) $(LOGCICO)
	-mkdir $(WORKSPACE)
	$(CH)-chmod 777 $(WORKSPACE)
	$(CH)-chown $(OWNER) $(WORKSPACE)
	$(CH)-chgrp $(GRP) $(WORKSPACE)
	-mkdir $(LOCKS)
	$(CH)-chmod 755 $(LOCKS)
	$(CH)-chown $(OWNER) $(LOCKS)
	$(CH)-chgrp $(GRP) $(LOCKS)
	-mkdir $(ADMIN)
	$(CH)-chmod 777 $(ADMIN)
	$(CH)-chown $(OWNER) $(ADMIN)
	$(CH)-chgrp $(GRP) $(ADMIN)
	-mkdir $(OLDLOG)
	$(CH)-chmod 777 $(OLDLOG)
	$(CH)-chown $(OWNER) $(OLDLOG)
	$(CH)-chgrp $(GRP) $(OLDLOG)

#  lint procedures

lint:	lintuucp lintuucico lintuux lintuuxqt \
	lintuuname

lintuucp:
	lint $(LINTOP) $(LUUCP) $(LFILES)

lintuucico:
	lint $(LINTOP) $(LUUCICO) $(LFILES)

lintuux:
	lint $(LINTOP) $(LUUX) $(LFILES)

lintuuxqt:
	lint $(LINTOP) $(LUUXQT) $(LFILES)

lintuuname:
	lint $(LINTOP) $(LUUNAME)
