#include <jerq.h>

#define BORDER	8
#define DXMIN	100
#define DYMIN	40

Rectangle
canon(r)
	Rectangle r;
{
	register d, dm;

	d = r.corner.x - r.origin.x;
	if (d != (dm = max(DXMIN, min(XMAX-2*BORDER-1, d)))) {
		r.origin.x = (r.corner.x + r.origin.x - dm)/2;
		r.corner.x = r.origin.x + dm;
	}
	if ((d = BORDER - r.origin.x) > 0 || (d = XMAX-BORDER-1 - r.corner.x) < 0)
		r.origin.x += d, r.corner.x += d;

	d = r.corner.y - r.origin.y;
	if (d != (dm = max(DYMIN, min(YMAX-2*BORDER-1, d)))) {
		r.origin.y = (r.corner.y + r.origin.y - dm)/2;
		r.corner.y = r.origin.y + dm;
	}
	if ((d = BORDER - r.origin.y) > 0 || (d = YMAX-BORDER-1 - r.corner.y) < 0)
		r.origin.y += d, r.corner.y += d;

	return r;
}
