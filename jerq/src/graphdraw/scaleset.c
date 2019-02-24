#include "defs.h" 

/* used only for shrink & expand (set) op's */
scaleset(setnum, op)
int setnum, op; {

	Rectangle oldrect, newrect;
	int sfactor = 900, efactor = 1100, i, factor, c, big = 1000; 
	Point center, newcenter;
	MEMBER *mbrptr;



	oldrect = set[setnum].Srect;

	center.x = (oldrect.corner.x + oldrect.origin.x)/2;
	center.y = (oldrect.corner.y + oldrect.origin.y)/2;

	if (op == 0) {    /* shrink */
		c = (big - sfactor)/2;
		newrect.origin.x = muldiv((big+c), oldrect.origin.x, big);
		newrect.origin.y = muldiv((big+c), oldrect.origin.y, big);
		newrect.corner.x = muldiv((big-c), oldrect.corner.x, big);
		newrect.corner.y = muldiv((big-c), oldrect.corner.y, big);
		factor = sfactor;
		/* if too small, return */
		if ((newrect.corner.x - newrect.origin.x) < 2)
			return;
		if ((newrect.corner.y - newrect.origin.y) < 2)
			return;
	}
	else {   /* op == 1;  expand */ 
		/* if already too big, return */
		if (oldrect.origin.x < (Dspace.origin.x + 12))
			return;
		if (oldrect.origin.y < (Dspace.origin.y + 12))
			return;
		if (oldrect.corner.x > (Dspace.corner.x - 12))
			return;
		if (oldrect.corner.y > (Dspace.corner.y - 12)) 
			return;
		c = (efactor - big)/2;
		newrect.origin.x = muldiv((big-c), oldrect.origin.x, big);
		newrect.origin.y = muldiv((big-c), oldrect.origin.y, big);
		newrect.corner.x = muldiv((big+c), oldrect.corner.x, big);
		newrect.corner.y = muldiv((big+c), oldrect.corner.y, big);
		factor = efactor;
	}

	set[setnum].Srect = newrect;

	newcenter.x = (newrect.corner.x + newrect.origin.x)/2;
	newcenter.y = (newrect.corner.y + newrect.origin.y)/2;


	/* scale all pts */
	mbrptr = set[setnum].next;
	while (mbrptr != 0) {
		i = mbrptr->ind;
		list[i].pt.x = muldiv((list[i].pt.x - center.x), factor, big) + newcenter.x;
		list[i].pt.y = muldiv((list[i].pt.y - center.y), factor, big) + newcenter.y;
		mbrptr = mbrptr->next;
	}

	tightsetrect(setnum);
	return;
}
