#include <jerq.h>
rectXrect(r, s)
	Rectangle r, s;
{
#define c corner
#define o origin
	return(r.o.x<s.c.x && s.o.x<r.c.x && r.o.y<s.c.y && s.o.y<r.c.y);
}
