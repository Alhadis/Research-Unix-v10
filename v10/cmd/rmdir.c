#include <stdio.h>

main(argc, argv)
int argc;
char **argv;
{
	int err = 0;

	if (argc <= 1) {
		fprintf(stderr, "usage: %s dir ...\n", argv[0]);
		exit(1);
	}
	while (--argc > 0)
		if (rmdir(*++argv) < 0) {
			perror(*argv);
			err = 1;
		}
	exit(err);
}
