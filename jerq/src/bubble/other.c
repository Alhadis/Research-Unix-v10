#include	<jerq.h>
#include	<font.h>
#include	<jerqio.h>
#include	<3d.h>
#include	"nodes.h"

struct Pair *pair;			/* list of atom pairs closer than 2 A */
struct Pair **lastpair;

Point midpoint;
extern Node *tab[];
extern short ncount;

resizer()
{	int i;

	announce("resizing");
	for (i = 0; i < NNODES; i++)
	{	resize(i);
		wait(CPU);
	}
}

mask(n)
{	extern Bitmap *off;
	extern struct BALLS store[NNODES];

	rectf(off, Drect, F_CLR);
	store[n].vis = 1 - store[n].vis;
}

mover()
{	Point dif, p;
	extern Texture bullseye;

	cursswitch(&bullseye);
	while(!button123()) nap(1);
	if (button123())
	{	p = mouse.xy;
		while(button123()) nap(1);
		dif = sub(mouse.xy, p);
		moveit(dif);
	}
}

setmidpt()
{
	midpoint.x = (Drect.corner.x + Drect.origin.x)/2;
	midpoint.y = (Drect.corner.y + Drect.origin.y)/2;
}

Point
Norm(p)
	Point p;
{	Point q;
	extern short delta;

	q.x = muldiv(p.x, 100, delta);
	q.y = muldiv(p.y, 100, delta);
	return add(midpoint, q);
}

drawbonds()
{	struct Pair *n;
	Node *p, *q;

	for (n = pair; n; n = n->nxt)
	{	p = n->a;
		q = n->b;
		if (store[p->flag].vis && store[q->flag].vis)
			connect(p, q);
	}
}

makebonds()
{	Node *n, *m;
	short i, j;
	long cutoff = 2500L;	/* 2 Angstrom scaled: (2 x 25 (input scale))^2 */
	long distance2();

	pair = (struct Pair *) NULL;
	lastpair = &pair;
	for (i = 0; i < ncount; i++)
	{	n = tab[i];
		for (j = i+1; j < ncount; j++)
		{	m = tab[j];
			if (distance2(n, m) <= cutoff)
				addpair(i, j);
			wait(CPU);	
	}	}
	
}

struct Pair *
newpair(n, m)
	short n, m;
{	struct Pair *tmp;

	tmp = (struct Pair *) alloc(sizeof(struct Pair));
	if (tmp)
	{	tmp->a = tab[n];
		tmp->b = tab[m];
		tmp->nxt = (struct Pair *)NULL;
		return tmp;
	}
	else
	{	outofmem();
		return (struct Pair *)NULL;
	}
}

addpair(n, m)
	short n, m;
{
	*lastpair = newpair(n, m);
	lastpair = &((*lastpair)->nxt);
}
