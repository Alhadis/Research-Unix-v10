#include	"sfhdr.h"

/*	Return the length of a double value if coded in a portable format
**
**	Written by Kiem-Phong Vo (08/05/90)
*/

#if __STDC__ || __cplusplus
sfdlen(reg double v)
#else
sfdlen(v)
reg double	v;
#endif
{
#define N_ARRAY		(16*sizeof(double))
	reg int		n, w;
	reg double	x;
	int		exp;

	if(v < 0)
		v = -v;

	/* make the magnitude of v < 1 */
	if(v != 0.)
		v = frexp(v,&exp);
	else	exp = 0;

	for(w = 1; w <= N_ARRAY; ++w)
	{	/* get 2^SF_PRECIS precision at a time */
		n = (int)(x = ldexp(v,SF_PRECIS));
		if((v = x-n) <= 0.)
			break;
	}

	return 1 + sfulen(exp) + w;
}
