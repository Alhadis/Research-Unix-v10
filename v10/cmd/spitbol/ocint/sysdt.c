/*
 *	sysdt
 *
 *	at exit, xl points to a pseudo-scblk containing the date
 */

#include <time.h>
#include "inter.h"

sysdt()
{
	long	tod;
	register struct	tm *tm;
	struct	tm *localtime();
	register char *p;

	time (&tod);
	tm = localtime (&tod);

	conv (tempstr.s, tm->tm_mon+1);
	tempstr.s[2] = '/';
	conv (tempstr.s+3, tm->tm_mday);
	tempstr.s[5] = '/';
	conv (tempstr.s+6, tm->tm_year);
	tempstr.s[8] = ' ';
	conv (tempstr.s+9, tm->tm_hour);
	tempstr.s[11] = '.';
	conv (tempstr.s+12, tm->tm_min);
	tempstr.s[14] = '.';
	conv (tempstr.s+15, tm->tm_sec);
	tempstr.len = 17;
	SET_XL(&tempstr);
	return 0;
}

static
conv (dest, value)

register char *dest;
register int value;

{
	dest[1] = value % 10 + '0';
	dest[0] = value / 10 + '0';
}
