#include "defs.h"

int pickrect(rp)
register Rectangle *rp;
{
	*rp=getrect(1);
	if(rp->corner.x && rp->corner.x-rp->origin.x<=1
	&& rp->corner.y-rp->origin.y<=1)
		*rp=Dspace;
	return(rectclip(rp, Dspace)
	   && rp->corner.x-rp->origin.x>0 && rp->corner.y-rp->origin.y>0);
}

tightsetrect(setnum)
int setnum; {
	MEMBER *mbrptr;
	int i, Xmin=1000, Ymin=2000, Xmax=-1, Ymax=-1;
	
	mbrptr = set[setnum].next;
	while (mbrptr != 0) {
		i = mbrptr->ind;
		if ( list[i].pt.x < Xmin )
			Xmin = list[i].pt.x;
		if ( list[i].pt.y < Ymin )
			Ymin = list[i].pt.y;
		if ( list[i].pt.x > Xmax )
			Xmax = list[i].pt.x;
		if ( list[i].pt.y > Ymax )
			Ymax = list[i].pt.y;
		mbrptr = mbrptr->next;
	}
	set[setnum].Srect.origin.x = Xmin - 8;
	set[setnum].Srect.origin.y = Ymin - 8;
	set[setnum].Srect.corner.x = Xmax + 8;
	set[setnum].Srect.corner.y = Ymax + 8;
	return;
}



selset() {
	int i, pickrect(), deadset;
	Rectangle setrect;
	MEMBER *temp, *mbrptr;

    for (;;) {
	if (pickrect(&setrect) != 1)
		continue;
	for (i = 1; i < last; i++) {
		if (list[i].status == 1 && ptinrect(list[i].pt, setrect)) {

			if (list[i].set != -1) {
				/* dissolve this set */
				deadset = list[i].set;
				rectf(&display, set[deadset].Srect, f2); 
				mbrptr = set[deadset].next;
				while (mbrptr != 0) {
					list[mbrptr->ind].set = -1;
					mbrptr = mbrptr->next;
				}
				set[deadset].next = 0;
				set[deadset].status = 0;
			}

			list[i].set = lastset;	
			temp = set[lastset].next;
			set[lastset].next = (MEMBER *)alloc(sizeof(MEMBER));
			set[lastset].status = 1;
			set[lastset].next->ind = i;
			set[lastset].next->next = temp;
/*
			mkmbr(&(set[lastset].next), i);
*/
		}
	}
	tightsetrect(lastset);
	rectf(&display, set[lastset].Srect, f2);
	lastset++;
	break;
    }
    return;
}

/*
mkmbr(mbraddr, indx)
MEMBER **mbraddr;
int indx; {
	MEMBER *tempaddr;

	tempaddr = *mbraddr;
	*mbraddr = (MEMBER *)alloc(sizeof(MEMBER));
	(*mbraddr)->ind = indx;
	(*mbraddr)->next = tempaddr;
}
*/


unselset() {
	int i;
	MEMBER *mbrptr;

	for(;; wait(MOUSE)) {
		if (button1()) {
			while (button1()) ;
			if (ptinrect(mouse.xy, box)) {
				cmds();
				return;
			}
			for (i = 1; i < lastset; i++)
				if (ptinrect(mouse.xy, set[i].Srect) && set[i].status) {
					rectf(&display, set[i].Srect, f2); 
					break;
				}
			mbrptr = set[i].next;
			while (mbrptr != 0) {
				list[mbrptr->ind].set = -1;
				mbrptr = mbrptr->next;
			}
			set[i].next = 0;
			set[i].status = 0;
				
		}
		if (button3())
			return;
	}
			
}

eraseset(i)
int i; {
	MEMBER *mbrptr;

	rectf(&display, set[i].Srect, f2); 
	mbrptr = set[i].next;
	while (mbrptr != 0) {
		if (list[mbrptr->ind].status == 1)
			delptinlist(mbrptr->ind, list[mbrptr->ind].next);
		mbrptr = mbrptr->next;
	}
	set[i].next = 0;
	set[i].status = 0;
	return;
}


copyset(i, p1)
int i;
Point p1; {
	MEMBER *mbrptr, *temp;
	NODE *nodeptr;

	/* show new rect */
	set[lastset].status = 1;
	set[lastset].Srect = set[i].Srect;
	rectf(&display, set[lastset].Srect, f2);

	/* create new set */
	mbrptr = set[i].next;
	while (mbrptr != 0) {
		if (list[mbrptr->ind].status == 1) {
			/* create new vertex */
			list[mbrptr->ind].status = last;
				/* only temporarily! */
			list[last].status = 1;
			list[last].pt = list[mbrptr->ind].pt;
			sprintf(list[last].wt, "%d\0",last);
			strcpy(list[last].color, list[mbrptr->ind].color);
			list[last].set = lastset;
			numverts++;

			/* show new vertex */
			Disc(last);

			/* put in set */
			temp = set[lastset].next;
			set[lastset].next = (MEMBER *)alloc(sizeof(MEMBER));
			set[lastset].next->ind = last;
			set[lastset].next->next = temp;
			last++;
		}
		mbrptr = mbrptr->next;
	}

	mbrptr = set[i].next;
	while (mbrptr != 0) {
		nodeptr = list[mbrptr->ind].next;
		while (nodeptr != 0) {
			/* create new edge */
			if (nodeptr->ind >= mbrptr->ind || list[nodeptr->ind].set != i) {
				if (list[nodeptr->ind].status > 1)
					putedge(list[mbrptr->ind].status, list[nodeptr->ind].status, nodeptr->color, nodeptr->type);
				else
					putedge(list[mbrptr->ind].status, nodeptr->ind, nodeptr->color, nodeptr->type);
				/* show new edge */
				Segment(list[mbrptr->ind].status, list[list[mbrptr->ind].status].next);
			}
			nodeptr = nodeptr->next;
		}	
		mbrptr = mbrptr->next;
	}
	
	/* change flags back */
	mbrptr = set[i].next;
	while (mbrptr != 0) {
		if (list[mbrptr->ind].status > 1)
			list[mbrptr->ind].status = 1;
		mbrptr = mbrptr->next;
	}

	moveset(lastset,p1);
	lastset++;
}


moveset(i,p1)	
int i;
Point p1; {
	Point p2, ptdist;
	int stopped;
	MEMBER *mbrptr;
	NODE *nodeptr;

	stopped = 0;
	while ( !stopped ) {
		if ( !button1() ) stopped = 1;
		nap(2);
		p2 = p1;
		/* erase old */
		rectf(&display, set[i].Srect, f2);
		setmbrs(i);
		nap(2);
		if (!ptinrect(p1 = mouse.xy, Dspace))
			p1 = p2;
		/* don't allow set rect to move out of window */
		if ((set[i].Srect.origin.x < Dspace.origin.x) && (p1.x < p2.x))
			p1 = p2;
		if ((set[i].Srect.origin.y < Dspace.origin.y) && (p1.y < p2.y))
			p1 = p2;
		if ((set[i].Srect.corner.x > Dspace.corner.x) && (p1.x > p2.x))
			p1 = p2;
		if ((set[i].Srect.corner.y > Dspace.corner.y) && (p1.y > p2.y))
			p1 = p2;

		if (stopped) p1 = grid_enforce(p1,gridstate);

		ptdist = sub(p1, p2);
		set[i].Srect = raddp(set[i].Srect, ptdist);
		/* put new */
		rectf(&display, set[i].Srect, f2);
		mbrptr = set[i].next;
		while (mbrptr != 0) {
			if (list[mbrptr->ind].status == 1) {
				list[mbrptr->ind].pt = add(list[mbrptr->ind].pt, ptdist);
				Disc(mbrptr->ind);
				setmbrsedges(mbrptr->ind);
			}
			mbrptr = mbrptr->next;
		}
	
	}
}
