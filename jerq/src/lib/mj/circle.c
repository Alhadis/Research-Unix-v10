#ifndef	MUX
#define	Jcursinhibit()	cursinhibit()
#define	Jcursallow()	cursallow()
#define	lrectf(l, r, f)	rectf(l, r, f)
#define	lpoint(l, p, f)	point(l, p, f)
#endif

#include <jerq.h>
#define	DB		((Word *)0x700000)
#define	ONSCREEN(b)	(b->base>=DB && b->base<DB+XMAX/WORDSIZE*YMAX)
/*	Form a circle of radius r centered at x1,y1
 *	The boundary is a sequence of vertically, horizontally,
 *	or diagonally adjacent points that minimize 
 *	abs(x^2+y^2-r^2).
 *
 *	The circle is guaranteed to be symmetric about
 *	the horizontal, vertical, and diagonal axes
 */
circle(b,p,r,f)
	Bitmap *b;
	Point p;
{
	short inhibited;
	int x1=p.x;
	register y1=p.y;
	register eps = 0;	/* x^2 + y^2 - r^2 */
	register dxsq = 1;		/* (x+dx)^2-x^2*/
	register dysq = 1 - 2*r;
	register exy;
	int x0 = x1;
	register y0 = y1 - r;
	y1 += r;
	if (f == F_XOR) {	/* endpoints coincide */
		point(b,Pt(x0,y0),f);
		point(b,Pt(x0,y1),f);
		inhibited=0;
	}else if(ONSCREEN(b)){
		Jcursinhibit();
		inhibited=1;
	}
	while(y1 > y0) {
		lpoint(b,Pt(x0,y0),f);
		lpoint(b,Pt(x0,y1),f);
		lpoint(b,Pt(x1,y0),f);
		lpoint(b,Pt(x1,y1),f);
		exy = eps + dxsq + dysq;
		if(-exy <= eps+dxsq) {
			y1--;
			y0++;
			eps += dysq;
			dysq += 2;
		}
		if(exy <= -eps) {
			x1++;
			x0--;
			eps += dxsq;
			dxsq += 2;
		}
	}
	lpoint(b,Pt(x0,y0),f);
	lpoint(b,Pt(x1,y0),f);
	if(inhibited)
		Jcursallow();
}
