#include <stdio.h>
#include "pret.h"

 struct FORMALS {
	char name[MAXNAME];
	int typefs;		/* qset or pvar	(default)   */
	int tag;		/* id			    */
	char used;		/* reference count	    */
	char reset;		/* default type overwritten */
	struct FORMALS *nxtpar;
 };

 struct {
	char name[MAXNAME];
	int status;
	int nrparams;
	int nrstates;
	int unreach;
	struct FORMALS *params;	/* formal parameters */
 } reftable[MAXPROC];

 int nrrefs = 0;

 extern int anyerror, pid, rid, assertion, inertion;
 extern char qsetname[MAXNAME];

refsize(n, m, k)
{	reftable[n].nrstates = m;
	reftable[n].unreach = k;
}

reorder()
{ int i, j, N, M;
  struct FORMALS * hook;

	if (rid == NONE)
		return;
	/*
	 * make sure that formal msg parameters
	 * are numbered in the order in which they
	 * will be pushed onto the call stack in trace.c
	 */

	j = reftable[rid].nrparams;
	hook = reftable[rid].params;

	for (i = N = M = 0; i < j; i++)
	{	if (hook->typefs == ISQ || hook->typefs == ISQN)
			N = renumqset(hook->tag, N);
		else
			hook->tag = M++;

		hook = hook->nxtpar;
	}
}

newreftask(str, mask)
	char *str;
{ register int i;

	for (i = 0; i < nrrefs; i++)
		if (strcmp(str, reftable[i].name) == 0)
			break;
	if (i == nrrefs)
	{	if (nrrefs >= MAXPROC)
			whoops("too many procedures");
		if (mask == RFR)
			yyerror("undeclared procedure, %s", str);

		reftable[i].status = 0;
		reftable[i].nrparams = 0;
		reftable[i].nrstates = 0;
		reftable[i].unreach = 0;
		strcpy(reftable[nrrefs++].name, str);
	} else
	{	if (mask == DCL && (reftable[i].status & DCL))
			yyerror("procedure redeclared, %s", str);
		if (mask == RFR && rid == i)
			yyerror("recursive procedure, %s", str);
	}
	reftable[i].status |= mask;

	return i;
}

struct FORMALS *
newparunit(str, nn, tp, ins)
	char *str;
{ struct FORMALS * try = (struct FORMALS *)
		Emalloc ( sizeof(struct FORMALS) );

	try->used  = 0;
	try->reset = 0;

	strcpy(try->name, str);

	try->typefs = tp;
	try->nxtpar = NULL;

	try->tag = nn;
	if (!ins)
		yyerror("unspecified parameter, %s", str);

	return try;
}

addFpar(to, stri, n, vt, ins)
	char *stri;
{ struct FORMALS *hook;
  int N = reftable[to].nrparams;
  int i = N;

	if (N == 0)
	{	reftable[to].params = newparunit(stri, n, vt, ins);
		reftable[to].nrparams++;
	} else
	{	hook = reftable[to].params;
		for (i = 0; i < N; i++, hook = hook->nxtpar)
		{	if (strcmp(hook->name, stri) == 0)
			{	if (ins || hook->reset == 1)
					yyerror("name clash, %s", stri);

				hook->tag = n;
				hook->typefs = vt;
				hook->reset = 1;
				break;
			}
			if (hook->nxtpar == NULL)
			{	hook->nxtpar = newparunit(stri, n, vt, ins);
				reftable[to].nrparams++;
				i++;
				break;
	}	}	}

	return i;
}

Fparname(str, which, vt, hit, how, qind)
	char *str;
{ int i, j, k;
  struct FORMALS * hook = reftable[which].params;

	for (i = 0, j = reftable[which].nrparams; i < j; i++)
	{	if (strcmp(str, hook->name) == 0 &&  hook->typefs == vt)
			break;
		if (hook->typefs == ISQ || hook->typefs == ISQN)
		{	switch (vt) {
			case ISM:
				if (hook->tag == hit
				&& (k = inqset(hook->tag, str, how, qind)) != -1)
					return k;
				break;
			case ISQ:
				if (matchowner(hook->tag, str, how, qind))
					return hook->tag;	/* id of qset */
				break;
		}	}
		hook = hook->nxtpar;
	}
	if (i == j || (vt == ISM && hook->tag != hit))
		return -1;
		
	hook->used |= how;

	if (vt == ISV)
		return hook->tag;

	return i;
}

checkrefs()
{ int i;
	if (nrrefs == 0)
		return;
	for (i = 0; i < nrrefs; i++)
		if (reftable[i].status == DCL)
		{	if (strcmp(reftable[i].name, " assert") != 0
			&&  strcmp(reftable[i].name, " error") != 0)
				printf("%s: unused procedure\n", reftable[i].name);
		} else if (reftable[i].status == RFR)
			printf("%s: undeclared procedure\n", reftable[i].name);
}

numrefs(fd)
	FILE *fd;
{	extern int linecode;
	fprintf(fd, "%d linecode\n", linecode);
	fprintf(fd, "%d procedures ", nrrefs);
	fprintf(fd, "(assert %d/%d)\n", assertion, inertion);
}

parrefs(n, m)
{ int i;
	if ((i = reftable[m].nrparams - n) == 0)
		return;
	if (i > 0)
		yyerror("missing parameters, %s", reftable[m].name);
	else
		yyerror("too many parameters, %s", reftable[m].name);
}

isdigit(c) {	return (c >= '0' && c <= '9'); }

addAspecial(val, which, pn)
{ struct FORMALS * hook = reftable[which].params;
  int i;

	if (pn >= reftable[which].nrparams || pn < 0)
		return;

	for (i = 0; i < pn; i++)
		if ((hook = hook->nxtpar) == NULL)
			whoops("cannot happen - addAspecial");

	switch (hook->typefs)
	{	case ISQN:
		case ISQ: yyerror("mismatched parameter, %s", "queue or qset");
			  break;
		case ISV: callentry(ISV, val);
			  break;
		default : whoops("cannot happen - addAspecial");
	}
}

addApars(what, which, pn, index)
	char *what;
{ struct FORMALS * hook = reftable[which].params;
  int val, hit, x;

	if (pn >= reftable[which].nrparams || pn < 0)
		return;

	for (x = 0; x < pn; x++)
		if ((hook = hook->nxtpar) == NULL)
			whoops("cannot happen - formals");

	switch (hook->typefs)
	{	case ISQN:
			  val = newqset(what, what, 0, index);
			  x = matchset(val, hook->tag);
			  hit = qsetowner(val, x);	/* find queue-id  */
			  callentry(ISQ, hit);		/* enter queue-id */
			  callist(val, hit);		/* enter messages */
			  break;
		case ISQ: val = newqset(what, "", RFR, index);
			  x = matchset(val, hook->tag);
			  hit = qsetowner(val, x);	/* find queue-id  */
			  callentry(ISQ, hit);		/* enter queue-id */
			  callist(val, hit);		/* enter messages */
			  break;
		case ISV: val = addvarname(what, RFR, NONE, index, 0);
			  callentry(ISV, val);
			  break;
		default : whoops("cannot happen - addApars");
	}
}

listrefs()
{	int i, k;

	for (i = 0; i < nrrefs; i++)
	{	printf("\t%2d\t%s, ", i+1, reftable[i].name);
		for (k = strlen(reftable[i].name)+1; k < 10; k++)
			putchar(' ');
		k = reftable[i].nrstates;
		printf("%d state%s", k, (k!=1)?"s":"");
		if ((k = reftable[i].unreach) > 0)
			printf(" (%d unreachable state%s)", k, (k>1)?"s":"");
		putchar('\n');
	}
}
