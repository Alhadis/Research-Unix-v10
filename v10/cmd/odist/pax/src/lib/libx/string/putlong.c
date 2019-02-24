/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * internal representation conversion support
 */

#include <swap.h>
#include <limits.h>

/*
 * put a long into buffer p according to swap op {HALF,LONG}
 */

#define PUTLONG(p,n,B0,B1,B2,B3) (p[B0]=n>>(CHAR_BIT*3),p[B1]=n>>(CHAR_BIT*2),p[B2]=n>>(CHAR_BIT),p[B3]=n)

void
putlong(op, p, n)
register int	op;
register char*	p;
register long	n;
{
	switch (op)
	{
	case 0:
		PUTLONG(p, n, 0, 1, 2, 3);
		break;
	case BYTE:
		PUTLONG(p, n, 1, 0, 3, 2);
		break;
	case HALF:
		PUTLONG(p, n, 2, 3, 0, 1);
		break;
	case BYTE|HALF:
		PUTLONG(p, n, 3, 2, 1, 0);
		break;
	}
}
