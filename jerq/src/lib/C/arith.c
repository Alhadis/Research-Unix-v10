#include <CC/jerq.h>

#define min(x,y)	((x < y) ? x : y)
#define max(x,y)	((x > y) ? x : y)

Rectangle Rectangle.mbb(Point p)
{
	Rectangle r;
	r.o.x = min(o.x,p.x);
	r.o.y = min(o.y,p.y);
	r.c.x = max(c.x,p.x);
	r.c.y = max(c.y,p.y);
	return r;
}

Rectangle Rectangle.mbb(Rectangle r)
{
	r.o.x = min(o.x,r.o.x);
	r.o.y = min(o.y,r.o.y);
	r.c.x = max(c.x,r.c.x);
	r.c.y = max(c.y,r.c.y);
	return r;
}
