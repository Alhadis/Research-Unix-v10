/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	@(#)picasso:movegen.c	1.0	*/
#include	"picasso.h"
#include	"y.tab.h"

obj *movegen()
{
static	double prevdx, prevdy;
static	int	xtab[] = { 1, 0, -1, 0 };	/* R=0, U=1, L=2, D=3 */
static	int	ytab[] = { 0, 1, 0, -1 };

struct	objattr	obat;
	double	defx, defy, dx, dy, tcolor;
	int	i, some;
	obj	*p;
	obj	*ppos;
	Attr	*ap;

	defx = getfval("movewid");
	defy = getfval("moveht");
	tcolor = -1;
	set_text();
	dx = dy = some = 0;
	for (i = 0; i < nattr; i++) {
		ap = &attr[i];
		switch (ap->a_type) {
		case FONT:
		case SIZE:
		case SPACE:
		case TEXTATTR:
			miscattrs(ap, &obat);	/* obat not really used here */ 
			break;
		case TCOLOR:
			tcolor = ap->a_val.f;
			break;
		case SAME:
			dx = prevdx;
			dy = prevdy;
			some++;
			break;
		case LEFT:
			dx -= (ap->a_sub==DEFAULT) ? defx : ap->a_val.f;
			some++;
			hvmode = L_DIR;
			break;
		case RIGHT:
			dx += (ap->a_sub==DEFAULT) ? defx : ap->a_val.f;
			some++;
			hvmode = R_DIR;
			break;
		case UP:
			dy += (ap->a_sub==DEFAULT) ? defy : ap->a_val.f;
			some++;
			hvmode = U_DIR;
			break;
		case DOWN:
			dy -= (ap->a_sub==DEFAULT) ? defy : ap->a_val.f;
			some++;
			hvmode = D_DIR;
			break;
		case TO:
			ppos = ap->a_val.o;
			dx = Xformx(ppos, 1, ppos->o_x, ppos->o_y) - curx;
			dy = Xformy(ppos, 0, ppos->o_x, ppos->o_y) - cury;
			some++;
			break;
		case BY:
			ppos = ap->a_val.o;
			dx = Xformx(ppos, 1, ppos->o_x, ppos->o_y);
			dy = Xformy(ppos, 0, ppos->o_x, ppos->o_y);
			some++;
			break;
		case FROM:
		case AT:
			ppos = ap->a_val.o;
			curx = Xformx(ppos, 1, ppos->o_x, ppos->o_y);
			cury = Xformy(ppos, 0, ppos->o_x, ppos->o_y);
			break;
		}
	}
	if (some) {
		defx = dx;
		defy = dy;
	} else {
		defx *= xtab[hvmode];
		defy *= ytab[hvmode];
	}
	prevdx = defx;
	prevdy = defy;
	extreme(curx, cury, Gbox);
	curx += defx;
	cury += defy;
	extreme(curx, cury, Gbox);
	p = makenode(MOVE, 0, 0);
	if (p->o_nt2 > p->o_nt1) {
		p->o_text = tcolor;
		checktextcolor(p);
		text_bounds(p);
	}
	return(p);
}
