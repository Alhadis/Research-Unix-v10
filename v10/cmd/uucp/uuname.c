/*	/sccs/src/cmd/uucp/s.uuname.c
	uuname.c	1.1	8/30/84 17:38:10
*/
#include "uucp.h"
VERSION(@(#)uuname.c	1.1);
 
/*
 * returns a list of all remote systems.
 * option:
 *	-l	-> returns only the local system name.
 */
main(argc,argv, envp)
int argc;
char **argv, **envp;
{
	FILE *np;
	register int nexpl = 0;
	register int err = 0;
	char s[BUFSIZ], prev[BUFSIZ], name[BUFSIZ];
#ifdef MANYSYS
	FILE *sysopen();
#endif

	while (--argc > 0) {
		if (strcmp(*++argv, "-l") == 0) {
			nexpl++;
			uucpname(name);
			/* initialize to null string */
			(void) printf("%s",name);
			(void) printf("\n");
		}
		else {
			nexpl++;
			if (versys(*argv, 0) != FAIL)
				printf("%s\n", argv[0]);
			else
				err++;
		}
	}
	if (nexpl)
		exit(err);
#ifndef MANYSYS
#define	SYSCLOSE	fclose
	np=fopen(SYSFILE, "r");
#else
#define	SYSCLOSE	pclose
	np = sysopen("---");
#endif
	if ((np) == NULL) {
		(void) fprintf(stderr, "Can't read Systems files");
		exit(1);
	}
	while (fgets(s, BUFSIZ, np) != NULL) {
		if((s[0] == '#') || (s[0] == ' ') || (s[0] == '\t') || 
		    (s[0] == '\n'))
			continue;
		(void) sscanf(s, "%s", name);
		if (EQUALS(name, prev))
		    continue;
		(void) printf("%s", name);
		(void) printf("\n");
		(void) strcpy(prev, name);
	}
	SYSCLOSE(np);
	exit(err);
}
