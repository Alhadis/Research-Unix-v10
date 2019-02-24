#include <jerq.h>
#include "layer.h"
#include <line.h>

/*ARGSUSED*/
static void
Lsegment(l, r, db, fp, o)
	Layer *l;
	Rectangle r;
	Bitmap *db;
	register struct{
		Point p0, p1;
		Code f;
	}*fp;
	Obscured *o;
{
	Jclipline(db, r, fp->p0, fp->p1, fp->f);
}

lsegment(l, p, q, f)
	Layer *l;
	Point p, q;
	Code f;
{
	Point p0, p1;
	p0=p;
	if(p.x==q.x && p.y==q.y)
		return;
	p1=Jsetline(p, q);
	layerop(l, Lsegment, l->rect, p0, p1, f);
}
