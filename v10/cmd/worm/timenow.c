#include	<libc.h>
#include	"worm.h"

char *
timenow()
{
	long tim;
	char *tims;

	time(&tim);
	tims = ctime(&tim);
	tims[19] = 0;
	return(tims);
}
