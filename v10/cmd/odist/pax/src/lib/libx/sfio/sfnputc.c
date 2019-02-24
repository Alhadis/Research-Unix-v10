#include	"sfhdr.h"

/*	Write out a character n times
**
**	Written by Kiem-Phong Vo (06/27/90)
*/

#if __STDC__ || __cplusplus
sfnputc(reg Sfile_t* f, reg uchar c, reg int n)
#else
sfnputc(f,c,n)
reg Sfile_t	*f;	/* file to write */
reg uchar	c;	/* char to be written */
reg int		n;	/* number of time to repeat */
#endif
{
	reg uchar	*ps;
	reg int		p, w;
	uchar		buf[128];

	if(f->mode != SF_WRITE && SFMODE(f,SF_WRITE) < 0)
		return -1;

	SFLOCK(f);

	/* write into a suitable buffer */
	if((p = (f->endb-(ps = f->next))) < n)
		{ ps = buf; p = sizeof(buf); }
	if(p > n)
		p = n;
	MEMSET(ps,c,p);
	ps -= p;

	w = n;
	if(ps == f->next)
	{	/* simple sfwrite */
		f->next += p;
		if(c == '\n')
			{ SFopen(f); _sfflsbuf(f,-1); }
		goto done;
	}

	for(;;)
	{	/* hard write of data */
		SFopen(f);
		if((p = sfwrite(f,ps,p)) <= 0 || (n -= p) <= 0)
		{	w -= n;
			goto done;
		}
		SFLOCK(f);
		if(p > n)
			p = n;
	}
done :
	SFOPEN(f);
	return w;
}
