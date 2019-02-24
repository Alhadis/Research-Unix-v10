/*
 *      queues.c 1.4
 *
 *	Queue Handling Functions for Spreadsheet Program `vis'
 *
 *      A. F. Gettier
 *      Bell Laboratories
 *      Update made 11/1/82 11:13:33
 *      Retrieved 11/15/82 13:22:42
 */
#include	"vis.h"
#include	<stdio.h>

/*LINTLIBRARY*/
/*
 *	Add to the end of a queue
 */
void qadd( hdr, val )
struct qheader	*hdr;
struct node	*val;
{
	struct qentry	*t, *a, *b;
	struct node	*x;
	t = NODE( qentry );
	t->next = 0;
	t->nodeptr = val;
	a = hdr->first;
	b = 0;
	if ( a == 0 ) {
		hdr->first = hdr->last = t;
		return;
	}
	loop {
		x = a->nodeptr;
		/*
		 *	don't enter if present
		 */
		if ( (val->row == x->row && val->col < x->col )
		    || val->row < x->row ) {
			b = a;
			a = a->next;
			if ( a == 0 ) {
				b->next = hdr->last = t;
				return;
			}
		}
		else {
			if (val->row == x->row && val->col == x->col )
				return;
			if ( b == 0 ) {
				t->next = hdr->first;
				hdr->first = t;
			}
			else {
				t->next = b->next;
				b->next = t;
			}
			return;
		}
	}
}



/*
 *	Read from the front of a queue
 */
struct node *qread( hdr )
struct qheader	*hdr;
{
	struct node	*t;
	struct qentry	*s;
	if ( hdr->first == 0 )  return( 0 );
	t = (hdr->first)->nodeptr;
	s = (hdr->first)->next;
	free( (char *)hdr->first );
	hdr->first = s;
	if ( s == 0 ) hdr->last = 0;
	return( t );
}



/*
 *	Empty a queue
 */
void qempty( hdr )
struct qheader	*hdr;
{
	struct qentry	*t;
	while ( hdr->first != 0 ) {
		t = hdr->first;
		free( (char *)hdr->first );
		hdr->first = t;
	}
	hdr->last = 0;
}



/*
 *	Initialize a queue
 */
void qinit( hdr )
struct qheader	*hdr;
{
	hdr->first = 0;
	hdr->last = 0;
}



/*
 *	Copy a Queue
 */
struct qheader *qcopy( hdr )
struct qheader	*hdr;
{
	struct qheader	*t;
	struct qentry	*s, *s1;
	t = NODE( qheader );
	t->first = 0;
	t->last = 0;
	s1 = hdr->first;
	while( s1 != 0 ) {
		s = NODE( qentry );
		if ( t->first == 0 )
			t->first = s;
		else 
			(t->last)->next = s;
		t->last = s;
		s->next = 0;
		s->nodeptr = s1->nodeptr;
		s1 = s1->next;
	}
	return( t );
}
