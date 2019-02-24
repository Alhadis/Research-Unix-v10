#include	<stdio.h>
#include	"re.h"

void
re_error(char *s)
{
	fprintf(stderr, "pattern error: %s\n", s);
	exit(1);
	/* NOTREACHED */
}
