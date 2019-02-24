#include <stdio.h>
#include "geom.h"
#include "thing.h"
#include "text.h"
#include "wire.h"
#include "conn.h"
#include "box.h"

Box.Box()
{
	suffix = 0;
	pins = new ConnList(8);
}

Box.Box(Rectangle r)
{
	R = r;
	suffix = 0;
	pins = new ConnList(8);
}

int Box.namepart(Text *t)
{
	register char *p;
	if (t->p < R) {
		part = t;
		for (p = t->s->s; *p; p++)
			if (*p == '.')
				suffix = p+1;
		return 1;
	}
	return 0;
}

int Box.namepin(Text *t)
{
	if (t->p <= R && !(t->p < R)) {
		pins->append(new Conn(t));
		return 1;
	}
	return 0;
}

void Box.put(FILE *ouf)
{
	if (suffix) {
		*(suffix-1) = 0;
		pins->apply((int *) suffix,Conn::fixup);
	}
	if (part == 0)
		fprintf(stderr,"unnamed part at %d,%d\n",X,Y);
	else
		fprintf(ouf,".c	%s	%s\n",part->s->s,part->t->s);
	pins->put(ouf);
}

void Box.getname(WireList *wl)
{
	if (part == 0)
		wl->apply((int *) this,Wire::namebox);
}

int BoxList.parts(Text *t)
{
	register i,j;
	for (i = 0, j = 0; i < n; i++)
		j |= box(i)->namepart(t);
	return j;
}

int BoxList.pins(Text *t)
{
	register i,j;
	for (i = 0, j = 0; i < n; i++)
		j |= box(i)->namepin(t);
	return j;
}

void BoxList.nets(WireList *w)
{
	register i;
	for (i = 0; i < n; i++)
		(box(i)->pins)->nets(w);
}

void BoxList.expand(Vector *v)
{
	register i;
	for (i = 0; i < n; i++)
		a[i]->expand(v);
}
