#include <stdio.h>
#include <sys/reboot.h>
/*
 * Reboot
 */
static	char *sccsid = "@(#)reboot.c	4.1 (Berkeley) 10/9/80";

main(argc, argv)
	int argc;
	char **argv;
{
	int how;
	register char *argp;

	argc--, argv++;
	how = 0;
	while (argc > 0) {
		if (!strcmp(*argv, "-s"))
			how |= RB_SINGLE;
		else if (!strcmp(*argv, "-n"))
			how |= RB_NOSYNC;
		else if (!strcmp(*argv, "-a"))
			how |= RB_ASKNAME;
		else if (!strcmp(*argv, "-d"))
			how |= RB_PANIC;
		else {
			fprintf(stderr,
			    "usage: reboot [ -a ] [ -n ] [ -s ] [ -d ]\n");
			exit(1);
		}
		argc--, argv++;
	}
	reboot(how);
	perror("reboot");
}
