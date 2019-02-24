
/*
 *
 * Drawing routines used dimpress. Most, if not all the complicated code
 * has been eliminated for systems running version 1.9 or later systems.
 *
 * If your printer doesn't support the Impress graphics primitives used
 * in this file make sure you use an old version of draw.c.
 *
 * Apparently there's a bug in the Impress version 2.0 ellipse code. If
 * you're drawing part of an ellipse and radiusa < radiusb things get
 * messed up. That won't happen often - in fact only when the apsect
 * ratio is less than 1. All other cases seem to work properly. I expect
 * the problem to be fixed soon so I haven't bothered trying to get
 * around it.
 *
 */


#include <stdio.h>
#include <math.h>

#include "gen.h"			/* general purpose definitions */
#include "impcodes.h"			/* version 2.0 Impress codes */


#define XSCALE(A)	((int) ((A) * xfac + .5 + xoffset))
#define YSCALE(A)	((int) ((A) * yfac + .5 + yoffset))


int			maxdots = 32000;

/*
 *
 * All these guys should be defined in file dimpress.c.
 *
 */


extern int		hpos;
extern int		vpos;
extern int		xoffset;
extern int		yoffset;
extern float		xfac;
extern float		yfac;
extern int		lastx;
extern int		lasty;
extern int		DX;
extern int		DY;
extern FILE		*tf;


/*****************************************************************************/


dist(x1, y1, x2, y2)


	int	x1, y1, x2, y2;


{


	float	dx, dy;


/*
 *
 * Returns the integer distance between points (x1, y1) and (x2, y2).
 *
 */


	dx = x2 - x1;
	dy = y2 - y1;

	return(sqrt(dx*dx + dy*dy) + 0.5);

}    /* End of dist */


/*****************************************************************************/


char *getstr(p, temp)


	char	*p;			/* next string copied from here */
	char	*temp;			/* and saved here */


{


/*
 *
 * Copies the next non-blank string from p to temp. p is updated and the final
 * final value of the pointer is returned to the caller. Probably could do all
 * this stuff just as easily using strtok().
 *
 */


	while (*p == ' ' || *p == '\t' || *p == '\n')
		p++;

	if (*p == '\0') {
		temp[0] = 0;
		return(NULL);
	}

	while (*p != ' ' && *p != '\t' && *p != '\n' && *p != '\0')
		*temp++ = *p++;
	*temp = '\0';

	return(p);

}    /* End of getstr */


/*****************************************************************************/


xymove(x, y)


	int	x, y;			/* move printer to this point */


{


/*
 *
 * Moves the printer's current position to the pixel corresponding to the
 * troff position (x, y).
 *
 */


	putc(ASETAH, tf);
	putint(lastx = XSCALE(x), tf);
	putc(ASETAV, tf);
	putint(lasty = YSCALE(y), tf);

}   /* End of xymove */


/*****************************************************************************/


drawline(dx, dy)


	int	dx, dy;			/* endpoint is (hpos+dx, vpos+dy) */


{


/*
 *
 * Draws a line form the current position (hpos, vpos) to the point
 * (hpos+dx, vpos+dy), and leaves the current position at the endpoint.
 * We're assuming here that after the path is drawn Impress sets its current
 * position to the endpoint of the path. If that's not the case call xymove()
 * before returning.
 *
 */


	putc(ASETP, tf);		/* define the path */
	putint(2, tf);

	putint(XSCALE(hpos), tf);
	putint(YSCALE(vpos), tf);
	putint(XSCALE(hpos + dx), tf);
	putint(YSCALE(vpos + dy), tf);

	putc(ADRAWP, tf);		/* then draw it */
	putc(OR_OP, tf);

	hgoto(hpos + dx);		/* adjust troff's position */
	vgoto(vpos + dy);

	xymove(hpos, vpos);		/* think we need it */

}    /* End of drawline */


/*****************************************************************************/


drawcirc(d)


	int	d;			/* diameter of the circle */


{


/*
 *
 * Draws a circle of diameter d with the left 'side' of the circle at the
 * current point. After we're finished drawing we move the current position
 * to the right side.
 *
 */

	drawellip(d, d);

}    /* End of drawcirc */


/*****************************************************************************/


drawellip(a, b)


	int	a, b;			/* axes lengths for the ellipse */


{


/*
 *
 * Draws an ellipse having axes lengths horizontally and vertically of a and
 * b. The left side of the ellipse is at the current point. After we're done
 * drawing the path we move the current position to the right side.
 *
 */


	xymove(hpos + a/2, vpos);

	putc(AELLIPSEARC, tf);		/* define the ellipse */
	putint((int) ((a * xfac) / 2 + .5), tf);
	putint((int) ((b * yfac) / 2 + .5), tf);
	putint(0, tf);
	putint(0, tf);
	putint(16383, tf);

	putc(ADRAWP, tf);
	putc(OR_OP, tf);

	hgoto(hpos + a);		/* where troff wants to be */
	vgoto(vpos);

	xymove(hpos, vpos);		/* move the printer there too */

}    /* End of drawellip */


/*****************************************************************************/


drawarc(dx1, dy1, dx2, dy2)


	int	dx1, dy1;		/* vector from current pos to center */
	int	dx2, dy2;		/* from center to end of the arc */


{


	int	angle0;			/* to get to first point */
	int	angle1;			/* for second point - always negative */
	int	radius;


/*
 *
 * Draws a circular arc from the current position (hpos, vpos) to the point
 * (hpos + dx1 + dx2, vpos + dy1 + dy2). The center of the circle is the
 * point (hpos + dx1, vpos + dy1). Arcs always go in the counter-clockwise
 * direction from the starting point to the endpoint.
 *
 * After we're finished troff expects its current position to be the end
 * point of the arc. I've also made sure that Impress has its current
 * position set there too, although it's probably not necessary.
 *
 * Used elliptical arcs in case the aspect ratio is different than 1.0.
 * Things seem to work OK for aspect ratios greater than or equal to
 * 1.0, but there appears to be a bug in the Impress ellipse drawing
 * code that causes problems if radiusa < radiusb. I haven't bothered
 * to try and fix the code since I expect the Impress problem to
 * disappear, and besides apsect ratios different from 1.0 are rarely
 * used.
 *
 */


	if ( dx1 == 0  &&  dy1 == 0 )	/* nothing to do */
		return;

	xymove(hpos + dx1, vpos + dy1);	/* center of the circle */

	radius = sqrt((float) dx1 * dx1 + (float) dy1 * dy1) + .5;
	angle0 = (atan2((double) -dy1, (double) -dx1) * (1 << 14)) / (2 * PI);
	angle1 = (atan2((double) dy2, (double) dx2) * (1 << 14)) / (2 * PI);

	if ( angle0 < angle1 )
		if ( angle0 < 0 ) angle0 += (1 << 14);
		else angle1 -= (1 << 14);

	putc(AELLIPSEARC, tf);		/* define the arc */
	putint((int) (radius * xfac + .5), tf);
	putint((int) (radius * yfac + .5), tf);
	putint(0, tf);
	putint(angle0, tf);
	putint(angle1, tf);

	putc(ADRAWP, tf);		/* draw the arc */
	putc(OR_OP, tf);

	hgoto(hpos + dx1 + dx2);	/* all done so get to the end point */
	vgoto(vpos + dy1 + dy2);

	xymove(hpos, vpos);		/* make sure the printer's there too */

}   /* End of drawarc */


/*****************************************************************************/


drawspline(fp, flag)
	FILE	*fp;			/* input for point list */
	int	flag;			/* flag!=1 connect end points */
{
	int x[50], y[50], xp, yp, pxp, pyp;
	float t1, t2, t3, w;
	int i, j, numdots, N;
	char temp[50], *p, *getstr();
/*
 *
 * I haven't made any real changes to this routine. Efficiency depends
 * on the size of DX and DY.
 *
 */
	for (N = 2;  N < sizeof(x)/sizeof(x[0]); N++)
		if (fscanf(fp, "%d %d", &x[N], &y[N]) != 2)
			break;
	x[0] = x[1] = hpos;
	y[0] = y[1] = vpos;
	for (i = 1; i < N; i++) {
		x[i+1] += x[i];
		y[i+1] += y[i];
	}
	x[N] = x[N-1];
	y[N] = y[N-1];
	pxp = pyp = -9999;
	for (i = 0; i < N-1; i++) {	/* interval */
		numdots = (dist(x[i],y[i], x[i+1],y[i+1]) + dist(x[i+1],y[i+1], x[i+2],y[i+2])) / 2;
		numdots /= DX;
		numdots = MIN(numdots, maxdots);
		for (j = 0; j < numdots; j++) {	/* points within */
			w = (float) j / numdots;
			t1 = 0.5 * w * w;
			w = w - 0.5;
			t2 = 0.75 - w * w;
			w = w - 0.5;
			t3 = 0.5 * w * w;
			xp = t1 * x[i+2] + t2 * x[i+1] + t3 * x[i] + 0.5;
			yp = t1 * y[i+2] + t2 * y[i+1] + t3 * y[i] + 0.5;
			if (xp != pxp || yp != pyp) {
				if (flag!=1 || (i!=0 && i!=N-2))   /* pg 3/21/87 */
					drawline(xp-hpos,yp-vpos); /* jsp 6/15/83 */
				hgoto(xp);
				vgoto(yp);
				pxp = xp;
				pyp = yp;
			}
		}
	}

}   /* End of drawspline */


/*****************************************************************************/


