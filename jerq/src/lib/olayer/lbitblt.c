#include	<jerq.h>
#include	<layer.h>

#define		MAXAREA		75

static lessthan();
typedef struct{
	Bitmap *bp;	/* bit map */
	Rectangle rect;	/* rectangle to be copied */
}Area;
typedef struct{
	Area list[MAXAREA];
	Area *next;
}List;
/*static*/ List Dest, Src;	/* XXXX static removed so they're put in bss */
static Point delta;

static
Pass(lp, r, sb, ap, op)
	Layer *lp;
	Rectangle r;
	Bitmap *sb;
	register struct{
		List *l;
	}*ap;
	Obscured *op;
{
	register List *lp;
	lp = ap->l;
	if(lp->next != &(lp->list[MAXAREA])){
		register Area *n;

		n = lp->next;
		n->bp = sb;
		n->rect = r;
		for(n = lp->list; n != lp->next; n++)
			if(! lessthan(n, lp->next))
				break;
		if(n == lp->next)
			(lp->next)++;
		else{
			register Area *m;

			for(m = (lp->next)++; m != n; m--)
				*m = *(m-1);
			n->bp = sb;
			n->rect = r;
		}
	}
}

void
lbitblt(sl, rect, dl, pt, f)
	Layer *sl, *dl;
	Rectangle rect;
	Point pt;
	Code f;
{
	register Area *d, *s;
	Rectangle r;

	/*
		check for degenerate cases
	 */
	if(sl->obs == (Obscured *)0){
		extern bitblt(), lblt();

		(*(dl->obs? lblt:bitblt))(sl, rect, dl, pt, f);
		return;
	}
	/*
		build up the source rectangle lists and destination
		rectangle lists
	*/

	delta = sub(pt, rect.origin);
	Dest.next = &(Dest.list[0]);
	layerop(dl, Pass, raddp(rect, delta), &Dest);
	Src.next = &(Src.list[0]);
	layerop(sl, Pass, rect, &Src);


	for(s = Src.list; s != Src.next; s++){
		for(d = Dest.list; d != Dest.next; d++){
			r = raddp(s->rect, delta);
			if(rectclip(&r, d->rect))
				bitblt(s->bp, rsubp(r, delta), d->bp, r.origin, f);
		}
	}
}

static
lessthan(a, b)
	register Area *a, *b;
{
	register n;

	if((a->rect.origin.y < b->rect.corner.y) && (b->rect.origin.y < a->rect.corner.y))
		return(((a->rect.origin.x - b->rect.origin.x)*(long)delta.x) >= 0);
	else
		return(((a->rect.origin.y - b->rect.origin.y)*(long)delta.y) >= 0);
}
