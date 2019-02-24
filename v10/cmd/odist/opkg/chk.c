#include "asd.h"
#include <stdio.h>

/*
 *	little subroutines to check return codes
 */

void
nchk (int n)
{
	if (n < 0) {
		fprintf (stderr, "unexpected error return -- help!\n");
		exit (1);
	}
}

void
schk (void *s)
{
	if (s == NULL) {
		fprintf (stderr, "unexpected error return -- help!\n");
		exit (1);
	}
}
