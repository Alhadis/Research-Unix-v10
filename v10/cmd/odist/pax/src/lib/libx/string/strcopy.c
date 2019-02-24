/*
 * copy t into s, return a pointer to the end of s ('\0')
 */

char*
strcopy(s, t)
register char*	s;
register char*	t;
{
	if (!t) return(s);
	while (*s++ = *t++);
	return(--s);
}
