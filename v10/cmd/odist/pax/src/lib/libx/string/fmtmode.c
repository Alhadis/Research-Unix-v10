/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * return ls -l style file mode string given file mode bits
 */

#include "modelib.h"

char*
fmtmode(mode)
register int	mode;
{
	register char*		s;
	register struct modeop*	p;

	static char		buf[MODELEN + 1];

	s = buf;
	for (p = modetab; p < &modetab[MODELEN]; p++)
		*s++ = p->name[((mode & p->mask1) >> p->shift1) | ((mode & p->mask2) >> p->shift2)];
	return(buf);
}
