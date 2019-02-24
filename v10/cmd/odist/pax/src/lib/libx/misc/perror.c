/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * perror() using strerror()
 */

#include <stdio.h>

#include "FEATURE/strerror"

#ifdef _lib_strerror

int	_lib_dummy;

#else

extern int	errno;

extern int	write();
extern char*	strerror();

void
perror(s)
char*	s;
{
	char	buf[1024];

	if (s && *s) sprintf(buf, "%s: %s\n", s, strerror(errno));
	else sprintf(buf, "%s\n", strerror(errno));
	(void)write(2, buf, strlen(buf));
}

#endif
