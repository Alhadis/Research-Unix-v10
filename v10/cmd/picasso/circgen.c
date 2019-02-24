/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	@(#)picasso:circgen.c	1.0	*/
#include	"picasso.h"
#include	"y.tab.h"

extern	int	pic_compat;

obj *circgen(type)
	int	type;
{
static	double	rad[2]	= { HT2, WID2 };
static	double	rad2[2]	= { HT2, HT2 };

struct	objattr	obat;
	double	xwith, ywith, r, r2;
	int	i, at, t, with;
	obj	*p, *ppos;
	Attr	*ap;

	obat.a_layer = (int)getfval("curlayer");
	obat.a_flags = EDGED;
	obat.a_weight = obat.a_lcolor = obat.a_pcolor = obat.a_tcolor = -1;
	obat.a_dashpat.a = (float *)0;
	at = with = xwith = ywith = 0;
	if (t = (type == CIRCLE) ? 0 : 1) {
		r = getfval("ellipsewid") / 2;
		r2 = getfval("ellipseht") / 2;
	}
	else
		r = r2 = getfval("circlerad");
	set_text();
	for (i = 0; i < nattr; i++) {
		ap = &attr[i];
		switch (ap->a_type) {
		default:
			miscattrs(ap, &obat);
			break;
		case RADIUS:
			r = ap->a_val.f;
			break;
		case DIAMETER:
		case WIDTH:
			r = ap->a_val.f / 2;
			break;
		case HEIGHT:
			r2 = ap->a_val.f / 2;
			break;
		case SAME:
			r = rad[t];
			r2 = rad2[t];
			break;
		case WITH:
			with = ap->a_val.i;
			break;
		case AT:
			ppos = ap->a_val.o;
			curx = Xformx(ppos, 1, ppos->o_x, ppos->o_y);
			cury = Xformy(ppos, 0, ppos->o_x, ppos->o_y);
			at++;
			break;
		}
	}
	if (type == CIRCLE)
		r2 = r;	/* probably superfluous */
	if (with) {
		if (pic_compat)		/* map NE to 2nd, etc. */
			with =	with == NE ? 2 :
				with == NW ? 4 :
				with == SW ? 6 :
				with == SE ? 8 : with;
		switch (with) {
		case NORTH:	ywith = -r2;			break;
		case SOUTH:	ywith = r2;			break;
		case EAST:	xwith = -r;			break;
		case WEST:	xwith = r;			break;
		case NE:	xwith = -r;	ywith = -r2;	break;
		case SE:	xwith = -r;	ywith =  r2;	break;
		case NW:	xwith =  r;	ywith = -r2;	break;
		case SW:	xwith =  r;	ywith =  r2;	break;
		case CENTER:
		case START:
		case END:					break;
		default:	xwith =  -r * xdelta[with % 8];
				ywith = -r2 * ydelta[with % 8];
				if (with % 2 == 0) {
					xwith *= M_SQRT1_2;
					ywith *= M_SQRT1_2;
				}
				break;
		}
		curx += xwith;
		cury += ywith;
	}
	if (!at) {
		if (isright(hvmode))
			curx += r;
		else if (isleft(hvmode))
			curx -= r;
		else if (isup(hvmode))
			cury += r2;
		else
			cury -= r2;
	}
	if (r <= 0 || r2 <= 0)
		yyerror("%s has invalid radius %g",
			(type==CIRCLE) ? "circle" : "ellipse", r<r2 ? r : r2);
	p = makenode(type, N_VAL, obat.a_layer);
	p->o_wid = 2 * (rad[t]  = r);
	p->o_ht  = 2 * (rad2[t] = r2);
	primattrs(p, &obat);
	text_bounds(p);
	if (isright(hvmode))
		curx += r;
	else if (isleft(hvmode))
		curx -= r;
	else if (isup(hvmode))
		cury += r2;
	else
		cury -= r2;
	r  += p->o_weight/2;
	r2 += p->o_weight/2;
	track_bounds (p->o_x - r, p->o_y - r2, p->o_x + r, p->o_y + r2);
	return(p);
}
