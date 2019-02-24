#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../lib/lib.h"

int
main(int argc, char *argv[])
{
	int i;
	char *q;

	for (i = 1; i < argc; ++i) {
		q = xmalloc(strlen(argv[i]) * 4 + 1);
		quote(q, argv[i]);
		argv[i] = q;
	}
	execvp(argv[1], &argv[1]);
}
