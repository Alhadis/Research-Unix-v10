#include <sys/vlimit.h>
#include <stdio.h>

main(argc, argv)
int argc;
char **argv;
{

	if (argc < 2) {
		fprintf(stderr, "usage: bigcore command\n");
		exit(1);
	}
	vlimit(LIM_CORE, INFINITY);
	execvp(argv[1], &argv[1]);
	perror(argv[1]);
	exit(1);
}
