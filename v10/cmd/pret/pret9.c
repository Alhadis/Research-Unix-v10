#include <stdio.h>
#include "pret.h"

#define MAXACTUALS	(MANY)

 struct ACTUALS {
	int typefs[MAXACTUALS];		/* queue mesg or pvar */
	int index [MAXACTUALS];		/* qid,  if mesg      */
	struct ACTUALS *nxtcall;
	int nractuals;
	int whattask;			/* id of task called  */
 };

 struct ACTUALS *calls;		/* procedure call data   */
 int nrcalls;			/* nr of procedure calls */

 extern pid, rid, linenumber;

newcalltable()
{	nrcalls = 0;
	calls = NULL;
}

scrapcalltable()
{ struct ACTUALS *tmp;
  struct ACTUALS *hook = calls;
  int i;

	for (i = 0; i < nrcalls; i++)
	{	tmp = hook;
		hook = hook->nxtcall;
		free(tmp);
	}
}

newcall(ofwhat)
{ int i, j;
  struct ACTUALS *new, *old;

	new = ( struct ACTUALS *)
		Emalloc( sizeof(struct ACTUALS) );
	new->nractuals = 0;
	new->whattask = ofwhat;

	if ((j = nrcalls++) == 0)
		calls = new;
	else
	{	old = calls;
		for (i = 1; i < j; i++)
			old = old->nxtcall;
		old->nxtcall = new;
	}
	return nrcalls-1;
}

putcalls(tb)
	FILE *tb;
{ struct ACTUALS *hook;
  int j, k, a, b;
  int maxm = 0;
  int maxv = 0;

	fprintf(tb, "FCT_CALLS %d\n",  nrcalls);
	for (j = 0, hook = calls; j < nrcalls; j++)
	{
		for (k = a = b = 0; k < hook->nractuals; k++)
		{	if (hook->typefs[k] == ISM)
				a++;
			else if (hook->typefs[k] == ISV)
				b++;
		}
		if (a > maxm) maxm = a;
		if (b > maxv) maxv = b;

		fprintf(tb, "%d-%d/%d:", hook->whattask, a, b);
		for (k = 0; k < hook->nractuals; k++)
		{	if (hook->typefs[k] != ISQ)
			fprintf(tb, " %d/%d", hook->typefs[k], hook->index[k]);
		}
		putc('\n', tb);
		hook = hook->nxtcall;
	}
	fprintf(tb, "PARS %d/%d\n",  maxm, maxv);
}

callentry(x, val)
{ int i;
  struct ACTUALS *hook = calls;

	for (i = 1; i < nrcalls; i++)
		hook = hook->nxtcall;

	if ((i = hook->nractuals++) >= MAXACTUALS)
		whoops("too many parameters");
	hook->typefs[i] = x;
	hook->index[i] = val;
}
