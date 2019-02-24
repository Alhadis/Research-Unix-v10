#ifndef	MUX
#define	Jcursinhibit()	cursinhibit()
#define	Jcursallow()	cursallow()
#define	lrectf(l, r, f)	rectf(l, r, f)
#define	lsegment(l, p, q, f)	segment(l, p, q, f)
#endif

#include	<jerq.h>
#define	DB		((Word *)0x700000)
#define	ONSCREEN(b)	(b->base>=DB && b->base<DB+XMAX/WORDSIZE*YMAX)
#define	ONSCREEN(b)	(b->base>=DB && b->base<DB+XMAX/WORDSIZE*YMAX)

static int yaxis;
static int xaxis;
static Point lp;

static void
scan(bp, p, f)
	Bitmap *bp;
	Point p;
	Code f;
{
	register x, y;

	if((p.y != lp.y) && (lp.y != -1))
	{
		x = xaxis - lp.x;
		y = yaxis - lp.y;
		lrectf(bp, Rect(lp.x, lp.y, x+1, lp.y+1), f);
		lrectf(bp, Rect(lp.x, y, x+1, y+1), f);
	}
	lp = p;
}
eldisc(bp, p, a, b, f)
	Bitmap *bp;
	Point p;
	int a, b;
	Code f;
{
	short inhibited;
	register x0 = p.x;
	register y0 = p.y;

	yaxis = 2*p.y;
	xaxis = 2*p.x;
	lp.y = -1;
	inhibited=0;
	if(f!=F_XOR && ONSCREEN(bp)){
		Jcursinhibit();
		inhibited=1;
	}
	if(a==0 || b==0)
		lsegment(bp, Pt(x0-a,y0-b), Pt(x0+a,y0+b), f);
	else
	{
		lrectf(bp, Rect(p.x-a, p.y, p.x+a+1, p.y+1), f);
		ellip1(bp, p, a, b, scan, Pt(0, -b), Pt(-a, 0), f);
		scan(bp, Pt(0, -1), f);
	}
	if(inhibited)
		Jcursallow();
}
