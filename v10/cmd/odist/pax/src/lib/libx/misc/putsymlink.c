/*
* G. S. Fowler
* AT&T Bell Laboratories
*/

#include "univlib.h"

extern int	symlink();

/*
 * create symbolic name from external representation text in buf
 * the arg order matches link(2)
 */

int
putsymlink(buf, name)
char*	buf;
char*	name;
{
	register char*	t = buf;
#ifdef UNIV_MAX
	register char*	s = buf;
	register char*	v;
	int		n;
	char		tmp[PATH_MAX];

	extern int	strncmp();

	while (*s)
	{
		if (*s++ == univ_cond[0] && !strncmp(s - 1, univ_cond, univ_size))
		{
			s--;
			t = tmp;
			for (n = 0; n < UNIV_MAX; n++)
				if (*univ_name[n])
			{
				*t++ = ' ';
#ifdef ATT_UNIV
				*t++ = '1' + n;
				*t++ = ':';
#else
				for (v = univ_name[n]; *t = *v++; t++);
				*t++ = '%';
#endif
				for (v = buf; v < s; *t++ = *v++);
				for (v = univ_name[n]; *t = *v++; t++);
				for (v = s + univ_size; *t = *v++; t++);
			}
			t = tmp;
			break;
		}
	}
#endif
	return(symlink(t, name));
}
