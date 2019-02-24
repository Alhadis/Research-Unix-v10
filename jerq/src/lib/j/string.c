#include <jerq.h>
#include <font.h>

Point
string(f, s, b, p, fc)
	register Font *f;
	char *s;
	register Bitmap *b;
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
		bitblt(f->bits,r,b,Pt(p.x+
			((i->left & 0x80) ? i->left | 0xffffff00 : i->left),
			p.y+r.origin.y),fc);
	}
	return(p);
}
