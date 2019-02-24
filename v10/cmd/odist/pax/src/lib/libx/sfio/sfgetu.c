#include	"sfhdr.h"

/*	Read an unsigned long value coded in a portable format.
**
**	Written by Kiem-Phong Vo (06/27/90)
*/

#if __STDC__ || __cplusplus
ulong _sfgetu(reg Sfile_t* f)
#else
ulong _sfgetu(f)
reg Sfile_t	*f;
#endif
{
	reg uchar	*s, *ends, c;
	reg int		p;
	reg ulong	v;

	if(f->mode != SF_READ && SFMODE(f,SF_READ) < 0)
		return -1L;

	SFLOCK(f);
	v = SFUVALUE(_Sfi);
	for(;;)
	{
		if(SFRPEEK(f,s,p) <= 0)
		{	f->flags |= SF_ERROR;
			v = (ulong)(-1);
			goto done;
		}
		for(ends = s+p; s < ends;)
		{
			c = *s++;
			v = (v << SF_UBITS) | SFUVALUE(c);
			if(!(c&SF_MORE))
			{
				f->next = s;
				goto done;
			}
		}
		f->next = s;
	}
done:
	SFOPEN(f);
	return v;
}
