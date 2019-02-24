/*
 *      vis.c 1.8
 *
 *	Executive for spreadsheet program
 *
 *      A. F. Gettier
 *      Bell Laboratories
 *      Update made 11/10/82 19:33:40
 *      Retrieved 11/15/82 13:22:53
 */
#include	<stdio.h>
#include	"curses.h"
#include	<signal.h>
#include	"vis.h"

static char	*sccs__id="@(#)vis.c	1.8\n";

extern int	LINES, COLS;

/* extern FILE* _outf; */

main(argc, argv)
int argc;
char *argv[];
{
	int	i;

	/*
	 *	Initialize the parser
	 */

/*	_outf = fopen("outf","w"); */
	pinit();

	/*
	 *	Initialize the screen
	 */

	scrinit();

	/*
	 *	Initialize the Hashing Functions
	 */

	hashinit();

	/*
	 *	Read from file if it is given as arg
	 */
	while ( argc > 1 ) {

		FILE	*file;
		move( LINES-2, 0 );
		clrtoeol();
		standout();

		if ( (file = fopen( argv[1], "r" )) == 0 ) {
			printw( "CANNOT OPEN FILE:  %s", argv[1] );
			move( LINES-1, 0 );
			standend();
			refresh();
			exit();
		}

		printw( "READING FILE:  %s", argv[1] );
		standend();
		refresh();
		startlex( "" );
		readfile( file );
		(void)yyparse();
		argc--; argv++;
	}

	/*
	 *	Now loop on the keyboard
	 */

	loop {
		char	buffer[80];
		char	*tbuf;

		do {
			/*
			 *	Print the prompt
			 */

			move( 0, 0 );
			printw( ">>  " );
			clrtoeol();
			refresh();

			/*
			 *	Get a line of input
			 */

		} while( !vgline( buffer ) );

		/*
		 *	erase the old prompt
		 */

		move( 0, 0 );
		clrtoeol();

		/*
		 *	Print the user input
		 */

		move( LINES-1, 0 );
		clrtoeol();
		printw( "%s", buffer );

		/*
		 *	clear for errors
		 */

		move( LINES-2, 0 );
		clrtoeol();

		i = 0;
		while ( buffer[i] == ' ' || buffer[i] == '\t' 
		    || buffer[i] == '\n' )
			i++;
		if ( buffer[i] == '\0' )  continue;

		/*
		 *	Do the work
		 */

		refresh();
		startlex( buffer );
		(void)yyparse();
	}
}

vgline( buffer )
char	*buffer;
{
	char	*tbuf;

	tbuf = buffer;
	loop {
		*tbuf = (char)getch();

		if ( *tbuf == '\014' ) 
			wrefresh( curscr );
		else if ( *tbuf == _tty.sg_erase ) {
			if ( tbuf == buffer ) {
				beep();
				continue;
			}
			tbuf--;
			addch( '\b' );
			addch( ' ' );
			addch( '\b' );
		}

		else if ( *tbuf == _tty.sg_kill )
			return(0);

		else if ( *tbuf == '\n' || *tbuf == '\r') {
			*tbuf = '\n';
			addch( *tbuf );
			*tbuf = '\0';
			break;
		}

		else if ( *tbuf >= ' ' || *tbuf == '\t' ) {
			addch( *tbuf );
			tbuf++;
		}

		refresh();
	}
	return(1);
}

yyerror(s)
char	*s;
{
	move( LINES-2, 0 );
	standout();
	printw( "%s", s );
	standend();
	clrtoeol();
}
