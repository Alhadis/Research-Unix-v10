#include	<jerq.h>
#include	"layer.h"

extern int blitdir;

static Point delta;

extern bitblt(), lblt();

static void
srcop(sl, r, sb, fp, op)
	Layer *sl;
	Rectangle r;
	Bitmap *sb;
	register struct {
		Bitmap *db;
		Code code;
	} *fp;
	Obscured *op;
{
	bitblt(sb, r, fp->db, add(r.origin, delta), fp->code);
}

static void
destop(dl, r, db, fp, op)
	Layer *dl;
	Rectangle r;
	Bitmap *db;
	register struct {
		Layer *sl;
		Code code;
		Point delta;
	} *fp;
	Obscured *op;
{
	layerop(fp->sl, srcop, rsubp(r, delta), db, fp->code);
}

void
lbitblt(sl, rect, dl, pt, f)
	Layer *sl, *dl;
	Rectangle rect;
	Point pt;
	Code f;
{
	delta = sub(pt, rect.origin);
	blitdir = ( (delta.x <= 0)? 0 : 1 ) + ( (delta.y <= 0)? 0 : 2 );
	/*
		check for degenerate cases
	 */
	if(sl->someobs == sl->nonevis){
		(*((dl->someobs != dl->nonevis)? lblt:bitblt))
			(sl->nonevis?sl->obs:(Bitmap *)sl, rect,
			 dl->nonevis?dl->obs:(Bitmap *)dl, pt, f);
		return;
	}

	layerop(dl, destop, raddp(rect, delta), sl, f);
}
