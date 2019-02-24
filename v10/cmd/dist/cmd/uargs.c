#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../lib/lib.h"

int
main(int argc, char *argv[])
{
	int i;
	char *u;

	for (i = 1; i < argc; ++i) {
		u = xmalloc(strlen(argv[i]) + 1);
		unquote(u, argv[i]);
		argv[i] = u;
	}
	execvp(argv[1], &argv[1]);
}
