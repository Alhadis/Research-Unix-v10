#include <stdio.h>
#include "pret.h"

struct PROCTABLE proctable[MAXPROC];		/* index to process names */

int nrprocs = 0;

procsize(n, m, k)
{	proctable[n].nrstates = m;
	proctable[n].unreach = k;
}

newprocname(str, repl)
	char *str;
{ register int i;

	for (i = 0; i < nrprocs; i++)
		if (strcmp(str, proctable[i].name) == 0)
			break;

	if (i == nrprocs)
	{	if (nrprocs >= MAXPROC)
			whoops("too many processes");
		proctable[i].nrstates = 0;
		proctable[i].unreach = 0;
		proctable[i].replic = repl;
		strcpy(proctable[nrprocs++].name, str);
	} else
		yyerror("process redeclared, %s", str);

	return i;
}

numprocs(fd)
	FILE *fd;
{
	extern int extras;
	fprintf(fd, "%d processes\n", nrprocs+extras);
}

chatter()
{	int i;
	extern int verbose, nrqs, nrmesgs, nrrefs, assertion, inertion, extras;

	if (verbose)
	{	i = ((assertion != -1) + (inertion != -1));
		printf("\nOverview:\n========\n");
		printf("%d queue%s:\n", nrqs, (nrqs==1)?"":"s");
		listqs();
		printf("%d process%s:\n",
				nrprocs+extras, (nrprocs+extras==1)?"":"es");
		listprs();
		printf("%d procedure%s:\n", nrrefs, (nrrefs==1)?"":"s");
		listrefs();
		printf("%d assertion%s\n", i, (i==1)?"":"s");
	}
}

listprs()
{	int i, k;

	for (i = 0; i < nrprocs; i++)
	{	printf("\t%2d", i+1);
		if (proctable[i].replic != NONE)
			printf("x%d", proctable[i].replic);
		printf("\t%s, ", proctable[i].name);
		for (k = strlen(proctable[i].name)+1; k < 10; k++)
			putchar(' ');
		k = proctable[i].nrstates;
		printf("%d state%s", k, (k!=1)?"s":"");
		if ((k = proctable[i].unreach) > 0)
			printf(" (%d unreachable state%s)", k, (k>1)?"s":"");
		putchar('\n');
	}
}
