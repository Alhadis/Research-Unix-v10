/*
 *	Generic dialer routine.  Usage:
 *		dialer mount-pt [unit]
 */
#include <stdio.h>
#include <ipc.h>

/* export */
char *av0;
int debug;

/* cheap foiegn imports */
extern void dodialout();

usage(name)
{
	fprintf(stderr, "usage: %s [-m mount-pt]\n", name);
	exit(1);
}

main(ac, av)
	int ac;
	char *av[];
{
	char *mtpt="tpc", *cp;
	int ai;

	av0 = av[0];
	chdir("/cs");
	for (ai=1; ai<ac; ai++) {
		if (av[ai][0] == '-')
			for (cp=&av[ai][1]; *cp; cp++) {
				switch(*cp) {
				case 'd':
					debug = 1;
					break;
				case 'm':
					if (ai+1>=ac || *cp!='m')
						usage(av0);
					mtpt = av[++ai];
					break;
				default:
					usage(av0);
				}
			}
		else
			usage(av[0]);
	}
	if (!debug)
		detach(mtpt);
	for(;;)
		dodialout(mtpt);
}
