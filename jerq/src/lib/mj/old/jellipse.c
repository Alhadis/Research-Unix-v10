#include <jerq.h>
extern Point Jtransform();
jellipse(p,a,b,f)
	Point p;
	int a, b, f;
{
	ellipse(&display, transform(p),
		muldiv(a, Drect.corner.x-Drect.origin.x, XMAX),
		muldiv(b, Drect.corner.y-Drect.origin.y, YMAX),
		f);
}
