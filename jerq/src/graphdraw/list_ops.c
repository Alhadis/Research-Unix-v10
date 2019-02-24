#include "defs.h"

int findind( pnt )
Point pnt; {

	int i=1;
	
	while ( i < last ) {
		if ( list[i].status == 1 ) 
			if ( list[i].pt.x == pnt.x && list[i].pt.y == pnt.y )
				return( i );
		i++;
	}
	return( -1 );
}

Point closestpt( p1 )
Point p1; {
	Point p0, p2;
	int i=1;
	short dist, smallest=MXDIST;

	while ( i < last )  {
		/* find smallest dist */
		if ( list[i].status == 1 ) {
			p2 = list[i].pt;
			dist = abs(p1.x-p2.x) + abs(p1.y-p2.y);
			if ( dist < smallest ) {
				smallest = dist;
				p0 = p2;
			}
		}
		i++;
	}
	if ( smallest < MXDIST )
		return( p0 );
	else return ( p1 );
}

delptinlist( ind1, edge )
int ind1;
NODE *edge; {
	int ind2;

	Disc(ind1);  /* erase disc & labels */
	while ( edge != 0 ) { 
			/* del edges connected to del'd pt */
		ind2 = edge->ind ; 
		if (edgesumflag == 1)
			subtfromEsum(edge->wt);
/*subt from edgesum as going thru list of ind1's edges*/
		Segment(ind1, edge);
		deledgeinlist( ind1, ind2, &(list[ind2].next) );
		edge = edge->next;
	}
	list[ind1].status = 0;
	strcpy( list[ind1].wt, " " );
	strcpy( list[ind1].color, " " );
	list[ind1].next = 0;
	numedges -= list[ind1].deg;
	numverts--;
	list[ind1].deg = 0;
	list[ind1].set = -1;

/*  don't need to del. pt. in set bec. when pt. in set del'd, whole set del'd. 
			see eraseset()...
	if (list[ind1].set != -1)
		delmbrinset(ind1, &set[list[ind1].set].next);
*/

		
	return;
}


delmbrinset( ind, mbrptr )
int ind; 
MEMBER **mbrptr; {

	if ( (*mbrptr)->ind ==  ind ) 
		(*mbrptr) = (*mbrptr)->next;
	else {

	/*     should check for end ... ?? (to avoid mem. fault even tho w/b wrong ) 
		while ( (*mbrptr)->next->next != 0 && (*mbrptr)->next->ind != ind )
	*/

		while ((*mbrptr)->next->ind != ind)
			mbrptr = &((*mbrptr)->next);

			/* mbr found */
		(*mbrptr)->next = (*mbrptr)->next->next;
	}
	list[ind].set = -1;
	return;
}



putedgeinlist( indx, wgt, color, type, edgeaddr )
int indx, type;  
char wgt[], color[];
NODE **edgeaddr; {

	NODE *temp;

	temp = (*edgeaddr);

	(*edgeaddr) = ( NODE * )alloc( sizeof( NODE ) );
	(*edgeaddr)->ind = indx;
	strcpy( (*edgeaddr)->wt, wgt );
	strcpy( (*edgeaddr)->color, color );
	(*edgeaddr)->type = type;
	(*edgeaddr)->next = temp;
	return;
} 


deledgeinlist( i1, i2, edgeptr )
int i1, i2; 
NODE **edgeptr; {
	NODE *deadedge;

	if ( (*edgeptr)->ind ==  i1 )  {
		deadedge = (*edgeptr);
		(*edgeptr) = (*edgeptr)->next;
		free(deadedge);
	}
	else {

	/*     should check for end ... ?? (to avoid mem. fault even tho w/b wrong ) 
		while ( (*edgeptr)->next->next != 0 && (*edgeptr)->next->ind != i1 )
	*/

		while ( (*edgeptr)->next->ind != i1 )
			edgeptr = &((*edgeptr)->next);

			/* edge found */
		deadedge = (*edgeptr)->next;
		(*edgeptr)->next = (*edgeptr)->next->next;
		free(deadedge);
	}	
	list[i2].deg--;
	numedges--;
	return;
}
