/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * library error handler
 */

#include <error.h>

#if __STDC__

#include <stdarg.h>

void
liberror(const char* lib, int level, ...)

#else

#include <varargs.h>

void
liberror(va_alist)
va_dcl

#endif

{
	va_list	ap;
#if !__STDC__
	char*	lib;
	int	level;
#endif

#if __STDC__
	va_start(ap, level);
#else
	va_start(ap);
	lib = va_arg(ap, char*);
	level = va_arg(ap, int);
#endif
	verror(lib, level, ap);
	va_end(ap);
}
