#include <jerq.h>
#include <layer.h>

static Obscured *obs;
static failed;

/*
 * See the second big comment in newlayer()
 */
/*ARGSUSED*/
static void
Newlayer(l, r, db, lp, o)
	Layer *l;
	Rectangle r;
	Bitmap *db;
	Layer **lp;
	Obscured *o;
{
	/*
	 * If o==0, rectangle is occupied by only one layer.
	 */
	if(o || failed)
		return;
	/*
	 * Put piece in the obs list
	 */
	(void)addrect(r, *lp);
}

Layer *
newlayer(r)
	Rectangle r;
{
	register Layer *l, *newl;
	register Obscured *o, *next;
	register Bitmap *bp;
	register Obscured *last;
	register a;
	/*
	 * First build a list of all the screen rectangles that currently
	 * have obscured pieces.  While doing that, subdivide any obscured
	 * rectangles that will be cut by the new layer
	 */
	failed=0;
	obs=0;
	for(l=lfront; l; l=l->back)
		for(last=0,o=l->obs; o; o=next){
			next=o->next;
			if(rectXrect(r, o->rect)
				&& (a=addobs(o, o->rect, r, l))){	/* box was split */
				if(a==-1){	/* alloc failure */
					/*
					 * The ugly part.  addobs() couldn't
					 * find the storage, so free up all the
					 * parts and restore this portion of
					 * l's obscured list to its original
					 * state.
					 */
					freeobs((Layer *)0);
					if(last==0){
						if(l->obs!=o){
							/* free the new stuff */
							o->prev->next=0;
							obs=l->obs;
							freeobs((Layer *)0);
							/* repair the list */
							l->obs=o;
							o->prev=0;
						}
					}else if(last->next!=o){
						/* free the new stuff */
						o->prev->next=0;
						obs=last->next;
						freeobs((Layer *)0);
						/* repair the list */
						last->next=o;
						o->prev=last;
					}
					return 0;
				}
				/* there must be o->prev */
				o->prev->next=o->next;
				if(o->next)
					o->next->prev=o->prev;
				last=o->prev;
				bfree(o->bmap);
				free((char *)o);
			}else
				last=o;
		}
	/*
	 * obs is now a list of obscured rectangles that currently have
	 * at least one layer on them.  To get the intersection of
	 * the new layer with currently single-layer parts of the screen,
	 * use layerop to intersect the current layers with the layer
	 * being built, and only look at pieces that are supposedly
	 * on screen.
	 */
	newl=(Layer *)alloc(sizeof (Layer));
	if(newl==0){
		freeobs((Layer *)0);
		return 0;
	}
	newl->rect=r;
	newl->obs=obs;
	for(l=lfront; l; l=l->back)
		layerop(newl, Newlayer, l->rect, l);
	newl->obs=obs;	/* because it's changed */
	/*
	 * Allocate bitmap storage
	 */
	for(o=newl->obs; o; o=o->next)
		if((bp=balloc(o->rect))==0)
			goto Failed;
		else
			o->bmap=bp;
	if(failed){
    Failed:
		freeobs(newl);
		return 0;
	}
	/*
	 * Put new layer at the back
	 */
	newl->front=lback;
	newl->back=0;
	if(lback)
		lback->back=newl;
	if(lfront==0)
		lfront=newl;
	if(lback==0)
		lback=newl;
	/*
	 * Make it look like a Bitmap
	 */
	newl->base=addr(&display, r.origin);
	newl->width=display.width;
	/*
	 * Pull it to the front and clear it!
	 */
	upfront(newl);
	Lbox(newl);
	return(newl);
}

/*
 * Add an obscured rectangle to the list, doing subdivision as necessary.
 * The structure is similar to layerop, doing recursive subdivision of
 * the rectangle of interest (the obscured rectangle), clipping against
 * the rectangle of the new layer being created.  If the rectangle must
 * be cut, the cut new piece is chained into the obscured list of the
 * obscured layer BEFORE the rectangle from which it has been clipped.
 * newlayer()'s inner loop uses the return value from addobs() to decide
 * whether to remove the obscured rectangle from the obscured list because
 * it has been subdivided, but it must be done there because addobs() has
 * no control over the order of subdivision.
 */
#define out(r)	if(addobs(o, r, b, l)==-1)return(-1)

addobs(o, ar, b, l)
	Obscured *o;
	Rectangle ar, b;
	Layer *l;
{
	register Obscured *op;
	Rectangle r;
	register Rectangle *bp= &b, *rp= &r;
	register Bitmap *bmp;
	r=ar;
	if(rectXrect(*rp, *bp)==0)
		goto Addrect;	/* No clip, just subdivide */
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
	/* r now is contained in the rectangle of the new layer */
	if(eqrect(*rp, ar))	/* no clip; just do the bookkeeping */
		return addrect(*rp, o->lobs);
	/* Box was clipped, so break the piece out of the old box and save it */
	if(addrect(*rp, o->lobs)==-1)
		return -1;
    Addrect:
	if((bmp=balloc(*rp))==0)
		return -1;
	op=(Obscured *)alloc(sizeof (Obscured));
	if(op==0){
		bfree(bmp);
		return -1;
	}
	bitblt(o->bmap, r, bmp, r.origin, F_STORE);
	op->lobs=o->lobs;
	op->rect= *rp;
	op->bmap=bmp;
	op->next=o;
	op->prev=o->prev;
	if(o->prev)
		o->prev->next=op;
	else
		l->obs=op;
	o->prev=op;
	return 1;
}

/*
 * Put rectangle on list of rectangles that will be obscured rectangles
 * of the new layer when it is put at the rear of the screen (got that?).
 * If the origin point of the rectangle is already in the list, the
 * rectangle is in the list and we needn't continue.
 * This routine would be better if it sorted
 */
addrect(r, l)
	Rectangle r;
	Layer *l;
{
	register Obscured *newo, *op;
	register x=r.origin.x, y=r.origin.y;
	for(op=obs; op; op=op->next)
		if(op->rect.origin.x==x && op->rect.origin.y==y)
			return 0;	/* rectangle already there */
	newo=(Obscured *)alloc(sizeof (Obscured));
	if(newo==0)
		return -1;
	newo->lobs=l;
	newo->rect=r;
	newo->bmap=0;		/* for now */
	newo->next=obs;
	newo->prev=0;
	if(obs)
		obs->prev=newo;
	obs=newo;
	return 0;
}
/*
 * In case of failure, free up all the storage
 */
freeobs(newl)
	Layer *newl;
{
	register Obscured *o, *next;
	for(o=obs; o; o=next){
		next=o->next;    /* just in case; we are going to free it */
		if(o->bmap)
			bfree(o->bmap);
		free((char *)o);
	}
	if(newl)
		free((char *)newl);
}
