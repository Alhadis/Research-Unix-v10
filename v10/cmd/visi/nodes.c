/*
 *      nodes.c 1.5
 *
 *	Node Handling Functions for Spreadsheet Program `vis'
 *
 *      A. F. Gettier
 *      Bell Laboratories
 *      Update made 11/3/82 02:08:13
 *      Retrieved 11/15/82 13:22:35
 */
#include	<stdio.h>
#include	<math.h>
#include	"curses.h"
#include	"vis.h"

static struct node	BLANKNODE={-1,-1,UNRES,0,0,0,0,0,0};

extern int	Width;
extern int	Scale;
extern struct rowhdr	Row;
extern struct colhdr	Col;
extern struct qheader	Fixup;

/*
 *	get a node for reading - returns undef
 *	    if not present
 */
struct node *
getnode( row, col )
int	row, col;
{
	struct rowlabel	**r1;
	struct node	*n1, *n2, *n3;
	/*
	 *	Is the row for the new node there?
	 */
	if ( Row.size <= row ) rowexpand( row );
	/*
	 *	Is the col for the new node there?
	 */
	if ( Col.size <= col ) colexpand( col );
	/*
	 *	Get the table addresses
	 */
	r1 = Row.table;
	/*
	 *	Is the node present at the first
	 *	  location?
	 */
	n1 = r1[row]->next;
	if ( n1 == 0 || n1->col > col ) {
		r1[row]->next = n2 = newnode( row, col );
		n2->next = n1;
		n2->type = UNDEF;
		n2->def = 0;
		return( n2 );
	}
	/*
	 *	Look Harder!!
	 */
	while ( n1 != 0 && n1->col <= col ) {
		if ( n1->col == col ) return( n1 );
		n2 = n1;
		n1 = n1->next;
	}
	n3 = newnode( row, col );
	n3->next = n1;
	n2->next = n3;
	n3->type = UNDEF;
	n3->def = 0;
	return( n3 );
}
/*
 *	Make a new unresolved node
 */
struct node *
newnode( row, col )
int	row, col;
{
	struct node	*n;
	n = NODE( node );
	n->row = row;
	n->col = col;
	n->type = UNRES;
	n->value = 0;
	n->def = 0;
	n->svalue = 0;
	n->next = 0;
	qinit( &(n->depend) );
	return( n );
}
/*
 *	mathop - do the math operation type checking, and return a
 *	    blank node
 */
struct node
mathop( n1, n2 )
struct node	*n1, *n2;
{
	struct node	n;
	n = BLANKNODE;
	if ( n1->type == NUM && n2->type == NUM ) {
		n.type = NUM;
	}
	else {
		if ( n1->row == -1 && n1->type == STRING )
			free( n1->svalue );
		if ( n2->row == -1 && n2->type == STRING )
			free( n2->svalue );
	}
	return( n );
}

/*
 *	List the definitions to the Terminal
 */
listfile()
{
	int	i, cnt;
	char	tbuf[128];
	struct node	*x;
	struct rowlabel	**r1;
	WINDOW	*listwin;
	listwin = newwin( LINES, COLS, 0, 0 );
	wmove( listwin, 0, 0 );
	wclear( listwin );
	wrefresh( listwin );
	cnt = 2;
	r1 = Row.table;
	for ( i=0; i<Row.size; i++ ) {
		x = r1[i]->next;
		while ( x != 0 ) {
			if ( x->def == 0 ) {
				x = x->next;
				continue;
			}
			(void)wprintw( listwin, "%s\n", x->def );
			x = x->next;
			if ( ++cnt < LINES ) continue;
			cnt = 2;
			wmove( listwin, LINES-2, (COLS-25)/2 );
			wstandout( listwin );
			(void)wprintw(listwin,"PRESS <RETURN> WHEN READY");
			wstandend(listwin);
			wrefresh( listwin );
			wgetstr(  listwin, tbuf );
			werase( listwin );
			wmove( listwin, 0, 0 );
		}
	}
	if ( cnt > 0 ) {
		wmove( listwin, LINES-2, (COLS-25)/2 );
		wstandout( listwin );
		(void)wprintw(listwin,"PRESS <RETURN> WHEN READY");
		wstandend(listwin);
		wrefresh( listwin );
		wgetstr(  listwin, tbuf );
	}
	delwin( listwin );
	touchwin( stdscr );
}

/*
 *	Dump the definitions to a file
 */
dumpfile( fp )
FILE	*fp;
{
	int	i;
	struct node	*x;
	struct rowlabel	**r1;
	r1 = Row.table;
	for ( i=0; i<Row.size; i++ ) {
		x = r1[i]->next;
		while ( x != 0 ) {
			if ( x->def != 0 )
				(void)fprintf( fp, "%s\n", x->def );
			x = x->next;
		}
	}
}

/*
 *	Zero all current the definitions
 */
zerodef()
{
	int	i;
	struct node	*x, *y;
	struct rowlabel	**r1;
	r1 = Row.table;
	for ( i=0; i<Row.size; i++ ) {
		x = r1[i]->next;
		while ( x != 0 ) {
			if ( x->def != 0 )
				free( x->def );
			if ( x->type == STRING && x->svalue != 0 )
				free( x->svalue );
			y = x;
			x = x->next;
			free( (char *)y );
		}
		r1[i]->next = 0;
	}
}

/*
 *	Expand the number of defined Rows
 */
rowexpand( amt )
int	amt;
{
	int	i,j;
	struct rowlabel	**r1, **r2;
	/*
	 *	check for validity
	 */
	if ( Row.size > amt ) return;
	/*
	 *	Allocate a new Header
	 */
	r1 = Row.table;
	i = amt + 1;
	r2 = (struct rowlabel **)malloc((unsigned)(i*sizeof(struct rowlabel **)));
	for ( j=0; j<Row.size; j++ )
		r2[j] = r1[j];
	for ( j=Row.size; j<i; j++ ) {
		r2[j] = NODE( rowlabel );
		r2[j]->rownum = j;
		r2[j]->position = 0;
		r2[j]->next = 0;
	}
	free( (char *)r1 );
	Row.size = i;
	Row.table = r2;
}
/*
 *	Expand the number of defined Cols
 */
colexpand( amt )
int	amt;
{
	int	i, j;
	struct collabel	**c1, **c2;
	/*
	 *	check for validity
	 */
	if ( Col.size > amt ) return;
	/*
	 *	Allocate a new Header
	 */
	c1 = Col.table;
	i = amt + 1;
	c2 = (struct collabel **)malloc((unsigned)(i*sizeof(struct collabel **)));
	for ( j=0; j<Col.size; j++ )
		c2[j] = c1[j];
	for ( j=Col.size; j<i; j++ ) {
		c2[j] = NODE( collabel );
		c2[j]->position = 0;
		c2[j]->cell = -1;
		c2[j]->colnum = j;
		c2[j]->width = Width;
		c2[j]->scale = Scale;
	}
	free( (char *)c1 );
	Col.size = i;
	Col.table = c2;
}
