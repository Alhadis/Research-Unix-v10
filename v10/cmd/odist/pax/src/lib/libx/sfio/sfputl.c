#include	"sfhdr.h"

/*	Write out a long value in a portable format
**
**	Written by Kiem-Phong Vo (06/27/90)
*/

#if __STDC__ || __cplusplus
_sfputl(reg Sfile_t* f, reg long v)
#else
_sfputl(f,v)
reg Sfile_t	*f;	/* write a portable long to this stream */
reg long	v;	/* the value to be written */
#endif
{
#define N_ARRAY		(2*sizeof(long))
	reg uchar	*s, *ps;
	reg int		n, p;
	uchar		c[N_ARRAY];

	if(f->mode != SF_WRITE && SFMODE(f,SF_WRITE) < 0)
		return -1;

	s = ps = &(c[N_ARRAY-1]);
	if(v < 0)
	{	/* add 1 to avoid 2-complement problems with -MAXINT */
		v = -(v+1);
		*s = (uchar)(SFSVALUE(v) | SF_SIGN);
	}
	else	*s = (uchar)(SFSVALUE(v));
	v = (ulong)v >> SF_SBITS;

	while(v > 0)
	{
		*--s = (uchar)(SFUVALUE(v) | SF_MORE);
		v = (ulong)v >> SF_UBITS;
	}
	n = (ps-s)+1;

	/* write the hard way */
	if(n > 7 || SFWPEEK(f,ps,p) < n)
		return sfwrite(f,s,n);

	SFLOCK(f);
	switch(n)
	{
	case 7 : *ps++ = *s++;
	case 6 : *ps++ = *s++;
	case 5 : *ps++ = *s++;
	case 4 : *ps++ = *s++;
	case 3 : *ps++ = *s++;
	case 2 : *ps++ = *s++;
	case 1 : *ps++ = *s++;
	}
	f->next = ps;
	SFOPEN(f);

	return n;
}
