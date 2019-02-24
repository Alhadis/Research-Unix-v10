#include "defs.h"
#include <font.h>

labelvert() {
	Point p1;
	int ind1, r, done;
	char label[MEDSTR];

	for ( ;; wait(MOUSE) ) {
		if ( button1() ) {
			while ( button1() ) ;   /* wait for release */

			if ( ptinrect(mouse.xy, box) ) {
				rectf( &display, box, f1 );
				cmds();
				return;
			}

			p1 = closestpt( mouse.xy );
			if ( (ind1 = findind( p1 )) != -1 ) {
				circle( &display, p1, csize, f2 ); 
						/* circle  pt to be labeled */

				/* in case user changes mind ...? 
				if ( button3() ) {
					circle( &display, p1, csize, f2 ); 
					return;
				}  
				*/

				string( &defont, " \0", &display, boxstart, f1 );
				showcurrentlabel( list[ind1].wt, boxpt );

				done = 0;
				while ( !done ) {
		/* wait for typing (change label) or button1 click (keep label) */
					r = wait( KBD|MOUSE );
					if ( r&KBD ) {
						rectf( &display, box, f1 );
						string( &defont, " \0", &display, boxstart, f1 );
						strcpy( label, getkbdstr() );
						if ( strcmp(label, "-1\0") == 0 ) {
							circle( &display, p1, csize, f2 ); 
							break;
						}
						if ( showingvertlabs == 1 )
							showcurrentlabel(list[ind1].wt, vertlabpt(p1));
							/* erase old label */
						strcpy( list[ind1].wt, label );
						circle( &display, p1, csize, f2 );  /* erase circ */
						if ( showingvertlabs == 1 )
							showcurrentlabel(label, vertlabpt(p1));	
							/* put new label */
						done = 1;
					}
					else 	if ( r&MOUSE ) 
							if ( button1() ) {
								while ( button1() ) ;
								rectf( &display, box, f1 );
								circle( &display, p1, csize, f2 ); 
								done = 1;
							}
						
				}
			}
		}
		if ( button3() ) {
			rectf( &display, box, f1 );
			return;
		}
	}
	

}


labeledge() {
	Point p1, p[2], midpt;
	int i=0, ind[2], ok=1, r, done;
	NODE *nodeptr0, *nodeptr1;
	char label[MEDSTR];

	if (eucflag == 1) {
		rectf(&display, box, f1);
		string(&defont, "euclidean edge flag on; can't relabel edges", &display, boxstart, f2);
		return;
	}
	for ( ;; wait(MOUSE) ) {
		if ( button1() ) {
			while ( button1() ) ; 

			if ( ptinrect(mouse.xy, box) ) {
				if ( i == 1 )
					/* remove remaining circle */ 
					circle ( &display, p[0], csize, f2 );
				cmds();
				return;
			}

			p1 = closestpt( mouse.xy );
			if ( (ind[i] = findind( p1 )) != -1 ) {
				if ( list[ind[i]].next != 0 ) {
					if ( i == 1 ) {
					/* see if 2nd pt is conn to 1st */
						nodeptr0 = list[ind[1]].next;
						ok = 0;
						while ( nodeptr0 != 0 && !ok ) {
							if ( nodeptr0->ind != ind[0] )
								nodeptr0 = nodeptr0->next;
							else ok = 1;
						}
					}
					if ( i != 1 || ok ) {
						circle( &display, p1, csize, f2 );
						p[i] = p1;
						i++;
					}
				}
			}
			if ( i == 2 ) {
				i = 1;

				/* 2 pts were chosen; ready to label */
				rectf(&display, box, f1);
				string( &defont, " \0", &display, boxstart, f1 );
				showcurrentlabel( nodeptr0->wt, boxpt );

				done = 0;
				while ( !done ) { 
					r = wait( KBD|MOUSE );
					if ( r&KBD ) {
						rectf( &display, box, f1 );
						string( &defont, " \0", &display, boxstart, f1 );
	
						strcpy( label, getkbdstr() );
						if ( strcmp(label, "-1\0") == 0 ) {
							circle( &display, p[1], csize, f2 );
							break;
						}
						if ( showingedgelabs == 1 ) {
							/* erase old label */
							midpt = div(add(p[0], p[1]), 2);
							showcurrentlabel(nodeptr0->wt, midpt);	
						}
						if (edgesumflag == 1)
							subtfromEsum(nodeptr0->wt);
						nodeptr1 = list[ind[0]].next;
						while ( nodeptr1->ind != ind[1] )
							nodeptr1 = nodeptr1->next;

						strcpy( nodeptr0->wt, label );
						strcpy( nodeptr1->wt, label );

						if (edgesumflag == 1)
							addtoEsum(nodeptr0->wt);

						circle( &display, p[1], csize, f2 );
						/* erase 2nd circle */

						if ( showingedgelabs == 1 ) 
							/* put new label */
							showcurrentlabel(label, midpt);	
						done = 1;
						i = 1;
					}
					else if ( r&MOUSE ) 
						if ( button1() ) {
							while ( button1() ) ;
							/* keep current label */
							rectf( &display, box, f1 );
							circle( &display, p[1], csize, f2 ); 
							done = 1;
							i = 1;
						}
				}

			}
		}

		if ( button2() && i == 1) {
			circle ( &display, p[0], csize, f2 );
				/* erase 1st circle */
			i = 0;
		}

		if ( button3() ) {
			if ( i == 1 )
				/* remove remaining circle */ 
				circle ( &display, p[0], csize, f2 );
			rectf( &display, box, f1 );
				/*  to erase space */
			return;
		}

	}  /* for */
}



nmbrverts() {
	int i, j, vertlabflag=0;

	if ( showingvertlabs == 1 ) {
		showvertlabels();
		vertlabflag = 1;
	}
	j = 1;
	for ( i=1; i < last; i++)
		if ( list[i].status == 1) {
			sprintf(list[i].wt, "%d\0", j);
			j++;
		}
	if ( vertlabflag )
		showvertlabels();
	return;
}


