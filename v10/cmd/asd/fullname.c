/*
 *	fullname -- return the full pathname corresponding to the
 *	abbreviated pathname given as argument.  Returned value
 *	is in a buffer that will stay around no longer than its
 *	argument or the next call to fullname, whichever is earlier.
 */

#include "asd.h"

char *
fullname (s)
	register char *s;
{
	register char *t;
	static char *r;
	static int size;
	register unsigned n;

	/* if first char is slash, absolute path */
	if (s[0] == '/')
		return s;
	
	/* strip leading './' */
	while (s[0] == '.' && s[1] == '/')
		s += 2;
	
	t = pwd();

	/* null string or "." means current directory */
	if (s[0] == '\0' || strcmp (s, ".") == 0)
		return t;
	
	n = strlen (s) + strlen (t) + 2;
	if (n > size) {
		r = ralloc (r, n);
		size = n;
	}

	strcpy (r, t);
	strcat (r, "/");
	strcat (r, s);

	return r;
}
