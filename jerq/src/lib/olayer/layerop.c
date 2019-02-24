#include <jerq.h>
#include <layer.h>

/*
 * Magic
 *
 * Intersect a rectangle to a layer, doing all subdivision into rectangles.
 * layerop() clips rectangle to outer edge of layer, then calls Rlayerop
 * which recursively chains along the obscured list, doing op's (specified
 * by a pointer to a function) whenever there is an intersection.
 *
 */
#define	out(r)	Rlayerop(l, fn, r, fp, o->next)	/* r outside box */
#define in(r)	(*fn)(l, r, o->bmap, fp, o);	/* r inside box */
Rlayerop(l, fn, r, fp, o)
	register Layer *l;
	void (*fn)();
	Rectangle r;	/* Screen coords */
	int *fp;
	register Obscured *o;
{
	register Rectangle *rp= &r, *bp;

Top:
	bp= &o->rect;
	if(o==0)	/* We have a piece that wasn't obscured */
		(*fn)(l, r, l, fp, o);
	else if(rectXrect(*rp, *bp)==0){
		o=o->next;	/* efficiency hack */
		goto Top;
	}else{
		/* They must overlap */
		if(rp->origin.x<bp->origin.x){
			out(Rpt(rp->origin, Pt(bp->origin.x, rp->corner.y)));
			rp->origin.x=bp->origin.x;
		}
		if(rp->origin.y<bp->origin.y){
			out(Rpt(rp->origin, Pt(rp->corner.x, bp->origin.y)));
			rp->origin.y=bp->origin.y;
		}
		if(rp->corner.x>bp->corner.x){
			out(Rpt(Pt(bp->corner.x, rp->origin.y), rp->corner));
			rp->corner.x=bp->corner.x;
		}
		if(rp->corner.y>bp->corner.y){
			out(Rpt(Pt(rp->origin.x, bp->corner.y), rp->corner));
			rp->corner.y=bp->corner.y;
		}
		in(*rp);
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
	if(rectclip(&r, l->rect)==0)
		return;
	Rlayerop(l, fn, r, &f, l->obs);
}
