#include "defs.h"

connsets(setnum1, setnum2)
int setnum1, setnum2; {
	int ind, ind1, ind2, edgethere;
	MEMBER *mbr1, *mbr2;
	NODE *nodeptr;

	mbr1 = set[setnum1].next;
	while (mbr1 != 0 ) {
		ind1 = mbr1->ind;
		mbr2 = set[setnum2].next;
		while (mbr2 != 0) {
			ind2 = mbr2->ind;
			/* in case set1 == set2 */
			if (ind1 == ind2) {
				mbr2 = mbr2->next;
				continue;
			}
			/* see if edge already there */
			edgethere = 0;
			nodeptr = list[ind1].next;
			while (nodeptr != 0 && !edgethere) {
				if (nodeptr->ind != ind2)
					nodeptr = nodeptr->next;
				else
					edgethere = 1;
			}

			if (edgethere) {
				mbr2 = mbr2->next;
				continue;
			}

			putedge(ind1, ind2, "L", SEG);
			Segment(ind1, list[ind1].next);

			mbr2 = mbr2->next;
		}
		mbr1 = mbr1->next;	
	}
	return;
}


disconnsets(setnum1, setnum2)
int setnum1, setnum2; {
	int ind, ind1, ind2, edgethere;
	MEMBER *mbr1, *mbr2;
	NODE *nodeptr;


	mbr1 = set[setnum1].next;
	while (mbr1 != 0 ) {
		ind1 = mbr1->ind;
		mbr2 = set[setnum2].next;
		while (mbr2 != 0) {
			ind2 = mbr2->ind;
			/* in case set1 == set2 */
			if (ind1 == ind2) {
				mbr2 = mbr2->next;
				continue;
			}
			/* see if edge already there */
			edgethere = 0;
			nodeptr = list[ind1].next;
			while (nodeptr != 0 && !edgethere) {
				if (nodeptr->ind != ind2)
					nodeptr = nodeptr->next;
				else
					edgethere = 1;
			}

			if (edgethere == 0) {
				mbr2 = mbr2->next;
				continue;
			}
			Segment(ind1, nodeptr);
			deledge(ind1, ind2);

			mbr2 = mbr2->next;
		}
		mbr1 = mbr1->next;	
	}
}


compsubg() {
	int i=0, ind1, ind2, edgethere;
	Point p1;
	MEMBER *mbrptr1, *mbrptr2;
	NODE *nodeptr;

	for(;; wait(MOUSE) ) {
		if (button1()) {
			while ( button1() ) ;

			if (ptinrect(mouse.xy,  box)) {
				cmds();
				return;
			}

			for (i = 1; i < lastset; i++)
				if (ptinrect(mouse.xy, set[i].Srect) && set[i].status)
					break;
			/* connect set i */
			mbrptr1 = set[i].next;
			while (mbrptr1 != 0 ) {
				ind1 = mbrptr1->ind;
				mbrptr2 = set[i].next;
				while (mbrptr2 != 0 ) {
					ind2 = mbrptr2->ind;

					if (ind1 == ind2) {
						mbrptr2 = mbrptr2->next;
						continue;
					}

					/* see if edge already there */
					edgethere = 0;
					nodeptr = list[ind1].next;
					while (nodeptr != 0 && !edgethere) {
						if (nodeptr->ind != ind2)
							nodeptr = nodeptr->next;
						else
							edgethere = 1;
					}

					if (edgethere) {
						mbrptr2 = mbrptr2->next;
						continue;
					}

					putedge(ind1, ind2, "L", SEG);
					Segment(ind1, list[ind1].next);

					mbrptr2 = mbrptr2->next;

				}
				mbrptr1 = mbrptr1->next;	
	
			}
		}
		
		if (button3())
			return;
	}
}


disconnsubg() {
	int i=0, ind1, ind2, edgethere;
	Point p1;
	MEMBER *mbrptr1, *mbrptr2;
	NODE *nodeptr;

	for(;; wait(MOUSE) ) {
		if (button1()) {
			while ( button1() ) ;

			if (ptinrect(mouse.xy,  box)) {
				cmds();
				return;
			}

			for (i = 1; i < lastset; i++)
				if (ptinrect(mouse.xy, set[i].Srect) && set[i].status)
					break;
			/* connect set i */
			mbrptr1 = set[i].next;
			while (mbrptr1 != 0 ) {
				ind1 = mbrptr1->ind;
				mbrptr2 = set[i].next;
				while (mbrptr2 != 0 ) {
					ind2 = mbrptr2->ind;

					/* see if edge already there */
					edgethere = 0;
					nodeptr = list[ind1].next;
					while (nodeptr != 0 && !edgethere) {
						if (nodeptr->ind != ind2)
							nodeptr = nodeptr->next;
						else
							edgethere = 1;
					}

					if (!edgethere) {
						mbrptr2 = mbrptr2->next;
						continue;
					}

					Segment(ind1, nodeptr);
					deledge(ind1, ind2);

					mbrptr2 = mbrptr2->next;

				}
				mbrptr1 = mbrptr1->next;	
	
			}
		}
		
		if (button3())
			return;
	}
}
