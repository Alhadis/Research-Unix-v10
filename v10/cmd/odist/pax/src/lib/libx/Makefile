/*
 * libx make
 */

VERSION = 3.0

.SOURCE : comp dir hash man misc port preroot re \
		sfio stdio string tm vec

.SOURCE.h : include

DEBUG ==
LIBX == 1

HEADERSRC = error.h ftwalk.h hash.h hashpart.h ls.h namval.h \
	option.h re.h sfio.h stack.h stak.h stdio.h swap.h tar.h tm.h

HEADERGEN = align.h dirent.h fsinfo.h limits.h sigdata.h

HEADERMK = lcldirent.h lcllimits.h lclparam.h lclstdio.h \
	preroot.h sig.h ttyinfo.h

x $(VERSION) :LIBRARY: README hash.3 ftwalk.3 re.3 sfio.3 stak.3 tm.3 \
	$(HEADERSRC) \
	dirlib.h opendir.c readdir.c rewinddir.c seekdir.c telldir.c \
	getcwd.c setcwd.c getshell.c \
	hashlib.h hashalloc.c hashdump.c hashfree.c hashlook.c \
	hashscan.c hashsize.c hashwalk.c \
	memhash.c memsum.c strhash.c strsum.c \
	cvtatoe.c cvtetoa.c \
	chresc.c ctoi.c streval.c strmatch.c strcopy.c \
	modelib.h strmode.c \
	stresc.c stropt.c strtape.c tok.c \
	pathaccess.c pathcanon.c pathcheck.c pathpath.c \
	pathkey.c pathprobe.c pathtemp.c \
	cmdargs.c fs3d.c ftwalk.c perror.c putenv.c calloc.c malloc.c \
	getnamval.c optget.c hsort.c query.c \
	error.c liberror.c strerror.c strsignal.c \
	fmtelapsed.c fmtls.c fmtmode.c fmtperm.c fmtuid.c fmtgid.c \
	strelapsed.c strperm.c struid.c strgid.c stack.c stak.c \
	memswap.c gethalf.c getlong.c puthalf.c putlong.c \
	fgetline.c sigcrit.c sigunblock.c \
	cmdlib.h cmdopen.c cmdclose.c cmdrun.c cmdwait.c cmdkill.c \
	relib.h recomp.c reerror.c reexec.c resub.c \
	tmdate.c tmfix.c tmform.c tmgoff.c \
	tminit.c tmleap.c tmlex.c tmmake.c tmpoff.c \
	tmtime.c tmtype.c tmword.c tmzone.c \
	vecargs.c vecfile.c vecfree.c vecload.c vecstring.c \
	getsymlink.c putsymlink.c univlib.h univdata.c getuniv.c setuniv.c \
	winsize.c fclex.c sigdata.c touch.c lpstat.c \
	memcmp.c memcpy.c memdup.c memset.c \
	mkdir.c rmdir.c remove.c rename.c link.c unlink.c \
	strtok.c strspn.c strpbrk.c strdup.c strchr.c strrchr.c \
	strtod.c strtol.c strton.c sigsetmask.c mktemp.c tmpnam.c \
	vfork.c killpg.c lstat.c readlink.c symlink.c setpgid.c \
	atexit.c getdents.c getwd.c copy.c dup2.c errno.c sgetl.c sputl.c \
	getpreroot.c ispreroot.c realopen.c setpreroot.c \
	mount.c popen.c system.c iblocks.c \
	modedata.c tmdata.c \
	sfhdr.h sfclose.c sfclrlock.c sfcvt.c sfdlen.c sfdopen.c \
	sfexcept.c sfextern.c sffilbuf.c sfflsbuf.c sfgetd.c sfgetl.c \
	sfgets.c sfgetu.c sfllen.c sfmode.c sfmove.c sfnew.c sfnotify.c \
	sfnputc.c sfopen.c sfpeek.c sfpool.c sfpopen.c sfprintf.c sfputd.c \
	sfputl.c sfputs.c sfputu.c sfread.c sfscanf.c sfseek.c \
	sfsetbuf.c sfsetdisc.c sfset.c sfstack.c sfstrtod.c \
	sfsync.c sftable.c sftell.c sftmpfile.c sfungetc.c sfvprintf.c \
	sfvscanf.c sfwrite.c \
	_sfhdr.h _sfclearerr.c _sffileno.c _sforigin.c _sfputl.c _sftmp.c \
	_sfecvt.c _sfgetc.c _sfpeek.c _sfputu.c _sfulen.c _sfeof.c _sfgetl.c \
	_sfpushed.c _sfslen.c _sferror.c _sfgetu.c _sfputc.c _sffcvt.c \
	_sfputd.c _sfsync.c \
	stdgets.c stdprintf.c stdscanf.c stdsetvbuf.c stdsprintf.c \
	stdvsprintf.c stdvsscanf.c sprintf.c vsprintf.c

:READONLY: modedata.c sftable.c sigdata.c tmdata.c univdata.c

$(INCLUDEDIR) :INSTALLDIR: $(HEADERSRC) $(HEADERGEN) $(HEADERMK)

:INSTALLDIR: mkhdr genhdr

mkhdr :: mkhdr.sh

genhdr :: genhdr.c

$(HEADERGEN) : mkhdr genhdr
	$(*:N=*mkhdr) $(<:B) "$(TARGET:/\\\\/\\/G) $(*:N=*genhdr)" "$(CC)" $(CCFLAGS) > $(<)

$(HEADERMK) : mkhdr
	$(*) $(<:B) "" "$(CC)" $(CCFLAGS) > $(<)
