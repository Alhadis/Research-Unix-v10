#include "ts.h"
#include <stdio.h>

int verbose = 0, cyclep = 1, verify = 0;
char *progname = 0;

main(argc,argv)
int argc;
char **argv;
{
	char name[100], *s;
	int c, odd;
	struct obj *o, *ts();
	struct ref *r;
	extern struct ref *undefrefs;
	extern struct obj *obj0;

	progname = argv[0];
	if (argc >= 2 && !freopen(argv[1], "r", stdin)) {
		fprintf(stderr, "%s: cannot open %s\n", progname, argv[1]);
		exit(1);
		}
	init();
	odd = 1;
	do {
		
		while((c = getchar()) <= ' ') if (c == EOF) goto eof;

		s = name;
		do *s++ = c; while((c = getchar()) > ' ');
		*s = 0;
		if (odd) defobjref(name,1);
		else newref(name);
		odd = 1 - odd;
	} while (c != EOF);
eof:
	if (!odd) {
		fprintf(stderr, "%s: odd data\n", progname);
		exit(1);
		}
	for (o = ts(1); o; o = o->next)  if (o != obj0) puts(o->name);
	for (r = undefrefs; r; r = r->next) puts(r->name);
	return 0;
	}
