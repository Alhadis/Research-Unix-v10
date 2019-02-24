#include <jerq.h>
#include "layer.h"
/*ARGSUSED*/
static void
Ltexture(l, r, db, fp, o)
	Layer *l;
	Rectangle r;
	Bitmap *db;
	register struct{
		Word *map;
		Code f;
	}*fp;
	Obscured *o;
{
	texture(db, r, fp->map, fp->f);
}
ltexture(l, r, map, f)
	Layer *l;
	Rectangle r;
	Word *map;
	Code f;
{
	layerop(l, Ltexture, r, map, f);
}
