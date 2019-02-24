/*
 *      screen.c 1.4
 *
 *	Screen Handling Functions for Spreadsheet Program `vis'
 *
 *      A. F. Gettier
 *      Bell Laboratories
 *      Update made 11/1/82 11:13:42
 *      Retrieved 11/15/82 13:22:44
 */
#include	<math.h>
#include	<stdio.h>
#include	<signal.h>
#include	"curses.h"
#include	"vis.h"

extern int	LINES, COLS;
extern struct qheader	Fixup;

int	Width=DEFWIDTH;
int	Scale=DEFSCALE;

extern struct colhdr	Col;
extern struct rowhdr	Row;

/*
 *	Initialize the screen
 */
void
scrinit()
{
	extern quit();

	initscr();

	savetty();
	signal(SIGINT, quit);

	noecho();
	crmode();
	clear();

	/*
	 *	Set up the Heading
	 */

	setheading();

	/*
	 *	Print out the heading
	 */

	prheading();
}
/*
 *	Set the headings to the initial setting
 */
setheading()
{
	int	i;
	int	size;
	struct collabel	**c1;
	struct rowlabel	**r1;

	/*
	 *	Get the number of Rows
	 */
	size = LINES - 5;
	rowexpand( size );
	/*
	 *	Fix the print Positions Stuff
	 */
	r1 = Row.table;
	for ( i=0; i<size; i++ ) {
		r1[i]->position = i+3;
	}
	for ( i=size; i<Row.size; i++ ) {
		r1[i]->position = 0;
	}
	/*
	 *	Get the number of Columns
	 */
	size = ( COLS - 6 ) / (Width + 1);
	colexpand( size );
	/*
	 *	Fix the print Positions Stuff
	 */
	c1 = Col.table;
	for ( i=0; i<size; i++ ) {
		c1[i]->position = i * (Width + 1) + 6;
		c1[i]->cell = i;
	}
	for ( i=size; i<Col.size; i++ ) {
		c1[i]->position = 0;
		c1[i]->cell = i;
	}
}
/*
 *	Print the heading
 */
void
prheading()
{
	int	i, j, k, l;
	struct collabel	**c1;
	struct rowlabel	**r1;
	/*
	 *	Print the Frame
	 */
	erase();
	move( 2, 4 );
	printw( "------------------------------------" );
	printw( "----------------------------------------" );
	for ( i=3; i<(LINES-2); i++ ) {
		move( i, 4 );
		printw( "|" );
	}
	/*
	 *	Print the Labels
	 */
	r1 = Row.table;
	c1 = Col.table;
	/*
	 *	First the numbers on the Vertical
	 */
	for ( i=0; i<Row.size; i++ ) {
		if ( r1[i]->position < 0 )  continue;
		if ( r1[i]->position == 0 )  break;
		move( r1[i]->position, 1 );
		printw( "%d", i+1 );
	}
	/*
	 *	and the letters on the Horizontal
	 */
	l = i = 0;
	while ( i < Col.size && c1[i]->position+c1[i]->width+1 < COLS ) {
		if ( c1[i]->position <= 0 ) {
			i++;
			if ( l == 0 ) continue;
			else	break;
		}
		l = 0;
		move( 1, c1[i]->position + c1[i]->width/2 );
		j = i++;
		k = j / 26;
		if ( k > 0 )  k = k + 'A' - 1;
		else k = ' ';
		j =  j % 26 + 'A';
		printw( "%c%c", k, j );
	}
}

/*
 *	Print the Node
 */
prnode( node )
struct node	*node;
{
	int	i, j;
	struct collabel	**c1;
	struct rowlabel	**r1;

	/*
	 *	Get the print position
	 *
	 *
	 *	get the correct row
	 */
	r1 = Row.table;
	i = node->row;
	if ( i >= Row.size || r1[i]->position <= 0 ) return;
	/*
	 *	get the correct Column
	 */
	c1 = Col.table;
	j = node->col;
	if ( j >= Col.size || c1[j]->position <= 0 ) return;
	/*
	 *	Print the node
	 */
	move( r1[i]->position, c1[j]->position );
	noutput( node, c1[j]->width, c1[j]->scale );
}
/*
 *	output a single element
 *	  assume the cursor is positioned
 */
noutput( node, wid, scale )
struct node	*node;
int	wid, scale;
{
	int	i;
	char	tbuf[80], *x;

	switch( node->type ) {
	case UNDEF:
		for ( i=0; i<wid; i++ ) tbuf[i] = ' ';
		i = wid/2;
		tbuf[i++] = '*';
		tbuf[i] = '*';
		tbuf[wid] = '\0';
		break;
	case UNRES:
		for ( i=0; i<wid; i++ ) tbuf[i] = ' ';
		i = wid/2;
		tbuf[i++] = '?';
		tbuf[i] = '?';
		tbuf[wid] = '\0';
		break;
	case NUM:
		if ( (x=dtoa( node->value, wid, scale )) == 0 ) {
			for ( i=0; i<wid; i++ ) tbuf[i] = '#';
			tbuf[wid] = '\0';
		}
		else (void)strcpy( tbuf, x );
		break;
	case STRING:
		(void)strncpy( tbuf, node->svalue, wid );
		for ( i=strlen( tbuf ); i<wid; i++ ) tbuf[i] = ' ';
		tbuf[wid] = '\0';
		break;
	}
	printw( "%s", tbuf );
}

/*	dtoa.c
 *	S. Coffin, BTL Dept. 43372, FJ 1A110, x5151
 *	Apr. 15, 1982
 *
 *	Converts a double precision float to a right-justified
 *	printable character string according to the parameters
 *	size = total field width desired
 *	scale = number of digits to the right of the decimal
 *
 *	returns a pointer to the string.  If the size given
 *	is too small for the number, the routine attempts
 *	to use "e" notation.  If it still will not fit, NULL
 *	is returned.
 */

char *
dtoa( input, size, scale )
	double input;
	int size, scale;
{
	static char output[80];
	char pattern[80];
	int j=0, i;

	output[0] = '\0';

	/* get a sample string */
	(void)sprintf( pattern, "%-39.39lf", input );

	/* if the number is near zero, then try "e" notation */
	j=0;
	while( (pattern[j]=='0' || pattern[j]=='.') && pattern[j++]!='\0' );

	if( j < size && j > size/2 ) pattern[size] = '\0';
	else if ( j>size/2 && size>=9 )
		(void)sprintf( pattern, "%9.2e", input );
	else {
		/* not too near zero, so find the decimal point */
		j=0;
		while( pattern[j++] != '.' );

		/* truncate it to the scale */
		if( scale == 0 ) pattern[j-1] = '\0';
		else pattern[j+scale] = '\0';

		/* if its still too big, try "e" notation */
		if( (j-1 > size) || (j+scale > size) )
			(void)sprintf( pattern, "%9.2e",input);
	}

	/* is it still too big? */
	i=strlen(pattern);
	if( i <= size ) {
		/* ok, so right-justify the result, and return */
		for( j = 0; j < (size-i); j++ ) output[j]=' ';
		output[j] = '\0';
		strcat( output, pattern );
		return( output );
	}
	else return( NULL );
}

/*
 *	Copy out the Standard screen to a file
 */
copyfile( fp )
FILE	*fp;
{
	int	i, j, k;
	char	bf[150];
	char	*tbf;
	(void)fprintf( fp, "\n\n" );
	for ( i=1; i<LINES-2; i++ ) {

		tbf = bf;

		j = 0;
		k = COLS - 2;
		while ( j < COLS ) {
			bf[j] = (char)(stdscr->_y[i][j]);
			if (  bf[j] > ' ' ) k = j;
			j++;
		}
		bf[k+1] = '\0';
		(void)fprintf( fp, "%s\n", bf );
	}
	(void)fprintf( fp, "\n\n" );
}
/*
 *	reset the global scale
 */
setscale( n )
int	n;
{
	int	i;
	struct collabel	**c1;
	Scale = n;
	/*
	 *	Reset the individual headings
	 */
	c1 = Col.table;
	for ( i=0; i<Col.size; i++ ) {
		c1[i]->scale = Scale;
	}
	/*
	 *	Repaint the screen
	 */
	prheading();
	repaint();
}
/*
 *	reset the global width
 */
setwidth( n )
int	n;
{
	int	i;
	struct collabel	**c1;

	Width = n;
	/*
	 *	Reset the individual headings
	 */
	c1 = Col.table;
	for ( i=0; i<Col.size; i++ ) {
		c1[i]->width = Width;
	}
	fixcolheading();
	/*
	 *	Repaint the screen
	 */
	prheading();
	repaint();
}
/*
 *	reset an individual scale
 */
isetscale( col, n )
int	col, n;
{
	struct collabel	**c1;
	/*
	 *	Make sure it's valid
	 */
	if ( col > Col.size )  colexpand( col );
	if ( col < 0 ) {
		char	bfr[64];
		lexinit();
		unput( '\n' );
		(void)sprintf( bfr, "Attempt to change scale of column %d",
		    col );
		yyerror( bfr );
		return;
	}
	/*
	 *	Reset the individual headings
	 */
	c1 = Col.table;
	c1[col]->scale = n;
	/*
	 *	Repaint the screen
	 */
	prheading();
	repaint();
}
/*
 *	reset an individual width
 */
isetwidth( col, n )
int	col, n;
{
	struct collabel	**c1;

	/*
	 *	Make sure it's valid
	 */
	if ( col > Col.size )  colexpand( col );
	if ( col < 0 ) {
		char	bfr[64];
		lexinit();
		unput( '\n' );
		(void)sprintf( bfr, "Attempt to change scale of column %d",
		    col );
		yyerror( bfr );
		return;
	}
	/*
	 *	Reset the individual headings
	 */
	c1 = Col.table;
	c1[col]->width = n;
	fixcolheading();
	/*
	 *	Repaint the screen
	 */
	prheading();
	repaint();
}
/*
 *	Repaint the Screen
 */
repaint()
{
	int	i, j;
	struct node	*n;
	struct collabel	**c1;
	struct rowlabel	**r1;

	r1 = Row.table;
	c1 = Col.table;
	for ( i=0; i<Row.size; i++ ) {
		if( r1[i]->position == 0 ) break;
		if( r1[i]->position < 0 ) continue;
		n = r1[i]->next;
		while ( n != 0 ) {
			j = n->col;
			if( c1[j]->position == 0 ) break;
			if( c1[j]->position > 0 ) prnode( n );
			n = n->next;
		}
	}
}
/*
 *	Set the new Row headings
 */
fixrowheading()
{
	int	i, start;
	int	size;
	struct rowlabel	**r1;

	size = LINES - 5;
	/*
	 *	Get the Starting position
	 */
	r1 = Row.table;
	for ( start=0; start<Row.size; start++ )
		if ( r1[start]->position >= 0 ) break;
	/*
	 *	Get more rows if needed
	 */
	rowexpand( start + size + 1 );
	r1 = Row.table;
	/*
	 *	Fix the print Positions Stuff
	 */
	for ( i=0; i<size; i++ ) {
		r1[i+start]->position = i+3;
	}
	for ( i=size+start; i<Row.size; i++ ) {
		r1[i]->position = 0;
	}
}
/*
 *	Set the new Col headings
 */
fixcolheading()
{
	int	i, j, cpos, start;
	struct collabel	**c1;
	/*
	 *	Find the First cell to print
	 */
	c1 = Col.table;
	for ( start=0; start<Col.size; start++ ) {
		if ( c1[start]->position >= 0 ) break;
	}
	/*
	 *	Fix the print Positions Stuff
	 */
	cpos = 6;
	for ( i=start; i<Col.size; i++ ) {
		c1[i]->position = cpos;
		cpos += c1[i]->width + 1;
		if ( cpos > COLS ) {
			c1[i]->position = 0;
		}
	}
	i = (COLS - cpos ) / ( Width + 1 );
	j = Col.size;
	colexpand( i+j+1 );
	c1 = Col.table;
	for ( i=j; i<Col.size; i++ ) {
		c1[i]->position = cpos;
		cpos += c1[i]->width + 1;
		if ( cpos > COLS ) {
			c1[i]->position = 0;
		}
	}
}
/*
 *	zero out all the screen stuff
 */
zeroscreen()
{
	initscr();
	erase();
	Row.size = 0;
	Col.size = 0;
	Width = DEFWIDTH;
	Scale = DEFSCALE;
	/*
	 *	Set up the Heading
	 */
	setheading();
	/*
	 *	Print out the heading
	 */
	prheading();
}

/*
 *	SCROLLING
 */
scrup( n )
int	n;
{
	int	i, j, start;
	struct rowlabel	**r1;

	r1 = Row.table;
	/*
	 *	Get the starting position
	 */
	for ( start=0; start<Row.size; start++ ) {
		if (  r1[start]->position >= 0 )  break;
	}
	/*
	 *	Fix the offending rows
	 */
	i = start - n;
	if ( i < 0 )  i = 0;
	for ( j=i; j<Row.size; j++ ) {
		r1[j]->position = 0;
	}
	/*
	 *	Redraw the screen
	 */
	fixrowheading();
	prheading();
	repaint();
}
scrdown( n )
int	n;
{
	int	i, start;
	struct rowlabel	**r1;

	r1 = Row.table;
	/*
	 *	Get the starting position
	 */
	for ( start=0; start<Row.size; start++ ) {
		if (  r1[start]->position >= 0 )  break;
	}
	/*
	 *	Increase number of rows, if needed
	 */
	rowexpand( start + n + 1 );
	r1 = Row.table;
	/*
	 *	Get rid of the offending rows
	 */
	move( 3, 0 );
	for ( i=0; i<n; i++ ) {

		r1[start+i]->position = -1;

	}
	/*
	 *	Redraw the screen
	 */
	fixrowheading();
	prheading();
	repaint();
}
scrleft( n )
int	n;
{
	int	i, j, start;
	struct collabel	**c1;

	c1 = Col.table;
	/*
	 *	Get the starting position
	 */
	for ( start=0; start<Col.size; start++ ) {
		if (  c1[start]->position >= 0 )  break;
	}
	/*
	 *	Fix the offending rows
	 */
	i = start - n;
	if ( i < 0 )  i = 0;
	for ( j=i; j<Col.size; j++ ) {
		c1[j]->position = 0;
	}
	/*
	 *	Redraw the screen
	 */
	fixcolheading();
	prheading();
	repaint();
}
scrright( n )
int	n;
{
	int	i, start;
	struct collabel	**c1;

	c1 = Col.table;
	/*
	 *	Get the starting position
	 */
	for ( start=0; start<Col.size; start++ ) {
		if (  c1[start]->position >= 0 )  break;
	}
	/*
	 *	Increase number of cols, if needed
	 */
	colexpand( start + n + 1 );
	c1 = Col.table;
	/*
	 *	Get rid of the offending rows
	 */
	for ( i=0; i<n; i++ ) {
		c1[start+i]->position = -1;
	}
	/*
	 *	Redraw the screen
	 */
	fixcolheading();
	prheading();
	repaint();
}


void beep(){}
