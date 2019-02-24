#include	"sfhdr.h"

/*	Set the IO pointer to a specific location in the stream
**
**	Written by Kiem-Phong Vo (06/27/90)
*/

#if __STDC__ || __cplusplus
long sfseek(reg Sfile_t* f, reg long p, reg int type)
#else
long sfseek(f,p,type)
reg Sfile_t	*f;	/* seek to a new location in this stream */
reg long	p;	/* place to seek to */
int		type;	/* 0: from org, 1: from here, 2: from end */
#endif
{
	reg long	r, s;
	reg long	(*seekf)_SFA_((Sfile_t*,long,int,char*));
	reg char	*handle;

	/* set the stream to the right mode */
	if((!f->mode || (f->mode&~(SF_RDWR|SF_STRING))) && _sfmode(f,0) < 0)
		return -1;

	if(type < 0 || type > 2 || f->orig < 0L)
		return -1;

	/* lock the stream for internal manipulations */
	SFLOCK(f);

	/* clear error and eof bits */
	f->flags &= ~(SF_EOF|SF_ERROR);

	if(f->mode&SF_STRING)
	{	/* actual current end of data */
		if((f->flags&SF_WRITE) &&
		   (s = f->next-f->data) > f->here && s != f->orig)
			f->here = s;

		/* new location */
		if(type == 1)
			p += f->next - f->data;
		else if(type == 2)
			p += f->here;

		if(p < 0 || p > f->size)
		{	p = -1;
			goto done;
		}

		f->next = f->data+p;
		f->orig = p;	/* use this to determine if writes occured */

		/* clear the part skipped over */
		if(p > f->here)
			memclear((char*)(f->data+f->here),(int)(p-f->here));

		goto done;
	}

	seekf = f->disc ? f->disc->seekf :
			NIL(long(*)_SFA_((Sfile_t*,long,int,char*)));
	handle = f->disc ? f->disc->handle : NIL(char*);

	/* currently known seek location */
	s = f->here + ((f->mode&SF_READ) ? f->next-f->endb : f->next-f->data);

	if(type == 1 && p == 0 && !(f->flags&(SF_APPEND|SF_SHARE)) )
	{	/* certify current location only */
		if((p = SEEKF(seekf,f,f->here,0,handle)) != f->here)
			p = -1;
		else	p = s - f->orig;
		goto done;
	}

	if(f->mode&SF_WRITE)
	{	if((f->flags&(SF_APPEND|SF_SHARE)) ||
		   (type == 0 ? p+f->orig : type == 1 ? p+s : -1) != s)
		{	/* sync physical and virtual views */
			SFopen(f);
			if(_sfflsbuf(f,-1) < 0)
			{	p = -1;
				goto done;
			}
			SFLOCK(f);
			s = f->here;
		}

		if(f->flags&SF_APPEND)
		{	/* the true location for a SF_APPEND stream */
			s = f->here = SEEKF(seekf,f,0L,2,handle);
			r = p + (type == 0 ? f->orig : s);
		}
		else if(!(f->flags&SF_SHARE))
			r = type == 0 ? p+f->orig : type == 1 ? p+s : -1;
		else	r = -1;
		if(r == s)
		{	/* at where we are supposed to be */
			p = r - f->orig;
			goto done;
		}
	}
	else if(type < 2 && !(f->flags&(SF_SHARE|SF_APPEND)) &&
		(r = p + (type == 0 ? f->orig : s)) > f->here)
	{	/* move forward by reading */
		if(f->next == f->endb)
			f->endb = f->endr = f->next = f->data;
		if(r < (s + (f->size - (f->endb-f->data))))
		{	SFopen(f);
			_sffilbuf(f,-1);
			SFLOCK(f);
		}
	}

	if(type < 2)
	{	/* place to seek to */	
		p += type == 0 ? f->orig : s;

		if((f->mode&SF_READ) && !(f->flags&(SF_SHARE|SF_APPEND)) &&
		   p >= (f->here - (f->endb-f->data)) && p <= f->here)
		{	/* still in bound, safe to just move the pointer */
			f->next = f->endb - (f->here-p);
			p -= f->orig;
			goto done;
		}

		/* now must seek */
		p = SEEKF(seekf,f,p,0,handle);
	}
	else	p = SEEKF(seekf,f,p,2,handle);

	/* moved backward too far */
	if(p >= 0 && p < f->orig)
	{	SEEKF(seekf,f,f->here,0,handle);
		p = -1;
	}
	if(p < 0)
		goto done;

	f->here = p;
	if(f->mode&SF_READ)
	{	/* any buffered data is invalid */
		f->next = f->endr = f->endb = f->data;

		/* seek to a rounded boundary to improve performance */
		if((s = f->size) > SF_GRAIN)
			s = SF_GRAIN;
		r = s > 1 ? (p/s)*s : p;
		if(r < p && r >= f->orig && SEEKF(seekf,f,r,0,handle) == r)
		{	/* now fill buffer */
			f->here = r;
			SFopen(f); (void) _sffilbuf(f,-1); SFLOCK(f);
			if(f->here < p)
			{	p = -1;
				goto done;
			}
			f->next = f->endb - (f->here-p);
		}
	}

	p -= f->orig;

done :
	SFOPEN(f);
	return p;
}
