/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	@(#)picasso:boxgen.c	1.0	*/
#include	"picasso.h"
#include	"y.tab.h"

/*	BOX parameters are a radius (default 0) for rounding corners	*/
/*	and the NW corner of the box -- so that the box can be rebuilt	*/
/*	(under transformation, the bounding box by itself may not be	*/
/*	sufficient, though it contains 4 of the 8 coordinate values.)	*/

obj *boxgen()
{
	static	double	prevht	= HT;
	static	double	prevwid	= WID;	/* golden mean, sort of */
	static	double	prevrad	= 0;

struct	objattr	obat;
	double	x0, y0, x1, y1, xwith, ywith;
	int	i, at, with;
	obj	*p, *ppos;
	Attr	*ap;

	obat.a_ht  = getfval("boxht");
	obat.a_wid = getfval("boxwid");
	obat.a_rad = getfval("boxrad");
	obat.a_layer = (int)getfval("curlayer");
	obat.a_flags = EDGED;
	obat.a_weight = obat.a_lcolor = obat.a_pcolor = obat.a_tcolor = -1;
	obat.a_dashpat.a = (float *)0;
	set_text();
	at = with = xwith = ywith = 0;
	for (i = 0; i < nattr; i++) {
		ap = &attr[i];
		switch (ap->a_type) {
		default:
			miscattrs(ap, &obat);
			break;
		case SAME:
			obat.a_ht  = prevht;
			obat.a_wid = prevwid;
			obat.a_rad = prevrad;
			break;
		case WITH:
			with = ap->a_val.i;	/* corner */
			break;
		case AT:
			ppos = ap->a_val.o;
			curx = Xformx(ppos, 1, ppos->o_x, ppos->o_y);
			cury = Xformy(ppos, 0, ppos->o_x, ppos->o_y);
			at++;
			break;
		}
	}
	if (with) {
		switch (with) {
		case NORTH:	ywith = -obat.a_ht/2;			break;
		case SOUTH:	ywith =  obat.a_ht/2;			break;
		case EAST:	xwith = -obat.a_wid/2;			break;
		case WEST:	xwith =  obat.a_wid/2;			break;
		case NE:	xwith = -obat.a_wid/2;
				ywith = -obat.a_ht/2;			break;
		case SE:	xwith = -obat.a_wid/2;
				ywith =  obat.a_ht/2;			break;
		case NW:	xwith =  obat.a_wid/2;
				ywith = -obat.a_ht/2;			break;
		case SW:	xwith =  obat.a_wid/2;
				ywith =  obat.a_ht/2;			break;
		case CENTER:
		case START:
		case END:						break;
		default:	xwith =  -obat.a_wid/2 * xdelta[with%8];
				ywith =  -obat.a_ht/2  * ydelta[with%8];	 break;
		}
		curx += xwith;
		cury += ywith;
	}
	if (!at) {
		if (isright(hvmode))
			curx += obat.a_wid/2;
		else if (isleft(hvmode))
			curx -= obat.a_wid/2;
		else if (isup(hvmode))
			cury += obat.a_ht/2;
		else
			cury -= obat.a_ht/2;
	}
	p = makenode(BOX, N_VAL+1, obat.a_layer);
	p->o_val[N_VAL].f = obat.a_rad;
	p->o_wid = obat.a_wid;
	p->o_ht  = obat.a_ht;
	x0 = curx - obat.a_wid/2;
	y0 = cury - obat.a_ht/2;
	x1 = curx + obat.a_wid/2;
	y1 = cury + obat.a_ht/2;
	primattrs(p,&obat);
	text_bounds(p);
	track_bounds(x0-p->o_weight/2, y0-p->o_weight/2,
		     x1+p->o_weight/2, y1+p->o_weight/2);
	if (isright(hvmode))
		curx = x1;
	else if (isleft(hvmode))
		curx = x0;
	else if (isup(hvmode))
		cury = y1;
	else
		cury = y0;
	prevht  = obat.a_ht;
	prevwid = obat.a_wid;
	prevrad = obat.a_rad;
	return(p);
}
