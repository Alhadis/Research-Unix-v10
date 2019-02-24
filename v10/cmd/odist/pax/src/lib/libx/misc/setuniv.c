/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * set universe by name
 */

#include "univlib.h"

#define streq(a,b)	(*(a)==*(b)&&!strcmp(a,b))

int
setuniv(buf)
char*	buf;
{
#if _lib_getuniverse
	return(setuniverse(buf));
#else
#ifndef UNIV_MAX
	char		tmp[32];

	extern char*	putenv();
	extern char*	strdup();

	(void)sprintf(tmp, "%s=%-*.*s", univ_env, UNIV_SIZE - 1, UNIV_SIZE - 1, buf);
	if ((buf = strdup(tmp)) && putenv(buf)) return(0);
	errno = ENOMEM;
	return(-1);
#else
#ifdef ATT_UNIV
	register int	n;

	for (n = 0; n < UNIV_MAX; n++)
		if (streq(buf, univ_name[n]))
			return(setuniverse(n + 1) >= 0 ? 0 : -1);
#endif
	errno = EINVAL;
	return(-1);
#endif
#endif
}
