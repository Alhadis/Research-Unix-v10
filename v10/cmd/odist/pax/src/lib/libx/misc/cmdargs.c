/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * multi-pass commmand line option parse assist
 *
 *	int fun(char** argv, int last)
 *
 * each fun() argument parses as much of argv as
 * possible starting at (opt_index,opt_char) using
 * optget()
 *
 * if last!=0 then fun is the last pass to view
 * the current arg, otherwise fun sets opt_again=1
 * and another pass will get a crack at it
 *
 * 0 fun() return causes immediate cmdargs() 0 return
 *
 * cmdargs() returns non-zero if more args remain
 * to be parsed at opt_index
 */

#include <option.h>

typedef int (*PFI)();

int	opt_again;		/* repeat last position in next optget	*/

#if __STDC__

#include <stdarg.h>

int
cmdargs(char** argv, ...)

#else

#include <varargs.h>

int
cmdargs(va_alist)
va_dcl

#endif

{
	va_list			ap;
	register PFI		fun;
	register PFI		rep;
	int			last_index;
	int			last_char;
#if !__STDC__
	register char		**argv;
#endif

	rep = 0;
	for (;;)
	{
#if __STDC__
		va_start(ap, argv);
#else
		va_start(ap);
		argv = va_arg(ap, char**);
#endif
		while (fun = va_arg(ap, PFI))
		{
			last_index = opt_index;
			last_char = opt_char;
			if (!(*fun)(argv, fun == rep) || !argv[opt_index]) return(argv[opt_index] != 0);
			if (opt_again)
			{
				opt_again = 0;
				opt_index = opt_pindex;
				opt_char = opt_pchar;
			}
			if (!rep || opt_index != last_index || opt_char != last_char) rep = fun;
			else if (fun == rep) return(1);
		}
		va_end(ap);
	}
}
