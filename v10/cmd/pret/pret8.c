#include <stdio.h>
#include "pret.h"

struct {
	char name[MAXNAME];
	int status;
	int initval;
	int index;	/* set if used as array */
	int width;	/* in bits: for supertrace */
} vartbl[MANY];

struct {
	int m;		/* index of basename in vartbl */
	int n;		/* reference to the index      */
} ivartbl[MANY];
int realnrvars=0;

struct {
	int m;		/* index of basename in globvartbl */
	int n;		/* reference to the index      */
} igvartbl[MANY];
int realgvars=0;

struct {
	char name[MAXNAME];
	int status;
	int initval;
	int index;
	int width;
} globvartbl[MANY];

int nrvars = 0;
int nrglobvars = 0;
int varwidths = 0;

extern char procname[MAXNAME];
extern char refname[MAXNAME];
extern int anyerror, pid, rid;

reallocal(m, n)
{	int i;
	for (i = 0; i < realnrvars; i++)
		if (ivartbl[i].m == m && ivartbl[i].n == n)
			return i;
	if ((i = realnrvars++) >= MANY)
		whoops("reallocal overflow");
	ivartbl[i].n = n;
	ivartbl[i].m = m;
	return i;
}

realglobal(m, n)
{	int i;
	for (i = 0; i < realgvars; i++)
		if (igvartbl[i].m == m && igvartbl[i].n == n)
			return i;
	if ((i = realgvars++) >= MANY)
		whoops("realglobal overflow");
	igvartbl[i].n = n;
	igvartbl[i].m = m;
	return i;
}

twiddle(suff)
	char *suff;
{	int i; extern int extras;

	for (i = 0; i < nrvars; i++)
		if (strcmp(vartbl[i].name, suff) == 0)
		{	vartbl[i].initval++;
			return;
		}
	whoops("cannot happen, twiddle");
}

isarrayvar(suff)
	char *suff;
{	int i;

	for (i = 0; i < nrvars; i++)
		if (strcmp(vartbl[i].name, suff) == 0
		&& vartbl[i].index != NONE)
			return 1;
	for (i = 0; i < nrglobvars; i++)
		if (strcmp(globvartbl[i].name, suff) == 0
		&& globvartbl[i].index != NONE)
			return 1;
	return 0;
}

looklocal(suff, how, index)
	char *suff;
{	int i;

	for (i = 0; i < nrvars; i++)
		if (strcmp(vartbl[i].name, suff) == 0)
		{	vartbl[i].status |= how;
			break;
		}

	if (i != nrvars && how == DCL)
		yyerror("local variable redeclared, %s", suff);

	if (i != nrvars && how == RFR && index != vartbl[i].index
	&& (index == NONE || vartbl[i].index == NONE))
		yyerror("local variable array indexing error, %s", suff);
	return i;
}

lookglobal(suff, how, index)
	char *suff;
{	int i;

	for (i = 0; i < nrglobvars; i++)
		if (strcmp(globvartbl[i].name, suff) == 0)
		{	globvartbl[i].status |= how;
			break;
		}

	if (pid == NONE && rid == NONE && i != nrglobvars && how == DCL)
		yyerror("global variable redeclared, %s", suff);

	if (i != nrglobvars && how == RFR && index != globvartbl[i].index
	&& (index == NONE || globvartbl[i].index == NONE))
		yyerror("global variable array indexing error, %s", suff);
	return i;
}

putlocal(what, ival, how, index, width)
	char *what;
{	int i;

	if ((i = nrvars++) >= MANY)
		whoops("too many variables");

	strcpy(vartbl[i].name, what);
	vartbl[i].initval = ival;
	vartbl[i].status = how;
	vartbl[i].index = index;
	vartbl[i].width = width;
	if (index != NONE && ival != NONE)
	{	fprintf(stderr,"error: no automatic initialization");
		fprintf(stderr, " of array `%s'\n",what);
		anyerror++;
	}
	if (width != 0) varwidths=1;
	return (MANY + i);
}

putglobal(what, ival, how, index, width)
	char *what;
{	int i;

	if ((i = nrglobvars++) >= MANY)
		whoops("too many variables");
	strcpy(globvartbl[i].name, what);
	globvartbl[i].initval = ival;
	globvartbl[i].status = how;
	globvartbl[i].index = index;
	globvartbl[i].width = width;
	if (index != NONE && ival != NONE)
	{	fprintf(stderr,"error: no automatic initialization");
		fprintf(stderr, " of array `%s'\n",what);
		anyerror++;
	}
	if (width != 0) varwidths=1;

	return i;
}
/*
 * arrays are merely patched in for now
 */
addvarname(what, how, ival, index, width)
	char *what;
{
	int i, islocal = (rid != NONE || pid != NONE);

	if (rid != NONE && (i = Fparname(what, rid, ISV, NONE, how, index)) != -1)
		return (2*MANY + i);
	/* formal parameter may not be an array reference */

	if (islocal && (i = looklocal(what, how, index)) < nrvars)
		return (MANY + reallocal(i, index));
	if ((i = lookglobal(what, how, index)) < nrglobvars)
		return realglobal(i, index);

	if (how == RFR)
		yyerror("undeclared variable, %s", what);
	if (islocal)
		return putlocal(what, ival, how, index, width);

	return putglobal(what, ival, how, index, width);
}

checklocvars()
{	int i;

	for (i = 0; i < nrvars; i++)
		if (vartbl[i].status == DCL)
		{	printf("%s: ", (pid != NONE)?procname:refname);
			printf("pvar %s is never used\n", vartbl[i].name);
		}
}

checkglobvars()
{	int i;

	for (i = 0; i < nrglobvars; i++)
		if (globvartbl[i].status == DCL)
		{	printf("global: ");
			printf("pvar %s is never used\n", globvartbl[i].name);
		}
}

numglobvars(fd)
	FILE *fd;
{	int i;

	fprintf(fd, "%d g-variables: ", nrglobvars);
	for (i = 0; i < nrglobvars; i++)
		fprintf(fd, "%d/%d,", globvartbl[i].initval, globvartbl[i].index);
	putc('\n', fd);
	if (varwidths)
	{	fprintf(fd, "%d g-widths: ", nrglobvars);
		for (i = 0; i < nrglobvars; i++)
			fprintf(fd, "%d,", globvartbl[i].width);
		putc('\n', fd);
	}
	fprintf(fd, "%d g-uses: ", realgvars);
	for (i = 0; i < realgvars; i++)
		fprintf(fd, "%d/%d,", igvartbl[i].m, igvartbl[i].n);		
	putc('\n', fd);
}

numlocvars(fd)
	FILE *fd;
{	int i;

	fprintf(fd, "%d l-variables: ", nrvars);
	for (i = 0; i < nrvars; i++)
		fprintf(fd, "%d/%d,", vartbl[i].initval, vartbl[i].index);
	putc('\n', fd);
	if (varwidths)
	{	fprintf(fd, "%d l-widths: ", nrvars);
		for (i = 0; i < nrvars; i++)
			fprintf(fd, "%d,", vartbl[i].width);
		putc('\n', fd);
	}

	fprintf(fd, "%d l-uses: ", realnrvars);
	for (i = 0; i < realnrvars; i++)
		fprintf(fd, "%d/%d,", ivartbl[i].m, ivartbl[i].n);		
	putc('\n', fd);
}
