#ifndef	MUX
#define	Jcursinhibit()	cursinhibit()
#define	Jcursallow()	cursallow()
#define	lrectf(l, r, f)	rectf(l, r, f)
#endif

#include <jerq.h>
#define	DB		((Word *)0x700000)
#define	ONSCREEN(b)	(b->base>=DB && b->base<DB+XMAX/WORDSIZE*YMAX)
/*	Fill a disc of radius r centered at x1,y1
 *	The boundary is a sequence of vertically, horizontally,
 *	or diagonally adjacent points that minimize 
 *	abs(x^2+y^2-r^2).
 *
 *	The circle is guaranteed to be symmetric about
 *	the horizontal, vertical, and diagonal axes
 */
disc(b, p, r, f)
	Bitmap *b;
	Point p;
{
	register x1=p.x, y1=p.y;
	register eps = 0;	/* x^2 + y^2 - r^2 */
	int dxsq = 1;		/* (x+dx)^2-x^2*/
	int dysq = 1 - 2*r;
	int exy;
	short inhibited;
	register x0 = x1;
	register y0 = y1 - r;
	x1++;		/* to offset jerq's half-open lines*/
	y1 += r;
	inhibited=0;
	if(f!=F_XOR && ONSCREEN(b)){
		Jcursinhibit();
		inhibited=1;
	}
	while(y1 > y0) {
		exy = eps + dxsq + dysq;
		if(-exy <= eps+dxsq) {
			lrectf(b, Rect(x0, y0, x1, y0+1), f);
			lrectf(b, Rect(x0, y1, x1, y1+1), f);
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
	lrectf(b, Rect(x0, y0, x1, y0+1), f);
	if(inhibited)
		Jcursallow();
}
