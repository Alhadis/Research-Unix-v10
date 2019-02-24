#include <jerq.h>
#include "layer.h"

static void
Lsetvis(l, r, b, f, o)
Layer *l, *o;
Rectangle r;
Bitmap *b;
char *f;
{
	if (o)
		o->nonevis = 0;
}

setvis() {
	register Layer *l;

	for (l=lfront; l; l=l->back)
		l->nonevis=1;
	layerop((Layer *)0, Lsetvis, Jrect);
}
