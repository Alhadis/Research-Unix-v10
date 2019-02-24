/*
 *      lexio.c 1.4
 *
 *	Modified Lexical Analyzer I/O Package for Spreadsheet Program `vis'
 *
 *      A. F. Gettier
 *      Bell Laboratories
 *      Update made 11/1/82 11:12:16
 *      Retrieved 11/15/82 13:22:30
 */
#include	<stdio.h>
#include	"curses.h"
#include	"vis.h"

extern struct qheader	Fixup, Depend;
extern int	COLS, LINES;

enum states { FIRST, FFILE, FPEND, OTHER };

char	Inline[128];

static enum states	lstate;
static char	*line=0, unbuf[128];
static char	buffer[128];
static int	uncnt=0, first=TRUE;
static FILE	*fp;

/*
 *	Start lex up for this input line
 *	  This includes passing the initial line to the
 *	  lexical analyzer
 */
void
startlex( inbuf )
char	*inbuf;
{
	lexinit();
	(void)strcpy( buffer, inbuf );
	lstate = FIRST;
}

/*
 *	Reset lex for next line
 *	  This routine allows lex to restart as though 
 *	  it had never been run
 */
void
lexinit()
{
	/* set the current line as empty */

	Inline[0] = '\0';
	line = 0;

	/* empty the unput buffer */

	uncnt = 0;

	/* flush any fixups */

	while ( qread( &Fixup ) != 0 );

	/* turn off the file */

	/* lstate = OTHER; */
}

readfile( file )
FILE	*file;
{
	fp = file;
	lstate = FPEND;
}


/*
 *	New input routine for the lexical analyzer
 */
char input()
{
int	i;
	struct node	*t;
	/*
	 *	return stored up characters if present
	 */
	if ( uncnt > 0 ) {
		return( unbuf[ --uncnt ] );
	}
	/*
	 *	Do we need a new buffer?
	 */
	if ( line == 0 ) {
		switch( lstate ) {
		/*
		 *	First time in, get the input buffer
		 */
		case FIRST:
			(void)strcpy( Inline, cannon( buffer ) );
			line = buffer;
			lstate = OTHER;
			break;
		/*
		 *	Get a line from the file
		 */
		case FFILE:
		    tryfile:
			if ( fgets( buffer, 128, fp ) == NULL ) {
				lstate = OTHER;
				goto tryother;
			}
			for ( i=0; buffer[i]!='\0'; i++ )
				if ( buffer[i] == '\n' ) {
					buffer[i] = '\0';
					break;
				}
			(void)cannon( buffer );
			(void)strcpy( Inline, buffer );
			line = buffer;
			lstate = FPEND;
			break;
		case FPEND:
			loop {
				if ( (t = qread( &Fixup )) == 0 ) {
					lstate = FFILE;
					goto tryfile;
				}
				if ( t->def == 0 ) continue;
				strcpy( buffer, t->def );
				(void)strcpy( Inline, buffer );
				line = buffer;
				break;
			}
			break;
		case OTHER:
		    tryother:
			loop {
				if ( (t = qread( &Fixup )) == 0 ) {
					first = TRUE;
					return( '\0' );
				}
				if ( t->def == 0 ) continue;
				strcpy( buffer, t->def );
				(void)strcpy( Inline, buffer );
				line = buffer;
				break;
			}
		}
	}
	if ( *line == '#' || *line == ';' || *line == '\0' || *line == '\n' ) {
		line = 0;
		return( '\n' );
	}
	return( *line++ );
}
/*
 *	Unput the character
 */
unput( chr )
char	chr;
{
	unbuf[ uncnt++ ] = chr;
}
/*
 *	Fold all lower case to upper case
 */
char *foldup( string )
char	*string;
{
	char	*ts;
	/*
	 *	Fold up
	 */
	ts = string;
	while ( *ts != '\0' ) {
		if ( *ts >= 'a' && *ts <= 'z' )  *ts += 'A' - 'a';
		ts++;
	}
	return( string );
}
/*
 *	Remove all backspaces, general canonical processesing
 */
char *cannon( string )
char	*string;
{
	char	*ts, *ts2;
	/*
	 *	Take out all backspaces
	 */
	ts2 = ts = string;
	while ( *ts != '\0' ) {
		if ( *ts == '\b' ) {
			if ( ts != string ) ts2--;
		}
		else {
			if ( ts != ts2 ) *ts2 = *ts;
			ts2++;
		}
		ts++;
	}
	if ( ts != ts2 ) *ts2 = '\0';
	return( string );
}
/*
 *	Get the next token and treat it as a value for the previous
 *		token, such as the file name for a read
 */
char *
collect()
{
	static char	buff[64];
	char	c;
	int	i;
	while ( (c=input()) == ' ' || c == '\t' );
	if ( c == '\n' ) {
		unput( c );
		return( 0 );
	}
	i = 1;
	buff[0] = c;
	while ( (c=input()) > ' ' )  buff[i++] = c;
	buff[i] = '\0';
	unput( c );
	return( buff );
}
