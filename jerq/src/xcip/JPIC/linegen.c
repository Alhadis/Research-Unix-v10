#include	<stdio.h>
#include	"pic.h"
#include	"y.tab.h"

struct obj *linegen(type)
{
	static int prevdx = HT;
	static int prevdy = 0;
	static int prevw = HT/10;
	static int prevh = HT/5;
	int i, j, some, head, ddtype, ddval, invis;
	int chop, chop1, chop2, x0, y0, x1, y1;
	double sin(), cos(), atan2(), theta;
	int defx, defy;
	struct obj *p, *ppos;
	static int xtab[] = { 1, 0, -1, 0 };	/* R=0, U=1, L=2, D=3 */
	static int ytab[] = { 0, 1, 0, -1 };
	int ndxy, dx[50], dy[50];
	coord nx, ny;

	nx = curx;
	ny = cury;
	defx = getvar("linewid");
	defy = getvar("lineht");
	prevh = getvar("arrowht");
	prevw = getvar("arrowwid");
	dx[0] = dy[0] = ndxy = some = head = invis = 0;
	chop = chop1 = chop2 = 0;
	ddtype = ddval = 0;
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
		case CHOP:
			if (chop++ == 0)
				chop1 = chop2 = attr[i].a_val;
			else
				chop2 = attr[i].a_val;
			break;
		case DOT:
		case DASH:
			ddtype = attr[i].a_type;
			ddval = attr[i].a_val;
			if (ddval == 0)
				ddval = getvar("dashwid");
			break;
		case SAME:
			dx[ndxy] = prevdx;
			dy[ndxy] = prevdy;
			some++;
			break;
		case LEFT:
			dx[ndxy] -= (attr[i].a_val==0) ? defx : attr[i].a_val;
			some++;
			hvmode = L_DIR;
			break;
		case RIGHT:
			dx[ndxy] += (attr[i].a_val==0) ? defx : attr[i].a_val;
			some++;
			hvmode = R_DIR;
			break;
		case UP:
			dy[ndxy] += (attr[i].a_val==0) ? defy : attr[i].a_val;
			some++;
			hvmode = U_DIR;
			break;
		case DOWN:
			dy[ndxy] -= (attr[i].a_val==0) ? defy : attr[i].a_val;
			some++;
			hvmode = D_DIR;
			break;
		case HEIGHT:	/* length of arrowhead */
			prevh = attr[i].a_val;
			break;
		case WIDTH:	/* width of arrowhead */
			prevw = attr[i].a_val;
			break;
		case TO:
			if (some) {
				nx += dx[ndxy];
				ny += dy[ndxy];
				ndxy++;
				dx[ndxy] = dy[ndxy] = some = 0;
			}
			ppos = (struct obj *) attr[i].a_val;
			dx[ndxy] = ppos->o_x - nx;
			dy[ndxy] = ppos->o_y - ny;
			some++;
			break;
		case BY:
			ppos = (struct obj *) attr[i].a_val;
			dx[ndxy] = ppos->o_x;
			dy[ndxy] = ppos->o_y;
			some++;
			break;
		case THEN:	/* turn off any previous accumulation */
			if (some) {
				nx += dx[ndxy];
				ny += dy[ndxy];
				ndxy++;
				dx[ndxy] = dy[ndxy] = some = 0;
			}
			break;
		case FROM:
		case AT:
			ppos = (struct obj *) attr[i].a_val;
			nx = curx = ppos->o_x;
			ny = cury = ppos->o_y;
			break;
		}
	}
	if (some) {
		nx += dx[ndxy];
		ny += dy[ndxy];
		ndxy++;
		defx = dx[ndxy-1];
		defy = dy[ndxy-1];
	} else {
		defx *= xtab[hvmode];
		defy *= ytab[hvmode];
		dx[ndxy] = defx;
		dy[ndxy] = defy;
		ndxy++;
		nx += defx;
		ny += defy;
	}
	prevdx = defx;
	prevdy = defy;
	if (chop) {
		if (chop == 1 && chop1 == 0)	/* just said "chop", so use default */
			chop1 = chop2 = getvar("circlerad");
		theta = atan2((float) defy, (float) defx);
		x0 = chop1 * cos(theta);
		y0 = chop1 * sin(theta);
		curx += x0;
		cury += y0;
		x1 = chop2 * cos(theta);
		y1 = chop2 * sin(theta);
		nx -= x1;
		ny -= y1;
		dx[0] -= x0;
		dy[0] -= y0;
		dx[ndxy-1] -= x1;
		dy[ndxy-1] -= y1;
		if(dbg)printf("chopping %d %d %d %d; cur=%d,%d end=%d,%d\n",
			x0, y0, x1, y1, curx, cury, nx, ny);
	}
	p = makenode(type, 5 + 2 * ndxy);
	curx = p->o_val[0] = nx;
	cury = p->o_val[1] = ny;
	if (head || type == ARROW) {
		p->o_val[2] = prevw;
		p->o_val[3] = prevh;
		if (head == 0)
			head = HEAD2;	/* default arrow head */
	}
	p->o_attr = head | invis;
	p->o_val[4] = ndxy;
	nx = p->o_x;
	ny = p->o_y;
	for (i = 0, j = 5; i < ndxy; i++, j += 2) {
		p->o_val[j] = dx[i];
		p->o_val[j+1] = dy[i];
		extreme(nx += dx[i], ny += dy[i]);
	}
	p->o_dotdash = ddtype;
	p->o_ddval = ddval;
	if (dbg) {
		printf("S or L from %d %d to %d %d with %d elements:\n", p->o_x, p->o_y, curx, cury, ndxy);
		for (i = 0, j = 5; i < ndxy; i++, j += 2)
			printf("%d %d\n", p->o_val[j], p->o_val[j+1]);
	}
	extreme(p->o_x, p->o_y);
	extreme(curx, cury);
	return(p);
}

struct obj *splinegen(type)
{
	linegen(type);
}
