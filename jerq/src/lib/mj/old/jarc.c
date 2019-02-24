#include <jerq.h>
extern Point Jtransform();
jarc(p0, p1, p2, f)
	Point p0, p1, p2, f;
{
	register r;

	r = norm(p0.x-p1.x, p0.y-p1.y, 0);
	elarc(&display, transform(p0),
		muldiv(Drect.corner.x-Drect.origin.x, r, XMAX),
		muldiv(Drect.corner.y-Drect.origin.y, r, YMAX),
		transform(p1), transform(p2), f);
}
