/*
 * AT&T Bell Laboratories
 *
 * regular expression match substitution
 */

#include "relib.h"

#include <ctype.h>

/*
 * do a single substitution
 */

/*ARGSUSED*/
#if __STDC__ || __cplusplus
static char*
sub(register const char* sp, register char* dp, register rematch* mp, int flags)
#else
static char*
sub(sp, dp, mp, flags)
register char*		sp;
register char*		dp;
register rematch*	mp;
int			flags;
#endif
{
	register int	i;
	char*		s;

	for (;;) switch (*dp = *sp++)
	{
	case 0:
		return(dp);
	case '\\':
		switch (i = *sp++)
		{
		case 0:
			sp--;
			break;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			i -= '0';
			if (mp[i].sp)
			{
				s = mp[i].sp;
				while (s < mp[i].ep) *dp++ = *s++;
			}
			break;
		default:
			*dp++ = chresc(sp - 2, &s);
			sp = s;
			break;
		}
		break;
	case '&':
		if (mp[0].sp)
		{
			s = mp[0].sp;
			while (s < mp[0].ep) *dp++ = *s++;
		}
		break;
	default:
		dp++;
		break;
	}
}

/*
 * ed(1) style substitute using matches from last reexec()
 */

#if __STDC__ || __cplusplus
char*
resub(reprogram* re, register const char* op, const char* sp, register char* dp, int flags)
#else
char*
resub(re, op, sp, dp, flags)
reprogram*		re;	/* compiled re				*/
register char*		op;	/* original subject string		*/
char*			sp;	/* substitution string			*/
register char*		dp;	/* destination string			*/
int			flags;	/* RE_ALL, RE_LOWER, RE_UPPER		*/
#endif
{
	register rematch*	mp;

	mp = re->match;
	do
	{
		while (op < mp->sp) *dp++ = *op++;
		op = dp;
		dp = sub(sp, dp, mp, flags);
		if (flags & (RE_LOWER|RE_UPPER))
		{
			while (op < dp)
			{
				if (flags & RE_LOWER)
				{
					if (isupper(*op)) *(char*)op = tolower(*op);
				}
				else if (islower(*op)) *(char*)op = toupper(*op);
				op++;
			}
		}
		op = mp->ep;
	} while ((flags & RE_ALL) && *op && mp->sp != mp->ep && reexec(re, op));
	while (*dp++ = *op++);
	return(--dp);
}
