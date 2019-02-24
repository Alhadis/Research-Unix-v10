#include <signal.h>
#include <stdio.h>

#define Signal(s, f)	if (signal(s, SIG_IGN) != SIG_IGN) (void)signal(s, f)

char fname[128];

quit() {
	funmount(fname);
	unlink(fname);
	exit(0);
}

main()
{
	int fd;
	char *nametty();
	strcpy(fname, getenv("HOME"));
	strcat(fname, "/.Gray");

	Signal(SIGHUP, quit);
	Signal(SIGINT, quit);
	Signal(SIGQUIT, quit);
	Signal(SIGPIPE, quit);
	if ((fd = creat(fname, 0222)) < 0) {
		perror(fname);
		exit(1);
	}
	close(fd);
	if (nametty(0, fname) == 0) {
		fprintf(stderr, "can't mount tty\n");
		exit(1);
	}

	if (system("jx /usr/jerq/mbin/pvmon.m")) exit(1);
	close(0);
	for (;;) pause();
}
