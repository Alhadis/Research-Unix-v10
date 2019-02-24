/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * internal representation conversion support
 */

#include <swap.h>

/*
 * swap buffer b of n bytes according to swap op {BYTE,HALF} in place
 */

char*
memswap(op, b, n)
int		op;
char*		b;
register int	n;
{
	register char*	p;
	register int	c;

	p = b;
	switch (op)
	{
	case BYTE:
		n = (n + 1) / 2;
		while (n--)
		{
			c = *p;
			*p = *(p + 1);
			*(p + 1) = c;
			p += 2;
		}
		break;
	case HALF:
		n = (n + 3) / 4;
		while (n--)
		{
			c = *p;
			*p = *(p + 2);
			*(p + 2) = c;
			c = *(p + 3);
			*(p + 3) = *(p + 1);
			*(p + 1) = c;
			p += 4;
		}
		break;
	case BYTE|HALF:
		n = (n + 3) / 4;
		while (n--)
		{
			c = *p;
			*p = *(p + 3);
			*(p + 3) = c;
			c = *(p + 1);
			*(p + 1) = *(p + 2);
			*(p + 2) = c;
			p += 4;
		}
		break;
	}
	return(b);
}
