/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * string vector argv insertion
 */

#include <ctype.h>

extern char*	malloc();
extern void	vecfree();

/*
 * insert the string vector vec between
 * (*argvp)[0] and (*argvp)[1], sliding (*argvp)[1] ... over
 * null and blank args are deleted
 *
 * vecfree(vec) always called
 *
 * -1 returned if insertion failed
 */

int
vecargs(vec, argcp, argvp)
register char**	vec;
int*		argcp;
char***		argvp;
{
	register char**	argv;
	register char**	oargv;
	char**		ovec;
	char*		s;
	int		num;

	if (!vec) return(-1);
	if ((num = (char**)(*(vec - 1)) - vec) > 0)
	{
		if (!(argv = (char**)malloc((num + *argcp + 1) * sizeof(char*))))
		{
			vecfree(vec, 0);
			return(-1);
		}
		oargv = *argvp;
		*argvp = argv;
		*argv++ = *oargv++;
		ovec = vec;
		while (s = *argv = *vec++)
		{
			while (isspace(*s)) s++;
			if (*s) argv++;
		}
		vecfree(ovec, 1);
		while (*argv = *oargv++) argv++;
		*argcp = argv - *argvp;
	}
	else vecfree(vec, 0);
	return(0);
}
