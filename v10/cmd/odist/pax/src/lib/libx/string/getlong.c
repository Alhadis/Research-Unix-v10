/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * internal representation conversion support
 */

#include <swap.h>
#include <limits.h>

/*
 * construct a long from buffer p according to swap op {HALF,LONG}
 */

#define GETLONG(p,B0,B1,B2,B3)	((((long)p[B0])<<(CHAR_BIT*3))|(((long)p[B1])<<(CHAR_BIT*2))|(((long)p[B2])<<(CHAR_BIT))|(((long)p[B3])))

#if __STDC__ || __cplusplus
long
getlong(register int op, const char* p)
#else
long
getlong(op, p)
register int	op;
char*		p;
#endif
{
	register unsigned char*	b;

	b = (unsigned char*)p;
	switch (op)
	{
	case 0:
		return(GETLONG(b, 0, 1, 2, 3));
	case BYTE:
		return(GETLONG(b, 1, 0, 3, 2));
	case HALF:
		return(GETLONG(b, 2, 3, 0, 1));
	case BYTE|HALF:
		return(GETLONG(b, 3, 2, 1, 0));
	}
	return(0);
}
