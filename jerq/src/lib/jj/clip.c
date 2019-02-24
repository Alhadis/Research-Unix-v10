#include	<jerq.h>
#include	<font.h>

static Rectangle clipr;

Clip(r)
	Rectangle r;
{
	clipr = r;
}

Cbitblt(sb, sr, db, dp, fc)
	Bitmap *sb, *db;
	Rectangle sr;
	Point dp;
{
	Rectangle r;

	r = raddp(sr, sub(dp, sr.origin));
	if(rectclip(&r, clipr))
	{
		r = rsubp(r, sub(dp, sr.origin));
		dp = add(dp, sub(r.origin, sr.origin));
		bitblt(sb, r, db, dp, fc);
	}
}

Crectf(bp, r, fc)
	Bitmap *bp;
	Rectangle r;
{
	if(rectclip(&r, clipr))
		rectf(bp, r, fc);
}

Point
Cstring(f, s, bp, p, fc)
	Font *f;
	char *s;
	Bitmap *bp;
	Point p;
	Code fc;
{
	register c;
	int full = (fc == F_STORE);
	Rectangle r;
	register Fontchar *i;

	if (full) {
		r.origin.y = 0;
		r.corner.y = f->height;
	}
	for (; c = *s++; p.x += i->width) {
		i = f->info + c;
		if (!full) {
			r.origin.y = i->top;
			r.corner.y = i->bottom;
		}
		r.origin.x = i->x;
		r.corner.x = (i+1)->x;
		Cbitblt(f->bits,r,bp,Pt(p.x+
			((i->left & 0x80) ? i->left | 0xffffff00 : i->left),
			p.y+r.origin.y),fc);
	}
	return(p);
}

Ctexture(bp, r, t, fc)
	Bitmap *bp;
	Rectangle r;
	Texture *t;
	Code fc;
{
	if(rectclip(&r, clipr))
		texture(bp, r, t, fc);
}
