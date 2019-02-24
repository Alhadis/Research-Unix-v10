#include "defs.h" 

/* called by reshape() (for set) */

reshaperect(setnum, newrect)
int setnum;
Rectangle newrect; {

	int i,
	    Xmin=1000, Ymin=2000, Xmax=-1, Ymax=-1,
	    Xdistold, Ydistold, Xdistnew, Ydistnew,
	    Xdist, Ydist,
	    Xratio, Yratio, distnew, distold,
	    Xmaxind, Ymaxind, centdist,
            big=2000, /* to shift over ratio result so precision is not lost */ 
	    bdr=10;   /* allow room for border */
	MEMBER *mbrptr;
	Rectangle oldrect;


	oldrect = set[setnum].Srect;

	Xdistold = oldrect.corner.x - oldrect.origin.x;
	Ydistold = oldrect.corner.y - oldrect.origin.y;

	Xdistnew = newrect.corner.x - newrect.origin.x;
	Ydistnew = newrect.corner.y - newrect.origin.y;

	/* avoid dividing by zero */
	if (Xdistold == 0)
		Xdistold = 1;
	if (Ydistold == 0)
		Ydistold = 1;

	/* now scale set */
	mbrptr = set[setnum].next;
	while (mbrptr != 0) {
		i = mbrptr->ind;
		Xdist = list[i].pt.x - oldrect.origin.x;
		list[i].pt.x = (Xdistnew * Xdist)/Xdistold + newrect.origin.x + bdr;
		Ydist = list[i].pt.y - oldrect.origin.y;
		list[i].pt.y = (Ydistnew * Ydist)/Ydistold + newrect.origin.y + bdr;
		mbrptr = mbrptr->next;
	}
	tightsetrect(setnum);
	return;

	/* center picture */
         /* scaled across x; center along y */
		/* scaled along y; center across x */
/*
	if (Xratio < Yratio) {
		centdist = (newrect.corner.y - list[Ymaxind].pt.y)/2;
		mbrptr = set[setnum].next;
		while (mbrptr != 0) {
			i = mbrptr->ind;
			list[i].pt.y += centdist;
			mbrptr = mbrptr->next;
		}
	}
	else  {
		centdist = (newrect.corner.x - list[Xmaxind].pt.x)/2;
		mbrptr = set[setnum].next;
		while (mbrptr != 0) {
			i = mbrptr->ind;
			list[i].pt.x += centdist;
			mbrptr = mbrptr->next;
		}
	}
*/
}
