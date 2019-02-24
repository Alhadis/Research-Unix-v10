#include <sys/ioctl.h>
#include <stdio.h>

#define PARGVAL	((*argv)[2] ? (*argv)+2 : --argc ? *++argv : (char *)0)

char *linkname = 0, *namelink();
int outfd, debug;

#define Getc()	(--nchin > 0 ? *inp++ : (inp = getbuf()) ? *inp++ : -1)

int pgrp, nchin, infd; char inbuf[256], *getbuf();

int Argc; char **Argv;

main(argc,argv)
int argc; char **argv;
{
	register char *inp;

	while (--argc > 0 && (*++argv)[0] == '-' && (*argv)[1])
		switch ((*argv)[1]) {
		case 'o':
			linkname = PARGVAL; break;
		case 'D':
			debug++; break;
		default:
			fprintf(stderr, "unknown option %s\n", *argv);
			exit(1);
		}
	Argc = ++argc; Argv = --argv;

	if (!linkname && !(linkname=namelink("THINK"))) {
		fprintf(stderr, "no name for THINK\n");
		exit(1);
	}
	if ((outfd = open(linkname, 1)) < 0) {
		fprintf(stderr, "cannot open %s\n", linkname);
		exit(1);
	}
	if (debug)
		fprintf(stderr, "%s\n", linkname);
	if (ioctl(outfd, TIOCGPGRP, &pgrp) < 0) {
		fprintf(stderr, "TIOCGPGRP failed on %s\n", linkname);
		if (!debug)
			exit(1);
	} else
		setpgrp(0, pgrp);
	if ((infd = (argc > 1) ? Open() : 0) < 0)
		exit(0);

	inp = getbuf();
	write(outfd, "\377\377\377\377", 4);
	if (nchin >= 2 && inp[0] == 033 && inp[1] == 'N') {
		nchin--;
		thinksort(inp += 2);
	} else do {
		wrt(inbuf, nchin);
	} while (getbuf());
	wrtflush();
	write(outfd, "\0\0", 2);
	exit(0);
}

char *
getbuf()
{
	while ((nchin = read(infd, inbuf, sizeof inbuf)) <= 0) {
		if (infd) close(infd);
		infd = Open();
		if (infd < 0) break;
	}
	return (nchin > 0) ? inbuf : 0;
}

Open()
{
	int infd = -1;
	while (--Argc > 0) {
		if (strcmp("-",*++Argv) == 0) infd = 0;
		else infd = open(*Argv,0);
		if (infd >= 0) break;
		fprintf(stderr,"cannot open %s\n",*Argv);
	}
	return infd;
}
