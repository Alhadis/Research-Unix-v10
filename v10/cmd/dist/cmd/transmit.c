#define _POSIX_SOURCE
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "../paths.h"
#include "../lib/lib.h"
#include "proto.h"

static void
sendjob(char *job, int num, char *sys)
{
	char busy[MAXNAME], done[MAXNAME], mesg[MAXLINE];
	int fd, attempt;

	sprintf(busy, "%d.busy", num);
	sprintf(done, "%d.done", num);

	fd = lcreat(busy, 0600);
	if (fd < 0)
		return;
	dup2(fd, 2);
	close(fd);

	/* avoid race condition */
	if (access(done, F_OK) == 0)
		goto out;

	fd = open("data", O_RDONLY);
	if (fd < 0)
		goto out;

	if (!chkperm(fd)) {
		close(fd);
		goto out;
	}

	sprintf(mesg, M_BEGIN, job);
	write(1, mesg, strlen(mesg));

	if (!getline(mesg) || sscanf(mesg, M_REQUEST, &attempt) != 1)
		goto err;
	if (!attempt)
		goto out;

	if (!sendfile(fd))
		goto err;

	if (!getline(mesg) || sscanf(mesg, M_ATTEMPT, &attempt) != 1)
		goto err;
	if (!attempt)
		goto out;

	if (!recvfile(2))
		goto err;

	rename(busy, done);

	fd = open("/dev/null", O_WRONLY);
	dup2(fd, 2);
	close(fd);
	return;

 out:
	/* continue with the next job */
	remove(busy);
	return;

 err:
	/* failure that's too hard to resynchronize */
	eprintf("sendjob really barfed");
	remove(busy);
	exit(1);
}

int
main(int argc, char *argv[])
{
	prog = argv[0];
	openlog(SDIR "/log");
	if (argc < 2) {
		eprintf("usage: transmit system");
		return 1;
	}
	scanq(argv[1], sendjob);
	return 0;
}
