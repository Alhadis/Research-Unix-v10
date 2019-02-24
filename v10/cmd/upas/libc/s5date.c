#include <sys/types.h>
#include <time.h>

extern long time();
extern void tzset();
extern char *tzname[];

/*
 *  return an ascii daytime string
 */
extern char *
thedate()
{
	static char date[32];
	char *tp, *dp;
	struct tm *bp;
	long thetime;

	thetime = time((long *)0);
	tzset();
	bp = localtime(&thetime);
	dp = asctime(bp);
	tp = bp->tm_isdst ? tzname[1] : tzname[0];
	sprintf(date, "%.16s %.3s %.4s", dp, tp, dp+20);
	return date;
}
