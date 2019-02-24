#define _POSIX_SOURCE
#define _RESEARCH_SOURCE
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <libv.h>
#include "../paths.h"
#include "../lib/lib.h"
#include "proto.h"

char **jobs, **insargs;

void
recvjob(void)
{
	char data[MAXNAME], errs[MAXNAME], mesg[MAXLINE], **jobp;
	int dfd, efd, attempt, request, status;

	if (!getline(mesg))
		exit(0);

	if (sscanf(mesg, M_BEGIN, data) != 1) {
		eprintf("job header munged: mesg = %s", mesg);
		exit(1);
	}

	if (jobs) {
		request = 0;
		for (jobp = jobs; *jobp; ++jobp)
			if (strcmp(data, *jobp) == 0)
				request = 1;
		sprintf(mesg, M_REQUEST, request);
	} else
		sprintf(mesg, M_REQUEST, 1);
	write(1, mesg, strlen(mesg));

	sprintf(data, "data.%d", getpid());
	remove(data);
	dfd = creat(data, 0600); /* O_CREATE|O_RDWR isn't in v10 */
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

	lseek(dfd, 0L, 0);
	attempt = tryperm(dfd);
	sprintf(mesg, M_ATTEMPT, attempt);
	write(1, mesg, strlen(mesg));

	if (attempt) {
		sprintf(errs, "errs.%d", getpid());
		efd = creat(errs, 0600);	/* O_CREATE|O_RDWR bogosity */
		close(efd);
		efd = open(errs, O_RDWR);
		if (efd < 0) {
			eprintf("can't create errs file %s", errs);
			exit(1);
		}

		if (fork()) {
			status = 0xFF; /* evilly catch wait failures */
			wait(&status);
			if (status &= 0xFF) {
				sprintf(mesg, "insdist: exit %d\n", status);
				write(efd, mesg, strlen(mesg));
			}
		} else {
			lseek(dfd, 0L, 0);
			dup2(dfd, 0);
			dup2(efd, 1);
			dup2(efd, 2);
			close(dfd);
			close(efd);
			execv(LDIR "/insdist", insargs);
			eprintf("can't exec insdist");
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
	int c, n, na;

	prog = argv[0];
	openlog(SDIR "/log");

	n = na = 1;
	insargs = xmalloc(na * sizeof (char *) + 1);
	insargs[0] = "insdist";
	while ((c = getopt(argc, argv, "D:R:")) != EOF)
		if (c != '?') {
			if (n == na)
				insargs = xrealloc(insargs, (na *= 2) + 1);
			insargs[n] = xmalloc(strlen(optarg) + 3);
			insargs[n][0] = '-';
			insargs[n][1] = c;
			strcpy(insargs[n] + 2, optarg);
			++n;
		}
	insargs[n] = NULL;
	if (optind < argc)
		jobs = &argv[optind];

	for (;;)
		recvjob();
}
