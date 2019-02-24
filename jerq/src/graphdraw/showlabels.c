#include "defs.h"
extern char *labsmenutext[];

Point vertlabpt(vertpt)         /* for placement of label on screen */
Point vertpt; {

	Point labpt;

	/* see which side of screen pt is on */
	if ( vertpt.x < Dspace.corner.x/2 ) {
		labpt.x = vertpt.x + 10;
		labpt.y = vertpt.y - 5;
	}
	else {
		labpt.x = vertpt.x - 20;
		labpt.y = vertpt.y + 10;
	}
	return(labpt);
}

showvertlabels() {

	int i;

		/* show all vertex labels */
	for ( i = 1; i < last; i++ ) {
		showcurrentlabel( list[i].wt, vertlabpt(list[i].pt) );
	}

	if ( (showingvertlabs == -1) && (numverts > 0) )  
			/* were not on, but are now showing */
		labsmenutext[2] =  "hide vertex labels";
			/* next selection is hide...*/
	else
		labsmenutext[2] = "show vertex labels";

	if (numverts > 0)
		showingvertlabs *= -1;

	return;

}


showedgelabels() {

	int i, j, indx, looplabshown;
	NODE *edge;
	Point midpt;

	for ( i = 1; i < last; i++ ) {
		looplabshown = 0;
		edge = list[i].next;
		while (edge != 0) {
/*    deg. must be wrong... gets mem. fault sometimes 
		for ( j = 0; j < list[i].deg; j++ ) {
*/
			indx = edge->ind;
			if ( indx > i ) {   
			/* wt of seg hasn't yet been shown */
				midpt = div( add(list[i].pt, list[indx].pt), 2 );
				showcurrentlabel( edge->wt, midpt );
			}
			else
				if ((i == indx) && (looplabshown == 0) && (loopflag)) {
					showcurrentlabel( edge->wt, looplabpt(list[i].pt) );
					looplabshown = 1;
				}
			edge = edge->next;
		}
	}

	if ( (showingedgelabs == -1) && (numedges > 0) )   
			/* were not on, but are now showing */
		labsmenutext[3] = "hide edge labels";
	else
		labsmenutext[3] = "show edge labels";

	if (numedges > 0)
		showingedgelabs *= -1;

	return;
}
