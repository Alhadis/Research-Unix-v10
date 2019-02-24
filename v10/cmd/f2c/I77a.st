./ ADD NAME=Version.c TIME=706280454
static char junk[] = "\n@(#) LIBI77 VERSION pjw,dmg-mods 19 May 1992\n";

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
./ ADD NAME=backspace.c TIME=708815578
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
	b= &units[a->aunit];
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
		if (nowreading(b))
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
./ ADD NAME=close.c TIME=708917753
#include "f2c.h"
#include "fio.h"
#ifdef KR_headers
integer f_clos(a) cllist *a;
#else
#undef abs
#include "stdlib.h"
#ifdef MSDOS
#include "io.h"
#else
#ifdef __cplusplus
extern "C" int unlink(char*);
#else
extern int unlink(char*);
#endif
#endif

integer f_clos(cllist *a)
#endif
{	unit *b;

	if(a->cunit >= MXUNIT) return(0);
	b= &units[a->cunit];
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
		if(units[i].ufd != NULL) (void) fflush(units[i].ufd);
}
./ ADD NAME=dfe.c TIME=708823203
#include "f2c.h"
#include "fio.h"
#include "fmt.h"

y_rsk(Void)
{
	if(curunit->uend || curunit->url <= recpos
		|| curunit->url == 1) return 0;
	do {
		getc(cf);
	} while(++recpos < curunit->url);
	return 0;
}
y_getc(Void)
{
	int ch;
	if(curunit->uend) return(-1);
	if((ch=getc(cf))!=EOF)
	{
		recpos++;
		if(curunit->url>=recpos ||
			curunit->url==1)
			return(ch);
		else	return(' ');
	}
	if(feof(cf))
	{
		curunit->uend=1;
		errno=0;
		return(-1);
	}
	err(elist->cierr,errno,"readingd");
}
#ifdef KR_headers
y_putc(c)
#else
y_putc(int c)
#endif
{
	recpos++;
	if(recpos <= curunit->url || curunit->url==1)
		putc(c,cf);
	else
		err(elist->cierr,110,"dout");
	return(0);
}
y_rev(Void)
{	/*what about work done?*/
	if(curunit->url==1 || recpos==curunit->url)
		return(0);
	while(recpos<curunit->url)
		(*putn)(' ');
	recpos=0;
	return(0);
}
y_err(Void)
{
	err(elist->cierr, 110, "dfe");
}

y_newrec(Void)
{
	if(curunit->url == 1 || recpos == curunit->url) {
		hiwater = recpos = cursor = 0;
		return(1);
	}
	if(hiwater > recpos)
		recpos = hiwater;
	y_rev();
	hiwater = cursor = 0;
	return(1);
}

#ifdef KR_headers
c_dfe(a) cilist *a;
#else
c_dfe(cilist *a)
#endif
{
	sequential=0;
	formatted=external=1;
	elist=a;
	cursor=scale=recpos=0;
	if(a->ciunit>MXUNIT || a->ciunit<0)
		err(a->cierr,101,"startchk");
	curunit = &units[a->ciunit];
	if(curunit->ufd==NULL && fk_open(DIR,FMT,a->ciunit))
		err(a->cierr,104,"dfe");
	cf=curunit->ufd;
	if(!curunit->ufmt) err(a->cierr,102,"dfe")
	if(!curunit->useek) err(a->cierr,104,"dfe")
	fmtbuf=a->cifmt;
	(void) fseek(cf,(long)curunit->url * (a->cirec-1),SEEK_SET);
	curunit->uend = 0;
	return(0);
}
#ifdef KR_headers
integer s_rdfe(a) cilist *a;
#else
integer s_rdfe(cilist *a)
#endif
{
	int n;
	if(!init) f_init();
	if(n=c_dfe(a))return(n);
	reading=1;
	if(curunit->uwrt && nowreading(curunit))
		err(a->cierr,errno,"read start");
	getn = y_getc;
	doed = rd_ed;
	doned = rd_ned;
	dorevert = donewrec = y_err;
	doend = y_rsk;
	if(pars_f(fmtbuf)<0)
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
	if(!init) f_init();
	if(n=c_dfe(a)) return(n);
	reading=0;
	if(curunit->uwrt != 1 && nowwriting(curunit))
		err(a->cierr,errno,"startwrt");
	putn = y_putc;
	doed = w_ed;
	doned= w_ned;
	dorevert = y_err;
	donewrec = y_newrec;
	doend = y_rev;
	if(pars_f(fmtbuf)<0)
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
./ ADD NAME=dolio.c TIME=708905794
#include "f2c.h"

#ifdef __cplusplus
extern "C" {
#endif
#ifdef KR_headers
extern int (*lioproc)();

integer do_lio(type,number,ptr,len) ftnint *number,*type; char *ptr; ftnlen len;
#else
extern int (*lioproc)(ftnint*, char*, ftnlen, ftnint);

integer do_lio(ftnint *type, ftnint *number, char *ptr, ftnlen len)
#endif
{
	return((*lioproc)(number,ptr,len,*type));
}
#ifdef __cplusplus
	}
#endif
./ ADD NAME=due.c TIME=708866794
#include "f2c.h"
#include "fio.h"

#ifdef KR_headers
c_due(a) cilist *a;
#else
c_due(cilist *a)
#endif
{
	if(!init) f_init();
	if(a->ciunit>=MXUNIT || a->ciunit<0)
		err(a->cierr,101,"startio");
	recpos=sequential=formatted=0;
	external=1;
	curunit = &units[a->ciunit];
	elist=a;
	if(curunit->ufd==NULL && fk_open(DIR,UNF,a->ciunit) ) err(a->cierr,104,"due");
	cf=curunit->ufd;
	if(curunit->ufmt) err(a->cierr,102,"cdue")
	if(!curunit->useek) err(a->cierr,104,"cdue")
	if(curunit->ufd==NULL) err(a->cierr,114,"cdue")
	(void) fseek(cf,(long)(a->cirec-1)*curunit->url,SEEK_SET);
	curunit->uend = 0;
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
	reading=1;
	if(curunit->uwrt && nowreading(curunit))
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
	reading=0;
	if(curunit->uwrt != 1 && nowwriting(curunit))
		err(a->cierr,errno,"write start");
	return(0);
}
integer e_rdue(Void)
{
	if(curunit->url==1 || recpos==curunit->url)
		return(0);
	(void) fseek(cf,(long)(curunit->url-recpos),SEEK_CUR);
	if(ftell(cf)%curunit->url)
		err(elist->cierr,200,"syserr");
	return(0);
}
integer e_wdue(Void)
{
	return(e_rdue());
}
./ ADD NAME=endfile.c TIME=708893804
#include "f2c.h"
#include "fio.h"
#include "fcntl.h"
#include "rawio.h"
#ifndef O_RDONLY
#define O_RDONLY 0
#endif

#ifdef KR_headers
extern char *strcpy();
#else
#undef abs
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
	b = &units[a->aunit];
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

 int
#ifdef KR_headers
t_runc(a) alist *a;
#else
t_runc(alist *a)
#endif
{
	char nm[16];
	long loc, len;
	unit *b;
	int rc = 0;

	b = &units[a->aunit];
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
	(void) strcpy(nm,"tmp.FXXXXXX");
	(void) mktemp(nm);
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
./ ADD NAME=err.c TIME=708894334
#include "sys/types.h"
#ifndef MSDOS
#include "sys/stat.h"
#endif
#include "f2c.h"
#include "fio.h"
#include "fcntl.h"
#include "rawio.h"
#ifdef NON_UNIX_STDIO
#ifdef KR_headers
extern char *malloc();
#else
#undef abs
#include "stdlib.h"
#endif
#endif
#ifndef O_WRONLY
#define O_WRONLY 1
#endif

/*global definitions*/
unit units[MXUNIT];	/*unit table*/
flag init;	/*0 on entry, 1 after initializations*/
cilist *elist;	/*active external io list*/
flag reading;	/*1 if reading, 0 if writing*/
flag cplus,cblank;
char *fmtbuf;
flag external;	/*1 if external io, 0 if internal */
#ifdef KR_headers
int (*doed)(),(*doned)();
int (*doend)(),(*donewrec)(),(*dorevert)();
int (*getn)(),(*putn)();	/*for formatted io*/
#else
int (*getn)(void),(*putn)(int);	/*for formatted io*/
int (*doed)(struct syl*, char*, ftnlen),(*doned)(struct syl*);
int (*dorevert)(void),(*donewrec)(void),(*doend)(void);
#endif
flag sequential;	/*1 if sequential io, 0 if direct*/
flag formatted;	/*1 if formatted io, 0 if unformatted*/
FILE *cf;	/*current file*/
unit *curunit;	/*current unit*/
int recpos;	/*place in current record*/
int cursor,scale;

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
canseek(f) FILE *f; /*SYSDEP*/
#else
canseek(FILE *f) /*SYSDEP*/
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
fatal(n,s) char *s;
#else
fatal(int n, char *s)
#endif
{
	if(n<100 && n>=0) perror(s); /*SYSDEP*/
	else if(n >= (int)MAXERR || n < -1)
	{	fprintf(stderr,"%s: illegal error number %d\n",s,n);
	}
	else if(n == -1) fprintf(stderr,"%s: end of file\n",s);
	else
		fprintf(stderr,"%s: %s\n",s,F_err[n-100]);
	if (curunit) {
		fprintf(stderr,"apparent state: unit %d ",curunit-units);
		fprintf(stderr, curunit->ufnm ? "named %s\n" : "(unnamed)\n",
			curunit->ufnm);
		}
	else
		fprintf(stderr,"apparent state: internal I/O\n");
	if (fmtbuf)
		fprintf(stderr,"last format: %s\n",fmtbuf);
	fprintf(stderr,"lately %s %s %s %s",reading?"reading":"writing",
		sequential?"sequential":"direct",formatted?"formatted":"unformatted",
		external?"external":"internal");
	sig_die(" IO", 1);
}
/*initialization routine*/
 VOID
f_init(Void)
{	unit *p;

	init=1;
	p= &units[0];
	p->ufd=stderr;
	p->useek=canseek(stderr);
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
	p = &units[5];
	p->ufd=stdin;
	p->useek=canseek(stdin);
	p->ufmt=1;
	p->uwrt=0;
	p= &units[6];
	p->ufd=stdout;
	p->useek=canseek(stdout);
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
nowreading(x) unit *x;
#else
nowreading(unit *x)
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
nowwriting(x) unit *x;
#else
nowwriting(unit *x)
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
		|| (cf = x->ufd = fdopen(k,w_mode[x->ufmt])) == NULL) {
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
./ ADD NAME=f2c.h TIME=708964532
/* f2c.h  --  Standard Fortran to C header file */

/**  barf  [ba:rf]  2.  "He suggested using FORTRAN, and everybody barfed."

	- From The Shogakukan DICTIONARY OF NEW ENGLISH (Second edition) */

#ifndef F2C_INCLUDE
#define F2C_INCLUDE

typedef long int integer;
typedef char *address;
typedef short int shortint;
typedef float real;
typedef double doublereal;
typedef struct { real r, i; } complex;
typedef struct { doublereal r, i; } doublecomplex;
typedef long int logical;
typedef short int shortlogical;

#define TRUE_ (1)
#define FALSE_ (0)

/* Extern is for use with -E */
#ifndef Extern
#define Extern extern
#endif

/* I/O stuff */

#ifdef f2c_i2
/* for -i2 */
typedef short flag;
typedef short ftnlen;
typedef short ftnint;
#else
typedef long flag;
typedef long ftnlen;
typedef long ftnint;
#endif

/*external read, write*/
typedef struct
{	flag cierr;
	ftnint ciunit;
	flag ciend;
	char *cifmt;
	ftnint cirec;
} cilist;

/*internal read, write*/
typedef struct
{	flag icierr;
	char *iciunit;
	flag iciend;
	char *icifmt;
	ftnint icirlen;
	ftnint icirnum;
} icilist;

/*open*/
typedef struct
{	flag oerr;
	ftnint ounit;
	char *ofnm;
	ftnlen ofnmlen;
	char *osta;
	char *oacc;
	char *ofm;
	ftnint orl;
	char *oblnk;
} olist;

/*close*/
typedef struct
{	flag cerr;
	ftnint cunit;
	char *csta;
} cllist;

/*rewind, backspace, endfile*/
typedef struct
{	flag aerr;
	ftnint aunit;
} alist;

/* inquire */
typedef struct
{	flag inerr;
	ftnint inunit;
	char *infile;
	ftnlen infilen;
	ftnint	*inex;	/*parameters in standard's order*/
	ftnint	*inopen;
	ftnint	*innum;
	ftnint	*innamed;
	char	*inname;
	ftnlen	innamlen;
	char	*inacc;
	ftnlen	inacclen;
	char	*inseq;
	ftnlen	inseqlen;
	char 	*indir;
	ftnlen	indirlen;
	char	*infmt;
	ftnlen	infmtlen;
	char	*inform;
	ftnint	informlen;
	char	*inunf;
	ftnlen	inunflen;
	ftnint	*inrecl;
	ftnint	*innrec;
	char	*inblank;
	ftnlen	inblanklen;
} inlist;

#define VOID void

union Multitype {	/* for multiple entry points */
	shortint h;
	integer i;
	real r;
	doublereal d;
	complex c;
	doublecomplex z;
	};

typedef union Multitype Multitype;

typedef long Long;	/* No longer used; formerly in Namelist */

struct Vardesc {	/* for Namelist */
	char *name;
	char *addr;
	ftnlen *dims;
	int  type;
	};
typedef struct Vardesc Vardesc;

struct Namelist {
	char *name;
	Vardesc **vars;
	int nvars;
	};
typedef struct Namelist Namelist;

#define abs(x) ((x) >= 0 ? (x) : -(x))
#define dabs(x) (doublereal)abs(x)
#define min(a,b) ((a) <= (b) ? (a) : (b))
#define max(a,b) ((a) >= (b) ? (a) : (b))
#define dmin(a,b) (doublereal)min(a,b)
#define dmax(a,b) (doublereal)max(a,b)

/* procedure parameter types for -A and -C++ */

#define F2C_proc_par_types 1
#ifdef __cplusplus
typedef int /* Unknown procedure type */ (*U_fp)(...);
typedef shortint (*J_fp)(...);
typedef integer (*I_fp)(...);
typedef real (*R_fp)(...);
typedef doublereal (*D_fp)(...), (*E_fp)(...);
typedef /* Complex */ VOID (*C_fp)(...);
typedef /* Double Complex */ VOID (*Z_fp)(...);
typedef logical (*L_fp)(...);
typedef shortlogical (*K_fp)(...);
typedef /* Character */ VOID (*H_fp)(...);
typedef /* Subroutine */ int (*S_fp)(...);
#else
#ifndef __LCC__
typedef int /* Unknown procedure type */ (*U_fp)();
typedef shortint (*J_fp)();
typedef integer (*I_fp)();
typedef real (*R_fp)();
typedef doublereal (*D_fp)(), (*E_fp)();
typedef /* Complex */ VOID (*C_fp)();
typedef /* Double Complex */ VOID (*Z_fp)();
typedef logical (*L_fp)();
typedef shortlogical (*K_fp)();
typedef /* Character */ VOID (*H_fp)();
typedef /* Subroutine */ int (*S_fp)();
#endif
#endif
/* E_fp is for real functions when -R is not specified */
typedef VOID C_f;	/* complex function */
typedef VOID H_f;	/* character function */
typedef VOID Z_f;	/* double complex function */
typedef doublereal E_f;	/* real function with -R not specified */

/* undef any lower-case symbols that your C compiler predefines, e.g.: */

#ifndef Skip_f2c_Undefs
#undef mips
#undef sgi
#undef unix
#endif
#endif

#ifdef __cplusplus
extern "C" {
extern void abort_(void);
extern double c_abs(complex *);
extern void c_cos(complex *, complex *);
extern void c_div(complex *, complex *, complex *);
extern void c_exp(complex *, complex *);
extern void c_log(complex *, complex *);
extern void c_sin(complex *, complex *);
extern void c_sqrt(complex *, complex *);
extern double d_abs(double *);
extern double d_acos(double *);
extern double d_asin(double *);
extern double d_atan(double *);
extern double d_atn2(double *, double *);
extern void d_cnjg(doublecomplex *, doublecomplex *);
extern double d_cos(double *);
extern double d_cosh(double *);
extern double d_dim(double *, double *);
extern double d_exp(double *);
extern double d_imag(doublecomplex *);
extern double d_int(double *);
extern double d_lg10(double *);
extern double d_log(double *);
extern double d_mod(double *, double *);
extern double d_nint(double *);
extern double d_prod(float *, float *);
extern double d_sign(double *, double *);
extern double d_sin(double *);
extern double d_sinh(double *);
extern double d_sqrt(double *);
extern double d_tan(double *);
extern double d_tanh(double *);
extern double derf_(double *);
extern double derfc_(double *);
extern void ef1asc_(long int *, long int *, long int *, long int *);
extern long int ef1cmc_(long int *, long int *, long int *, long int *);
extern double erf(double);
extern double erf_(float *);
extern double erfc(double);
extern double erfc_(float *);
extern void getarg_(long int *, char *, long int);
extern void getenv_(char *, char *, long int, long int);
extern int getpid(void);
extern short h_abs(short *);
extern short h_dim(short *, short *);
extern short h_dnnt(double *);
extern short h_indx(char *, char *, long int, long int);
extern short h_len(char *, long int);
extern short h_mod(short *, short *);
extern short h_nint(float *);
extern short h_sign(short *, short *);
extern short hl_ge(char *, char *, long int, long int);
extern short hl_gt(char *, char *, long int, long int);
extern short hl_le(char *, char *, long int, long int);
extern short hl_lt(char *, char *, long int, long int);
extern long int i_abs(long int *);
extern long int i_dim(long int *, long int *);
extern long int i_dnnt(double *);
extern long int i_indx(char *, char *, long int, long int);
extern long int i_len(char *, long int);
extern long int i_mod(long int *, long int *);
extern long int i_nint(float *);
extern long int i_sign(long int *, long int *);
extern long int iargc_(void);
extern long int l_ge(char *, char *, long int, long int);
extern long int l_gt(char *, char *, long int, long int);
extern long int l_le(char *, char *, long int, long int);
extern long int l_lt(char *, char *, long int, long int);
extern int main(int, char **);
extern int pause(void);
extern void pow_ci(complex *, complex *, long int *);
extern double pow_dd(double *, double *);
extern double pow_di(double *, long int *);
extern short pow_hh(short *, short *);
extern long int pow_ii(long int *, long int *);
extern double pow_ri(float *, long int *);
extern void pow_zi(doublecomplex *, doublecomplex *, long int *);
extern void pow_zz(doublecomplex *, doublecomplex *, doublecomplex *);
extern double r_abs(float *);
extern double r_acos(float *);
extern double r_asin(float *);
extern double r_atan(float *);
extern double r_atn2(float *, float *);
extern void r_cnjg(complex *, complex *);
extern double r_cos(float *);
extern double r_cosh(float *);
extern double r_dim(float *, float *);
extern double r_exp(float *);
extern double r_imag(complex *);
extern double r_int(float *);
extern double r_lg10(float *);
extern double r_log(float *);
extern double r_mod(float *, float *);
extern double r_nint(float *);
extern double r_sign(float *, float *);
extern double r_sin(float *);
extern double r_sinh(float *);
extern double r_sqrt(float *);
extern double r_tan(float *);
extern double r_tanh(float *);
extern void s_cat(char *, char **, long int *, long int *, long int);
extern long int s_cmp(char *, char *, long int, long int);
extern void s_copy(char *, char *, long int, long int);
extern void s_paus(char *, long int);
extern void s_rnge(char *, long int, char *, long int);
extern void s_stop(char *, long int);
extern void sig_die(char *, int);
extern long int signal_(long int *, void *);
extern int system_(char *, long int);
extern double z_abs(doublecomplex *);
extern void z_cos(doublecomplex *, doublecomplex *);
extern void z_div(doublecomplex *, doublecomplex *, doublecomplex *);
extern void z_exp(doublecomplex *, doublecomplex *);
extern void z_log(doublecomplex *, doublecomplex *);
extern void z_sin(doublecomplex *, doublecomplex *);
extern void z_sqrt(doublecomplex *, doublecomplex *);
	}
#endif
./ ADD NAME=fio.h TIME=708907413
#include "stdio.h"
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

extern int errno;
extern flag init;
extern cilist *elist;	/*active external io list*/
extern flag reading,external,sequential,formatted;
#ifdef KR_headers
#define Void /*void*/
extern int (*getn)(),(*putn)();	/*for formatted io*/
extern long inode();
extern VOID sig_die();
extern int (*donewrec)(), t_putc(), x_wSL();
extern int c_sfe();
#else
#define Void void
#ifdef __cplusplus
extern "C" {
#endif
extern int (*getn)(void),(*putn)(int);	/*for formatted io*/
extern long inode(char*,int*);
extern void sig_die(char*,int);
extern void fatal(int,char*);
extern int t_runc(alist*);
extern int nowreading(unit*), nowwriting(unit*);
extern int fk_open(int,int,ftnint);
extern int en_fio(void);
extern void f_init(void);
extern int (*donewrec)(void), t_putc(int), x_wSL(void);
extern void b_char(char*,char*,ftnlen), g_char(char*,ftnlen,char*);
extern int c_sfe(cilist*), z_rnew(void);
extern int isatty(int);
#ifdef __cplusplus
	}
#endif
#endif
extern FILE *cf;	/*current file*/
extern unit *curunit;	/*current unit*/
extern unit units[];
#define err(f,m,s) {if(f) errno= m; else fatal(m,s); return(m);}

/*Table sizes*/
#define MXUNIT 100

extern int recpos;	/*position in current record*/
extern int cursor;	/* offset to move to */
extern int hiwater;	/* so TL doesn't confuse us */

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
extern int canseek(struct _iobuf *);
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
extern char *icvt(long int, int *, int *, int);
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
extern int l_write(long int *, char *, long int, long int);
extern char *mktemp(char *);
extern int isdev(char *);
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
extern long int inode(char *, int *);
extern void mvgbt(int, int, char *, char *);
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
./ ADD NAME=fmt.c TIME=708822446
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
extern int cursor,scale;
extern flag cblank,cplus;	/*blanks in I and compulsory plus*/
struct syl syl[SYLMX];
int parenlvl,pc,revloc;

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
	if(elist->cierr) {
		errno = 100;
		return(NULL);
	}
	fatal(100, "bad string");
	/*NOTREACHED*/ return 0;
}
#ifdef KR_headers
op_gen(a,b,c,d)
#else
op_gen(int a, int b, int c, int d)
#endif
{	struct syl *p= &syl[pc];
	if(pc>=SYLMX)
	{	fprintf(stderr,"format too complicated:\n");
		sig_die(fmtbuf, 1);
	}
	p->op=a;
	p->p1=b;
	p->p2=c;
	p->p3=d;
	return(pc++);
}
#ifdef KR_headers
char *f_list();
char *gt_num(s,n) char *s; int *n;
#else
char *f_list(char*);
char *gt_num(char *s, int *n)
#endif
{	int m=0,cnt=0;
	char c;
	for(c= *s;;c = *s)
	{	if(c==' ')
		{	s++;
			continue;
		}
		if(c>'9' || c<'0') break;
		m=10*m+c-'0';
		cnt++;
		s++;
	}
	if(cnt==0) *n=1;
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
	if(parenlvl++ ==1) revloc=curloc;
	if(op_gen(RET,curloc,0,0)<0 ||
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
	struct syl *sp;
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
			sp = &syl[op_gen(H,n,0,0)];
			*(char **)&sp->p2 = s + 1;
			s+=n;
			break;
		}
		break;
	case GLITCH:
	case '"':
	case '\'':
		sp = &syl[op_gen(APOS,0,0,0)];
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
{	int n,w,d,e,found=0,x=0;
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
		found = 1;
		s = gt_num(s, &w);
		if(w==0) break;
		(void) op_gen(O, w, 0, 0);
		break;
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
		found=1;
		s=gt_num(s,&w);
		if(w==0) break;
		if(*s!='.')
		{	(void) op_gen(I,w,0,0);
			break;
		}
		s++;
		s=gt_num(s,&d);
		(void) op_gen(IM,w,d,0);
		break;
	}
	if(found==0)
	{	pc--; /*unSTACK*/
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
		{	if(--parenlvl==0)
			{
				(void) op_gen(REVERT,revloc,0,0);
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
	parenlvl=revloc=pc=0;
	if(f_s(s,0) == NULL)
	{
		return(-1);
	}
	return(0);
}
#define STKSZ 10
int cnt[STKSZ],ret[STKSZ],cp,rp;
flag workdone, nonl;

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
	case RET:
		return(RET);
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
	case O:
	case L:
	case E: case EE: case D:
	case G: case GE:
		return(ED);
	}
}
#ifdef KR_headers
integer do_fio(number,ptr,len) ftnint *number; ftnlen len; char *ptr;
#else
integer do_fio(ftnint *number, char *ptr, ftnlen len)
#endif
{	struct syl *p;
	int n,i;
	for(i=0;i<*number;i++,ptr+=len)
	{
loop:	switch(type_f((p= &syl[pc])->op))
	{
	default:
		fprintf(stderr,"unknown code in do_fio: %d\n%s\n",
			p->op,fmtbuf);
		err(elist->cierr,100,"do_fio");
	case NED:
		if((*doned)(p))
		{	pc++;
			goto loop;
		}
		pc++;
		continue;
	case ED:
		if(cnt[cp]<=0)
		{	cp--;
			pc++;
			goto loop;
		}
		if(ptr==NULL)
			return((*doend)());
		cnt[cp]--;
		workdone=1;
		if((n=(*doed)(p,ptr,len))>0) err(elist->cierr,errno,"fmt");
		if(n<0) err(elist->ciend,(EOF),"fmt");
		continue;
	case STACK:
		cnt[++cp]=p->p1;
		pc++;
		goto loop;
	case RET:
		ret[++rp]=p->p1;
		pc++;
		goto loop;
	case GOTO:
		if(--cnt[cp]<=0)
		{	cp--;
			rp--;
			pc++;
			goto loop;
		}
		pc=1+ret[rp--];
		goto loop;
	case REVERT:
		rp=cp=0;
		pc = p->p1;
		if(ptr==NULL)
			return((*doend)());
		if(!workdone) return(0);
		if((n=(*dorevert)()) != 0) return(n);
		goto loop;
	case COLON:
		if(ptr==NULL)
			return((*doend)());
		pc++;
		goto loop;
	case NONL:
		nonl = 1;
		pc++;
		goto loop;
	case S:
	case SS:
		cplus=0;
		pc++;
		goto loop;
	case SP:
		cplus = 1;
		pc++;
		goto loop;
	case P:	scale=p->p1;
		pc++;
		goto loop;
	case BN:
		cblank=0;
		pc++;
		goto loop;
	case BZ:
		cblank=1;
		pc++;
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
	workdone=cp=rp=pc=cursor=0;
	cnt[0]=ret[0]=0;
}
./ ADD NAME=fmt.h TIME=708906700
struct syl
{	int op,p1,p2,p3;
};
#define RET 1
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
extern struct syl syl[];
extern int pc,parenlvl,revloc;
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
extern int (*doed)(),(*doned)();
extern int (*dorevert)(), (*doend)();
extern int rd_ed(),rd_ned();
extern int w_ed(),w_ned();
#else
#ifdef __cplusplus
extern "C" {
#endif
extern int (*doed)(struct syl*, char*, ftnlen),(*doned)(struct syl*);
extern int (*dorevert)(void), (*doend)(void);
extern void fmt_bg(void);
extern int pars_f(char*);
extern int rd_ed(struct syl*, char*, ftnlen),rd_ned(struct syl*);
extern int w_ed(struct syl*, char*, ftnlen),w_ned(struct syl*);
extern int wrt_E(ufloat*, int, int, int, ftnlen);
extern int wrt_F(ufloat*, int, int, ftnlen);
extern int wrt_L(Uint*, int, ftnlen);
#ifdef __cplusplus
	}
#endif
#endif
extern flag cblank,cplus,workdone, nonl;
extern char *fmtbuf;
extern int scale;
#define GET(x) if((x=(*getn)())<0) return(x)
#define VAL(x) (x!='\n'?x:' ')
#define PUT(x) (*putn)(x)
extern int cursor;
./ ADD NAME=fmtlib.c TIME=708905846
/*	@(#)fmtlib.c	1.2	*/
#define MAXINTLENGTH 23
#ifdef __cplusplus
extern "C" {
#endif
#ifdef KR_headers
char *icvt(value,ndigit,sign, base) long value; int *ndigit,*sign;
 register int base;
#else
char *icvt(long value, int *ndigit, int *sign, int base)
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
./ ADD NAME=fp.h TIME=580998005
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
./ ADD NAME=iio.c TIME=708823315
#include "f2c.h"
#include "fio.h"
#include "fmt.h"
extern char *icptr;
char *icend;
extern icilist *svic;
int icnum;
extern int hiwater;
z_getc(Void)
{
	if(recpos++ < svic->icirlen) {
		if(icptr >= icend) err(svic->iciend,(EOF),"endfile");
		return(*icptr++);
		}
	err(svic->icierr,110,"recend");
}
#ifdef KR_headers
z_putc(c)
#else
z_putc(int c)
#endif
{
	if(icptr >= icend) err(svic->icierr,110,"inwrite");
	if(recpos++ < svic->icirlen)
		*icptr++ = c;
	else	err(svic->icierr,110,"recend");
	return 0;
}
z_rnew(Void)
{
	icptr = svic->iciunit + (++icnum)*svic->icirlen;
	recpos = 0;
	cursor = 0;
	hiwater = 0;
	return 1;
}

 static int
z_endp(Void)
{
	(*donewrec)();
	return 0;
	}

#ifdef KR_headers
c_si(a) icilist *a;
#else
c_si(icilist *a)
#endif
{
	elist = (cilist *)a;
	fmtbuf=a->icifmt;
	if(pars_f(fmtbuf)<0)
		err(a->icierr,100,"startint");
	fmt_bg();
	sequential=formatted=1;
	external=0;
	cblank=cplus=scale=0;
	svic=a;
	icnum=recpos=0;
	cursor = 0;
	hiwater = 0;
	icptr = a->iciunit;
	icend = icptr + a->icirlen*a->icirnum;
	curunit = 0;
	cf = 0;
	return(0);
}
y_ierr(Void)
{
	err(elist->cierr, 110, "iio");
}
#ifdef KR_headers
integer s_rsfi(a) icilist *a;
#else
integer s_rsfi(icilist *a)
#endif
{	int n;
	if(n=c_si(a)) return(n);
	reading=1;
	doed=rd_ed;
	doned=rd_ned;
	getn=z_getc;
	dorevert = y_ierr;
	donewrec = z_rnew;
	doend = z_endp;
	return(0);
}

z_wnew(Void)
{
	while(recpos++ < svic->icirlen)
		*icptr++ = ' ';
	recpos = 0;
	cursor = 0;
	hiwater = 0;
	icnum++;
	return 1;
}
#ifdef KR_headers
integer s_wsfi(a) icilist *a;
#else
integer s_wsfi(icilist *a)
#endif
{	int n;
	if(n=c_si(a)) return(n);
	reading=0;
	doed=w_ed;
	doned=w_ned;
	putn=z_putc;
	dorevert = y_ierr;
	donewrec = z_wnew;
	doend = z_endp;
	return(0);
}
integer e_rsfi(Void)
{	int n;
	n = en_fio();
	fmtbuf = NULL;
	return(n);
}
integer e_wsfi(Void)
{
	int n;
	n = en_fio();
	fmtbuf = NULL;
	if(icnum >= svic->icirnum)
		return(n);
	while(recpos++ < svic->icirlen)
		*icptr++ = ' ';
	return(n);
}
./ ADD NAME=ilnw.c TIME=708907683
#include "f2c.h"
#include "fio.h"
#include "lio.h"
extern char *icptr;
extern char *icend;
extern icilist *svic;
extern int icnum;
#ifdef KR_headers
extern int z_putc();
#else
extern int z_putc(int);
#endif

 static int
z_wSL(Void)
{
	while(recpos < svic->icirlen)
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
	reading = 0;
	external = 0;
	formatted = 1;
	putn = z_putc;
	L_len = a->icirlen;
	donewrec = z_wSL;
	svic = a;
	icnum = recpos = 0;
	cursor = 0;
	cf = 0;
	curunit = 0;
	icptr = a->iciunit;
	icend = icptr + a->icirlen*a->icirnum;
	elist = (cilist *)a;
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
	lioproc = l_write;
	c_liw(a);
	return(0);
	}

integer e_wsli(Void)
{
	z_wSL();
	return(0);
	}
./ ADD NAME=inquire.c TIME=708896213
#include "f2c.h"
#include "fio.h"
#ifdef KR_headers
integer f_inqu(a) inlist *a;
#else
#ifdef MSDOS
#undef abs
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
			if(units[i].ufd != NULL
			 && units[i].ufnm != NULL
			 && !strcmp(units[i].ufnm,buf)) {
				p = &units[i];
				break;
				}
#else
		x=inode(buf, &n);
		for(i=0,p=NULL;i<MXUNIT;i++)
			if(units[i].uinode==x
			&& units[i].ufd!=NULL
			&& units[i].udev == n) {
				p = &units[i];
				break;
				}
#endif
	}
	else
	{
		byfile=0;
		if(a->inunit<MXUNIT && a->inunit>=0)
		{
			p= &units[a->inunit];
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
	if(a->innum!=NULL) *a->innum= p-units;
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
./ ADD NAME=lio.h TIME=708907526
/*	copy of ftypes from the compiler */
/* variable types
 * numeric assumptions:
 *	int < reals < complexes
 *	TYDREAL-TYREAL = TYDCOMPLEX-TYCOMPLEX
 */

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
#define TYERROR 11

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
{	short	flshort;
	ftnint	flint;
	real	flreal;
	doublereal	fldouble;
} flex;
extern int scale;
#ifdef KR_headers
extern int (*lioproc)(), (*l_getc)(), (*l_ungetc)();
extern int l_read();
#else
#ifdef __cplusplus
extern "C" {
#endif
extern int (*lioproc)(ftnint*, char*, ftnlen, ftnint);
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
./ ADD NAME=local.h TIME=705213589
./ ADD NAME=lread.c TIME=708826270
#include "f2c.h"
#include "fio.h"
#include "fmt.h"
#include "lio.h"
#include "ctype.h"
#include "fp.h"

extern char *fmtbuf;
#ifdef KR_headers
extern double atof();
extern char *malloc(), *realloc();
int (*lioproc)(), (*l_getc)(), (*l_ungetc)();
#else
#undef abs
#include "stdlib.h"
int (*lioproc)(ftnint*, char*, ftnlen, ftnint), (*l_getc)(void),
	(*l_ungetc)(int,FILE*);
#endif
int l_eof;

#define isblnk(x) (ltab[x+1]&B)
#define issep(x) (ltab[x+1]&SX)
#define isapos(x) (ltab[x+1]&AX)
#define isexp(x) (ltab[x+1]&EX)
#define issign(x) (ltab[x+1]&SG)
#define iswhit(x) (ltab[x+1]&WH)
#define SX 1
#define B 2
#define AX 4
#define EX 8
#define SG 16
#define WH 32
char ltab[128+1] = {	/* offset one for EOF */
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
un_getc(x,cf) int x; FILE *cf;
#else
un_getc(int x, FILE *cf)
#endif
{ return ungetc(x,cf); }
#else
#define un_getc ungetc
#ifdef KR_headers
 extern int ungetc();
#endif
#endif

t_getc(Void)
{	int ch;
	if(curunit->uend) return(EOF);
	if((ch=getc(cf))!=EOF) return(ch);
	if(feof(cf))
		l_eof = curunit->uend = 1;
	return(EOF);
}
integer e_rsle(Void)
{
	int ch;
	if(curunit->uend) return(0);
	while((ch=t_getc())!='\n' && ch!=EOF);
	return(0);
}

flag lquit;
int lcount,ltype,nml_read;
char *lchar;
double lx,ly;
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
		if (lcount > 0)
			return(0);
		lcount = 1;
		}
	ltype = 0;
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
			err(elist->cierr,112,"bad repetition count")
			}
		poststar = havestar = 1;
		*sp = 0;
		lcount = atoi(s);
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
			err(elist->cierr,112,"exponent field")
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
	(void) Ungetc(ch, cf);
	if (sp > sp1) {
		++havenum;
		while(*--sp == '0')
			++exp;
		if (exp)
			sprintf(sp+1, "e%ld", exp);
		else
			sp[1] = 0;
		lx = atof(s);
		}
	else
		lx = 0.;
	if (havenum)
		ltype = TYLONG;
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
					lquit = 2;
					return 0;
					}
				err(elist->cierr,112,"invalid number")
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
	lcount = ch - '0';
	while(GETC(ch) >= '0' && ch <= '9')
		lcount = 10*lcount + ch - '0';
	Ungetc(ch,cf);
	return lcount <= 0;
	}

l_C(Void)
{	int ch, nml_save;
	double lz;
	if(lcount>0) return(0);
	ltype=0;
	GETC(ch);
	if(ch!='(')
	{
		if (nml_read > 1 && (ch < '0' || ch > '9')) {
			Ungetc(ch,cf);
			lquit = 2;
			return 0;
			}
		if (rd_count(ch))
			if(!cf || !feof(cf))
				err(elist->cierr,112,"complex format")
			else
				err(elist->cierr,(EOF),"lread");
		if(GETC(ch)!='*')
		{
			if(!cf || !feof(cf))
				err(elist->cierr,112,"no star")
			else
				err(elist->cierr,(EOF),"lread");
		}
		if(GETC(ch)!='(')
		{	Ungetc(ch,cf);
			return(0);
		}
	}
	else
		lcount = 1;
	while(iswhit(GETC(ch)));
	Ungetc(ch,cf);
	nml_save = nml_read;
	nml_read = 0;
	if (ch = l_R(1))
		return ch;
	if (!ltype)
		err(elist->cierr,112,"no real part");
	lz = lx;
	while(iswhit(GETC(ch)));
	if(ch!=',')
	{	(void) Ungetc(ch,cf);
		err(elist->cierr,112,"no comma");
	}
	while(iswhit(GETC(ch)));
	(void) Ungetc(ch,cf);
	if (ch = l_R(1))
		return ch;
	if (!ltype)
		err(elist->cierr,112,"no imaginary part");
	while(iswhit(GETC(ch)));
	if(ch!=')') err(elist->cierr,112,"no )");
	ly = lx;
	lx = lz;
	nml_read = nml_save;
	return(0);
}
l_L(Void)
{
	int ch;
	if(lcount>0) return(0);
	ltype=0;
	GETC(ch);
	if(isdigit(ch))
	{
		rd_count(ch);
		if(GETC(ch)!='*')
			if(!cf || !feof(cf))
				err(elist->cierr,112,"no star")
			else
				err(elist->cierr,(EOF),"lread");
		GETC(ch);
	}
	if(ch == '.') GETC(ch);
	switch(ch)
	{
	case 't':
	case 'T':
		lx=1;
		break;
	case 'f':
	case 'F':
		lx=0;
		break;
	default:
		if(isblnk(ch) || issep(ch) || ch==EOF)
		{	(void) Ungetc(ch,cf);
			return(0);
		}
		else	err(elist->cierr,112,"logical");
	}
	ltype=TYLONG;
	lcount = 1;
	while(!issep(GETC(ch)) && ch!=EOF);
	(void) Ungetc(ch, cf);
	return(0);
}
#define BUFSIZE	128
l_CHAR(Void)
{	int ch,size,i;
	char quote,*p;
	if(lcount>0) return(0);
	ltype=0;
	if(lchar!=NULL) free(lchar);
	size=BUFSIZE;
	p=lchar=malloc((unsigned int)size);
	if(lchar==NULL) err(elist->cierr,113,"no space");

	GETC(ch);
	if(isdigit(ch)) {
		/* allow Fortran 8x-style unquoted string...	*/
		/* either find a repetition count or the string	*/
		lcount = ch - '0';
		*p++ = ch;
		for(i = 1;;) {
			switch(GETC(ch)) {
				case '*':
					if (lcount == 0) {
						lcount = 1;
						goto noquote;
						}
					p = lchar;
					goto have_lcount;
				case ',':
				case ' ':
				case '\t':
				case '\n':
				case '/':
					Ungetc(ch,cf);
					/* no break */
				case EOF:
					lcount = 1;
					ltype = TYCHAR;
					return *p = 0;
				}
			if (!isdigit(ch)) {
				lcount = 1;
				goto noquote;
				}
			*p++ = ch;
			lcount = 10*lcount + ch - '0';
			if (++i == size) {
				lchar = realloc(lchar,
					(unsigned int)(size += BUFSIZE));
				p = lchar + i;
				}
			}
		}
	else	(void) Ungetc(ch,cf);
 have_lcount:
	if(GETC(ch)=='\'' || ch=='"') quote=ch;
	else if(isblnk(ch) || (issep(ch) && ch != '\n') || ch==EOF)
	{	(void) Ungetc(ch,cf);
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
					Ungetc(ch,cf);
					/* no break */
				case EOF:
					ltype = TYCHAR;
					return *p = 0;
				}
 noquote:
			*p++ = ch;
			if (++i == size) {
				lchar = realloc(lchar,
					(unsigned int)(size += BUFSIZE));
				p = lchar + i;
				}
			}
		}
	ltype=TYCHAR;
	for(i=0;;)
	{	while(GETC(ch)!=quote && ch!='\n'
			&& ch!=EOF && ++i<size) *p++ = ch;
		if(i==size)
		{
		newone:
			lchar= realloc(lchar, (unsigned int)(size += BUFSIZE));
			p=lchar+i-1;
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
		{	(void) Ungetc(ch,cf);
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
	fmtbuf="list io";
	if(a->ciunit>=MXUNIT || a->ciunit<0)
		err(a->cierr,101,"stler");
	scale=recpos=0;
	elist=a;
	curunit = &units[a->ciunit];
	if(curunit->ufd==NULL && fk_open(SEQ,FMT,a->ciunit))
		err(a->cierr,102,"lio");
	cf=curunit->ufd;
	if(!curunit->ufmt) err(a->cierr,103,"lio")
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
		if(lquit) return(0);
		if(l_eof)
			err(elist->ciend, EOF, "list in")
		if(lcount == 0) {
			ltype = 0;
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
					lquit = 1;
					goto loopend;
				case ',':
					lcount = 1;
					goto loopend;
				default:
					(void) Ungetc(ch, cf);
					goto rddata;
				}
			}
		}
	rddata:
		switch((int)type)
		{
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
		case TYLOGICAL:
			ERR(l_L());
			break;
		case TYCHAR:
			ERR(l_CHAR());
			break;
		}
	while (GETC(ch) == ' ' || ch == '\t');
	if (ch != ',' || lcount > 1)
		Ungetc(ch,cf);
	loopend:
		if(lquit) return(0);
		if(cf) {
			if (feof(cf))
				err(elist->ciend,(EOF),"list in")
			else if(ferror(cf)) {
				clearerr(cf);
				err(elist->cierr,errno,"list in")
				}
			}
		if(ltype==0) goto bump;
		switch((int)type)
		{
		case TYSHORT:
			Ptr->flshort=lx;
			break;
		case TYLOGICAL:
		case TYLONG:
			Ptr->flint=lx;
			break;
		case TYREAL:
			Ptr->flreal=lx;
			break;
		case TYDREAL:
			Ptr->fldouble=lx;
			break;
		case TYCOMPLEX:
			xx=(real *)ptr;
			*xx++ = lx;
			*xx = ly;
			break;
		case TYDCOMPLEX:
			yy=(doublereal *)ptr;
			*yy++ = lx;
			*yy = ly;
			break;
		case TYCHAR:
			b_char(lchar,ptr,len);
			break;
		}
	bump:
		if(lcount>0) lcount--;
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

	if(!init) f_init();
	if(n=c_le(a)) return(n);
	reading=1;
	external=1;
	formatted=1;
	lioproc = l_read;
	lquit = 0;
	lcount = 0;
	l_eof = 0;
	if(curunit->uwrt && nowreading(curunit))
		err(a->cierr,errno,"read start");
	l_getc = t_getc;
	l_ungetc = un_getc;
	return(0);
}
./ ADD NAME=lwrite.c TIME=708868720
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
	recpos++;
	putc(c,cf);
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
	if(recpos+strlen(buf)>=L_len)
#else
	if(recpos + sprintf(buf," %ld",(long)n) >= L_len)
#endif
		(*donewrec)();
	for(p=buf;*p;PUT(*p++));
}
 static VOID
#ifdef KR_headers
lwrt_L(n, len) ftnint n; ftnlen len;
#else
lwrt_L(ftnint n, ftnlen len)
#endif
{
	if(recpos+LLOGW>=L_len)
		(*donewrec)();
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
	if(recpos+len>=L_len)
		(*donewrec)();
	if (!recpos)
		{ PUT(' '); ++recpos; }
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
		goto ret;
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
				goto ret;
			case '.':
				while(*++b);
				goto ret;
			case 'E':
				for(c1 = '.', c = 'E';  *b = c1;
					c1 = c, c = *++b);
				goto ret;
			}
 ret:
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
	register int c, (*pn)() = putn;
#else
	register int c, (*pn)(int) = putn;
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

	if(recpos + l_g(buf,n) >= L_len)
		(*donewrec)();
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
	if(recpos + al + bl + 3 >= L_len && recpos)
		(*donewrec)();
	PUT(' ');
	PUT('(');
	l_put(ba);
	PUT(',');
	if (recpos + bl >= L_len) {
		(*donewrec)();
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
		default: fatal(204,"unknown type in lio");
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
		case TYLOGICAL:
			lwrt_L(Ptr->flint, len);
			break;
		case TYCHAR:
			lwrt_A(ptr,len);
			break;
		}
		ptr += len;
	}
	return(0);
}
./ ADD NAME=mktemp.c TIME=708905875
/* for Zortech */

#ifdef __cplusplus
extern "C" {
#endif
 char *
mktemp(char *s0)
{
	static int n;
	char *s = s0;

	while(*s != 'X')
		s++;
	*s++ = '0' + n/10;
	*s++ =  '0' + n % 10;
	*s = 0;
	n++;
	return s0;
	}
#ifdef __cplusplus
	}
#endif
./ ADD NAME=open.c TIME=708894835
#ifdef MSDOS
#include "types.h"
#include "stat.h"
#else
#include "sys/types.h"
#include "sys/stat.h"
#endif
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
#include "stdlib.h"
extern int canseek(FILE*);
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
isdev(s) char *s;
#else
isdev(char *s)
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
	curunit = b = &units[a->ounit];
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
		if (inode(buf,&n) == b->uinode && n == b->udev)
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
		(void) strcpy(buf,"tmp.FXXXXXX");
		(void) mktemp(buf);
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
	if(isdev(buf))
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
	b->useek=canseek(b->ufd);
#ifndef MSDOS
	if((b->uinode=inode(buf,&b->udev))==-1)
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
./ ADD NAME=rawio.h TIME=708896068
#ifndef KR_headers
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
#ifndef NO_OPEN_DECL
extern int creat(const char*,int), open(const char*,int);
#endif
extern int close(int);
extern int read(int,void*,size_t), write(int,void*,size_t);
extern int unlink(const char*);
extern FILE *fdopen(int, const char*);
#endif

extern char *mktemp(char*);

#ifdef __cplusplus
	}
#endif
#endif
./ ADD NAME=rdfmt.c TIME=708870801
#include "f2c.h"
#include "fio.h"
#include "fmt.h"
#include "fp.h"

extern int cursor;
#ifdef KR_headers
extern double atof();
#else
#undef abs
#include "stdlib.h"
#endif

 static int
#ifdef KR_headers
rd_I(n,w,len, base) ftnlen len; Uint *n; register int base;
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
	if (*ps==' ') {if (cblank) x *= base; ps++; goto loop;}
	if(sign) x = -x;
	if(len==sizeof(integer)) n->il=x;
	else if(len == sizeof(char)) n->ic = x;
	else n->is=x;
	if (*ps) return(errno=115); else return(0);
}
 static int
#ifdef KR_headers
rd_L(n,w) ftnint *n;
#else
rd_L(ftnint *n, int w)
#endif
{	int ch;
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
	if (*ps=='t' || *ps == 'T') { *n=1; return(0); }
	else if (*ps == 'f' || *ps == 'F') { *n=0; return(0); }
	else return(errno=116);
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
	if (ch == ' ' && cblank)
		goto blankdrop;
	scale1 = scale;
	while(isdigit(ch)) {
digloop1:
		if (sp < spe) *sp++ = ch;
		else ++exp;
digloop1e:
		if (!w--) goto done;
		GET(ch);
		}
	if (ch == ' ') {
		if (cblank)
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
				if (cblank) goto skip01;
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
			if (cblank)
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
					if (cblank)
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
		if((ch=(*getn)())<0) return(ch);
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
		else if((ch=(*getn)())<0) return(ch);
		else *s = ch=='\n'?' ':ch;
	return(1);
}
#ifdef KR_headers
rd_ed(p,ptr,len) struct syl *p; char *ptr; ftnlen len;
#else
rd_ed(struct syl *p, char *ptr, ftnlen len)
#endif
{	int ch;
	for(;cursor>0;cursor--) if((ch=(*getn)())<0) return(ch);
	if(cursor<0)
	{	if(recpos+cursor < 0) /*err(elist->cierr,110,"fmt")*/
			cursor = -recpos;	/* is this in the standard? */
		if(external == 0) {
			extern char *icptr;
			icptr += cursor;
		}
		else if(curunit && curunit->useek)
			(void) fseek(cf,(long) cursor,SEEK_CUR);
		else
			err(elist->cierr,106,"fmt");
		recpos += cursor;
		cursor=0;
	}
	switch(p->op)
	{
	default: fprintf(stderr,"rd_ed, unexpected code: %d\n", p->op);
		sig_die(fmtbuf, 1);
	case I: ch = (rd_I((Uint *)ptr,p->p1,len, 10));
		break;
	case IM: ch = (rd_I((Uint *)ptr,p->p1,len, 10));
		break;
	case O: ch = (rd_I((Uint *)ptr, p->p1, len, 8));
		break;
	case L: ch = (rd_L((ftnint *)ptr,p->p1));
		break;
	case A:	ch = (rd_A(ptr,len));
		break;
	case AW:
		ch = (rd_AW(ptr,p->p1,len));
		break;
	case E: case EE:
	case D:
	case G:
	case GE:
	case F:	ch = (rd_F((ufloat *)ptr,p->p1,p->p2,len));
		break;
	}
	if(ch == 0) return(ch);
	else if(ch == EOF) return(EOF);
	if (cf)
		clearerr(cf);
	return(errno);
}
#ifdef KR_headers
rd_ned(p) struct syl *p;
#else
rd_ned(struct syl *p)
#endif
{
	switch(p->op)
	{
	default: fprintf(stderr,"rd_ned, unexpected code: %d\n", p->op);
		sig_die(fmtbuf, 1);
	case APOS:
		return(rd_POS(*(char **)&p->p2));
	case H:	return(rd_H(p->p1,*(char **)&p->p2));
	case SLASH: return((*donewrec)());
	case TR:
	case X:	cursor += p->p1;
		return(1);
	case T: cursor=p->p1-recpos - 1;
		return(1);
	case TL: cursor -= p->p1;
		if(cursor < -recpos)	/* TL1000, 1X */
			cursor = -recpos;
		return(1);
	}
}
./ ADD NAME=rewind.c TIME=708863391
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
	b = &units[a->aunit];
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
./ ADD NAME=rsfe.c TIME=708863550
/* read sequential formatted external */
#include "f2c.h"
#include "fio.h"
#include "fmt.h"

xrd_SL(Void)
{	int ch;
	if(!curunit->uend)
		while((ch=getc(cf))!='\n' && ch!=EOF);
	cursor=recpos=0;
	return(1);
}
x_getc(Void)
{	int ch;
	if(curunit->uend) return(EOF);
	ch = getc(cf);
	if(ch!=EOF && ch!='\n')
	{	recpos++;
		return(ch);
	}
	if(ch=='\n')
	{	(void) ungetc(ch,cf);
		return(ch);
	}
	if(curunit->uend || feof(cf))
	{	errno=0;
		curunit->uend=1;
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
	if(!init) f_init();
	if(n=c_sfe(a)) return(n);
	reading=1;
	sequential=1;
	formatted=1;
	external=1;
	elist=a;
	cursor=recpos=0;
	scale=0;
	fmtbuf=a->cifmt;
	curunit= &units[a->ciunit];
	cf=curunit->ufd;
	if(pars_f(fmtbuf)<0) err(a->cierr,100,"startio");
	getn= x_getc;
	doed= rd_ed;
	doned= rd_ned;
	fmt_bg();
	doend=x_endp;
	donewrec=xrd_SL;
	dorevert=x_rev;
	cblank=curunit->ublnk;
	cplus=0;
	if(curunit->uwrt && nowreading(curunit))
		err(a->cierr,errno,"read start");
	return(0);
}
./ ADD NAME=rsli.c TIME=708864284
#include "f2c.h"
#include "fio.h"
#include "lio.h"

extern flag lquit;
extern int lcount;
extern char *icptr;
extern char *icend;
extern icilist *svic;
extern int icnum, recpos;

int i_getc(Void)
{
	if(recpos >= svic->icirlen) {
		if (recpos++ == svic->icirlen)
			return '\n';
		z_rnew();
		}
	recpos++;
	if(icptr >= icend) err(svic->iciend,(EOF),"endfile");
	return(*icptr++);
	}

#ifdef KR_headers
int i_ungetc(ch, f) int ch; FILE *f;
#else
int i_ungetc(int ch, FILE *f)
#endif
{
	if (--recpos == svic->icirlen)
		return '\n';
	if (recpos < -1)
		err(svic->icierr,110,"recend");
	/* *--icptr == ch, and icptr may point to read-only memory */
	return *--icptr /* = ch */;
	}

 static void
#ifdef KR_headers
c_lir(a) icilist *a;
#else
c_lir(icilist *a)
#endif
{
	extern int l_eof;
	reading = 1;
	external = 0;
	formatted = 1;
	svic = a;
	L_len = a->icirlen;
	recpos = -1;
	icnum = recpos = 0;
	cursor = 0;
	l_getc = i_getc;
	l_ungetc = i_ungetc;
	l_eof = 0;
	icptr = a->iciunit;
	icend = icptr + a->icirlen*a->icirnum;
	cf = 0;
	curunit = 0;
	elist = (cilist *)a;
	}


#ifdef KR_headers
integer s_rsli(a) icilist *a;
#else
integer s_rsli(icilist *a)
#endif
{
	lioproc = l_read;
	lquit = 0;
	lcount = 0;
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
./ ADD NAME=rsne.c TIME=708907688
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
 extern ftnlen typesize[];

 extern flag lquit;
 extern int lcount, nml_read;
 extern t_getc(Void);

#ifdef KR_headers
 extern char *malloc(), *memset();

#ifdef ungetc
 static int
un_getc(x,cf) int x; FILE *cf;
{ return ungetc(x,cf); }
#else
#define un_getc ungetc
 extern int ungetc();
#endif

#else
#undef abs
#include "stdlib.h"
#include "string.h"

#ifdef ungetc
 static int
un_getc(int x, FILE *cf)
{ return ungetc(x,cf); }
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

	if(!init)
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
		err(elist->cierr, ch, "namelist read");
		}
	while(*s = Alphanum[GETC(ch) & 0xff])
		if (s < se)
			s++;
	if (ch == EOF)
		err(elist->cierr, EOF, "namelist read");
	if (ch > ' ')
		Ungetc(ch,cf);
	return *s = 0;
	}

 static int
getnum(int *chp, ftnlen *val)
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
	reading=1;
	formatted=1;
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
		err(elist->cierr, 113, where0);
	for(;;) {
		for(;;) switch(GETC(ch)) {
			case EOF:
				if (got1)
					return 0;
				err(a->ciend,(EOF),where0);
			case '/':
			case '$':
				return 0;
			default:
				if (ch <= ' ' && ch >= 0 || ch == ',')
					continue;
				Ungetc(ch,cf);
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
			size = typesize[type];
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
		lcount = 0;
	 readloop:
		for(;;) {
			if (iva >= ivae || iva < 0) {
				lquit = 1;
				goto mustend;
				}
			else if (iva + no1*size > ivae)
				no1 = (ivae - iva)/size;
			lquit = 0;
			l_read(&no1, vaddr + iva, size, type);
			if (lquit == 1)
				return 0;
 mustend:
			if (GETC(ch) == '/' || ch == '$') {
				lquit = 1;
				return 0;
				}
			else if (lquit) {
				while(ch <= ' ' && ch >= 0)
					GETC(ch);
				Ungetc(ch,cf);
				if (!Alpha[ch & 0xff] && ch >= 0)
					err(a->cierr, 125, where);
				break;
				}
			Ungetc(ch,cf);
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
	int n;
	external=1;
	if(n = c_le(a))
		return n;
	if(curunit->uwrt && nowreading(curunit))
		err(a->cierr,errno,where0);
	l_getc = t_getc;
	l_ungetc = un_getc;
	if (n = x_rsne(a))
		return n;
	return e_rsle();
	}
./ ADD NAME=sfe.c TIME=708865458
/* sequential formatted external common routines*/
#include "f2c.h"
#include "fio.h"

extern char *fmtbuf;

integer e_rsfe(Void)
{	int n;
	n=en_fio();
	if (cf == stdout)
		fflush(stdout);
	else if (cf == stderr)
		fflush(stderr);
	fmtbuf=NULL;
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
	p = &units[a->ciunit];
	if(p->ufd==NULL && fk_open(SEQ,FMT,a->ciunit)) err(a->cierr,114,"sfe")
	if(!p->ufmt) err(a->cierr,102,"sfe")
	return(0);
}
integer e_wsfe(Void)
{	return(e_rsfe());
}
./ ADD NAME=sig_die.c TIME=708905928
#include "stdio.h"
#include "signal.h"

#ifndef SIGIOT
#define SIGIOT SIGABRT
#endif

#ifdef __cplusplus
extern "C" {
#endif
#ifdef KR_headers
void sig_die(s, kill) register char *s; int kill;
#else
#include "stdlib.h"
 extern void f_exit(void);

void sig_die(register char *s, int kill)
#endif
{
	/* print error message, then clear buffers */
	fprintf(stderr, "%s\n", s);
	fflush(stderr);
	f_exit();
	fflush(stderr);

	if(kill)
		{
		/* now get a core */
		signal(SIGIOT, SIG_DFL);
		abort();
		}
	else
		exit(1);
	}
#ifdef __cplusplus
	}
#endif
./ ADD NAME=sue.c TIME=708865914
#include "f2c.h"
#include "fio.h"
extern uiolen reclen;
long recloc;

#ifdef KR_headers
c_sue(a) cilist *a;
#else
c_sue(cilist *a)
#endif
{
	if(a->ciunit >= MXUNIT || a->ciunit < 0)
		err(a->cierr,101,"startio");
	external=sequential=1;
	formatted=0;
	curunit = &units[a->ciunit];
	elist=a;
	if(curunit->ufd==NULL && fk_open(SEQ,UNF,a->ciunit))
		err(a->cierr,114,"sue");
	cf=curunit->ufd;
	if(curunit->ufmt) err(a->cierr,103,"sue")
	if(!curunit->useek) err(a->cierr,103,"sue")
	return(0);
}
#ifdef KR_headers
integer s_rsue(a) cilist *a;
#else
integer s_rsue(cilist *a)
#endif
{
	int n;
	if(!init) f_init();
	reading=1;
	if(n=c_sue(a)) return(n);
	recpos=0;
	if(curunit->uwrt && nowreading(curunit))
		err(a->cierr, errno, "read start");
	if(fread((char *)&reclen,sizeof(uiolen),1,cf)
		!= 1)
	{	if(feof(cf))
		{	curunit->uend = 1;
			err(a->ciend, EOF, "start");
		}
		clearerr(cf);
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
	if(!init) f_init();
	if(n=c_sue(a)) return(n);
	reading=0;
	reclen=0;
	if(curunit->uwrt != 1 && nowwriting(curunit))
		err(a->cierr, errno, "write start");
	recloc=ftell(cf);
	(void) fseek(cf,(long)sizeof(uiolen),SEEK_CUR);
	return(0);
}
integer e_wsue(Void)
{	long loc;
	(void) fwrite((char *)&reclen,sizeof(uiolen),1,cf);
	loc=ftell(cf);
	(void) fseek(cf,recloc,SEEK_SET);
	(void) fwrite((char *)&reclen,sizeof(uiolen),1,cf);
	(void) fseek(cf,loc,SEEK_SET);
	return(0);
}
integer e_rsue(Void)
{
	(void) fseek(cf,(long)(reclen-recpos+sizeof(uiolen)),SEEK_CUR);
	return(0);
}
./ ADD NAME=typesize.c TIME=640009768
#include "f2c.h"

ftnlen typesize[] = { 0, 0, sizeof(shortint), sizeof(integer),
			sizeof(real), sizeof(doublereal),
			sizeof(complex), sizeof(doublecomplex),
			sizeof(logical), sizeof(char) };
./ ADD NAME=uio.c TIME=708866134
#include "f2c.h"
#include "fio.h"
uiolen reclen;

#ifdef KR_headers
do_us(number,ptr,len) ftnint *number; char *ptr; ftnlen len;
#else
do_us(ftnint *number, char *ptr, ftnlen len)
#endif
{
	if(reading)
	{
		recpos += *number * len;
		if(recpos>reclen)
			err(elist->ciend, 110, "do_us");
		if (fread(ptr,(int)len,(int)(*number),cf) != *number)
			err(elist->ciend, EOF, "do_us");
		return(0);
	}
	else
	{
		reclen += *number * len;
		(void) fwrite(ptr,(int)len,(int)(*number),cf);
		return(0);
	}
}
#ifdef KR_headers
integer do_ud(number,ptr,len) ftnint *number; char *ptr; ftnlen len;
#else
integer do_ud(ftnint *number, char *ptr, ftnlen len)
#endif
{
	recpos += *number * len;
	if(recpos > curunit->url && curunit->url!=1)
		err(elist->cierr,110,"do_ud");
	if(reading)
	{
		if(fread(ptr,(int)len,(int)(*number),cf) != *number)
			err(elist->cierr,EOF,"do_ud")
		else return(0);
	}
	(void) fwrite(ptr,(int)len,(int)(*number),cf);
	return(0);
}
#ifdef KR_headers
integer do_uio(number,ptr,len) ftnint *number; char *ptr; ftnlen len;
#else
integer do_uio(ftnint *number, char *ptr, ftnlen len)
#endif
{
	if(sequential)
		return(do_us(number,ptr,len));
	else	return(do_ud(number,ptr,len));
}
./ ADD NAME=util.c TIME=708907661
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
long inode(a, dev) char *a; int *dev;
#else
long inode(char *a, int *dev)
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
mvgbt(n,len,a,b) char *a,*b;
#else
mvgbt(int n, int len, char *a, char *b)
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
./ ADD NAME=wref.c TIME=708866396
#include "f2c.h"
#include "fio.h"
#include "fmt.h"
#include "fp.h"
#ifndef VAX
#include "ctype.h"
#endif

#ifndef KR_headers
#undef abs
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
	if(scale) {
		if(scale >= d + 2 || scale <= -d)
			goto nogood;
		}
	if(scale <= 0)
		--d;
	if (len == sizeof(real))
		dd = p->pf;
	else
		dd = p->pd;
	if (dd >= 0.) {
		sign = 0;
		signspace = cplus;
#ifndef VAX
		if (!dd)
			dd = 0.;	/* avoid -0 */
#endif
		}
	else {
		signspace = sign = 1;
		dd = -dd;
		}
	delta = w - (2 /* for the . and the d adjustment above */
			+ 2 /* for the E+ */ + signspace + d + e);
	if (delta < 0) {
nogood:
		while(--w >= 0)
			PUT('*');
		return(0);
		}
	if (scale < 0)
		d += scale;
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
	if (scale != 1 && dd)
		sprintf(se, "%+.2d", atoi(se) + 1 - scale);
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
	i = scale;
	if (scale <= 0) {
		PUT('.');
		for(; i < 0; ++i)
			PUT('0');
		PUT(*s);
		s += 2;
		}
	else if (scale > 1) {
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

	if (n = scale)
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
	if (sign || cplus)
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
	else if (cplus)
		PUT('+');
	while(n = *b++)
		PUT(n);
	while(--d1 >= 0)
		PUT('0');
	return 0;
	}
./ ADD NAME=wrtfmt.c TIME=708868978
#include "f2c.h"
#include "fio.h"
#include "fmt.h"
extern int cursor;
#ifdef KR_headers
extern char *icvt();
#else
extern char *icvt(long, int*, int*, int);
#endif
int hiwater;
icilist *svic;
char *icptr;
mv_cur(Void)	/* shouldn't use fseek because it insists on calling fflush */
		/* instead we know too much about stdio */
{
	if(external == 0) {
		if(cursor < 0) {
			if(hiwater < recpos)
				hiwater = recpos;
			recpos += cursor;
			icptr += cursor;
			cursor = 0;
			if(recpos < 0)
				err(elist->cierr, 110, "left off");
		}
		else if(cursor > 0) {
			if(recpos + cursor >= svic->icirlen)
				err(elist->cierr, 110, "recend");
			if(hiwater <= recpos)
				for(; cursor > 0; cursor--)
					(*putn)(' ');
			else if(hiwater <= recpos + cursor) {
				cursor -= hiwater - recpos;
				icptr += hiwater - recpos;
				recpos = hiwater;
				for(; cursor > 0; cursor--)
					(*putn)(' ');
			}
			else {
				icptr += cursor;
				recpos += cursor;
			}
			cursor = 0;
		}
		return(0);
	}
	if(cursor > 0) {
		if(hiwater <= recpos)
			for(;cursor>0;cursor--) (*putn)(' ');
		else if(hiwater <= recpos + cursor) {
#ifndef NON_UNIX_STDIO
			if(cf->_ptr + hiwater - recpos < buf_end(cf))
				cf->_ptr += hiwater - recpos;
			else
#endif
				(void) fseek(cf, (long) (hiwater - recpos), SEEK_CUR);
			cursor -= hiwater - recpos;
			recpos = hiwater;
			for(; cursor > 0; cursor--)
				(*putn)(' ');
		}
		else {
#ifndef NON_UNIX_STDIO
			if(cf->_ptr + cursor < buf_end(cf))
				cf->_ptr += cursor;
			else
#endif
				(void) fseek(cf, (long)cursor, SEEK_CUR);
			recpos += cursor;
		}
	}
	if(cursor<0)
	{
		if(cursor+recpos<0) err(elist->cierr,110,"left off");
#ifndef NON_UNIX_STDIO
		if(cf->_ptr + cursor >= cf->_base)
			cf->_ptr += cursor;
		else
#endif
		if(curunit && curunit->useek)
			(void) fseek(cf,(long)cursor,SEEK_CUR);
		else
			err(elist->cierr,106,"fmt");
		if(hiwater < recpos)
			hiwater = recpos;
		recpos += cursor;
		cursor=0;
	}
	return(0);
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
	ans=icvt(x,&ndigit,&sign, base);
	spare=w-ndigit;
	if(sign || cplus) spare--;
	if(spare<0)
		for(i=0;i<w;i++) (*putn)('*');
	else
	{	for(i=0;i<spare;i++) (*putn)(' ');
		if(sign) (*putn)('-');
		else if(cplus) (*putn)('+');
		for(i=0;i<ndigit;i++) (*putn)(*ans++);
	}
	return(0);
}
 static int
#ifdef KR_headers
wrt_IM(n,w,m,len) Uint *n; ftnlen len;
#else
wrt_IM(Uint *n, int w, int m, ftnlen len)
#endif
{	int ndigit,sign,spare,i,xsign;
	long x;
	char *ans;
	if(sizeof(integer)==len) x=n->il;
	else if(len == sizeof(char)) x = n->ic;
	else x=n->is;
	ans=icvt(x,&ndigit,&sign, 10);
	if(sign || cplus) xsign=1;
	else xsign=0;
	if(ndigit+xsign>w || m+xsign>w)
	{	for(i=0;i<w;i++) (*putn)('*');
		return(0);
	}
	if(x==0 && m==0)
	{	for(i=0;i<w;i++) (*putn)(' ');
		return(0);
	}
	if(ndigit>=m)
		spare=w-ndigit-xsign;
	else
		spare=w-m-xsign;
	for(i=0;i<spare;i++) (*putn)(' ');
	if(sign) (*putn)('-');
	else if(cplus) (*putn)('+');
	for(i=0;i<m-ndigit;i++) (*putn)('0');
	for(i=0;i<ndigit;i++) (*putn)(*ans++);
	return(0);
}
 static int
#ifdef KR_headers
wrt_AP(s) char *s;
#else
wrt_AP(char *s)
#endif
{	char quote;
	if(cursor && mv_cur()) return(mv_cur());
	quote = *s++;
	for(;*s;s++)
	{	if(*s!=quote) (*putn)(*s);
		else if(*++s==quote) (*putn)(*s);
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
	if(cursor && mv_cur()) return(mv_cur());
	while(a--) (*putn)(*s++);
	return(1);
}
#ifdef KR_headers
wrt_L(n,len, sz) Uint *n; ftnlen sz;
#else
wrt_L(Uint *n, int len, ftnlen sz)
#endif
{	int i;
	long x;
	if(sizeof(integer)==sz) x=n->il;
	else if(sz == sizeof(char)) x = n->ic;
	else x=n->is;
	for(i=0;i<len-1;i++)
		(*putn)(' ');
	if(x) (*putn)('T');
	else (*putn)('F');
	return(0);
}
 static int
#ifdef KR_headers
wrt_A(p,len) char *p; ftnlen len;
#else
wrt_A(char *p, ftnlen len)
#endif
{
	while(len-- > 0) (*putn)(*p++);
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
		(*putn)(' ');
	}
	while(w-- > 0)
		(*putn)(*p++);
	return(0);
}

 static int
#ifdef KR_headers
wrt_G(p,w,d,e,len) ufloat *p; ftnlen len;
#else
wrt_G(ufloat *p, int w, int d, int e, ftnlen len)
#endif
{	double up = 1,x;
	int i,oldscale=scale,n,j;
	x= len==sizeof(real)?p->pf:p->pd;
	if(x < 0 ) x = -x;
	if(x<.1) return(wrt_E(p,w,d,e,len));
	for(i=0;i<=d;i++,up*=10)
	{	if(x>=up) continue;
		scale=0;
		if(e==0) n=4;
		else	n=e+2;
		i=wrt_F(p,w-n,d-i,len);
		for(j=0;j<n;j++) (*putn)(' ');
		scale=oldscale;
		return(i);
	}
	return(wrt_E(p,w,d,e,len));
}
#ifdef KR_headers
w_ed(p,ptr,len) struct syl *p; char *ptr; ftnlen len;
#else
w_ed(struct syl *p, char *ptr, ftnlen len)
#endif
{
	if(cursor && mv_cur()) return(mv_cur());
	switch(p->op)
	{
	default:
		fprintf(stderr,"w_ed, unexpected code: %d\n", p->op);
		sig_die(fmtbuf, 1);
	case I:	return(wrt_I((Uint *)ptr,p->p1,len, 10));
	case IM:
		return(wrt_IM((Uint *)ptr,p->p1,p->p2,len));
	case O:	return(wrt_I((Uint *)ptr, p->p1, len, 8));
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
	}
}
#ifdef KR_headers
w_ned(p) struct syl *p;
#else
w_ned(struct syl *p)
#endif
{
	switch(p->op)
	{
	default: fprintf(stderr,"w_ned, unexpected code: %d\n", p->op);
		sig_die(fmtbuf, 1);
	case SLASH:
		return((*donewrec)());
	case T: cursor = p->p1-recpos - 1;
		return(1);
	case TL: cursor -= p->p1;
		if(cursor < -recpos)	/* TL1000, 1X */
			cursor = -recpos;
		return(1);
	case TR:
	case X:
		cursor += p->p1;
		return(1);
	case APOS:
		return(wrt_AP(*(char **)&p->p2));
	case H:
		return(wrt_H(p->p1,*(char **)&p->p2));
	}
}
./ ADD NAME=wsfe.c TIME=708863158
/*write sequential formatted external*/
#include "f2c.h"
#include "fio.h"
#include "fmt.h"
extern int hiwater;

#ifdef KR_headers
x_putc(c)
#else
x_putc(int c)
#endif
{
	/* this uses \n as an indicator of record-end */
	if(c == '\n' && recpos < hiwater) {	/* fseek calls fflush, a loss */
#ifndef NON_UNIX_STDIO
		if(cf->_ptr + hiwater - recpos < buf_end(cf))
			cf->_ptr += hiwater - recpos;
		else
#endif
			(void) fseek(cf, (long)(hiwater - recpos), SEEK_CUR);
	}
	recpos++;
	return putc(c,cf);
}
x_wSL(Void)
{
	(*putn)('\n');
	recpos=0;
	cursor = 0;
	hiwater = 0;
	return(1);
}
xw_end(Void)
{
	if(nonl == 0)
		(*putn)('\n');
	hiwater = recpos = cursor = 0;
	return(0);
}
xw_rev(Void)
{
	if(workdone) (*putn)('\n');
	hiwater = recpos = cursor = 0;
	return(workdone=0);
}

#ifdef KR_headers
integer s_wsfe(a) cilist *a;	/*start*/
#else
integer s_wsfe(cilist *a)	/*start*/
#endif
{	int n;
	if(!init) f_init();
	if(n=c_sfe(a)) return(n);
	reading=0;
	sequential=1;
	formatted=1;
	external=1;
	elist=a;
	hiwater = cursor=recpos=0;
	nonl = 0;
	scale=0;
	fmtbuf=a->cifmt;
	curunit = &units[a->ciunit];
	cf=curunit->ufd;
	if(pars_f(fmtbuf)<0) err(a->cierr,100,"startio");
	putn= x_putc;
	doed= w_ed;
	doned= w_ned;
	doend=xw_end;
	dorevert=xw_rev;
	donewrec=x_wSL;
	fmt_bg();
	cplus=0;
	cblank=curunit->ublnk;
	if(curunit->uwrt != 1 && nowwriting(curunit))
		err(a->cierr,errno,"write start");
	return(0);
}
./ ADD NAME=wsle.c TIME=708862784
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
	if(!init) f_init();
	if(n=c_le(a)) return(n);
	reading=0;
	external=1;
	formatted=1;
	putn = t_putc;
	lioproc = l_write;
	L_len = LINE;
	donewrec = x_wSL;
	if(curunit->uwrt != 1 && nowwriting(curunit))
		err(a->cierr, errno, "list output start");
	return(0);
	}

integer e_wsle(Void)
{
	t_putc('\n');
	recpos=0;
	if (cf == stdout)
		fflush(stdout);
	else if (cf == stderr)
		fflush(stderr);
	return(0);
	}
./ ADD NAME=wsne.c TIME=708862700
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

	if(!init)
		f_init();
	if(n=c_le(a))
		return(n);
	reading=0;
	external=1;
	formatted=1;
	putn = t_putc;
	L_len = LINE;
	donewrec = x_wSL;
	if(curunit->uwrt != 1 && nowwriting(curunit))
		err(a->cierr, errno, "namelist output start");
	x_wsne(a);
	return e_wsle();
	}
./ ADD NAME=xwsne.c TIME=708824981
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
	extern ftnlen typesize[];

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
		if (recpos+strlen(s)+2 >= L_len)
			(*donewrec)();
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
			size = typesize[type];
		l_write(number, v->addr, size, type);
		if (vd < vde) {
			if (recpos+2 >= L_len)
				(*donewrec)();
			PUT(',');
			PUT(' ');
			}
		else if (recpos+1 >= L_len)
			(*donewrec)();
		}
	PUT('/');
	}
./ ENDUP
