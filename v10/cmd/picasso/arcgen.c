/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	@(#)picasso:arcgen.c	1.0	*/
#include	"picasso.h"
#include	"y.tab.h"

obj *arcgen(type)	/* handles circular and (eventually) elliptical arcs */
{
	static	double	prevwid	= HT10;
	static	double	prevht	= HT5;
	static	double	prevrad	= HT2;
	static	int	dtox[2][4]   ={ 1, -1, -1, 1, 1, 1, -1, -1 };
	static	int	dtoy[2][4]   ={ 1, 1, -1, -1, -1, 1, 1, -1 };
	static	int	dctrx[2][4]  ={ 0, -1, 0, 1, 0, 1, 0, -1 };
	static	int	dctry[2][4]  ={ 1, 0, -1, 0, -1, 0, 1, 0 };
	static	int	nexthv[2][4] ={ U_DIR, L_DIR, D_DIR, R_DIR, D_DIR,
					R_DIR, U_DIR, L_DIR };
struct	objattr	obat;
	double	dx2, dy2, phi, r, d, fromx, fromy, tox, toy;
	int	i, head, to, at, cw;
	obj	*p, *ppos;
	Attr	*ap;

	obat.a_ht  = getfval("arrowht");
	obat.a_wid = getfval("arrowwid");
	obat.a_rad = getfval("arcrad");
	obat.a_layer = (int)getfval("curlayer");
	obat.a_flags = EDGED;
	obat.a_weight = obat.a_lcolor = obat.a_pcolor = obat.a_tcolor = -1;
	obat.a_dashpat.a = (float *)0;
	set_text();
	fromx = curx;
	fromy = cury;
	head = to = at = cw = 0;
	for (i = 0; i < nattr; i++) {
		ap = &attr[i];
		switch (ap->a_type) {
		default:
			miscattrs(ap, &obat);
			break;
		case HEAD:
			head += ap->a_val.i;
			break;
		case CW:
			cw = 1;
			break;
		case FROM:	/* start point of arc */
			ppos = ap->a_val.o;
			fromx = Xformx(ppos, 1, ppos->o_x, ppos->o_y);
			fromy = Xformy(ppos, 0, ppos->o_x, ppos->o_y);
			break;
		case TO:	/* end point of arc */
			ppos = ap->a_val.o;
			tox = Xformx(ppos, 1, ppos->o_x, ppos->o_y);
			toy = Xformy(ppos, 0, ppos->o_x, ppos->o_y);
			to++;
			break;
		case AT:	/* center of arc */
			ppos = ap->a_val.o;
			curx = Xformx(ppos, 1, ppos->o_x, ppos->o_y);
			cury = Xformy(ppos, 0, ppos->o_x, ppos->o_y);
			at = 1;
			break;
		case UP:
			hvmode = U_DIR;
			break;
		case DOWN:
			hvmode = D_DIR;
			break;
		case RIGHT:
			hvmode = R_DIR;
			break;
		case LEFT:
			hvmode = L_DIR;
			break;
		case SAME:
			obat.a_ht  = prevht;
			obat.a_wid = prevwid;
			obat.a_rad = prevrad;
			break;
		}
	}
	if (!at && !to) {	/* the defaults are mostly OK */
		curx = fromx + obat.a_rad * dctrx[cw][hvmode];
		cury = fromy + obat.a_rad * dctry[cw][hvmode];
		tox  = fromx + obat.a_rad * dtox[cw][hvmode];
		toy  = fromy + obat.a_rad * dtoy[cw][hvmode];
		hvmode = nexthv[cw][hvmode];
	}
	else if (!at) {
		dx2 = (tox - fromx) / 2;
		dy2 = (toy - fromy) / 2;
		phi = atan2(dy2, dx2) + (cw ? -M_PI_2 : M_PI_2);
		if (obat.a_rad <= 0.0)
			obat.a_rad = sqrt(dx2*dx2+dy2*dy2);
		for (r=obat.a_rad; (d = r*r - (dx2*dx2+dy2*dy2)) <= 0.0; r *= 2)
			;	/* this kludge gets around too-small radii */
		obat.a_rad = r;
		d = sqrt(d);
		curx = fromx + dx2 + d * cos(phi);
		cury = fromy + dy2 + d * sin(phi);
	}
	else if (at && !to) {	/* do we have all the cases??? */
		tox = fromx + obat.a_rad * dtox[cw][hvmode];
		toy = fromy + obat.a_rad * dtoy[cw][hvmode];
		hvmode = nexthv[cw][hvmode];
	}
	p = makenode(type, N_VAL + 10, obat.a_layer);
	prevrad = p->o_val[N_VAL+0].f = p->o_val[N_VAL+1].f = obat.a_rad;
	prevwid = p->o_val[N_VAL+8].f = obat.a_wid;
	prevht  = p->o_val[N_VAL+9].f = obat.a_ht;
	if (cw) {	/* interchange roles of from-to and heads */
		double temp;
		temp = fromx; curx = fromx = tox; tox = temp;
		temp = fromy; cury = fromy = toy; toy = temp;
		if (head == HEAD1)
			head = HEAD2;
		else if (head == HEAD2)
			head = HEAD1;
		p->o_attr |= CW_ARC;
	}
	else {
		curx = tox;
		cury = toy;
	}
	p->o_val[N_VAL+2].f = fromx;
	p->o_val[N_VAL+3].f = fromy;
	p->o_val[N_VAL+4].f = tox;
	p->o_val[N_VAL+5].f = toy;
	if (head)
		p->o_attr |= head | arrowfill();
	primattrs(p, &obat);
	text_bounds(p);
	arc_extreme(p);
	return(p);
}

quadrant(x,y)
	double x, y;
{
	if (     x>=0.0 && y> 0.0) return(1);
	else if( x< 0.0 && y>=0.0) return(2);
	else if( x<=0.0 && y< 0.0) return(3);
	else if( x> 0.0 && y<=0.0) return(4);
	else
		{ fprintf(stderr,"can't happen: x,y=%g,%g",x,y); exit(1);}
}

/***************************************************************************
   bounding box of a circular arc             Eric Grosse  24 May 84

Conceptually, this routine generates a list consisting of the start,
end, and whichever north, east, south, and west points lie on the arc.
The bounding box is then the range of this list.
    list = {start,end}
    j = quadrant(start)
    k = quadrant(end)
    if( j==k && long way 'round )  append north,west,south,east
    else
      while( j != k )
         append center+radius*[j-th of north,west,south,east unit vectors]
         j += 1  (mod 4)
    set bounds (return object with wid/ht and offset from arc center)
The following code implements this, with simple optimizations.
***********************************************************************/

arc_extreme(p)
	obj	*p;
{
	/* assumes center isn't too far out */

	double	x0, y0, x1, y1, xc, yc;  /* start, end, center */
	double	r, xmin, ymin, xmax, ymax, wgt;
	int	j, k;

	x0 = p->o_val[N_VAL+2].f - (xc = p->o_x);    /* translate to center */
	y0 = p->o_val[N_VAL+3].f - (yc = p->o_y);
	x1 = p->o_val[N_VAL+4].f - xc;
	y1 = p->o_val[N_VAL+5].f - yc;
	xmin = (x0<x1)?x0:x1; ymin = (y0<y1)?y0:y1;
	xmax = (x0>x1)?x0:x1; ymax = (y0>y1)?y0:y1;
	r = p->o_val[N_VAL].f;
	if (r > 0.0) {
		j = quadrant(x0,y0);
		k = quadrant(x1,y1);
		if (j == k && y1*x0 < x1*y0) {
		/* viewed as complex numbers, if Im(z1/z0)<0, arc is big */
			if( xmin > -r) xmin = -r; if( ymin > -r) ymin = -r;
			if( xmax <  r) xmax =  r; if( ymax <  r) ymax =  r;
		} else {
			while (j != k) {
				switch (j) {
					case 1: if( ymax <  r) ymax =  r; break; /* north */
					case 2: if( xmin > -r) xmin = -r; break; /* west */
					case 3: if( ymin > -r) ymin = -r; break; /* south */
					case 4: if( xmax <  r) xmax =  r; break; /* east */
				}
				j = j%4 + 1;
			}
		}
	}
	if (p->o_type == SECTOR) {	/* include center */
		if (xmin * xmax > 0)
			if (xmin > 0) xmin = 0; else xmax = 0;
		if (ymin * ymax > 0)
			if (ymin > 0) ymin = 0; else ymax = 0;
	}
	p->o_wid = xmax - xmin;
	p->o_ht  = ymax - ymin;
	p->o_val[N_VAL+6].f = -0.5 * (xmin+xmax);  /* record offset to center */
	p->o_val[N_VAL+7].f = -0.5 * (ymin+ymax);
	wgt = p->o_weight/2;
	track_bounds(xmin+xc-wgt, ymin+yc-wgt, xmax+xc+wgt, ymax+yc+wgt);
}
