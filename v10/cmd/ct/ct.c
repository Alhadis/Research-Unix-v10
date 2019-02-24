/*
 *	ct - call terminal
 */

#include <sgtty.h>
#include <stdio.h>
#include <signal.h>
#include <utmp.h>

#define GETTY "/etc/getty"
#define ROOT 0
#define OTHER 1

extern int optind;
extern char *optarg;

char *ttyname();

char *num, *class = "D1200";

int hangup = 0;
char *verbose; FILE *tfile;
int errflg = 0;
int count = 5;		/* how many times to try to get through */
int waitint = 60;	/* how many seconds to wait between attempts */

#define SCPYN(a, b)	strncpy(a, b, sizeof(a))
#define SCMPN(a, b)	strncmp(a, b, sizeof(a))

char utmp[] = "/etc/utmp";
char wtmpf[] = "/usr/adm/wtmp";
char Usage[] = "usage: ct [-h] [-v file] [-c count] [-w interval] telno [class]";

struct utmp wtmp;

main (argc, argv)
	int argc;
	char **argv;
{
	register int c;
	int p, pid, status, fd;
	char *tty;

	if (argc < 2)
		usage();

	while ((c = getopt (argc, argv, "hv:c:w:")) != EOF) {
		switch (c) {
		case 'h':
			hangup = 1; break;
		case 'v':
			verbose = optarg; break;
		case 'c':
			count = atoi (optarg); break;
		case 'w':
			waitint = atoi (optarg); break;
		case '?':
			errflg++; break;
		}
	}

	if (errflg || optind > argc-1)
		usage();

	if (verbose) {
		if (!(tfile = fopen(verbose, "w")))
			fatal("ct: could not fopen %s\n", verbose);
	}

	num = argv[optind];
	if (optind < argc-1)
		class = argv[optind+1];

	/* hang up the phone if requested and standard input is a terminal */
	if (hangup) {
		struct ttydevb s;
		if (ioctl(0, TIOCGDEV, &s) >= 0) {
			setsigs (SIG_IGN);
			s.ispeed = s.ospeed = 0;
			ioctl(0, TIOCSDEV, &s);
			trace("ct: hung up on fd 0\n");
			sleep (8);	/* let modems quiesce */
		} else {
			hangup = 0;
			fatal("ct: hangup failed\n");
		}
	}

	while (--count >= 0) {
		trace("ct: dialing %s %s\n", num, class);
		fd = dialout (num, class);
		trace("ct: got fd %d\n", fd);
		if (fd >= 0)
			break;
		if (count >= 0)
			sleep (waitint);
	}

	if (fd < 0)
		fatal("ct: connect failed\n");

	/* we don't want exclusive use of the line */
	ioctl (fd, TIOCNXCL, 0);

	/* figure out the name we were given */
	if (!(tty = ttyname (fd)))
		fatal("ct: can't find tty\n");
	
	setsigs (SIG_IGN);

	trace("ct: connected\n");
	session (fd);
	trace("ct: session ended\n");
	cleanup (tty);

	exit(0);
}

/* conduct a terminal session on file "fd" */
session (fd)
register int fd;
{
	int status, pid;
	register int p;

	switch (pid = fork()) {
	default:	/* parent */
		do {
			p = wait (&status);
		} while (p >= 0 && p != pid);
		trace("ct: session exit status %d\n", status);
		break;
	
	case 0:		/* child */
		/* Allow the terminal session to run unmasked */
		setsigs (SIG_DFL);

		/* become the head of a new process group */
		ioctl (fd, TIOCSPGRP, 0);

		/* set up standard input, output, error, tty */
		for (p=0; p<4; p++)
			close (p);
		for (p=0; p<4; p++)
			dup (fd);
		for (; p<_NFILE; p++)
			close(p);

		/* start the session */
		execl (GETTY, "-", strchr (class, '3')? "5": "3", 0);
		exit (1);	/* exec failed */
	
	case -1:
		exit (1);
	}
	return status;
}

cleanup (dev)
	char *dev;
{
	register f;
	register char *line;

	line = dev;
	if (strncmp(line, "/dev/", 5) == 0)
		line += 5;

	/* indicate this user is no longer signed on */
	f = open(utmp, 2);
	if(f >= 0) {
		while(read(f, (char *)&wtmp, sizeof(wtmp)) == sizeof(wtmp)) {
			if (SCMPN(wtmp.ut_line, line))
				continue;
			lseek(f, -(long)sizeof(wtmp), 1);
			SCPYN(wtmp.ut_name, "");
			time(&wtmp.ut_time);
			write(f, (char *)&wtmp, sizeof(wtmp));
		}
		close(f);
	}

	/* write logout information for accounting */
	f = open(wtmpf, 1);
	if (f >= 0) {
		SCPYN(wtmp.ut_line, line);
		SCPYN(wtmp.ut_name, "");
		time(&wtmp.ut_time);
		lseek(f, (long)0, 2);
		write(f, (char *)&wtmp, sizeof(wtmp));
		close(f);
	}

	/* reset device to pristine state */
	chown (dev, ROOT, OTHER);
	chmod (dev, 0666);
}

/* set all important signals to f */
setsigs (f)
	register int (*f)();
{
	signal (SIGHUP, f);
	signal (SIGINT, f);
	signal (SIGQUIT, f);
}

trace(a, b, c, d, e, f)
{
	long *ltime; char *ctime();
	register char *tstr;
	if (!tfile)
		return;
	time(&ltime);
	tstr = ctime(&ltime);
	fprintf(tfile, "[%.2s%.4s%.9s] ", tstr+8, tstr+3, tstr+10);
	fprintf(tfile, a, b, c, d, e, f);
	fflush(tfile);
}

fatal(a, b, c, d, e, f)
{
	trace(a, b, c, d, e, f);
	if (!hangup)
		fprintf(stderr, a, b, c, d, e, f);
	exit(1);
}

usage()
{
	fprintf(stderr, "%s\n", Usage);
	exit(1);
}
