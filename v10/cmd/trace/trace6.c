#include <stdio.h>
#include "trace.h"
#include "trace.d"

 extern struct QUEUE **head, **tail;
 extern int *qsize, nrqs, level;

 char *Realloc(), *Emalloc(), *Smalloc();

 struct CONTS ***oldqueues;		/* index of queue states */
 long *nrqstates, *qbound;

iniqtable()
{ register int i;
	nrqstates = (long *)
		Smalloc(nrqs * sizeof(long));
	qbound = (long *)
		Smalloc(nrqs * sizeof(long));

	oldqueues = (struct CONTS ***)
		Smalloc(nrqs * sizeof(struct CONTS **));

	for (i = 0; i < nrqs; i++)
		nrqstates[i] = qbound[i] = 0;
}

growqtable(p)
{ int nsz = qbound[p] + 32;

	if (nsz == 32)
		oldqueues[p] = (struct CONTS **)
			Emalloc(nsz * sizeof(struct CONTS *));
	else
		oldqueues[p] = (struct CONTS **)
			Realloc(oldqueues[p], nsz * sizeof(struct CONTS *));
	
	qbound[p] = nsz;
}

struct CONTS *
Qinsert(p)
{ struct CONTS * try;
  struct QUEUE * tmp;
  register int i, j;

	i = p;
	j = qsize[p];

	if (nrqstates[p] >= qbound[p])
		growqtable(p);

	try = (struct CONTS *)
		Smalloc((j+1) * sizeof(struct CONTS));

	try[0].mesg  = i;
	try[0].cargo = j;

	for (tmp = head[i], j = 1; tmp != tail[i]; tmp = tmp->next, j++)
	{	try[j].mesg  = (tmp->mesg & ~PASSED);
		try[j].cargo = tmp->cargo;
	}

	if (--j != qsize[p])
		whoops("cannot happen - Qinsert");

	oldqueues[p][nrqstates[p]] = try;
	nrqstates[p] += 1;

	return try;
}

struct CONTS *
inqtable(p)
{	register int i;

	if (qsize[p] == 0)
		whoops("cannot happen - inqtable");

	for (i = 0; i < nrqstates[p]; i++)
		if (samequeue(oldqueues[p][i]))
			return oldqueues[p][i];

	return Qinsert(p);
}

samequeue(at)
	struct CONTS *at;
{
	register struct QUEUE *tmp;
	register int j, k, m;

	k = at[0].mesg;		/* which queue is this */
	m = at[0].cargo;	/* how many elements are here */

	if (qsize[k] != m)
		return 0;

	for (tmp = head[k], j = 1; tmp != tail[k]; tmp = tmp->next, j++)
	{	if (at[j].mesg  != (tmp->mesg & ~PASSED)
		||  at[j].cargo != tmp->cargo)
			return 0;
	}
	return 1;
}

Queuesmatch(vis, k)
	struct VISIT *vis;
	register int k;
{
	register int i;

	for (i = 0; i < k; i++)
		if (samequeue(vis->c[i]) == 0)
			return 0;
	return 1;
}
