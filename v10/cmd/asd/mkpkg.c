#include "asd.h"

main (argc, argv)
	int argc;
	char **argv;
{
	register int i;
	static int errbuf[BUFSIZ];

	setbuf (stderr, errbuf);

	getargs (argc, argv, "vx:X:D:", (int (*)) 0);

	pkgstart();
	for (i = optind; i < argc; i++)
		pkgfile (argv[i]);

	i = pkgend();

	return i;
}
