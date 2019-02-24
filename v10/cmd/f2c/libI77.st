./ ADD NAME=libI77/Notice TIME=695663806
/****************************************************************
Copyright 1990, 1991, 1992 by AT&T Bell Laboratories and Bellcore.

Permission to use, copy, modify, and distribute this software
and its documentation for any purpose and without fee is hereby
granted, provided that the above copyright notice appear in all
copies and that both that the copyright notice and this
permission notice and warranty disclaimer appear in supporting
documentation, and that the names of AT&T Bell Laboratories or
Bellcore or any of their entities not be used in advertising or
publicity pertaining to distribution of the software without
specific, written prior permission.

AT&T and Bellcore disclaim all warranties with regard to this
software, including all implied warranties of merchantability
and fitness.  In no event shall AT&T or Bellcore be liable for
any special, indirect or consequential damages or any damages
whatsoever resulting from loss of use, data or profits, whether
in an action of contract, negligence or other tortious action,
arising out of or in connection with the use or performance of
this software.
****************************************************************/

./ ADD NAME=libI77/README TIME=711898327
If your compiler does not recognize ANSI C headers,
compile with KR_headers defined:  either add -DKR_headers
to the definition of CFLAGS in the makefile, or insert

#define KR_headers

at the top of f2c.h .


If your system lacks /usr/include/local.h ,
then you should create an appropriate local.h in
this directory.  An appropriate local.h may simply
be empty, or it may #define VAX or #define CRAY
(or whatever else you must do to make fp.h work right).
Alternatively, edit fp.h to suite your machine.

If your system lacks /usr/include/fcntl.h , then you
should simply create an empty fcntl.h in this directory.
If your compiler then complains about creat and open not
having a prototype, compile with OPEN_DECL defined.
On many systems, open and creat are declared in fcntl.h .

If your system's sprintf does not work the way ANSI C
specifies -- specifically, if it does not return the
number of characters transmitted -- then insert the line

#define USE_STRLEN

at the end of fmt.h .  This is necessary with
at least some versions of Sun software.

If your system's fopen does not like the ANSI binary
reading and writing modes "rb" and "wb", then you should
compile open.c with NON_ANSI_RW_MODES #defined.

If you get error messages about references to cf->_ptr
and cf->_base when compiling wrtfmt.c and wsfe.c or to
stderr->_flag when compiling err.c, then insert the line

#define NON_UNIX_STDIO

at the beginning of fio.h, and recompile these modules.

Unformatted sequential records consist of a length of record
contents, the record contents themselves, and the length of
record contents again (for backspace).  Prior to 17 Oct. 1991,
the length was of type int; now it is of type long, but you
can change it back to int by inserting

#define UIOLEN_int

at the beginning of fio.h.  This affects only sue.c and uio.c .

You may need to supply the following non-ANSI routines:

  fstat(int fileds, struct stat *buf) is similar
to stat(char *name, struct stat *buf), except that
the first argument, fileds, is the file descriptor
returned by open rather than the name of the file.
fstat is used in the system-dependent routine
canseek (in the libI77 source file err.c), which
is supposed to return 1 if it's possible to issue
seeks on the file in question, 0 if it's not; you may
need to suitably modify err.c .  On non-UNIX systems,
you can avoid references to fstat and stat by compiling
err.c, inquire.c, open.c, and util.c with MSDOS defined;
in that case, you may need to supply access(char *Name,0),
which is supposed to return 0 if file Name exists,
nonzero otherwise.

  char * mktemp(char *buf) is supposed to replace the
6 trailing X's in buf with a unique number and then
return buf.  The idea is to get a unique name for
a temporary file.

On non-UNIX systems, you may need to change a few other,
e.g.: the form of name computed by mktemp() in endfile.c and
open.c; the use of the open(), close(), and creat() system
calls in endfile.c, err.c, open.c; and the modes in calls on
fopen() and fdopen() (and perhaps the use of fdopen() itself
-- it's supposed to return a FILE* corresponding to a given
an integer file descriptor) in err.c and open.c (component ufmt
of struct unit is 1 for formatted I/O -- text mode on some systems
-- and 0 for unformatted I/O -- binary mode on some systems).

For Turbo C++, in particular, you need to adjust the mktemp
invocations and should compile all of libI77 with -DMSDOS .
You also need to #undef ungetc in lread.c and rsne.c .
Don't use -mh -- it is horribly broken.

If you want to be able to load against libI77 but not libF77,
then you will need to add sig_die.o (from libF77) to libI77.

If you wish to use translated Fortran that has funny notions
of record length for direct unformatted I/O (i.e., that assumes
RECL= values in OPEN statements are not bytes but rather counts
of some other units -- e.g., 4-character words for VMS), then you
should insert an appropriate #define for url_Adjust at the
beginning of open.c .  For VMS Fortran, for example,
#define url_Adjust(x) x *= 4
would suffice.

To check for transmission errors, issue the command
	make check
This assumes you have the xsum program whose source, xsum.c,
is distributed as part of "all from f2c/src".  If you do not
have xsum, you can obtain xsum.c by sending the following E-mail
message to netlib@research.att.com
	send xsum.c from f2c/src

The makefile assumes you have installed f2c.h in a standard
place (and does not cause recompilation when f2c.h is changed);
f2c.h comes with "all from f2c" (the source for f2c) and is
available separately ("f2c.h from f2c").

By default, Fortran I/O units 5, 6, and 0 are pre-connected to
stdin, stdout, and stderr, respectively.  You can change this
behavior by changing f_init() in err.c to suit your needs.
Note that f2c assumes READ(*... means READ(5... and WRITE(*...
means WRITE(6... .  Moreover, an OPEN(n,... statement that does
not specify a file name (and does not specify STATUS='SCRATCH')
assumes FILE='fort.n' .  You can change this by editing open.c
and endfile.c suitably.
./ ADD NAME=libI77/Version.c TIME=719849651
static char junk[] = "\n@(#) LIBI77 VERSION pjw,dmg-mods 23 Oct. 1992\n";

/*
2.01	$ format added
2.02	Coding bug in open.c repaired
2.03	fixed bugs in lread.c (read * with negative f-format) and lio.c
	and lio.h (e-format conforming to spec)
2.04	changed open.c and err.c (fopen and freopen respectively) to
	update to new c-library (append mode)
2.05	added namelist capability
2.06	allow internal list and namelist I/O
*/

/*
close.c:
	allow upper-case STATUS= values
endfile.c
	create fort.nnn if unit nnn not open;
	else if (file length == 0) use creat() rather than copy;
	use local copy() rather than forking /bin/cp;
	rewind, fseek to clear buffer (for no reading past EOF)
err.c
	use neither setbuf nor setvbuf; make stderr buffered
fio.h
	#define _bufend
inquire.c
	upper case responses;
	omit byfile test from SEQUENTIAL=
	answer "YES" to DIRECT= for unopened file (open to debate)
lio.c
	flush stderr, stdout at end of each stmt
	space before character strings in list output only at line start
lio.h
	adjust LEW, LED consistent with old libI77
lread.c
	use atof()
	allow "nnn*," when reading complex constants
open.c
	try opening for writing when open for read fails, with
	special uwrt value (2) delaying creat() to first write;
	set curunit so error messages don't drop core;
	no file name ==> fort.nnn except for STATUS='SCRATCH'
rdfmt.c
	use atof(); trust EOF == end-of-file (so don't read past
	end-of-file after endfile stmt)
sfe.c
	flush stderr, stdout at end of each stmt
wrtfmt.c:
	use upper case
	put wrt_E and wrt_F into wref.c, use sprintf()
		rather than ecvt() and fcvt() [more accurate on VAX]
*/

/* 16 Oct. 1988: uwrt = 3 after write, rewind, so close won't zap the file. */

/* 10 July 1989: change _bufend to buf_end in fio.h, wsfe.c, wrtfmt.c */

/* 28 Nov. 1989: corrections for IEEE and Cray arithmetic */
/* 29 Nov. 1989: change various int return types to long for f2c */
/* 30 Nov. 1989: various types from f2c.h */
/*  6 Dec. 1989: types corrected various places */
/* 19 Dec. 1989: make iostat= work right for internal I/O */
/*  8 Jan. 1990: add rsne, wsne -- routines for handling NAMELIST */
/* 28 Jan. 1990: have NAMELIST read treat $ as &, general white
		 space as blank */
/* 27 Mar. 1990: change an = to == in rd_L(rdfmt.c) so formatted reads
		 of logical values reject letters other than fFtT;
		 have nowwriting reset cf */
/* 14 Aug. 1990: adjust lread.c to treat tabs as spaces in list input */
/* 17 Aug. 1990: adjust open.c to recognize blank='Z...' as well as
		 blank='z...' when reopening an open file */
/* 30 Aug. 1990: prevent embedded blanks in list output of complex values;
		 omit exponent field in list output of values of
		 magnitude between 10 and 1e8; prevent writing stdin
		 and reading stdout or stderr; don't close stdin, stdout,
		 or stderr when reopening units 5, 6, 0. */
/* 18 Sep. 1990: add component udev to unit and consider old == new file
		 iff uinode and udev values agree; use stat rather than
		 access to check existence of file (when STATUS='OLD')*/
/* 2 Oct. 1990:  adjust rewind.c so two successive rewinds after a write
		 don't clobber the file. */
/* 9 Oct. 1990:  add #include "fcntl.h" to endfile.c, err.c, open.c;
		 adjust g_char in util.c for segmented memories. */
/* 17 Oct. 1990: replace abort() and _cleanup() with calls on
		 sig_die(...,1) (defined in main.c). */
/* 5 Nov. 1990:  changes to open.c: complain if new= is specified and the
		 file already exists; allow file= to be omitted in open stmts
		 and allow status='replace' (Fortran 90 extensions). */
/* 11 Dec. 1990: adjustments for POSIX. */
/* 15 Jan. 1991: tweak i_ungetc in rsli.c to allow reading from
		 strings in read-only memory. */
/* 25 Apr. 1991: adjust namelist stuff to work with f2c -i2 */
/* 26 Apr. 1991: fix some bugs with NAMELIST read of multi-dim. arrays */
/* 16 May 1991:  increase LEFBL in lio.h to bypass NeXT bug */
/* 17 Oct. 1991: change type of length field in sequential unformatted
		 records from int to long (for systems where sizeof(int)
		 can vary, depending on the compiler or compiler options). */
/* 14 Nov. 1991: change uint to Uint in fmt.h, rdfmt.c, wrtfmt.c.
/* 25 Nov. 1991: change uint to Uint in lwrite.c; change sizeof(int) to
		 sizeof(uioint) in fseeks in sue.c (missed on 17 Oct.). */
/* 1 Dec. 1991:  uio.c: add test for read failure (seq. unformatted reads);
		 adjust an error return from EOF to off end of record */
/* 12 Dec. 1991: rsli.c: fix bug with internal list input that caused
		 the last character of each record to be ignored.
		 iio.c: adjust error message in internal formatted
		 input from "end-of-file" to "off end of record" if
		 the format specifies more characters than the
		 record contains. */
/* 17 Jan. 1992: lread.c, rsne.c: in list and namelist input,
		 treat "r* ," and "r*," alike (where r is a
		 positive integer constant), and fix a bug in
		 handling null values following items with repeat
		 counts (e.g., 2*1,,3); for namelist reading
		 of a numeric array, allow a new name-value subsequence
		 to terminate the current one (as though the current
		 one ended with the right number of null values).
		 lio.h, lwrite.c: omit insignificant zeros in
		 list and namelist output. To get the old
		 behavior, compile with -DOld_list_output . */
/* 18 Jan. 1992: make list output consistent with F format by
		 printing .1 rather than 0.1 (introduced yesterday). */
/* 3 Feb. 1992:  rsne.c: fix namelist read bug that caused the
		 character following a comma to be ignored. */
/* 19 May 1992:  adjust iio.c, ilnw.c, rdfmt.c and rsli.c to make err=
		 work with internal list and formatted I/O. */
/* 18 July 1992: adjust rsne.c to allow namelist input to stop at
		 an & (e.g. &end). */
/* 23 July 1992: switch to ANSI prototypes unless KR_headers is #defined ;
		 recognize Z format (assuming 8-bit bytes). */
/* 14 Aug. 1992: tweak wrt_E in wref.c to avoid -NaN */
/* 23 Oct. 1992: Supply missing l_eof = 0 assignment to s_rsne() in rsne.c
		 (so end-of-file on other files won't confuse namelist
		 reads of external files).  Prepend f__ to external
		 names that are only of internal interest to lib[FI]77. */
./ ADD NAME=libI77/backspace.c TIME=719848229
#include "f2c.h"
#include "fio.h"
#ifdef KR_headers
integer f_back(a) alist *a;
#else
integer f_back(alist *a)
#endif
{	unit *b;
	int n,i;
	long x;
	char buf[32];
	if(a->aunit >= MXUNIT || a->aunit < 0)
		err(a->aerr,101,"backspace")
	b= &f__units[a->aunit];
	if(b->useek==0) err(a->aerr,106,"backspace")
	if(b->ufd==NULL) {
		fk_open(1, 1, a->aunit);
		return(0);
		}
	if(b->uend==1)
	{	b->uend=0;
		return(0);
	}
	if(b->uwrt) {
		(void) t_runc(a);
		if (f__nowreading(b))
			err(a->aerr,errno,"backspace")
		}
	if(b->url>0)
	{	long y;
		x=ftell(b->ufd);
		y = x % b->url;
		if(y == 0) x--;
		x /= b->url;
		x *= b->url;
		(void) fseek(b->ufd,x,SEEK_SET);
		return(0);
	}

	if(b->ufmt==0)
	{	(void) fseek(b->ufd,-(long)sizeof(int),SEEK_CUR);
		(void) fread((char *)&n,sizeof(int),1,b->ufd);
		(void) fseek(b->ufd,-(long)n-2*sizeof(int),SEEK_CUR);
		return(0);
	}
	for(;;)
	{	long y;
		y = x=ftell(b->ufd);
		if(x<sizeof(buf)) x=0;
		else x -= sizeof(buf);
		(void) fseek(b->ufd,x,SEEK_SET);
		n=fread(buf,1,(int)(y-x), b->ufd);
		for(i=n-2;i>=0;i--)
		{
			if(buf[i]!='\n') continue;
			(void) fseek(b->ufd,(long)(i+1-n),SEEK_CUR);
			return(0);
		}
		if(x==0)
			{
			(void) fseek(b->ufd, 0L, SEEK_SET);
			return(0);
			}
		else if(n<=0) err(a->aerr,(EOF),"backspace")
		(void) fseek(b->ufd, x, SEEK_SET);
	}
}
./ ADD NAME=libI77/close.c TIME=719848229
#include "f2c.h"
#include "fio.h"
#ifdef KR_headers
integer f_clos(a) cllist *a;
#else
#undef abs
#undef min
#undef max
#include "stdlib.h"
#ifdef MSDOS
#include "io.h"
#else
#ifdef __cplusplus
extern "C" int unlink(const char*);
#else
extern int unlink(const char*);
#endif
#endif

integer f_clos(cllist *a)
#endif
{	unit *b;

	if(a->cunit >= MXUNIT) return(0);
	b= &f__units[a->cunit];
	if(b->ufd==NULL)
		goto done;
	if (!a->csta)
		if (b->uscrtch == 1)
			goto Delete;
		else
			goto Keep;
	switch(*a->csta) {
		default:
	 	Keep:
		case 'k':
		case 'K':
			if(b->uwrt == 1)
				(void) t_runc((alist *)a);
			if(b->ufnm) {
				(void) fclose(b->ufd);
				free(b->ufnm);
				}
			break;
		case 'd':
		case 'D':
		Delete:
			if(b->ufnm) {
				(void) fclose(b->ufd);
				(void) unlink(b->ufnm); /*SYSDEP*/
				free(b->ufnm);
				}
		}
	b->ufd=NULL;
 done:
	b->uend=0;
	b->ufnm=NULL;
	return(0);
	}
 void
#ifdef KR_headers
f_exit()
#else
f_exit(void)
#endif
{	int i;
	static cllist xx;
	if (!xx.cerr) {
		xx.cerr=1;
		xx.csta=NULL;
		for(i=0;i<MXUNIT;i++)
		{
			xx.cunit=i;
			(void) f_clos(&xx);
		}
	}
}
 void
#ifdef KR_headers
flush_()
#else
flush_(void)
#endif
{	int i;
	for(i=0;i<MXUNIT;i++)
		if(f__units[i].ufd != NULL && f__units[i].uwrt)
			fflush(f__units[i].ufd);
}
./ ADD NAME=libI77/dfe.c TIME=719848230
#include "f2c.h"
#include "fio.h"
#include "fmt.h"

y_rsk(Void)
{
	if(f__curunit->uend || f__curunit->url <= f__recpos
		|| f__curunit->url == 1) return 0;
	do {
		getc(f__cf);
	} while(++f__recpos < f__curunit->url);
	return 0;
}
y_getc(Void)
{
	int ch;
	if(f__curunit->uend) return(-1);
	if((ch=getc(f__cf))!=EOF)
	{
		f__recpos++;
		if(f__curunit->url>=f__recpos ||
			f__curunit->url==1)
			return(ch);
		else	return(' ');
	}
	if(feof(f__cf))
	{
		f__curunit->uend=1;
		errno=0;
		return(-1);
	}
	err(f__elist->cierr,errno,"readingd");
}
#ifdef KR_headers
y_putc(c)
#else
y_putc(int c)
#endif
{
	f__recpos++;
	if(f__recpos <= f__curunit->url || f__curunit->url==1)
		putc(c,f__cf);
	else
		err(f__elist->cierr,110,"dout");
	return(0);
}
y_rev(Void)
{	/*what about work done?*/
	if(f__curunit->url==1 || f__recpos==f__curunit->url)
		return(0);
	while(f__recpos<f__curunit->url)
		(*f__putn)(' ');
	f__recpos=0;
	return(0);
}
y_err(Void)
{
	err(f__elist->cierr, 110, "dfe");
}

y_newrec(Void)
{
	if(f__curunit->url == 1 || f__recpos == f__curunit->url) {
		f__hiwater = f__recpos = f__cursor = 0;
		return(1);
	}
	if(f__hiwater > f__recpos)
		f__recpos = f__hiwater;
	y_rev();
	f__hiwater = f__cursor = 0;
	return(1);
}

#ifdef KR_headers
c_dfe(a) cilist *a;
#else
c_dfe(cilist *a)
#endif
{
	f__sequential=0;
	f__formatted=f__external=1;
	f__elist=a;
	f__cursor=f__scale=f__recpos=0;
	if(a->ciunit>MXUNIT || a->ciunit<0)
		err(a->cierr,101,"startchk");
	f__curunit = &f__units[a->ciunit];
	if(f__curunit->ufd==NULL && fk_open(DIR,FMT,a->ciunit))
		err(a->cierr,104,"dfe");
	f__cf=f__curunit->ufd;
	if(!f__curunit->ufmt) err(a->cierr,102,"dfe")
	if(!f__curunit->useek) err(a->cierr,104,"dfe")
	f__fmtbuf=a->cifmt;
	(void) fseek(f__cf,(long)f__curunit->url * (a->cirec-1),SEEK_SET);
	f__curunit->uend = 0;
	return(0);
}
#ifdef KR_headers
integer s_rdfe(a) cilist *a;
#else
integer s_rdfe(cilist *a)
#endif
{
	int n;
	if(!f__init) f_init();
	if(n=c_dfe(a))return(n);
	f__reading=1;
	if(f__curunit->uwrt && f__nowreading(f__curunit))
		err(a->cierr,errno,"read start");
	f__getn = y_getc;
	f__doed = rd_ed;
	f__doned = rd_ned;
	f__dorevert = f__donewrec = y_err;
	f__doend = y_rsk;
	if(pars_f(f__fmtbuf)<0)
		err(a->cierr,100,"read start");
	fmt_bg();
	return(0);
}
#ifdef KR_headers
integer s_wdfe(a) cilist *a;
#else
integer s_wdfe(cilist *a)
#endif
{
	int n;
	if(!f__init) f_init();
	if(n=c_dfe(a)) return(n);
	f__reading=0;
	if(f__curunit->uwrt != 1 && f__nowwriting(f__curunit))
		err(a->cierr,errno,"startwrt");
	f__putn = y_putc;
	f__doed = w_ed;
	f__doned= w_ned;
	f__dorevert = y_err;
	f__donewrec = y_newrec;
	f__doend = y_rev;
	if(pars_f(f__fmtbuf)<0)
		err(a->cierr,100,"startwrt");
	fmt_bg();
	return(0);
}
integer e_rdfe(Void)
{
	(void) en_fio();
	return(0);
}
integer e_wdfe(Void)
{
	(void) en_fio();
	return(0);
}
./ ADD NAME=libI77/dolio.c TIME=719848230
#include "f2c.h"

#ifdef __cplusplus
extern "C" {
#endif
#ifdef KR_headers
extern int (*f__lioproc)();

integer do_lio(type,number,ptr,len) ftnint *number,*type; char *ptr; ftnlen len;
#else
extern int (*f__lioproc)(ftnint*, char*, ftnlen, ftnint);

integer do_lio(ftnint *type, ftnint *number, char *ptr, ftnlen len)
#endif
{
	return((*f__lioproc)(number,ptr,len,*type));
}
#ifdef __cplusplus
	}
#endif
./ ADD NAME=libI77/due.c TIME=719848230
#include "f2c.h"
#include "fio.h"

#ifdef KR_headers
c_due(a) cilist *a;
#else
c_due(cilist *a)
#endif
{
	if(!f__init) f_init();
	if(a->ciunit>=MXUNIT || a->ciunit<0)
		err(a->cierr,101,"startio");
	f__recpos=f__sequential=f__formatted=0;
	f__external=1;
	f__curunit = &f__units[a->ciunit];
	f__elist=a;
	if(f__curunit->ufd==NULL && fk_open(DIR,UNF,a->ciunit) ) err(a->cierr,104,"due");
	f__cf=f__curunit->ufd;
	if(f__curunit->ufmt) err(a->cierr,102,"cdue")
	if(!f__curunit->useek) err(a->cierr,104,"cdue")
	if(f__curunit->ufd==NULL) err(a->cierr,114,"cdue")
	(void) fseek(f__cf,(long)(a->cirec-1)*f__curunit->url,SEEK_SET);
	f__curunit->uend = 0;
	return(0);
}
#ifdef KR_headers
integer s_rdue(a) cilist *a;
#else
integer s_rdue(cilist *a)
#endif
{
	int n;
	if(n=c_due(a)) return(n);
	f__reading=1;
	if(f__curunit->uwrt && f__nowreading(f__curunit))
		err(a->cierr,errno,"read start");
	return(0);
}
#ifdef KR_headers
integer s_wdue(a) cilist *a;
#else
integer s_wdue(cilist *a)
#endif
{
	int n;
	if(n=c_due(a)) return(n);
	f__reading=0;
	if(f__curunit->uwrt != 1 && f__nowwriting(f__curunit))
		err(a->cierr,errno,"write start");
	return(0);
}
integer e_rdue(Void)
{
	if(f__curunit->url==1 || f__recpos==f__curunit->url)
		return(0);
	(void) fseek(f__cf,(long)(f__curunit->url-f__recpos),SEEK_CUR);
	if(ftell(f__cf)%f__curunit->url)
		err(f__elist->cierr,200,"syserr");
	return(0);
}
integer e_wdue(Void)
{
	return(e_rdue());
}
./ ADD NAME=libI77/endfile.c TIME=719848230
#include "f2c.h"
#include "fio.h"
#include "sys/types.h"
#include "fcntl.h"
#include "rawio.h"
#ifndef O_RDONLY
#define O_RDONLY 0
#endif

#ifdef KR_headers
extern char *strcpy();
#else
#undef abs
#undef min
#undef max
#include "stdlib.h"
#include "string.h"
#endif


#ifdef KR_headers
integer f_end(a) alist *a;
#else
integer f_end(alist *a)
#endif
{
	unit *b;
	if(a->aunit>=MXUNIT || a->aunit<0) err(a->aerr,101,"endfile");
	b = &f__units[a->aunit];
	if(b->ufd==NULL) {
		char nbuf[10];
		(void) sprintf(nbuf,"fort.%ld",a->aunit);
		close(creat(nbuf, 0666));
		return(0);
		}
	b->uend=1;
	return(b->useek ? t_runc(a) : 0);
}

 static int
#ifdef KR_headers
copy(from, len, to) char *from, *to; register long len;
#else
copy(char *from, register long len, char *to)
#endif
{
	register int n;
	int k, rc = 0, tmp;
	char buf[BUFSIZ];

	if ((k = open(from, O_RDONLY)) < 0)
		return 1;
	if ((tmp = creat(to,0666)) < 0)
		return 1;
	while((n = read(k, buf, len > BUFSIZ ? BUFSIZ : (int)len)) > 0) {
		if (write(tmp, buf, n) != n)
			{ rc = 1; break; }
		if ((len -= n) <= 0)
			break;
		}
	close(k);
	close(tmp);
	return n < 0 ? 1 : rc;
	}

#ifndef L_tmpnam
#define L_tmpnam 16
#endif

 int
#ifdef KR_headers
t_runc(a) alist *a;
#else
t_runc(alist *a)
#endif
{
	char nm[L_tmpnam];
	long loc, len;
	unit *b;
	int rc = 0;

	b = &f__units[a->aunit];
	if(b->url) return(0);	/*don't truncate direct files*/
	loc=ftell(b->ufd);
	(void) fseek(b->ufd,0L,SEEK_END);
	len=ftell(b->ufd);
	if (loc >= len || b->useek == 0 || b->ufnm == NULL)
		return(0);
	rewind(b->ufd);	/* empty buffer */
	if (!loc) {
		if (close(creat(b->ufnm,0666)))
			{ rc = 1; goto done; }
		if (b->uwrt)
			b->uwrt = 1;
		return 0;
		}
#ifdef _POSIX_SOURCE
	tmpnam(nm);
#else
	(void) strcpy(nm,"tmp.FXXXXXX");
	(void) mktemp(nm);
#endif
	if (copy(b->ufnm, loc, nm)
	 || copy(nm, loc, b->ufnm))
		rc = 1;
	unlink(nm);
done:
	fseek(b->ufd, loc, SEEK_SET);
	if (rc)
		err(a->aerr,111,"endfile");
	return 0;
	}
./ ADD NAME=libI77/err.c TIME=719848230
#ifndef MSDOS
#include "sys/types.h"
#include "sys/stat.h"
#endif
#include "f2c.h"
#include "fio.h"
#include "fcntl.h"
#include "rawio.h"
#include "fmt.h"	/* for struct syl */
#ifdef NON_UNIX_STDIO
#ifdef KR_headers
extern char *malloc();
#else
#undef abs
#undef min
#undef max
#include "stdlib.h"
#endif
#endif
#ifndef O_WRONLY
#define O_WRONLY 1
#endif

/*global definitions*/
unit f__units[MXUNIT];	/*unit table*/
flag f__init;	/*0 on entry, 1 after initializations*/
cilist *f__elist;	/*active external io list*/
flag f__reading;	/*1 if reading, 0 if writing*/
flag f__cplus,f__cblank;
char *f__fmtbuf;
flag f__external;	/*1 if external io, 0 if internal */
#ifdef KR_headers
int (*f__doed)(),(*f__doned)();
int (*f__doend)(),(*f__donewrec)(),(*f__dorevert)();
int (*f__getn)(),(*f__putn)();	/*for formatted io*/
#else
int (*f__getn)(void),(*f__putn)(int);	/*for formatted io*/
int (*f__doed)(struct f__syl*, char*, ftnlen),(*f__doned)(struct f__syl*);
int (*f__dorevert)(void),(*f__donewrec)(void),(*f__doend)(void);
#endif
flag f__sequential;	/*1 if sequential io, 0 if direct*/
flag f__formatted;	/*1 if formatted io, 0 if unformatted*/
FILE *f__cf;	/*current file*/
unit *f__curunit;	/*current unit*/
int f__recpos;	/*place in current record*/
int f__cursor,f__scale;

/*error messages*/
char *F_err[] =
{
	"error in format",				/* 100 */
	"illegal unit number",				/* 101 */
	"formatted io not allowed",			/* 102 */
	"unformatted io not allowed",			/* 103 */
	"direct io not allowed",			/* 104 */
	"sequential io not allowed",			/* 105 */
	"can't backspace file",				/* 106 */
	"null file name",				/* 107 */
	"can't stat file",				/* 108 */
	"unit not connected",				/* 109 */
	"off end of record",				/* 110 */
	"truncation failed in endfile",			/* 111 */
	"incomprehensible list input",			/* 112 */
	"out of free space",				/* 113 */
	"unit not connected",				/* 114 */
	"read unexpected character",			/* 115 */
	"bad logical input field",			/* 116 */
	"bad variable type",				/* 117 */
	"bad namelist name",				/* 118 */
	"variable not in namelist",			/* 119 */
	"no end record",				/* 120 */
	"variable count incorrect",			/* 121 */
	"subscript for scalar variable",		/* 122 */
	"invalid array section",			/* 123 */
	"substring out of bounds",			/* 124 */
	"subscript out of bounds",			/* 125 */
	"can't read file",				/* 126 */
	"can't write file",				/* 127 */
	"'new' file exists"				/* 128 */
};
#define MAXERR (sizeof(F_err)/sizeof(char *)+100)

#ifdef KR_headers
f__canseek(f) FILE *f; /*SYSDEP*/
#else
f__canseek(FILE *f) /*SYSDEP*/
#endif
{
#ifdef MSDOS
	return !isatty(fileno(f));
#else
	struct stat x;

	if (fstat(fileno(f),&x) < 0)
		return(0);
#ifdef S_IFMT
	switch(x.st_mode & S_IFMT) {
	case S_IFDIR:
	case S_IFREG:
		if(x.st_nlink > 0)	/* !pipe */
			return(1);
		else
			return(0);
	case S_IFCHR:
		if(isatty(fileno(f)))
			return(0);
		return(1);
#ifdef S_IFBLK
	case S_IFBLK:
		return(1);
#endif
	}
#else
#ifdef S_ISDIR
	/* POSIX version */
	if (S_ISREG(x.st_mode) || S_ISDIR(x.st_mode)) {
		if(x.st_nlink > 0)	/* !pipe */
			return(1);
		else
			return(0);
		}
	if (S_ISCHR(x.st_mode)) {
		if(isatty(fileno(f)))
			return(0);
		return(1);
		}
	if (S_ISBLK(x.st_mode))
		return(1);
#else
	Help! How does fstat work on this system?
#endif
#endif
	return(0);	/* who knows what it is? */
#endif
}

 void
#ifdef KR_headers
f__fatal(n,s) char *s;
#else
f__fatal(int n, char *s)
#endif
{
	if(n<100 && n>=0) perror(s); /*SYSDEP*/
	else if(n >= (int)MAXERR || n < -1)
	{	fprintf(stderr,"%s: illegal error number %d\n",s,n);
	}
	else if(n == -1) fprintf(stderr,"%s: end of file\n",s);
	else
		fprintf(stderr,"%s: %s\n",s,F_err[n-100]);
	if (f__curunit) {
		fprintf(stderr,"apparent state: unit %d ",f__curunit-f__units);
		fprintf(stderr, f__curunit->ufnm ? "named %s\n" : "(unnamed)\n",
			f__curunit->ufnm);
		}
	else
		fprintf(stderr,"apparent state: internal I/O\n");
	if (f__fmtbuf)
		fprintf(stderr,"last format: %s\n",f__fmtbuf);
	fprintf(stderr,"lately %s %s %s %s",f__reading?"reading":"writing",
		f__sequential?"sequential":"direct",f__formatted?"formatted":"unformatted",
		f__external?"external":"internal");
	sig_die(" IO", 1);
}
/*initialization routine*/
 VOID
f_init(Void)
{	unit *p;

	f__init=1;
	p= &f__units[0];
	p->ufd=stderr;
	p->useek=f__canseek(stderr);
#ifdef COMMENTED_OUT
	if(isatty(fileno(stderr))) {
		extern char *malloc();
		setbuf(stderr, malloc(BUFSIZ));
		/* setvbuf(stderr, _IOLBF, 0, 0); */
	}	/* wastes space, but win for debugging in windows */
#endif
#ifdef NON_UNIX_STDIO
	setbuf(stderr, malloc(BUFSIZ));
#else
	stderr->_flag &= ~_IONBF;
#endif
	p->ufmt=1;
	p->uwrt=1;
	p = &f__units[5];
	p->ufd=stdin;
	p->useek=f__canseek(stdin);
	p->ufmt=1;
	p->uwrt=0;
	p= &f__units[6];
	p->ufd=stdout;
	p->useek=f__canseek(stdout);
	/* IOLBUF and setvbuf only in system 5+ */
#ifdef COMMENTED_OUT
	if(isatty(fileno(stdout))) {
		extern char _sobuf[];
		setbuf(stdout, _sobuf);
		/* setvbuf(stdout, _IOLBF, 0, 0);	/* the buf arg in setvbuf? */
		p->useek = 1;	/* only within a record no bigger than BUFSIZ */
	}
#endif
	p->ufmt=1;
	p->uwrt=1;
}
#ifdef KR_headers
f__nowreading(x) unit *x;
#else
f__nowreading(unit *x)
#endif
{
	long loc;
	extern char *r_mode[];
	if (!x->ufnm)
		goto cantread;
	loc=ftell(x->ufd);
	if(freopen(x->ufnm,r_mode[x->ufmt],x->ufd) == NULL) {
 cantread:
		errno = 126;
		return(1);
		}
	x->uwrt=0;
	(void) fseek(x->ufd,loc,SEEK_SET);
	return(0);
}
#ifdef KR_headers
f__nowwriting(x) unit *x;
#else
f__nowwriting(unit *x)
#endif
{
	long loc;
	int k;
	extern char *w_mode[];

	if (!x->ufnm)
		goto cantwrite;
	if (x->uwrt == 3) { /* just did write, rewind */
		if (close(creat(x->ufnm,0666)))
			goto cantwrite;
		}
	else {
		loc=ftell(x->ufd);
		if (fclose(x->ufd) < 0
		|| (k = x->uwrt == 2 ? creat(x->ufnm,0666)
				     : open(x->ufnm,O_WRONLY)) < 0
		|| (f__cf = x->ufd = fdopen(k,w_mode[x->ufmt])) == NULL) {
			x->ufd = NULL;
 cantwrite:
			errno = 127;
			return(1);
			}
		(void) fseek(x->ufd,loc,SEEK_SET);
		}
	x->uwrt = 1;
	return(0);
}
./ ADD NAME=libI77/fio.h TIME=719848230
#include "stdio.h"
#include "errno.h"
#ifndef NULL
/* ANSI C */
#include "stddef.h"
#endif

#ifndef SEEK_SET
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif

#ifdef MSDOS
#ifndef NON_UNIX_STDIO
#define NON_UNIX_STDIO
#endif
#endif

#ifdef UIOLEN_int
typedef int uiolen;
#else
typedef long uiolen;
#endif

/*units*/
typedef struct
{	FILE *ufd;	/*0=unconnected*/
	char *ufnm;
#ifndef MSDOS
	long uinode;
	int udev;
#endif
	int url;	/*0=sequential*/
	flag useek;	/*true=can backspace, use dir, ...*/
	flag ufmt;
	flag uprnt;
	flag ublnk;
	flag uend;
	flag uwrt;	/*last io was write*/
	flag uscrtch;
} unit;

extern flag f__init;
extern cilist *f__elist;	/*active external io list*/
extern flag f__reading,f__external,f__sequential,f__formatted;
#ifdef KR_headers
#define Void /*void*/
extern int (*f__getn)(),(*f__putn)();	/*for formatted io*/
extern long f__inode();
extern VOID sig_die();
extern int (*f__donewrec)(), t_putc(), x_wSL();
extern int c_sfe();
#else
#define Void void
#ifdef __cplusplus
extern "C" {
#endif
extern int (*f__getn)(void),(*f__putn)(int);	/*for formatted io*/
extern long f__inode(char*,int*);
extern void sig_die(char*,int);
extern void f__fatal(int,char*);
extern int t_runc(alist*);
extern int f__nowreading(unit*), f__nowwriting(unit*);
extern int fk_open(int,int,ftnint);
extern int en_fio(void);
extern void f_init(void);
extern int (*f__donewrec)(void), t_putc(int), x_wSL(void);
extern void b_char(char*,char*,ftnlen), g_char(char*,ftnlen,char*);
extern int c_sfe(cilist*), z_rnew(void);
extern int isatty(int);
#ifdef __cplusplus
	}
#endif
#endif
extern FILE *f__cf;	/*current file*/
extern unit *f__curunit;	/*current unit*/
extern unit f__units[];
#define err(f,m,s) {if(f) errno= m; else f__fatal(m,s); return(m);}

/*Table sizes*/
#define MXUNIT 100

extern int f__recpos;	/*position in current record*/
extern int f__cursor;	/* offset to move to */
extern int f__hiwater;	/* so TL doesn't confuse us */

#define WRITE	1
#define READ	2
#define SEQ	3
#define DIR	4
#define FMT	5
#define UNF	6
#define EXT	7
#define INT	8

#define buf_end(x) (x->_flag & _IONBF ? x->_ptr : x->_base + BUFSIZ)
#ifdef __cplusplus
extern "C" {
extern long int f_back(alist *);
extern long int f_clos(cllist *);
extern void f_exit(void);
extern void flush_(void);
extern int y_rsk(void);
extern int y_getc(void);
extern int y_putc(int);
extern int y_rev(void);
extern int y_err(void);
extern int y_newrec(void);
extern int c_dfe(cilist *);
extern long int s_rdfe(cilist *);
extern long int s_wdfe(cilist *);
extern long int e_rdfe(void);
extern long int e_wdfe(void);
extern long int do_lio(long int *, long int *, char *, long int);
extern int c_due(cilist *);
extern long int s_rdue(cilist *);
extern long int s_wdue(cilist *);
extern long int e_rdue(void);
extern long int e_wdue(void);
extern long int f_end(alist *);
extern int f__canseek(struct _iobuf *);
extern char *ap_end(char *);
extern int op_gen(int, int, int, int);
extern char *gt_num(char *, int *);
extern char *f_s(char *, int);
extern int ne_d(char *, char **);
extern int e_d(char *, char **);
extern char *i_tem(char *);
extern char *f_list(char *);
extern int pars_f(char *);
extern int type_f(int);
extern long int do_fio(long int *, char *, long int);
extern void fmt_bg(void);
extern char *f__icvt(long int, int *, int *, int);
extern int z_getc(void);
extern int z_putc(int);
extern int c_si(icilist *);
extern int y_ierr(void);
extern long int s_rsfi(icilist *);
extern int z_wnew(void);
extern long int s_wsfi(icilist *);
extern long int e_rsfi(void);
extern long int e_wsfi(void);
extern void c_liw(icilist *);
extern int s_wsni(icilist *);
extern long int s_wsli(icilist *);
extern long int e_wsli(void);
extern long int f_inqu(inlist *);
extern int t_getc(void);
extern long int e_rsle(void);
extern int l_R(int);
extern int l_C(void);
extern int l_L(void);
extern int l_CHAR(void);
extern int c_le(cilist *);
extern int l_read(long int *, char *, long int, long int);
extern long int s_rsle(cilist *);
extern char *mktemp(char *);
extern int f__isdev(char *);
extern long int f_open(olist *);
extern long int f_rew(alist *);
extern int xrd_SL(void);
extern int x_getc(void);
extern int x_endp(void);
extern int x_rev(void);
extern long int s_rsfe(cilist *);
extern int i_getc(void);
extern int i_ungetc(int, struct _iobuf *);
extern long int s_rsli(icilist *);
extern long int e_rsli(void);
extern int s_rsni(icilist *);
extern int x_rsne(cilist *);
extern long int s_rsne(cilist *);
extern long int e_rsfe(void);
extern long int e_wsfe(void);
extern int c_sue(cilist *);
extern long int s_rsue(cilist *);
extern long int s_wsue(cilist *);
extern long int e_wsue(void);
extern long int e_rsue(void);
extern int do_us(long int *, char *, long int);
extern long int do_ud(long int *, char *, long int);
extern long int do_uio(long int *, char *, long int);
extern long int f__inode(char *, int *);
extern void f__mvgbt(int, int, char *, char *);
extern int mv_cur(void);
extern int x_putc(int);
extern int xw_end(void);
extern int xw_rev(void);
extern long int s_wsfe(cilist *);
extern long int s_wsle(cilist *);
extern long int e_wsle(void);
extern long int s_wsne(cilist *);
extern void x_wsne(cilist *);
	}
#endif
./ ADD NAME=libI77/fmt.c TIME=719848230
#include "f2c.h"
#include "fio.h"
#include "fmt.h"
#define skip(s) while(*s==' ') s++
#ifdef interdata
#define SYLMX 300
#endif
#ifdef pdp11
#define SYLMX 300
#endif
#ifdef vax
#define SYLMX 300
#endif
#ifndef SYLMX
#define SYLMX 300
#endif
#define GLITCH '\2'
	/* special quote character for stu */
extern int f__cursor,f__scale;
extern flag f__cblank,f__cplus;	/*blanks in I and compulsory plus*/
struct f__syl f__syl[SYLMX];
int f__parenlvl,f__pc,f__revloc;

#ifdef KR_headers
char *ap_end(s) char *s;
#else
char *ap_end(char *s)
#endif
{	char quote;
	quote= *s++;
	for(;*s;s++)
	{	if(*s!=quote) continue;
		if(*++s!=quote) return(s);
	}
	if(f__elist->cierr) {
		errno = 100;
		return(NULL);
	}
	f__fatal(100, "bad string");
	/*NOTREACHED*/ return 0;
}
#ifdef KR_headers
op_gen(a,b,c,d)
#else
op_gen(int a, int b, int c, int d)
#endif
{	struct f__syl *p= &f__syl[f__pc];
	if(f__pc>=SYLMX)
	{	fprintf(stderr,"format too complicated:\n");
		sig_die(f__fmtbuf, 1);
	}
	p->op=a;
	p->p1=b;
	p->p2=c;
	p->p3=d;
	return(f__pc++);
}
#ifdef KR_headers
char *f_list();
char *gt_num(s,n) char *s; int *n;
#else
char *f_list(char*);
char *gt_num(char *s, int *n)
#endif
{	int m=0,f__cnt=0;
	char c;
	for(c= *s;;c = *s)
	{	if(c==' ')
		{	s++;
			continue;
		}
		if(c>'9' || c<'0') break;
		m=10*m+c-'0';
		f__cnt++;
		s++;
	}
	if(f__cnt==0) *n=1;
	else *n=m;
	return(s);
}
#ifdef KR_headers
char *f_s(s,curloc) char *s;
#else
char *f_s(char *s, int curloc)
#endif
{
	skip(s);
	if(*s++!='(')
	{
		return(NULL);
	}
	if(f__parenlvl++ ==1) f__revloc=curloc;
	if(op_gen(RET1,curloc,0,0)<0 ||
		(s=f_list(s))==NULL)
	{
		return(NULL);
	}
	skip(s);
	return(s);
}
#ifdef KR_headers
ne_d(s,p) char *s,**p;
#else
ne_d(char *s, char **p)
#endif
{	int n,x,sign=0;
	struct f__syl *sp;
	switch(*s)
	{
	default:
		return(0);
	case ':': (void) op_gen(COLON,0,0,0); break;
	case '$':
		(void) op_gen(NONL, 0, 0, 0); break;
	case 'B':
	case 'b':
		if(*++s=='z' || *s == 'Z') (void) op_gen(BZ,0,0,0);
		else (void) op_gen(BN,0,0,0);
		break;
	case 'S':
	case 's':
		if(*(s+1)=='s' || *(s+1) == 'S')
		{	x=SS;
			s++;
		}
		else if(*(s+1)=='p' || *(s+1) == 'P')
		{	x=SP;
			s++;
		}
		else x=S;
		(void) op_gen(x,0,0,0);
		break;
	case '/': (void) op_gen(SLASH,0,0,0); break;
	case '-': sign=1;
	case '+':	s++;	/*OUTRAGEOUS CODING TRICK*/
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		s=gt_num(s,&n);
		switch(*s)
		{
		default:
			return(0);
		case 'P':
		case 'p': if(sign) n= -n; (void) op_gen(P,n,0,0); break;
		case 'X':
		case 'x': (void) op_gen(X,n,0,0); break;
		case 'H':
		case 'h':
			sp = &f__syl[op_gen(H,n,0,0)];
			*(char **)&sp->p2 = s + 1;
			s+=n;
			break;
		}
		break;
	case GLITCH:
	case '"':
	case '\'':
		sp = &f__syl[op_gen(APOS,0,0,0)];
		*(char **)&sp->p2 = s;
		if((*p = ap_end(s)) == NULL)
			return(0);
		return(1);
	case 'T':
	case 't':
		if(*(s+1)=='l' || *(s+1) == 'L')
		{	x=TL;
			s++;
		}
		else if(*(s+1)=='r'|| *(s+1) == 'R')
		{	x=TR;
			s++;
		}
		else x=T;
		s=gt_num(s+1,&n);
		s--;
		(void) op_gen(x,n,0,0);
		break;
	case 'X':
	case 'x': (void) op_gen(X,1,0,0); break;
	case 'P':
	case 'p': (void) op_gen(P,1,0,0); break;
	}
	s++;
	*p=s;
	return(1);
}
#ifdef KR_headers
e_d(s,p) char *s,**p;
#else
e_d(char *s, char **p)
#endif
{	int i,im,n,w,d,e,found=0,x=0;
	char *sv=s;
	s=gt_num(s,&n);
	(void) op_gen(STACK,n,0,0);
	switch(*s++)
	{
	default: break;
	case 'E':
	case 'e':	x=1;
	case 'G':
	case 'g':
		found=1;
		s=gt_num(s,&w);
		if(w==0) break;
		if(*s=='.')
		{	s++;
			s=gt_num(s,&d);
		}
		else d=0;
		if(*s!='E' && *s != 'e')
			(void) op_gen(x==1?E:G,w,d,0);	/* default is Ew.dE2 */
		else
		{	s++;
			s=gt_num(s,&e);
			(void) op_gen(x==1?EE:GE,w,d,e);
		}
		break;
	case 'O':
	case 'o':
		i = O;
		im = OM;
		goto finish_I;
	case 'Z':
	case 'z':
		i = Z;
		im = ZM;
		goto finish_I;
	case 'L':
	case 'l':
		found=1;
		s=gt_num(s,&w);
		if(w==0) break;
		(void) op_gen(L,w,0,0);
		break;
	case 'A':
	case 'a':
		found=1;
		skip(s);
		if(*s>='0' && *s<='9')
		{	s=gt_num(s,&w);
			if(w==0) break;
			(void) op_gen(AW,w,0,0);
			break;
		}
		(void) op_gen(A,0,0,0);
		break;
	case 'F':
	case 'f':
		found=1;
		s=gt_num(s,&w);
		if(w==0) break;
		if(*s=='.')
		{	s++;
			s=gt_num(s,&d);
		}
		else d=0;
		(void) op_gen(F,w,d,0);
		break;
	case 'D':
	case 'd':
		found=1;
		s=gt_num(s,&w);
		if(w==0) break;
		if(*s=='.')
		{	s++;
			s=gt_num(s,&d);
		}
		else d=0;
		(void) op_gen(D,w,d,0);
		break;
	case 'I':
	case 'i':
		i = I;
		im = IM;
 finish_I:
		found=1;
		s=gt_num(s,&w);
		if(w==0) break;
		if(*s!='.')
		{	(void) op_gen(i,w,0,0);
			break;
		}
		s++;
		s=gt_num(s,&d);
		(void) op_gen(im,w,d,0);
		break;
	}
	if(found==0)
	{	f__pc--; /*unSTACK*/
		*p=sv;
		return(0);
	}
	*p=s;
	return(1);
}
#ifdef KR_headers
char *i_tem(s) char *s;
#else
char *i_tem(char *s)
#endif
{	char *t;
	int n,curloc;
	if(*s==')') return(s);
	if(ne_d(s,&t)) return(t);
	if(e_d(s,&t)) return(t);
	s=gt_num(s,&n);
	if((curloc=op_gen(STACK,n,0,0))<0) return(NULL);
	return(f_s(s,curloc));
}
#ifdef KR_headers
char *f_list(s) char *s;
#else
char *f_list(char *s)
#endif
{
	for(;*s!=0;)
	{	skip(s);
		if((s=i_tem(s))==NULL) return(NULL);
		skip(s);
		if(*s==',') s++;
		else if(*s==')')
		{	if(--f__parenlvl==0)
			{
				(void) op_gen(REVERT,f__revloc,0,0);
				return(++s);
			}
			(void) op_gen(GOTO,0,0,0);
			return(++s);
		}
	}
	return(NULL);
}

#ifdef KR_headers
pars_f(s) char *s;
#else
pars_f(char *s)
#endif
{
	f__parenlvl=f__revloc=f__pc=0;
	if(f_s(s,0) == NULL)
	{
		return(-1);
	}
	return(0);
}
#define STKSZ 10
int f__cnt[STKSZ],f__ret[STKSZ],f__cp,f__rp;
flag f__workdone, f__nonl;

#ifdef KR_headers
type_f(n)
#else
type_f(int n)
#endif
{
	switch(n)
	{
	default:
		return(n);
	case RET1:
		return(RET1);
	case REVERT: return(REVERT);
	case GOTO: return(GOTO);
	case STACK: return(STACK);
	case X:
	case SLASH:
	case APOS: case H:
	case T: case TL: case TR:
		return(NED);
	case F:
	case I:
	case IM:
	case A: case AW:
	case O: case OM:
	case L:
	case E: case EE: case D:
	case G: case GE:
	case Z: case ZM:
		return(ED);
	}
}
#ifdef KR_headers
integer do_fio(number,ptr,len) ftnint *number; ftnlen len; char *ptr;
#else
integer do_fio(ftnint *number, char *ptr, ftnlen len)
#endif
{	struct f__syl *p;
	int n,i;
	for(i=0;i<*number;i++,ptr+=len)
	{
loop:	switch(type_f((p= &f__syl[f__pc])->op))
	{
	default:
		fprintf(stderr,"unknown code in do_fio: %d\n%s\n",
			p->op,f__fmtbuf);
		err(f__elist->cierr,100,"do_fio");
	case NED:
		if((*f__doned)(p))
		{	f__pc++;
			goto loop;
		}
		f__pc++;
		continue;
	case ED:
		if(f__cnt[f__cp]<=0)
		{	f__cp--;
			f__pc++;
			goto loop;
		}
		if(ptr==NULL)
			return((*f__doend)());
		f__cnt[f__cp]--;
		f__workdone=1;
		if((n=(*f__doed)(p,ptr,len))>0) err(f__elist->cierr,errno,"fmt");
		if(n<0) err(f__elist->ciend,(EOF),"fmt");
		continue;
	case STACK:
		f__cnt[++f__cp]=p->p1;
		f__pc++;
		goto loop;
	case RET1:
		f__ret[++f__rp]=p->p1;
		f__pc++;
		goto loop;
	case GOTO:
		if(--f__cnt[f__cp]<=0)
		{	f__cp--;
			f__rp--;
			f__pc++;
			goto loop;
		}
		f__pc=1+f__ret[f__rp--];
		goto loop;
	case REVERT:
		f__rp=f__cp=0;
		f__pc = p->p1;
		if(ptr==NULL)
			return((*f__doend)());
		if(!f__workdone) return(0);
		if((n=(*f__dorevert)()) != 0) return(n);
		goto loop;
	case COLON:
		if(ptr==NULL)
			return((*f__doend)());
		f__pc++;
		goto loop;
	case NONL:
		f__nonl = 1;
		f__pc++;
		goto loop;
	case S:
	case SS:
		f__cplus=0;
		f__pc++;
		goto loop;
	case SP:
		f__cplus = 1;
		f__pc++;
		goto loop;
	case P:	f__scale=p->p1;
		f__pc++;
		goto loop;
	case BN:
		f__cblank=0;
		f__pc++;
		goto loop;
	case BZ:
		f__cblank=1;
		f__pc++;
		goto loop;
	}
	}
	return(0);
}
en_fio(Void)
{	ftnint one=1;
	return(do_fio(&one,(char *)NULL,(ftnint)0));
}
 VOID
fmt_bg(Void)
{
	f__workdone=f__cp=f__rp=f__pc=f__cursor=0;
	f__cnt[0]=f__ret[0]=0;
}
./ ADD NAME=libI77/fmt.h TIME=719848230
struct f__syl
{	int op,p1,p2,p3;
};
#define RET1 1
#define REVERT 2
#define GOTO 3
#define X 4
#define SLASH 5
#define STACK 6
#define I 7
#define ED 8
#define NED 9
#define IM 10
#define APOS 11
#define H 12
#define TL 13
#define TR 14
#define T 15
#define COLON 16
#define S 17
#define SP 18
#define SS 19
#define P 20
#define BN 21
#define BZ 22
#define F 23
#define E 24
#define EE 25
#define D 26
#define G 27
#define GE 28
#define L 29
#define A 30
#define AW 31
#define O 32
#define NONL 33
#define OM 34
#define Z 35
#define ZM 36
extern struct f__syl f__syl[];
extern int f__pc,f__parenlvl,f__revloc;
typedef union
{	real pf;
	doublereal pd;
} ufloat;
typedef union
{	short is;
	char ic;
	long il;
} Uint;
#ifdef KR_headers
extern int (*f__doed)(),(*f__doned)();
extern int (*f__dorevert)(), (*f__doend)();
extern int rd_ed(),rd_ned();
extern int w_ed(),w_ned();
#else
#ifdef __cplusplus
extern "C" {
#endif
extern int (*f__doed)(struct f__syl*, char*, ftnlen),(*f__doned)(struct f__syl*);
extern int (*f__dorevert)(void), (*f__doend)(void);
extern void fmt_bg(void);
extern int pars_f(char*);
extern int rd_ed(struct f__syl*, char*, ftnlen),rd_ned(struct f__syl*);
extern int w_ed(struct f__syl*, char*, ftnlen),w_ned(struct f__syl*);
extern int wrt_E(ufloat*, int, int, int, ftnlen);
extern int wrt_F(ufloat*, int, int, ftnlen);
extern int wrt_L(Uint*, int, ftnlen);
#ifdef __cplusplus
	}
#endif
#endif
extern flag f__cblank,f__cplus,f__workdone, f__nonl;
extern char *f__fmtbuf;
extern int f__scale;
#define GET(x) if((x=(*f__getn)())<0) return(x)
#define VAL(x) (x!='\n'?x:' ')
#define PUT(x) (*f__putn)(x)
extern int f__cursor;
./ ADD NAME=libI77/fmtlib.c TIME=719848230
/*	@(#)fmtlib.c	1.2	*/
#define MAXINTLENGTH 23
#ifdef __cplusplus
extern "C" {
#endif
#ifdef KR_headers
char *f__icvt(value,ndigit,sign, base) long value; int *ndigit,*sign;
 register int base;
#else
char *f__icvt(long value, int *ndigit, int *sign, int base)
#endif
{	static char buf[MAXINTLENGTH+1];
	register int i;
	if(value>0) *sign=0;
	else if(value<0)
	{	value = -value;
		*sign= 1;
	}
	else
	{	*sign=0;
		*ndigit=1;
		buf[MAXINTLENGTH]='0';
		return(&buf[MAXINTLENGTH]);
	}
	for(i=MAXINTLENGTH-1;value>0;i--)
	{	*(buf+i)=(int)(value%base)+'0';
		value /= base;
	}
	*ndigit=MAXINTLENGTH-1-i;
	return(&buf[i+1]);
}
#ifdef __cplusplus
	}
#endif
./ ADD NAME=libI77/fp.h TIME=580998005
#define FMAX 40
#define EXPMAXDIGS 8
#define EXPMAX 99999999
/* FMAX = max number of nonzero digits passed to atof() */
/* EXPMAX = 10^EXPMAXDIGS - 1 = largest allowed exponent absolute value */

#include "local.h"

/* MAXFRACDIGS and MAXINTDIGS are for wrt_F -- bounds (not necessarily
   tight) on the maximum number of digits to the right and left of
 * the decimal point.
 */

#ifdef VAX
#define MAXFRACDIGS 56
#define MAXINTDIGS 38
#else
#ifdef CRAY
#define MAXFRACDIGS 9880
#define MAXINTDIGS 9864
#else
/* values that suffice for IEEE double */
#define MAXFRACDIGS 344
#define MAXINTDIGS 308
#endif
#endif
./ ADD NAME=libI77/iio.c TIME=719848231
#include "f2c.h"
#include "fio.h"
#include "fmt.h"
extern char *f__icptr;
char *f__icend;
extern icilist *f__svic;
int f__icnum;
extern int f__hiwater;
z_getc(Void)
{
	if(f__recpos++ < f__svic->icirlen) {
		if(f__icptr >= f__icend) err(f__svic->iciend,(EOF),"endfile");
		return(*f__icptr++);
		}
	err(f__svic->icierr,110,"recend");
}
#ifdef KR_headers
z_putc(c)
#else
z_putc(int c)
#endif
{
	if(f__icptr >= f__icend) err(f__svic->icierr,110,"inwrite");
	if(f__recpos++ < f__svic->icirlen)
		*f__icptr++ = c;
	else	err(f__svic->icierr,110,"recend");
	return 0;
}
z_rnew(Void)
{
	f__icptr = f__svic->iciunit + (++f__icnum)*f__svic->icirlen;
	f__recpos = 0;
	f__cursor = 0;
	f__hiwater = 0;
	return 1;
}

 static int
z_endp(Void)
{
	(*f__donewrec)();
	return 0;
	}

#ifdef KR_headers
c_si(a) icilist *a;
#else
c_si(icilist *a)
#endif
{
	f__elist = (cilist *)a;
	f__fmtbuf=a->icifmt;
	if(pars_f(f__fmtbuf)<0)
		err(a->icierr,100,"startint");
	fmt_bg();
	f__sequential=f__formatted=1;
	f__external=0;
	f__cblank=f__cplus=f__scale=0;
	f__svic=a;
	f__icnum=f__recpos=0;
	f__cursor = 0;
	f__hiwater = 0;
	f__icptr = a->iciunit;
	f__icend = f__icptr + a->icirlen*a->icirnum;
	f__curunit = 0;
	f__cf = 0;
	return(0);
}
y_ierr(Void)
{
	err(f__elist->cierr, 110, "iio");
}
#ifdef KR_headers
integer s_rsfi(a) icilist *a;
#else
integer s_rsfi(icilist *a)
#endif
{	int n;
	if(n=c_si(a)) return(n);
	f__reading=1;
	f__doed=rd_ed;
	f__doned=rd_ned;
	f__getn=z_getc;
	f__dorevert = y_ierr;
	f__donewrec = z_rnew;
	f__doend = z_endp;
	return(0);
}

z_wnew(Void)
{
	while(f__recpos++ < f__svic->icirlen)
		*f__icptr++ = ' ';
	f__recpos = 0;
	f__cursor = 0;
	f__hiwater = 0;
	f__icnum++;
	return 1;
}
#ifdef KR_headers
integer s_wsfi(a) icilist *a;
#else
integer s_wsfi(icilist *a)
#endif
{	int n;
	if(n=c_si(a)) return(n);
	f__reading=0;
	f__doed=w_ed;
	f__doned=w_ned;
	f__putn=z_putc;
	f__dorevert = y_ierr;
	f__donewrec = z_wnew;
	f__doend = z_endp;
	return(0);
}
integer e_rsfi(Void)
{	int n;
	n = en_fio();
	f__fmtbuf = NULL;
	return(n);
}
integer e_wsfi(Void)
{
	int n;
	n = en_fio();
	f__fmtbuf = NULL;
	if(f__icnum >= f__svic->icirnum)
		return(n);
	while(f__recpos++ < f__svic->icirlen)
		*f__icptr++ = ' ';
	return(n);
}
./ ADD NAME=libI77/ilnw.c TIME=719848231
#include "f2c.h"
#include "fio.h"
#include "lio.h"
extern char *f__icptr;
extern char *f__icend;
extern icilist *f__svic;
extern int f__icnum;
#ifdef KR_headers
extern int z_putc();
#else
extern int z_putc(int);
#endif

 static int
z_wSL(Void)
{
	while(f__recpos < f__svic->icirlen)
		z_putc(' ');
	return z_rnew();
	}

 VOID
#ifdef KR_headers
c_liw(a) icilist *a;
#else
c_liw(icilist *a)
#endif
{
	f__reading = 0;
	f__external = 0;
	f__formatted = 1;
	f__putn = z_putc;
	L_len = a->icirlen;
	f__donewrec = z_wSL;
	f__svic = a;
	f__icnum = f__recpos = 0;
	f__cursor = 0;
	f__cf = 0;
	f__curunit = 0;
	f__icptr = a->iciunit;
	f__icend = f__icptr + a->icirlen*a->icirnum;
	f__elist = (cilist *)a;
	}

#ifdef KR_headers
s_wsni(a) icilist *a;
#else
s_wsni(icilist *a)
#endif
{
	cilist ca;

	c_liw(a);
	ca.cifmt = a->icifmt;
	x_wsne(&ca);
	z_wSL();
	return 0;
	}

#ifdef KR_headers
integer s_wsli(a) icilist *a;
#else
integer s_wsli(icilist *a)
#endif
{
	f__lioproc = l_write;
	c_liw(a);
	return(0);
	}

integer e_wsli(Void)
{
	z_wSL();
	return(0);
	}
./ ADD NAME=libI77/inquire.c TIME=719848231
#include "f2c.h"
#include "fio.h"
#ifdef KR_headers
integer f_inqu(a) inlist *a;
#else
#ifdef MSDOS
#undef abs
#undef min
#undef max
#include "string.h"
#include "io.h"
#endif
integer f_inqu(inlist *a)
#endif
{	flag byfile;
	int i, n;
	unit *p;
	char buf[256];
	long x;
	if(a->infile!=NULL)
	{	byfile=1;
		g_char(a->infile,a->infilen,buf);
#ifdef MSDOS
		x = access(buf,0) ? -1 : 0;
		for(i=0,p=NULL;i<MXUNIT;i++)
			if(f__units[i].ufd != NULL
			 && f__units[i].ufnm != NULL
			 && !strcmp(f__units[i].ufnm,buf)) {
				p = &f__units[i];
				break;
				}
#else
		x=f__inode(buf, &n);
		for(i=0,p=NULL;i<MXUNIT;i++)
			if(f__units[i].uinode==x
			&& f__units[i].ufd!=NULL
			&& f__units[i].udev == n) {
				p = &f__units[i];
				break;
				}
#endif
	}
	else
	{
		byfile=0;
		if(a->inunit<MXUNIT && a->inunit>=0)
		{
			p= &f__units[a->inunit];
		}
		else
		{
			p=NULL;
		}
	}
	if(a->inex!=NULL)
		if(byfile && x != -1 || !byfile && p!=NULL)
			*a->inex=1;
		else *a->inex=0;
	if(a->inopen!=NULL)
		if(byfile) *a->inopen=(p!=NULL);
		else *a->inopen=(p!=NULL && p->ufd!=NULL);
	if(a->innum!=NULL) *a->innum= p-f__units;
	if(a->innamed!=NULL)
		if(byfile || p!=NULL && p->ufnm!=NULL)
			*a->innamed=1;
		else	*a->innamed=0;
	if(a->inname!=NULL)
		if(byfile)
			b_char(buf,a->inname,a->innamlen);
		else if(p!=NULL && p->ufnm!=NULL)
			b_char(p->ufnm,a->inname,a->innamlen);
	if(a->inacc!=NULL && p!=NULL && p->ufd!=NULL)
		if(p->url)
			b_char("DIRECT",a->inacc,a->inacclen);
		else	b_char("SEQUENTIAL",a->inacc,a->inacclen);
	if(a->inseq!=NULL)
		if(p!=NULL && p->url)
			b_char("NO",a->inseq,a->inseqlen);
		else	b_char("YES",a->inseq,a->inseqlen);
	if(a->indir!=NULL)
		if(p==NULL || p->url)
			b_char("YES",a->indir,a->indirlen);
		else	b_char("NO",a->indir,a->indirlen);
	if(a->infmt!=NULL)
		if(p!=NULL && p->ufmt==0)
			b_char("UNFORMATTED",a->infmt,a->infmtlen);
		else	b_char("FORMATTED",a->infmt,a->infmtlen);
	if(a->inform!=NULL)
		if(p!=NULL && p->ufmt==0)
		b_char("NO",a->inform,a->informlen);
		else b_char("YES",a->inform,a->informlen);
	if(a->inunf)
		if(p!=NULL && p->ufmt==0)
			b_char("YES",a->inunf,a->inunflen);
		else if (p!=NULL) b_char("NO",a->inunf,a->inunflen);
		else b_char("UNKNOWN",a->inunf,a->inunflen);
	if(a->inrecl!=NULL && p!=NULL)
		*a->inrecl=p->url;
	if(a->innrec!=NULL && p!=NULL && p->url>0)
		*a->innrec=ftell(p->ufd)/p->url+1;
	if(a->inblank && p!=NULL && p->ufmt)
		if(p->ublnk)
			b_char("ZERO",a->inblank,a->inblanklen);
		else	b_char("NULL",a->inblank,a->inblanklen);
	return(0);
}
./ ADD NAME=libI77/lio.h TIME=719848231
/*	copy of ftypes from the compiler */
/* variable types
 * numeric assumptions:
 *	int < reals < complexes
 *	TYDREAL-TYREAL = TYDCOMPLEX-TYCOMPLEX
 */

/* 0-10 retain their old (pre LOGICAL*1, etc.) */
/* values to allow mixing old and new objects. */

#define TYUNKNOWN 0
#define TYADDR 1
#define TYSHORT 2
#define TYLONG 3
#define TYREAL 4
#define TYDREAL 5
#define TYCOMPLEX 6
#define TYDCOMPLEX 7
#define TYLOGICAL 8
#define TYCHAR 9
#define TYSUBR 10
#define TYINT1 11
#define TYLOGICAL1 12
#define TYLOGICAL2 13
#define TYERROR 14

#define NTYPES (TYERROR+1)

#define	LINTW	12
#define	LINE	80
#define	LLOGW	2
#ifdef Old_list_output
#define	LLOW	1.0
#define	LHIGH	1.e9
#define	LEFMT	" %# .8E"
#define	LFFMT	" %# .9g"
#else
#define	LGFMT	"%.9G"
#endif
/* LEFBL 20 should suffice; 24 overcomes a NeXT bug. */
#define	LEFBL	24

typedef union
{
	char	flchar;
	short	flshort;
	ftnint	flint;
	real	flreal;
	doublereal	fldouble;
} flex;
extern int f__scale;
#ifdef KR_headers
extern int (*f__lioproc)(), (*l_getc)(), (*l_ungetc)();
extern int l_read(), l_write();
#else
#ifdef __cplusplus
extern "C" {
#endif
extern int (*f__lioproc)(ftnint*, char*, ftnlen, ftnint);
extern int l_write(ftnint*, char*, ftnlen, ftnint);
extern void x_wsne(cilist*);
extern int c_le(cilist*), (*l_getc)(void), (*l_ungetc)(int,FILE*);
extern int l_read(ftnint*,char*,ftnlen,ftnint);
extern integer e_rsle(void);
extern int z_rnew(void);
#ifdef __cplusplus
	}
#endif
#endif
extern int L_len;
./ ADD NAME=libI77/lread.c TIME=719848231
#include "f2c.h"
#include "fio.h"
#include "fmt.h"
#include "lio.h"
#include "ctype.h"
#include "fp.h"

extern char *f__fmtbuf;
#ifdef KR_headers
extern double atof();
extern char *malloc(), *realloc();
int (*f__lioproc)(), (*l_getc)(), (*l_ungetc)();
#else
#undef abs
#undef min
#undef max
#include "stdlib.h"
int (*f__lioproc)(ftnint*, char*, ftnlen, ftnint), (*l_getc)(void),
	(*l_ungetc)(int,FILE*);
#endif
int l_eof;

#define isblnk(x) (f__ltab[x+1]&B)
#define issep(x) (f__ltab[x+1]&SX)
#define isapos(x) (f__ltab[x+1]&AX)
#define isexp(x) (f__ltab[x+1]&EX)
#define issign(x) (f__ltab[x+1]&SG)
#define iswhit(x) (f__ltab[x+1]&WH)
#define SX 1
#define B 2
#define AX 4
#define EX 8
#define SG 16
#define WH 32
char f__ltab[128+1] = {	/* offset one for EOF */
	0,
	0,0,AX,0,0,0,0,0,0,WH|B,SX|WH,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	SX|B|WH,0,AX,0,0,0,0,AX,0,0,0,SG,SX,SG,0,SX,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,EX,EX,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	AX,0,0,0,EX,EX,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

#ifdef ungetc
 static int
#ifdef KR_headers
un_getc(x,f__cf) int x; FILE *f__cf;
#else
un_getc(int x, FILE *f__cf)
#endif
{ return ungetc(x,f__cf); }
#else
#define un_getc ungetc
#ifdef KR_headers
 extern int ungetc();
#endif
#endif

t_getc(Void)
{	int ch;
	if(f__curunit->uend) return(EOF);
	if((ch=getc(f__cf))!=EOF) return(ch);
	if(feof(f__cf))
		l_eof = f__curunit->uend = 1;
	return(EOF);
}
integer e_rsle(Void)
{
	int ch;
	if(f__curunit->uend) return(0);
	while((ch=t_getc())!='\n' && ch!=EOF);
	return(0);
}

flag f__lquit;
int f__lcount,f__ltype,nml_read;
char *f__lchar;
double f__lx,f__ly;
#define ERR(x) if(n=(x)) return(n)
#define GETC(x) (x=(*l_getc)())
#define Ungetc(x,y) (*l_ungetc)(x,y)

#ifdef KR_headers
l_R(poststar) int poststar;
#else
l_R(int poststar)
#endif
{
	char s[FMAX+EXPMAXDIGS+4];
	register int ch;
	register char *sp, *spe, *sp1;
	long e, exp;
	int havenum, havestar, se;

	if (!poststar) {
		if (f__lcount > 0)
			return(0);
		f__lcount = 1;
		}
	f__ltype = 0;
	exp = 0;
	havestar = 0;
retry:
	sp1 = sp = s;
	spe = sp + FMAX;
	havenum = 0;

	switch(GETC(ch)) {
		case '-': *sp++ = ch; sp1++; spe++;
		case '+':
			GETC(ch);
		}
	while(ch == '0') {
		++havenum;
		GETC(ch);
		}
	while(isdigit(ch)) {
		if (sp < spe) *sp++ = ch;
		else ++exp;
		GETC(ch);
		}
	if (ch == '*' && !poststar) {
		if (sp == sp1 || exp || *s == '-') {
			err(f__elist->cierr,112,"bad repetition count")
			}
		poststar = havestar = 1;
		*sp = 0;
		f__lcount = atoi(s);
		goto retry;
		}
	if (ch == '.') {
		GETC(ch);
		if (sp == sp1)
			while(ch == '0') {
				++havenum;
				--exp;
				GETC(ch);
				}
		while(isdigit(ch)) {
			if (sp < spe)
				{ *sp++ = ch; --exp; }
			GETC(ch);
			}
		}
	se = 0;
	if (issign(ch))
		goto signonly;
	if (isexp(ch)) {
		GETC(ch);
		if (issign(ch)) {
signonly:
			if (ch == '-') se = 1;
			GETC(ch);
			}
		if (!isdigit(ch)) {
bad:
			err(f__elist->cierr,112,"exponent field")
			}

		e = ch - '0';
		while(isdigit(GETC(ch))) {
			e = 10*e + ch - '0';
			if (e > EXPMAX)
				goto bad;
			}
		if (se)
			exp -= e;
		else
			exp += e;
		}
	(void) Ungetc(ch, f__cf);
	if (sp > sp1) {
		++havenum;
		while(*--sp == '0')
			++exp;
		if (exp)
			sprintf(sp+1, "e%ld", exp);
		else
			sp[1] = 0;
		f__lx = atof(s);
		}
	else
		f__lx = 0.;
	if (havenum)
		f__ltype = TYLONG;
	else
		switch(ch) {
			case ',':
			case '/':
				break;
			default:
				if (havestar && ( ch == ' '
						||ch == '\t'
						||ch == '\n'))
					break;
				if (nml_read > 1) {
					f__lquit = 2;
					return 0;
					}
				err(f__elist->cierr,112,"invalid number")
			}
	return 0;
	}

 static int
#ifdef KR_headers
rd_count(ch) register int ch;
#else
rd_count(register int ch)
#endif
{
	if (ch < '0' || ch > '9')
		return 1;
	f__lcount = ch - '0';
	while(GETC(ch) >= '0' && ch <= '9')
		f__lcount = 10*f__lcount + ch - '0';
	Ungetc(ch,f__cf);
	return f__lcount <= 0;
	}

l_C(Void)
{	int ch, nml_save;
	double lz;
	if(f__lcount>0) return(0);
	f__ltype=0;
	GETC(ch);
	if(ch!='(')
	{
		if (nml_read > 1 && (ch < '0' || ch > '9')) {
			Ungetc(ch,f__cf);
			f__lquit = 2;
			return 0;
			}
		if (rd_count(ch))
			if(!f__cf || !feof(f__cf))
				err(f__elist->cierr,112,"complex format")
			else
				err(f__elist->cierr,(EOF),"lread");
		if(GETC(ch)!='*')
		{
			if(!f__cf || !feof(f__cf))
				err(f__elist->cierr,112,"no star")
			else
				err(f__elist->cierr,(EOF),"lread");
		}
		if(GETC(ch)!='(')
		{	Ungetc(ch,f__cf);
			return(0);
		}
	}
	else
		f__lcount = 1;
	while(iswhit(GETC(ch)));
	Ungetc(ch,f__cf);
	nml_save = nml_read;
	nml_read = 0;
	if (ch = l_R(1))
		return ch;
	if (!f__ltype)
		err(f__elist->cierr,112,"no real part");
	lz = f__lx;
	while(iswhit(GETC(ch)));
	if(ch!=',')
	{	(void) Ungetc(ch,f__cf);
		err(f__elist->cierr,112,"no comma");
	}
	while(iswhit(GETC(ch)));
	(void) Ungetc(ch,f__cf);
	if (ch = l_R(1))
		return ch;
	if (!f__ltype)
		err(f__elist->cierr,112,"no imaginary part");
	while(iswhit(GETC(ch)));
	if(ch!=')') err(f__elist->cierr,112,"no )");
	f__ly = f__lx;
	f__lx = lz;
	nml_read = nml_save;
	return(0);
}
l_L(Void)
{
	int ch;
	if(f__lcount>0) return(0);
	f__ltype=0;
	GETC(ch);
	if(isdigit(ch))
	{
		rd_count(ch);
		if(GETC(ch)!='*')
			if(!f__cf || !feof(f__cf))
				err(f__elist->cierr,112,"no star")
			else
				err(f__elist->cierr,(EOF),"lread");
		GETC(ch);
	}
	if(ch == '.') GETC(ch);
	switch(ch)
	{
	case 't':
	case 'T':
		f__lx=1;
		break;
	case 'f':
	case 'F':
		f__lx=0;
		break;
	default:
		if(isblnk(ch) || issep(ch) || ch==EOF)
		{	(void) Ungetc(ch,f__cf);
			return(0);
		}
		else	err(f__elist->cierr,112,"logical");
	}
	f__ltype=TYLONG;
	f__lcount = 1;
	while(!issep(GETC(ch)) && ch!=EOF);
	(void) Ungetc(ch, f__cf);
	return(0);
}
#define BUFSIZE	128
l_CHAR(Void)
{	int ch,size,i;
	char quote,*p;
	if(f__lcount>0) return(0);
	f__ltype=0;
	if(f__lchar!=NULL) free(f__lchar);
	size=BUFSIZE;
	p=f__lchar=malloc((unsigned int)size);
	if(f__lchar==NULL) err(f__elist->cierr,113,"no space");

	GETC(ch);
	if(isdigit(ch)) {
		/* allow Fortran 8x-style unquoted string...	*/
		/* either find a repetition count or the string	*/
		f__lcount = ch - '0';
		*p++ = ch;
		for(i = 1;;) {
			switch(GETC(ch)) {
				case '*':
					if (f__lcount == 0) {
						f__lcount = 1;
						goto noquote;
						}
					p = f__lchar;
					goto have_lcount;
				case ',':
				case ' ':
				case '\t':
				case '\n':
				case '/':
					Ungetc(ch,f__cf);
					/* no break */
				case EOF:
					f__lcount = 1;
					f__ltype = TYCHAR;
					return *p = 0;
				}
			if (!isdigit(ch)) {
				f__lcount = 1;
				goto noquote;
				}
			*p++ = ch;
			f__lcount = 10*f__lcount + ch - '0';
			if (++i == size) {
				f__lchar = realloc(f__lchar,
					(unsigned int)(size += BUFSIZE));
				p = f__lchar + i;
				}
			}
		}
	else	(void) Ungetc(ch,f__cf);
 have_lcount:
	if(GETC(ch)=='\'' || ch=='"') quote=ch;
	else if(isblnk(ch) || (issep(ch) && ch != '\n') || ch==EOF)
	{	(void) Ungetc(ch,f__cf);
		return(0);
	}
	else {
		/* Fortran 8x-style unquoted string */
		*p++ = ch;
		for(i = 1;;) {
			switch(GETC(ch)) {
				case ',':
				case ' ':
				case '\t':
				case '\n':
				case '/':
					Ungetc(ch,f__cf);
					/* no break */
				case EOF:
					f__ltype = TYCHAR;
					return *p = 0;
				}
 noquote:
			*p++ = ch;
			if (++i == size) {
				f__lchar = realloc(f__lchar,
					(unsigned int)(size += BUFSIZE));
				p = f__lchar + i;
				}
			}
		}
	f__ltype=TYCHAR;
	for(i=0;;)
	{	while(GETC(ch)!=quote && ch!='\n'
			&& ch!=EOF && ++i<size) *p++ = ch;
		if(i==size)
		{
		newone:
			f__lchar= realloc(f__lchar, (unsigned int)(size += BUFSIZE));
			p=f__lchar+i-1;
			*p++ = ch;
		}
		else if(ch==EOF) return(EOF);
		else if(ch=='\n')
		{	if(*(p-1) != '\\') continue;
			i--;
			p--;
			if(++i<size) *p++ = ch;
			else goto newone;
		}
		else if(GETC(ch)==quote)
		{	if(++i<size) *p++ = ch;
			else goto newone;
		}
		else
		{	(void) Ungetc(ch,f__cf);
			*p = 0;
			return(0);
		}
	}
}
#ifdef KR_headers
c_le(a) cilist *a;
#else
c_le(cilist *a)
#endif
{
	f__fmtbuf="list io";
	if(a->ciunit>=MXUNIT || a->ciunit<0)
		err(a->cierr,101,"stler");
	f__scale=f__recpos=0;
	f__elist=a;
	f__curunit = &f__units[a->ciunit];
	if(f__curunit->ufd==NULL && fk_open(SEQ,FMT,a->ciunit))
		err(a->cierr,102,"lio");
	f__cf=f__curunit->ufd;
	if(!f__curunit->ufmt) err(a->cierr,103,"lio")
	return(0);
}
#ifdef KR_headers
l_read(number,ptr,len,type) ftnint *number,type; char *ptr; ftnlen len;
#else
l_read(ftnint *number, char *ptr, ftnlen len, ftnint type)
#endif
{
#define Ptr ((flex *)ptr)
	int i,n,ch;
	doublereal *yy;
	real *xx;
	for(i=0;i<*number;i++)
	{
		if(f__lquit) return(0);
		if(l_eof)
			err(f__elist->ciend, EOF, "list in")
		if(f__lcount == 0) {
			f__ltype = 0;
			for(;;)  {
				GETC(ch);
				switch(ch) {
				case EOF:
					goto loopend;
				case ' ':
				case '\t':
				case '\n':
					continue;
				case '/':
					f__lquit = 1;
					goto loopend;
				case ',':
					f__lcount = 1;
					goto loopend;
				default:
					(void) Ungetc(ch, f__cf);
					goto rddata;
				}
			}
		}
	rddata:
		switch((int)type)
		{
		case TYINT1:
		case TYSHORT:
		case TYLONG:
		case TYREAL:
		case TYDREAL:
			ERR(l_R(0));
			break;
		case TYCOMPLEX:
		case TYDCOMPLEX:
			ERR(l_C());
			break;
		case TYLOGICAL1:
		case TYLOGICAL2:
		case TYLOGICAL:
			ERR(l_L());
			break;
		case TYCHAR:
			ERR(l_CHAR());
			break;
		}
	while (GETC(ch) == ' ' || ch == '\t');
	if (ch != ',' || f__lcount > 1)
		Ungetc(ch,f__cf);
	loopend:
		if(f__lquit) return(0);
		if(f__cf) {
			if (feof(f__cf))
				err(f__elist->ciend,(EOF),"list in")
			else if(ferror(f__cf)) {
				clearerr(f__cf);
				err(f__elist->cierr,errno,"list in")
				}
			}
		if(f__ltype==0) goto bump;
		switch((int)type)
		{
		case TYINT1:
		case TYLOGICAL1:
			Ptr->flchar = f__lx;
			break;
		case TYLOGICAL2:
		case TYSHORT:
			Ptr->flshort=f__lx;
			break;
		case TYLOGICAL:
		case TYLONG:
			Ptr->flint=f__lx;
			break;
		case TYREAL:
			Ptr->flreal=f__lx;
			break;
		case TYDREAL:
			Ptr->fldouble=f__lx;
			break;
		case TYCOMPLEX:
			xx=(real *)ptr;
			*xx++ = f__lx;
			*xx = f__ly;
			break;
		case TYDCOMPLEX:
			yy=(doublereal *)ptr;
			*yy++ = f__lx;
			*yy = f__ly;
			break;
		case TYCHAR:
			b_char(f__lchar,ptr,len);
			break;
		}
	bump:
		if(f__lcount>0) f__lcount--;
		ptr += len;
		if (nml_read)
			nml_read++;
	}
	return(0);
#undef Ptr
}
#ifdef KR_headers
integer s_rsle(a) cilist *a;
#else
integer s_rsle(cilist *a)
#endif
{
	int n;

	if(!f__init) f_init();
	if(n=c_le(a)) return(n);
	f__reading=1;
	f__external=1;
	f__formatted=1;
	f__lioproc = l_read;
	f__lquit = 0;
	f__lcount = 0;
	l_eof = 0;
	if(f__curunit->uwrt && f__nowreading(f__curunit))
		err(a->cierr,errno,"read start");
	l_getc = t_getc;
	l_ungetc = un_getc;
	return(0);
}
./ ADD NAME=libI77/lwrite.c TIME=719848231
#include "f2c.h"
#include "fio.h"
#include "fmt.h"
#include "lio.h"
int L_len;

#ifdef KR_headers
t_putc(c)
#else
t_putc(int c)
#endif
{
	f__recpos++;
	putc(c,f__cf);
	return(0);
}
 static VOID
#ifdef KR_headers
lwrt_I(n) ftnint n;
#else
lwrt_I(ftnint n)
#endif
{
	char buf[LINTW],*p;
#ifdef USE_STRLEN
	(void) sprintf(buf," %ld",(long)n);
	if(f__recpos+strlen(buf)>=L_len)
#else
	if(f__recpos + sprintf(buf," %ld",(long)n) >= L_len)
#endif
		(*f__donewrec)();
	for(p=buf;*p;PUT(*p++));
}
 static VOID
#ifdef KR_headers
lwrt_L(n, len) ftnint n; ftnlen len;
#else
lwrt_L(ftnint n, ftnlen len)
#endif
{
	if(f__recpos+LLOGW>=L_len)
		(*f__donewrec)();
	wrt_L((Uint *)&n,LLOGW, len);
}
 static VOID
#ifdef KR_headers
lwrt_A(p,len) char *p; ftnlen len;
#else
lwrt_A(char *p, ftnlen len)
#endif
{
	int i;
	if(f__recpos+len>=L_len)
		(*f__donewrec)();
	if (!f__recpos)
		{ PUT(' '); ++f__recpos; }
	for(i=0;i<len;i++) PUT(*p++);
}

 static int
#ifdef KR_headers
l_g(buf, n) char *buf; double n;
#else
l_g(char *buf, double n)
#endif
{
#ifdef Old_list_output
	doublereal absn;
	char *fmt;

	absn = n;
	if (absn < 0)
		absn = -absn;
	fmt = LLOW <= absn && absn < LHIGH ? LFFMT : LEFMT;
#ifdef USE_STRLEN
	sprintf(buf, fmt, n);
	return strlen(buf);
#else
	return sprintf(buf, fmt, n);
#endif

#else
	register char *b, c, c1;

	b = buf;
	*b++ = ' ';
	if (n < 0) {
		*b++ = '-';
		n = -n;
		}
	else
		*b++ = ' ';
	if (n == 0) {
		*b++ = '0';
		*b++ = '.';
		*b = 0;
		goto f__ret;
		}
	sprintf(b, LGFMT, n);
	if (*b == '0') {
		while(b[0] = b[1])
			b++;
		}
	/* Fortran 77 insists on having a decimal point... */
	else for(;; b++)
		switch(*b) {
			case 0:
				*b++ = '.';
				*b = 0;
				goto f__ret;
			case '.':
				while(*++b);
				goto f__ret;
			case 'E':
				for(c1 = '.', c = 'E';  *b = c1;
					c1 = c, c = *++b);
				goto f__ret;
			}
 f__ret:
	return b - buf;
#endif
	}

 static VOID
#ifdef KR_headers
l_put(s) register char *s;
#else
l_put(register char *s)
#endif
{
#ifdef KR_headers
	register int c, (*pn)() = f__putn;
#else
	register int c, (*pn)(int) = f__putn;
#endif
	while(c = *s++)
		(*pn)(c);
	}

 static VOID
#ifdef KR_headers
lwrt_F(n) double n;
#else
lwrt_F(double n)
#endif
{
	char buf[LEFBL];

	if(f__recpos + l_g(buf,n) >= L_len)
		(*f__donewrec)();
	l_put(buf);
}
 static VOID
#ifdef KR_headers
lwrt_C(a,b) double a,b;
#else
lwrt_C(double a, double b)
#endif
{
	char *ba, *bb, bufa[LEFBL], bufb[LEFBL];
	int al, bl;

	al = l_g(bufa, a);
	for(ba = bufa; *ba == ' '; ba++)
		--al;
	bl = l_g(bufb, b) + 1;	/* intentionally high by 1 */
	for(bb = bufb; *bb == ' '; bb++)
		--bl;
	if(f__recpos + al + bl + 3 >= L_len && f__recpos)
		(*f__donewrec)();
	PUT(' ');
	PUT('(');
	l_put(ba);
	PUT(',');
	if (f__recpos + bl >= L_len) {
		(*f__donewrec)();
		PUT(' ');
		}
	l_put(bb);
	PUT(')');
}
#ifdef KR_headers
l_write(number,ptr,len,type) ftnint *number,type; char *ptr; ftnlen len;
#else
l_write(ftnint *number, char *ptr, ftnlen len, ftnint type)
#endif
{
#define Ptr ((flex *)ptr)
	int i;
	ftnint x;
	double y,z;
	real *xx;
	doublereal *yy;
	for(i=0;i< *number; i++)
	{
		switch((int)type)
		{
		default: f__fatal(204,"unknown type in lio");
		case TYINT1:
			x = Ptr->flchar;
			goto xint;
		case TYSHORT:
			x=Ptr->flshort;
			goto xint;
		case TYLONG:
			x=Ptr->flint;
		xint:	lwrt_I(x);
			break;
		case TYREAL:
			y=Ptr->flreal;
			goto xfloat;
		case TYDREAL:
			y=Ptr->fldouble;
		xfloat: lwrt_F(y);
			break;
		case TYCOMPLEX:
			xx= &Ptr->flreal;
			y = *xx++;
			z = *xx;
			goto xcomplex;
		case TYDCOMPLEX:
			yy = &Ptr->fldouble;
			y= *yy++;
			z = *yy;
		xcomplex:
			lwrt_C(y,z);
			break;
		case TYLOGICAL1:
			x = Ptr->flchar;
			goto xlog;
		case TYLOGICAL2:
			x = Ptr->flshort;
			goto xlog;
		case TYLOGICAL:
			x = Ptr->flint;
		xlog:	lwrt_L(Ptr->flint, len);
			break;
		case TYCHAR:
			lwrt_A(ptr,len);
			break;
		}
		ptr += len;
	}
	return(0);
}
./ ADD NAME=libI77/makefile TIME=711902952
.SUFFIXES: .c .o

CC = cc
CFLAGS = -DSkip_f2c_Undefs -O
SHELL = /bin/sh

# compile, then strip unnecessary symbols
.c.o:
	$(CC) $(CFLAGS) -c $*.c
	ld -r -x -o $*.xxx $*.o
	mv $*.xxx $*.o

OBJ =	Version.o backspace.o close.o dfe.o dolio.o due.o endfile.o err.o \
	fmt.o fmtlib.o iio.o ilnw.o inquire.o lread.o lwrite.o open.o \
	rdfmt.o rewind.o rsfe.o rsli.o rsne.o sfe.o sue.o typesize.o uio.o \
	util.o wref.o wrtfmt.o wsfe.o wsle.o wsne.o xwsne.o
libI77.a:	$(OBJ)
		ar r libI77.a $?
		ranlib libI77.a
install:	libI77.a
	cp libI77.a /usr/lib/libI77.a
	ranlib /usr/lib/libI77.a

Version.o: Version.c
	$(CC) -c Version.c


clean:
	rm -f $(OBJ) libI77.a

clobber:	clean
	rm -f libI77.a

backspace.o:	fio.h
close.o:	fio.h
dfe.o:		fio.h
dfe.o:		fmt.h
due.o:		fio.h
endfile.o:	fio.h rawio.h
err.o:		fio.h rawio.h
fmt.o:		fio.h
fmt.o:		fmt.h
iio.o:		fio.h
iio.o:		fmt.h
ilnw.o:		fio.h
ilnw.o:		lio.h
inquire.o:	fio.h
lread.o:	fio.h
lread.o:	fmt.h
lread.o:	lio.h
lread.o:	fp.h
lwrite.o:	fio.h
lwrite.o:	fmt.h
lwrite.o:	lio.h
open.o:		fio.h rawio.h
rdfmt.o:	fio.h
rdfmt.o:	fmt.h
rdfmt.o:	fp.h
rewind.o:	fio.h
rsfe.o:		fio.h
rsfe.o:		fmt.h
rsli.o:		fio.h
rsli.o:		lio.h
rsne.o:		fio.h
rsne.o:		lio.h
sfe.o:		fio.h
sue.o:		fio.h
uio.o:		fio.h
util.o:		fio.h
wref.o:		fio.h
wref.o:		fmt.h
wref.o:		fp.h
wrtfmt.o:	fio.h
wrtfmt.o:	fmt.h
wsfe.o:		fio.h
wsfe.o:		fmt.h
wsle.o:		fio.h
wsle.o:		fmt.h
wsle.o:		lio.h
wsne.o:		fio.h
wsne.o:		lio.h
xwsne.o:	fio.h
xwsne.o:	lio.h
xwsne.o:	fmt.h

check:
	xsum Notice README Version.c backspace.c close.c dfe.c dolio.c \
	due.c endfile.c err.c fio.h fmt.c fmt.h fmtlib.c fp.h iio.c \
	ilnw.c inquire.c lio.h lread.c lwrite.c makefile open.c rawio.h \
	rdfmt.c rewind.c rsfe.c rsli.c rsne.c sfe.c sue.c typesize.c \
	uio.c util.c wref.c wrtfmt.c wsfe.c wsle.c wsne.c xwsne.c >zap
	cmp zap libI77.xsum && rm zap || diff libI77.xsum zap
./ ADD NAME=libI77/open.c TIME=719848232
#include "sys/types.h"
#include "sys/stat.h"
#include "f2c.h"
#include "fio.h"
#include "string.h"
#include "fcntl.h"
#include "rawio.h"
#ifndef O_WRONLY
#define O_RDONLY 0
#define O_WRONLY 1
#endif

#ifdef KR_headers
extern char *malloc(), *mktemp();
extern FILE *fdopen();
extern integer f_clos();
#else
#undef abs
#undef min
#undef max
#include "stdlib.h"
extern int f__canseek(FILE*);
extern integer f_clos(cllist*);
#endif

#ifdef NON_ANSI_RW_MODES
char *r_mode[2] = {"r", "r"};
char *w_mode[2] = {"w", "w"};
#else
char *r_mode[2] = {"rb", "r"};
char *w_mode[2] = {"wb", "w"};
#endif

#ifdef KR_headers
f__isdev(s) char *s;
#else
f__isdev(char *s)
#endif
{
#ifdef MSDOS
	int i, j;

	i = open(s,O_RDONLY);
	if (i == -1)
		return 0;
	j = isatty(i);
	close(i);
	return j;
#else
	struct stat x;

	if(stat(s, &x) == -1) return(0);
#ifdef S_IFMT
	switch(x.st_mode&S_IFMT) {
		case S_IFREG:
		case S_IFDIR:
			return(0);
		}
#else
#ifdef S_ISREG
	/* POSIX version */
	if(S_ISREG(x.st_mode) || S_ISDIR(x.st_mode))
		return(0);
	else
#else
	Help! How does stat work on this system?
#endif
#endif
		return(1);
#endif
}
#ifdef KR_headers
integer f_open(a) olist *a;
#else
integer f_open(olist *a)
#endif
{	unit *b;
	int n;
	char buf[256];
	cllist x;
#ifndef MSDOS
	struct stat stb;
#endif
	if(a->ounit>=MXUNIT || a->ounit<0)
		err(a->oerr,101,"open")
	f__curunit = b = &f__units[a->ounit];
	if(b->ufd) {
		if(a->ofnm==0)
		{
		same:	if (a->oblnk)
				b->ublnk = *a->oblnk == 'z' || *a->oblnk == 'Z';
			return(0);
		}
#ifdef MSDOS
		if (b->ufnm
		 && strlen(b->ufnm) == a->ofnmlen
		 && !strncmp(b->ufnm, b->ufnm, (unsigned)a->ofnmlen))
			goto same;
#else
		g_char(a->ofnm,a->ofnmlen,buf);
		if (f__inode(buf,&n) == b->uinode && n == b->udev)
			goto same;
#endif
		x.cunit=a->ounit;
		x.csta=0;
		x.cerr=a->oerr;
		if((n=f_clos(&x))!=0) return(n);
		}
	b->url=a->orl;
	b->ublnk = a->oblnk && (*a->oblnk == 'z' || *a->oblnk == 'Z');
	if(a->ofm==0)
	{	if(b->url>0) b->ufmt=0;
		else b->ufmt=1;
	}
	else if(*a->ofm=='f' || *a->ofm == 'F') b->ufmt=1;
	else b->ufmt=0;
#ifdef url_Adjust
	if (b->url && !b->ufmt)
		url_Adjust(b->url);
#endif
	if (a->ofnm) {
		g_char(a->ofnm,a->ofnmlen,buf);
		if (!buf[0])
			err(a->oerr,107,"open")
		}
	else
		sprintf(buf, "fort.%ld", a->ounit);
	b->uscrtch = 0;
	switch(a->osta ? *a->osta : 'u')
	{
	case 'o':
	case 'O':
#ifdef MSDOS
		if(access(buf,0))
#else
		if(stat(buf,&stb))
#endif
			err(a->oerr,errno,"open")
		break;
	 case 's':
	 case 'S':
		b->uscrtch=1;
#ifdef _POSIX_SOURCE
		tmpnam(buf);
#else
		(void) strcpy(buf,"tmp.FXXXXXX");
		(void) mktemp(buf);
#endif
		(void) close(creat(buf, 0666));
		break;
	case 'n':
	case 'N':
#ifdef MSDOS
		if(!access(buf,0))
#else
		if(!stat(buf,&stb))
#endif
			err(a->oerr,128,"open")
		/* no break */
	case 'r':	/* Fortran 90 replace option */
	case 'R':
		(void) close(creat(buf, 0666));
		break;
	}

	b->ufnm=(char *) malloc((unsigned int)(strlen(buf)+1));
	if(b->ufnm==NULL) err(a->oerr,113,"no space");
	(void) strcpy(b->ufnm,buf);
	b->uend=0;
	b->uwrt = 0;
	if(f__isdev(buf))
	{	b->ufd = fopen(buf,r_mode[b->ufmt]);
		if(b->ufd==NULL) err(a->oerr,errno,buf)
	}
	else {
		if((b->ufd = fopen(buf, r_mode[b->ufmt])) == NULL) {
			if ((n = open(buf,O_WRONLY)) >= 0) {
				b->uwrt = 2;
				}
			else {
				n = creat(buf, 0666);
				b->uwrt = 1;
				}
			if (n < 0
			|| (b->ufd = fdopen(n, w_mode[b->ufmt])) == NULL)
				err(a->oerr, errno, "open");
			}
	}
	b->useek=f__canseek(b->ufd);
#ifndef MSDOS
	if((b->uinode=f__inode(buf,&b->udev))==-1)
		err(a->oerr,108,"open")
#endif
	if(a->orl && b->useek) rewind(b->ufd);
	return(0);
}
#ifdef KR_headers
fk_open(seq,fmt,n) ftnint n;
#else
fk_open(int seq, int fmt, ftnint n)
#endif
{	char nbuf[10];
	olist a;
	(void) sprintf(nbuf,"fort.%ld",n);
	a.oerr=1;
	a.ounit=n;
	a.ofnm=nbuf;
	a.ofnmlen=strlen(nbuf);
	a.osta=NULL;
	a.oacc= seq==SEQ?"s":"d";
	a.ofm = fmt==FMT?"f":"u";
	a.orl = seq==DIR?1:0;
	a.oblnk=NULL;
	return(f_open(&a));
}
./ ADD NAME=libI77/rawio.h TIME=718658082
#ifdef KR_headers
extern FILE *fdopen();
#else
#ifdef MSDOS
#include "io.h"
#define close _close
#define creat _creat
#define open _open
#define read _read
#define write _write
#endif
#ifdef __cplusplus
extern "C" {
#endif
#ifndef MSDOS
#ifdef OPEN_DECL
extern int creat(const char*,int), open(const char*,int);
#endif
extern int close(int);
extern int read(int,void*,size_t), write(int,void*,size_t);
extern int unlink(const char*);
#ifndef _POSIX_SOURCE
extern FILE *fdopen(int, const char*);
#endif
#endif

extern char *mktemp(char*);

#ifdef __cplusplus
	}
#endif
#endif
./ ADD NAME=libI77/rdfmt.c TIME=719848232
#include "f2c.h"
#include "fio.h"
#include "fmt.h"
#include "fp.h"

extern int f__cursor;
#ifdef KR_headers
extern double atof();
#else
#undef abs
#undef min
#undef max
#include "stdlib.h"
#endif

 static int
#ifdef KR_headers
rd_Z(n,w,len) Uint *n; ftnlen len;
#else
rd_Z(Uint *n, int w, ftnlen len)
#endif
{
	long x[9];
	char *s, *s0, *s1, *se, *t;
	int ch, i, w1, w2;
	static char hex[256];
	static int one = 1;
	int bad = 0;

	if (!hex['0']) {
		s = "0123456789";
		while(ch = *s++)
			hex[ch] = ch - '0' + 1;
		s = "ABCDEF";
		while(ch = *s++)
			hex[ch] = hex[ch + 'a' - 'A'] = ch - 'A' + 11;
		} 
	s = s0 = (char *)x;
	s1 = (char *)&x[4];
	se = (char *)&x[8];
	if (len > 4*sizeof(long))
		return errno = 117;
	while (w) {
		GET(ch);
		if (ch==',' || ch=='\n')
			break;
		w--;
		if (ch > ' ') {
			if (!hex[ch & 0xff])
				bad++;
			*s++ = ch;
			if (s == se) {
				/* discard excess characters */
				for(t = s0, s = s1; t < s1;)
					*t++ = *s++;
				s = s1;
				}
			}
		}
	if (bad)
		return errno = 115;
	w = (int)len;
	w1 = s - s0;
	w2 = w1+1 >> 1;
	t = (char *)n;
	if (*(char *)&one) {
		/* little endian */
		t += w - 1;
		i = -1;
		}
	else
		i = 1;
	for(; w > w2; t += i, --w)
		*t = 0;
	if (!w)
		return 0;
	if (w < w2)
		s0 = s - (w << 1);
	else if (w1 & 1) {
		*t = hex[*s0++ & 0xff] - 1;
		if (!--w)
			return 0;
		t += i;
		}
	do {
		*t = hex[*s0 & 0xff]-1 << 4 | hex[s0[1] & 0xff]-1;
		t += i;
		s0 += 2;
		}
		while(--w);
	return 0;
	}

 static int
#ifdef KR_headers
rd_I(n,w,len, base) Uint *n; int w; ftnlen len; register int base;
#else
rd_I(Uint *n, int w, ftnlen len, register int base)
#endif
{	long x;
	int sign,ch;
	char s[84], *ps;
	ps=s; x=0;
	while (w)
	{
		GET(ch);
		if (ch==',' || ch=='\n') break;
		*ps=ch; ps++; w--;
	}
	*ps='\0';
	ps=s;
	while (*ps==' ') ps++;
	if (*ps=='-') { sign=1; ps++; }
	else { sign=0; if (*ps=='+') ps++; }
loop:	while (*ps>='0' && *ps<='9') { x=x*base+(*ps-'0'); ps++; }
	if (*ps==' ') {if (f__cblank) x *= base; ps++; goto loop;}
	if(sign) x = -x;
	if(len==sizeof(integer)) n->il=x;
	else if(len == sizeof(char)) n->ic = (char)x;
	else n->is = (short)x;
	if (*ps) return(errno=115); else return(0);
}
 static int
#ifdef KR_headers
rd_L(n,w,len) ftnint *n; ftnlen len;
#else
rd_L(ftnint *n, int w, ftnlen len)
#endif
{	int ch, lv;
	char s[84], *ps;
	ps=s;
	while (w) {
		GET(ch);
		if (ch==','||ch=='\n') break;
		*ps=ch;
		ps++; w--;
		}
	*ps='\0';
	ps=s; while (*ps==' ') ps++;
	if (*ps=='.') ps++;
	if (*ps=='t' || *ps == 'T')
		lv = 1;
	else if (*ps == 'f' || *ps == 'F')
		lv = 0;
	else return(errno=116);
	switch(len) {
		case sizeof(char):	*(char *)n = (char)lv;	 break;
		case sizeof(short):	*(short *)n = (short)lv; break;
		default:		*n = lv;
		}
	return 0;
}

#include "ctype.h"

 static int
#ifdef KR_headers
rd_F(p, w, d, len) ufloat *p; ftnlen len;
#else
rd_F(ufloat *p, int w, int d, ftnlen len)
#endif
{
	char s[FMAX+EXPMAXDIGS+4];
	register int ch;
	register char *sp, *spe, *sp1;
	double x;
	int scale1, se;
	long e, exp;

	sp1 = sp = s;
	spe = sp + FMAX;
	exp = -d;
	x = 0.;

	do {
		GET(ch);
		w--;
		} while (ch == ' ' && w);
	switch(ch) {
		case '-': *sp++ = ch; sp1++; spe++;
		case '+':
			if (!w) goto zero;
			--w;
			GET(ch);
		}
	while(ch == ' ') {
blankdrop:
		if (!w--) goto zero; GET(ch); }
	while(ch == '0')
		{ if (!w--) goto zero; GET(ch); }
	if (ch == ' ' && f__cblank)
		goto blankdrop;
	scale1 = f__scale;
	while(isdigit(ch)) {
digloop1:
		if (sp < spe) *sp++ = ch;
		else ++exp;
digloop1e:
		if (!w--) goto done;
		GET(ch);
		}
	if (ch == ' ') {
		if (f__cblank)
			{ ch = '0'; goto digloop1; }
		goto digloop1e;
		}
	if (ch == '.') {
		exp += d;
		if (!w--) goto done;
		GET(ch);
		if (sp == sp1) { /* no digits yet */
			while(ch == '0') {
skip01:
				--exp;
skip0:
				if (!w--) goto done;
				GET(ch);
				}
			if (ch == ' ') {
				if (f__cblank) goto skip01;
				goto skip0;
				}
			}
		while(isdigit(ch)) {
digloop2:
			if (sp < spe)
				{ *sp++ = ch; --exp; }
digloop2e:
			if (!w--) goto done;
			GET(ch);
			}
		if (ch == ' ') {
			if (f__cblank)
				{ ch = '0'; goto digloop2; }
			goto digloop2e;
			}
		}
	switch(ch) {
	  default:
		break;
	  case '-': se = 1; goto signonly;
	  case '+': se = 0; goto signonly;
	  case 'e':
	  case 'E':
	  case 'd':
	  case 'D':
		if (!w--)
			goto bad;
		GET(ch);
		while(ch == ' ') {
			if (!w--)
				goto bad;
			GET(ch);
			}
		se = 0;
	  	switch(ch) {
		  case '-': se = 1;
		  case '+':
signonly:
			if (!w--)
				goto bad;
			GET(ch);
			}
		while(ch == ' ') {
			if (!w--)
				goto bad;
			GET(ch);
			}
		if (!isdigit(ch))
			goto bad;

		e = ch - '0';
		for(;;) {
			if (!w--)
				{ ch = '\n'; break; }
			GET(ch);
			if (!isdigit(ch)) {
				if (ch == ' ') {
					if (f__cblank)
						ch = '0';
					else continue;
					}
				else
					break;
				}
			e = 10*e + ch - '0';
			if (e > EXPMAX && sp > sp1)
				goto bad;
			}
		if (se)
			exp -= e;
		else
			exp += e;
		scale1 = 0;
		}
	switch(ch) {
	  case '\n':
	  case ',':
		break;
	  default:
bad:
		return (errno = 115);
		}
done:
	if (sp > sp1) {
		while(*--sp == '0')
			++exp;
		if (exp -= scale1)
			sprintf(sp+1, "e%ld", exp);
		else
			sp[1] = 0;
		x = atof(s);
		}
zero:
	if (len == sizeof(real))
		p->pf = x;
	else
		p->pd = x;
	return(0);
	}


 static int
#ifdef KR_headers
rd_A(p,len) char *p; ftnlen len;
#else
rd_A(char *p, ftnlen len)
#endif
{	int i,ch;
	for(i=0;i<len;i++)
	{	GET(ch);
		*p++=VAL(ch);
	}
	return(0);
}
 static int
#ifdef KR_headers
rd_AW(p,w,len) char *p; ftnlen len;
#else
rd_AW(char *p, int w, ftnlen len)
#endif
{	int i,ch;
	if(w>=len)
	{	for(i=0;i<w-len;i++)
			GET(ch);
		for(i=0;i<len;i++)
		{	GET(ch);
			*p++=VAL(ch);
		}
		return(0);
	}
	for(i=0;i<w;i++)
	{	GET(ch);
		*p++=VAL(ch);
	}
	for(i=0;i<len-w;i++) *p++=' ';
	return(0);
}
 static int
#ifdef KR_headers
rd_H(n,s) char *s;
#else
rd_H(int n, char *s)
#endif
{	int i,ch;
	for(i=0;i<n;i++)
		if((ch=(*f__getn)())<0) return(ch);
		else *s++ = ch=='\n'?' ':ch;
	return(1);
}
 static int
#ifdef KR_headers
rd_POS(s) char *s;
#else
rd_POS(char *s)
#endif
{	char quote;
	int ch;
	quote= *s++;
	for(;*s;s++)
		if(*s==quote && *(s+1)!=quote) break;
		else if((ch=(*f__getn)())<0) return(ch);
		else *s = ch=='\n'?' ':ch;
	return(1);
}
#ifdef KR_headers
rd_ed(p,ptr,len) struct f__syl *p; char *ptr; ftnlen len;
#else
rd_ed(struct f__syl *p, char *ptr, ftnlen len)
#endif
{	int ch;
	for(;f__cursor>0;f__cursor--) if((ch=(*f__getn)())<0) return(ch);
	if(f__cursor<0)
	{	if(f__recpos+f__cursor < 0) /*err(elist->cierr,110,"fmt")*/
			f__cursor = -f__recpos;	/* is this in the standard? */
		if(f__external == 0) {
			extern char *f__icptr;
			f__icptr += f__cursor;
		}
		else if(f__curunit && f__curunit->useek)
			(void) fseek(f__cf,(long) f__cursor,SEEK_CUR);
		else
			err(f__elist->cierr,106,"fmt");
		f__recpos += f__cursor;
		f__cursor=0;
	}
	switch(p->op)
	{
	default: fprintf(stderr,"rd_ed, unexpected code: %d\n", p->op);
		sig_die(f__fmtbuf, 1);
	case IM:
	case I: ch = rd_I((Uint *)ptr,p->p1,len, 10);
		break;

		/* O and OM don't work right for character, double, complex, */
		/* or doublecomplex, and they differ from Fortran 90 in */
		/* showing a minus sign for negative values. */

	case OM:
	case O: ch = rd_I((Uint *)ptr, p->p1, len, 8);
		break;
	case L: ch = rd_L((ftnint *)ptr,p->p1,len);
		break;
	case A:	ch = rd_A(ptr,len);
		break;
	case AW:
		ch = rd_AW(ptr,p->p1,len);
		break;
	case E: case EE:
	case D:
	case G:
	case GE:
	case F:	ch = rd_F((ufloat *)ptr,p->p1,p->p2,len);
		break;

		/* Z and ZM assume 8-bit bytes. */

	case ZM:
	case Z:
		ch = rd_Z((Uint *)ptr, p->p1, len);
		break;
	}
	if(ch == 0) return(ch);
	else if(ch == EOF) return(EOF);
	if (f__cf)
		clearerr(f__cf);
	return(errno);
}
#ifdef KR_headers
rd_ned(p) struct f__syl *p;
#else
rd_ned(struct f__syl *p)
#endif
{
	switch(p->op)
	{
	default: fprintf(stderr,"rd_ned, unexpected code: %d\n", p->op);
		sig_die(f__fmtbuf, 1);
	case APOS:
		return(rd_POS(*(char **)&p->p2));
	case H:	return(rd_H(p->p1,*(char **)&p->p2));
	case SLASH: return((*f__donewrec)());
	case TR:
	case X:	f__cursor += p->p1;
		return(1);
	case T: f__cursor=p->p1-f__recpos - 1;
		return(1);
	case TL: f__cursor -= p->p1;
		if(f__cursor < -f__recpos)	/* TL1000, 1X */
			f__cursor = -f__recpos;
		return(1);
	}
}
./ ADD NAME=libI77/rewind.c TIME=719848232
#include "f2c.h"
#include "fio.h"
#ifdef KR_headers
integer f_rew(a) alist *a;
#else
integer f_rew(alist *a)
#endif
{
	unit *b;
	if(a->aunit>=MXUNIT || a->aunit<0)
		err(a->aerr,101,"rewind");
	b = &f__units[a->aunit];
	if(b->ufd == NULL || b->uwrt == 3)
		return(0);
	if(!b->useek)
		err(a->aerr,106,"rewind")
	if(b->uwrt) {
		(void) t_runc(a);
		b->uwrt = 3;
		}
	rewind(b->ufd);
	b->uend=0;
	return(0);
}
./ ADD NAME=libI77/rsfe.c TIME=719848232
/* read sequential formatted external */
#include "f2c.h"
#include "fio.h"
#include "fmt.h"

xrd_SL(Void)
{	int ch;
	if(!f__curunit->uend)
		while((ch=getc(f__cf))!='\n' && ch!=EOF);
	f__cursor=f__recpos=0;
	return(1);
}
x_getc(Void)
{	int ch;
	if(f__curunit->uend) return(EOF);
	ch = getc(f__cf);
	if(ch!=EOF && ch!='\n')
	{	f__recpos++;
		return(ch);
	}
	if(ch=='\n')
	{	(void) ungetc(ch,f__cf);
		return(ch);
	}
	if(f__curunit->uend || feof(f__cf))
	{	errno=0;
		f__curunit->uend=1;
		return(-1);
	}
	return(-1);
}
x_endp(Void)
{
	(void) xrd_SL();
	return(0);
}
x_rev(Void)
{
	(void) xrd_SL();
	return(0);
}
#ifdef KR_headers
integer s_rsfe(a) cilist *a; /* start */
#else
integer s_rsfe(cilist *a) /* start */
#endif
{	int n;
	if(!f__init) f_init();
	if(n=c_sfe(a)) return(n);
	f__reading=1;
	f__sequential=1;
	f__formatted=1;
	f__external=1;
	f__elist=a;
	f__cursor=f__recpos=0;
	f__scale=0;
	f__fmtbuf=a->cifmt;
	f__curunit= &f__units[a->ciunit];
	f__cf=f__curunit->ufd;
	if(pars_f(f__fmtbuf)<0) err(a->cierr,100,"startio");
	f__getn= x_getc;
	f__doed= rd_ed;
	f__doned= rd_ned;
	fmt_bg();
	f__doend=x_endp;
	f__donewrec=xrd_SL;
	f__dorevert=x_rev;
	f__cblank=f__curunit->ublnk;
	f__cplus=0;
	if(f__curunit->uwrt && f__nowreading(f__curunit))
		err(a->cierr,errno,"read start");
	return(0);
}
./ ADD NAME=libI77/rsli.c TIME=719848232
#include "f2c.h"
#include "fio.h"
#include "lio.h"

extern flag f__lquit;
extern int f__lcount;
extern char *f__icptr;
extern char *f__icend;
extern icilist *f__svic;
extern int f__icnum, f__recpos;

int i_getc(Void)
{
	if(f__recpos >= f__svic->icirlen) {
		if (f__recpos++ == f__svic->icirlen)
			return '\n';
		z_rnew();
		}
	f__recpos++;
	if(f__icptr >= f__icend) err(f__svic->iciend,(EOF),"endfile");
	return(*f__icptr++);
	}

#ifdef KR_headers
int i_ungetc(ch, f) int ch; FILE *f;
#else
int i_ungetc(int ch, FILE *f)
#endif
{
	if (--f__recpos == f__svic->icirlen)
		return '\n';
	if (f__recpos < -1)
		err(f__svic->icierr,110,"recend");
	/* *--icptr == ch, and icptr may point to read-only memory */
	return *--f__icptr /* = ch */;
	}

 static void
#ifdef KR_headers
c_lir(a) icilist *a;
#else
c_lir(icilist *a)
#endif
{
	extern int l_eof;
	f__reading = 1;
	f__external = 0;
	f__formatted = 1;
	f__svic = a;
	L_len = a->icirlen;
	f__recpos = -1;
	f__icnum = f__recpos = 0;
	f__cursor = 0;
	l_getc = i_getc;
	l_ungetc = i_ungetc;
	l_eof = 0;
	f__icptr = a->iciunit;
	f__icend = f__icptr + a->icirlen*a->icirnum;
	f__cf = 0;
	f__curunit = 0;
	f__elist = (cilist *)a;
	}


#ifdef KR_headers
integer s_rsli(a) icilist *a;
#else
integer s_rsli(icilist *a)
#endif
{
	f__lioproc = l_read;
	f__lquit = 0;
	f__lcount = 0;
	c_lir(a);
	return(0);
	}

integer e_rsli(Void)
{ return 0; }

#ifdef KR_headers
s_rsni(a) icilist *a;
#else
extern int x_rsne(cilist*);

s_rsni(icilist *a)
#endif
{
	cilist ca;
	ca.ciend = a->iciend;
	ca.cierr = a->icierr;
	ca.cifmt = a->icifmt;
	c_lir(a);
	return x_rsne(&ca);
	}
./ ADD NAME=libI77/rsne.c TIME=719850201
#include "f2c.h"
#include "fio.h"
#include "lio.h"

#define MAX_NL_CACHE 3	/* maximum number of namelist hash tables to cache */
#define MAXDIM 20	/* maximum number of subscripts */

 struct dimen {
	ftnlen extent;
	ftnlen curval;
	ftnlen delta;
	ftnlen stride;
	};
 typedef struct dimen dimen;

 struct hashentry {
	struct hashentry *next;
	char *name;
	Vardesc *vd;
	};
 typedef struct hashentry hashentry;

 struct hashtab {
	struct hashtab *next;
	Namelist *nl;
	int htsize;
	hashentry *tab[1];
	};
 typedef struct hashtab hashtab;

 static hashtab *nl_cache;
 static n_nlcache;
 static hashentry **zot;
 extern ftnlen f__typesize[];

 extern flag f__lquit;
 extern int f__lcount, nml_read;
 extern t_getc(Void);

#ifdef KR_headers
 extern char *malloc(), *memset();

#ifdef ungetc
 static int
un_getc(x,f__cf) int x; FILE *f__cf;
{ return ungetc(x,f__cf); }
#else
#define un_getc ungetc
 extern int ungetc();
#endif

#else
#undef abs
#undef min
#undef max
#include "stdlib.h"
#include "string.h"

#ifdef ungetc
 static int
un_getc(int x, FILE *f__cf)
{ return ungetc(x,f__cf); }
#else
#define un_getc ungetc
#endif
#endif

 static Vardesc *
#ifdef KR_headers
hash(ht, s) hashtab *ht; register char *s;
#else
hash(hashtab *ht, register char *s)
#endif
{
	register int c, x;
	register hashentry *h;
	char *s0 = s;

	for(x = 0; c = *s++; x = x & 0x4000 ? ((x << 1) & 0x7fff) + 1 : x << 1)
		x += c;
	for(h = *(zot = ht->tab + x % ht->htsize); h; h = h->next)
		if (!strcmp(s0, h->name))
			return h->vd;
	return 0;
	}

 hashtab *
#ifdef KR_headers
mk_hashtab(nl) Namelist *nl;
#else
mk_hashtab(Namelist *nl)
#endif
{
	int nht, nv;
	hashtab *ht;
	Vardesc *v, **vd, **vde;
	hashentry *he;

	hashtab **x, **x0, *y;
	for(x = &nl_cache; y = *x; x0 = x, x = &y->next)
		if (nl == y->nl)
			return y;
	if (n_nlcache >= MAX_NL_CACHE) {
		/* discard least recently used namelist hash table */
		y = *x0;
		free((char *)y->next);
		y->next = 0;
		}
	else
		n_nlcache++;
	nv = nl->nvars;
	if (nv >= 0x4000)
		nht = 0x7fff;
	else {
		for(nht = 1; nht < nv; nht <<= 1);
		nht += nht - 1;
		}
	ht = (hashtab *)malloc(sizeof(hashtab) + (nht-1)*sizeof(hashentry *)
				+ nv*sizeof(hashentry));
	if (!ht)
		return 0;
	he = (hashentry *)&ht->tab[nht];
	ht->nl = nl;
	ht->htsize = nht;
	ht->next = nl_cache;
	nl_cache = ht;
	memset((char *)ht->tab, 0, nht*sizeof(hashentry *));
	vd = nl->vars;
	vde = vd + nv;
	while(vd < vde) {
		v = *vd++;
		if (!hash(ht, v->name)) {
			he->next = *zot;
			*zot = he;
			he->name = v->name;
			he->vd = v;
			he++;
			}
		}
	return ht;
	}

static char Alpha[256], Alphanum[256];

 static VOID
nl_init(Void) {
	register char *s;
	register int c;

	if(!f__init)
		f_init();
	for(s = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; c = *s++; )
		Alpha[c]
		= Alphanum[c]
		= Alpha[c + 'a' - 'A']
		= Alphanum[c + 'a' - 'A']
		= c;
	for(s = "0123456789_"; c = *s++; )
		Alphanum[c] = c;
	}

#define GETC(x) (x=(*l_getc)())
#define Ungetc(x,y) (*l_ungetc)(x,y)

 static int
#ifdef KR_headers
getname(s, slen) register char *s; int slen;
#else
getname(register char *s, int slen)
#endif
{
	register char *se = s + slen - 1;
	register int ch;

	GETC(ch);
	if (!(*s++ = Alpha[ch & 0xff])) {
		if (ch != EOF)
			ch = 115;
		err(f__elist->cierr, ch, "namelist read");
		}
	while(*s = Alphanum[GETC(ch) & 0xff])
		if (s < se)
			s++;
	if (ch == EOF)
		err(f__elist->cierr, EOF, "namelist read");
	if (ch > ' ')
		Ungetc(ch,f__cf);
	return *s = 0;
	}

 static int
#ifdef KR_headers
getnum(chp, val) int *chp; ftnlen *val;
#else
getnum(int *chp, ftnlen *val)
#endif
{
	register int ch, sign;
	register ftnlen x;

	while(GETC(ch) <= ' ' && ch >= 0);
	if (ch == '-') {
		sign = 1;
		GETC(ch);
		}
	else {
		sign = 0;
		if (ch == '+')
			GETC(ch);
		}
	x = ch - '0';
	if (x < 0 || x > 9)
		return 115;
	while(GETC(ch) >= '0' && ch <= '9')
		x = 10*x + ch - '0';
	while(ch <= ' ' && ch >= 0)
		GETC(ch);
	if (ch == EOF)
		return EOF;
	*val = sign ? -x : x;
	*chp = ch;
	return 0;
	}

 static int
#ifdef KR_headers
getdimen(chp, d, delta, extent, x1)
 int *chp; dimen *d; ftnlen delta, extent, *x1;
#else
getdimen(int *chp, dimen *d, ftnlen delta, ftnlen extent, ftnlen *x1)
#endif
{
	register int k;
	ftnlen x2, x3;

	if (k = getnum(chp, x1))
		return k;
	x3 = 1;
	if (*chp == ':') {
		if (k = getnum(chp, &x2))
			return k;
		x2 -= *x1;
		if (*chp == ':') {
			if (k = getnum(chp, &x3))
				return k;
			if (!x3)
				return 123;
			x2 /= x3;
			}
		if (x2 < 0 || x2 >= extent)
			return 123;
		d->extent = x2 + 1;
		}
	else
		d->extent = 1;
	d->curval = 0;
	d->delta = delta;
	d->stride = x3;
	return 0;
	}

 static char where0[] = "namelist read start ";

#ifdef KR_headers
x_rsne(a) cilist *a;
#else
x_rsne(cilist *a)
#endif
{
	int ch, got1, k, n, nd;
	Namelist *nl;
	static char where[] = "namelist read";
	char buf[64];
	hashtab *ht;
	Vardesc *v;
	dimen *dn, *dn0, *dn1;
	ftnlen *dims, *dims1;
	ftnlen b, b0, b1, ex, no, no1, nomax, size, span;
	ftnint type;
	char *vaddr;
	long iva, ivae;
	dimen dimens[MAXDIM], substr;

	if (!Alpha['a'])
		nl_init();
	f__reading=1;
	f__formatted=1;
	got1 = 0;
	for(;;) switch(GETC(ch)) {
		case EOF:
			err(a->ciend,(EOF),where0);
		case '&':
		case '$':
			goto have_amp;
		default:
			if (ch <= ' ' && ch >= 0)
				continue;
			err(a->cierr, 115, where0);
		}
 have_amp:
	if (ch = getname(buf,sizeof(buf)))
		return ch;
	nl = (Namelist *)a->cifmt;
	if (strcmp(buf, nl->name))
		err(a->cierr, 118, where0);
	ht = mk_hashtab(nl);
	if (!ht)
		err(f__elist->cierr, 113, where0);
	for(;;) {
		for(;;) switch(GETC(ch)) {
			case EOF:
				if (got1)
					return 0;
				err(a->ciend,(EOF),where0);
			case '/':
			case '$':
			case '&':
				return 0;
			default:
				if (ch <= ' ' && ch >= 0 || ch == ',')
					continue;
				Ungetc(ch,f__cf);
				if (ch = getname(buf,sizeof(buf)))
					return ch;
				goto havename;
			}
 havename:
		v = hash(ht,buf);
		if (!v)
			err(a->cierr, 119, where);
		while(GETC(ch) <= ' ' && ch >= 0);
		vaddr = v->addr;
		type = v->type;
		if (type < 0) {
			size = -type;
			type = TYCHAR;
			}
		else
			size = f__typesize[type];
		ivae = size;
		iva = 0;
		if (ch == '(' /*)*/ ) {
			dn = dimens;
			if (!(dims = v->dims)) {
				if (type != TYCHAR)
					err(a->cierr, 122, where);
				if (k = getdimen(&ch, dn, (ftnlen)size,
						(ftnlen)size, &b))
					err(a->cierr, k, where);
				if (ch != ')')
					err(a->cierr, 115, where);
				b1 = dn->extent;
				if (--b < 0 || b + b1 > size)
					return 124;
				iva += b;
				size = b1;
				while(GETC(ch) <= ' ' && ch >= 0);
				goto scalar;
				}
			nd = dims[0];
			nomax = span = dims[1];
			ivae = iva + size*nomax;
			if (k = getdimen(&ch, dn, size, nomax, &b))
				err(a->cierr, k, where);
			no = dn->extent;
			b0 = dims[2];
			dims1 = dims += 3;
			ex = 1;
			for(n = 1; n++ < nd; dims++) {
				if (ch != ',')
					err(a->cierr, 115, where);
				dn1 = dn + 1;
				span /= *dims;
				if (k = getdimen(&ch, dn1, dn->delta**dims,
						span, &b1))
					err(a->cierr, k, where);
				ex *= *dims;
				b += b1*ex;
				no *= dn1->extent;
				dn = dn1;
				}
			if (ch != ')')
				err(a->cierr, 115, where);
			b -= b0;
			if (b < 0 || b >= nomax)
				err(a->cierr, 125, where);
			iva += size * b;
			dims = dims1;
			while(GETC(ch) <= ' ' && ch >= 0);
			no1 = 1;
			dn0 = dimens;
			if (type == TYCHAR && ch == '(' /*)*/) {
				if (k = getdimen(&ch, &substr, size, size, &b))
					err(a->cierr, k, where);
				if (ch != ')')
					err(a->cierr, 115, where);
				b1 = substr.extent;
				if (--b < 0 || b + b1 > size)
					return 124;
				iva += b;
				b0 = size;
				size = b1;
				while(GETC(ch) <= ' ' && ch >= 0);
				if (b1 < b0)
					goto delta_adj;
				}
			for(; dn0 < dn; dn0++) {
				if (dn0->extent != *dims++ || dn0->stride != 1)
					break;
				no1 *= dn0->extent;
				}
			if (dn0 == dimens && dimens[0].stride == 1) {
				no1 = dimens[0].extent;
				dn0++;
				}
 delta_adj:
			ex = 0;
			for(dn1 = dn0; dn1 <= dn; dn1++)
				ex += (dn1->extent-1)
					* (dn1->delta *= dn1->stride);
			for(dn1 = dn; dn1 > dn0; dn1--) {
				ex -= (dn1->extent - 1) * dn1->delta;
				dn1->delta -= ex;
				}
			}
		else if (dims = v->dims) {
			no = no1 = dims[1];
			ivae = iva + no*size;
			}
		else
 scalar:
			no = no1 = 1;
		if (ch != '=')
			err(a->cierr, 115, where);
		got1 = nml_read = 1;
		f__lcount = 0;
	 readloop:
		for(;;) {
			if (iva >= ivae || iva < 0) {
				f__lquit = 1;
				goto mustend;
				}
			else if (iva + no1*size > ivae)
				no1 = (ivae - iva)/size;
			f__lquit = 0;
			l_read(&no1, vaddr + iva, size, type);
			if (f__lquit == 1)
				return 0;
 mustend:
			if (GETC(ch) == '/' || ch == '$' || ch == '&') {
				f__lquit = 1;
				return 0;
				}
			else if (f__lquit) {
				while(ch <= ' ' && ch >= 0)
					GETC(ch);
				Ungetc(ch,f__cf);
				if (!Alpha[ch & 0xff] && ch >= 0)
					err(a->cierr, 125, where);
				break;
				}
			Ungetc(ch,f__cf);
			if ((no -= no1) <= 0)
				break;
			for(dn1 = dn0; dn1 <= dn; dn1++) {
				if (++dn1->curval < dn1->extent) {
					iva += dn1->delta;
					goto readloop;
					}
				dn1->curval = 0;
				}
			break;
			}
		}
	}

 integer
#ifdef KR_headers
s_rsne(a) cilist *a;
#else
s_rsne(cilist *a)
#endif
{
	extern int l_eof;
	int n;

	f__external=1;
	l_eof = 0;
	if(n = c_le(a))
		return n;
	if(f__curunit->uwrt && f__nowreading(f__curunit))
		err(a->cierr,errno,where0);
	l_getc = t_getc;
	l_ungetc = un_getc;
	if (n = x_rsne(a))
		return n;
	return e_rsle();
	}
./ ADD NAME=libI77/sfe.c TIME=719848232
/* sequential formatted external common routines*/
#include "f2c.h"
#include "fio.h"

extern char *f__fmtbuf;

integer e_rsfe(Void)
{	int n;
	n=en_fio();
	if (f__cf == stdout)
		fflush(stdout);
	else if (f__cf == stderr)
		fflush(stderr);
	f__fmtbuf=NULL;
	return(n);
}
#ifdef KR_headers
c_sfe(a) cilist *a; /* check */
#else
c_sfe(cilist *a) /* check */
#endif
{	unit *p;
	if(a->ciunit >= MXUNIT || a->ciunit<0)
		err(a->cierr,101,"startio");
	p = &f__units[a->ciunit];
	if(p->ufd==NULL && fk_open(SEQ,FMT,a->ciunit)) err(a->cierr,114,"sfe")
	if(!p->ufmt) err(a->cierr,102,"sfe")
	return(0);
}
integer e_wsfe(Void)
{	return(e_rsfe());
}
./ ADD NAME=libI77/sue.c TIME=719848233
#include "f2c.h"
#include "fio.h"
extern uiolen f__reclen;
long f__recloc;

#ifdef KR_headers
c_sue(a) cilist *a;
#else
c_sue(cilist *a)
#endif
{
	if(a->ciunit >= MXUNIT || a->ciunit < 0)
		err(a->cierr,101,"startio");
	f__external=f__sequential=1;
	f__formatted=0;
	f__curunit = &f__units[a->ciunit];
	f__elist=a;
	if(f__curunit->ufd==NULL && fk_open(SEQ,UNF,a->ciunit))
		err(a->cierr,114,"sue");
	f__cf=f__curunit->ufd;
	if(f__curunit->ufmt) err(a->cierr,103,"sue")
	if(!f__curunit->useek) err(a->cierr,103,"sue")
	return(0);
}
#ifdef KR_headers
integer s_rsue(a) cilist *a;
#else
integer s_rsue(cilist *a)
#endif
{
	int n;
	if(!f__init) f_init();
	f__reading=1;
	if(n=c_sue(a)) return(n);
	f__recpos=0;
	if(f__curunit->uwrt && f__nowreading(f__curunit))
		err(a->cierr, errno, "read start");
	if(fread((char *)&f__reclen,sizeof(uiolen),1,f__cf)
		!= 1)
	{	if(feof(f__cf))
		{	f__curunit->uend = 1;
			err(a->ciend, EOF, "start");
		}
		clearerr(f__cf);
		err(a->cierr, errno, "start");
	}
	return(0);
}
#ifdef KR_headers
integer s_wsue(a) cilist *a;
#else
integer s_wsue(cilist *a)
#endif
{
	int n;
	if(!f__init) f_init();
	if(n=c_sue(a)) return(n);
	f__reading=0;
	f__reclen=0;
	if(f__curunit->uwrt != 1 && f__nowwriting(f__curunit))
		err(a->cierr, errno, "write start");
	f__recloc=ftell(f__cf);
	(void) fseek(f__cf,(long)sizeof(uiolen),SEEK_CUR);
	return(0);
}
integer e_wsue(Void)
{	long loc;
	(void) fwrite((char *)&f__reclen,sizeof(uiolen),1,f__cf);
	loc=ftell(f__cf);
	(void) fseek(f__cf,f__recloc,SEEK_SET);
	(void) fwrite((char *)&f__reclen,sizeof(uiolen),1,f__cf);
	(void) fseek(f__cf,loc,SEEK_SET);
	return(0);
}
integer e_rsue(Void)
{
	(void) fseek(f__cf,(long)(f__reclen-f__recpos+sizeof(uiolen)),SEEK_CUR);
	return(0);
}
./ ADD NAME=libI77/typesize.c TIME=719848233
#include "f2c.h"

ftnlen f__typesize[] = { 0, 0, sizeof(shortint), sizeof(integer),
			sizeof(real), sizeof(doublereal),
			sizeof(complex), sizeof(doublecomplex),
			sizeof(logical), sizeof(char) };
./ ADD NAME=libI77/uio.c TIME=719848233
#include "f2c.h"
#include "fio.h"
uiolen f__reclen;

#ifdef KR_headers
do_us(number,ptr,len) ftnint *number; char *ptr; ftnlen len;
#else
do_us(ftnint *number, char *ptr, ftnlen len)
#endif
{
	if(f__reading)
	{
		f__recpos += *number * len;
		if(f__recpos>f__reclen)
			err(f__elist->ciend, 110, "do_us");
		if (fread(ptr,(int)len,(int)(*number),f__cf) != *number)
			err(f__elist->ciend, EOF, "do_us");
		return(0);
	}
	else
	{
		f__reclen += *number * len;
		(void) fwrite(ptr,(int)len,(int)(*number),f__cf);
		return(0);
	}
}
#ifdef KR_headers
integer do_ud(number,ptr,len) ftnint *number; char *ptr; ftnlen len;
#else
integer do_ud(ftnint *number, char *ptr, ftnlen len)
#endif
{
	f__recpos += *number * len;
	if(f__recpos > f__curunit->url && f__curunit->url!=1)
		err(f__elist->cierr,110,"do_ud");
	if(f__reading)
	{
		if(fread(ptr,(int)len,(int)(*number),f__cf) != *number)
			err(f__elist->cierr,EOF,"do_ud")
		else return(0);
	}
	(void) fwrite(ptr,(int)len,(int)(*number),f__cf);
	return(0);
}
#ifdef KR_headers
integer do_uio(number,ptr,len) ftnint *number; char *ptr; ftnlen len;
#else
integer do_uio(ftnint *number, char *ptr, ftnlen len)
#endif
{
	if(f__sequential)
		return(do_us(number,ptr,len));
	else	return(do_ud(number,ptr,len));
}
./ ADD NAME=libI77/util.c TIME=719848233
#ifndef MSDOS
#include "sys/types.h"
#include "sys/stat.h"
#endif
#include "f2c.h"
#include "fio.h"

 VOID
#ifdef KR_headers
g_char(a,alen,b) char *a,*b; ftnlen alen;
#else
g_char(char *a, ftnlen alen, char *b)
#endif
{
	char *x = a + alen, *y = b + alen;

	for(;; y--) {
		if (x <= a) {
			*b = 0;
			return;
			}
		if (*--x != ' ')
			break;
		}
	*y-- = 0;
	do *y-- = *x;
		while(x-- > a);
	}

 VOID
#ifdef KR_headers
b_char(a,b,blen) char *a,*b; ftnlen blen;
#else
b_char(char *a, char *b, ftnlen blen)
#endif
{	int i;
	for(i=0;i<blen && *a!=0;i++) *b++= *a++;
	for(;i<blen;i++) *b++=' ';
}
#ifndef MSDOS
#ifdef KR_headers
long f__inode(a, dev) char *a; int *dev;
#else
long f__inode(char *a, int *dev)
#endif
{	struct stat x;
	if(stat(a,&x)<0) return(-1);
	*dev = x.st_dev;
	return(x.st_ino);
}
#endif

#define INTBOUND sizeof(int)-1
 VOID
#ifdef KR_headers
f__mvgbt(n,len,a,b) char *a,*b;
#else
f__mvgbt(int n, int len, char *a, char *b)
#endif
{	register int num=n*len;
	if( ((int)a&INTBOUND)==0 && ((int)b&INTBOUND)==0 && (num&INTBOUND)==0 )
	{	register int *x=(int *)a,*y=(int *)b;
		num /= sizeof(int);
		if(x>y) for(;num>0;num--) *y++= *x++;
		else for(num--;num>=0;num--) *(y+num)= *(x+num);
	}
	else
	{	register char *x=a,*y=b;
		if(x>y) for(;num>0;num--) *y++= *x++;
		else for(num--;num>=0;num--) *(y+num)= *(x+num);
	}
}
./ ADD NAME=libI77/wref.c TIME=719848233
#include "f2c.h"
#include "fio.h"
#include "fmt.h"
#include "fp.h"
#ifndef VAX
#include "ctype.h"
#endif

#ifndef KR_headers
#undef abs
#undef min
#undef max
#include "stdlib.h"
#include "string.h"
#endif

#ifdef KR_headers
wrt_E(p,w,d,e,len) ufloat *p; ftnlen len;
#else
wrt_E(ufloat *p, int w, int d, int e, ftnlen len)
#endif
{
	char buf[FMAX+EXPMAXDIGS+4], *s, *se;
	int d1, delta, e1, i, sign, signspace;
	double dd;
#ifndef VAX
	int e0 = e;
#endif

	if(e <= 0)
		e = 2;
	if(f__scale) {
		if(f__scale >= d + 2 || f__scale <= -d)
			goto nogood;
		}
	if(f__scale <= 0)
		--d;
	if (len == sizeof(real))
		dd = p->pf;
	else
		dd = p->pd;
	if (dd < 0.) {
		signspace = sign = 1;
		dd = -dd;
		}
	else {
		sign = 0;
		signspace = f__cplus;
#ifndef VAX
		if (!dd)
			dd = 0.;	/* avoid -0 */
#endif
		}
	delta = w - (2 /* for the . and the d adjustment above */
			+ 2 /* for the E+ */ + signspace + d + e);
	if (delta < 0) {
nogood:
		while(--w >= 0)
			PUT('*');
		return(0);
		}
	if (f__scale < 0)
		d += f__scale;
	if (d > FMAX) {
		d1 = d - FMAX;
		d = FMAX;
		}
	else
		d1 = 0;
	sprintf(buf,"%#.*E", d, dd);
#ifndef VAX
	/* check for NaN, Infinity */
	if (!isdigit(buf[0])) {
		switch(buf[0]) {
			case 'n':
			case 'N':
				signspace = 0;	/* no sign for NaNs */
			}
		delta = w - strlen(buf) - signspace;
		if (delta < 0)
			goto nogood;
		while(--delta >= 0)
			PUT(' ');
		if (signspace)
			PUT(sign ? '-' : '+');
		for(s = buf; *s; s++)
			PUT(*s);
		return 0;
		}
#endif
	se = buf + d + 3;
	if (f__scale != 1 && dd)
		sprintf(se, "%+.2d", atoi(se) + 1 - f__scale);
	s = ++se;
	if (e < 2) {
		if (*s != '0')
			goto nogood;
		}
#ifndef VAX
	/* accommodate 3 significant digits in exponent */
	if (s[2]) {
#ifdef Pedantic
		if (!e0 && !s[3])
			for(s -= 2, e1 = 2; s[0] = s[1]; s++);

	/* Pedantic gives the behavior that Fortran 77 specifies,	*/
	/* i.e., requires that E be specified for exponent fields	*/
	/* of more than 3 digits.  With Pedantic undefined, we get	*/
	/* the behavior that Cray displays -- you get a bigger		*/
	/* exponent field if it fits.	*/
#else
		if (!e0) {
			for(s -= 2, e1 = 2; s[0] = s[1]; s++)
#ifdef CRAY
				delta--;
			if ((delta += 4) < 0)
				goto nogood
#endif
				;
			}
#endif
		else if (e0 >= 0)
			goto shift;
		else
			e1 = e;
		}
	else
 shift:
#endif
		for(s += 2, e1 = 2; *s; ++e1, ++s)
			if (e1 >= e)
				goto nogood;
	while(--delta >= 0)
		PUT(' ');
	if (signspace)
		PUT(sign ? '-' : '+');
	s = buf;
	i = f__scale;
	if (f__scale <= 0) {
		PUT('.');
		for(; i < 0; ++i)
			PUT('0');
		PUT(*s);
		s += 2;
		}
	else if (f__scale > 1) {
		PUT(*s);
		s += 2;
		while(--i > 0)
			PUT(*s++);
		PUT('.');
		}
	if (d1) {
		se -= 2;
		while(s < se) PUT(*s++);
		se += 2;
		do PUT('0'); while(--d1 > 0);
		}
	while(s < se)
		PUT(*s++);
	if (e < 2)
		PUT(s[1]);
	else {
		while(++e1 <= e)
			PUT('0');
		while(*s)
			PUT(*s++);
		}
	return 0;
	}

#ifdef KR_headers
wrt_F(p,w,d,len) ufloat *p; ftnlen len;
#else
wrt_F(ufloat *p, int w, int d, ftnlen len)
#endif
{
	int d1, sign, n;
	double x;
	char *b, buf[MAXINTDIGS+MAXFRACDIGS+4], *s;

	x= (len==sizeof(real)?p->pf:p->pd);
	if (d < MAXFRACDIGS)
		d1 = 0;
	else {
		d1 = d - MAXFRACDIGS;
		d = MAXFRACDIGS;
		}
	if (x < 0.)
		{ x = -x; sign = 1; }
	else {
		sign = 0;
#ifndef VAX
		if (!x)
			x = 0.;
#endif
		}

	if (n = f__scale)
		if (n > 0)
			do x *= 10.; while(--n > 0);
		else
			do x *= 0.1; while(++n < 0);

#ifdef USE_STRLEN
	sprintf(b = buf, "%#.*f", d, x);
	n = strlen(b) + d1;
#else
	n = sprintf(b = buf, "%#.*f", d, x) + d1;
#endif

	if (buf[0] == '0' && d)
		{ ++b; --n; }
	if (sign) {
		/* check for all zeros */
		for(s = b;;) {
			while(*s == '0') s++;
			switch(*s) {
				case '.':
					s++; continue;
				case 0:
					sign = 0;
				}
			break;
			}
		}
	if (sign || f__cplus)
		++n;
	if (n > w) {
		while(--w >= 0)
			PUT('*');
		return 0;
		}
	for(w -= n; --w >= 0; )
		PUT(' ');
	if (sign)
		PUT('-');
	else if (f__cplus)
		PUT('+');
	while(n = *b++)
		PUT(n);
	while(--d1 >= 0)
		PUT('0');
	return 0;
	}
./ ADD NAME=libI77/wrtfmt.c TIME=719848233
#include "f2c.h"
#include "fio.h"
#include "fmt.h"
extern int f__cursor;
#ifdef KR_headers
extern char *f__icvt();
#else
extern char *f__icvt(long, int*, int*, int);
#endif
int f__hiwater;
icilist *f__svic;
char *f__icptr;
mv_cur(Void)	/* shouldn't use fseek because it insists on calling fflush */
		/* instead we know too much about stdio */
{
	if(f__external == 0) {
		if(f__cursor < 0) {
			if(f__hiwater < f__recpos)
				f__hiwater = f__recpos;
			f__recpos += f__cursor;
			f__icptr += f__cursor;
			f__cursor = 0;
			if(f__recpos < 0)
				err(f__elist->cierr, 110, "left off");
		}
		else if(f__cursor > 0) {
			if(f__recpos + f__cursor >= f__svic->icirlen)
				err(f__elist->cierr, 110, "recend");
			if(f__hiwater <= f__recpos)
				for(; f__cursor > 0; f__cursor--)
					(*f__putn)(' ');
			else if(f__hiwater <= f__recpos + f__cursor) {
				f__cursor -= f__hiwater - f__recpos;
				f__icptr += f__hiwater - f__recpos;
				f__recpos = f__hiwater;
				for(; f__cursor > 0; f__cursor--)
					(*f__putn)(' ');
			}
			else {
				f__icptr += f__cursor;
				f__recpos += f__cursor;
			}
			f__cursor = 0;
		}
		return(0);
	}
	if(f__cursor > 0) {
		if(f__hiwater <= f__recpos)
			for(;f__cursor>0;f__cursor--) (*f__putn)(' ');
		else if(f__hiwater <= f__recpos + f__cursor) {
#ifndef NON_UNIX_STDIO
			if(f__cf->_ptr + f__hiwater - f__recpos < buf_end(f__cf))
				f__cf->_ptr += f__hiwater - f__recpos;
			else
#endif
				(void) fseek(f__cf, (long) (f__hiwater - f__recpos), SEEK_CUR);
			f__cursor -= f__hiwater - f__recpos;
			f__recpos = f__hiwater;
			for(; f__cursor > 0; f__cursor--)
				(*f__putn)(' ');
		}
		else {
#ifndef NON_UNIX_STDIO
			if(f__cf->_ptr + f__cursor < buf_end(f__cf))
				f__cf->_ptr += f__cursor;
			else
#endif
				(void) fseek(f__cf, (long)f__cursor, SEEK_CUR);
			f__recpos += f__cursor;
		}
	}
	if(f__cursor<0)
	{
		if(f__cursor+f__recpos<0) err(f__elist->cierr,110,"left off");
#ifndef NON_UNIX_STDIO
		if(f__cf->_ptr + f__cursor >= f__cf->_base)
			f__cf->_ptr += f__cursor;
		else
#endif
		if(f__curunit && f__curunit->useek)
			(void) fseek(f__cf,(long)f__cursor,SEEK_CUR);
		else
			err(f__elist->cierr,106,"fmt");
		if(f__hiwater < f__recpos)
			f__hiwater = f__recpos;
		f__recpos += f__cursor;
		f__cursor=0;
	}
	return(0);
}

 static int
#ifdef KR_headers
wrt_Z(n,w,minlen,len) Uint *n; int w, minlen; ftnlen len;
#else
wrt_Z(Uint *n, int w, int minlen, ftnlen len)
#endif
{
	register char *s, *se;
	register i, w1;
	static int one = 1;
	static char hex[] = "0123456789ABCDEF";
	s = (char *)n;
	--len;
	if (*(char *)&one) {
		/* little endian */
		se = s;
		s += len;
		i = -1;
		}
	else {
		se = s + len;
		i = 1;
		}
	for(;; s += i, w1 += 2)
		if (s == se || *s)
			break;
	w1 = (i*(se-s) << 1) + 1;
	if (*s & 0xf0)
		w1++;
	if (w1 > w)
		for(i = 0; i < w; i++)
			(*f__putn)('*');
	else {
		if ((minlen -= w1) > 0)
			w1 += minlen;
		while(--w >= w1)
			(*f__putn)(' ');
		while(--minlen >= 0)
			(*f__putn)('0');
		if (!(*s & 0xf0)) {
			(*f__putn)(hex[*s & 0xf]);
			if (s == se)
				return 0;
			s += i;
			}
		for(;; s += i) {
			(*f__putn)(hex[*s >> 4 & 0xf]);
			(*f__putn)(hex[*s & 0xf]);
			if (s == se)
				break;
			}
		}
	return 0;
	}

 static int
#ifdef KR_headers
wrt_I(n,w,len, base) Uint *n; ftnlen len; register int base;
#else
wrt_I(Uint *n, int w, ftnlen len, register int base)
#endif
{	int ndigit,sign,spare,i;
	long x;
	char *ans;
	if(len==sizeof(integer)) x=n->il;
	else if(len == sizeof(char)) x = n->ic;
	else x=n->is;
	ans=f__icvt(x,&ndigit,&sign, base);
	spare=w-ndigit;
	if(sign || f__cplus) spare--;
	if(spare<0)
		for(i=0;i<w;i++) (*f__putn)('*');
	else
	{	for(i=0;i<spare;i++) (*f__putn)(' ');
		if(sign) (*f__putn)('-');
		else if(f__cplus) (*f__putn)('+');
		for(i=0;i<ndigit;i++) (*f__putn)(*ans++);
	}
	return(0);
}
 static int
#ifdef KR_headers
wrt_IM(n,w,m,len,base) Uint *n; ftnlen len; int base;
#else
wrt_IM(Uint *n, int w, int m, ftnlen len, int base)
#endif
{	int ndigit,sign,spare,i,xsign;
	long x;
	char *ans;
	if(sizeof(integer)==len) x=n->il;
	else if(len == sizeof(char)) x = n->ic;
	else x=n->is;
	ans=f__icvt(x,&ndigit,&sign, base);
	if(sign || f__cplus) xsign=1;
	else xsign=0;
	if(ndigit+xsign>w || m+xsign>w)
	{	for(i=0;i<w;i++) (*f__putn)('*');
		return(0);
	}
	if(x==0 && m==0)
	{	for(i=0;i<w;i++) (*f__putn)(' ');
		return(0);
	}
	if(ndigit>=m)
		spare=w-ndigit-xsign;
	else
		spare=w-m-xsign;
	for(i=0;i<spare;i++) (*f__putn)(' ');
	if(sign) (*f__putn)('-');
	else if(f__cplus) (*f__putn)('+');
	for(i=0;i<m-ndigit;i++) (*f__putn)('0');
	for(i=0;i<ndigit;i++) (*f__putn)(*ans++);
	return(0);
}
 static int
#ifdef KR_headers
wrt_AP(s) char *s;
#else
wrt_AP(char *s)
#endif
{	char quote;
	if(f__cursor && mv_cur()) return(mv_cur());
	quote = *s++;
	for(;*s;s++)
	{	if(*s!=quote) (*f__putn)(*s);
		else if(*++s==quote) (*f__putn)(*s);
		else return(1);
	}
	return(1);
}
 static int
#ifdef KR_headers
wrt_H(a,s) char *s;
#else
wrt_H(int a, char *s)
#endif
{
	if(f__cursor && mv_cur()) return(mv_cur());
	while(a--) (*f__putn)(*s++);
	return(1);
}
#ifdef KR_headers
wrt_L(n,len, sz) Uint *n; ftnlen sz;
#else
wrt_L(Uint *n, int len, ftnlen sz)
#endif
{	int i;
	long x;
	if(sizeof(long)==sz) x=n->il;
	else if(sz == sizeof(char)) x = n->ic;
	else x=n->is;
	for(i=0;i<len-1;i++)
		(*f__putn)(' ');
	if(x) (*f__putn)('T');
	else (*f__putn)('F');
	return(0);
}
 static int
#ifdef KR_headers
wrt_A(p,len) char *p; ftnlen len;
#else
wrt_A(char *p, ftnlen len)
#endif
{
	while(len-- > 0) (*f__putn)(*p++);
	return(0);
}
 static int
#ifdef KR_headers
wrt_AW(p,w,len) char * p; ftnlen len;
#else
wrt_AW(char * p, int w, ftnlen len)
#endif
{
	while(w>len)
	{	w--;
		(*f__putn)(' ');
	}
	while(w-- > 0)
		(*f__putn)(*p++);
	return(0);
}

 static int
#ifdef KR_headers
wrt_G(p,w,d,e,len) ufloat *p; ftnlen len;
#else
wrt_G(ufloat *p, int w, int d, int e, ftnlen len)
#endif
{	double up = 1,x;
	int i,oldscale=f__scale,n,j;
	x= len==sizeof(real)?p->pf:p->pd;
	if(x < 0 ) x = -x;
	if(x<.1) return(wrt_E(p,w,d,e,len));
	for(i=0;i<=d;i++,up*=10)
	{	if(x>=up) continue;
		f__scale=0;
		if(e==0) n=4;
		else	n=e+2;
		i=wrt_F(p,w-n,d-i,len);
		for(j=0;j<n;j++) (*f__putn)(' ');
		f__scale=oldscale;
		return(i);
	}
	return(wrt_E(p,w,d,e,len));
}
#ifdef KR_headers
w_ed(p,ptr,len) struct f__syl *p; char *ptr; ftnlen len;
#else
w_ed(struct f__syl *p, char *ptr, ftnlen len)
#endif
{
	if(f__cursor && mv_cur()) return(mv_cur());
	switch(p->op)
	{
	default:
		fprintf(stderr,"w_ed, unexpected code: %d\n", p->op);
		sig_die(f__fmtbuf, 1);
	case I:	return(wrt_I((Uint *)ptr,p->p1,len, 10));
	case IM:
		return(wrt_IM((Uint *)ptr,p->p1,p->p2,len,10));

		/* O and OM don't work right for character, double, complex, */
		/* or doublecomplex, and they differ from Fortran 90 in */
		/* showing a minus sign for negative values. */

	case O:	return(wrt_I((Uint *)ptr, p->p1, len, 8));
	case OM:
		return(wrt_IM((Uint *)ptr,p->p1,p->p2,len,8));
	case L:	return(wrt_L((Uint *)ptr,p->p1, len));
	case A: return(wrt_A(ptr,len));
	case AW:
		return(wrt_AW(ptr,p->p1,len));
	case D:
	case E:
	case EE:
		return(wrt_E((ufloat *)ptr,p->p1,p->p2,p->p3,len));
	case G:
	case GE:
		return(wrt_G((ufloat *)ptr,p->p1,p->p2,p->p3,len));
	case F:	return(wrt_F((ufloat *)ptr,p->p1,p->p2,len));

		/* Z and ZM assume 8-bit bytes. */

	case Z: return(wrt_Z((Uint *)ptr,p->p1,0,len));
	case ZM:
		return(wrt_Z((Uint *)ptr,p->p1,p->p2,len));
	}
}
#ifdef KR_headers
w_ned(p) struct f__syl *p;
#else
w_ned(struct f__syl *p)
#endif
{
	switch(p->op)
	{
	default: fprintf(stderr,"w_ned, unexpected code: %d\n", p->op);
		sig_die(f__fmtbuf, 1);
	case SLASH:
		return((*f__donewrec)());
	case T: f__cursor = p->p1-f__recpos - 1;
		return(1);
	case TL: f__cursor -= p->p1;
		if(f__cursor < -f__recpos)	/* TL1000, 1X */
			f__cursor = -f__recpos;
		return(1);
	case TR:
	case X:
		f__cursor += p->p1;
		return(1);
	case APOS:
		return(wrt_AP(*(char **)&p->p2));
	case H:
		return(wrt_H(p->p1,*(char **)&p->p2));
	}
}
./ ADD NAME=libI77/wsfe.c TIME=719848233
/*write sequential formatted external*/
#include "f2c.h"
#include "fio.h"
#include "fmt.h"
extern int f__hiwater;

#ifdef KR_headers
x_putc(c)
#else
x_putc(int c)
#endif
{
	/* this uses \n as an indicator of record-end */
	if(c == '\n' && f__recpos < f__hiwater) {	/* fseek calls fflush, a loss */
#ifndef NON_UNIX_STDIO
		if(f__cf->_ptr + f__hiwater - f__recpos < buf_end(f__cf))
			f__cf->_ptr += f__hiwater - f__recpos;
		else
#endif
			(void) fseek(f__cf, (long)(f__hiwater - f__recpos), SEEK_CUR);
	}
	f__recpos++;
	return putc(c,f__cf);
}
x_wSL(Void)
{
	(*f__putn)('\n');
	f__recpos=0;
	f__cursor = 0;
	f__hiwater = 0;
	return(1);
}
xw_end(Void)
{
	if(f__nonl == 0)
		(*f__putn)('\n');
	f__hiwater = f__recpos = f__cursor = 0;
	return(0);
}
xw_rev(Void)
{
	if(f__workdone) (*f__putn)('\n');
	f__hiwater = f__recpos = f__cursor = 0;
	return(f__workdone=0);
}

#ifdef KR_headers
integer s_wsfe(a) cilist *a;	/*start*/
#else
integer s_wsfe(cilist *a)	/*start*/
#endif
{	int n;
	if(!f__init) f_init();
	if(n=c_sfe(a)) return(n);
	f__reading=0;
	f__sequential=1;
	f__formatted=1;
	f__external=1;
	f__elist=a;
	f__hiwater = f__cursor=f__recpos=0;
	f__nonl = 0;
	f__scale=0;
	f__fmtbuf=a->cifmt;
	f__curunit = &f__units[a->ciunit];
	f__cf=f__curunit->ufd;
	if(pars_f(f__fmtbuf)<0) err(a->cierr,100,"startio");
	f__putn= x_putc;
	f__doed= w_ed;
	f__doned= w_ned;
	f__doend=xw_end;
	f__dorevert=xw_rev;
	f__donewrec=x_wSL;
	fmt_bg();
	f__cplus=0;
	f__cblank=f__curunit->ublnk;
	if(f__curunit->uwrt != 1 && f__nowwriting(f__curunit))
		err(a->cierr,errno,"write start");
	return(0);
}
./ ADD NAME=libI77/wsle.c TIME=719848233
#include "f2c.h"
#include "fio.h"
#include "fmt.h"
#include "lio.h"

#ifdef KR_headers
integer s_wsle(a) cilist *a;
#else
integer s_wsle(cilist *a)
#endif
{
	int n;
	if(!f__init) f_init();
	if(n=c_le(a)) return(n);
	f__reading=0;
	f__external=1;
	f__formatted=1;
	f__putn = t_putc;
	f__lioproc = l_write;
	L_len = LINE;
	f__donewrec = x_wSL;
	if(f__curunit->uwrt != 1 && f__nowwriting(f__curunit))
		err(a->cierr, errno, "list output start");
	return(0);
	}

integer e_wsle(Void)
{
	t_putc('\n');
	f__recpos=0;
	if (f__cf == stdout)
		fflush(stdout);
	else if (f__cf == stderr)
		fflush(stderr);
	return(0);
	}
./ ADD NAME=libI77/wsne.c TIME=719848234
#include "f2c.h"
#include "fio.h"
#include "lio.h"

 integer
#ifdef KR_headers
s_wsne(a) cilist *a;
#else
s_wsne(cilist *a)
#endif
{
	int n;
	extern integer e_wsle(Void);

	if(!f__init)
		f_init();
	if(n=c_le(a))
		return(n);
	f__reading=0;
	f__external=1;
	f__formatted=1;
	f__putn = t_putc;
	L_len = LINE;
	f__donewrec = x_wSL;
	if(f__curunit->uwrt != 1 && f__nowwriting(f__curunit))
		err(a->cierr, errno, "namelist output start");
	x_wsne(a);
	return e_wsle();
	}
./ ADD NAME=libI77/xwsne.c TIME=719848234
#include "f2c.h"
#include "fio.h"
#include "lio.h"
#include "fmt.h"

#ifdef KR_headers
x_wsne(a) cilist *a;
#else
#include "string.h"

 VOID
x_wsne(cilist *a)
#endif
{
	Namelist *nl;
	char *s;
	Vardesc *v, **vd, **vde;
	ftnint *number, type;
	ftnlen *dims;
	ftnlen size;
	static ftnint one = 1;
	extern ftnlen f__typesize[];

	nl = (Namelist *)a->cifmt;
	PUT('&');
	for(s = nl->name; *s; s++)
		PUT(*s);
	PUT(' ');
	vd = nl->vars;
	vde = vd + nl->nvars;
	while(vd < vde) {
		v = *vd++;
		s = v->name;
		if (f__recpos+strlen(s)+2 >= L_len)
			(*f__donewrec)();
		while(*s)
			PUT(*s++);
		PUT(' ');
		PUT('=');
		number = (dims = v->dims) ? dims + 1 : &one;
		type = v->type;
		if (type < 0) {
			size = -type;
			type = TYCHAR;
			}
		else
			size = f__typesize[type];
		l_write(number, v->addr, size, type);
		if (vd < vde) {
			if (f__recpos+2 >= L_len)
				(*f__donewrec)();
			PUT(',');
			PUT(' ');
			}
		else if (f__recpos+1 >= L_len)
			(*f__donewrec)();
		}
	PUT('/');
	}
./ ENDUP
