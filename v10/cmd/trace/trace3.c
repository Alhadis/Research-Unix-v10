#include <stdio.h>
#include "trace.h"
#include "trace.d"

#define DEBUG	0

#define BS(n)	((n) & ~(PASSED))

 extern struct TBL       *tbl;
 extern struct LBT	 *lbt;
 extern struct MBOX      *mbox;
 extern struct MNAME     *fullname;
 extern struct PROCSTACK **procstack;

 extern struct VARPARS	 *procpars;
 extern struct TBLPARS	 *tblpars;

 extern struct LOCVARS   *tblvars;
 extern struct TBLPARS   *tablpars;

 extern struct QUEUE **starter, **head, **tail;
 extern struct QUEUE *s_first, *s_last;
 extern struct VISIT *lastvisit;

 extern int *reftasks, *processes, *basics;
 extern int *globvars, *inits, *state, *qsize;

 extern int nrtbl, nrqs, nrrefs, nrprocs, nrinit;
 extern int nrvars, nrmesgs, msgbase;
 extern int maxlevel, maxreached;

 extern char noshortcut, prbyq, timedd, blast, qandirty, muststore;
 extern char maxxed, completed, lockplus, firstlock;

 extern long locksf, normf, loopsf;
 extern double zapper;

 short lastqueue;		/* the last queue addressed */
 int level = 0;
 double COUNT = 0;

 char *emalloc(), *Realloc(), *Emalloc();

determine(m)
{
	if (m >= 3*MANY)	return 0;	/* constant  */
	else if (m >= 2*MANY)	return 1;	/* parameter */
	else if (m >= MANY)	return 2;	/* local     */
	else if (m >= 0)	return 3;	/* global    */
	else if (m < -3*MANY)	return 5;	/* negative number */
	else if (m <= -2)	return 4;	/* expression */
	else
		whoops("cannot happen - determine");
}

convert(m, pr)
{ int res, n = m;

	/* convert a pvar: global, local, parameter, or a constant */

	switch (determine(n)) {
	case 5: res = n + 3*MANY; break;
	case 4: res = evalcond(-(m+2), pr); break;
	case 0: res = n - 3*MANY; break;
	case 1: res = wapper(m, pr); break;
	case 2: n -= MANY;
		if (n >= 0 && n < (int) procpars[pr].nrlvars)
			res = (int) procpars[pr].lvarvals[n];
		else
			whoops("reference to non-existing local var");
		break;
	case 3:	if (n >= 0 && n < nrvars)
			res = globvars[n];
		else
			whoops("reference to non-existing global var");
		break;
	default:
		whoops("cannot happen - convert");	/* got a parameter */
		break;
	}
	return res;
}

wapper(n, pr)
{ int x = n - 2*MANY;
  int y;
	if (x < 0 || x >= MANY)
		return n;	/* not a parameter */
	else
	{	if (x >= procpars[pr].nrvs)
			whoops("unknown parameter of type pvar");
		y = (int) procpars[pr].vs[x];
		if (y >= 2*MANY && y < 3*MANY)
			whoops("unresolved parameter reference");
		return y;
	}
}

mapper(n, pr)			/* convert a message parameter */
{ register int x = n - MANY;

	if (x < 0 || x >= MANY)
		return n;	/* not a parameter */
	else
	{	if (x >= procpars[pr].nrms)
			whoops("bad news: illegal message parameter");
		return ((int)procpars[pr].ms[x]);
	}
}

matchon(n, m, t, trial, TT, pr)
{
#if DEBUG
	printf("trial %d, pr %d; ", trial, pr);
	if (TT == INP || TT == OUTP)
		printf("msg %s, ", fullname[n-msgbase].mname);
	else
		printf("code %d, ", n);
	printf("q %d, tbl %d, type %d\n", m, t, TT);
#endif

	if (trial == 2)
		return (TT == TMO && qsize[m] == 0);
	else
	{
		switch (TT) {
		case FCT :
		case SPN : return 1;
		case CND : return (evalcond(n, pr));
		case DFL : return (qsize[m] > 0 && no_other(t,head[m]->mesg,pr));
		case INP : return (qsize[m] > 0 && BS(head[m]->mesg) == n);
		case TMO : return (noshortcut && qsize[m] == 0);
		case OUTP: return (qsize[m] < mbox[m].limit);
		default  : whoops("unknown type in column head");
		}
	}
}

no_other(x, yy, pr)
{ struct TBL *tmp = &(tbl[x]);
  int y = BS(yy);
  int j = tmp->nrcols;
  int h = state[pr];
  int i;

	for (i = 0; i < j; i++)
	{	if (tmp->coltyp[i] == INP
		&&  tmp->ptr[h][i].nrpils > 0
		&&  y == lbt[pr].mapcol[i])
			return 0;
	}
	return 1;
}

send(m, to, with, pr, nst)
{ struct QUEUE *tmp;
  register struct QUEUE *hook = tail[to];
  int what = NONE;

	if (with != NONE)
	{	if ((what = convert(with, pr)) >= USED - 1 || what < 0)
		{	output("aborted: ", 0);
			whoops("cargo too large or negative");
		}
		hook->cargo = (unsigned short) ( what | USED );
	} else
		hook->cargo = (unsigned short) 0;

	if (qsize[to] >= mbox[to].limit)
		whoops("cannot happen - send");

	tmp = (struct QUEUE *) emalloc(sizeof(struct QUEUE));
	tmp->last  = hook;
#if TRAIL
	hook->tstate[0] = (short) processes[pr];
	hook->tstate[1] = (short) nst;
#endif TRAIL

	hook->mesg = (short) m;
	hook->next = tmp;
	hook->s_back = s_last;
	hook->s_forw = NULL;
	if (s_last == NULL)
		s_first = hook;
	else
		s_last->s_forw = hook;

	s_last = hook;
	tail[to] = tmp;
	qsize[to]++;
#if DEBUG
	printf("\tsent %d to %d, size %d, ", m, to, qsize[to]);
	printf("cargo %d [%d]\n", what, hook->cargo);
#endif
	require(OUTP, m, to);

	return 1;
}

receive(from, with, pr, ice)
	struct FREEZE *ice;
{ struct QUEUE *hook;
  int what=NONE, wither;

	if (qsize[from] <= 0)
		whoops("trying to receive from empty queue");

	hook = head[from];
	if (hook->cargo & USED)
	{	what = (int) ((hook->cargo) & (~USED));
		if (with == NONE)
		{	fprintf(stderr, "value %d sent (msg: ", what);
			putname(hook, 1);
			fprintf(stderr, ") but not expected\n");
		}
		else
		{	ice->whichvar = wither = wapper(with, pr);
			if (wither >= 3*MANY || wither <= -3*MANY)
				whoops("receiving into a constant...");
			if (wither < MANY)
			{	ice->oldvalue = globvars[wither];
				globvars[wither] = what;
			} else
			{ int n = wither - MANY;	
				if (n >= 0 && n < (int) procpars[pr].nrlvars)
				{	ice->oldvalue = procpars[pr].lvarvals[n];
					procpars[pr].lvarvals[n] = (short) what;
				} else
					whoops("unknown local var");
			}
		}
	} else
	{	if (with != NONE)
		{	fprintf(stderr, "value %d expected (msg: ", with);
			putname(hook, 1);
			fprintf(stderr, ") but not sent\n");
		}
	}

	hook->mesg |= PASSED;
#if DEBUG
	printf("\trecv %d from queue %d, ", head[from]->mesg & (~PASSED), from);
	printf("cargo %d [%d], into var %d\n", what, hook->cargo, with);
#endif

	head[from] = hook->next;
	qsize[from]--;

	require(INP, (hook->mesg & (~PASSED)), from);

	return 1;
}

unrecv(from)
{
	if (head[from] == starter[from])
		whoops("unreceiving beyond initial state");

	head[from] = head[from]->last;
	head[from]->mesg &= (~PASSED);

	qsize[from]++;
}

unsend()
{ short i = lastqueue;

	if (tail[i] == starter[i])
		whoops("unsending beyond initial state");
	if ((tail[i] = tail[i]->last) != s_last)
		whoops("unsend: tail not last sent message");

	if ((s_last = s_last->s_back) != NULL)
		s_last->s_forw = NULL;
	else
		s_first = NULL;
	efree(tail[i]->next);

	qsize[i]--;
}

output(tag, willabort)
	char *tag;
{ struct QUEUE *tmp;

	printf("%s", tag);

	if ((tmp = s_first) != NULL)
	{	formatted((struct QUEUE *) 0);
		if (prbyq != 2)
		do putname(tmp, 0); while ((tmp = tmp->s_forw) != NULL);
		putchar('\n');
	} else
		printf("null output\n");

	if (willabort == 2 || (firstlock && willabort == 1))
	{	completed = 1;
		postlude();
	}
}

putname(tmp, how)
	struct QUEUE *tmp;
{ int i, k = (int) (BS(tmp->mesg)) - msgbase;
  char nnn[128];

	i = strlen(fullname[k].mname);
	if (tmp->mesg & PASSED)
		sprintf(nnn, "%s", fullname[k].mname);
	else
	{	sprintf(nnn, "[%s]", fullname[k].mname);
		i += 2;
	}

	if (tmp->cargo & USED)
		sprintf(&nnn[i], "(%d),", (tmp->cargo & (~USED)));
	else
		sprintf(&nnn[i], ",");

	if (how)
		fprintf(stderr, "%s", nnn);
	else
		printf("%s", nnn);

	return strlen(nnn);
}

inendstate()
{ int i, j, k;

	for (i = 0, k = nrprocs; i < nrprocs; i++)
	{	j = processes[i];
		if (j != basics[i] || tbl[j].endrow[state[i]] != 1)
			k--;
	}
	return k;
}

formatted(at)
	struct QUEUE *at;
{ struct QUEUE *tmp = s_first;
  int i, j; extern int linecode;

	if (tmp == NULL)
		return;

	switch((int) prbyq) {
	case 0: break;
	case 1:
		for (i = 0; i < nrqs; i++)
		{	printf("\n\t%s = {", mbox[i].qname);
			for (tmp = starter[i]; tmp != tail[i]; tmp = tmp->next)
				putname(tmp, 0);
			printf("}");
		}
		printf("\nexecution sequence:\n\t");
		break;
	case 2: putchar('\n');
		for (i = 0; i < nrqs; i++)
			printf("%2d = %s\n", i, mbox[i].qname);
		for (i = 0; i < nrqs; i++)
			printf("\t%2d", i);
#if TRAIL
		if (linecode)
			printf("               \tsource");
#endif TRAIL
		putchar('\n');
		do
		{	if (tmp == at)
				printf(" >>");
			for (i = j = whichq(BS(tmp->mesg)); i >= 0; i--)
				putchar('\t');
			i = putname(tmp, 0);
#if TRAIL
			if (linecode)
			{ for (i = 16-i;   i > 0; i--) putchar(' ');
			  for (j = nrqs-j; j > 0; j--) putchar('\t');
			  printf("%s", tbl[tmp->tstate[0]].rowname[tmp->tstate[1]]);
			}
#endif TRAIL
			putchar('\n');
		} while ((tmp = tmp->s_forw) != NULL);
		if (at != NULL)
			printf(" >>\n");
		break;
	}
}

putloop(at, aa)
	struct QUEUE *at;
	char aa;
{ register struct QUEUE *tmp = s_first;

	if (aa)
		printf("loop:\t");
	else
		printf("assertion violated: ");

	if (s_first != NULL)
	{	formatted(at);
		if (prbyq != 2)
		{	do
			{	putname(tmp, 0);
				if (tmp == at)
					printf("//");
			} while ((tmp = tmp->s_forw) != NULL);
			printf("//\n");
		}
	} else
		printf("null output\n");
}

ppop(pr)
{ struct PROCSTACK *tmp = procstack[pr];
  int i = (int) procstack[pr]->uptable;

	if (procstack[pr] == NULL)
		whoops("null stack");

	restorvarpars(tmp->varparsaved,  pr);
	procstack[pr] = procstack[pr]->follow;

	efree(tmp->varparsaved);
	efree(tmp);

	return i;	/* we're returing to this table */
}

ppush(pr, what, tr)
{ struct PROCSTACK *tmp;

	tmp = (struct PROCSTACK *)
		emalloc(sizeof(struct PROCSTACK));

	tmp->varparsaved  = (struct VARPARS *)
			emalloc(sizeof(struct VARPARS));

	savevarpars (tmp->varparsaved,  pr);

	tmp->uptable = (short) what;
	tmp->uptransf = (short) tr;
	tmp->follow = procstack[pr];
	procstack[pr] = tmp;
}

setlvars(to, pr)
{ register int i;
  short z = tblvars[to].nrlvars;

	if (z > tablpars[pr].nrlvars)
	{	if (tablpars[pr].nrlvars > 0)
			procpars[pr].lvarvals = (short *)
				Realloc(procpars[pr].lvarvals, z * sizeof(short));
		else
			procpars[pr].lvarvals = (short *)
				Emalloc(z * sizeof(short));

		tablpars[pr].nrlvars = z;
	}

	procpars[pr].nrlvars = z;

	for (i = 0; i < z; i++)
		procpars[pr].lvarvals[i] = convert(tblvars[to].lvarvals[i], pr);
}

setpars(from, to, pr)
	struct CPARS *from;
{ struct VARPARS tbuff;
  register int i;
  short x = tblpars[to].nrms;
  short y = tblpars[to].nrvs;

	savemapped(&tbuff, from, pr);	/* mapper() needs old nrms & nrvs */

	if (x > tablpars[pr].nrms)
	{	if (tablpars[pr].nrms > 0)
			procpars[pr].ms = (short *)
				Realloc(procpars[pr].ms, x * sizeof(short));
		else
			procpars[pr].ms = (short *)
				Emalloc(x * sizeof(short));

		tablpars[pr].nrms = x;
	}
	if (y > tablpars[pr].nrvs)
	{	if (tablpars[pr].nrvs > 0)
			procpars[pr].vs = (short *)
				Realloc(procpars[pr].vs, y * sizeof(short));
		else
			procpars[pr].vs = (short *)
				Emalloc(y * sizeof(short));

		tablpars[pr].nrvs = y;
	}

	procpars[pr].nrms = tbuff.nrms;
	procpars[pr].nrvs = tbuff.nrvs;

	for (i = 0; i < procpars[pr].nrms; i++)
		procpars[pr].ms[i] = tbuff.ms[i];

	for (i = 0; i < procpars[pr].nrvs; i++)
		procpars[pr].vs[i] = tbuff.vs[i];

	efree(tbuff.ms); efree(tbuff.vs);
}

#if DEBUG
showstate()
{ int i, j;
	printf("%2d: S:", level);
	for (i = 0; i < nrprocs; i++)
		printf("%d,", state[i]);
	printf("M:");
	for (i = 0; i < nrprocs; i++)
		printf("%d,", processes[i]);
	printf("Q:");
	for (i = 0; i < nrqs; i++)
		printf("%d,", qsize[i]);
	printf("H:");
	for (i = 0; i < nrqs; i++)
		if (qsize[i] == 0)
			printf("-,");
		else
		{	j = (int) BS(head[i]->mesg);
			printf("%s,", fullname[j-msgbase].mname);
		}
	printf("\tV:");
	for (i = 0; i < nrprocs; i++)
	{	for (j = 0; j < procpars[i].nrlvars; j++)
			printf("%d ", procpars[i].lvarvals[j]);
		putchar(';');
	}
	putchar('\n');
}
#endif

retable(prc, ice)
	struct FREEZE *ice;
{ struct CUBE *it, *here;
  int t = processes[prc];

	if (ice->cube == NULL)
	{	here = ice->cube = (struct CUBE *)
			emalloc(sizeof(struct CUBE));
		here->pntr = here->rtnp = NULL;
	} else
	{	for (it = ice->cube; it->pntr != NULL; it = it->pntr)
			;
		it->pntr = (struct CUBE *)
			emalloc(sizeof(struct CUBE));
		it->pntr->rtnp = it;
		here = it->pntr;
		here->pntr = NULL;
	}
	here->poporpush = POP;
	here->which = (short) prc;
	here->procsaved = (short) t;
	here->transfsaved = procstack[prc]->uptransf;
	here->varparsaved  = (struct VARPARS *)
			emalloc(sizeof(struct VARPARS));

	savevarpars(here->varparsaved, prc);
	processes[prc] = ppop(prc);
	fiddler(prc);
	state[prc] = (int) here->transfsaved;
	muststore = tbl[processes[prc]].labrow[state[prc]];
}

savevarpars(at, j)
	struct VARPARS *at;
{ register int i;
  struct VARPARS *it;

	it = &(procpars[j]);
	at->nrms    = it->nrms;
	at->nrvs    = it->nrvs;
	at->nrlvars = it->nrlvars;

	at->ms = (short *) emalloc(it->nrms * sizeof(short));
	at->vs = (short *) emalloc(it->nrvs * sizeof(short));
	at->lvarvals = (short *) emalloc(it->nrlvars * sizeof(short));

	for (i = 0; i < at->nrms; i++)
		at->ms[i] = it->ms[i];
	for (i = 0; i < at->nrvs; i++)
		at->vs[i] = it->vs[i];
	for (i = 0; i < it->nrlvars; i++)
		at->lvarvals[i] = it->lvarvals[i];
}

savemapped(at, it, j)
	struct VARPARS *at;
	struct CPARS  *it;
{ register int i;

	at->nrms = it->nrms;
	at->nrvs = it->nrvs;

	at->ms = (short *)
		emalloc(it->nrms * sizeof(short));
	at->vs = (short *)
		emalloc(it->nrvs * sizeof(short));

	for (i = 0; i < at->nrms; i++)
		at->ms[i] = (short) mapper(it->ms[i], j);

	for (i = 0; i < at->nrvs; i++)
		at->vs[i] = (short) convert(it->vs[i], j);
}

restorvarpars(at, pr)
	struct VARPARS *at;
{ register int i;

	procpars[pr].nrms     = at->nrms;
	procpars[pr].nrvs     = at->nrvs;
	procpars[pr].nrlvars = at->nrlvars;

	for (i = 0; i < at->nrms; i++)
		procpars[pr].ms[i] = at->ms[i];

	for (i = 0; i < at->nrvs; i++)
		procpars[pr].vs[i] = at->vs[i];

	for (i = 0; i < at->nrlvars; i++)
		procpars[pr].lvarvals[i] = at->lvarvals[i];

	efree(at->ms);
	efree(at->vs);
	efree(at->lvarvals);
}

freeze(icy)
	struct FREEZE *icy;
{ register int i;
  struct FREEZE *ice = icy;

  ice->statsaved = (short *) emalloc(nrprocs * sizeof(short));
  ice->varsaved  = (short *) emalloc(nrvars  * sizeof(short));

  ice->lastsav = lastqueue;
  ice->cube = NULL;

  for (i = 0; i < nrvars; i++)
  	ice->varsaved[i] = (short) globvars[i];

  for (i = 0; i < nrprocs; i++)
  {
	ice->statsaved[i] = (short) state[i];

	while (tbl[processes[i]].deadrow[state[i]] && procstack[i] != NULL)
		retable(i, ice);
 }
}

unfreeze(ice)
	struct FREEZE *ice;
{ struct CUBE *here;
  register int i;

  lastqueue = ice->lastsav;

  for (i = 0; i < nrprocs; i++)
  	state[i] = (int) ice->statsaved[i];
  for (i = 0; i < nrvars; i++)
  	globvars[i] = (int) ice->varsaved[i];

  if ((here = ice->cube) != NULL)
	while (here->pntr != NULL)
		here = here->pntr;

  for (; here != NULL;)
  {	i = (int) here->which;
	if (here->poporpush == PUSH)
	{	processes[i] = ppop(i);
	} else
	{	/* use cube to restore the values from before the ppop */
		ppush(i, processes[i], (int) here->transfsaved);
		restorvarpars (here->varparsaved,  i);
		processes[i] = (int) here->procsaved;
		efree(here->varparsaved);
	}
	efree(here);
	fiddler(i);

	if (here == ice->cube)
		break;
	else
		here = here->rtnp;
  }
  efree(ice->statsaved);
  efree(ice->varsaved);
}

FSE(I)
{ short g, h, i, j, k, t, x, y, z, X, Y, how;
  char progress = 0, internal;
  struct FREEZE *delta = (struct FREEZE *) emalloc(sizeof(struct FREEZE));
  struct STATE *iam = NULL, *inloop();
  struct VISIT *ticket;

  if (level >= maxreached)
  {	if (maxxed && level >= maxlevel)
	{	efree(delta);
		return;
	}
	if (level > maxreached)
		maxreached = level;
  }
  freeze(delta);

  if (muststore && (iam = inloop()) == NULL)
  {	unfreeze(delta);
	efree(delta);
	return;
  }

/*
 *	this state has not been seen before; the state
 *	information has been saved in the structure
 *	`STATE'; queue information has been saved in the
 *	last `VISIT' template of this state;
 *	we save a pointer to this template in a local variable:
 *	to be able to mark the state `analyzed' when we return
 *	for efficiency a pointer to the last visit is kept in a global `lastvisit'
 */
  ticket = lastvisit;
  level++;
  COUNT += (double) 1;

/* three tries:
 *	1st try accepts internal moves (no timeouts, no outputs),
 *	2nd try accepts any moves except timeouts,
 *	3rd try accepts only timeouts.
 */

  for (X = 0; X <= 2; X++)
  {	internal = 0;
	for (g = 0, i = I; g < nrprocs; g++, i = (i+1)%nrprocs)
	{	t = processes[i];
		k = state[i];

		if (X == 0 && tbl[t].badrow[k])
			continue;	/* first try: skip fsm without receives */

	for (j = 0; j < tbl[t].nrcols; j++)
	{	if ((z = tbl[t].ptr[k][j].nrpils) == 0)
			continue;

		x = lbt[i].mapcol[j];
		y = lbt[i].orgcol[j];
		Y = tbl[t].coltyp[j];

		if (matchon(x, y, t, X, Y, i))
		{	for (h = 0; h < z; h++)
			{	how = forward(t, k, j, h, x, y, i, Y, delta);

				if (qandirty || how == 0 || how >= LV || how == TC)
					internal = 1;

				progress++;
#if DEBUG
				printf("\tdown   - %d/%d/%d\n", t, j, h);
				showstate();
#endif
			/*	FSE(mbox[lastqueue].owner); */
				FSE(0);

				backup(k, how, y, i, delta);
#if DEBUG
				printf("\tup   - %d/%d/%d\n", t, j, h);
				showstate();
#endif
    		}	}		/* innermost loop: non-determinism */
		if (blast && progress > 0)
			break;
	}				/* inner loop: options per process */
	if (internal)
		break;
	}				/* outer loop: parallelism         */
	if (progress)
		break;			/* normal exit */
  }					/* outermost loop: 2 trials */
  if (progress == 0)
  {
	if ((k = inendstate()) == nrprocs)
	{	normf++;
		if (assertholds())
		{	if (lockplus) output("endstate: ", 0);
		} else
			output("assertion violated: ", 0);
	}
	else
	{	locksf++;
		if (k == 0)
			output("deadlock: ", 1);
		else
			output("partial lock: ", 1);
		if (linecode)
		{	printf("State at time of lock:\n");
			for (i = 0; i < nrprocs; i++)
				printf("\tprocess %3d: state %s\n", i,
			tbl[processes[i]].rowname[state[i]]);
			putchar('\n');
  }	}	}

  level--;

  unfreeze(delta); efree(delta);
  if (iam != NULL)
  {	mark(iam, ticket);
	if (zapper > 0 && (progress == 0 || level >= maxlevel-3))
		swiffle(iam, ticket);
  }
}

forward(tb, k, j, h, m, from, pr, TT, ice)
	struct FREEZE *ice;
{ int how=0, n=m;
  struct ELM *at;

	ice->whichvar = NONE;

	at = &(tbl[tb].ptr[k][j].one[h]);

	switch (TT) {
		case SPN: how = evalexpr(at->valtrans, pr, ice);
			  break;
		case CND: break;	/* `matchon()' already checked it */
		case FCT: m = (int) tbl[tb].calls[n].callwhat;
			  ppush(pr, processes[pr], (int) at->transf);
			  setpars(&(tbl[tb].calls[n]), reftasks[m], pr);
			  setlvars(reftasks[m], pr);
			  processes[pr] = reftasks[m];
			  fiddler(pr);
			  state[pr] = 0;
			  muststore = tbl[processes[pr]].labrow[0];
			  return TC;
		case TMO: send(m, from, NONE, pr, at->transf);
			  receive(from, NONE, pr, ice);
			  lastqueue = (short) from;
			  how = TO;
			  break;
		case DFL:
		case INP: receive(from, (int) at->valtrans, pr, ice);
			  how = RO;
			  break;
		case OUTP: send(m, from, (int) at->valtrans, pr, at->transf);
			  lastqueue = (short) from;
			  how |= SO;
			  break;
	}
	state[pr] = (int) at->transf;
	muststore = tbl[processes[pr]].labrow[state[pr]];

	return (how);
}

backup(k, how, bx, i, ice)
	struct FREEZE *ice;
{ int u = (int) ice->whichvar;

	if (u != NONE)
	{	if (u >= MANY)
			procpars[i].lvarvals[u-MANY] = ice->oldvalue;
		else
			globvars[u] = ice->oldvalue;
	}
	switch (how) {
		case TC: u = processes[i];
			 processes[i] = ppop(i);
			 fiddler(i);
			 break;
		case TO: unrecv(bx);
		case SO: unsend();
			 peekassert(ice);
			 break;
		case SR: unsend();
		case RO: unrecv(bx);
			 peekassert(ice);
		default: break;
	} state[i] = k;
}

fiddler(pr)
{ register int i, t = processes[pr];

	for (i = 0; i < tbl[t].nrcols; i++)
		if (tbl[t].colmap[i] >= MANY)
		{	lbt[pr].mapcol[i] = mapper(tbl[t].colmap[i], pr);
			lbt[pr].orgcol[i] = whichq(lbt[pr].mapcol[i]);
		} else
		{	lbt[pr].mapcol[i] = tbl[t].colmap[i];
			lbt[pr].orgcol[i] = tbl[t].colorg[i];
		}
	
}
