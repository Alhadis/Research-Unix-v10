#include <ctype.h>

char
lc(ch)
char ch;
{
	if (isupper(ch))
		return(tolower(ch));
	else
		return(ch);
}

/*
 * Compare strings:  s1>s2: >0  s1==s2: 0  s1<s2: <0
 */

strcasecmp(s1, s2)
register char *s1, *s2;
{

	while (lc(*s1) == lc(*s2++))
		if (*s1++=='\0')
			return(0);
	return(lc(*s1) - lc(*--s2));
}

/*
 * Compare strings (at most n bytes):  s1>s2: >0  s1==s2: 0  s1<s2: <0
 */

strncasecmp(s1, s2, n)
register char *s1, *s2;
register n;
{

	while (--n >= 0 && lc(*s1) == lc(*s2++))
		if (*s1++ == '\0')
			return(0);
	return(n<0 ? 0 : lc(*s1) - lc(*--s2));
}
