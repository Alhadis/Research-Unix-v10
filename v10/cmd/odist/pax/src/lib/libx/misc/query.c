/*
 * AT&T Bell Laboratories
 *
 * output printf prompt and read response
 * if format==0 then verify that interaction is possible
 *
 * return:
 *
 *	0	[1yY+]
 *	-1	[qQ] or EOF
 *	1	otherwise
 *
 * if quit>=0 then [qQ] or EOF calls exit(quit)
 */

#include <stdio.h>

extern int	errno;

#if __STDC__

#include <stdarg.h>

int
query(int quit, char* format, ...)

#else

#include <varargs.h>

int
query(va_alist)
va_dcl

#endif

{
	va_list		ap;
	register int	n;
	register int	c;
#if !__STDC__
	int		quit;
	char*		format;
#endif
	static FILE*	rfp;
	static FILE*	wfp;

	if (!rfp)
	{
		c = errno;
		if (isatty(fileno(stdin))) rfp = stdin;
		else if (!(rfp = fopen("/dev/tty", "r"))) return(-1);
		if (isatty(fileno(stderr))) wfp = stderr;
		else if (!(wfp = fopen("/dev/tty", "w"))) return(-1);
		errno = c;
	}
#if __STDC__
	va_start(ap, format);
#else
	va_start(ap);
	quit = va_arg(ap, int);
	format = va_arg(ap, char*);
#endif
	if (!format) return(0);
	fflush(stdout);
	vfprintf(wfp, format, ap);
	fflush(wfp);
	for (n = c = getc(rfp);; c = getc(rfp)) switch (c)
	{
	case EOF:
		n = c;
		/*FALLTHROUGH*/
	case '\n':
		switch (n)
		{
		case EOF:
		case 'q':
		case 'Q':
			if (quit >= 0) exit(quit);
			return(-1);
		case '1':
		case 'y':
		case 'Y':
		case '+':
			return(0);
		default:
			return(1);
		}
	}
	va_end(ap);
}
