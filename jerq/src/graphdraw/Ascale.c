#include "defs.h" 

Point Ascale(x, y)
int x, y; {
	Point pt;

	int 
	    Xmin=0, Ymin=0, 
			/* 'origin' and 'corner' of old pic */
	    Xdistold, Ydistold, Xdistnew, Ydistnew,
	    Xdist, Ydist,
	    bdr=10;   /* allow room for border */

	static int Xmax=10000, Ymax=10000;

	if (x == -1) {
	/*flag to set scale factor to y; from program window scale anim. cmd*/
		Xmax = y;
		Ymax = y;
		return;
	}

	pt.x = x;
	pt.y = y;

	Xdistold = Xmax - Xmin;
	Ydistold = Ymax - Ymin;
	Xdistnew = Dspace.corner.x - Dspace.origin.x - 2*bdr;
	Ydistnew = Dspace.corner.y - Dspace.origin.y - 2*bdr;
		/* obviously don't need this */


	/* scale pt */
	Xdist = pt.x - Xmin;
	pt.x = (Xdistnew * Xdist)/Xdistold + Dspace.origin.x + bdr;
	Ydist = pt.y - Ymin;
	pt.y = (Ydistnew * Ydist)/Ydistold + Dspace.origin.y + bdr;

	return(pt);
}
