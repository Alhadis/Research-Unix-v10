/*
 * run a command with login name changed
 */

#include <stdio.h>

main(argc, argv)
int argc;
char **argv;
{

	if (argc < 3) {
		fprintf(stderr, "usage: %s username command\n", argv[0]);
		exit(1);
	}
	if (setlogname(argv[1]) < 0) {
		perror("setlogname");
		exit(1);
	}
	execvp(argv[2], &argv[2]);
	perror(argv[2]);
	exit(1);
}
