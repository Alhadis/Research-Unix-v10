#include	"sfhdr.h"

/*	Fundamental function to create a new stream.
**	The argument flags defines the type of stream and the scheme
**	of buffering.
**
**	Written by Kiem-Phong Vo (06/27/90)
*/

/* to be done on exit */
static int	_Cleanup = 0;
static void _cleanup()
{	/* sync all streams */
	if(_Cleanup >= 0)
	{	_Cleanup = -1;
		sfsync(NIL(Sfile_t*));
	}
}

#if __STDC__ || __cplusplus
Sfile_t *sfnew(Sfile_t* oldf, uchar* buf, int size, int file, int flags)
#else
Sfile_t *sfnew(oldf,buf,size,file,flags)
Sfile_t	*oldf;	/* old stream to be reused */
uchar	*buf;	/* a buffer to read/write, if NULL, will be allocated */
int	size;	/* buffer size if buf is given or desired buffer size */
int	file;	/* file descriptor to read/write from */
int	flags;	/* type of file stream */
#endif
{
	reg Sfile_t	*f;
	reg long	(*seekf)_SFA_((Sfile_t*,long,int,char*));
	reg char	*handle;

	if(!_Cleanup)
	{	_Cleanup = 1;
		(void)atexit(_cleanup);
	}

	if((oldf && SFFROZEN(oldf)) || (!oldf && !(flags&SF_RDWR)))
		return NIL(Sfile_t*);

	/* make a stream structure */
	if(oldf)
	{	f = oldf;
		if(flags&SF_REUSE)
			flags = f->flags;
		if((f->mode&SF_STRING) && f->data && (f->flags&SF_MALLOC))
		{	free((char*)f->data);
			f->data = f->endb = f->next = NIL(uchar*);
			f->size = 0;
		}
	}
	else
	{	if(file >= 0 && file <= 2 && !(flags&SF_STRING))
		{	/* reuse a sfstd-stream if possible */
			f = file == 0 ? sfstdin : file == 1 ? sfstdout : sfstderr;
			if(f->file >= 0 ||
			   (f == sfstdin && (flags&SF_RDWR) != SF_READ) ||
			   (f != sfstdin && (flags&SF_RDWR) != SF_WRITE))
				f = NIL(Sfile_t*);
		}
		else	f = NIL(Sfile_t*);

		if(!f && !SFALLOC(f))
			return NIL(Sfile_t*);
		SFCLEAR(f);
	}

	/* stream type */
	f->mode = (flags&SF_STRING) ? SF_STRING :
		  ((flags&SF_WRITE) && oldf && (oldf->mode&SF_WRITE)) ? SF_WRITE :
		  (flags&SF_READ) ? SF_READ : SF_WRITE;
	f->flags = (flags&SF_FLAGS)&~SF_REUSE;
	if((f->flags&SF_RDWR) == SF_RDWR)
		f->flags |= SF_BOTH;

	if(f->mode&SF_STRING)
	{	/* note that f->here is set in sfsetbuf() */
		f->file = -1;
		f->orig = 0L;
	}
	else
	{	SFLOCK(f);
		f->file = file;
		f->orig = 0L;

		if((f == sfstdin || f == sfstdout || f == sfstderr) &&
		   !f->data && isatty(file))
		{	/* this stream is not seekable */
			f->orig = -1L;
			if(f != sfstdin)
				f->flags |= SF_LINE;
		}

		if(f->orig == 0L)
		{	/* set seek location */
			seekf = (oldf && oldf->disc) ? oldf->disc->seekf :
					NIL(long(*)_SFA_((Sfile_t*,long,int,char*)));
			handle = (oldf && oldf->disc) ? oldf->disc->handle : NIL(char*);
			f->orig = SEEKF(seekf,f,0L,(flags&SF_APPEND) ? 2 : 1,handle);
			if(f->orig > 0 && !(flags&SF_RELATIVE))
				f->orig = 0L;
		}
		f->here = f->orig > 0L ? f->orig : 0L;

		if(!f->data && !buf && size < 0)
		{	/* special cases for stream buffer size */
			if(f == sfstderr)
				size = 0;
			else if(f->orig < 0L && (f->mode&SF_WRITE) && (f->flags&SF_LINE))
				size = SF_GRAIN;
			else	size = SF_BUFSIZE;
		}
	}

	/* set buffer pointers */
	SFOPEN(f);
	sfset(f,f->flags,1);
	if(!(flags&SF_STRING) && oldf && size < 0)
	{	buf = f->data;
		size = f->size;
		f->data = f->next = f->endr = f->endw = f->endb = NIL(uchar*);
		f->size = 0;
	}
	sfsetbuf(f,buf,size);

	if(!oldf)
	{	/* link to the list of active streams */
		f->pool = &_Sfpool;
		f->fore = _Sfpool.head;
		if(_Sfpool.head)
			_Sfpool.head->back = f;
		_Sfpool.head = f;
	}

	if(_Sfnotify)
		(*_Sfnotify)(f,0);

	return f;
}
