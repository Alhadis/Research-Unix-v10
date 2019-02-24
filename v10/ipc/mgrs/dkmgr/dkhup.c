#include <stdio.h>
#include <sys/dkio.h>
#include <signal.h>
#include <errno.h>
#include <ctype.h>

char *netfiles = "/dev/dk/dk";
#define	TRAFFIC	2	/* if unit != b, dkctlchan ignores this anyway */
#define	DEFUNIT	'a'

main(argc, argv)
int argc;
char **argv;
{
	int unit;
	int noinit;
	int fd;
	int i;

	unit = DEFUNIT;	/* look at netfiles by default */
	noinit = 0;
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-t") == 0)
			continue;
		if (isdigit(*argv[i])) {
			unit = *argv[i];
			continue;
		}
		if (strcmp(argv[i], "-i") == 0) {
			noinit = 1;
			continue;
		}
		if (strcmp(argv[i], "-N") == 0 && ++i < argc) {
			netfiles = argv[i];
			continue;
		}
		fprintf(stderr, "usage: dkhup [-N netfiles] [-t [02]]\n");
		exit(1);
	}
	setup();
	fd = init(unit, noinit);
	i = 0;
	for (;;) {
		if (takemesg(fd) > 0) {
			i = 0;
			continue;
		}
		if (++i < 5) {		/* give it a few tries */
			sleep(1);
			continue;
		}
		fd = reset(unit, fd);
	}
}

setup()
{
	if (fork())
		exit(0);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGHUP, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
}

int
init(unit, noinit)
int unit, noinit;
{
	int fd;

	if ((fd = dkctlchan(TRAFFIC, unit)) < 0) {
		fprintf(stderr, "dkhup %s %c: can't open control channel\n", netfiles, unit);
		return (-1);
	}
	if (noinit == 0) {
		ioctl(fd, DIOCHUP, 0);
		ioctl(fd, DIOCHUP, 0);
		ioctl(fd, DIOCHUP, 0);
	}
	ioctl(fd, DIOCNXCL, 0);
	return (fd);
}

/*
 * wait for a message from the CMC
 * this shouldn't happen;
 * messages are supposed to be intercepted by the system
 * if the read returns an error,
 * the interface shut down
 */

takemesg(fd)
int fd;
{
	int n;
	char buf[64];

	if ((n = read(fd, buf, sizeof(buf))) <= 0)
		return (-1);
	/*
	 * if it was interesting to print the message,
	 * this would be the place to do so
	 */
	return (1);
}

/*
 * reset after an error
 * close the channel, try to reset the KMC (assumed)
 */

int
reset(unit, fd)
int unit;
int fd;
{
	fprintf(stderr, "dkhup %s %c: error %d; resetting\n", netfiles, unit, errno);
	ioctl(fd, KIOCSHUT, 0);
	close(fd);
	sleep(45);
	loadkmc(unit);
	sleep(5);
	while ((fd = init(unit, 0)) < 0) {
		fprintf(stderr, "dkhup %s %c: can't reinit\n", netfiles, unit);
		sleep(60);	/* or just exit? */
	}
	return (fd);
}

loadkmc(unit)
int unit;
{
	char buf[64];

	if (unit == DEFUNIT)
		unit = '2';		/* ugh */
	sprintf(buf, "cd /usr/dk; /etc/kmcdump %c", unit);
	system(buf);
	sprintf(buf, "/etc/kdiload %c", unit);
	system(buf);
}
