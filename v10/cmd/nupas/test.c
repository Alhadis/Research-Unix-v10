#include <sys/types.h>
#include <time.h>
#include <stdio.h>

extern long time();
extern void tzset();
extern char *tzname[];

/*
 *  return an ascii daytime string
 */
int
main()
{
	static char date[32];
	char *tp, *dp;
	struct tm *bp;
	long thetime;

	thetime = time((long *)0);
	bp = localtime(&thetime);
	dp = asctime(bp);

	printf("%.16s %d\n", dp, bp->tm_isdst);
	return;
}
