#include "strip.h"

main(argc, argv)
int argc; char **argv;
{
	register Adotout *a;
	register struct nlist *sym;

	prognam = *argv;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s file.out...\n", prognam);
		exit(1);
	}

	while (--argc > 0 && (*++argv)[0] == '-' && (*argv)[1])
		switch ((*argv)[1]) {
		case 'D':
			++strdebug; break;
		default:
			fatal("unknown option " ,*argv);
		}

	while (--argc >= 0) {
		if ((a = rdout(*argv++, 1)) == 0)
			continue;
		prtout(a);
		for (sym=a->symtab; sym<a->symend; sym++) {
			if (sym->n_un.n_name)
				sym->n_un.n_name += (int)a->strtab;
			prtsym(stdout,sym);
		}
		freeout(a);
	}
	exit(0);
}

fatal(m1, m2)
char *m1, *m2;
{
	if (m1)
		fprintf(stderr,"%s: %s%s\n", prognam, m1, m2);
	exit(1);
}
