#include "defs.h"

vc(p1)  /* create vertex */
Point p1; {
	
	disc(&display, p1, dsize, f2);
	putptinlist(p1);
}


vd(i)  /* delete vetex */
int i; {
	delptinlist(i, list[i].next);
}


vm(i, destpt)  /* move vertex */
int i;
Point destpt; {
	NODE *nodeptr;

	if (list[i].set == -1) {
		/* erase old */
		nodeptr = list[i].next;
		while (nodeptr != 0) {
			Segment(i, nodeptr);
			nodeptr = nodeptr->next;
		}
		Disc(i);  /* erase */
		list[i].pt = destpt;
		/* put  new */
		nodeptr = list[i].next;
		while (nodeptr != 0) {
			Segment(i, nodeptr);
			nodeptr = nodeptr->next;
		}
		Disc(i);  /* put*/
	}	
/*	else  list[i].set == -1 ; vertex in a set, move set */
	/*	sm(list[i].set, p1);
	 better check set status first ...*/
}

vp(ind, pt)  /* copy vertex */
int ind;
Point pt; {
	int newind;
	NODE *nodeptr;

	if (list[ind].set != -1)
		/* copy set */
		sc(list[ind].set, pt); 
	else {    /* copy vertex */


		/* create new vertex */
		newind = last;
		list[last].status = 1;

		list[last].pt = pt; 
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
			/* show new edge */
			Segment(newind, list[newind].next);
			nodeptr = nodeptr->next;
		}	

	}
}


vg(ind1, code)  /* vertex graphics */
int ind1;
char code[]; {
	MEMBER *mbrptr;

	if (list[ind1].set == -1) {
	/* not in set; do for single vert */
		vertcolor(ind1);
		changevertcolor(ind1, code);
	}
	else {  /* apply to set */
		mbrptr = set[list[ind1].set].next;
		while (mbrptr != 0) {
	
			vertcolor(mbrptr->ind);
			changevertcolor(mbrptr->ind, code);
			mbrptr = mbrptr->next;
		}
	}
}


vl(ind1, label)  /* label vertex */ 
int ind1;
char label[]; {

	if ( showingvertlabs == 1 )
		showcurrentlabel(list[ind1].wt, vertlabpt(list[ind1].pt));
		/* erase old label */
	strcpy( list[ind1].wt, label );
	if ( showingvertlabs == 1 )
		showcurrentlabel(label, vertlabpt(list[ind1].pt));	
		/* put new label */
}
