#include "strip.h"
#include "hash.h"
#include <signal.h>

main(argc, argv)
int argc; char **argv;
{
	register Adotout *a;
	register struct nlist *sym;
	int sighup();

	prognam = *argv;
	++stripflag;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s [options] file.out...\n", prognam);
		exit(1);
	}

	while (--argc > 0 && (*++argv)[0] == '-' && (*argv)[1])
		switch ((*argv)[1]) {
		case 'D':
			++strdebug; break;
		case 'v':
			++prtflag; break;
		case 'g':
			++gflag; /* fall through */
		case 's':
			++shrinkflag; stripflag = 0; break;
		default:
			fatal("unknown option " ,*argv);
		}

	Signal(SIGHUP, sighup);
	Signal(SIGINT, sighup);
	Signal(SIGQUIT, sighup);

	while (--argc >= 0) {
		tmpnam[0] = 0;
		if ((a = rdout(*argv++, !stripflag)) == 0)
			continue;
		if (prtflag)
			prtout(a);
		if (a->hd.a_syms == 0) {
			fprintf(stderr, "%s already stripped\n", a->name);
			freeout(a);
			continue;
		}
		if (shrinkflag)
			shrink(a);
		symwrite(a);
		freeout(a);
	}
	exit(0);
}

fatal(m1, m2)
char *m1, *m2;
{
	if (m1)
		fprintf(stderr,"%s: %s%s\n", prognam, m1, m2);
	if (!copyrace && tmpnam[0])
		unlink(tmpnam);
	exit(1);
}

sighup(s)
{
	if (copyrace) {
		signal(s, sighup);
		return;
	}
	if (tmpnam[0])
		unlink(tmpnam);
	exit(1);
}
