#include <stdio.h>
#include "rf.h"

char *ctime();
long time();

rflog(s, p1, p2, p3, p4, p5, p6, p7, p8, p9)
char *s;
{
	long now;

	time(&now);
	fseek(stderr, 0L, 2);
	fprintf(stderr, "%.15s %s %d: ", 4+ctime(&now), rfclient, getpid());
	fprintf(stderr, s, p1, p2, p3, p4, p5, p6, p7, p8, p9);
	fflush(stderr);
}
