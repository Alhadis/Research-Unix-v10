#include	<stdio.h>
#include	"pic.h"
#include	"y.tab.h"

struct obj *boxgen(type)
{
	static int prevh = HT;
	static int prevw = WID;	/* golden mean, sort of */
	int i, invis, at, ddtype, ddval;
	int with, xwith, ywith;
	int h, w;
	coord x0, y0, x1, y1;
	struct obj *p, *ppos;

	h = getvar("boxht");
	w = getvar("boxwid");
	invis = at = 0;
	with = xwith = ywith = 0;
	ddtype = ddval = 0;
	for (i = 0; i < nattr; i++) {
		switch (attr[i].a_type) {
		case HEIGHT:
			h = attr[i].a_val;
			break;
		case WIDTH:
			w = attr[i].a_val;
			break;
		case SAME:
			h = prevh;
			w = prevw;
			break;
		case WITH:
			with = attr[i].a_val;	/* corner */
			break;
		case AT:
			ppos = (struct obj *) attr[i].a_val;
			curx = ppos->o_x;
			cury = ppos->o_y;
			at++;
			break;
		case INVIS:
			invis = INVIS;
			break;
		case DOT:
		case DASH:
			ddtype = attr[i].a_type;
			ddval = attr[i].a_val;
			if (ddval == 0)
				ddval = getvar("dashwid");
			break;
		case LJUST: case RJUST: case CENTER: case SPREAD: case FILL: case ABOVE: case BELOW:
			savetext(attr[i].a_type, attr[i].a_val);
			break;
		}
	}
	if (with) {
		switch (with) {
		case NORTH:	ywith = -((h+1) / 2); break;
		case SOUTH:	ywith = h / 2; break;
		case EAST:	xwith = -((w+1) / 2); break;
		case WEST:	xwith = w / 2; break;
		case NE:	xwith = -((w+1) / 2); ywith = -((h+1) / 2); break;
		case SE:	xwith = -((w+1) / 2); ywith = h / 2; break;
		case NW:	xwith = w / 2; ywith = -((h+1) / 2); break;
		case SW:	xwith = w / 2; ywith = h / 2; break;
		}
		curx += xwith;
		cury += ywith;
	}
	if (!at) {
		if (isright(hvmode))
			curx += (w+1) / 2;
		else if (isleft(hvmode))
			curx -= w / 2;
		else if (isup(hvmode))
			cury += (h+1) / 2;
		else
			cury -= h / 2;
	}
	x0 = curx - w / 2;
	y0 = cury - h / 2;
	x1 = curx + (w+1) / 2;
	y1 = cury + (h+1) / 2;
	extreme(x0, y0);
	extreme(x1, y1);
	p = makenode(BOX, 2);
	p->o_val[0] = w;
	p->o_val[1] = h;
	p->o_dotdash = ddtype;
	p->o_ddval = ddval;
	p->o_attr = invis;
	dprintf("B %d %d %d %d at %d %d, h=%d, w=%d\n", x0, y0, x1, y1, curx, cury, h, w);
	if (isright(hvmode))
		curx = x1;
	else if (isleft(hvmode))
		curx = x0;
	else if (isup(hvmode))
		cury = y1;
	else
		cury = y0;
	prevh = h;
	prevw = w;
	return(p);
}
