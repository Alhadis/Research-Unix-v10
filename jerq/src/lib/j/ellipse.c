#ifndef	MUX
#define	Jcursinhibit()	cursinhibit()
#define	Jcursallow()	cursallow()
#define	lrectf(l, r, f)	rectf(l, r, f)
#define	lsegment(l, p, q, f)	segment(l, p, q, f)
#endif

#include	<jerq.h>
#define	DB		((Word *)0x700000)
#define	ONSCREEN(b)	(b->base>=DB && b->base<DB+XMAX/WORDSIZE*YMAX)
#define labs(x,y) if((x=y)<0) x= -x
#define samesign(x,y) (((int)(x)^(int)(y)) > 0)

#define		BIG	077777
#define		HUGE	07777777777L

/* draw an ellipse centered at x0,y0 with half-axes a,b */
#ifdef	MUX
#define	POINT	Tvoid(26)
#else
extern void point();
#define	POINT	point
#endif
ellipse(bp, p, a, b, f)
	Bitmap *bp;
	Point p;
	long a, b;
	Code f;
{
	short inhibited;
	inhibited=0;
	if(f!=F_XOR && ONSCREEN(bp)){
		Jcursinhibit();
		inhibited=1;
	}
	if(a==0 || b==0)
		lsegment(bp, Pt(p.x-a, p.y-b), Pt(p.x+a, p.y+b), f);
	else
		ellip1(bp, p, a, b, POINT, Pt(0, b), Pt(0, b), f);
	if(inhibited)
		Jcursallow();
}

/* calculate b*b*x*x + a*a*y*y - a*a*b*b avoiding ovfl */

static long
resid(a,b,x,y)
register long a,b;
long x,y;
{
	long e = 0;
	long u = b*(a*a - x*x);
	long v = a*y*y;
	register q = u>BIG? HUGE/u: BIG;
	register r = v>BIG? HUGE/v: BIG;
	while(a || b) {
		if(e>=0 && b) {
			if(q>b) q = b;
			e -= q*u;
			b -= q;
		} else {
			if(r>a) r = a;
			e += r*v;
			a -= r;
		}
	}
	return(e);
}

/* service routine used for both elliptic arcs and ellipses 
 * traces clockwise an ellipse centered at x0,y0 with half-axes
 * a,b starting from the point x1,y1 and ending at x2,y2
 * performing an action at each point
 * x1,y1,x2,y2 are measured relative to center
 * when x1,y1 = x2,y2 the whole ellipse is traced
 * e is the error b^2 x^2 + a^2 y^2 - a^2 b^2
*/

ellip1(bp, p0, a, b, action, p1, p2, f)
	Point p0, p1, p2;
	long a, b;
	register void (*action)();
	register Bitmap *bp;
	Code f;
{
	int dx = p1.y>0? 1: p1.y<0? -1: p1.x>0? -1: 1;
	int dy = p1.x>0? -1: p1.x<0? 1: p1.y>0? -1: 1;
	long a2 = a*a;
	long b2 = b*b;
	register long dex = b2*(2*dx*p1.x+1);
	register long e;
	register long dey = a2*(2*dy*p1.y+1);
	register long ex, ey, exy;
	int partial = !eqpt(p1, p2);

	if(partial &&
	   (p1.x==0 && p2.x==0 && samesign(p1.y, p2.y) ||
	    p1.y==0 && p2.y==0 && samesign(p1.x, p2.x))) {
		lsegment(bp, add(p0, p1), add(p0,p2), f);
		return;
	}
	e = resid(a, b, p1.x, p1.y);
	a2 *= 2;
	b2 *= 2;
	do {
		labs(ex, e+dex);
		labs(ey, e+dey);
		labs(exy, e+dex+dey);
		if(exy<=ex || ey<ex) {
			p1.y += dy;
			e += dey;
			dey += a2;
		}
		if(exy<=ey || ex<ey) {
			p1.x += dx;
			e += dex;
			dex += b2;
		}
		if(p1.x == 0) {
			if(abs(p1.y) == b) {
				dy = -dy;
				dey = -dey + a2;
				partial = 0;
			} else if(!samesign(p1.y, dy) && !partial)
				continue;	/* don't double-draw skinny ends */
		} else if(p1.y == 0) {
			if(abs(p1.x) == a) {
				dx = -dx;
				dex = -dex + b2;
				partial = 0;
			} else if(!samesign(p1.x, dx) && !partial)
				continue;
		}
		(*action)(bp, add(p0, p1), f);
	} while(! eqpt(p1, p2));
}
