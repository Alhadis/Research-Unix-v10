/*
 * Concatenate files.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#define BLOCK 256

char	buf[BLOCK];

main (argc, argv)
	int argc;
	char **argv;
{
	register int fflg = 0;
	register int fi, fo;
	register int n;
	register int dev, ino = -1;
	struct stat statb;
	int retcode=0;

	if (fstat (1, &statb) < 0) {
		perror ("cat stdout");
		return 1;
	}

	statb.st_mode &= S_IFMT;
	if (statb.st_mode!=S_IFCHR && statb.st_mode!=S_IFBLK) {
		dev = statb.st_dev;
		ino = statb.st_ino;
	}
	if (argc < 2) {
		argc = 2;
		fflg++;
	}
	while (--argc > 0) {
		if (fflg || **++argv == '-' && (*argv)[1] == '\0')
			fi = 0;
		else {
			if ((fi = open (*argv, 0)) < 0) {
				fprintf(stderr, "cat: ");
				perror (*argv);
				retcode = 1;
				continue;
			}
		}
		fstat (fi, &statb);
		if (fstat (fi, &statb) >= 0
		    && statb.st_dev == dev
		    && statb.st_ino == ino) {
			fprintf (stderr, "cat: input %s is output\n",
			   fflg? "-": *argv);
			close (fi);
			retcode = 1;
			continue;
		}
		while ((n = read (fi, buf, sizeof buf)) > 0) {
			if (write (1, buf, n) != n){
				perror ("cat output");
				return 1;
			}
			sleep(buf[0]%7+2);
		}
		if (fi != 0)
			close (fi);
	}
	return retcode;
}
