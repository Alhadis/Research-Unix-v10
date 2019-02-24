/*
 *	sysdt
 *
 *	at exit, xl points to a pseudo-scblk containing the date
 */

#include <time.h>
#include "osint.h"
#include "spitblks.h"

extern struct scblk	tscblk;

sysdt()
{
	long	tod;
	register struct tm *tm;
	struct	tm *localtime();
	register char *p;

	time( &tod );
	tm = localtime( &tod );

	conv( tscblk.str, tm->tm_mon+1 );
        tscblk.str[2] = '/';
	conv( tscblk.str+3, tm->tm_mday );
        tscblk.str[5] = '/';
	conv( tscblk.str+6, tm->tm_year );
        tscblk.str[8] = ' ';
	conv( tscblk.str+9, tm->tm_hour );
        tscblk.str[11] = '.';
	conv( tscblk.str+12, tm->tm_min );
        tscblk.str[14] = '.';
	conv( tscblk.str+15, tm->tm_sec );
	tscblk.len = 17;
	SET_XL( &tscblk );
	return NORMAL_RETURN;
}

static
conv (dest, value)

register char *dest;
register int value;

{
        dest[1] = value % 10 + '0';
        dest[0] = value / 10 + '0';
}

