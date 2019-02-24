#include <stdio.h>
#include "pret.h"
#include "pret.d"

 struct COL column[MANY];
 struct ROW row[MANY];

 struct ENTRY *rowtail;
 struct ENTRY *coltail;
 struct ENTRY *base;		/* base of transition table */

 struct ENTRY *newentry();
 struct PILAR *newunit();

 int nrrows = 0;	/* raw nr of rows (incremented by addrow() */
 int nrcols = 0;	/* number of columns per table             */
 int redrows = 0;	/* reduced nr of rows (set by remap()      */
 int extras = 0;	/* counts replicated processes		   */

extern char procname[MAXNAME], refname[MAXNAME];
extern int anyerror, pid, rid, curstate, lastloop;

struct ENTRY *
find(R, C)
{ register int i;
  register int r = R;
  register int c = C;
  register struct ENTRY *here = base;

	if (r >= nrrows || c >= nrcols || r < 0 || c < 0)
		whoops ("bad table index");
	for (i = 0; i < r; i++)
		here = here->nextrow;
	for (i = 0; i < c; i++)
		here = here->nextcol;
	return here;
}

getrowname(where, which)
	char *where;
{	strcpy(where, row[which].name);
}

setrowname(which)
{ char stro[256];
  extern linenumber;
  extern char filename[];

	sprintf(stro, "%d/%d:%s", linenumber, nrrows, filename);
	strcpy(row[which].name, stro);
}

deadlabels()
{ register int i;
	for (i = 0; i < nrrows; i++)
	switch(row[i].refcount)
	{	case ADR: printf("%s: undeclared label\n", row[i].name);
			  anyerror++;
			  break;
		case DCL: printf("%s: unused label\n", row[i].name);
			  break;
		case ADR+DCL:
			  break;
		default:  whoops("cannot happen - label");
	}
}

remap()
{ register int i, j;

	for (i = j = 0; i < nrrows; i++)
		if (row[i].mapping == NOSTATE)
			row[i].maptwo = j++;
	redrows = j;
}

purgerows(how)
{ int i; extern int linecode, assertion, inertion;

	for (i = 0; i < nrrows; i++)
	{	row[i].mapping = NOSTATE;
		row[i].maptwo = i;
	}
	redrows = nrrows;
/* -> bug if (!linecode) */
	if (!linecode || rid==assertion || rid==inertion)
		transitrows();	/* remove dummy transitions */
	remap();

	if (!linecode && !how)
	{	minimize();
		remap();
	}
}

target(y)
{ int i = 0;
  int x = y;
	while (row[x].mapping != NOSTATE)
	{	x = row[x].mapping;
		if (i++ > nrrows)
		{	printf("%s: ", (pid == NONE) ? procname : refname);
			whoops("contains unconditional, infinite loop");
		}
	}
	return x;
}

enterowname(how, stri, bang)
	char *stri, bang;
{ register int i;
  char stro[256];
  extern int linenumber;
  extern char filename[];

	switch(how) {
	case LAB:	sprintf(stro, "%s: %s", stri, filename);
			for (i = 0; i < nrrows; i++)
				if (strcmp(stro, row[i].name) == 0)
					break;

			if (i < nrrows)
			{	if (bang == DCL && (row[i].refcount & DCL))
				{	fprintf(stderr, "%d/%d %s %s\n",
						i, nrrows, stro, row[i].name);
					yyerror("label redeclared, %s", stro);
				}else
					row[i].refcount |= bang;
				break;
			} else
			{	addrow();
				row[i].refcount = bang;
				row[i].labeled = 0;
				strcpy(row[i].name, stro);
			}
			break;
	case NEW:	i = nrrows;
			sprintf(stro, "%d/%d:%s", linenumber, i, filename);
			addrow();
			row[i].refcount = bang;
			row[i].labeled = 0;
			strcpy(row[i].name, stro);
			break;
	case OLD:	for (i = 0; i < nrrows; i++)
				if (strcmp(stri, row[i].name) == 0)
					break;
			if (i == nrrows)
				whoops("cannot happen - rows");
			break;
	}
	return i;
}

labelrow(n)
{
	if (n < 0 || n >= nrrows)
		whoops("cannot happen - labelrow");
	row[n].labeled = 1;

}

deadrows(p, r)
{ register int i, j, k;
  struct ENTRY *it;
  struct PILAR  *at;
  char fld[128];
  extern int nrprocs;

	for (i = 0; i < nrrows; i++)
		row[i].reached = 0;

	for (i = 0; i < nrrows; i++)
	{	if (row[i].mapping != NOSTATE)
			continue;
		it = find(i, 0);
		for (j = 0; j < nrcols; j++, it = it->nextcol)
		{	at = it->pilar;
			do
			{	if (at->transf != NOSTATE)
				{	k = target(at->transf);
					if (k != i)
						row[k].reached = 1;
				}
				at = at->nxtp;
			} while (at != NULL);
	}	}

	fld[0] = '\0';
	for (i = k = 0; i < nrrows; i++)
	{	if (row[i].mapping != NOSTATE)
			continue;
		if (row[i].reached == 0 && row[i].maptwo != 0)
		{	j = strlen(fld); k++;
			sprintf(&fld[j], "%d, ", row[i].maptwo);
	}	}

	if (p == NONE)
		refsize(r, redrows, k);
	else
		procsize(p, redrows, k);
}

deadends(tb)
	FILE *tb;
{ register int i, j, x;
  struct ENTRY *it;
  struct PILAR  *at;
  char field[256];
  int nrdeads = 0;

	if (nrrows == 0)
		return;

	field[0] = '\0';
	for (i = 0; i < nrrows; i++)
	{	if (row[i].mapping != NOSTATE)
			continue;
		it = find(i, 0);
		for (j = 0; j < nrcols; j++, it = it->nextcol)
		{	for (at = it->pilar; at != NULL; at = at->nxtp)
				if (at->transf != NOSTATE)
					break;
			if (at != NULL)
				break;
		}
		if (j == nrcols)
		{	x = strlen(field);
			sprintf(&field[x], "%d,", row[i].maptwo);
			nrdeads++;
	}	}
	if ((i = lastloop) >= 0)
	{	if (i >= nrrows || row[i].mapping != NOSTATE)
			whoops("cannot happen - deadends");
		x = strlen(field);
		sprintf(&field[x], "%d,", row[i].maptwo);
		nrdeads++;
	}
	fprintf(tb, "ENDSTATES %d: %s\n", nrdeads, field);
}

transitrows()
{ register int i, j;
  struct ENTRY *here, *there;
					/* maintain row 0 as initial state */
	here = base->nextrow;
	for (i = 1; i < nrrows; i++, here = here->nextrow)
	{	if (here->nrpils != 1 || here->pilar->code != NONE)
			continue;

		there = here->nextcol;
		for (j = 1; j < nrcols; j++, there = there->nextcol)
			if (there->nrpils != 0)
				break;
		if (j == nrcols)
		{	row[i].mapping = here->pilar->transf;
			row[here->pilar->transf].labeled |= row[i].labeled;
	}	}
}

entercolname(val, what)
{ register int i;

	for (i = 0; i < nrcols; i++)
		if (column[i].ccode == val && column[i].coltype == what)
			break;
	if (i == nrcols)
	{	addcol();
		column[i].ccode = val;
		column[i].coltype = what;
	}
	return i;
}

addrow()
{ register i = nrcols;
  struct ENTRY *lastcol, *thiscol;
  char uni[32];

	if (nrcols == 0)
	{	rowtail = coltail = base = newentry();
		sprintf(uni, "%4d", curstate);
		column[0].ccode = -1;
		column[0].coltype = SPN;
		strcpy(row[0].name, uni);
		nrrows = nrcols = 1;
	} else
	{	if (nrrows == MANY)
			whoops("too many rows");
		lastcol = rowtail;
		rowtail = thiscol = lastcol->nextrow = newentry();
		for (--i; i > 0; i--)
		{	thiscol = thiscol->nextcol = newentry();
			lastcol = lastcol->nextcol;
			lastcol->nextrow = thiscol;
		}
		nrrows++;
	}
}

addcol()
{ register int i = nrrows;
  struct ENTRY *lstrow, *thisrow;

	if (nrcols == 0)
		whoops("cannot happen - columns");
	else
	{	lstrow = coltail;
		coltail = thisrow = lstrow->nextcol = newentry();
		if (nrcols++ == MANY)
			whoops("too many columns");
		for (--i; i > 0; i--)
		{	thisrow = thisrow->nextrow = newentry();
			lstrow = lstrow->nextrow;
			lstrow->nextcol = thisrow;
	}	}
}

setrans(r, c, tr, to)
{ struct ENTRY *ft;
  struct PILAR  *pt;
  int i;
	ft = find(r, c);
	pt = ft->pilar;
	for (i = ft->nrpils++; i > 0; i--)
		pt = pt->nxtp;

	pt->code = to;
	pt->transf = tr;

	pt->nxtp = newunit();
}

badstates(tb)
	FILE *tb;
{ struct ENTRY *this, *that;
  struct PILAR *at;
  int i, k, x, nrbads = 0, nrlabs = 0;
  char field[512], labfield[512];

	if (nrrows == 0)
		return;

	labfield[0] = field[0] = '\0';
	for (i = 0, this = base; this != NULL; this = this->nextrow, i++)
	{	if (row[i].mapping != NOSTATE)
			continue;

		if (row[i].labeled)
		{	sprintf(&labfield[strlen(labfield)], "%d,", row[i].maptwo);
			nrlabs++;
		}
		that = find(i, 0);
		for (k = 0; k < nrcols; k++, that = that->nextcol)
		{	if (!SPNT(column[k].coltype))
				continue;

			for (at = that->pilar, x = 0; at != NULL; at = at->nxtp)
			{	if (at->transf == NOSTATE)
					continue;
				x++; break;
			}
			if (x > 0)
			{	sprintf(&field[strlen(field)],"%d,",row[i].maptwo);
				nrbads++;
				break;
		}	}
	}
	fprintf(tb, "BADSTATES %d: %s\n", nrbads, field);
	fprintf(tb, "LABSTATES %d: %s\n", nrlabs, labfield);
}

dumpforw(tb)
	FILE *tb;
{ struct ENTRY *this, *that;
  struct PILAR *at;
  char field[256];
  int i, j, k, x, y; extern int linecode;

	if (pid == NONE)
		fprintf(tb, "%s %d:", "REF", rid);
	else
		fprintf(tb, "%s %d:", "PROC", pid+extras);

	fprintf(tb, "%d/%d:", redrows, nrcols);
	for (i = 0; i < nrcols; i++)
		fprintf(tb, "%d(%d),", column[i].ccode, column[i].coltype);
	putc('\n', tb);

	for (i = 0, this = base; this != NULL; this = this->nextrow, i++)
	{	if (row[i].mapping != NOSTATE)
			continue;

		that = find(i, 0);
		for (k = 0; k < nrcols; k++, that = that->nextcol)
		{	y = 0; field[0] = '\0';
			for (at = that->pilar; at != NULL; at = at->nxtp)
			{	if (at->transf == NOSTATE)
					continue;

				j = row[ target(at->transf) ].maptwo;

				if ((x = strlen(field)) > 200)
					whoops("string overflow");

				sprintf(&field[x], "[%d,%d]", j, at->code);
				y++;
		   	}

			if (y > 0)
		   	{	fprintf(tb, "%d/%d ", row[i].maptwo, k);
				fprintf(tb, "(%d) %s\n", y, field);
	}	}	}
	fprintf(tb, "0/0 (0)\n");

	if (linecode)
	{	fprintf(tb, "rownames:\n");
		for (i = 0; i < redrows; i++)
			fprintf(tb, "%s\n", row[i].name);
	}
}

wrapup(r, p, tb, np, vb)
	FILE *tb;
{
	int i;
	extern struct PROCTABLE proctable[MAXPROC];

	purgerows(np);
	dumpforw(tb);
	deadends(tb);
	badstates(tb);
	putcalls(tb);
	numlocvars(tb);

	if (r == NONE)
	for (i = 1; i < proctable[p].replic; i++)
	{	extras++;
		twiddle("_PROCID");
		dumpforw(tb);
		deadends(tb);
		badstates(tb);
		putcalls(tb);
		numlocvars(tb);
	}

	if (vb)
	{	deadrows(p, r);
		deadlabels();
	}

	release();
	scrapcalltable();
}
