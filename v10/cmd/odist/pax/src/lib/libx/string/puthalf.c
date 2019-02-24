/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * internal representation conversion support
 */

#include <swap.h>
#include <limits.h>

/*
 * put a short into buffer p according to swap op {HALF,LONG}
 */

#define PUTHALF(p,n,B0,B1)	(p[B0]=n>>(CHAR_BIT),p[B1]=n)

void
puthalf(op, p, n)
register int	op;
register char*	p;
register int	n;
{
	switch (op)
	{
	case 0:
	case HALF:
		PUTHALF(p, n, 0, 1);
		break;
	case BYTE:
	case BYTE|HALF:
		PUTHALF(p, n, 1, 0);
		break;
	}
}
