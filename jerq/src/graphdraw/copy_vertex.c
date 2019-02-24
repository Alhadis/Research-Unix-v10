#include "defs.h"


copy() {
	Point p1, p2, ptdist;
	int ind;

	for(;; wait(MOUSE)) {
		if (button1()) {
			if (ptinrect((p1 = mouse.xy), box)) {
				cmds();
				return;
			}
			if (!ptinrect((p1 = closestpt(p1)), Dspace))
				continue;
			if ((ind = findind(p1)) == -1)
				continue;

			if (list[ind].set != -1)
				/* copy set */
				copyset(list[ind].set, p1);
			else
				/* copy vertex */
				copyvert(ind, p1);

		}
		if (button3())
			return;
	}
}


copyvert(ind, p1)
int ind;
Point p1; {
	int newind, d, stopped;
	Point p2, ptdist;
	NODE *nodeptr, *np2;
	char inttostr();

	if (!ptinrect(p2 = mouse.xy, Dspace))
		p1 = p2;
	ptdist = sub(p1, p2);

	/* create new vertex */
	newind = last;
	list[last].status = 1;

	list[last].pt = list[ind].pt;
	sprintf(list[last].wt, "%d\0",last);
	strcpy(list[last].color, list[ind].color);
	list[last].set = -1;
	numverts++;

	/* show new vertex */
	Disc(last);

	last++;

	nodeptr = list[ind].next;
	while (nodeptr != 0) {
		/* create new edge */
		putedge(newind, nodeptr->ind, nodeptr->color, nodeptr->type);
		if (eucflag == 1) {
			d = eucdist(list[newind].pt,list[nodeptr->ind].pt);
			strcpy(nodeptr->wt,inttostr(d));
			addtoEsum(nodeptr->wt);
		}
		/* show new edge */
		Segment(newind, list[newind].next);
		nodeptr = nodeptr->next;
	}	

	move_vertex_now(newind);
}
