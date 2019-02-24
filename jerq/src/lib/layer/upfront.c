/*
 * Move a layer to the front of the screen
 */
#include <jerq.h>
#include "layer.h"

Layer *lfront, *lback;

static void
Lupfront(l, r, b, f, o)
Layer *l, *o;
Rectangle r;
Bitmap *b;
char *f;
{
	if (o != l) {
		o->someobs = 1;
		bitblt(o, r, o->obs, r.origin, F_STORE);
		bitblt(l->obs, r, l, r.origin, F_STORE);
	}
}

upfront(lp)
	register Layer *lp;
{
	if (!lp)
		return;
	if (!lp->front) {
		lp->someobs = lp->nonevis = 0;
		return;
	}
	cursinhibit();
	/*
	 * Move bits of lp to front
	 */
	lp->nonevis = 0;	/* can't shortcut to whole obscured */
	if (lp->someobs)
		layerop(lp, Lupfront, lp->rect);
	/*
	 * Put lp at front
	 */
	if(lfront!=lp){
		if(lback==lp)
			lback=lp->front;
		if(lp->back)
			lp->back->front=lp->front;
		lp->front->back=lp->back;
		lfront->front=lp;
		lp->back=lfront;
		lp->front=0;
		lfront=lp;
	}
	/*
	 * lp is now at the front, so update nonevis for all layers.
	 * if lp was fully visible before, we can't have changed it, so
	 * don't bother.
	 */
	if (lp->someobs)
		setvis();
	lp->someobs = 0;
	cursallow();
}
