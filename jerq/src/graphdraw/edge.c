#include "defs.h"
#include <font.h>

edge( f ) 
Code f; {

	Point p1, p[2];
	int i=0, ind[2], edgethere, etype;
	NODE *nodeptr;
	MEMBER *mbrptr;
	char tmp[MEDSTR];
	Rectangle Brect[2];  /* border rects */

	etype = SEG;  /* default */
	for (;; sleep(2), wait(MOUSE) ) {


 		if ( button1() ) {
			while( button1() ) wait(MOUSE);
			rectf(&display, box, f1);   /* erase msg */

			if ( ptinrect(mouse.xy, box) ) {
				if ( i == 1 ) {
					if (list[ind[0]].set == -1)
						circle( &display, p[0], csize, f2 );
						/* remove remaining circle */
					else
						rectf(&display, Brect[0], f2);
						/*remove border of rect */
				}
				cmds();  
				return;
			}

			p1 = closestpt( mouse.xy );

			/* check for user errors */
			if ( ( ind[i] = findind( p1 ) ) == -1 )
				continue;
			if (f == f2 && list[ind[i]].next == 0)
				continue;

			if ( i == 1 ) {
			/* check to see if 2nd pt is conn to 1st */
				edgethere = 0;
				nodeptr = list[ind[1]].next; 
				while (nodeptr != 0 && !edgethere) {
					if (nodeptr->ind != ind[0])
						nodeptr = nodeptr->next;	
					else 
						edgethere = 1;
				}
			}

			if (i == 1 && f == f1) { 
			     	if (edgethere) { 
					if (!multedflag) {
						sprintf(tmp, "multiple edges not allowed\0");
						string(&defont, tmp, &display, boxstart, f2);
						continue;
		             		}
					else
						etype = ARC;
				}

				if (eqpt(p[0], p1)) {
					if (!loopflag) {
						sprintf(tmp, "loops not allowed");
						string(&defont, tmp, &display, boxstart, f2);
						continue;
					}
					else
						etype = LOOP;
				}

			}

			if (i == 1 && f == f2)
				if (!edgethere)
					continue;

			if (list[ind[i]].set == -1)
				circle( &display, p1, csize, f2 );
			else {
				Brect[i].origin.x = set[list[ind[i]].set].Srect.origin.x - 5;
				Brect[i].origin.y = set[list[ind[i]].set].Srect.origin.y - 5;
				Brect[i].corner.x = set[list[ind[i]].set].Srect.corner.x + 5;
				Brect[i].corner.y = set[list[ind[i]].set].Srect.corner.y + 5;
				rectf(&display, Brect[i], f2);
			}
		 	p[i] = p1;
			i++;

			if ( i == 2 ) {
				i = 1;
				if (list[ind[1]].set == -1)
					circle( &display, p[1], csize, f2 ); /* erase circ */
				else
					rectf(&display, Brect[1], f2);

			      for (;;) { 
				/* so only one case is done... ('break' used) */
				if (list[ind[0]].set == -1 && list[ind[1]].set == -1) {
					if (f == f1) {
						putedge(ind[0], ind[1], "L", etype);
						Segment(ind[0], list[ind[0]].next);
					}
					else {
						nodeptr = list[ind[0]].next; 
						while (nodeptr != 0) {
							if (nodeptr->ind == ind[1])
								break;
							else
								nodeptr = nodeptr->next;	
						}
						Segment(ind[0], nodeptr);
						deledge(ind[0], ind[1]);
					}
					break;
				}

				if (list[ind[0]].set != -1 && list[ind[1]].set != -1 ) {
		/* also check status of set! and make sure 2 diff sets !! */
					if (f == f1) 
						connsets(list[ind[0]].set, list[ind[1]].set);
					else 
						disconnsets(list[ind[0]].set, list[ind[1]].set);
					break;
				}
				

				if (list[ind[0]].set == -1 && list[ind[1]].set != -1) {
					mbrptr = set[list[ind[1]].set].next;
					while (mbrptr != 0) {
						nodeptr = list[ind[0]].next; 
						while (nodeptr != 0) {
							if (nodeptr->ind == mbrptr->ind)
								break;
							else
								nodeptr = nodeptr->next;	
						}
						if (nodeptr == 0 && f == f1) {
						/* edge not already there */
							putedge(ind[0], mbrptr->ind, "L", SEG);
							Segment(ind[0], list[ind[0]].next);
						}
						if (nodeptr != 0 && f == f2) {
							Segment(ind[0], nodeptr);
							deledge(ind[0], mbrptr->ind);
						}
						mbrptr = mbrptr->next;
					}
					break;
				}

				if (list[ind[0]].set != -1 && list[ind[1]].set == -1) {
					mbrptr = set[list[ind[0]].set].next;
					while (mbrptr != 0) {
						nodeptr = list[ind[1]].next; 
						while (nodeptr != 0) {
							if (nodeptr->ind == mbrptr->ind)
								break;
							else
								nodeptr = nodeptr->next;	
						}
						if (nodeptr == 0 && f == f1) {
						/* edge not already there */
							putedge(ind[1], mbrptr->ind, "L", SEG);
							Segment(ind[1], list[ind[1]].next);
						}
						if (nodeptr != 0 && f == f2) {
							Segment(ind[1], nodeptr);
							deledge(ind[1], mbrptr->ind);
						}
						mbrptr = mbrptr->next;
					}
					break;
				}


			   }
			   etype = SEG;  /*back to default*/
			}
 	 	}	

		if ( button2() && i == 1 )  {
			if (list[ind[0]].set == -1)
				circle( &display, p[0], csize, f2 );
			else
				rectf(&display, Brect[0], f2);
			i = 0;
			etype = SEG;
		}


		if ( button3() ) { 
			if ( i == 1 ) {
				if (list[ind[0]].set == -1)
					circle( &display, p[0], csize, f2 );
				else
					rectf(&display, Brect[0], f2);
			}
			return;
		}

	}
}
