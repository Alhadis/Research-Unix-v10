/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	@(#)picasso:linegen.c	1.0	*/
#include	"picasso.h"
#include	"y.tab.h"

static	float	*dx = NULL;
static	float	*dy = NULL;
static	int	nxy = 0;
static	int	nxyvec = 0;

growxy()
{
	if (nxy++ >= nxyvec - 1) {
		dx = (float *)grow((char *)dx,"dx",nxyvec += 256,sizeof(float));
		dy = (float *)grow((char *)dy,"dy",nxyvec,sizeof(float));
	}
}

obj *linegen(type)
{
static	double	prevdx	= HT;
static	double	prevdy	= 0;
static	double	prevw	= HT10;	/* arrow dimensions */
static	double	prevh	= HT5;
static	double	prevrad	= 0;
static	int xtab[] = { 1, 0, -1, 0 };	/* R=0, U=1, L=2, D=3 */
static	int ytab[] = { 0, 1, 0, -1 };

struct	objattr	obat;
	double	theta, defx, defy, nx, ny, x0, y0, x1, y1, chop1, chop2;
	float	bbox[4];
	int	i, j, some, head, chop;
	obj	*p, *ppos;
	Attr	*ap;
	struct	symtab	*xlist, *ylist;

	nx    = curx;
	ny    = cury;
	defy  = getfval("lineht");
	defx  = getfval("linewid");
	obat.a_ht  = getfval("arrowht");
	obat.a_wid = getfval("arrowwid");
	obat.a_rad = getfval("linerad");
	obat.a_layer = (int)getfval("curlayer");
	obat.a_flags = EDGED;
	obat.a_weight = obat.a_lcolor = obat.a_pcolor = obat.a_tcolor = -1;
	obat.a_dashpat.a = (float *)0;
	set_text();
	if (nxyvec == 0)
		growxy();
	dx[0] = dy[0] = chop1 = chop2 = 0;
	nxy = some = head = chop = 0;
	for (i = 0; i < nattr; i++) {
		ap = &attr[i];
		switch (ap->a_type) {
		default:
			miscattrs(ap, &obat);
			break;
		case HEAD:
			head += ap->a_val.i;
			break;
		case CHOP:
			if (chop++ == 0)
				chop1 = chop2 = ap->a_val.f;
			else
				chop2 = ap->a_val.f;
			break;
		case SAME:
			dx[nxy]  = prevdx;
			dy[nxy]  = prevdy;
			obat.a_rad = prevrad;
			obat.a_ht  = prevh;
			obat.a_wid = prevw;
			some++;
			break;
		case LEFT:
			dx[nxy] -= (ap->a_sub==DEFAULT) ? defx : ap->a_val.f;
			some++;
			hvmode = L_DIR;
			break;
		case RIGHT:
			dx[nxy] += (ap->a_sub==DEFAULT) ? defx : ap->a_val.f;
			some++;
			hvmode = R_DIR;
			break;
		case UP:
			dy[nxy] += (ap->a_sub==DEFAULT) ? defy : ap->a_val.f;
			some++;
			hvmode = U_DIR;
			break;
		case DOWN:
			dy[nxy] -= (ap->a_sub==DEFAULT) ? defy : ap->a_val.f;
			some++;
			hvmode = D_DIR;
			break;
		case XLIST:
			xlist = findvar(ap->a_val.p, VARNAME);
			break;
		case YLIST:
			ylist = findvar(ap->a_val.p, VARNAME);
			if (some) {
				nx += dx[nxy];
				ny += dy[nxy];
				growxy();
			}
			first_xy(xlist, ylist, &x0, &y0);
			dx[nxy] = x0 - nx;
			dy[nxy] = y0 - ny;
			while (next_xy(xlist, ylist, &x0, &y0)) {
				nx += dx[nxy];
				ny += dy[nxy];
				growxy();
				dx[nxy] = x0 - nx;
				dy[nxy] = y0 - ny;
			}
			some++;
			break;
		case TO:
			if (some) {
				nx += dx[nxy];
				ny += dy[nxy];
				growxy();
			}
			ppos = ap->a_val.o;
			dx[nxy] = Xformx(ppos, 1, ppos->o_x, ppos->o_y) - nx;
			dy[nxy] = Xformy(ppos, 0, ppos->o_x, ppos->o_y) - ny;
			some++;
			break;
		case BY:
			if (some) {
				nx += dx[nxy];
				ny += dy[nxy];
				growxy();
			}
			ppos = ap->a_val.o;
			dx[nxy] = Xformx(ppos, 1, ppos->o_x, ppos->o_y);
			dy[nxy] = Xformy(ppos, 0, ppos->o_x, ppos->o_y);
			some++;
			break;
		case THEN:	/* turn off any previous accumulation */
			if (some) {
				nx += dx[nxy];
				ny += dy[nxy];
				growxy();
				dx[nxy] = dy[nxy] = some = 0;
			}
			break;
		case FROM:
		case AT:
			ppos = ap->a_val.o;
			nx = curx = Xformx(ppos, 1, ppos->o_x, ppos->o_y);
			ny = cury = Xformy(ppos, 0, ppos->o_x, ppos->o_y);
			break;
		}
	}
	if (some) {
		nx += dx[nxy];
		ny += dy[nxy];
		growxy();
		defx = dx[nxy-1];
		defy = dy[nxy-1];
	} else {
		defx *= xtab[hvmode];
		defy *= ytab[hvmode];
		dx[nxy] = defx;
		dy[nxy] = defy;
		growxy();
		nx += defx;
		ny += defy;
	}
	prevdx  = defx;
	prevdy  = defy;
	prevh   = obat.a_ht;
	prevw   = obat.a_wid;
	prevrad = obat.a_rad;
	if (chop) {
		if (chop1 == -1.0) /* just said "chop", so use default */
			chop1 = getfval("circlerad");
		if (chop2 == -1.0)
			chop2 = getfval("circlerad");
		x0 = dx[0] * dx[0] + dy[0] *dy[0];
		if (chop1 > sqrt(x0)) {
			yyerror("can't chop that much");
			chop1 = x0;
		}
		theta = atan2(dy[0], dx[0]);
		x0 = chop1 * cos(theta);
		y0 = chop1 * sin(theta);
		curx += x0;
		cury += y0;
		dx[0] -= x0;
		dy[0] -= y0;
		
		x1 = dx[nxy-1] * dx[nxy-1] + dy[nxy-1] *dy[nxy-1];
		if (chop2 > sqrt(x1)) {
			yyerror("can't chop that much");
			chop2 = x1;
		}
		theta = atan2(dy[nxy-1], dx[nxy-1]);
		x1 = chop2 * cos(theta);
		y1 = chop2 * sin(theta);
		nx -= x1;
		ny -= y1;
		dx[nxy-1] -= x1;
		dy[nxy-1] -= y1;
	}
	/* input file could specify ...to 3,2 to 3,2 ...
				so eliminate repeated pts */
	for (i = j = 0; i < nxy; i++) {
	    if (dx[i] == 0. && dy[i] == 0.)
		continue;
	    else if (j != i) {
		dx[j] = dx[i];
		dy[j] = dy[i];
	    }
	    j++;
	}
	nxy = j;
	p = makenode(type, N_VAL + 6 + 2 * nxy, obat.a_layer);
	primattrs(p, &obat);
	text_bounds(p);
	prevrad	= p->o_val[N_VAL].f = obat.a_rad;
	p->o_val[N_VAL+3].f = nxy;		/* note: segments, not points */
	p->o_val[N_VAL+4].f = bbox[0] = bbox[2] = curx;
	p->o_val[N_VAL+5].f = bbox[1] = bbox[3] = cury;

	for (i = 0, j = N_VAL+6; i < nxy; i++, j += 2) {
			/* restoring absolute coordinates */
		p->o_val[j].f   = curx += dx[i];
		p->o_val[j+1].f = cury += dy[i];
		extreme(curx, cury, bbox);	/* OVERestimate, for splines */
#if 0
		if (type == LINE || type == ARROW)
			extreme(curx, cury, bbox);
		else if (type == SPLINE) {
			if (i < nxy-1) {
				/* to compute approx extreme of spline at p, */
				/* compute midway between p-1 and p+1,	     */
				/* then go 3/4 from there to p		     */
				double ex, ey, xi, yi, xi1, yi1;

				xi  = curx-dx[i];
				yi  = cury-dy[i];    /* p-1   */
				xi1 = curx+dx[i+1];
				yi1 = cury+dy[i+1];  /* p+1   */
				ex = (curx+xi1)/2;
				ey = (cury+yi1)/2;   /* midpt */
				ex += 0.75*(curx-ex);	ey += 0.75*(cury-ey);
				extreme(ex, ey, bbox);
			}
			else extreme(curx, cury, bbox);
		}
#endif
	}
	if (head || type == ARROW) {
		if (head == 0)
			head = HEAD2;	/* default arrow head */
		p->o_attr |= head | arrowfill();
		p->o_val[N_VAL+1].f = obat.a_wid;
		p->o_val[N_VAL+2].f = obat.a_ht;
		if (head & HEAD1)
			track_head (p->o_val[N_VAL+6].f, p->o_val[N_VAL+7].f,
				    p->o_val[N_VAL+4].f, p->o_val[N_VAL+5].f,
					prevw, prevh, bbox);
		if (head & HEAD2) {
			j = N_VAL + 2 + 2 * nxy;
			track_head (p->o_val[j+0].f, p->o_val[j+1].f,
				    p->o_val[j+2].f, p->o_val[j+3].f,
					prevw, prevh, bbox);
		}
	}
	track_bounds(bbox[0]-p->o_weight/2, bbox[1]-p->o_weight/2,
		     bbox[2]+p->o_weight/2, bbox[3]+p->o_weight/2);
	p->o_x = 0.5 * (bbox[0] + bbox[2]);
	p->o_y = 0.5 * (bbox[1] + bbox[3]);
	p->o_wid = bbox[2] - bbox[0];
	p->o_ht  = bbox[3] - bbox[1];
	return(p);
}

arrowfill ()	/* a bit of a kludge for an obsolescent feature; arroprevw */
{		/* now is taken simply as odd (fill) or even (nofill) and  */
		/* has no implication of "number of lines drawn" as in pic */

	return (((int)getfval("arrowfill")) % 2) * HEADFILL;
}

/* NOTE: DBK added bbox parameter to track_head 9/30/89.  Only calls to
   track_head are in linegen() above.  The purpose is to get o_wid and o_ht
   computed large enough so get_bounds won't lie about arrows.  Note that
   o_x and o_y may also be affected.  */
static
track_head (x0, y0, x1, y1, w, h, bbox)
	double	x0, y0, x1, y1, w, h;
	float	bbox[];
{
	double	rot, hyp, phi;

	rot = atan2(w/2, h);
	hyp = hypot(w/2, h);
	phi = atan2(y1-y0, x1-x0) + M_PI - rot;
	x0 = x1 + hyp * cos(phi);	y0 = y1 + hyp * sin(phi);
	extreme (x0, y0, Gbox);
	extreme (x0, y0, bbox);
	phi += 2 * rot;
	x0 = x1 + hyp * cos(phi);	y0 = y1 + hyp * sin(phi);
	extreme (x0, y0, Gbox);
	extreme (x0, y0, bbox);
}
