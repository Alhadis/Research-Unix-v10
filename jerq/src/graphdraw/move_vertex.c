#include "defs.h"
#include <font.h>

move_vertex() {
	Point p1;
	int ind1;

	for ( ;; sleep(2), wait(MOUSE) ) {

		if ( button1() ) {

			if ( ptinrect(p1 = mouse.xy, box) ) {
				cmds();
				return;
			}

			if (!ptinrect(p1 = closestpt( p1 ), Dspace) )
				continue;
			if ( (ind1 = findind( p1 )) == -1 )
				continue;

			if (list[ind1].set == -1)

				move_vertex_now(ind1);
	
			else   /* list[ind1].set == -1 ; vertex in a set, move set */
				moveset(list[ind1].set,p1);
			/* better check set status first ...*/
		}

		if ( button3() )
			return;

	}
}

move_vertex_now(ind1)
int ind1;
{
	Point p1, p2, ptdist;
	NODE *nodeptr, *np2;
	int d, stopped;
	char inttostr();  /*to update euc. lab's*/

	stopped = 0;
	p1 = list[ind1].pt;
	while ( !stopped ) {
		if (! button1()) stopped = 1;
		nap(2);
		p2 = p1;
		/* erase old */
		nodeptr = list[ind1].next;
		while (nodeptr != 0) {
			Segment(ind1, nodeptr);
			nodeptr = nodeptr->next;
		}
		Disc(ind1);
		nap(2);
		if ( !ptinrect(p1 = mouse.xy, Dspace) )
			p1 = p2;
		if ( stopped ) p1 = grid_enforce(p1,gridstate);
		ptdist = sub(p1, p2);
		list[ind1].pt = add(list[ind1].pt, ptdist);
		/* put  new */
		nodeptr = list[ind1].next;
		while (nodeptr != 0) {
			if (eucflag == 1) {
				subtfromEsum(nodeptr->wt);
					/* subtract old wts */
				rectf(&display, box, f1);  
					/*clear box*/
				d = eucdist(list[ind1].pt, list[nodeptr->ind].pt);
				strcpy(nodeptr->wt, inttostr(d));
				addtoEsum(nodeptr->wt);
				if (edgesumflag == -1)
					rectf(&display, box, f1); 
			}
			Segment(ind1, nodeptr);
			nodeptr = nodeptr->next;
		}
		Disc(ind1);
	}
	if (eucflag == 1) {
		/*update other node for each changed edge*/	
		nodeptr = list[ind1].next;
		while (nodeptr != 0) {
			np2 = list[nodeptr->ind].next;
			while (np2 != 0 && np2->ind != ind1)
				np2 = np2->next;
			/*found edge; update*/
			strcpy(np2->wt, nodeptr->wt);
			nodeptr = nodeptr->next;
		}
	}
}
