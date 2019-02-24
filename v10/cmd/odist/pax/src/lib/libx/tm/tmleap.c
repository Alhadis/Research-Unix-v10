/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * time conversion support
 */

#include <tm.h>

/*
 * return clock with leap seconds adjusted
 * for direct localtime() access
 */

time_t
#if __STDC__ || __cplusplus
tmleap(register time_t* clock)
#else
tmleap(clock)
register time_t*	clock;
#endif
{
	register struct tl*	lp;
	time_t			now;

	tmset();
	if (clock) now = *clock;
	else (void)time(&now);
	if (tm_info.flags & TM_ADJUST)
	{
		for (lp = &tm_data.leap[0]; now < (lp->time - lp->total); lp++);
		now += lp->total;
	}
	return(now);
}
