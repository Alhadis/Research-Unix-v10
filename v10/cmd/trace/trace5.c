#include <stdio.h>
#include "trace.h"
#include "trace.d"

 extern struct QUEUE **head, **tail;
 extern int *qsize, nrqs, level, maxreached;
 extern double iseen, ireseen, zapper;
 extern double tryswiff, noswiff, cannot;
 char *Realloc(), *Emalloc(), *Smalloc(), *emalloc();

 struct HTABLE {
	struct STATE **index;		/* index [h] [ibound]    */
	short ibound;			/* nr of available slots */
	short nr;			/* nr of occupied  slots */
 } oldstates[NOTOOBIG+1];		/* index of hash values  */

 int hbound=0, hlast=0;

growindex(h)
{ int nsz = (int) oldstates[h].ibound + 8;

	if (nsz == 8)
		oldstates[h].index = (struct STATE **)
			Emalloc(nsz * sizeof(struct STATE *));
	else
		oldstates[h].index = (struct STATE **)
			Realloc(oldstates[h].index, nsz * sizeof(struct STATE *));
	
	oldstates[h].ibound = (short) nsz;
}

initable()
{	register int i;

	for (i = 0; i < NOTOOBIG+1; i++)
	{	oldstates[i].nr = 0;
		oldstates[i].ibound = 0;
	}
	hbound = NOTOOBIG+1;
}

insert(h, pnt)		/* enter state pointer into the table at hash value h */
	struct STATE *pnt;
{ short cin;

	if (h >= hbound)
	{	fprintf(stderr, "h %d, hbound %d, NOTOOBIG %d\n",h,hbound,NOTOOBIG);
		whoops("cannot happen - insert");
	}
	cin = oldstates[h].nr++;
	iseen += (double) 1;

	if (cin >= oldstates[h].ibound)
		growindex(h);

	oldstates[h].index[cin] = pnt;
}

mark(stt, vis)
	struct STATE *stt;
	struct VISIT *vis;
{
	vis->prop.countme = 0;
	vis->depth |= ANALYZED;
}

relink(vis)
	struct VISIT *vis;
{ struct CONTS *inqtable();
  register int i, j, k;

	for (i = j = k = 0; i < nrqs ; i++)
		if (qsize[i] > 0)
		{	j++;
			k += (1 << i);
		}
	vis->howmany = k;

	if (zapper == 0)	/* grow without bound */
		vis->c = (struct CONTS **)
			Smalloc(j * sizeof(struct CONTS *));
	else
		vis->c = (struct CONTS **)
			emalloc(j * sizeof(struct CONTS *));

	for (i = k = 0; i < nrqs; i++)
	{	if (qsize[i] == 0)
			continue;

		vis->c[k++] = inqtable(i);
	}
}

member(h) {	return (h < hbound) ? oldstates[h].nr : 0; }

struct STATE *
giveme(h, n)
{ int m = n - 1;
	if (h >= hbound || oldstates[h].nr <= m || oldstates[h].index[m] == NULL)
		whoops("cannot happen - giveme");

	return oldstates[h].index[m];
}

struct VISIT *
findany(avoid)
	struct STATE *avoid;
{ register int i, j;
  struct VISIT *try, *pickstate();

	for (i = (hlast+1)%hbound; i != hlast; i++, i %= hbound)
	for (j = 0; j < oldstates[i].nr; j++)
		if (oldstates[i].index[j] != avoid
		&&  (try = pickstate(oldstates[i].index[j])) != NULL)
		{	hlast = i;
			return try;
		}

	cannot += (double) 1;	/* couldn't find a victim */
	try = (struct VISIT *) Smalloc(sizeof(struct VISIT));
	try->next = NULL;

	return try;
}

struct VISIT *
findastate(avoid)
	struct STATE *avoid;
{ struct VISIT *try = NULL;
  struct VISIT *findany(), *picknown();
  struct Swiffle *unswiffle(), *trs;

	if (ireseen < zapper || zapper == 0)
	{	try = (struct VISIT *) Smalloc(sizeof(struct VISIT));
		try->next = NULL;
		return try;
	}
	tryswiff += (double) 1;
	if ((trs = unswiffle(avoid)) != NULL
	&& (try = picknown(trs->st, trs->vi)) != NULL)
		return try;
	noswiff += (double) 1;

	return findany(avoid);
}
