#include	"sfhdr.h"

/*	Write out an unsigned long value in a portable format.
**
**	Written by Kiem-Phong Vo (06/27/90)
*/

#if __STDC__ || __cplusplus
_sfputu(reg Sfile_t* f, reg ulong v)
#else
_sfputu(f,v)
reg Sfile_t	*f;	/* write a portable ulong to this stream */
reg ulong	v;	/* the unsigned value to be written */
#endif
{
#define N_ARRAY		(2*sizeof(long))
	reg uchar	*s, *ps;
	reg int		n, p;
	uchar		c[N_ARRAY];

	if(f->mode != SF_WRITE && SFMODE(f,SF_WRITE) < 0)
		return -1;

	/* code v as integers in base SF_UBASE */
	s = ps = &(c[N_ARRAY-1]);
	*s = (uchar)SFUVALUE(v);
	while((v >>= SF_UBITS) > 0)
		*--s = (uchar)(SFUVALUE(v) | SF_MORE);
	n = (ps-s)+1;

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
