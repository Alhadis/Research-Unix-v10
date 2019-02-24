#include	"sfhdr.h"

/*	Return the portion of the internal stream buffer available
**	for reading/writing. If a stream is SF_READ and the buffer is
**	empty, it will be filled. If a stream is SF_WRITE and the buffer
**	full, it will be flushed.
**	A peek on a SF_READ stream that is line buffered will return the
**	portion of data up to \n.
**	Note also that this function does not advance the buffer pointer.
**	That must be done by an appropriate sfread or sfwrite call.
**
**	Written by Kiem-Phong Vo (06/27/90).
*/

#if __STDC__ || __cplusplus
_sfpeek(reg Sfile_t* f, uchar** bp)
#else
_sfpeek(f,bp)
reg Sfile_t	*f;	/* file to peek */
uchar		**bp;	/* start of data area */
#endif
{
	reg int		n;
	reg uchar	*s, *ends;
	reg int		c, o, a;

	/* for consistency the desired mode for a RDWR stream is always READ */
	n = (f->flags&SF_RDWR) == SF_RDWR ? SF_READ : (f->mode&SF_RDWR);
	if(f->mode != n && SFMODE(f,n) < 0)
		return -1;

	SFLOCK(f);
	if((f->flags&(SF_RDWR|SF_STRING)) == (SF_RDWR|SF_STRING))
	{	/* a read/write string stream, update current end */
		if((o = f->next-f->data) > f->here && o != f->orig)
			f->here = f->orig = o;
		n = (f->data+f->here)-f->next;
		if(!bp)
			goto done;
	}
	else if((n = f->endb - f->next) == 0)
	{	SFopen(f);
		if(f->flags&SF_READ)
			_sffilbuf(f,-1);
		else	_sfflsbuf(f,-1);
		SFLOCK(f);
		n = f->endb - f->next;
	}

	if(n <= 0 || (f->mode&SF_WRITE) || !(f->flags&SF_READ) || !(f->flags&SF_LINE))
	{	/* normal business */
		*bp = f->next;
		goto done;
	}

	/* in line-buffering read mode, find \n */
	n = (ends = f->endb) - (s = f->next);
	if((f->flags&SF_STRING) && !(f->flags&SF_WRITE))
	{	/* must do this slowly */
		while(s < ends && *s++ != '\n')
			;
		c = s[-1];
	}
	else
	{	/* fast loop */
		c = ends[-1]; ends[-1] = '\n';
		while(*s++ != '\n')
			;
		ends[-1] = c;
	}

	/* we are done */
	if(s < ends || c == '\n' || (f->flags&SF_STRING))
	{	n = s - (*bp = f->next);
		goto done;
	}

	/* must read more, try to align data by moving it back */
	if((f->mode&SF_READ) && (o = f->next - f->data) > 0)
	{	/* shift left data, make sure alignment is correct */
		a = f->size >= (2*SF_GRAIN) ? SF_GRAIN :
			((f->size/2)/sizeof(void*)+1)*sizeof(void*);
		if((c = (n/a + 1)*a - n) > o)
		{	n = ends - (*bp = f->next);
			goto done;
		}
		else if(c < o)
		{	/* copy data */
			ends = f->next;
			f->next = s = f->data+c;
			f->endb = s+n;
			MEMCPY(s,ends,n);
		}
	}

	SFopen(f);
	n = _sffilbuf(f,0);
	*bp = f->next;

done:
	SFOPEN(f);
	return n;
}
