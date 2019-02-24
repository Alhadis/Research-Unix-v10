#include	<stdio.h>
#include	"pic.h"
#include	"y.tab.h"

struct obj *textgen(s, garb)
{
	int i, dx, dy, type;
	struct obj *p, *ppos;

	type = 'C';
	dx = dy = 0;
	for (i = 0; i < nattr; i++)
		switch (attr[i].a_type) {
		case LEFT:
			dx -= attr[i].a_val;
			break;
		case RIGHT:
			dx += attr[i].a_val;
			break;
		case UP:
			dy += attr[i].a_val;
			break;
		case DOWN:
			dy -= attr[i].a_val;
			break;
		case AT:
			ppos = (struct obj *) attr[i].a_val;
			curx = ppos->o_x;
			cury = ppos->o_y;
			break;
		case LJUST:
			type = 'L';
			break;
		case RJUST:
			type = 'R';
			break;
		case SPREAD:
			type = 'S';
			break;
		case FILL:
			type = 'F';
			break;
		case ABOVE:
			type = 'A';
			break;
		case BELOW:
			type = 'B';
			break;
		}
	dprintf("T %c %s\n", type, s);
	extreme(curx, cury);
	curx += dx;
	cury += dy;
	extreme(curx, cury);
	p = makenode(TEXT, 2);
	p->o_val[0] = s;
	p->o_val[1] = type;
	return(p);
}
