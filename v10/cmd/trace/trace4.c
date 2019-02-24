#include <stdio.h>
#include "trace.h"
#include "trace.d"

 extern char lockplus, prefix;
 extern struct QUEUE *s_last;
 extern int *processes, *globvars, *state, *qsize;
 extern int nrprocs, nrrefs, nrvars, nrqs, level, assertbl, nrtbl;
 extern long loopsf, zapped;
 extern struct TBL *tbl;
 extern struct PROCSTACK **procstack;
 extern struct VARPARS *procpars;

 short *Factor, maxr = 0;
 double iseen=0, ireseen=0, painful=0, kurshan=0;

 struct VISIT *lastvisit;
 struct STATE *giveme(), *setstate();
 char *Smalloc(), *emalloc();

struct STATE *
inloop()
{ struct STATE *tmp;
  register struct VISIT *hook;
  register int x; char aa;
  int i, nrnonempty=0, nrnon=0;
  short h, hashvalue();

	for (i = 0; i < nrqs; i++)
		if (qsize[i] > 0)
		{	nrnonempty += (1<<i);
			nrnon++;
		}
	h = hashvalue(nrnonempty);
	for (x = member(h); x > 0; x--)
	{	tmp = giveme(h, x);
		if (samestate(tmp))
		{	for (hook = tmp->next; hook != NULL; hook = hook->next)
			{
				if (hook->howmany != nrnonempty
				|| !Queuesmatch(hook, nrnon))
					continue;

				if (ISANA(hook))
				{	if (DEPTH(hook) > level)
					{	painful += 1;
						continue;
					}
					if (prefix)
						output("prefix: ", 0);
					hook->prop.countme += 1;
					kurshan += (double) 1;
					return NULL;
				} else
				{	loopsf++;
					aa = (assertbl == NONE)? 1 : assertholds();
					if (aa == 0 || lockplus)
						putloop(hook->prop.s, aa);
					return NULL;
			}	}
			return setstate(tmp, h);
		}
	}
	return setstate((struct STATE *) NULL, h);
}

cmplvars(one, two)
	struct LOCVARS *one;
	struct VARPARS *two;
{ int i;

	if (one->nrlvars != two->nrlvars)
		return 0;

	for (i = 0; i < one->nrlvars; i++)
		if (one->lvarvals[i] != two->lvarvals[i])
			return 0;

	return 1;
}

cmplocals(one, two)
	struct VARPARS *one, *two;
{ int i;

	if (one->nrlvars != two->nrlvars)
		return 0;

	for (i = 0; i < one->nrlvars; i++)
		if (one->lvarvals[i] != two->lvarvals[i])
			return 0;

	return 1;
}

cmparams(one, two)
	struct VARPARS *one, *two;
{ int i;

	if (one->nrms != two->nrms || one->nrvs != two->nrvs)
		whoops("cannot happen - cmparams");

	for (i = 0; i < one->nrms; i++)
		if (one->ms[i] != two->ms[i])
			return 0;

	for (i = 0; i < one->nrvs; i++)
		if (one->vs[i] != two->vs[i])
			return 0;

	return 1;
}

cmpstacks(older, newer)
	struct PROCSTACK *older, *newer;
{ struct PROCSTACK *tmp1 = older;
  struct PROCSTACK *tmp2 = newer;

	while (tmp2 != NULL)
	{	
		if (tmp1->uptable  != tmp2->uptable
		||  tmp1->uptransf != tmp2->uptransf)
			return 0;

		if (cmplocals(tmp1->varparsaved, tmp2->varparsaved) == 0
		||  cmparams (tmp1->varparsaved, tmp2->varparsaved) == 0)
			return 0;

		tmp1 = tmp1->follow;
		tmp2 = tmp2->follow;
	}
	return 1;
}

samestate(at)
	struct STATE *at;
{
	return (sameP(at->pstate) && sametempl(at->pcon));
}

cpylvars(into, from)
	struct LOCVARS  *into;
	struct VARPARS *from;
{ register int i;

	into->nrlvars = from->nrlvars;
	into->lvarvals = (short *)
		Smalloc(from->nrlvars * sizeof(short));
	for (i = 0; i < from->nrlvars; i++)
		into->lvarvals[i] = from->lvarvals[i];
}

cpylocals(into, from)
	struct VARPARS *into, *from;
{ register int i;

	into->nrlvars = from->nrlvars;
	into->lvarvals = (short *)
		Smalloc(from->nrlvars * sizeof(short));
	for (i = 0; i < from->nrlvars; i++)
		into->lvarvals[i] = from->lvarvals[i];
}

cpyparams(into, from)
	struct VARPARS *into, *from;
{ int i;

	into->nrms = from->nrms;
	into->ms = (short *)
		Smalloc(from->nrms * sizeof(short));
	for (i = 0; i < from->nrms; i++)
		into->ms[i] = from->ms[i];

	into->nrvs = from->nrvs;
	into->vs = (short *)
		Smalloc(from->nrvs * sizeof(short));
	for (i = 0; i < from->nrvs; i++)
		into->vs[i] = from->vs[i];
}

cpystacks(left, right)
	struct PROCSTACK *left, *right;
{ struct PROCSTACK *into = left;
  struct PROCSTACK *from = right;

	while (from != NULL)
	{	into->varparsaved = (struct VARPARS *)
			Smalloc(sizeof(struct VARPARS));
		cpylocals(into->varparsaved, from->varparsaved);
		cpyparams(into->varparsaved, from->varparsaved);

		into->uptable  = from->uptable;
		into->uptransf = from->uptransf;

		if ((from = from->follow) != NULL)
		{	into->follow = (struct PROCSTACK *)
				Smalloc(sizeof(struct PROCSTACK));
			into = into->follow;
	}	}
}

struct STATE *
newstate(pha)
	int pha;
{ struct STATE *hook;
  struct VISIT *findastate();
  struct TEMPLATE *inTtable();
  unsigned short *inPtable();

	hook = (struct STATE *) Smalloc(sizeof(struct STATE));
	hook->pstate = inPtable();
	hook->pcon   = inTtable();
	hook->next   = findastate(hook);	/* first visit */

	insert(pha, hook);

	return hook;
}

struct VISIT *
oldstate(where)
	struct STATE *where;
{ register struct VISIT *tmp;
  struct VISIT *try, *findastate();

	ireseen += (double)1;
	try = findastate(where);
	
	if (where->next == NULL)
		where->next = try;
	else
	{	for (tmp = where->next; tmp->next != NULL; tmp = tmp->next)
			;
		tmp->next = try;
	}

	return try;
}

struct STATE *
setstate(where, ha)
	struct STATE *where;
{ struct STATE *tmp;
  struct VISIT *work;

	if (where == NULL)
	{	tmp = newstate(ha);
		work = tmp->next;
	} else
	{	tmp = where;
		work = oldstate(where);
	}

	work->prop.s = s_last;
	work->depth = (short) level;

	lastvisit = work;
	relink(work);

	return tmp;
}

struct VISIT *
pickstate(at)
	struct STATE *at;
{ struct VISIT *latter = NULL;
  register struct VISIT *hook = at->next;

	for (hook = at->next; hook != NULL; hook = hook->next)
	{	if (ISANA(hook))
		{	if (latter == NULL)
				at->next = hook->next;
			else
				latter->next = hook->next;

			efree(hook->c);
			hook->next = NULL;
			zapped++;

			return hook;
		}
		latter = hook;
	}
	return NULL;
}

struct VISIT *
picknown(at, want)
	struct STATE *at;
	struct VISIT *want;
{ struct VISIT *latter = NULL;
  register struct VISIT *hook;

	for (hook = at->next; hook != NULL; hook = hook->next)
	{	if (hook == want)
			break;
		latter = hook;
	}
	if (hook == NULL)
		whoops("cannot happen - picknown");

	if (latter == NULL)
		at->next = hook->next;
	else
		latter->next = hook->next;

	efree(hook->c);
	hook->next = NULL;

	zapped++;
	return hook;
}

inihash()
{ register int i;
  char *Smalloc();

	for (i = 0; i < nrtbl; i++)
		if (tbl[i].nrrows > maxr)
			maxr = tbl[i].nrrows;

	Factor = (short *)
		Smalloc(maxr * sizeof(short));

	for (i = 0; i < maxr; i++)
		Factor[i] = rand()%NOTOOBIG;	/* number between 0 and 16k */
}

short
hashvalue(g)
{ register int i, h;

	for (i = h = 0; i < nrprocs; i++)
	{	h = ((h << 2) | (h >> 13));	/* rotate */
		h ^= Factor[state[i]] ^ Factor[processes[i]];
	}
	h += g;
	return (h & NOTOOBIG);
}
