/*
 * make configuration files for unix v9
 */

#include <libc.h>
#include "mkconf.h"

Mdev *mlist;	/* master list of devices; no particular order */
Dev *dlist;	/* configured devices; clustered by type, sorted within by id */
Scb *llist;	/* scb offsets; sorted by offset */

char *tfile = "tab";
char *dfile = "devs";
char *lfile = "low";
char *oconf = "conf.c";
char *olow = "low.s";

#define	NCF	10
char *cfile[NCF] = {"conf"};

int errs;

main(argc, argv)
int argc;
char **argv;
{
	register char **p;

	p = cfile;
	while (--argc > 0) {
		if (**++argv != '-') {
			if (p >= &cfile[NCF-1]) {
				fprint(STDERR, "too many files\n");
				exit(1);
			}
			*p++ = *argv;
			continue;
		}
		if (argc == 1)
			usage();
		if (strcmp(*argv, "-d") == 0) {
			dfile = *++argv;
			argc--;
		} else if (strcmp(*argv, "-t") == 0) {
			tfile = *++argv;
			argc--;
		} else if (strcmp(*argv, "-l") == 0) {
			lfile = *++argv;
			argc--;
		} else if (strcmp(*argv, "-c") == 0) {
			oconf = *++argv;
			argc--;
		} else if (strcmp(*argv, "-s") == 0) {
			olow = *++argv;
			argc--;
		} else
			usage();
	}
	readdevs(dfile);
	readtab(tfile);
	readlow(lfile);
	for (p = cfile; *p; p++)
		readconf(*p);
	fixup();
	if (errs)
		exit(1);
	writconf(oconf);
	writlow(olow);
	exit(0);
}

usage()
{
	fprint(STDERR, "usage: mkconf [-d devices] [-l veclist] [-c conf.c] [-s low.s] conf\n");
	exit(1);
}
