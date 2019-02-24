#include <jerq.h>
#include "dline.h"

/*ARGSUSED*/
static void
dLsegment(l, r, db, fp, o)
	Layer *l;
	Rectangle r;
	Bitmap *db;
	register struct{
		Point p0, p1;
		Code f;
	}*fp;
	Obscured *o;
{
	Jdclipline(db, r, fp->p0, fp->p1, fp->f);
}

dlsegment(l, p, q, f)
	Layer *l;
	Point p, q;
	Code f;
{
	Point p0, p1;
	p0=p;
	if(p.x==q.x && p.y==q.y)
		return;
	p1=Jdsetline(p, q);
	layerop(l, dLsegment, l->rect, p0, p1, f);
}
