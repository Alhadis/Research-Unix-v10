/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	@(#)picasso:print.c	1.0	*/

#include	<assert.h>
#include	<ctype.h>
#include	<string.h>
#include	"picasso.h"
#include	"y.tab.h"

extern int  eqn_count;
int	flyback;
int	redo_gbox = 0;
int	objcount  = 0;

print_buf()
{
	int	n;
	obj	*p;

	if (++objcount > objbuf) {
		p = objhead->o_next;
		if (p->o_type != BLOCK)
			print_obj(p->o_layer, p);
		else if (p->o_val[N_VAL].o == (obj *)0)
			return;
		else for (n = draftlayer; n <= top_layer; n++)
			print_obj(n,p);
		freenode(p);
	}
}

extern	FILE	*pipefp, *eqnfp;
extern	char	psfname[];

print(c)		/* show everything (above draft layers) */
	int	c;
{
	int	n;

	/* Added 10/15/90--DBK.  Is there any reason NOT to do this? */
	redo_gbox = 1;
	if (pipefp) {
		pclose(pipefp);
		pipefp = NULL;
		if ((eqnfp = fopen(psfname,"r")) == NULL)
			fatal("cannot read eqn output");
	}
	flyback = (c == 'F');
	if (c != 'N')
		for (n = draftlayer; n <= top_layer; ++n)
			print_layer(n);
	if (eqnfp) {
		fclose(eqnfp);
		eqnfp = NULL;
		eqn_count = 0;
		unlink(psfname);
	}
}

print_layer(n)	/* show contents of a single layer (maybe a draft layer) */
	int	n;
{
	obj	*p;

	for (p = objhead->o_next; p!= objtail; p = p->o_next)
		p = print_obj(n, p);
}

print_bnd(p, q)	/* show everything (non-draft) between given bounds */
	obj	*p, *q;
{
	int	n;

	for (n = draftlayer; n <= top_layer; ++n)
		print_layer_bnd(n, p, q);
}

print_layer_bnd(n, p, q)	/* show objects in layer n from p to q */
	int	n;
	obj	*p, *q;
{
	obj	*r;
	float	x0, y0, x1, y1, bnd[4];

	if (p->o_x < q->o_x)
		{ x0 = p->o_x;	x1 = q->o_x; }
	else
		{ x0 = q->o_x;	x1 = p->o_x; }
	if (p->o_y < q->o_y)
		{ y0 = p->o_y;	y1 = q->o_y; }
	else
		{ y0 = q->o_y;	y1 = p->o_y; }

	for (r = objhead->o_next; r!= objtail; r = r->o_next)
		if (r->o_x >= x0 && r->o_x <= x1
		&&  r->o_y >= y0 && r->o_y <= y1)
			r = print_obj(n, r);
}

obj	*print_obj(layer, p)
	int	layer;
	obj	*p;
{
	double	r, dx, dy, ox, oy, x0, y0, x1, y1;
	obj	*q;
	int	n;

	if (redo_gbox) {
		float	bnd[4];
		Gbox[2] = Gbox[3] = -(Gbox[0] = Gbox[1] = 32767);
		for (q = objhead->o_next; q != objtail; q = q->o_next) {
			get_bounds(q, bnd, 1);
			track_bounds (bnd[0], bnd[1], bnd[2], bnd[3]);
			if (q->o_type == BLOCK)
				q = q->o_val[N_VAL].o;
		}
		redo_gbox = 0;
	}
	openpl("");
	if (p->o_type != BLOCK && p->o_layer != layer)
		return p;
	if (p->o_type <= TEXT && (p->o_mxx!=1 || p->o_myy!=1 || p->o_mxy!=0 ||
				  p->o_myx!=0 || p->o_mxt!=0 || p->o_myt!=0))
		return print_xform(layer,p);

	ox = p->o_x;
	oy = p->o_y;
	if (p->o_type < TEXT && (p->o_attr & (FILLED | EDGED)))
		chk_attrs (p);
	switch (p->o_type) {

	case TROFF:
		n = p->o_nt1;
		if (text[n].t_type & EQNTXT)
			puteqn(ox, oy, text[n].t_type, atoi(text[n].t_val));
		else
			troff(text[n].t_val);
		return p;
	case BLOCK:
		for (q = p->o_next; q != p->o_val[N_VAL].o; q = q->o_next)
			if (q->o_type <= TEXT || q->o_nt2 > q->o_nt1)
				q = print_obj(layer,q);
		p = q;
		break;
	case PSFILE:
		puteps(p);
		/* CAREFUL!! THIS FLOWS THROUGH INTO BOX!! */
	case BOX:
		if (p->o_attr & (FILLED|EDGED)) {
			x0 = ox - p->o_wid/2;	x1 = ox + p->o_wid/2;
			y0 = oy - p->o_ht/2;	y1 = oy + p->o_ht/2;
			r  = p->o_val[N_VAL].f;
			box(x0, y0, x1, y1, r);
		}
		break;
	case CIRCLE:
	case ELLIPSE:
		if (p->o_attr & (FILLED|EDGED))
			ellipse(ox,oy,p->o_wid/2,0.,0.,p->o_ht/2,0.,2*M_PI,0);
		break;
	case SECTOR:
	case ARC:
		if (p->o_attr & (FILLED|EDGED)) {
			register double ang1, ang2;

			if (p->o_attr & HEAD12) {
				print_xform(layer, p);
				break;
			}
			r  = p->o_val[N_VAL+0].f;
			x0 = p->o_val[N_VAL+2].f;	/* starting point */
			y0 = p->o_val[N_VAL+3].f;
			x1 = p->o_val[N_VAL+4].f;	/* ending point   */
			y1 = p->o_val[N_VAL+5].f;
			ang1 = atan2(y0-oy, x0-ox);
			ang2 = atan2(y1-oy, x1-ox);
			ellipse(ox, oy, r, 0., 0., r, ang1, ang2, p->o_type);
		}
		break;
	case LINE:
	case ARROW:
	case SPLINE:
		if (p->o_attr & (FILLED|EDGED)) {
			int	c, nxy;

			if (p->o_attr & HEAD12) {
				print_xform(layer, p);
				break;
			}
			r   = p->o_val[N_VAL+0].f;
			nxy = p->o_val[N_VAL+3].f;	    /* segment count */
			x0  = p->o_val[N_VAL+4].f;	    /* first point   */
			y0  = p->o_val[N_VAL+5].f;
			x1  = p->o_val[N_VAL+4+2*nxy].f;    /* last point    */
			y1  = p->o_val[N_VAL+5+2*nxy].f;
			c = (x0 == x1 && y0 == y1);	    /* flags closure */
			if (nxy == 1)
				line(x0, y0, x1, y1);
			else if (p->o_type == SPLINE)
				spline(nxy, c, &p->o_val[N_VAL+4]);
			else
				pline (nxy, c, &p->o_val[N_VAL+4], r);
		}
		break;
	}
	if (p->o_nt1 < p->o_nt2)
		objtext(ox, oy, p);
	return p;
}

obj	*print_xform (layer,p)
	int	layer;
	obj	*p;
{
	double	r, ox, oy, dx, dy, x0, y0, x1, y1, M[4];
	double	b, tx0, ty0, tx1, ty1;
	valtype	*X;
	obj	*q;
	int	n;

	if (p->o_type == BLOCK) {
		for (q = p->o_next; q != p->o_val[N_VAL].o; q = q->o_next)
			if (q->o_type <= TEXT || q->o_nt2 > q->o_nt1)
				q = print_xform(layer,q);
		p = q;
	}
	if (p->o_layer != layer && p->o_type != BLOCK)
		return p;
#if 0
	if (p->o_type == TEXT) {
		ox = Xformx(p->o_parent, 1, p->o_x, p->o_y);
		oy = Xformy(p->o_parent, 0, p->o_x, p->o_y);
	}
	else {
#endif
		ox = Xformx(p, 1, p->o_x, p->o_y);
		oy = Xformy(p, 0, p->o_x, p->o_y);
#if 0
	}
#endif
	X = (valtype *)0;	
	if (p->o_type < TEXT && (p->o_attr & (FILLED | EDGED)))
		chk_attrs (p);

	switch (p->o_type) {

	case TROFF:
		n = p->o_nt1;
		if (text[n].t_type & EQNTXT)
			puteqn(ox, oy, text[n].t_type, atoi(text[n].t_val));
		else
			troff(text[n].t_val);
		return p;
	case PSFILE:
		puteps(p);
		/* CAREFUL!! THIS FLOWS THROUGH INTO BOX!! */
	case BOX:
		if (p->o_attr & (FILLED|EDGED)) {
			if ((X = (valtype *)malloc(10*sizeof(valtype))) == NULL)
				yyerror("out of room in print_xform");
			r = p->o_val[N_VAL].f;
			X[6].f = X[0].f = -(X[2].f = X[4].f = p->o_wid/2);
			X[1].f = X[3].f = -(X[5].f = X[7].f = p->o_ht/2);
			for (n = 0; n < 8; n += 2) {
				x1	 = ox + Linx(p, 0, X[n].f, X[n+1].f);
		  		X[n+1].f = oy + Liny(p, 0, X[n].f, X[n+1].f);
				X[n].f	 = x1;
			}
			X[n] = X[0];	X[++n] = X[1];
			x1 = Linx(p, 0, r, 0.);
			r =  Liny(p, 0, r, 0.);
			r = sqrt(x1 * x1 + r * r);
			pline(4, 1, X, r);
		}
		break;
	case CIRCLE:
	case ELLIPSE:
		if (p->o_attr & (FILLED|EDGED)) {
			get_matrix(&x0, &y0, &x1, &y1);
			x0 *= p->o_wid/2;
			y0 *= p->o_wid/2;
			x1 *= p->o_ht/2;
			y1 *= p->o_ht/2;
			ellipse(ox, oy, x0, y0, x1, y1, 0., 2*M_PI, 0);
		}
		break;
	case SECTOR:
	case ARC:
		if (p->o_attr & (FILLED|EDGED)) {
		    register double ang1, ang2;
		    double  xa, ya;
		    double  dx0, dy0, dx1, dy1;

		    r  = p->o_val[N_VAL+0].f;
		    x0 = p->o_val[N_VAL+2].f;	/* starting point */
		    y0 = p->o_val[N_VAL+3].f;
		    x1 = p->o_val[N_VAL+4].f;	/* ending point   */
		    y1 = p->o_val[N_VAL+5].f;
		    get_matrix(M, M+1, M+2, M+3);
	/*
	 *	If there are arrowheads and non-zero line thickness, we
	 *	should adjust the end points so as to get nice arrowheads.
	 */
		    if (p->o_attr & HEAD12) {
                    	dx0 = Xformx(p, 1, x0, y0);	/* Remember */
		    	dy0 = Xformy(p, 0, x0, y0);	/* directions for */
		    	dx1 = Xformx(p, 0, x1, y1);	/* arrowheads */
		    	dy1 = Xformy(p, 0, x1, y1);
		    /* in here we need to change the points used to compute
		       the angles. */
			dx = p->o_val[N_VAL+8].f;
			dy = p->o_val[N_VAL+9].f;
			if (p->o_weight > 0.) {
			    double	linex, liney, a;

			    b = p->o_weight / 2 / p->o_val[N_VAL+8].f;
			    b *= sqrt(dx*dx + 4*dy*dy);
			    if (p->o_attr & HEAD2) {
				linex = - (y1 - p->o_y);
				liney = (x1 - p->o_x);
				a = b / sqrt(linex*linex + liney*liney);
				x1 -= a * linex;
				y1 -= a * liney;
			    }
			    if (p->o_attr & HEAD1) {
				linex = (y0 - p->o_y);
				liney = - (x0 - p->o_x);
				a = b / sqrt(linex*linex + liney*liney);
				x0 -= a * linex;
				y0 -= a * liney;
			    }
			}
		    }
                    tx0 = Xformx(p, 1, x0, y0); /* transformed starting point */
		    ty0 = Xformy(p, 0, x0, y0);
		    tx1 = Xformx(p, 0, x1, y1);   /* transformed ending point */
		    ty1 = Xformy(p, 0, x1, y1);
		    ang1 = atan2(ty0 - oy, tx0 - ox);
		    ang2 = atan2(ty1 - oy, tx1 - ox);

		    dx = M[0] * M[3] - M[1] * M[2];
		    if (dx < 0) {			/* reflection */
			xa = ang1;
			ang1 = ang2;
			ang2 = xa;
			M[0] = -M[0];
			M[2] = -M[2];
		    }
		    ellipse(ox, oy, r*M[0], r*M[1], r*M[2], r*M[3],
					ang1, ang2, p->o_type);

		    if (p->o_attr & (HEAD1 | HEAD2)) {
			ang1 = 0;
			if (dx < 0) {
			    ang1 = M_PI;
			    dx = -dx;
			}
			dy = dx = sqrt(dx);
			dx *= p->o_val[N_VAL+8].f;
			dy *= p->o_val[N_VAL+9].f;
			if (p->o_attr & HEAD1)
			    arrow(tx0-(dy0-oy), ty0+(dx0-ox), tx0, ty0,
					dx, dy, dy/r/2 + ang1, p->o_attr);
			if (p->o_attr & HEAD2)
			    arrow(tx1+(dy1-oy), ty1-(dx1-ox), tx1, ty1,
					dx, dy, -dy/r/2 + ang1, p->o_attr);
		    }
		}
		break;
	case LINE:
	case ARROW:
	case SPLINE:
		if (p->o_attr & (FILLED|EDGED)) {
			int	c, i, nxy;

			r   = p->o_val[N_VAL+0].f;
			x1 = Linx(p, 0, r, 0.);
			r =  Liny(p, 0, r, 0.);
			r = sqrt(x1 * x1 + r * r);
			nxy = p->o_val[N_VAL+3].f;	/* segment count */
			X = (valtype *)malloc((2*nxy+2)*sizeof(valtype));
			if (X == NULL)
				yyerror("out of room in print_xform");
			for (i = 0, n = N_VAL+4; i <= 2 * nxy; ) {
				register double	xx = p->o_val[n++].f,
						yy = p->o_val[n++].f;
				X[i++].f = Xformx(p, 0, xx, yy);
				X[i++].f = Xformy(p, 0, xx, yy);
			}
			x0 = X[0].f;
			y0 = X[1].f;
			x1 = X[i-2].f;
			y1 = X[i-1].f;
			tx0 = ty0 = tx1 = ty1 = 0.;
			n = 2 * nxy - 2;
			if (p->o_attr & HEAD12) {
			    get_matrix(M, M+1, M+2, M+3);
		/*  DBK: I added the fabs to eliminate sqrt errors
		 *  and make it same as xprint.c.  9/20/90	*/
			    dy = dx = sqrt(fabs(M[0]*M[3] - M[1]*M[2]));
			    dx *= p->o_val[N_VAL+1].f;
			    dy *= p->o_val[N_VAL+2].f;
			    if (p->o_weight > 0.) {
				double	linex, liney, a;

				b = p->o_weight / 2 / p->o_val[N_VAL+1].f;
				b *= sqrt(dx*dx + 4*dy*dy);
				if (p->o_attr & HEAD2) {
				    linex = x1 - X[n].f;
				    liney = y1 - X[n+1].f;
				    a = b / sqrt(linex*linex + liney*liney);
				    tx1 = -a * linex;
				    ty1 = -a * liney;
				    x1 = X[n+2].f += tx1;
				    y1 = X[n+3].f += ty1;
				}
				if (p->o_attr & HEAD1) {
				    linex = x0 - X[2].f;
				    liney = y0 - X[3].f;
				    a = b / sqrt(linex*linex + liney*liney);
				    tx0 = -a * linex;
				    ty0 = -a * liney;
				    x0 = X[0].f += tx0;
				    y0 = X[1].f += ty0;
				}
			    }
			}
                /*      The first two args are just to give a direction to
                 *      the second two.  To make sure that the direction
                 *      isn't reversed, adjust the first two by the same
                 *      amount as we adjusted x1 and y1 above.
                 */
			c = (x0 == x1 && y0 == y1);	    /* flags closure */
			if (nxy == 1)
				line(X[0].f, X[1].f, X[2].f, X[3].f);
			else if (p->o_type == SPLINE)
				spline(nxy, c, X);
			else
				pline (nxy, c, X, r);
                        if (p->o_attr & HEAD2) {
                            tx1 += X[n].f;
                            ty1 += X[n+1].f;
                            arrow(tx1, ty1, x1, y1, dx, dy, 0.0, p->o_attr);
                        }
                        if (p->o_attr & HEAD1) {
                            tx0 += X[2].f;
                            ty0 += X[3].f;
                            arrow(tx0, ty0, x0, y0, dx, dy, 0.0, p->o_attr);
                        }
		}
		break;
	case TEXT:
		tmp_xform(p);
#if 0
		dotext(ox, oy, p);
#else
		dotext(p->o_x, p->o_y, p);
#endif
		undo_tmpx();
		return p;
	}
	if (X)
		free(X);
	if (p->o_nt1 < p->o_nt2)
		objtext(ox, oy, p);
	return p;
}

objtext(x, y, p)
	double x, y;
	obj    *p;
{
	double  *bnd;
	int     i, t;

	t = text[p->o_nt1].t_type;
	for (i = p->o_nt1; i < p->o_nt2; i++)
		if (text[i].t_type != t)
			break;
	if (i >= p->o_nt2 && p->o_type != TEXT) {  /* all strings same type */
		bnd = text_bounds(p);
		if (t & RJUST)
			x += (bnd[2] - bnd[0]) / 2;
		if (t & LJUST)
			x -= (bnd[2] - bnd[0]) / 2;
	}
	dotext(x, y, p);
}

dotext(x, y, p)	/* print text strings of p in proper vertical spacing   */
	double x, y;
	obj *p;
{
	double	h, v, w, dely;
	int	i, j, n, t;

	i = p->o_nt1;
	j = text[i].t_line;			/* sequence # of first line   */
	n = text[p->o_nt2-1].t_line - j;	/* total number of lines (-1) */
	v = abs(text[i].t_size) / (2 * pgscale);
	dely = abs(text[i].t_space) / (2 * pgscale);
	v = n * dely - v/2;
	new_color(p->o_text);
	for (h = w = 0; i < p->o_nt2; ++j, v -= 2*dely, h = w = 0) {
		t = text[i].t_type;
		if (t & ABOVE)
			v += dely;
		else if (t & BELOW)
			v -= dely;
		for (n = i; text[n].t_line == j; n++)
			w += text[n].t_width*72/pgscale;
		if (t & RJUST)
			h = w;
		else if (t & CENTER)
			h = w/2;
		newlabel(t, text[i].t_val, abs(text[i].t_font), x-h, y+v,
						text[i].t_size / pgscale,
						text[i].t_width * 72 / pgscale);
		while (++i < n)
			addlabel(text[i].t_type, text[i].t_val,
				 text[i].t_font, text[i].t_size / pgscale,
					    text[i].t_width * 72 / pgscale);
	}
}
