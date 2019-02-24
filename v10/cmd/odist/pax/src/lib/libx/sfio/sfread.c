#include	"sfhdr.h"

/*	Read n bytes from a tream into a buffer
**
**	Written by Kiem-Phong Vo (06/27/90)
*/

#if __STDC__ || __cplusplus
sfread(reg Sfile_t* f, reg uchar* s, reg int n)
#else
sfread(f,s,n)
reg Sfile_t	*f;	/* read from this stream */
reg uchar	*s;	/* buffer to read into */
reg int		n;	/* number of bytes to be read */
#endif
{
	reg uchar	*data;
	reg int		r;
	reg int		(*readf) _SFA_((Sfile_t*, uchar*, int, char*));
	reg long	(*seekf) _SFA_((Sfile_t*, long, int, char*));
	reg char	*handle;
	reg uchar	*begs = s;	/* true start of buffer */

redo_loop:
	if(f->mode != SF_READ && SFMODE(f,SF_READ) < 0)
		return -1;
	SFLOCK(f);
	if(s == f->next)
	{	/* fast read after sfpeek() */
		if((r = f->endb - f->next) > n)
			r = n <= 0 ? 0 : n;
		if(r > 0)
			f->next = (s += r);
		goto done;
	}

	readf = f->disc ? f->disc->readf :
			NIL(int(*)_SFA_((Sfile_t*, uchar*, int, char*)));
	seekf = f->disc ? f->disc->seekf :
			NIL(long(*)_SFA_((Sfile_t*, long, int, char*)));
	handle = f->disc ? f->disc->handle : NIL(char*);

	if((r = f->endb - (data = f->next)) > n)
		r = n <= 0 ? 0 : n;
	for(;;)
	{	/* copy data - note that s is advanced too */
		MEMCPY(s,data,r);

		f->next += r;
		if((n -= r) <= 0)
			goto done;

		if(f->flags&SF_STRING)
		{	r = 0;
			goto at_eof;
		}

		/* read directly into requested buffer */
		f->next = f->endb = f->endr = f->data;
		while(n >= (r = f->size))
		{	/* unbuffered read */
			if(r <= 1)
				r = n;
			else
			{	/* read by a round-up unit */
				if(r > SF_GRAIN)
					r = SF_GRAIN;
				r = (n/r)*r;
			}
			if(f->flags&SF_SHARE)
				f->here = SEEKF(seekf,f,f->here,0,handle);
			if((r = READF(readf,f,s,r,handle)) > 0)
			{
				f->here += r;
				s += r;
				if((n -= r) <= 0)
					goto done;
			}
			else
			{	/* eof or error */
			at_eof:
				switch(_sfexcept(f,SF_READ,r))
				{
				case SF_EDISC :
				case SF_ESTACK :
					SFopen(f);
					goto redo_loop;
				case SF_EDONE :
					goto done;
				default :
					continue;
				}
			}
		}

		/* fill our buffer and continue */
		SFopen(f);
		if((r = _sffilbuf(f,-1)) <= 0)
			goto done;
		SFLOCK(f);
		if(r > n)
			r = n;
		data = f->next;
	}
done :
	SFOPEN(f);
	return s-begs;
}
