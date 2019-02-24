#include <sys/types.h>
#include <sys/timeb.h>
#include <time.h>

extern long time();
extern char *timezone();
extern struct tm *localtime();
extern char *asctime();

/*
 *  return an ascii daytime string
 */
extern char *
thedate()
{
	static char date[32];
	char *tp, *dp;
	struct timeb timbuf;
	struct tm *bp;
	long thetime;

	thetime = time((long *)0);
	bp = localtime(&thetime);
	dp = asctime(bp);
	ftime(&timbuf);
	tp = timezone(timbuf.timezone, bp->tm_isdst ? 1 : 0);
	sprintf(date, "%.19s %.3s %.4s", dp, tp, dp+20);
	return date;
}
