/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * universe support
 */

#include "univlib.h"

int
getuniv(buf)
char*	buf;
{
#if _lib_getuniverse
	return(getuniverse(buf));
#else
#ifndef UNIV_MAX
	register char*	s;

	extern char*	getenv();

	if ((s = getenv(univ_env)) && *s)
	{
		(void)strncpy(buf, s, UNIV_SIZE - 1);
		buf[UNIV_SIZE - 1] = 0;
		return(0);
	}
#else
#ifdef ATT_UNIV
	register int	n;

	if ((n = setuniverse(ATT_UNIV)) < 0) return(-1);
	(void)setuniverse(n);
	if (n >= 1 && n <= UNIV_MAX)
	{
		(void)strncpy(buf, univ_name[n - 1], UNIV_SIZE - 1);
		buf[UNIV_SIZE - 1] = 0;
		return(0);
	}
#endif
#endif
	errno = EINVAL;
	return(-1);
#endif
}
