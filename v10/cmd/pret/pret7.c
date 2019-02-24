#include <stdio.h>
#include "pret.h"
#include "pret.d"

extern FILE *tb;
extern int anyerror, linenumber;
extern int nrrows, nrcols, curstate, curdepth;
extern int nrvars, realnrvars;
extern struct ENTRY *base;
extern char filename[256];

char *
Emalloc(N)
	unsigned N;
{ char *try, *malloc();
	if ((try = malloc(N)) == NULL)
		whoops("out of memory");
	return try;
}

whoops(s)
	char *s;
{	yyerror(s, "aborting");
	fclose(tb);
	unlink("pret.tmp");
	exit(1);
}

yyerror(s1, s2)
	char *s1, *s2;
{
	char buf[512];
	sprintf(buf, s1, s2);

	printf("\"%s\", line %2d: %s\n", filename, linenumber, buf);
	fflush(stdout);
	anyerror++;
}

warning(s1, s2)
	char *s1, *s2;
{
	printf("\"%s\", line %2d, warning: %s", filename, linenumber, s1);
	if (strlen(s2) > 0 && strcmp(s1, "syntax error") != 0)
		printf(" (%s)\n", s2);
	else
		printf("\n");
}

release()
{ register struct ENTRY *this, *temp1, *temp2;
  register struct PILAR  *that, *temp3;

	this = base;
	while (this != NULL)
	{	temp1 = this->nextrow;
		do
		{	temp2 = this->nextcol;
			that = this->pilar;
			do
			{	temp3 = that->nxtp;
				free(that);
				that = temp3;
			} while (that != NULL);
			free(this);
			this = temp2;
		} while (this != NULL);
		this = temp1;
	}
	nrrows = nrcols = curstate = curdepth = 0;
	nrvars = realnrvars = 0;
}

struct PILAR *
newunit()
{ struct PILAR  *try;

	try = (struct PILAR *) Emalloc(sizeof(struct PILAR));

	try->transf = NOSTATE;
	try->code = NONE;
	try->nxtp = NULL;

	return try;
}

struct ENTRY *
newentry()
{ struct ENTRY *try;

	try = (struct ENTRY *) Emalloc(sizeof(struct ENTRY));

	try->pilar = newunit();
	try->nrpils = 0;
	try->nextrow = try->nextcol = NULL;
	return try;
}
