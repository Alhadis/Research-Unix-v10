#include	<jerq.h>

jelarc(p0, a, b, p1, p2, f)
	Point p0, p1, p2;
	Code f;
{
	elarc(&display, p0, a, b, p1, p2, f);
}
