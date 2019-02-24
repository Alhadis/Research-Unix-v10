#include	"sfhdr.h"

/*	Write data out to the file system
**
**	Written by Kiem-Phong Vo (06/27/90)
*/

#if __STDC__ || __cplusplus
sfwrite(reg Sfile_t* f, reg const uchar* s, reg int n)
#else
sfwrite(f,s,n)
reg Sfile_t	*f;	/* write to this stream */
reg uchar	*s;	/* buffer to be written */
reg int		n;	/* number of bytes */
#endif
{
	reg uchar	*data;
	reg int		w;
	reg int		line_fls;
	reg int		(*writef)_SFA_((Sfile_t*, const uchar*, int, char*));
	reg long	(*seekf)_SFA_((Sfile_t*, long, int, char*));
	reg char	*handle;
#if __STDC__ || __cplusplus
	reg const uchar	*begs;
#else
	reg uchar	*begs;
#endif
	begs = s;

redo_loop:
	if(f->mode != SF_WRITE && SFMODE(f,SF_WRITE) < 0)
		return -1;
	SFLOCK(f);

	writef = f->disc ? f->disc->writef :
			NIL(int(*)_SFA_((Sfile_t*,const uchar*,int,char*)));
	seekf  = f->disc ? f->disc->seekf  :
			NIL(long(*)_SFA_((Sfile_t*, long, int, char*)));
	handle = f->disc ? f->disc->handle : NIL(char*);

	/* see if must flush because of line buffering */
	line_fls = 0;
	if((f->flags&SF_LINE) && !(f->flags&SF_STRING))
	{	if(n >= HIFORLINE)
			line_fls = 1;
		else for(data = (uchar*)s+n-1; data >= (uchar*)s;)
			if(*data-- == '\n')
			{	line_fls = 1;
				break;
			}
	}

	if((uchar*)s == (data = f->next))
	{	/* user wrote directly into this buffer */
		if((w = f->endb - data) > n)
			w = n <= 0 ? 0 : n;
		f->next += w;
		s += w;
		if((n -= w) <= 0)
			goto done;
		data += w;
	}

	if((w = f->endb - data) == f->size && w <= n && !(f->mode&SF_STRING))
		goto do_write;
	else if(w > n)
		w = n;
	for(;;)
	{	/* copy data - note that s is advanced too */
		MEMCPY(data,s,w);

		f->next += w;
		if((n -= w) <= 0)
			goto done;

		if(f->mode&SF_STRING)
		{	w = n;
			goto at_eof;
		}

		if(f->next > f->data)
		{	/* clear buffer before direct writes */	
			SFopen(f);
			if(_sfflsbuf(f,-1) < 0)
				goto done;
			SFLOCK(f);
		}

	do_write :
		while(n >= (w = f->size) || line_fls)
		{
			if(w <= 1 || line_fls)	/* unbuffered write */
				w = n;
			else
			{	/* write using a round-up unit */
				if(w > SF_GRAIN)
					w = SF_GRAIN;
				w = (n/w)*w;
			}

#ifdef NO_OFLAGS	/* simulate append mode */
			if(f->flags&SF_APPEND)
				f->here = SEEKF(seekf,f,0L,2,handle);
			else /* not really dangling */
#endif
			if(f->flags&SF_SHARE)
				f->here = SEEKF(seekf,f,f->here,0,handle);

			if((w = WRITEF(writef,f,s,w,handle)) > 0)
			{	/* successful write */
				f->here += w;
				s += w;
				if((n -= w) <= 0)
					goto done;
			}
			else
			{	/* a write exception occured */
			at_eof :
				switch(_sfexcept(f,SF_WRITE,w))
				{
				case SF_EDISC :
				case SF_ESTACK :
					SFopen(f);
					goto redo_loop;
				case SF_EDONE :
					goto done;
				default :
					if(f->mode&SF_STRING)
						goto set_copy;
					else	continue;
				}
			}
		}

		/* prepare to copy data into buffer */
	set_copy:
		data = f->next;
		if((w = f->endb - data) > n)
			w = n;
	}
done :
	SFOPEN(f);
	if(line_fls && f->next > f->data)
		_sfflsbuf(f,-1);
	return s-begs;
}
