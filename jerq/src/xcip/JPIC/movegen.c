#include	<stdio.h>
#include	"pic.h"
#include	"y.tab.h"

struct obj *movegen(type)
{
	static int prevdx, prevdy;
	int i, dx, dy, some;
	int defx, defy;
	struct obj *p;
	struct obj *ppos;
	static int xtab[] = { 1, 0, -1, 0 };	/* R=0, U=1, L=2, D=3 */
	static int ytab[] = { 0, 1, 0, -1 };

	defx = getvar("movewid");
	defy = getvar("moveht");
	dx = dy = some = 0;
	for (i = 0; i < nattr; i++) {
		switch (attr[i].a_type) {
		case LJUST: case RJUST: case CENTER: case SPREAD: case FILL: case ABOVE: case BELOW:
			savetext(attr[i].a_type, attr[i].a_val);
			break;
		case SAME:
			dx = prevdx;
			dy = prevdy;
			some++;
			break;
		case LEFT:
			dx -= (attr[i].a_val==0) ? defx : attr[i].a_val;
			some++;
			hvmode = L_DIR;
			break;
		case RIGHT:
			dx += (attr[i].a_val==0) ? defx : attr[i].a_val;
			some++;
			hvmode = R_DIR;
			break;
		case UP:
			dy += (attr[i].a_val==0) ? defy : attr[i].a_val;
			some++;
			hvmode = U_DIR;
			break;
		case DOWN:
			dy -= (attr[i].a_val==0) ? defy : attr[i].a_val;
			some++;
			hvmode = D_DIR;
			break;
		case TO:
			ppos = (struct obj *) attr[i].a_val;
			dx = ppos->o_x - curx;
			dy = ppos->o_y - cury;
			some++;
			break;
		case BY:
			ppos = (struct obj *) attr[i].a_val;
			dx = ppos->o_x;
			dy = ppos->o_y;
			some++;
			break;
		case FROM:
		case AT:
			ppos = (struct obj *) attr[i].a_val;
			curx = ppos->o_x;
			cury = ppos->o_y;
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
	extreme(curx, cury);
	curx += defx;
	cury += defy;
	extreme(curx, cury);
	p = makenode(MOVE, 0);
	dprintf("M %d %d\n", curx, cury);
	return(p);
}
