
/*
 *	getdate.c -- fill a buffer with the date and time
 */

#include <time.h>

getdate (p)

char *p;

{
	long	tod;
	register struct tm *tm;
	struct	tm *localtime();

	time (&tod);
	tm = localtime (&tod);

	conv (p, tm->tm_mon+1);
        p[2] = '/';
	conv (p+3, tm->tm_mday);
        p[5] = '/';
	conv (p+6, tm->tm_year);
        p[8] = ' ';
	conv (p+9, tm->tm_hour);
        p[11] = '.';
	conv (p+12, tm->tm_min);
        p[14] = '.';
	conv (p+15, tm->tm_sec);
}

static
conv (dest, value)

register char *dest;
register int value;

{
        dest[1] = value % 10 + '0';
        dest[0] = value / 10 + '0';
}

