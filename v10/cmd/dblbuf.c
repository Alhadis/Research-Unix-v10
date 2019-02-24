#include <stdio.h>
#include <signal.h>

#define Read 1
#define Write 2
#define Endfile 3
#define Error 4

int inpipe, outpipe;
int infd = 0, outfd = 1;
int bufmax = 32768;
char *buffer;
int reclen;
extern char *optarg;
extern int optind;
extern char *malloc();

int
gettoken()
{
	char c;

	switch (read (inpipe, &c, 1)) {
	case 0:	return Endfile;
	case 1:	return c;
	default:
		perror ("gettoken");
		return Error;
	}
}

void
puttoken(t)
	char t;
{
	write (outpipe, &t, 1);
}

int
readbuf()
{
	reclen = read (infd, buffer, bufmax);

	if (reclen < 0) {
		perror ("read");
		return 0;
	}

	return reclen > 0;
}

int
writebuf()
{
	if (write (outfd, buffer, reclen) != reclen) {
		perror ("write");
		return 0;
	}
	return 1;
}

int
copyloop()
{
	for (;;) {
		int t;
		if ((t = gettoken()) != Read)
			return t;
		if (!readbuf()) {
			puttoken (Endfile);
			return Endfile;
		}
		puttoken (Read);
		if ((t = gettoken()) != Write)
			return t;
		if (!writebuf()) {
			puttoken (Error);
			return Error;
		}
		puttoken (Write);
	}
}

int
copyfile()
{
	int p0[2], p1[2];
	int p, pid, status;
	pipe (p0);
	pipe (p1);
	switch (pid = fork()) {
	default:			/* parent */
		inpipe = p0[0];
		close (p0[1]);
		outpipe = p1[1];
		close (p1[0]);

		status = copyloop();
		if (status != Endfile) {
			fprintf (stderr, "error: parent status %d\n", status);
			return 1;
		}

		do	p = wait (&status);
		while (p >= 0 && p != pid);
		if (p < 0) {
			perror ("wait");
			return 1;
		}

		if (status != Endfile<<8) {
			fprintf (stderr, "error: child status %d\n", status);
			return 1;
		}
		
		return 0;

	case 0:				/* child */
		inpipe = p1[0];
		close (p1[1]);
		outpipe = p0[1];
		close (p0[0]);
		puttoken (Read);
		puttoken (Write);
		exit (copyloop());

	case -1:			/* error */
		perror ("fork");
		return 1;
	}
}

main (argc, argv)
	int argc;
	char **argv;
{
	int c;
	while ((c = getopt (argc, argv, "b:")) != -1) {
		switch (c) {
		case 'b':
			bufmax = atoi (optarg);
			if (bufmax <= 0) {
				fprintf (stderr, "invalid buffer size\n");
				exit (1);
			}
		}
	}

	if ((buffer = malloc (bufmax)) == NULL) {
		fprintf (stderr, "out of memory\n");
		exit (1);
	}

	if (argc > optind + 1) {
		fprintf (stderr, "only one input file\n");
		exit (1);
	}

	if (argc > optind) {
		infd = open (argv[optind], 0);
		if (infd < 0) {
			perror (argv[optind]);
			exit (1);
		}
	}

	signal (SIGPIPE, SIG_IGN);
	return copyfile();
}
