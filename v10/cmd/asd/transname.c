/*
 *	transname - take a pathname, expand it, and translate it
 *	according to the replist.  The result is in a buffer
 *	that will stay around no longer than the next call
 *	to transname.  If no translation, transname will
 *	just return fullname applied to its argument.
 */

#include "asd.h"

char *
transname (s)
	register char *s;
{
	register struct replist *rl;
	static char *res;
	static int size;
	register int n;

	s = fullname (s);

	/* look for substitution */
	for (rl = replist; rl; rl = rl->link) {
		register char *p = rl->source;
		register char *q = s;

		/* comparison loop */
		while (*p != '\0' && *p == *q) {
			p++;
			q++;
		}

		if (*p == '\0') {
			/* comparison successful */
			n = strlen (rl->dest) + strlen (q) + 1;
			if (n > size) {
				size = n;
				res = ralloc (res, (unsigned) n);
			}
			strcpy (res, rl->dest);
			strcat (res, q);
			return res;
		}
	}

	/* search loop failed, return expanded input */
	return s;
}
