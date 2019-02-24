#ifndef	MUX
#define	Jcursinhibit()	cursinhibit()
#define	Jcursallow()	cursallow()
#define	lrectf(l, r, f)	rectf(l, r, f)
#define	lpoint(l, p, f)	point(l, p, f)
#endif

#include <jerq.h>
static Point nearby();
#define sq(x) ((long)(x)*(x))
#define	sgn(x)	((x)<0? -1 : (x)==0? 0 : 1)
#define labs(x) 	(x<0? -x : x)
#define	DB		((Word *)0x700000)
#define	ONSCREEN(b)	(b->base>=DB && b->base<DB+XMAX/WORDSIZE*YMAX)

/*	Draw an approximate arc centered at x0,y0 of an
 *	integer grid and running anti-clockwise from
 *	x1,y1 to the vicinity of x2,y2.
 *	If the endpoints coincide, draw a complete circle.
 *
 *	The "arc" is a sequence of vertically, horizontally,
 *	or diagonally adjacent points that minimize 
 *	abs(x^2+y^2-r^2).
 *
 *	The circle is guaranteed to be symmetric about
 *	the horizontal, vertical, and diagonal axes
 */
arc(bp, p0, p2, p1, f)
	register Bitmap *bp;
	Point p0, p1, p2;
{
	short inhibited;
	register dx, dy;
	register eps;	/* x^2 + y^2 - r^2 */
	int dxsq, dysq;	/* (x+dx)^2-x^2, ...*/
	int ex, ey, exy;
	p1=sub(p1, p0);
	p2=sub(p2, p0);
	p2=nearby(p1, p2);
	dx = -sgn(p1.y);	/* y1==0 is soon fixed */
	dy = sgn(p1.x);
	dxsq = (2*p1.x + dx)*dx;
	dysq = (2*p1.y + dy)*dy;
	eps = 0;
	inhibited=0;
	if(f!=F_XOR && ONSCREEN(bp)){
		Jcursinhibit();
		inhibited=1;
	}
	do {
		if(p1.x == 0) {
			dy = -sgn(p1.y);
			dysq = (2*p1.y + dy)*dy;
		} else if(p1.y == 0) {
			dx = -sgn(p1.x);
			dxsq = (2*p1.x + dx)*dx;
		}
		ex = abs(eps + dxsq);
		ey = abs(eps + dysq);
		exy = abs(eps + dxsq + dysq);
		if(ex<ey || exy<=ey) {
			p1.x += dx;
			eps += dxsq;
			dxsq += 2;
		}
		if(ey<ex || exy<=ex) {
			p1.y += dy;
			eps += dysq;
			dysq += 2;
		}
		lpoint(bp, Pt(p0.x+p1.x, p0.y+p1.y), f);
	} while(!(p1.x==p2.x && p1.y==p2.y));	/* Note1 */
	if(inhibited)
		Jcursallow();
}
/*	Note1: the equality end test is justified
 *	because it is impossible that
 *	abs(x^2+y^2-r^2)==abs((x++-1)^2+y^2-r^2) or
 *	abs(x^2+y^2-r^2)==abs(x^2+(y++-1)-r^2),
 *	and no values of x or y are skipped.
 *
 */
static Point nearby(p1, p2)
	Point p1, p2;
{
	long eps, exy;	/*integers but many bits*/
	int d;
	register dy;
	register dx;
	register x1 = p1.x;
	register y1 = p1.y;
	register x2 = p2.x;
	register y2 = p2.y;
	eps = sq(x2) + sq(y2) - sq(x1) - sq(y1);
	d = eps>0? -1: 1;
	for( ; ; eps=exy, x2+=dx, y2+=dy) {
		if(abs(y2) > abs(x2)) {
			dy = d*sgn(y2);
			dx = 0;
		} else {
			dy = 0;
			dx = d*sgn(x2);
			if(dx==0)
				dx = 1;
		}
		exy = eps + (2*x2+dx)*dx + (2*y2+dy)*dy;
		if(labs(eps) <= labs(exy))
			break;
	}
	p2.x = x2;
	p2.y = y2;
	return(p2);
}
