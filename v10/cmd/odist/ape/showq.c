#include <stddef.h>
#include <stdio.h>
#include "dist.h"

static void
showq(char *job, int num, char *sys)
{
	printf("%s: %d %s\n", job, num, sys);
}

int
main(int argc, char *argv[])
{
	prog = argv[0];
	scanq(0, showq);
	return 0;
}
