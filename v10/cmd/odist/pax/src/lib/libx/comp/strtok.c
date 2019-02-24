#include "FEATURE/strtok"

#ifdef _lib_strtok

int	_lib_dummy;

#else

/*
 * uses strpbrk and strspn to break string into tokens on
 * sequentially subsequent calls.  returns 0 when no
 * non-separator characters remain.
 * `subsequent' calls are calls with first argument 0.
 */

extern int	strspn();
extern char*	strpbrk();

char*
strtok(string, sepset)
char*	string
char*	sepset;
{
	register char*	p;
	register char*	q;
	register char*	r;
	static char*	savept;

	/*first or subsequent call*/
	/* return if no tokens remaining */
	if (!(p = string ? string : savept)) return(0);
	/* skip leading separators */
	/* return if no tokens remaining */
	if (!*(q = p + strspn(p, sepset))) return(0);
	/* move past token */
	if (!(r = strpbrk(q, sepset))) savept = 0;	/* this is last token */
	else
	{
		*r = 0;
		savept = ++r;
	}
	return(q);
}

#endif
