#include "defs.h" 

/* called by cmds() for 'r' and 'a' cmds */

scale(newrect, firstind)
Rectangle newrect;
int firstind; {
	int i,
	    Xmin=1000, Ymin=2000, Xmax=-1, Ymax=-1,
	    Xdistold, Ydistold, Xdistnew, Ydistnew,
	    Xdist, Ydist,
	    Xratio, Yratio, distnew, distold,
	    Xmaxind, Ymaxind, centdist,
            big=2000, /* to shift over ratio result so precision is not lost */ 
	    bdr=10;   /* allow room for border */


	/* find 'origin' and 'corner' of old pic */
	for ( i = firstind; i < last; i++ ) {
		if (list[i].status == 1) {
			if ( list[i].pt.x < Xmin )
				Xmin = list[i].pt.x;
			if ( list[i].pt.y < Ymin )
				Ymin = list[i].pt.y;
			if ( list[i].pt.x > Xmax ) {
				Xmax = list[i].pt.x;
				Xmaxind = i;
			}
			if ( list[i].pt.y > Ymax ) {
				Ymax = list[i].pt.y;
				Ymaxind = i;
			}
		}
	}

	Xdistold = Xmax - Xmin;
	Ydistold = Ymax - Ymin;
	Xdistnew = newrect.corner.x - newrect.origin.x - 2*bdr;
	Ydistnew = newrect.corner.y - newrect.origin.y - 2*bdr;

	/* avoid dividing by zero */
	if (Xdistold == 0)
		Xdistold = 1;
	if (Ydistold == 0)
		Ydistold = 1;

	Xratio = muldiv( big, Xdistnew, Xdistold );
	Yratio = muldiv( big, Ydistnew, Ydistold );

	if ( Xratio < Yratio ) {
		distnew = Xdistnew;
		distold =  Xdistold;
	}
	else  {
		distnew = Ydistnew;
		distold =  Ydistold;
	}



	if ( distnew == 0 )
		distnew = 1;
	if ( distold == 0 )
		distold = 1;

	/* scale all pts */
	for ( i = firstind; i < last; i++ ) {
           if (list[i].status == 1) {	
		Xdist = list[i].pt.x - Xmin;
		list[i].pt.x = (distnew * Xdist)/distold + newrect.origin.x + bdr;
		Ydist = list[i].pt.y - Ymin;
		list[i].pt.y = (distnew * Ydist)/distold + newrect.origin.y + bdr;
	   }
	}

	/* center picture */
	if (Xratio < Yratio) {         /* scaled across x; center along y */
		centdist = (newrect.corner.y - list[Ymaxind].pt.y)/2;
		for ( i=firstind; i<last; i++ )
	            if (list[i].status == 1)	
			list[i].pt.y += centdist;
	}
	else  {
		/* scaled along y; center across x */
		centdist = (newrect.corner.x - list[Xmaxind].pt.x)/2;
		for ( i=firstind; i<last; i++ )
	            if (list[i].status == 1)	
			list[i].pt.x += centdist;
	}
	return;
}
