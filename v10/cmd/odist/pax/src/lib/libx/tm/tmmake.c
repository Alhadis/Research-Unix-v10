/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * time conversion support
 */

#include <tm.h>

/*
 * return tm struct for clock
 * leap seconds accounted for in return value
 */

struct tm*
#if __STDC__ || __cplusplus
tmmake(time_t* clock)
#else
tmmake(clock)
time_t*	clock;
#endif
{
	register struct tm*	tp;
	register struct tl*	lp;
	int			leapsec;
	time_t			now;

	tmset();
	if (clock) now = *clock;
	else (void)time(&now);
	leapsec = 0;
	if ((tm_info.flags & (TM_ADJUST|TM_LEAP)) == (TM_ADJUST|TM_LEAP) && now > 0)
	{
		for (lp = &tm_data.leap[0]; now < lp->time; lp++);
		if (lp->total)
		{
			if (now == lp->time && (leapsec = (lp->total - (lp+1)->total)) < 0) leapsec = 0;
			now -= lp->total;
		}
	}
	tp = (tm_info.flags & TM_UTC) ? (struct tm*)gmtime(&now) : (struct tm*)localtime(&now);
	tp->tm_sec += leapsec;
	return(tp);
}
