#include "defs.h"


NODE *findnode(nodeptr, ind)
NODE **nodeptr;
int ind; {
	while ((*nodeptr) != 0) {
		if ((*nodeptr)->ind == ind)
			return(*nodeptr);
		else
			(*nodeptr) = (*nodeptr)->next;
	}
	return(0);	
}


Segment(ind1, ind2ptr)   /* 2nd arg pointS to node which gives info abt edge(type)*/
int ind1;
NODE *ind2ptr; {

	int ind2;
	Point midpt;

	ind2 = ind2ptr->ind;
	switch (ind2ptr->type) {
		case LOOP:
			loop(list[ind1].pt);
			break;

		case ARC:
			if (ind1 < ind2)
				drawarc(list[ind1].pt, list[ind2].pt);
			else
				drawarc(list[ind2].pt, list[ind1].pt);
			/*for consistency*/
			break;

		case SEG:
			segment( &display, list[ind1].pt, list[ind2].pt, f2 );
			point(&display, list[ind1].pt, f2);
			break;
        }	

	if ( showingedgelabs == 1 ) {
		switch (ind2ptr->type) {
			case LOOP:
		   		midpt = looplabpt(list[ind1].pt);
				break;

			case ARC:
				midpt = arclabpt(list[ind1].pt, list[ind2].pt);	
				break;

			case SEG:
		   		midpt = div( add(list[ind1].pt, list[ind2].pt), 2 );
				break;
          	}	
		showcurrentlabel(ind2ptr->wt, midpt);
	}

	if (strcmp(ind2ptr->color, "H") == 0) {
		/* put/erase extra edges */
		switch (ind2ptr->type) {
			case LOOP:
				heavyloop(list[ind1].pt);
				break;

			case ARC:
				heavyarcs(list[ind1].pt, list[ind2].pt);
				break;

			case SEG:
				heavyedges( list[ind1].pt, list[ind2].pt );
				break;

		}
	}
	return;
}




putedge(ind1, ind2, color, type)
int ind1, ind2, type;
char color[]; {

	int eucdist();
	char str[5], inttostr();

	if (eucflag == 1)
		strcpy(str, inttostr(eucdist(list[ind1].pt, list[ind2].pt)));
	else
		sprintf(str, "1");
		/* default wt (label) for edge is "1" */
	putedgeinlist( ind1, str, color, type, &(list[ind2].next) );
	list[ind2].deg++;
	numedges++;
	if (edgesumflag == 1)
		addtoEsum(list[ind2].next->wt);

	if (ind1 == ind2)   /* loop; return */
		return;
	putedgeinlist( ind2, str, color, type, &(list[ind1].next) );
	list[ind1].deg++;
	numedges++;

	return;
}


deledge(ind1, ind2)
int ind1, ind2; {
	NODE *edgeptr;
	
	if (edgesumflag == 1) {
/* if edges being summed, subtract once for both records of edge */
		edgeptr = list[ind1].next;
		while (edgeptr->ind != ind2)
			edgeptr = edgeptr->next;
		subtfromEsum(edgeptr->wt);
	}

	deledgeinlist( ind1, ind2, &(list[ind2].next) );
	if (ind1 == ind2)
		return;
	deledgeinlist( ind2, ind1, &(list[ind1].next) );
	return;
}
