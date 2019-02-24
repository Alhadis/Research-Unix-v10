#include "defs.h"

vertex( f ) 
Code f; {

	Point p1, p2;
	int ind1;

	for (;; sleep(2), wait( MOUSE ) ) {
		if ( button1() ) {

			if ( ptinrect(p1 = mouse.xy, box) ) {
				cmds();
				return;
			}

			if ( !(ptinrect(p1, Dspace) ) )
				continue;

			if ( f == f1 ) {
			/* create point */
				disc( &display, p1, dsize, f2 );
				while ( button1() ) {
					p2 = p1;
					if (!(ptinrect((p1 =  mouse.xy), Dspace)))
						p1 = p2;
					disc( &display, p2, dsize, f2 ); 
						/* erase old */
					disc( &display, p1, dsize, f2 ); 
						/* put new */
				}
				disc( &display, p1, dsize, f2 );
				p1 = grid_enforce(p1,gridstate);
				disc( &display, p1, dsize, f2 ); 
				putptinlist(p1);
			}
			else {
			/* delete point */
				while ( button1() ) wait( MOUSE );
				p1 = closestpt( p1 );
				if ( (ind1 = findind( p1 )) != -1 )
				/* then point exists */
					if (list[ind1].set == -1)
					/* then not in set; delete vertex */
						delptinlist( ind1, list[ind1].next ); 
					else
					/* else in set; delete set */
						eraseset(list[ind1].set);
			}
		}
		if ( button3() )
			return;
	}
}
