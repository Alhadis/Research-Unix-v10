#include	"sfhdr.h"

#ifdef LIBX
#include	<fsinfo.h>
#else
#ifdef BSD
#define	 stat_blksize
#endif
#endif

/*	Set a (new) buffer for a stream.
**	If size < 0, it is assigned a suitable value depending on the
**	kind of stream. The actual buffer size allocated is dependent
**	on how much memory is available.
**
**	Written by Kiem-Phong Vo (06/27/90)
*/

#if __STDC__ || __cplusplus
uchar *sfsetbuf(reg Sfile_t* f, reg uchar* buf, reg int size)
#else
uchar *sfsetbuf(f,buf,size)
reg Sfile_t	*f;	/* stream to be buffered */
reg uchar	*buf;	/* new buffer, !NULL if size > 0 */
reg int		size;	/* buffer size, -1 for default size */
#endif
{
	reg int		sf_malloc;
	reg uchar	*obuf;

	if(!f->mode)
	{	/* a std-stream */
		obuf = f->data;
		(void)sfnew(f,buf,size,f->file,f->flags);
		return obuf;
	}

	/* set the stream to the right mode */
	if((f->mode&~(SF_RDWR|SF_STRING)) && _sfmode(f,0) < 0)
		return NIL(uchar*);

	/* pure read/string streams must have a valid string */
	if((f->flags&(SF_RDWR|SF_STRING)) == SF_RDSTR && (size < 0 || !buf))
		size = 0;

	/* sync stream before switching buffer */
	if((f->mode&SF_READ) && f->next < f->endb)
		sfsync(f);

	/* lock for internal work */
	SFLOCK(f);

	/* get buffer space */
	sf_malloc = 0;
	if(size < 0)
	{	/* define a default size suitable for block transfer */
		buf = NIL(uchar*);
		if(f->mode&SF_STRING)
			size = SF_GRAIN;
		else
		{	/* try to set an optimal size for io */
			struct stat	sb;
			if(fstat(f->file,&sb) < 0)
				size = SF_GRAIN;
			else
			{	if(f->mode&SF_READ)
				{	/* maybe only need small buffer */
					if((size = (int)sb.st_size) > SF_BUFSIZE)
						size = SF_BUFSIZE;
					size = ((size+SF_GRAIN-1)/SF_GRAIN)*SF_GRAIN;
				}
#ifdef stat_blksize
				if(size <= 0 || size > sb.st_blksize)
					size = (int)sb.st_blksize;
#endif
				if(size <= 0)
					size = SF_BUFSIZE;
			}
		}
	}
	if(size > 0 && !buf)
	{	/* try to allocate it */
		while(!(buf = (uchar*) malloc(size)) && size > 0)
			size /= 2;
		if(size > 0)
			sf_malloc = SF_MALLOC;
		else if(f->mode&SF_STRING)
		{	SFOPEN(f);
			return NIL(uchar*);
		}
	}
	if(size == 0 && (f->mode&SF_READ))
	{	/* use the internal buffer */
		size = sizeof(f->tiny);
		buf = f->tiny;
		sf_malloc = 0;
	}

	/* current buffer */
	obuf = f->data;

	/* set up new buffer */
	f->size = size;
	f->next = f->data = f->endr = f->endw = buf;
	f->endb = (f->mode&SF_READ) ? buf : buf+size;
	if(f->mode&SF_STRING)
	{
		f->orig = 0L;
		f->here = sf_malloc ? 0L : size;
	}

	if(obuf && obuf != buf && (f->flags&SF_MALLOC))
	{	free((char*)obuf);
		obuf = NIL(uchar*);
	}

	f->flags = (f->flags & ~SF_MALLOC)|sf_malloc;

	SFOPEN(f);
	return obuf;
}
