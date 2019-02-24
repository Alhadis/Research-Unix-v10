#include "jerq.h"
/* These routines are NOT portable, but they are fast. */

Point
add(a, b)
	Point a, b;
{
	register short *ap= &a.x, *bp= &b.x;
	*ap++ += *bp++;
	*ap += *bp;
	return a;
}

Point
sub(a, b)
	Point a, b;
{
	register short *ap= &a.x, *bp= &b.x;
	*ap++ -= *bp++;
	*ap -= *bp;
	return a;
}

Rectangle
inset(r,n)
	Rectangle r;
	register n;
{
	register short *rp= &r.origin.x;
	*rp++ += n;
	*rp++ += n;
	*rp++ -= n;
	*rp   -= n;
	return r;
}

Point
div(a, b)
	Point a;
	register b;
{
	register short *ap= &a.x;
	*(ap++)/=b;
	*ap/=b;
	return a;
}

Point
mul(a, b)
	Point a;
	register b;
{
	register short *ap= &a.x;	
	*(ap++)*=b;
	*ap*=b;
	return a;
}

Rectangle
rsubp(r, p)
	Rectangle r;
	Point p;
{
	register short *rp= &r.origin.x, *pp= &p.x;
	*rp++ -= *pp++;
	*rp++ -= *pp--;
	*rp++ -= *pp++;
	*rp   -= *pp;
	return r;
}

Rectangle
raddp(r, p)
	Rectangle r;
	Point p;
{
	register short *rp= &r.origin.x, *pp= &p.x;
	*rp++ += *pp++;
	*rp++ += *pp--;
	*rp++ += *pp++;
	*rp   += *pp;
	return r;
}

eqpt(p, q)
	Point p, q;
{
	register long *pp=(long *)&p, *qq=(long *)&q;
	return *pp==*qq;
}
eqrect(r, s)
	Rectangle r, s;
{
	register long *rr=(long *)&r, *ss=(long *)&s;
	return *rr++==*ss++ && *rr==*ss;
}
