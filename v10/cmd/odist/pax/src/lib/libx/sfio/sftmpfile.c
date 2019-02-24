#include	"sfhdr.h"
#ifndef F_SETFD
#ifndef FIOCLEX
#ifdef V9
#include	<sys/filio.h>
#endif /*V9*/
#endif /*FIOCLEX*/
#endif /*F_SETFD*/


/*	Create a temporary stream for read/write.
**	The stream is originally created as a memory-resident stream.
**	When this memory is exceeded, a real temp file will be created.
**	The temp file creation sequence is somewhat convoluted so that
**	pool/stack/discipline will work correctly.
**
**	Written by David Korn and Kiem-Phong Vo (12/10/90)
*/

#if __STDC__ || __cplusplus
static _tmpexcept(Sfile_t* f, int type, char* hdl)
#else
static _tmpexcept(f,type,hdl)
Sfile_t	*f;
int	type;
char	*hdl;
#endif
{
	reg int		here, extent;
	Sfile_t		newf;

	/* do nothing cases */
	if(type == SF_EOF || type == SF_READ)
		return 0;

	/* get the extent of current data */
	here = extent = f->next-f->data;
	if(extent < f->here)
		extent = (int)(f->here);

	/* try to create the temp file */
	SFCLEAR(&newf);
	hdl = mktemp("/tmp/sfXXXXXXXX");
	if(!sfopen(&newf,hdl,"w+"))
		return -1;

	/* remove the temp file and set close-on-exec */
	while(unlink(hdl) < 0 && errno == EINTR)
		errno = 0;
#ifdef F_SETFD
	(void)fcntl(newf.file,F_SETFD,0);
#else
#ifdef FIOCLEX
	(void)ioctl(newf.file,FIOCLEX,0);
#endif
#endif

	/* now remake the old stream into the new image */
	f->disc = NIL(Sfdisc_t*);
	if(!sfnew(f,NIL(uchar*),-1,newf.file,SF_READ|SF_WRITE))
	{	while(close(newf.file) < 0 && errno == EINTR)
			errno = 0;
		return -1;
	}
	(void)sfwrite(f,f->data,extent);
	(void)sfseek(f,(long)here,0);

	return 1;
}

#if __STDC__ || __cplusplus
Sfile_t* _sftmpfile(int s)
#else
Sfile_t* _sftmpfile(s)
int	s;
#endif
{
	reg Sfile_t	*f;
	static Sfdisc_t	Tmpdisc;

	/* make this a memory resident stream at first */
	if(!(f = sfnew(NIL(Sfile_t*),NIL(uchar*),s,-1,SF_STRING|SF_READ|SF_WRITE)))
		return NIL(Sfile_t*);

	/* set up a discipline for out-of-bound, etc. */
	Tmpdisc.exceptf = _tmpexcept;
	sfsetdisc(f,&Tmpdisc);
	return f;
}
