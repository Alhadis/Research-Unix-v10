/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * return error message string given errno
 */

#include "FEATURE/strerror"

#ifdef _lib_strerror

int	_lib_dummy;

#else

/*
 * the define/undef makes the non-macro sprintf visible below (huh?)
 */

#define _stdsprintf	sprintf
#include <stdio.h>
#undef	sprintf

char*
strerror(err)
int	err;
{
	static char	msg[28];

	extern int	sys_nerr;
	extern char*	sys_errlist[];

	if (err > 0 && err <= sys_nerr) return(sys_errlist[err]);
	(void)sprintf(msg, "Error %d", err);
	return(msg);
}

#endif
