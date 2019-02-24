#include <stdio.h>
#include <time.h>
#include "../lib/lib.h"

static void
showq(char *job, int num, char *sys)
{
	printf("%s: %d %s\n", job, num, sys);
}

int
main(int argc, char *argv[])
{
	prog = argv[0];
	scanq(argv[1], showq);
	return 0;
}
