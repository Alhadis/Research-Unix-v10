#include	"sfhdr.h"

/*	Read a portably coded double value
**
**	Written by Kiem-Phong Vo (08/05/90)
*/

#if __STDC__ || __cplusplus
double sfgetd(Sfile_t* f)
#else
double sfgetd(f)
Sfile_t	*f;
#endif
{
	reg uchar	*s, *ends, c;
	reg double	v;
	reg int		p, sign, exp;

	if((sign = sfgetc(f)) < 0 || (exp = (int)sfgetu(f)) < 0)
		return -1.;

	SFLOCK(f);

	v = 0.;
	for(;;)
	{	/* fast read for data */
		if(SFRPEEK(f,s,p) <= 0)
		{	f->flags |= SF_ERROR;
			v = -1.;
			goto done;
		}

		for(ends = s+p; s < ends; )
		{
			c = *s++;
			v += SFUVALUE(c);
			v = ldexp(v,-SF_PRECIS);
			if(!(c&SF_MORE))
			{	f->next = s;
				goto done;
			}
		}
		f->next = s;
	}

done:
	v = ldexp(v,(sign&02) ? -exp : exp);
	if(sign&01)
		v = -v;
	SFOPEN(f);
	return v;
}
