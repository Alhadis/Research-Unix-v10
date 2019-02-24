/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * time conversion support
 */

#include <tm.h>

/*
 * return minutes west of GMT for timezone name and type
 *
 * if type==0 then all time zone types match
 * otherwise type must be a value returned by tmtype()
 *
 * if end is non-null then it will point to the next
 * unmatched char in name
 *
 * if dst is non-null then it will point to the DST
 * offset for standard zones and 0 for daylight zones
 *
 * TM_LOCALZONE returned for no match
 */

int
#if __STDC__ || __cplusplus
tmzone(register const char* name, char** end, const char* type, int* dst)
#else
tmzone(name, end, type, dst)
register char*	name;
char**		end;
char*		type;
int*		dst;
#endif
{
	register struct tz*	zp;
	register char*		prev;

	tmset();
	zp = &tm_info.local;
	prev = 0;
	do
	{
		if (zp->type) prev = zp->type;
		if (!type || type == prev || !prev)
		{
			if (tmword(name, end, zp->standard, (char**)0, 0))
			{
				if (dst) *dst = zp->dst;
				return(zp->west);
			}
			if (zp->dst && zp->daylight && tmword(name, end, zp->daylight, (char**)0, 0))
			{
				if (dst) *dst = 0;
				return(zp->west + zp->dst);
			}
		}
		if (zp == &tm_info.local) zp = &tm_data.zone[0];
		else zp++;
	} while (zp->standard);
	return(TM_LOCALZONE);
}
