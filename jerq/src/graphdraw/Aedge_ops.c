#include "defs.h"
#include <font.h>

ecd(f, i1, i2)   /*create/delete edge */
Code f;
int i1, i2;  {
	int edgethere, etype;
	NODE *nodeptr;
	MEMBER *mbrptr;
	char tmp[MEDSTR];

	etype = SEG;  /* default */

	/* check to see if 2nd pt is conn to 1st */
	edgethere = 0;
	nodeptr = list[i1].next; 
	while (nodeptr != 0 && !edgethere) {
		if (nodeptr->ind != i2)
			nodeptr = nodeptr->next;	
		else 
			edgethere = 1;
	}

	if (f == f1 && edgethere) { 
		if (!multedflag) {
/*
			sprintf(tmp, "multiple edges not allowed\0");
			rectf(&display, box, f1);
			string(&defont, tmp, &display, boxstart, f2);
*/
			return;
             	}
		else
			etype = ARC;
	}

	if (f == f1 && eqpt(list[i1].pt, list[i2].pt)) {
		if (!loopflag) {
/*
			sprintf(tmp, "loops not allowed");
			rectf(&display, box, f1);
			string(&defont, tmp, &display, boxstart, f2);
*/
			return;
		}
		else
			etype = LOOP;
	}


	if (f == f2 && !edgethere) {
/*
		sprintf(tmp, "no edge between indices %d & %d\0", i1, i2);
		rectf(&display, box, f1);
		string(&defont, tmp, &display, boxstart, f2);
*/
		return;
	}



	 for (;;) { 
		/* so only one case is done... ('break' used) */
		if (list[i1].set == -1 && list[i2].set == -1) {
			if (f == f1) {
				putedge(i1, i2, "L", etype);
				Segment(i1, list[i1].next);
			}
			else {
				nodeptr = list[i1].next; 
				while (nodeptr != 0) {
					if (nodeptr->ind == i2)
						break;
					else
						nodeptr = nodeptr->next;	
				}
				Segment(i1, nodeptr);
				deledge(i1, i2);
			}
			break;
		}

		if (list[i1].set != -1 && list[i2].set != -1 ) {
	/* also check status of set! and make sure 2 diff sets !! */
			if (f == f1) 
				connsets(list[i1].set, list[i2].set);  /* FIX...! */
			else 
				disconnsets(list[i1].set, list[i2].set);
			break;
		}
		

		if (list[i1].set == -1 && list[i2].set != -1) {
			mbrptr = set[list[i2].set].next;
			while (mbrptr != 0) {
				nodeptr = list[i1].next; 
				while (nodeptr != 0) {
					if (nodeptr->ind == mbrptr->ind)
						break;
					else
						nodeptr = nodeptr->next;	
				}
				if (nodeptr == 0 && f == f1) {
				/* edge not already there */
					putedge(i1, mbrptr->ind, "L", SEG);
					Segment(i1, list[i1].next);
				}
				if (nodeptr != 0 && f == f2) {
					Segment(i1, nodeptr);
					deledge(i1, mbrptr->ind);
				}
				mbrptr = mbrptr->next;
			}
			break;
		}

		if (list[i1].set != -1 && list[i2].set == -1) {
			mbrptr = set[list[i1].set].next;
			while (mbrptr != 0) {
				nodeptr = list[i2].next; 
				while (nodeptr != 0) {
					if (nodeptr->ind == mbrptr->ind)
						break;
					else
						nodeptr = nodeptr->next;	
				}
				if (nodeptr == 0 && f == f1) {
				/* edge not already there */
					putedge(i2, mbrptr->ind, "L", SEG);
					Segment(i2, list[i2].next);
				}
				if (nodeptr != 0 && f == f2) {
					Segment(i2, nodeptr);
					deledge(i2, mbrptr->ind);
				}
				mbrptr = mbrptr->next;
			}
			break;
			


		   }

	}
}



eg(i1, i2, code)  /* edge graphics */
int i1, i2;
char code[]; {
	int ok=1, edgethere, ind1, ind2;
	NODE *nodeptr;
	MEMBER *mbrptr, *mbrptr1, *mbrptr2;
	char tmp[MEDSTR];

	if ( (list[i1].next == 0) || (list[i2].next == 0 ) )
		return;

	/* see if 2nd pt is conn to 1st */
	nodeptr = list[i1].next;
	ok = 0;
	while ( nodeptr != 0 && !ok ) {
		if ( nodeptr->ind != i2 )
			nodeptr = nodeptr->next;
		else ok = 1;
	}
		
	if (!ok) {
/*
		sprintf(tmp, "no edge between indices %d & %d\0", i1, i2);
		rectf(&display, box, f1);
		string(&defont, tmp, &display, boxstart, f2);
*/
		return;
	}

	for (;;) {
	/* so only one case is done... ('break' used) */
		if (list[i1].set == -1 && list[i2].set == -1) {
			/* 2 pts were chosen; ready to change code */
			changecolor(i1, i2, code);
			break;
		}

		if (list[i1].set != -1 && list[i2].set != -1 ) {
		/* also check status of set! and make sure 2 diff sets !! */
			mbrptr1 = set[list[i1].set].next;
			while (mbrptr1 != 0 ) {
				ind1 = mbrptr1->ind;
				mbrptr2 = set[list[i2].set].next;
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

					if (list[i1].set == list[i2].set) {
						if (ind2 > ind1)
							changecolor(ind1, ind2, code);
					}
					else
						changecolor(ind1, ind2, code);

					mbrptr2 = mbrptr2->next;
				}

				mbrptr1 = mbrptr1->next;	
			}

			break;
		}

		if (list[i1].set == -1 && list[i2].set != -1) {
			mbrptr = set[list[i2].set].next;
			while (mbrptr != 0) {
				nodeptr = list[i1].next; 
				while (nodeptr != 0) {
					if (nodeptr->ind == mbrptr->ind)
						break;
					else
						nodeptr = nodeptr->next;	
				}
				if (nodeptr != 0)
					changecolor(i1, mbrptr->ind, code);

				mbrptr = mbrptr->next;
			}
			break;
		}

		if (list[i1].set != -1 && list[i2].set == -1) {
			mbrptr = set[list[i1].set].next;
			while (mbrptr != 0) {
				nodeptr = list[i2].next; 
				while (nodeptr != 0) {
					if (nodeptr->ind == mbrptr->ind)
						break;
					else
						nodeptr = nodeptr->next;	
				}
				if (nodeptr != 0)
					changecolor(mbrptr->ind, i2, code);

				mbrptr = mbrptr->next;
			}
			break;
		}
	}

}


el(i1, i2, label)  /* label edge */
int i1, i2;
char label[]; {
	NODE *nodeptr0, *nodeptr1;
	int ok=0;
	Point midpt;
	char tmp[MEDSTR];

	if ( list[i1].next != 0 && list[i2].next != 0) {
		/* see if 2nd pt is conn to 1st */
		nodeptr0 = list[i1].next;
		ok = 0;
		while ( nodeptr0 != 0 && !ok ) {
			if ( nodeptr0->ind != i2 )
				nodeptr0 = nodeptr0->next;
			else ok = 1;
		}
	}

	if (!ok) {
/*
		sprintf(tmp, "no edge between indices %d & %d\0", i1, i2);
		rectf(&display, box, f1);
		string(&defont, tmp, &display, boxstart, f2);
*/
		return;
	}

	if ( showingedgelabs == 1 ) {
		/* erase old label */
		midpt = div(add(list[i1].pt, list[i2].pt), 2);
		showcurrentlabel(nodeptr0->wt, midpt);	
	}
	nodeptr1 = list[i2].next;
	while ( nodeptr1->ind != i1 )
		nodeptr1 = nodeptr1->next;

	strcpy( nodeptr0->wt, label );
	strcpy( nodeptr1->wt, label );

	if ( showingedgelabs == 1 ) 
		/* put new label */
		showcurrentlabel(label, midpt);	
}


sc(i,p) /*for sets??*/
int i;
Point p; {
}
