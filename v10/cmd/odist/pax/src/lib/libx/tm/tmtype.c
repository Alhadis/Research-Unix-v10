/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * time conversion support
 */

#include <tm.h>

/*
 * return the tm_data.zone[] time zone type name for s
 *
 * if e is non-null then it will point to the first
 * unmatched char in s
 *
 * 0 returned for no match
 */

char*
#if __STDC__ || __cplusplus
tmtype(register const char* s, char** e)
#else
tmtype(s, e)
register char*	s;
char**		e;
#endif
{
	register struct tz*	zp;
	register char*		t;

	tmset();
	zp = &tm_info.local;
	do
	{
		if ((t = zp->type) && tmword(s, e, t, (char**)0, 0)) return(t);
		if (zp == &tm_info.local) zp = &tm_data.zone[0];
		else zp++;
	} while (zp->standard);
	return(0);
}
