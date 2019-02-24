#include <jerq.h>
#include "layer.h"

/*
 * Magic
 *
 * Intersect a rectangle to a layer, doing all subdivision into rectangles.
 * layerop() clips rectangle to outer edge of layer, then calls mRlayerop
 * which recursively chains along the layer list, doing op's (specified
 * by a pointer to a function) whenever there is an intersection.  Differs
 * from rob's by taking a direction of evaluation, so that the subrectangles
 * produced are properly sorted for bitblt from a layer to itself.
 *
 */

/* For the portion outside the current layer, we use recursion */
#define	out(or,co)	mRlayerop(l, fn, Rpt(or,co), fp, o->back)

/* For the portion inside, we perform the function */
#define in(r)	(*fn)(l, r, (l?l->obs:0), fp, o)

#define C_DONE	0
#define C_UP	1
#define C_LEFT	2
#define C_MID	3
#define C_RIGHT	4
#define C_DOWN	5

int blitdir = 0;	/* direction from source to destination */

/*
 * directions:  0 = up and left,
 *		1 = up and right,
 *		2 = down and left,
 *		3 = down and right
 */

static short nextcase[4][6] = {
	C_UP, C_LEFT, C_MID, C_RIGHT, C_DOWN, C_DONE,
	C_UP, C_RIGHT, C_MID, C_LEFT, C_DOWN, C_DONE,
	C_DOWN, C_LEFT, C_MID, C_RIGHT, C_UP, C_DONE,
	C_DOWN, C_RIGHT, C_MID, C_LEFT, C_UP, C_DONE,
};

mRlayerop(l, fn, r, fp, o)
	register Layer *l;
	void (*fn)();
	Rectangle r;	/* Screen coords */
	int *fp;
	register Layer *o;
{
	Rectangle clipr;
	register Rectangle *rp= &r, *cp= &clipr, *bp;
	register short *next;

	*cp = *rp;
Top:
	bp= &o->rect;
	if((o==l) || (o==0))	/* We have a piece that wasn't obscured */
		(*fn)(l, r, l, fp, o);
	else if (rectclip(cp, *bp)==0) {	/* note side-effect to clipr */
		o=o->back;	/* efficiency hack */
		goto Top;
	} else {
		/* They must overlap */
		for (next=&nextcase[blitdir][0]; *next != C_DONE; )
			switch(*next++) {
			case C_UP:
				if (rp->origin.y<bp->origin.y)
					out(rp->origin,
					    Pt(rp->corner.x, bp->origin.y));
				break;
			case C_DOWN:
				if (rp->corner.y>bp->corner.y)
					out(Pt(rp->origin.x, bp->corner.y),
					    rp->corner);
				break;
			case C_LEFT:
				if (rp->origin.x<bp->origin.x)
					out(Pt(rp->origin.x, cp->origin.y),
					    Pt(bp->origin.x, cp->corner.y));
				break;
			case C_RIGHT:
				if (rp->corner.x>bp->corner.x)
					out(Pt(bp->corner.x, cp->origin.y),
					    Pt(rp->corner.x, cp->corner.y));
				break;
			case C_MID:
				in(*cp);
				break;
			}
	}
}

/*
 * Clip to outer rectangle of layer, then call layerop
 */
/*VARARGS3*/
layerop(l, fn, r, f)
	register Layer *l;
	void (*fn)();
	Rectangle r;	/* Screen coords */
	int f;
{
	if(l && (rectclip(&r, l->rect)==0))
		return;
	if (l && (l->someobs == l->nonevis))
		(*fn)(l, r, l->nonevis?l->obs:(Bitmap *)l, &f,
		      (l->nonevis?lfront:l));
	else
		mRlayerop(l, fn, r, &f, lfront);
}
