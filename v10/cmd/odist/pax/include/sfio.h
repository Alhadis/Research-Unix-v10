#ifndef _SFIO_H		/* protect against multiple #includes */
#define _SFIO_H

#if __STDC__ || __cplusplus || c_plusplus
#define _SFA_(x)	x
#else
#define _SFA_(x)	()
#endif

#ifndef NULL
#define NULL	0
#endif
#ifndef EOF
#define EOF	(-1)
#endif
#ifndef SEEK_SET
#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2
#endif

typedef struct _sfdc_	Sfdisc_t;
typedef struct _sfio_	Sfile_t, SFIO;

/* discipline structure */
struct _sfdc_
{
	int	(*readf) _SFA_((Sfile_t*, unsigned char*, int, char*));
	int	(*writef) _SFA_((Sfile_t*, const unsigned char*, int, char*));
	long	(*seekf) _SFA_((Sfile_t*, long, int, char*));
	int	(*exceptf) _SFA_((Sfile_t*, int, char*));
	char	*handle;	/* to store any state information	*/
};

/* a file structure */
struct _sfio_
{
	unsigned char	*next;	/* next position to read/write from	*/
	unsigned char	*endw;	/* end of write buffer			*/
	unsigned char	*endr;	/* end of read buffer			*/
	unsigned char	*endb;	/* end of buffer			*/
	struct _sfio_	*push;	/* the stream that was pushed on	*/
	short		flags;	/* type of stream			*/
	short		file;	/* file descriptor			*/
	long		orig;	/* where we start in the file		*/
#ifdef _SFIO_PRIVATE
	_SFIO_PRIVATE
#endif
};

/* bits for various types of files */
#define	SF_READ		000001	/* open for reading			*/
#define SF_WRITE	000002	/* open for writing			*/
#define SF_STRING	000004	/* a string stream			*/
#define SF_APPEND	000010	/* associated file is in append mode	*/
#define SF_RELATIVE	000020	/* file pos is relative to starting pos	*/
#define SF_MALLOC	000040	/* buffered space malloc-ed		*/
#define SF_LINE		000100	/* line buffering			*/
#define SF_KEEPFD	000200	/* keep file opened when closing stream	*/
#define SF_SHARE	000400	/* file stream that is shared		*/
#define SF_REUSE	001000	/* keep stream space after closing	*/
#define SF_FLAGS	000777	/* PUBLIC FLAGS PASSABLE TO SFNEW()	*/
#define SF_SETS		001743	/* flags passable to sfset()		*/

#define SF_EOF		002000	/* eof was detected			*/
#define SF_ERROR	004000	/* an error happened			*/

#define SF_BUFSIZE	8192	/* suggested default buffer size	*/
#define SF_UNBOUND	(-1)	/* unbounded buffer size		*/

#define	sfstdin		(&_Sfstdin)	/* standard input stream	*/
#define	sfstdout	(&_Sfstdout)	/* standard output stream	*/
#define	sfstderr	(&_Sfstderr)	/* standard error stream	*/

#if __cplusplus
extern "C"
{
#endif

extern int		_Sfi;
extern Sfile_t		_Sfstdin, _Sfstdout, _Sfstderr;

extern Sfile_t		*sfnew _SFA_((Sfile_t*, unsigned char*, int, int, int));
extern Sfile_t		*sfopen _SFA_((Sfile_t*, const char*, const char*));
extern Sfile_t		*sfdopen _SFA_((int, const char*));
extern Sfile_t		*sfpopen _SFA_((const char*, const char*, Sfile_t**));
extern Sfile_t		*sfstack _SFA_((Sfile_t*, Sfile_t*));
extern Sfile_t		*_sftmpfile _SFA_((int));
extern int		_sfflsbuf _SFA_((Sfile_t*, int));
extern int		_sffilbuf _SFA_((Sfile_t*, int));
extern int		_sfsync _SFA_((Sfile_t*));
extern int		_sfpeek _SFA_((Sfile_t*, unsigned char**));
extern int		sfclrlock _SFA_((Sfile_t*));
extern unsigned char*	sfsetbuf _SFA_((Sfile_t*, unsigned char*, int));
extern Sfdisc_t*	sfsetdisc _SFA_((Sfile_t*,Sfdisc_t*));
extern int		sfnotice _SFA_((void(*)(Sfile_t*, int)));
extern int		sfset _SFA_((Sfile_t*, int, int));
extern int		sfpool _SFA_((Sfile_t*, Sfile_t*, int));
extern int		sfread _SFA_((Sfile_t*, unsigned char*, int));
extern int		sfwrite _SFA_((Sfile_t*, const unsigned char*, int));
extern int		sfmove _SFA_((Sfile_t*, Sfile_t*, long, const char*));
extern int		sfclose _SFA_((Sfile_t*));
extern long		sftell _SFA_((Sfile_t*));
extern long		sfseek _SFA_((Sfile_t*, long, int));
extern int		sfllen _SFA_((long));
extern int		sfdlen _SFA_((double));
extern int		sfputs _SFA_((Sfile_t*, const char*, int));
extern char		*sfgets _SFA_((Sfile_t*, char*, int));
extern int		sfnputc _SFA_((Sfile_t*, unsigned char, int));
extern int		_sfputu _SFA_((Sfile_t*, unsigned long));
extern int		_sfputl _SFA_((Sfile_t*, long));
extern long		_sfgetl _SFA_((Sfile_t*));
extern unsigned long	_sfgetu _SFA_((Sfile_t*));
extern long		_sfgetl _SFA_((Sfile_t*));
extern int		_sfputd _SFA_((Sfile_t*, double));
extern double		sfgetd _SFA_((Sfile_t*));
extern int		sfungetc _SFA_((Sfile_t*, int));
extern char		*_sfcvt _SFA_((double, int, int*, int*, int));
extern int		sfprintf _SFA_((Sfile_t*, const char*, ...));
extern int		sfsprintf _SFA_((char*, int, const char*, ...));
extern int		sfscanf _SFA_((Sfile_t*, const char*, ...));
extern int		sfsscanf _SFA_((const char*, const char*, ...));

#if __STDC__ || __cplusplus
#include	<stdarg.h>
#endif
extern int		sfvprintf _SFA_((Sfile_t*, const char*, va_list));
extern int		sfvscanf _SFA_((Sfile_t*, const char*, va_list));

/* function analogues of fast in-line functions */
extern int		sfgetc _SFA_((Sfile_t*));
extern long		sfgetl _SFA_((Sfile_t*));
extern unsigned long	sfgetu _SFA_((Sfile_t*));
extern int		sfputc _SFA_((Sfile_t*,int));
extern int		sfputd _SFA_((Sfile_t*,double));
extern int		sfputl _SFA_((Sfile_t*,long));
extern int		sfputu _SFA_((Sfile_t*,unsigned long));
extern int		sfpeek _SFA_((Sfile_t*, unsigned char**));
extern int		sfsync _SFA_((Sfile_t*));
extern Sfile_t*		sfpushed _SFA_((Sfile_t*));
extern Sfile_t*		sftmp _SFA_((int));
extern int		sfslen _SFA_((void));
extern int		sfulen _SFA_((unsigned long));
extern long		sforigin _SFA_((Sfile_t*));
extern int		sfclearerr _SFA_((Sfile_t*));
extern int		sfeof _SFA_((Sfile_t*));
extern int		sferror _SFA_((Sfile_t*));
extern int		sffileno _SFA_((Sfile_t*));
extern char*		sfecvt _SFA_((double,int,int*,int*));
extern char*		sffcvt _SFA_((double,int,int*,int*));

#if __cplusplus
}
#endif

/* fast in-line functions */
#define sfputc(f,c)	((f)->next >= (f)->endw ? \
				_sfflsbuf(f,(int)((unsigned char)(c))) : \
				(int)(*(f)->next++ = (unsigned char)(c)))
#define sfgetc(f)	((f)->next >= (f)->endr ? _sffilbuf(f,1) : (int)(*(f)->next++))
#define sfslen()	(_Sfi)
#define sffileno(f)	((f)->file)
#define sforigin(f)	(((f)->flags&SF_STRING) ? 0L : (f)->orig)
#define sfeof(f)	(((f)->flags&(SF_EOF|SF_STRING)) && (f)->next >= (f)->endb)
#define sferror(f)	((f)->flags&SF_ERROR)
#define sfclearerr(f)	((f)->flags &= ~(SF_ERROR|SF_EOF))
#define sfpushed(f)	((f)->push)
#define sftmp(n)	((n) >= 0 ? _sftmpfile(n) : \
				    sfnew((Sfile_t*)0,(unsigned char*)0,-1,-1, \
					SF_READ|SF_WRITE|SF_STRING))

#define sfpeek(f,bufp)	(((bufp) || \
			  ((f)->flags&(SF_READ|SF_WRITE|SF_STRING)) == \
			   (SF_READ|SF_WRITE|SF_STRING)) ? _sfpeek(f,bufp) : \
				((f)->endb - (f)->next))
#define sfsync(f)	(((f) && (((Sfile_t*)(f))->flags&SF_STRING)) ? 0 : _sfsync(f))

/* coding long integers in a portable and compact fashion */
#define SF_SBITS	6
#define SF_UBITS	7
#define SF_SIGN		(1 << SF_SBITS)
#define SF_MORE		(1 << SF_UBITS)
#define SF_U1		SF_MORE
#define SF_U2		(SF_U1*SF_U1)
#define SF_U3		(SF_U2*SF_U1)
#define SF_U4		(SF_U3*SF_U1)
#define sfulen(v)	((v) < SF_U1 ? 1 : (v) < SF_U2 ? 2 : \
			 (v) < SF_U3 ? 3 : (v) < SF_U4 ? 4 : 5)
#define sfgetu(f)	((_Sfi = sfgetc(f)) < 0 ? -1 : \
				((_Sfi&SF_MORE) ? _sfgetu(f) : (unsigned long)_Sfi))
#define sfgetl(f)	((_Sfi = sfgetc(f)) < 0 ? -1 : \
				((_Sfi&(SF_MORE|SF_SIGN)) ? _sfgetl(f) : (long)_Sfi))
#define sfputu(f,v)	_sfputu((f),(unsigned long)(v))
#define sfputl(f,v)	_sfputl((f),(long)(v))
#define sfputd(f,v)	_sfputd((f),(double)(v))

#define sfecvt(v,n,d,s)	_sfcvt((v),(n),(d),(s),1)
#define sffcvt(v,n,d,s)	_sfcvt((v),(n),(d),(s),0)

#endif /* _SFIO_H */
