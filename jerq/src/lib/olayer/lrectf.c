#include <jerq.h>
#include <layer.h>
/*ARGSUSED*/
static void
Lrectf(l, r, db, f, o)
	Layer *l;
	Rectangle r;
	Bitmap *db;
	Code *f;
	Obscured *o;
{
	rectf(db, r, *f);
}
lrectf(l, r, f)
	Layer *l;
	Rectangle r;
	Code f;
{
	layerop(l, Lrectf, r, f);
}
