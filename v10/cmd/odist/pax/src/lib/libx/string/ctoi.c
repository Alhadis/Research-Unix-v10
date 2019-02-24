/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * convert a 0 terminated character constant string to an int
 */

#include <limits.h>

extern int	chresc(/* char* string, char** next */);

int
ctoi(s)
register char*	s;
{
	register int	c;
	register int	n;
	register int	x;
	char*		p;

	c = 0;
	for (n = 0; n < sizeof(int) * CHAR_BIT; n += CHAR_BIT)
	{
		switch (x = *s++)
		{
		case '\\':
			x = chresc(s - 1, &p);
			s = p;
			break;
		case 0:
			return(c);
		}
		c |= x << n;
	}
	return(c);
}
