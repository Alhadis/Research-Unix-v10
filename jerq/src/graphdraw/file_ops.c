#include "defs.h"

packlist() {
	int empty=1, i;
	NODE *edge1, *edge2;
	MEMBER *mbrptr;

	while ( empty < last ) {
		while ( list[empty].status  == 1 )  empty++;
		while ( list[last].status != 1 ) last--; 
		if ( empty < last ) {
			list[empty] = list[last];
				/* empty is new ind, last is old */

			list[last].status = 0;

			edge1 = list[empty].next;
			for ( i = 0; i < list[empty].deg; i++ ) {
				edge2 = list[edge1->ind].next;
				while ( edge2->ind != last )
					edge2 = edge2->next;
				edge2->ind = empty;
				edge1 = edge1->next;
			}

			if (list[empty].set != -1) {
			/* change index in set */
				mbrptr = set[list[empty].set].next;
				while ((mbrptr != 0) && (mbrptr->ind != last))
					mbrptr = mbrptr->next;
				mbrptr->ind = empty;
			}
		}
		else {
			last = empty;  /* make 'last' next free index */
			return;
		}
	}
}



sendlist() {
	int i, j, nedges, c=0;
	NODE *edge;
	char gtype[MINSTR], tmp[MXSTR];
	
	nedges = numedges/2;

	if (dirflag==1)
		gtype[c] = 'd';
	else
		gtype[c] = 'u';
	c++;
	if (multedflag==1) {
		gtype[c] = 'm';
		c++;
	}
	if (loopflag==1) {
		gtype[c] = 'l';
		c++;
	}
	gtype[c] = '\0';

        sprintf(tmp, "%d %d %s\n", numverts, nedges, gtype);  
		/* puts args in a string */
        sendnchars(strlen(tmp), tmp);

	for (i = 1; i < last; i++ ) {
			/* coords are type short but scanned as dec...*/
                sprintf(tmp, "%d %s %d %d %s\n", list[i].deg, list[i].wt, list[i].pt.x, list[i].pt.y, list[i].color);   
                sendnchars(strlen(tmp), tmp); 

		edge = list[i].next;
		for ( j = 0; j < list[i].deg; j++ ) {
                        sprintf(tmp, "%d %s %s\n", edge->ind, edge->wt, edge->color);
                        sendnchars(strlen(tmp), tmp); 
			edge = edge->next;
		}
	}
	return;
}



int mkint() { 
	register int ch=0;
	register int i=0;

		
	for (;; wait(RCV) ) {
		while ( (ch = rcvchar()) == -1 )
			wait(RCV);
		if ( (ch != ' ') && (ch != '\n') && (ch != '\t') ) 
			i = ( ch - '0' ) + i * 10;
		else 
			return( i );
	}
}


char mkline() {   /* used to get msg from Host when animating */
	register int ch=0;
	static char str[MXLINE];
	int i;
		
	for (i = 0; ; i++, wait(RCV) ) {
		while ( (ch = rcvchar()) == -1 )
			wait(RCV);
		if (ch != '\n')
			str[i] = ch;
		else {
			str[i] = '\0';
			return( str );
		}
	}
}


char mkstr() { 
	register int ch=0;
	static char str[MXLINE];
	int i;
		
	for (i = 0; ; i++, wait(RCV) ) {
		while ( (ch = rcvchar()) == -1 )
			wait(RCV);
		if ( (ch != ' ') && (ch != '\n') && (ch != '\t') ) 
			str[i] = ch;
		else {
			str[i] = '\0';
			return( str );
		}
	}
}


rcvfile() {
	int i, j, indx, mkint(), nverts, nedges;
	char gtype[MINSTR], wgt[MEDSTR], color[MINSTR], mkstr();

	nverts = mkint();
	numverts += nverts;

	nedges = mkint();
	numedges += (2*nedges);

	strcpy(gtype, mkstr());
	setgraphtype(0);
	setgraphtype(5);
		/* default types */
	if ( gtype[0] == 'd' ) {
		dirflag = 1;
		setgraphtype(1);
	}
	if ( gtype[1] == 'm' ) {
		multedflag = 1;
		setgraphtype(3);
	}
	if ( gtype[1] == 'l' || gtype[2] == 'l' ) {
		loopflag = 1;
		setgraphtype(4);
	}

	for ( i = last; i < (last + nverts); i++ ) {
		list[i].status = 1;
		list[i].deg = mkint();
		strcpy( list[i].wt, mkstr() );   /* copy mkstr() to list[i].wt */
		list[i].pt.x = mkint();
		list[i].pt.y = mkint();
		strcpy( list[i].color, mkstr() );   

		for ( j = 0; j < list[i].deg; j++ ) {
			indx = last - 1 + mkint();    
				/* in case appending file */
			strcpy( wgt, mkstr() );
			strcpy( color, mkstr() );
			putedgeinlist( indx, wgt, color, SEG, &(list[i].next) );
		}
	}
	last += nverts;  /* last is next free index */
	return;
}



displaylist(firstind)
int firstind; {
	int i, j, indx;
	NODE *edge;


	for ( i = firstind; i < last; i++ ) { 
	   if (list[i].status == 1)
		Disc(i); /* draw  all the vertices */
	}

	for ( i = firstind; i < last; i++ ) {      /* now fill in the edges */
	   if (list[i].status == 1) {
		edge = list[i].next;
/*
		while (edge != 0) {
*/
		for ( j = 0; j < list[i].deg; j++ ) {
			indx = edge->ind;
			if ( (indx > i) && (list[indx].status == 1))
			/* seg hasn't yet been drawn */
				Segment(i, edge);
			edge = edge->next;
		}
	   }
	}
	return;
}


heavyedges( p1, p2 ) 
Point p1, p2; {
	int x, y;

	/* find min x or y */
	x = abs(p1.x - p2.x);
	y = abs(p1.y - p2.y);

	if ( x < y ) {
		p1.x++;
		p2.x++;
		segment( &display, p1, p2, f2);
		point(&display, p1, f2);  /* so both endpoints are open */
		p1.x -= 2;
		p2.x -= 2; 
		segment( &display, p1, p2, f2);
		point(&display, p1, f2); 
	}
	else {
		p1.y++;
		p2.y++;
		segment( &display, p1, p2, f2);
		point(&display, p1, f2); 
		p1.y -= 2;
		p2.y -= 2;
		segment( &display, p1, p2, f2);
		point(&display, p1, f2); 
	}
}
