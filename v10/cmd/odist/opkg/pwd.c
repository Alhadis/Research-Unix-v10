/*
 *	pwd() - return the name of the current directory.
 *	We assume that the current directory never changes.
 */

#include "asd.h"
#include <libv.h>
#include <stdio.h>

#define CHUNK 64

char *
pwd(void)
{
	register FILE *f;
	static char *r;
	static unsigned size;
	register int n, c;

	if (r == NULL) {
		f = popen ("pwd", "r");
		schk ((char *) f);
		
		n = 0;
		while ((c = getc (f)) != EOF) {
			if (n >= size) {
				size += CHUNK;
				r = ralloc (r, size);
			}
			r[n++] = c;
		}

		/* replace the trailing newline by a null */
		r[n - 1] = '\0';

		pclose (f);
	}

	return r;
}
