/*
 *	rdenv ( varname, result ) - read the environment variable named
 *		"varname", and if it can be read, put its value in "result"
 *		return 0 on success, -1 on failure
 */

#include "spitblks.h"

/* the environment pointer */
extern char **environ;

rdenv ( varname, result )
	register struct scblk *varname, *result;
{
	register n;
	register char *p, *q, **env;

	/* one iteration per environment variable */
	env = environ;
	while ( p = *env ) {
		q = varname -> str;
		n = varname -> len;

		/* match the variable against the environment */
		while ( n > 0 && *p == *q && *p != '\0' && *p != '=' ) {
			p++;
			q++;
			n--;
		}

		/* if successful, copy result and return */
		if ( *p == '=' && n == 0 ) {
			p++;
			q = result -> str;
			while ( *p != '\0' && n < result -> len ) {
				*q++ = *p++;
				n++;
			}
			result -> len = n;
			return 0;
		}

		env++;
	}

	return -1;
}
