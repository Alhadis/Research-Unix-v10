#ifndef _SFHDR_H
#define _SFHDR_H

/*	Internal definitions for sfio.
**	Written by Kiem-Phong Vo (07/16/90)
**	AT&T Bell Laboratories
*/

/* the parts of Sfio_t private to sfio functions */
#define _SFIO_PRIVATE \
	int		size;	/* buffer size			*/ \
	unsigned char	*data;	/* data buffer			*/ \
	long		here;	/* current physical location	*/ \
	unsigned char	mode;	/* current io mode		*/ \
	unsigned char	tiny[1];/* for unbuffered read stream	*/ \
	struct _sfdc_	*disc;	/* discipline			*/ \
	struct _sfpl_	*pool;	/* the pool containing this	*/ \
	struct _sfio_	*back;	/* backward link in pool	*/ \
	struct _sfio_	*fore;	/* forward link in pool		*/

#include	"sfio.h"
#include	<errno.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<ctype.h>
#include	<fcntl.h>
#if __STDC__ || __cplusplus
#include	<stdarg.h>
#else
#include	<varargs.h>
#endif

#define uchar		unsigned char
#define ulong		unsigned long
#define uint		unsigned int
#define reg		register
#define NIL(p)		((p)0)

/* stream pool structure. */
typedef struct _sfpl_
{
	int		mode;		/* type of pool			*/
	Sfile_t		*head;		/* head of pool's stream list	*/
	struct _sfpl_	*back;		/* linked list of pools		*/
	struct _sfpl_	*fore;
} Sfpool_t;

/* extension structures for sfvprintf/sfvscanf */
typedef int(*Argf_s)_SFA_((char,char*,uint));
typedef int(*Extf_s)_SFA_((Sfile_t*,int,int,char**));
typedef int(*Argf_p)_SFA_((int,char*));
typedef int(*Extf_p)_SFA_((char*,int,int,char*));
typedef struct _fa_
{
	char		*form;		/* format string		*/
	va_list		args;		/* corresponding arglist	*/
	union
	{ Argf_s	s;		/* argf for sfvscanf		*/
	  Argf_p	p;		/* argf for sfvprintf		*/
	}		argf;
	union
	{ Extf_s	s;		/* extf for sfvscanf		*/
	  Extf_p	p;		/* extf for sfvprintf		*/
	}		extf;
	struct _fa_	*next;		/* next on the stack		*/
} Fa_t;
#define FAMALLOC(f)	((f = _Fafree) ? (_Fafree = f->next, f) : \
				(f = (Fa_t*)malloc(sizeof(Fa_t))))
#define FAFREE(f)	(f->next = _Fafree, _Fafree = f)

/* function to clear an sfio structure */
#define SFCLEAR(f) \
	((f)->next = (f)->endw = (f)->endr = (f)->endb = (f)->data = NIL(uchar*), \
	 (f)->flags = 0, (f)->file = -1, (f)->orig = (f)->here = -1L, \
	 (f)->mode = 0, (f)->size = 0, (f)->disc = NIL(Sfdisc_t*), \
	 (f)->pool = NIL(Sfpool_t*), (f)->push = (f)->back = (f)->fore = NIL(Sfile_t*))

/* get the real value of a byte in a coded long or ulong */
#define SFUVALUE(v)	((v)&(SF_MORE-1))
#define SFSVALUE(v)	((v)&(SF_SIGN-1))

/* amount of precision to get in each iteration during coding of doubles */
#define SF_PRECIS	(SF_UBITS-1)

/* garin size for buffer increment */
#define SF_GRAIN	1024

/* bits for the mode field */
#define SF_SYNC		0010	/* stream was synced			*/
#define SF_PUSH		0020	/* stream has been pushed		*/
#define SF_LOCK		0040	/* stream is locked for io op		*/
#define SF_POOL		0200	/* stream is in a pool but not current	*/

/* check to see if the stream can be accessed */
#define SFFROZEN(f)	((f)->mode&(SF_PUSH|SF_LOCK))

/* NOTE: these flags share the same space with the public flags */
#define SF_PROCESS	0010000	/* this stream is sfpopen		*/
#define SF_PUTC		0020000	/* sfputc ok on this stream		*/
#define SF_GETC		0040000	/* sfgetc ok on this stream		*/
#define SF_BOTH		0100000	/* both read/write			*/

/* lock/open a stream */
#define SFlock(f)	((f)->mode |= SF_LOCK)
#define SFopen(f)	((f)->mode &= ~SF_LOCK)
#define SFLOCK(f)	(SFlock(f), (f)->endr = (f)->endw = (f)->data)
#define SFOPEN(f)	(SFopen(f), \
			 (f)->endr = ((f)->flags&SF_GETC) ? (f)->endb : (f)->data, \
			 (f)->endw = ((f)->flags&SF_PUTC) ? (f)->endb : (f)->data)

/* short-hand for common stream types */
#define SF_RDWR		(SF_READ|SF_WRITE)
#define SF_RDSTR	(SF_READ|SF_STRING)
#define SF_WRSTR	(SF_WRITE|SF_STRING)

/* exception types */
#define SF_EDONE	0	/* stop this operation and return	*/
#define SF_EDISC	1	/* discipline says it's ok		*/
#define SF_ESTACK	2	/* stack was popped			*/
#define SF_ECONT	3	/* can continue normally		*/

/* more than this for a line buffer, we might as well flush */
#define HIFORLINE	128

/* read/write/seek functions with disciplines */
#define READF(rdfn,f,d,n,hdl)	((rdfn) ? (*(rdfn))(f,d,n,hdl) : read((f)->file,d,n))
#define WRITEF(wrfn,f,d,n,hdl)	((wrfn) ? (*(wrfn))(f,d,n,hdl) : write((f)->file,(char*)d,n))
#define SEEKF(skfn,f,p,o,hdl)	((skfn) ? (*(skfn))(f,p,o,hdl) : lseek((f)->file,p,o))

/* control flags for open() */
#ifndef O_CREAT	/* V9 */
#define NO_OFLAGS
#define O_CREAT		004
#define O_TRUNC		010
#define O_APPEND	020

#ifndef O_RDONLY
#define	O_RDONLY	000
#endif
#ifndef O_WRONLY
#define O_WRONLY	001
#endif
#ifndef O_RDWR
#define O_RDWR		002
#endif
#endif

/* floating point to ascii conversion */
#define MAXLONG		((long)(((ulong)~0L) >> 1))
#define MAXEXP10	6
#define MAXPOW10	(1 << MAXEXP10)
#define FDIGITS		60		/* max allowed fractional digits */
#define IDIGITS		1536		/* max number of digits in int part */
#define MAXDIGITS	(((FDIGITS+IDIGITS)/sizeof(int) + 1)*sizeof(int))

/* sfucvt() converts decimal integers to ASCII */
#define SFDIGIT(v,scale,digit) \
	{ if(v < 5*scale) \
		if(v < 2*scale) \
			if(v < 1*scale) \
				{ digit = '0'; } \
			else	{ digit = '1'; v -= 1*scale; } \
		else	if(v < 3*scale) \
				{ digit = '2'; v -= 2*scale; } \
			else if(v < 4*scale) \
				{ digit = '3'; v -= 3*scale; } \
			else	{ digit = '4'; v -= 4*scale; } \
	  else	if(v < 7*scale) \
			if(v < 6*scale) \
				{ digit = '5'; v -= 5*scale; } \
			else	{ digit = '6'; v -= 6*scale; } \
		else	if(v < 8*scale) \
				{ digit = '7'; v -= 7*scale; } \
			else if(v < 9*scale) \
				{ digit = '8'; v -= 8*scale; } \
			else	{ digit = '9'; v -= 9*scale; } \
	}
#define sfucvt(v,s,n,w,list) \
	{ list = (char*)_Sfdec; \
	  while((ulong)v >= 100000) \
	  {	w = v; v = ((ulong)v)/100000; n = ((ulong)w) - ((ulong)v)*100000; \
		SFDIGIT(n,10000,s[-5]); \
		SFDIGIT(n,1000,s[-4]); \
		SFDIGIT(n,100,s[-3]); \
		*--s = (char)('0' + ((n = (uchar)list[n])&017)); \
		*--s = (char)('0' + (n >> 4)); \
		s -= 3; \
	  } \
	  if(v >= 100) \
	  {	if(v >= 1000) \
		{	if(v >= 10000) \
			{	w = 3; \
				SFDIGIT(v,10000,s[-5]); \
			} \
			else	w = 2; \
			SFDIGIT(v,1000,s[-4]); \
		} \
		else	w = 1; \
		SFDIGIT(v,100,s[-3]); \
	  	*--s = (char)('0' + ((n = (uchar)list[v])&017)); \
		*--s = (char)('0' + (n >> 4)); \
		s -= w; \
	  } \
	  else \
	  {	*--s = (char)('0' + ((n = (uchar)list[v])&017)); \
	  	if((n >>= 4) > 0) \
			*--s = (char)('0' + n); \
	  } \
	}

/* fast peek of a stream */
#define SFAVAIL(f,s,n)	((n) = (f)->endb - ((s) = (f)->next) )
#define SFRPEEK(f,s,n)	(SFAVAIL((f),(s),(n)) > 0 ? (n) : \
			 ((SFopen(f), (n) = _sffilbuf(f,-1), SFLOCK(f)), \
			  (n) <= 0 ? -1 : SFAVAIL(f,s,n)))
#define SFWPEEK(f,s,n)	(SFAVAIL((f),(s),(n)) > 0 ? (n) : \
			 ((SFopen(f), (n) = _sfflsbuf(f,-1), SFLOCK(f)), \
			  (n) <  0 ? -1 : SFAVAIL(f,s,n)))
#define SFMODE(f,m)	((f)->mode != SF_STRING ? _sfmode((f),(m)) : \
			 (((f)->flags&(m)) == (m) ? 0 : -1))

/* malloc and free of streams */
#define SFFREE(f)	(f->fore = _Sffree, _Sffree = f)
#define SFALLOC(f)	((f = _Sffree) ? (_Sffree = f->fore, f) : \
				   (f = (Sfile_t*)malloc(sizeof(Sfile_t))))

/* handy functions */
#define min(x,y)	((x) < (y) ? (x) : (y))
#define max(x,y)	((x) > (y) ? (x) : (y))

/* fast functions for memory copy and memory clear */
#ifdef LIBX
#include	"FEATURE/bcopy"
#include	"FEATURE/atexit"
#include	"FEATURE/onexit"
#include	"FEATURE/on_exit"
#else
#ifdef BSD
#define	_lib_bcopy
#define _lib_on_exit
#else
#ifdef V9
#define _lib_onexit
#endif /*V9*/
#endif /*BSD*/
#endif /*LIBX*/

#ifdef _lib_bcopy
#undef	memcpy
#define memcpy(to,fr,n)	bcopy((fr),(to),(n))
#define memclear(s,n)	bzero((s),(n))
#else
#define memclear(s,n)	memset((s),'\0',(n))
#endif

#ifndef _lib_atexit
#ifdef _lib_on_exit
#define atexit	on_exit
#else
#ifdef _lib_onexit
#define atexit	onexit
#endif /*onexit*/
#endif /*on_exit*/
#endif /*atexit*/

#ifdef atexit
#define _lib_atexit
#endif

/* note that MEMCPY advances the associated pointers */
#define MEMCPY(to,fr,n) \
	switch(n) \
	{ default : memcpy((char*)to,(char*)fr,n); to += n; fr += n; break; \
	  case  7 : *to++ = *fr++; \
	  case  6 : *to++ = *fr++; \
	  case  5 : *to++ = *fr++; \
	  case  4 : *to++ = *fr++; \
	  case  3 : *to++ = *fr++; \
	  case  2 : *to++ = *fr++; \
	  case  1 : *to++ = *fr++; \
	  case  0 : break; \
	}
#define MEMSET(s,c,n) \
	switch(n) \
	{ default : memset((char*)s,(char)c,n); s += n; break; \
	  case  7 : *s++ = c; \
	  case  6 : *s++ = c; \
	  case  5 : *s++ = c; \
	  case  4 : *s++ = c; \
	  case  3 : *s++ = c; \
	  case  2 : *s++ = c; \
	  case  1 : *s++ = c; \
	  case  0 : break; \
	}

#ifdef __cplusplus
extern "C" {
#endif

extern Fa_t	*_Fafree;
extern Sfile_t	*_Sffree;
extern Sfpool_t	_Sfpool;
extern double	_Sfpos10[MAXEXP10], _Sfneg10[MAXEXP10];
extern uchar	_Sfdec[];
extern int	(*_Sfpclose)_SFA_((Sfile_t*));
extern int	(*_Sfpmove)_SFA_((Sfile_t*, int));
extern Sfile_t	*(*_Sfstack)_SFA_((Sfile_t*, Sfile_t*));
extern void	(*_Sfnotify)_SFA_((Sfile_t*, int));

extern int	errno;
extern int	_sfmode _SFA_((Sfile_t*, int));
extern int	_sfexcept _SFA_((Sfile_t*, int, int));
extern double	frexp _SFA_((double, int*)), ldexp _SFA_((double,int));
extern double	strtod _SFA_((char*, char**));
extern char	*malloc _SFA_((int));
extern char	*realloc _SFA_((char*,int));
extern char	*mktemp _SFA_((char*));
extern int	unlink _SFA_((char*));
extern void	free _SFA_((char*));
extern int	strlen _SFA_((const char*));
extern int	isatty _SFA_((int));
extern void	memset _SFA_((char*,char,int));

#ifdef _lib_bcopy
extern void	bcopy _SFA_((char*,char*,int));
#ifndef __cplusplus
extern void	bzero _SFA_((char*,int));
#endif
#else
extern void	memcpy _SFA_((void*,void*,int));
#endif

extern int	read _SFA_((int,void*,int));
extern int	write _SFA_((int,void*,int));
extern long	lseek _SFA_((int,long,int));
extern int	open _SFA_((char*, int, int));
extern int	close _SFA_((int));
extern int	dup _SFA_((int));
extern void	_exit _SFA_((int));
extern int	atexit _SFA_((void(*)()));

#if __cplusplus
}
#endif

#endif /* _SFHDR_H */
