/*
 * remove uucp lock files if idle
 * assume it really is a lockfile;
 * assume ascii locks for now (it's easy to fix)
 */

#include <stdio.h>

#define	HUGE	30000

main(argc, argv)
int argc;
char **argv;
{
	FILE *fp;
	int pid;

	if (argc <= 1) {
		fprintf(stderr, "usage: %s lockfile ...\n", argv[0]);
		exit(1);
	}
	while (--argc > 0) {
		if ((fp = fopen(*++argv, "r")) == NULL) {
			fprintf(stderr, "%s: cannot open\n", *argv);
			continue;
		}
		if (fscanf(fp, "%d", &pid) != 1 || pid <= 0) {
			fprintf(stderr, "%s: bad format\n", *argv);
			continue;
		}
		if (kill(pid, 0) >= 0
		||  errno != ESRCH)
			continue;
		unlink(*argv);
	}
	exit(0);
}
