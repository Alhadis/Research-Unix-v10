/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 */

#include <namval.h>

#define streq(a,b)	(*(a)==*(b)&&!strcmp(a,b))

extern int		strcmp();

/*
 * return value for name in tab
 *
 * the last name in tab must be 0
 * the last value in tab is returned if name is not found
 */

int
getnamval(tab, name)
register struct namval*	tab;
register char*		name;
{
	for (; tab->name; tab++)
		if (streq(tab->name, name))
			break;
	return(tab->value);
}
