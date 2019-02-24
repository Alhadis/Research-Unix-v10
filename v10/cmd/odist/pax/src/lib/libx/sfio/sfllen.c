#include	"sfhdr.h"

/*	Get size of a long value coded in a portable format
**
**	Written by Kiem-Phong Vo (06/27/90)
*/
#if __STDC__ || __cplusplus
sfllen(reg long v)
#else
sfllen(v)
reg long	v;
#endif
{
	if(v < 0)
		v = -(v+1);
	v = (ulong)v >> SF_SBITS;
	return 1 + (v > 0 ? sfulen(v) : 0);
}
