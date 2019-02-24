#include "defs.h"

setmbrs(i)
int i; {
	MEMBER *mbrptr;

	mbrptr = set[i].next;
	while (mbrptr != 0) {
		if (list[mbrptr->ind].status == 1) {
			/* erase old/draw new vert */
			Disc(mbrptr->ind);
			setmbrsedges(mbrptr->ind);
		}
		mbrptr = mbrptr->next;
	}
	return;
}


setmbrsedges(mbrind)
int mbrind; {
	NODE *nodeptr;

	nodeptr = list[mbrind].next;
	while (nodeptr != 0) {
		if (list[nodeptr->ind].set != list[mbrind].set || nodeptr->ind >= mbrind)
			/* erase old/draw new edges */
			Segment(mbrind, nodeptr);
		nodeptr = nodeptr->next;
	}
	return;
}
