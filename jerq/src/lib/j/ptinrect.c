#include <jerq.h>
ptinrect(p, r)
	Point p;
	Rectangle r;
{
	return(p.x>=r.origin.x && p.x<r.corner.x
	    && p.y>=r.origin.y && p.y<r.corner.y);
}
