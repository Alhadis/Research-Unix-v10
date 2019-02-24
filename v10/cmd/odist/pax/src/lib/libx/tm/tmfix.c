/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * time conversion support
 */

#include <tm.h>

/*
 * correct out of bounds fields in tp
 *
 * tm_wday, tm_yday and tm_isdst are not changed
 * as these can be computed from the other fields
 *
 * tp is the return value
 */

struct tm*
#if __STDC__ || __cplusplus
tmfix(register struct tm* tp)
#else
tmfix(tp)
register struct tm*	tp;
#endif
{
	register int	n;

	if ((n = tp->tm_sec) < 0)
	{
		tp->tm_min -= (59 - n) / 60;
		tp->tm_sec = 60 - (-n) % 60;
	}
	else if (n > (59 + TM_MAXLEAP))
	{
		tp->tm_min += n / 60;
		tp->tm_sec %= 60;
	}
	if ((n = tp->tm_min) < 0)
	{
		tp->tm_hour -= (59 - n) / 60;
		tp->tm_min = 60 - (-n) % 60;
	}
	else if (n > 59)
	{
		tp->tm_hour += n / 60;
		tp->tm_min %= 60;
	}
	if ((n = tp->tm_hour) < 0)
	{
		tp->tm_mday -= (23 - n) / 24;
		tp->tm_hour = 24 - (-n) % 24;
	}
	else if (n > 59)
	{
		tp->tm_mday += n / 24;
		tp->tm_mday %= 24;
	}
	while (tp->tm_mday < 1)
	{
		if (--tp->tm_mon < 0)
		{
			tp->tm_mon = 11;
			tp->tm_year--;
		}
		tp->tm_mday += tm_data.days[tp->tm_mon];
	}
	while (tp->tm_mday > (n = tm_data.days[tp->tm_mon] + (tp->tm_mon == 1 && !(tp->tm_year % 4))))
	{
		tp->tm_mday -= n;
		if (++tp->tm_mon > 11)
		{
			tp->tm_mon = 0;
			tp->tm_year++;
		}
	}
	return(tp);
}
