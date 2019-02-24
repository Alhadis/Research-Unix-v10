#include "defs.h"

vertcolor(ind1)
int ind1; {

	switch (list[ind1].color[0]) {
	/* show/erase */
		case 'L':
			disc(&display, list[ind1].pt, dsize, f2);
			break;

		case 'H':
			disc(&display, list[ind1].pt, dsize, f2);
			circle( &display, list[ind1].pt, Hcsize, f2 ); 
				/* erase */
			break;

		case 'E':
			circle( &display, list[ind1].pt, Hcsize, f2 ); 
				/* erase */
			break;

		case 'F':
			disc(&display, list[ind1].pt, Hcsize, f2);
				/* erase */
			break;

		case 'I':
			break;
	}
}

 
changevertcolor(ind1, code)
int ind1;
char code[]; {

	switch (code[0]) {
	/* put in code */
		case 'L':
			disc(&display, list[ind1].pt, dsize, f2);
			strcpy(list[ind1].color, "L"); 
			break;

		case 'H':
			disc(&display, list[ind1].pt, dsize, f2);
			circle( &display, list[ind1].pt, Hcsize, f2 ); 
			/* show heavy */
			strcpy(list[ind1].color, "H"); 
			break;

		case 'E':
			circle( &display, list[ind1].pt, Hcsize, f2 ); 
			/* show */
			strcpy(list[ind1].color, "E"); 
			break;

		case 'F':
			disc( &display, list[ind1].pt, Hcsize, f2 ); 
			/* show */
			strcpy(list[ind1].color, "F"); 
			break;

		case 'I':
			/* invisible vertex for lines w/o endpoints */
			strcpy(list[ind1].color, "I"); 
			break;

	}
}


/* change vertex graphics */
vertgraphics(code)
char code[]; {
	Point p1;
	int ind1;
	MEMBER *mbrptr;

	for ( ;; wait(MOUSE) ) {
		if ( button1() ) {
			while ( button1() ) ;   /* wait for release */

			if ( ptinrect(mouse.xy, box) ) {
				rectf( &display, box, f1 );
				cmds();
				return;
			}

			p1 = closestpt( mouse.xy );
			if ( (ind1 = findind( p1 )) != -1 )
				vg(ind1, code);
		}

		if ( button3() )
			return;
	}
	
}


/* change edge graphics code in list & on screen */
changecolor(ind1, ind2, code)
int ind1, ind2;
char code[]; {
	NODE *nodeptr1, *nodeptr2;

	nodeptr1 = list[ind1].next;
	while ( nodeptr1->ind != ind2 )
		nodeptr1 = nodeptr1->next;

	nodeptr2 = list[ind2].next;
	while ( nodeptr2->ind != ind1 )
		nodeptr2 = nodeptr2->next;

	switch (nodeptr2->color[0]) {
		case 'L':
			switch (code[0]) {
				case 'L':
					break;

				case 'H':
					strcpy( nodeptr1->color, "H" );
					strcpy( nodeptr2->color, "H" );
					heavyedges(list[ind1].pt, list[ind2].pt);
						/* show  */
					break;
			}
			break;

		case 'H':
			switch (code[0]) {
				case 'H':
					break;

				case 'L':
					strcpy( nodeptr1->color, "L" );
					strcpy( nodeptr2->color, "L" );
					heavyedges(list[ind1].pt, list[ind2].pt);
						/* erase  */
					break;
			}
			break;

	}
}



/* change edge graphics */
edgegraphics(code)
char code[]; {
	Point p1, p[2];
	int i=0, ind[2], ok=1, ind1, ind2, edgethere;
	NODE *nodeptr;
	MEMBER *mbrptr, *mbrptr1, *mbrptr2;
	Rectangle Brect[2];  /* border rects */

	for ( ;; wait(MOUSE) ) {
		if ( button1() ) {
			while ( button1() ) ; 

			if ( ptinrect(mouse.xy, box) ) {
				if ( i == 1 ) {
					if (list[ind[0]].set == -1)
					/* remove remaining circle */ 
						circle ( &display, p[0], csize, f2 );
					else
						rectf(&display, Brect[0], f2);
				}
				cmds();
				return;
			}

			p1 = closestpt( mouse.xy );
			if ( (ind[i] = findind( p1 )) != -1 ) {
				if ( list[ind[i]].next != 0 ) {
					if ( i == 1 ) {
					/* see if 2nd pt is conn to 1st */
						nodeptr = list[ind[1]].next;
						ok = 0;
						while ( nodeptr != 0 && !ok ) {
							if ( nodeptr->ind != ind[0] )
								nodeptr = nodeptr->next;
							else ok = 1;
						}
					}
					if ( i != 1 || ok ) {
						if (list[ind[i]].set == -1)
							circle ( &display, p1, csize, f2 );
						else {
							Brect[i].origin.x = set[list[ind[i]].set].Srect.origin.x - 5;
							Brect[i].origin.y = set[list[ind[i]].set].Srect.origin.y - 5;
							Brect[i].corner.x = set[list[ind[i]].set].Srect.corner.x + 5;
							Brect[i].corner.y = set[list[ind[i]].set].Srect.corner.y + 5;
							rectf(&display, Brect[i], f2);
						}
						p[i] = p1;
						i++;
					}
				}
			}
			if ( i == 2 ) {
				i = 1;
			      for (;;) {
				/* so only one case is done... ('break' used) */
				if (list[ind[0]].set == -1 && list[ind[1]].set == -1) {
					/* 2 pts were chosen; ready to change code */
					changecolor(ind[0], ind[1], code);
					break;
				}

				if (list[ind[0]].set != -1 && list[ind[1]].set != -1 ) {
		/* also check status of set! and make sure 2 diff sets !! */
					mbrptr1 = set[list[ind[0]].set].next;
					while (mbrptr1 != 0 ) {
						ind1 = mbrptr1->ind;
						mbrptr2 = set[list[ind[1]].set].next;
						while (mbrptr2 != 0 ) {
							ind2 = mbrptr2->ind;
		

						/* see if edge already there */
							edgethere = 0;
							nodeptr = list[ind1].next;
							while (nodeptr != 0 && !edgethere) {
								if (nodeptr->ind != ind2)
									nodeptr = nodeptr->next;
								else
									edgethere = 1;
							}

							if (!edgethere) {
								mbrptr2 = mbrptr2->next;
								continue;
							}

							if (list[ind[0]].set == list[ind[1]].set) {
								if (ind2 > ind1)
									changecolor(ind1, ind2, code);
							}
							else
								changecolor(ind1, ind2, code);

							mbrptr2 = mbrptr2->next;

						}
						mbrptr1 = mbrptr1->next;	
	
					}
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
						if (nodeptr != 0)
							changecolor(ind[0], mbrptr->ind, code);

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
						if (nodeptr != 0)
							changecolor(mbrptr->ind, ind[1], code);

						mbrptr = mbrptr->next;
					}
					break;
				}

			   }

		           if (list[ind[1]].set == -1)	
			   	circle( &display, p[1], csize, f2 ); 
		           	/* erase circle */	
			   else
		                rectf(&display, Brect[1], f2);	
			}
		}

		if ( button2() && i == 1) {
		           if (list[ind[0]].set == -1)	
			   	circle( &display, p[0], csize, f2 ); 
		           	/* erase 1st circle */	
			   else
		                rectf(&display, Brect[0], f2);	
		           i = 0;
		}

		if ( button3() ) {
			if ( i == 1 )
				/* remove remaining circle or border */ 
		           if (list[ind[0]].set == -1)	
			   	circle( &display, p[0], csize, f2 ); 
			   else
		                rectf(&display, Brect[0], f2);	
			return;
		}

	}  /* for */
}
