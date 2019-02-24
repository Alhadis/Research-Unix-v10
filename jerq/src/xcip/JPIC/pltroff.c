/*
 *	dmd driver
 */

#include <stdio.h>
#include <math.h>
#include "y.tab.h"
#include "pic.h"


#define	abs(n)	(n >= 0 ? n : -(n))
#define	PI	3.141592654
#define	PI2	PI/2

extern	int	res;
extern	int	DX;	/* step size in x */
extern	int	DY;	/* step size in y */

extern	float	hshift;	/* how much to move left by for text */
extern	float	vshift;	/* how much down */

/* scaling stuff, specific to typesetter */
/* defined by s command as X0,Y0 to X1,Y1 */
/* output dimensions set by -l,-w options to 0,0 to hmax, vmax */
/* default output is 6x6 inches */

float	xscale;
float	yscale;

int	hpos	= 0;	/* current horizontal position in output coordinate system */
int	vpos	= 0;	/* current vertical position; 0 is top of page */

int	X0, Y0;		/* left bottom of input */
int	X1, Y1;		/* right top of input */
#define	CX	0	/* center of dmd screen */
#define	CY	0
#define	JERQRES	100

int	hmax;		/* right end of output */
int	vmax;		/* top of output (down is positive) */

extern	float	deltx;
extern	float	delty;
extern	int	xmin, ymin, xmax, ymax, sxmin, symin, sxmax, symax;
extern	int	crop;

openpl(s)	/* initialize device */
char *s;	/* residue of .PS invocation line */
{
	hpos = vpos = 0;
	res = JERQRES;
	if (delty == 0)
		delty = deltx;	/* -w alone implies -s */
	hmax = vmax = 8 * res;	/* default = 8 x 8 */
	if (deltx > 0 && delty > 0) {	/* have to change default size */
		if (deltx < 9)	/* assume inches were specified */
			hmax = res * deltx;
		else
			hmax = deltx;
		if (deltx < 9)	/* assume inches were specified */
			vmax = res * delty;
		else
			vmax = delty;
	}
	if (crop) {
		if (xmax == xmin)
			space(xmin, ymin, xmin + ymax-ymin, ymax);
		else
			space(xmin, ymin, xmax, ymin + xmax-xmin);	/* assumes 1:1 aspect ratio */
	}
	else
		space(sxmin, symin, sxmax, symax);
	printf("... %d %d %d %d %d %d %d %d\n",
		(xmin), (ymin), (xmax), (ymax),
		(sxmin), (symin), (sxmax), (symax));
	printf("... %du %du %du %du %du %du %du %du\n",
		xconv(xmin), yconv(ymin), xconv(xmax), yconv(ymax),
		xconv(sxmin), yconv(symin), xconv(sxmax), yconv(symax));
	printf(".PS %d %d %s", yconv(ymin), xconv(xmax), s);
		/* assumes \n comes as part of s */
	erase();
}

closepl(type)	/* clean up after finished */
{
	printf("S\n");
}

move(x, y)	/* go to position x, y in external coords */
int x, y;
{
	hgoto(xconv(x));
	vgoto(yconv(y));
}

movehv(h, v)	/* go to internal position h, v */
int h, v;
{
	hgoto(h);
	vgoto(v);
}

hmot(n)	/* generate n units of horizontal motion */
int n;
{
	hpos += n;
}

vmot(n)	/* generate n units of vertical motion */
int n;
{
	vpos += n;
}

hgoto(n)
{
	hpos = n;
}

vgoto(n)
{
	vpos = n;
}

hvflush()	/* get to proper point for output */
{
	printf("m %d %d\n", hpos, vpos);
}

troff(s)	/* output troff right here */
char *s;
{
	printf("%s\n", s);
}

label(s, t, nh)	/* text s of type t nh half-lines up */
char *s;
int t, nh;
{
	int x0, y0, x1, y1;
	int q;
	char *p;

	if (t == 'A')
		nh++;
	else if (t == 'B')
		nh--;
	vpos += nh * 10;	/* approx 1/2 line */
	switch (t) {
	default:
		t = ' ';
		/* FALL THROUGH */
	case 'L':
	case 'A':
	case 'B':
	case 'C':
	case 'R':
		printf("t %d %d %c%s\n", hpos, vpos, t, s);
		break;
	}
}

/* line(x0, y0, x1, y1)	/* draw line from x0,y0 to x1,y1 */
/* int x0, y0, x1, y1;
/* {
/* 	printf("l %d %d %d %d\n", xconv(x0), yconv(y0), xconv(x1), yconv(y1));
/* }
*/

arrow(x0, y0, x1, y1, w, h)	/* draw arrow (without line), head wid w & len h */
{
	double alpha, rot, hyp;
	int dx, dy;
	extern int dbg;

	rot = atan2( ((double) w) / 2, (double) h );
	hyp = sqrt( (float) (w/2 * w/2 + h * h) );
	alpha = atan2((float)(y1-y0), (float)(x1-x0));
	if (dbg)
		printf("rot=%f, hyp=%f, alpha=%f\n", rot, hyp, alpha);
	dx = hyp * cos(alpha + PI + rot) + 0.5;
	dy = hyp * sin(alpha + PI + rot) + 0.5;
	if (dbg) printf("dx,dy = %d,%d\n", dx, dy);
	/* line(x1+dx, y1+dy, x1, y1); */
	printf("l 2 %d %d %d %d\n",
		xconv(x1+dx), yconv(y1+dy), xconv(x1), yconv(y1));
	dx = hyp * cos(alpha + PI - rot) + 0.5;
	dy = hyp * sin(alpha + PI - rot) + 0.5;
	if (dbg) printf("dx,dy = %d,%d\n", dx, dy);
	/* line(x1+dx, y1+dy, x1, y1); */
	printf("l 2 %d %d %d %d\n",
		xconv(x1+dx), yconv(y1+dy), xconv(x1), yconv(y1));
}

box(x0, y0, x1, y1, dotdash, ddval)
{
	printf("b");
	if (dotdash == DOT)
		printf(". %d", xsc(ddval));
	else if (dotdash == DASH)
		printf("- %d", xsc(ddval));
	printf(" %d %d %d %d\n", xconv(x0), yconv(y0), xconv(x1), yconv(y1));
}

circle(x, y, r)
{
	hpos = xconv(x);
	vpos = yconv(y);
	printf("c %d %d %d\n", hpos, vpos, xsc(r) );
}

spline(x, y, n, p, head)
	int x, y, n, *p, head;
{
	int i, j, dx, dy;

	printf("~ ");
	if (head & HEAD1)
		printf("<");
	if (head & HEAD2)
		printf(">");
	hpos = xconv(x);
	vpos = yconv(y);
	printf(" %d %d %d", n+1, hpos, vpos );
	for (i = 0; i < 2 * n; i += 2) {
		x += p[i];
		y += p[i+1];
		printf(" %d %d", xconv(x), yconv(y));
	}
	printf("\n");
}

line(x, y, n, p, head, dotdash, ddval)
	int x, y, n, *p, head;
	int dotdash, ddval;
{
	int i, j, dx, dy;

	printf("l ");	/* a line */
	if (head & HEAD1)
		printf("<");
	if (head & HEAD2)
		printf(">");
	if (dotdash == DOT)
		printf(". %d", xsc(ddval));
	else if (dotdash == DASH)
		printf("- %d", xsc(ddval));
	hpos = xconv(x);
	vpos = yconv(y);
	printf(" %d %d %d", n+1, hpos, vpos );
	for (i = 0; i < 2 * n; i += 2) {
		x += p[i];
		y += p[i+1];
		printf(" %d %d", xconv(x), yconv(y));
	}
	printf("\n");
}


ellipse(x, y, r1, r2)
{
	r1 = xsc(r1);
	r2 = ysc(r2);
	printf("e %d %d %d %d\n", xconv(x), yconv(y), abs(r1), abs(r2));
}

arc(x, y, x0, y0, x1, y1)	/* draw arc with center x,y */
{
	float r;
	float dx2, dy2, phi, ht, d;
	int cx, cy;

	r = sqrt( (float) (x0-x) * (x0-x) + (float) (y0-y) * (y0-y) );
	/* decide which radius is really needed: */
	dx2 = (float)(x1 - x0) / 2;
	dy2 = (float)(y1 - y0) / 2;
	phi = atan2(dy2, dx2) + PI2;
	ht = sqrt(r*r - (dx2*dx2 + dy2*dy2));
	cx = x0 + dx2 + ht * cos(phi) + 0.5;
	cy = y0 + dy2 + ht * sin(phi) + 0.5;
	if (abs(cx-x) >= 5 || abs(cy-y) >= 5)
		r = -r;	/* wrong center was picked */
	printf("a %d %d %d %d %d %d\n", xconv(x), yconv(y),
		xconv(x0), yconv(y0), xconv(x1), yconv(y1));
}

erase()	/* get to bottom of frame */
{
	printf("E\n");
	fflush(stdout);
}

point(x, y)	/* put point at x,y */
int x, y;
{
	static char *temp = ".";

	move(x, y);
	label(temp, 'L');
}

space(x0, y0, x1, y1)	/* set limits of page */
int x0, y0, x1, y1;
{
	if (x0 == x1)
		x1 = x0 + 1;
	if (y0 == y1)
		y1 = y0 - 1;	/* kludge */
	X0 = x0;
	Y0 = y0;
	X1 = x1;
	Y1 = y1;
	xscale = (float) hmax / (float) (X1-X0);
	yscale = (float) vmax / (float) (Y0-Y1);
}

xconv(x)	/* convert x from external to internal form */
int x;
{
	int v;

	v = (x-X0) * xscale + 0.5;
	return v + CX;
}

xsc(x)	/* convert x from external to internal form, scaling only */
int x;
{
	int v;

	v = (x) * xscale + 0.5;
	return v;
}

yconv(y)	/* convert y from external to internal form */
int y;
{
	int v;

	y += Y1 - ymax;
	v = (y-Y1) * yscale + 0.5;
	return v + CY;
}

ysc(y)	/* convert y from external to internal form, scaling only */
int y;
{
	int v;

	v = (y) * yscale + 0.5;
	return v;
}

linemod(s)
char *s;
{
}

dot() {
	hvflush();
	printf("tc.\n");
}
