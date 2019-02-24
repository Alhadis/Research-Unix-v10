/*
 * Move a layer to the back of the screen
 */
#include <jerq.h>
#include "layer.h"

static void
LLdownback(l, r, b, f, o)
Layer *l, *o;
Rectangle r;
Bitmap *b;
char *f;
{
	if (o) {
		o->nonevis = 0;
		o->someobs = 2;	/* so we know to update it */
		l->someobs = 1;
		bitblt(l, r, l->obs, r.origin, F_STORE);
		bitblt(o->obs, r, o, r.origin, F_STORE);
	} else
		l->nonevis = 0;
}

static void
Ldownback(l, r, b, f, o)
Layer *l, *o;
Rectangle r;
Bitmap *b;
char *f;
{
	if (l==o)
		mRlayerop(l, LLdownback, r, f, l->back);
	else
		l->someobs = 1;
}

downback(lp)
	register Layer *lp;
{
	register Layer *l, *visl;

	if(lp==0 || lp->back==0)
		return;		/* Why continue? */
	/*
	 * Move bits of lp to back
	 */
	cursinhibit();
	if (!lp->nonevis) {
		lp->nonevis = 1; /* if any stays on screen, this gets unset */
		if (lp->someobs) {
			lp->someobs = 0; /* hack to make layerop work out */
			layerop(lp, Ldownback, lp->rect);
		} else
			mRlayerop(lp, LLdownback, lp->rect, (char *)0, lp->back);
	}
	cursallow();
	/*
	 * Put lp at back
	 */
	if(lback!=lp){
		if(lfront==lp)
			lfront=lp->back;
		if(lp->front)
			lp->front->back=lp->back;
		lp->back->front=lp->front;
		lback->back=lp;
		lp->front=lback;
		lp->back=0;
		lback=lp;
	}
	/*
	 * We may have made some of the rectangles behind lp fully
	 * visible, which is flagged by someobs==2.
	 */
	for (visl=lfront; visl; visl=visl->back) {
		if (visl->someobs == 2) {
			visl->someobs = 0;
			for (l=lfront; l!=visl; l=l->back)
				if (rectXrect(visl->rect, l->rect)) {
					visl->someobs = 1;
					break;
				}
		}
	}
}
