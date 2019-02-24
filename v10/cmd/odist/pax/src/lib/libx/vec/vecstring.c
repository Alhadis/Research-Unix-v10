/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * string vector load support
 */

extern char**	vecload();
extern char*	strdup();

/*
 * load a string vector from lines in str
 */

char**
vecstring(str)
char*	str;
{
	register char*	buf;
	register char**	vec;

	if (!str || !*str || !(buf = strdup(str))) vec = 0;
	else if (!(vec = vecload(buf))) free(buf);
	return(vec);
}
