#include "asd.h"
#include <libv.h>
#include <stdio.h>

int
main (int argc, char **argv)
{
	register int i;
	static char errbuf[BUFSIZ];

	setbuf (stderr, errbuf);

	getargs (argc, argv, "vx:X:D:", 0);

	pkgstart();
	for (i = optind; i < argc; i++)
		pkgfile (argv[i]);

	i = pkgend();

	return i;
}
