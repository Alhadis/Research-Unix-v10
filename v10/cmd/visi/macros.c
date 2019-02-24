/*
 *      macros.c 1.8
 *
 *	Screen Handling Functions for Spreadsheet Program `vis'
 *
 *      A. F. Gettier
 *      Bell Laboratories
 *      Update made 11/15/82 10:50:58
 *      Retrieved 11/15/82 13:22:33
 */
#include	<math.h>
#include	<stdio.h>
#include	"curses.h"
#include	"vis.h"

extern int	LINES, COLS;
extern struct qheader	Fixup;

extern struct colhdr	Col;
extern struct rowhdr	Row;

/*
 *	duplicate a block of definitions
 */
duplicate( fr, fc, tr, tc, ar, ac )
int	fr, fc, tr, tc, ar, ac;
{
	char	tbuf[128];
	int	i, or, oc, nr, nc;
	struct rowlabel	**r1;
	struct node	*n, *n2;
	/*
	 *	get the row offset
	 */
	or = ar - fr;
	oc = ac - fc;
	/*
	 *	Now make all the copies
	 */
	r1 = Row.table;
	for ( i=fr; i<=tr && i<Row.size; i++ ) {
		n = r1[i]->next;
		while ( n != 0 ) {
			if ( n->col > tc )  break;
			if ( n->col >= fc ) {
				nr =  n->row + or;
				nc =  n->col + oc;
				n2 = getnode( nr, nc );
				(void)strcat(strcpy( tbuf, makevar( nr, nc ) ),
				    strchr( n->def, '=' ));
				n2->def = copystr( tbuf );
				qadd( &Fixup, n2 );
			}
			n = n->next;
		}
	}
}

/*
 *	replicate a block of definitions
 */
replicate( ar, ac, fr, fc, tr, tc )
int	ar, ac, fr, fc, tr, tc;
{
	char	tbuf[128], *defin;
	int	i, j;
	struct node	*n;
	/*
	 *	Is there a def to be copied
	 */
	n = getnode( ar, ac );
	if ( n->def == 0 )  {
		(void)sprintf( tbuf, "Not previous definition at %s", 
		    makevar( ar, ac ) );
		yyerror( tbuf );
		lexinit();
	}
	/*
	 *	get the definition part
	 */
	defin = strchr( n->def, '=' );
	if ( defin == 0 )  {
		(void)sprintf( tbuf, "Bug in the definition at %s", 
		    makevar( ar, ac ) );
		yyerror( tbuf );
		lexinit();
	}
	/*
	 *	Now make all the copies
	 */
	for ( i=fr; i<=tr; i++ ) {
		for ( j=fc; j<=tc; j++ ) {
			n = getnode( i, j );
			(void)strcat(strcpy( tbuf, makevar( i, j ) ), defin );
			n->def = copystr( tbuf );
			qadd( &Fixup, n );
		}
	}
}

/*
 *	Convert from a row and column format to a character string
 */
char *
makevar( row, col )
int	row, col;
{
	static char	buf[16];
	int	i;
	/*
	 *	and the letters on the vertical
	 */
	i = col / 26;
	if ( i > 0 )  i += 'A' - 1;
	else i = ' ';
	col =  col % 26 + 'A';
	(void)sprintf( buf, "%c%c%d", i, col, row+1 );
	return( buf );
}

/*
 *	List the HELP to the Terminal
 */
listhelp()
{
	int	cnt;
	char	tbuf[128];
	FILE	*fp;
	WINDOW	*listwin;

	if ( (fp=fopen( HELPFILE, "r" )) == 0 ) {
		(void)sprintf( tbuf, "Cannot open HELPFILE (%s)", HELPFILE );
		yyerror( tbuf );
		return;
	}
	listwin = newwin( LINES, COLS, 0, 0 );
	wmove( listwin, 0, 0 );
	wclear( listwin );
	wrefresh( listwin );
	loop {
		werase( listwin );
		for( cnt=0; cnt<LINES-2; cnt++ ) {
			if ( fgets( tbuf, 80, fp ) == NULL )  break;
			if( tbuf[0] == '\f' )  break;
			wmove( listwin, cnt, 0 );
			(void)wprintw( listwin, "%s", tbuf );
		}
		if ( cnt == 0 ) break;
		wmove( listwin, LINES-2, (COLS-25)/2 );
		wstandout( listwin );
		(void)wprintw(listwin,"PRESS <RETURN> WHEN READY");
		wstandend( listwin );
		wrefresh( listwin );
		wgetstr(  listwin, tbuf );
	}
	delwin( listwin );
	touchwin( stdscr );
}

/*
 *	edit the definitions file
 */
editfile()
{
	FILE	*fp;
	char	*file, bfr[128], *editor;


	clear();
	move( 0, 25 );
	standout( );
	printw( "Entering Editor, Please Wait" );
	standend( );
	move( 1, 0 );
	refresh();
	file = mktemp("/tmp/visXXXXXX");

	/*
	 *	Write the definitions out
	 */

	refresh();
	if ( (fp=fopen( file, "w" )) == 0 ) {
		char	errbuf[64];
		(void)sprintf( errbuf, "Cannot open '%s'", file );
		yyerror( errbuf );
		lexinit();
		return;
	}

	else dumpfile( fp );

	(void)fclose( fp );

	/*
	 *	zero out the current definitions
	 */

	zerodef();

	/*
	 *	edit the file
	 */

	editor = getenv( "ED" );

	if ( editor == 0 || *editor == '\0' )
		(void)sprintf( bfr, "ed %s", file );
	else
		(void)sprintf( bfr, "%s %s", editor, file );

	resetty();
	(void)system( bfr );
	noecho();
	crmode();

	/*
	 *	clean up the screen
	 */

	clear();
	prheading();
	refresh();

	/*
	 *	Read in the new definitions
	 */

	if ( (fp=fopen( file, "r" )) == 0 ) {
		char	errbuf[64];
		(void)sprintf( errbuf, "Cannot open '%s'", file );
		yyerror( errbuf );
		lexinit();
	}
	else readfile( fp );
	(void)unlink( file );
}

quit()
{
	move( LINES-1, 0 );
	clrtoeol();
	refresh();
	endwin();
	resetty();
	exit( 0 );
}
