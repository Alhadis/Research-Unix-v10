#include	<stdio.h>
#include	"pic.h"
#include	"y.tab.h"

struct obj *arcgen(type)	/* handles circular and (eventually) elliptical arcs */
{
	static int prevw = HT/10;
	static int prevh = HT/5;
	static int prevrad = HT/2;
	static int dtox[2][4] ={ 1, -1, -1, 1, 1, 1, -1, -1 };
	static int dtoy[2][4] ={ 1, 1, -1, -1, -1, 1, 1, -1 };
	static int dctrx[2][4] ={ 0, -1, 0, 1, 0, 1, 0, -1 };
	static int dctry[2][4] ={ 1, 0, -1, 0, -1, 0, 1, 0 };
	static int nexthv[2][4] ={ U_DIR, L_DIR, D_DIR, R_DIR, D_DIR, R_DIR, U_DIR, L_DIR };
	double sqrt(), atan2(), sin(), cos();
	float dx2, dy2, ht, phi, r, d;
	int i, head, to, at, cw, invis;
	struct obj *p, *ppos;
	coord fromx, fromy, tox, toy;

	prevrad = getvar("arcrad");
	prevh = getvar("arrowht");
	prevw = getvar("arrowwid");
	fromx = curx;
	fromy = cury;
	head = to = at = cw = invis = 0;
	for (i = 0; i < nattr; i++) {
		switch (attr[i].a_type) {
		case LJUST: case RJUST: case CENTER: case SPREAD: case FILL: case ABOVE: case BELOW:
			savetext(attr[i].a_type, attr[i].a_val);
			break;
		case HEAD:
			head += attr[i].a_val;
			break;
		case INVIS:
			invis = INVIS;
			break;
		case HEIGHT:	/* length of arrowhead */
			prevh = attr[i].a_val;
			break;
		case WIDTH:	/* width of arrowhead */
			prevw = attr[i].a_val;
			break;
		case RADIUS:
			prevrad = attr[i].a_val;
			break;
		case DIAMETER:
			prevrad = attr[i].a_val / 2;
			break;
		case CW:
			cw = 1;
			break;
		case FROM:	/* start point of arc */
			ppos = (struct obj *) attr[i].a_val;
			fromx = ppos->o_x;
			fromy = ppos->o_y;
			break;
		case TO:	/* end point of arc */
			ppos = (struct obj *) attr[i].a_val;
			tox = ppos->o_x;
			toy = ppos->o_y;
			to++;
			break;
		case AT:	/* center of arc */
			ppos = (struct obj *) attr[i].a_val;
			curx = ppos->o_x;
			cury = ppos->o_y;
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
		}
	}
	if (!at && !to) {	/* the defaults are mostly OK */
		curx = fromx + prevrad * dctrx[cw][hvmode];
		cury = fromy + prevrad * dctry[cw][hvmode];
		tox = fromx + prevrad * dtox[cw][hvmode];
		toy = fromy + prevrad * dtoy[cw][hvmode];
		hvmode = nexthv[cw][hvmode];
	}
	else if (!at) {
		dx2 = (float)(tox - fromx) / 2;
		dy2 = (float)(toy - fromy) / 2;
		phi = atan2(dy2, dx2) + (cw ? -PI2 : PI2);
		for (r=prevrad; (d = r*r - (dx2*dx2+dy2*dy2)) <= 0.0; r *= 2)
			;	/* this kludge gets around too-small radii */
		ht = sqrt(d);
		curx = fromx + dx2 + ht * cos(phi) + 0.5;
		cury = fromy + dy2 + ht * sin(phi) + 0.5;
		dprintf("dx2,dy2=%g,%g, phi=%g, r,ht=%g,%g\n",
			dx2, dy2, phi, r, ht);
	}
	else if (at && !to) {	/* do we have all the cases??? */
		tox = fromx + prevrad * dtox[cw][hvmode];
		toy = fromy + prevrad * dtoy[cw][hvmode];
		hvmode = nexthv[cw][hvmode];
	}
	if (cw) {	/* interchange roles of from-to and heads */
		coord temp;
		temp = fromx; fromx = tox; tox = temp;
		temp = fromy; fromy = toy; toy = temp;
		if (head == HEAD1)
			head = HEAD2;
		else if (head == HEAD2)
			head = HEAD1;
	}
	p = makenode(type, 6);
	/* these are wrong in general */
	extreme(fromx, fromy);
	extreme(tox, toy);
	p->o_val[0] = fromx;
	p->o_val[1] = fromy;
	p->o_val[2] = tox;
	p->o_val[3] = toy;
	if (cw) {
		curx = fromx;
		cury = fromy;
	} else {
		curx = tox;
		cury = toy;
	}
	p->o_val[4] = prevw;
	p->o_val[5] = prevh;
	p->o_attr = head | (cw ? CW_ARC : 0) | invis;
	dprintf("arc at %d %d from %d %d to %d %d head %d %d\n",
		p->o_x, p->o_y,
		p->o_val[0], p->o_val[1], p->o_val[2], p->o_val[3], p->o_val[4], p->o_val[5]);
	return(p);
}
