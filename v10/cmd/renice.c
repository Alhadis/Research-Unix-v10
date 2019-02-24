#include <stdio.h>
#include <errno.h>
#include <sys/pioctl.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/vtimes.h>
#include <sys/proc.h>

extern int errno;

char *progname, procname[16]; struct proc p; int nerrors;

main(argc, argv)
char **argv;
{
	int fd, pri = NZERO-1, dpri;
	progname = *argv;
	if (argv[1]) {
		if (argv[1][0] == '-')
			pri = atoi(*++argv+1);
	} else
		error();
	while (*++argv) {
		sprintf(procname, "/proc/%05d", atoi(*argv));
		if ((fd = open(procname, 0)) < 0 ||
		    ioctl(fd, PIOCGETPR, &p) < 0 ||
		    (dpri = pri + NZERO - p.p_nice,
		    ioctl(fd, PIOCNICE, &dpri)) < 0)
			error();
		close(fd);
	}
	exit(nerrors != 0);
}

error()
{
	switch (errno) {
	case 0:
		fprintf(stderr, "usage: %s [ -pri ] pid ...\n", progname); break;
	case EACCES:
	case EPERM:
		fprintf(stderr, "%s: no permission\n", procname); break;
	case ENOENT:
		fprintf(stderr, "%s: no such process\n", procname); break;
	default:
		fprintf(stderr, "%s: unexpected error %d\n", procname, errno);
	}
	++nerrors;
}
