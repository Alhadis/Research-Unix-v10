#include <stdio.h>

/*
 *	little subroutines to check return codes
 */

nchk (n)
	register int n;
{
	if (n < 0) {
		fprintf (stderr, "unexpected error return -- help!\n");
		exit (1);
	}
}

schk (s)
	register char *s;
{
	if (s == NULL) {
		fprintf (stderr, "unexpected error return -- help!\n");
		exit (1);
	}
}
