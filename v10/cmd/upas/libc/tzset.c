#include <sys/types.h>
#include <sys/timeb.h>
/*
 *	mimic system V's wretched timezone stuff for v8 and 4.2
 */

char *tzname[2];

void
tzset() {
	struct timeb timbuf;
	extern char *timezone();

	ftime(&timbuf);
	tzname[0] = timezone(timbuf.timezone, 0);
	tzname[1] = timezone(timbuf.timezone, 1);
}
