/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * return file mode bits given ls -l style file mode string
 */

#include "modelib.h"

int
strmode(s)
register char*	s;
{
	register int		c;
	register char*		t;
	register struct modeop*	p;
	int			mode;

	mode = 0;
	for (p = modetab; (c = *s++) && p < &modetab[MODELEN]; p++)
		for (t = p->name; *t; t++)
			if (*t == c)
			{
				c = t - p->name;
				mode |= (p->mask1 & (c << p->shift1)) | (p->mask2 & (c << p->shift2));
				break;
			}
	return(mode);
}
