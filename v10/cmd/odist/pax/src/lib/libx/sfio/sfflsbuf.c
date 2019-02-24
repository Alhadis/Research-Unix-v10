#include	"sfhdr.h"

/*	Write a buffer out to a file descriptor or
**	extending a buffer for a SF_STRING stream.
**
**	Written by Kiem-Phong Vo (06/27/90)
*/

#if __STDC__ || __cplusplus
_sfflsbuf(reg Sfile_t* f, reg int c)
#else
_sfflsbuf(f,c)
reg Sfile_t	*f;	/* write out the buffered content of this stream */
reg int		c;	/* if c>=0, c is also written out */ 
#endif
{
	reg int		n, w;
	reg uchar	*data;
	reg int		(*writef) _SFA_((Sfile_t*, const uchar*, int, char*));
	reg long	(*seekf) _SFA_((Sfile_t*, long, int, char*));
	reg char	*handle;
	uchar		outc;
	int		inpc = c;

redo_loop:
	if(f->mode != SF_WRITE && SFMODE(f,SF_WRITE) < 0)
		return -1;
	SFLOCK(f);

	writef = f->disc ? f->disc->writef :
			NIL(int(*)_SFA_((Sfile_t*, const uchar*, int, char*)));
	seekf  = f->disc ? f->disc->seekf  :
			NIL(long(*)_SFA_((Sfile_t*, long, int, char*)));
	handle = f->disc ? f->disc->handle : NIL(char*);

	n = f->next - (data = f->data);
	if(n >= f->size && (f->mode&SF_STRING))
	{	/* at eof */
		w = 0;
		goto at_eof;
	}

	if(c >= 0)
	{	/* there is still space in the buffer */
		if(n < f->size)
		{	*f->next++ = c;
			w = 1;
			if(c != '\n' || (f->flags&SF_STRING) || !(f->flags&SF_LINE))
				goto done;
			n += 1;
			c = -1;
		}
		else if(n == 0)
		{	/* unbuffered io */
			outc = (uchar)c;
			data = &outc;
			n = 1;
			c = -1;
		}
	}
	else if(n <= 0)
	{	/* no op */
		w = 0;
		goto done;
	}

	/* write data to file */
	f->next = f->data;
	for(;;)
	{	/* make sure we are at the right place before writing */
#ifdef NO_OFLAGS /* simulate append mode */
		if(f->flags&SF_APPEND)
			f->here = SEEKF(seekf,f,0L,2,handle);
		else /* no, it doesn't dangle */
#endif
		if(f->flags&SF_SHARE)
			f->here = SEEKF(seekf,f,f->here,0,handle);

		if((w = WRITEF(writef,f,data,n,handle)) > 0)
		{	/* successful write */
			f->here += w;
			if((n -= w) > 0)
				data += w;
			else if(c < 0)
				goto done;
			else if(c == '\n' && (f->flags&SF_LINE))
			{	/* line buffering */
				outc = '\n';
				data = &outc;
				n = 1;
				c = -1;
			}
			else
			{	*f->next++ = c;
				goto done;
			}
		}
		else
		{
		at_eof :
			if(data == &outc)
				c = outc;
			else
			{	/* save unwritten data */
				if(data != f->data)
					memcpy((char*)f->data,(char*)data,n);
				f->next = f->data+n;
			}

			switch(_sfexcept(f,SF_WRITE,w))
			{
			default :
			case SF_EDISC :
			case SF_ESTACK :
				SFopen(f);
				goto redo_loop;
			case SF_EDONE :
				if(c >= 0 && f->next < f->endb)
					*f->next++ = c;
				w = -1;
				goto done;
			}
		}
	}
done :
	SFOPEN(f);
	return (inpc < 0 || w < 0) ? w : inpc;
}
