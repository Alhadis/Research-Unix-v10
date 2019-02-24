/*
 * Print working (current) directory
 */
#include <stdio.h>

char *getwd();

main()
{
	char pathname[512];

	if (getwd(pathname) == NULL) {
		fprintf(stderr, "pwd: %s\n", pathname);
		exit(1);
	}
	printf("%s\n", pathname);
	exit(0);
}
