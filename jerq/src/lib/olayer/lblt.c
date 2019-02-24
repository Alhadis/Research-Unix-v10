#include <jerq.h>
#include <layer.h>
/*ARGSUSED*/
static void
Lblt(l, r, db, fp, o)
	Layer *l;
	Rectangle r;
	Bitmap *db;
	register struct{
		Bitmap *sb;
		Point q;
		Code f;
	} *fp;
	Obscured *o;
{
	bitblt(fp->sb, rsubp(r, fp->q), db, r.origin, fp->f);
}
lblt(b, r, l, p, f)
	Bitmap *b;
	Rectangle r;
	Layer *l;
	Point p;
	Code f;
{
	Point q;
	Rectangle dr;
	q=sub(p, r.origin);
	dr=raddp(r, q);
	layerop(l, Lblt, dr, b, q, f);
}
