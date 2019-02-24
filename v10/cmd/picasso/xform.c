/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	@(#)picasso:xform.c	1.0	*/

#include	"picasso.h"
#include	"y.tab.h"
#include    <math.h>

/* shorthand for a 3x3 affine matrix applied as follows:
 *
 *	C(x, y)	= |x y 1| | C[0]  C[1]  0 | 
 *		 	  | C[2]  C[3]  0 |
 *		 	  | C[4]  C[5]  1 |
 *
 *		= (C[0]x+C[2]y+C[4], C[1]x+C[3]y+C[5])
 */

matmult (A, B)			/* A <- A * B; A is an object transformation */
	valtype	A[6];		/* or cur_xform updated by a new transform.  */
	double	B[6];
{
	register double	p;

	p = A[0].f * B[0] + A[1].f * B[2];
	A[1].f = A[0].f * B[1] + A[1].f * B[3];
	A[0].f = p;

	p = A[2].f * B[0] + A[3].f * B[2];
	A[3].f = A[2].f * B[1] + A[3].f * B[3];
	A[2].f = p;

	p = A[4].f * B[0] + A[5].f * B[2] + B[4];
	A[5].f = A[4].f * B[1] + A[5].f * B[3] + B[5];
	A[4].f = p;
}

matmult2 (A, B)			/* same, but updates a double array; this is  */
	double	A[6];		/* done only in going up the block hierarchy. */
	valtype	B[6];
{
	register double	p;

	p = A[0] * B[0].f + A[1] * B[2].f;
	A[1] = A[0] * B[1].f + A[1] * B[3].f;
	A[0] = p;

	p = A[2] * B[0].f + A[3] * B[2].f;
	A[3] = A[2] * B[1].f + A[3] * B[3].f;
	A[2] = p;

	p = A[4] * B[0].f + A[5] * B[2].f + B[4].f;
	A[5] = A[4] * B[1].f + A[5] * B[3].f + B[5].f;
	A[4] = p;
}

mat_inverse(Ainv, A)
	double Ainv[6];
	float  A[6];
{
	double	det, det_1;

	det = A[0] * A[3] - A[1] * A[2];
	if (det == 0.0) {
		Ainv[0] = Ainv[3] = 1.0;
		Ainv[1] = Ainv[2] = 0.0;
		Ainv[4] = Ainv[5] = 0.0;
		return;
	}
	det_1 = 1.0 / det;
	Ainv[0] = A[3] * det_1;
	Ainv[1] = - A[1] * det_1;
	Ainv[2] = - A[2] * det_1;
	Ainv[3] = A[0] * det_1;
	Ainv[4] = (A[2] * A[5] - A[3] * A[4]) * det_1;
	Ainv[5] = (A[1] * A[4] - A[0] * A[5]) * det_1;
}

double	T[6];

compose (p)			/* accumulate xforms over block hierarchy */
	obj	*p;
{
	if (p == NULL || p->o_type > TEXT) {
		T[0] = T[3] = 1;
		T[1] = T[2] =
		T[4] = T[5] = 0;
		if (p == NULL)
			return;
	}
	else {
		T[0] = p->o_mxx;	T[1] = p->o_myx;
		T[2] = p->o_mxy;	T[3] = p->o_myy;
		T[4] = p->o_mxt;	T[5] = p->o_myt;
	}
	while ((p = p->o_parent) != NULL)
		if (p->o_type <= TEXT)
			if (p->o_mxx != 1 || p->o_mxy != 0 || p->o_mxt != 0
			||  p->o_myx != 0 || p->o_myy != 1 || p->o_myt != 0)
				matmult2(T, p->o_xform);
}

get_matrix(M0, M1, M2, M3)
	double	*M0, *M1, *M2, *M3;
{
	*M0 = T[0];	*M1 = T[1];	*M2 = T[2];	*M3 = T[3];
}

double	Xformx (p, flag, x, y)
	obj	*p;
	int	flag;
	double	x, y;
{
	if (flag)
		compose(p);
	return x*T[0] + y*T[2] + T[4];
}

double	Xformy (p, flag, x, y)
	obj	*p;
	int	flag;
	double	x, y;
{
	if (flag)
		compose(p);
	return x*T[1] + y*T[3] + T[5];
}

double	Linx (p, flag, x, y)		/* linear component, only */
	obj	*p;
	int	flag;
	double	x, y;
{
	if (flag)
		compose(p);
	return T[0]*x + T[2]*y;
}

double	Liny (p, flag, x, y)
	obj	*p;
	int	flag;
	double	x, y;
{
	if (flag)
		compose(p);
	return T[1]*x + T[3]*y;
}

apply (p, M, d)			/* apply matrix M (determinant d) to object p */
	obj	*p;
	double  *M, d;
{
	float   bnd[4];

	if (p->o_type == PLACE) {
		matmult (cur_xform, M);
		if (d != 1)
			checkscale(sqrt(d));
	}
	else if (p->o_type <= TEXT) {
		matmult (p->o_xform, M);
		get_bounds(p, bnd, 0);
		track_bounds(bnd[0], bnd[1], bnd[2], bnd[3]);
		redo_gbox = 1;
	}
}

translate (p, x, y)           /* the easy case of applying a transformation */
	obj     *p;
	double	x, y;
{
	float	bnd[4];
	int	i;

	if (p->o_type == PLACE) {
		cur_xform[4].f += x;
		cur_xform[5].f += y;
	}
	else if (p->o_type <= TEXT) {
		if (p->o_type == BLOCK  || p->o_mxx != 1 || p->o_myy != 1
					|| p->o_mxy != 0 || p->o_myx != 0
					|| p->o_mxt != 0 || p->o_myt != 0) {
			p->o_mxt += x;
			p->o_myt += y;
		}
		else {	/* no existing transformation, move center */
			/* DBK: probably should also test to make sure
			   there is no higher level transformation?? */
			p->o_x += x;
			p->o_y += y;
			switch (p->o_type) {

			case LINE:
			case ARROW:
			case SPLINE:
				for (i = 0; i <= p->o_val[N_VAL+3].f; i++) {
					p->o_val[N_VAL+4+i+i].f += x;
					p->o_val[N_VAL+5+i+i].f += y;
				}
				break;
			case ARC:
			case SECTOR:
				p->o_val[N_VAL+2].f += x;
				p->o_val[N_VAL+3].f += y;
				p->o_val[N_VAL+4].f += x;
				p->o_val[N_VAL+5].f += y;
				break;
			}
		}
		get_bounds(p, bnd, 0);
		track_bounds(bnd[0], bnd[1], bnd[2], bnd[3]);
		redo_gbox = 1;
	}
}

xlate (p, q)
	obj	*p, *q;
{
	translate (p, q->o_x, q->o_y);
}

xlate_to (p, q)			/* destination given in absolute coordinates */
	obj	*p, *q;
{
	translate (p, q->o_x - p->o_x, q->o_y - p->o_y);
}

static double	picscale = 1.0;

rescale (v)		/* implicit scaling from user's "scale = v" statement */
	double	v;	/* rather unintuitively, this is like "scale O 1/v".  */
{
	picscale = v;
}

scale_pic()		/* for PIC compatibility--called at end of parse */
{
	double	sc;
	obj	*o;
	int	i;
	float	bnd[4];

	if (picscale == 0.0)
		picscale = 1.0;
	if (picscale == 1.0)
		return;
	sc = 1.0 / picscale;
	
	for (o = objhead; o != objtail; o = o->o_next) {
		if (o->o_type == PLACE || o->o_type == TEXT ||
						o->o_type == MOVE) {
			o->o_x *= sc;
			o->o_y *= sc;
	/* NOTE!! may need to adjust center offset of text */
		}
		else if (o->o_type < TEXT) {	/* PIC couldn't scale text */
			for (i = 0; i < 6; i++)
				o->o_xform[i].f *= sc;
		}
		if (o->o_type == BLOCK)		/* skip the contents */
			o = o->o_val[N_VAL].o;
	}
	/* make arrowheads large to compensate for later shrinkage */
	for (o = objhead; o != objtail; o = o->o_next) {
		if ((o->o_type == LINE || o->o_type == SPLINE ||
		    o->o_type == ARROW) && (o->o_attr & HEAD12) != 0) {
			o->o_val[N_VAL+1].f *= picscale;
			o->o_val[N_VAL+2].f *= picscale;
		}
		if (o->o_type == ARC && (o->o_attr & HEAD12) != 0) {
			o->o_val[N_VAL+8].f *= picscale;
			o->o_val[N_VAL+9].f *= picscale;
		}
	}
	picscale = 1.0;

	Gbox[2] = Gbox[3] = -(Gbox[0] = Gbox[1] = 32767);
	for (o = objhead->o_next; o != objtail; o = o->o_next) {
		get_bounds(o, bnd, 1);
		track_bounds (bnd[0], bnd[1], bnd[2], bnd[3]);
		if (o->o_type == BLOCK)
			o = o->o_val[N_VAL].o;
	}
#if 0
	cur_xform[0].f /= v;
	cur_xform[1].f /= v;
	cur_xform[2].f /= v;
	cur_xform[3].f /= v;
	checkscale(v);
#endif
}

scale (p, n, q)
	obj	*p, *q;
	int	n;
{
register double	det;
	double	M[6];

	M[0] = exprlist[nexpr-n];
	M[3] = (n == 1 ? M[0] : exprlist[nexpr-n+1]);
	nexpr = 0;
	if ((det = M[0] * M[3]) == 0.0) {
		yyerror("can't scale by 0 factor");
		return;
	}
	M[1] = M[2] = M[4] = M[5] = 0.;
	if (q != NULL) {
		M[4] = (1 - M[0]) * Xformx(q,1,q->o_x,q->o_y);
		M[5] = (1 - M[3]) * Xformy(q,0,q->o_x,q->o_y);
	}
	apply(p, M, det);
}

olpscaleobj (p, sx, sy, cx, cy)
	obj	*p;
        double sx, sy;    /* scale factor in x and y directions */
        double cx, cy;    /* center for scaling in real (inches) units */
{
register double	det;
       	 double	M[6];

	M[0] = sx;
	M[3] = sy;
	if ((det = M[0] * M[3]) == 0.0) {
		yyerror("can't scale by 0 factor");
		return;
	}
	M[1] = M[2] = 0.0;
        M[4] = cx * (1. - sx);
        M[5] = cy * (1. - sy);
        
	apply(p, M, det);
}

rotate (p, phi, q)
	obj	*p, *q;
	double	phi;	/* in radians */
{
register double	x, y;
	double	M[6];

	M[3] =   M[0] = Cos(phi);
	M[2] = -(M[1] = Sin(phi));
	if (q == NULL)
		M[4] = M[5] = 0;
	else {
		x = Xformx(q, 1, q->o_x, q->o_y);
		y = Xformy(q, 0, q->o_x, q->o_y);
		M[4] = (1 - M[0]) * x + M[1] * y;
		M[5] = (1 - M[0]) * y - M[1] * x;
	}
	apply(p, M, 1);
}

void 
rotate_point(x, y,  angle, cx, cy, pointx, pointy)
        double  x, y;
	double	angle;	/* in radians */
        double  cx, cy; /* center for rotation in screen units. */
        double *pointx, *pointy;    /* rotated point to be passed back */
{
   	double tempx, tempy;

        tempx = x - cx;
        tempy = y - cy;

        *pointx = (tempx * cos(angle)) - (tempy* sin(angle)) + cx;
        *pointy = (tempx * sin(angle)) + (tempy* cos(angle)) + cy;
}

void
olprotateobj (p, angle, cx, cy)
	obj	*p;
	double	angle;	/* in radians */
        double  cx, cy; /* center for rotation in real (inches) units. */
{
	double	M[6];

	M[3] =   M[0] = cos(angle);
	M[2] = -(M[1] = sin(angle));


	M[4] = (1 - M[0]) * cx + M[1] * cy;
	M[5] = (1 - M[0]) * cy - M[1] * cx;

	apply(p, M, 1);
}

xform (p, n)
	obj	*p;
	int	n;
{
register double	det;
	double	M[6];
	int	i;

	for (i = 0; i < n; ++i)
		M[i] = exprlist[i];
	while (i < 6)
		M[i++] = 0;
	nexpr = 0;
	if ((det = M[0]*M[3] - M[1]*M[2]) == 0)
		yyerror ("singular transformation matrix");
	else 
		apply(p, M, det);
}

reflect (p, q)
	obj	*p, *q;
{
	float	bnd[4];
	double	phi, c, s, x0, y0, x1, y1, M[6];
	int	axial;

	if (q == NULL)
		axial = x0 = x0 = x1 = y1 = 0;
	else
		switch (q->o_type) {
		case ARC:
		case ARROW:
		case LINE:
		case SPLINE:	whatpos(q, START, bnd, bnd+1);
				whatpos(q, END,   bnd+2, bnd+3);
				x0 = Xformx(q, 1, bnd[0], bnd[1]);
				y0 = Xformy(q, 0, bnd[0], bnd[1]);
				x1 = Xformx(q, 0, bnd[2], bnd[3]);
				y1 = Xformy(q, 0, bnd[2], bnd[3]);
				if (axial = (x0 != x1 || y0 != y1))
					break;
				/* fall through to default case; i.e., use  */
				/* center, for closed lines. otherwise, use */
				/* closest point to p along q as start of q */
#if 0      /*MS: The next few lines never get used */                                 
				S = (y1-y0)/(x1-x0);	/* slope of the line  */
				B = y0 - S*x0;		/* intercept term.    */
				s = -1/S;		/* perpendicular slope*/
				x = Xformx(p, 1, p->o_x, p->o_y);
				y = Xformy(p, 0, p->o_x, p->o_y);
				b = y - s*x;
				x0 = (b - B)/(S - s);	y0 = S*x0 + B;
#endif                                

		default:
				x0 = Xformx(q, 1, q->o_x, q->o_y);
				y0 = Xformy(q, 0, q->o_x, q->o_y);
				axial = 0;
		}
	if (!axial) {
		M[0] = M[3] = -1;
		M[1] = M[2] = 0;
		M[4] = 2 * x0;
		M[5] = 2 * y0;
	}
	else {
		if (x1 == x0 || y1 == y0) {	/* note: not both */
			s = 0;
			c = (x1 == x0 ? -1 : 1);
		}
		else if (fabs(x1-x0) == fabs(y1-y0)) {
			c = 0;
			s = (x1-x0 == y1-y0 ? 1 : -1);
		}
		else {
			c = cos(phi = 2 * atan2(y1-y0, x1-x0));
			s = sin(phi);
		}
		M[1] = M[2] = s;
		M[3] = -(M[0] = c);
		M[4] = x0*(1-c) - y0*s;
		M[5] = y0*(1+c) - x0*s;
	}
	apply(p, M, 1);
}

olpreflectobj (p, x0, y0, x1, y1)
	obj	*p;
        double  x0, y0, x1, y1;
{
	double	phi, c, s, M[6];
	int	axial;

        if  (x0 != x1 || y0 != y1)
           axial = 1;
        else    axial = 0;

	if (!axial) {
		M[0] = M[3] = -1;
		M[1] = M[2] = 0;
		M[4] = 2 * x0;
		M[5] = 2 * y0;
	}
	else {
		if (x1 == x0 || y1 == y0) {	/* note: not both */
			s = 0;
			c = (x1 == x0 ? -1 : 1);
		}
		else if (fabs(x1-x0) == fabs(y1-y0)) {
			c = 0;
			s = (x1-x0 == y1-y0 ? 1 : -1);
		}
		else {
			c = cos(phi = 2 * atan2(y1-y0, x1-x0));
			s = sin(phi);
		}
		M[1] = M[2] = s;
		M[3] = -(M[0] = c);
		M[4] = x0*(1-c) - y0*s;
		M[5] = y0*(1+c) - x0*s;
	}
	apply(p, M, 1);
}
