#include <stdio.h>
#include <time.h>
#define time_t ducky_time_t	/* v10 kludge */
#include <sys/types.h>
#undef time_t
#include <sys/stat.h>
#include <fcntl.h>
#include "../lib/lib.h"
#include "proto.h"
#include "../paths.h"

int
main(int argc, char *argv[])
{
	char mesg[MAXLINE], job[MAXNAME];
	int status, fd, i;

	prog = argv[0];
	openlog(SDIR "/log");
	if (argc < 2) {
		eprintf("usage: notified system");
		exit(1);
	}
	if (!getline(mesg) || sscanf(mesg, M_ANNOUNCE, job) != 1)
		exit(1);
	if (fork()) {
		wait(&status);
		sprintf(mesg, M_ACK);
		write(1, mesg, strlen(mesg));
	} else {
		fd = open(SDIR "/log", O_RDWR);
		dup2(fd, 0);
		dup2(fd, 1);
		dup2(fd, 2);
		for (i = 3; i <= fd; ++i)
			close(i);
		execl(LDIR "/dist", "dist", "-r", argv[1], job, (char *) NULL);
		eprintf("can't call back %s for job %s", argv[1], job);
		exit(1);
	}
}
