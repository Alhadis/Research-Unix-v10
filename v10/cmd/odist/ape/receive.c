#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "dist.h"

void
recvjob(void)
{
	char data[MAXNAME], errs[MAXNAME], mesg[MAXLINE];
	int dfd, efd, attempt, status;

	if (!getline(mesg))
		exit(0);

	if (sscanf(mesg, "begin job %s", data) != 1) {
		eprintf("job header munged: mesg = %s", mesg);
		exit(1);
	}

	sprintf(data, "data.%d", getpid());
	remove(data);
	dfd = creat(data, 0600); /* ape deficiency */
	close(dfd);
	dfd = open(data, O_RDWR);
	if (dfd < 0) {
		eprintf("can't create data file %s", data);
		exit(1);
	}

	if (!recvfile(dfd)) {
		remove(data);
		eprintf("recvfile failed");
		exit(1);
	}

	/* see if file system permissions will likely allow inspkg to succeed */
	lseek(dfd, 0L, 0);
	attempt = tryperm(dfd);
	sprintf(mesg, "client attempt=%d\n", attempt);
	write(1, mesg, strlen(mesg));

	if (attempt) {
		sprintf(errs, "errs.%d", getpid());
		efd = creat(errs, 0600);	/* more ape bogosity */
		close(efd);
		efd = open(errs, O_RDWR);
		if (efd < 0) {
			eprintf("can't create errs file %s", errs);
			exit(1);
		}

		if (fork()) {
			status = 0xFF; /* evilly catch wait failures */
			wait(&status);
			if (status & 0xFF) {
				sprintf(mesg, "inspkg: exit %d\n", status & 0xFF);
				write(efd, mesg, strlen(mesg));
			}
		} else {
			lseek(dfd, 0L, 0);
			dup2(dfd, 0);
			dup2(efd, 1);
			dup2(efd, 2);
			close(dfd);
			close(efd);
			execl(LDIR "/inspkg", "inspkg", 0);
			eprintf("can't exec inspkg");
			exit(1);
		}

		lseek(efd, 0L, 0);
		if (!sendfile(efd)) {
			remove(data);
			remove(errs);
			eprintf("can't send error log");
			exit(1);
		}
	}

	close(dfd);
	remove(data);
	close(efd);
	remove(errs);
}

int
main(int argc, char *argv[])
{
	prog = argv[0];
	for (;;)
		recvjob();
}
