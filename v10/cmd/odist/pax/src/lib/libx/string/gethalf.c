/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * internal representation conversion support
 */

#include <swap.h>
#include <limits.h>

/*
 * construct a short from buffer p according to swap op {BYTE,HALF}
 */

#define GETHALF(p,B0,B1)	((p[B0]<<(CHAR_BIT))|(p[B1]))

#if __STDC__ || __cplusplus
int
gethalf(register int op, const char* p)
#else
int
gethalf(op, p)
register int	op;
char*		p;
#endif
{
	register unsigned char*	b;

	b = (unsigned char*)p;
	switch (op)
	{
	case 0:
	case HALF:
		return(GETHALF(b, 0, 1));
	case BYTE:
	case BYTE|HALF:
		return(GETHALF(b, 1, 0));
	}
	return(0);
}
