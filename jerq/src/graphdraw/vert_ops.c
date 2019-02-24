#include "defs.h"

Disc(ind)
int ind; {

	vertcolor(ind);
	if (showingvertlabs == 1)
		showcurrentlabel(list[ind].wt, vertlabpt(list[ind].pt));
	return;
}


putptinlist(p1)
Point p1; {
	if ( last < MXPTS ) {
			/* put pt in list */
		list[last].status = 1;
		list[last].pt = p1;
		sprintf( list[last].wt, "%d\0", last ); 
			/* default weight (label) is index */
		strcpy( list[last].color, "L\0" ); 
			/* default color is light */
		if ( showingvertlabs == 1 )
			showcurrentlabel(list[last].wt, vertlabpt(p1));
		last++;
		numverts++;
	}
	else 
		disc( &display, p1, dsize, f2 );  /*erase disc already put there*/
}
