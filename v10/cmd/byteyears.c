/*
 *	byte-years --
 *	the size of a file times the length of time since
 *	last modified, expressed in years.
 *	Also print size, date, name
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ftw.h>

#define YEAR (24L*60L*60L*365L)

time_t now, year;
int aflag, errflag;

int
consider (name, buf, code)
	char *name;
	register struct stat *buf;
	int code;
{
	double by;
	register char *p;
	char *ctime();
	time_t t;

	switch (code) {

	case FTW_F:
		t = aflag? buf->st_atime: buf->st_mtime;
		by = (double) buf->st_size * (double) (now - t) / YEAR;

		printf ("%12.0f %10d", by + 0.5, buf->st_size);
		p = ctime (&t);
		if (t < year)
			printf(" %-7.7s %-4.4s ", p+4, p+20);
		else
			printf(" %-12.12s ", p+4);
		printf ("%s\n", name);
		break;

	case FTW_NS:
		perror (name);
		break;
	}

	return 0;
}

main (argc, argv)
	int argc;
	char **argv;
{
	int i;
	extern char _sobuf[BUFSIZ];
	register int c;
	extern int optind;
	int rc = 0;

	setbuf (stdout, _sobuf);

	while ((c = getopt (argc, argv, "a")) != EOF) {
		switch (c) {

		case 'a':
			aflag++;
			break;

		case '?':
			errflag++;
		}
	}

	if (errflag) {
		fprintf (stderr, "usage: byteyears [-a] [file]...\n");
		exit (1);
	}

	time (&now);
	year = now - (YEAR/2);

	if (argc <= optind) {
		if (ftw ("", consider, 8) < 0) {
			fprintf (stderr, "cannot access current directory\n");
			rc++;
		}
	} else {
		for (i = optind; i < argc; i++) {
			if (ftw (argv[i], consider, 8) < 0) {
				fprintf (stderr, "cannot access %s\n", argv[i]);
				rc++;
			}
		}
	}
	return rc;
}
