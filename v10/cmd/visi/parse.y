%{
/*
 *      parse.y 1.7
 *
 *	Parser Description File for Spreadsheet Program `vis'
 *
 *      A. F. Gettier
 *      Bell Laboratories
 *      Update made 11/15/82 10:51:06
 *      Retrieved 11/15/82 13:22:37
 */

#include	<stdio.h>
#include	<math.h>
#include	"curses.h"
#include	"vis.h"

extern struct qheader	Fixup;

int	Inrow, Incol;
%}

%start lines

%union {
	char	*sval;
	double	dval;
	int	ival;
	struct vdef	vval;
	struct node	nval;
	struct range	rval;
	struct colstat  cval;
}

%type <ival> amount direc
%type <nval> expr
%type <rval> range
%type <cval> colstat

%left '+' '-'
%left '*' '/' '%'
%left EXPON
%left UMINUS

%token	PI ABS ACOS ASIN ATAN ATAN2 COS EXP GAMMA HYPOT
%token	INT LOG POW SIN SQRT COL DUP DUPLICATE
%token	EDIT POSITION REDRAW REFRESH REP REPLICATE ROW
%token	SCALE SHIFT SLIDE SHELL SH WIDTH QUIT TERM LIST
%token	ERROR ZERO THRU AT DEBUG VER UP DOWN LEFT RIGHT
%token	HELP 
%token	<sval>	STR LETTERS READ WRITE COPY
%token	<dval>	NUMBER
%token	<ival>	FUNC
%token	<vval>	AVARIABLE
%token	<nval>	VARIABLE

%%

lines	:	/* empty */
	|	lines line
	;
line	:	AVARIABLE '=' expr TERM
	    {
		struct node	*nt;
		/*
		 *	Get the Target Node
		 */
		nt = getnode( $1.row, $1.col );
		/*
		 *	Assign the String
		 */
		if ( nt->def != 0 )
			free( nt->def );
		nt->def = $1.tval;
		/*
		 *	Was the Old Value a String?
		 */
		if ( nt->type == STRING ) {
			free( nt->svalue );
			nt->svalue = 0;
		}
		/*
		 *	Do the assignment
		 */
		switch ( $3.type ) {
		case NUM:
			if ( nt->type != NUM || nt->value != $3.value ) {
				struct qheader	*y;
				struct node	*x;
				y = qcopy( &(nt->depend) );
				while ( (x=qread( y ) ) != 0 )
					qadd( &Fixup, x );
				nt->value = $3.value;
				free( (char *)y );
			}
			nt->type = $3.type;
			break;
			
		case STRING:
			nt->svalue = $3.svalue;
			nt->type = $3.type;
			break;
		default:
			nt->type = UNRES;
		}
		prnode( nt );
	    }
	|	COPY TERM
	    {
		FILE	*fp;
		char	*fn;
		if ( $1 == 0 )
			fn = getfn();
		else
			fn = $1;
		if ( (fp=fopen( fn, "w" )) == 0 ) {
			char	errbuf[64];
			(void)sprintf( errbuf, "Cannot open '%s'", fn );
			yyerror( errbuf );
			lexinit();
		}
		else {
			move( LINES-2, 0 );
			printw( "Copying Screen Image to File '%s'", fn );
			clrtoeol();
			refresh();
			copyfile( fp );
			(void)fclose( fp );
		}
	    }
	|	DEBUG TERM
	    {
/*
		yydebug = 1 - yydebug;
*/
	    }
	|	DUPLICATE range AT VARIABLE TERM
	    {
		duplicate( $2.fromrow, $2.fromcol, $2.torow, $2.tocol,
		    $4.row, $4.col );
	    }
	|	DUP range AT VARIABLE TERM
	    {
		duplicate( $2.fromrow, $2.fromcol, $2.torow, $2.tocol,
		    $4.row, $4.col );
	    }
	|	EDIT TERM
	    {
		editfile();
	    }
	|	HELP TERM
	    {
		listhelp();
	    }
	|	LIST TERM
	    {
		listfile();
	    }
	|	QUIT TERM
	    {
		quit();
	    }
	|	READ TERM
	    {
		FILE	*fp;
		char	*fn;
		if ( $1 == 0 )
			fn = getfn();
		else
			fn = $1;
		if ( (fp=fopen( fn, "r" )) == 0 ) {
			char	errbuf[64];
			(void)sprintf( errbuf, "Cannot open '%s'", fn );
			yyerror( errbuf );
			lexinit();
		}
		else {
			move( LINES-2, 0 );
			printw( "Reading from File '%s'", fn );
			clrtoeol();
			refresh();
			readfile( fp );
		}
	    }
	|	REDRAW TERM
	    {
		wrefresh( curscr );
	    }
	|	REFRESH TERM
	    {
		wrefresh( curscr );
	    }
	|	REPLICATE VARIABLE AT range TERM
	    {
		replicate( $2.row, $2.col, $4.fromrow, $4.fromcol, $4.torow,
		    $4.tocol );
	    }
	|	REP VARIABLE AT range TERM
	    {
		replicate( $2.row, $2.col, $4.fromrow, $4.fromcol, $4.torow,
		    $4.tocol );
	    }
	|	SCALE colstat
	    {
		if ( $2.colnum < 0 )
			setscale( $2.number );
		else
			isetscale( $2.colnum, $2.number );
	    }
	|	SHIFT direc amount
	    {
		switch( $2 ) {
		case 1:
			scrup( $3 );
			break;
		case 2:
			scrdown( $3 );
			break;
		case 3:
			scrleft( $3 );
			break;
		case 4:
			scrright( $3 );
		}
	    }
	|	SH TERM
	    {
		move( LINES-1, 0 );
		clrtoeol();
		refresh();
		resetty();
		(void)system( "/bin/sh" );
		noecho();
		crmode();
		wrefresh( curscr );
	    }
	|	SHELL TERM
	    {
		move( LINES-1, 0 );
		clrtoeol();
		refresh();
		resetty();
		(void)system( "/bin/sh" );
		noecho();
		crmode();
		wrefresh( curscr );
	    }
	|	VER TERM
	    {
		yyerror("VIS 1.7 11/15/82");
	    }
	|	WIDTH colstat
	    {
		if ( $2.colnum < 0 )
			setwidth( $2.number );
		else
			isetwidth( $2.colnum, $2.number );
	    }
	|	WRITE TERM
	    {
		FILE	*fp;
		char	*fn;
		if ( $1 == 0 )
			fn = getfn();
		else
			fn = $1;
		if ( (fp=fopen( fn, "w" )) == 0 ) {
			char	errbuf[64];
			(void)sprintf( errbuf, "Cannot open '%s'", fn );
			yyerror( errbuf );
			lexinit();
		}
		else {
			move( LINES-2, 0 );
			printw( "Writing to File '%s'", fn );
			clrtoeol();
			refresh();

			dumpfile( fp );

			(void)fclose( fp );
		}
	    }
	|	ZERO TERM
	    {
		/*
		 *	zero out the current definitions
		 */
		zerodef();
		zeroscreen();
	    }
	|	error TERM
	;
expr	:	'(' expr ')'
	    {
		$$ = $2;
	    }
	|	expr '+' expr
	    {
		$$ = mathop( &($1), &($3) );
		if ( $$.type == NUM )  $$.value = $1.value + $3.value;
	    }
	|	expr '-' expr
	    {
		$$ = mathop( &($1), &($3) );
		if ( $$.type == NUM )  $$.value = $1.value - $3.value;
	    }
	|	expr '*' expr
	    {
		$$ = mathop( &($1), &($3) );
		if ( $$.type == NUM )  $$.value = $1.value * $3.value;
	    }
	|	expr '/' expr
	    {
		$$ = mathop( &($1), &($3) );
		if ( $$.type == NUM ) {
			if ( $3.value == 0 ) $$.value = BIG;
			else $$.value = $1.value / $3.value;
		}
	    }
	|	expr EXPON expr
	    {
		$$ = mathop( &($1), &($3) );
		if ( $$.type == NUM )  $$.value = pow( $1.value, $3.value );
	    }
	|	'-' expr %prec UMINUS
	    {
		$$ = $2;
		$$.value = - $2.value;
	    }
	|	'+' expr %prec UMINUS
	    {
		$$ = $2;
	    }
	|	FUNC '(' expr ')'
	    {
		double	xval;
		$$ = mathop( &($3), &($3) );
		if ( $$.type == NUM ) {
			xval = $3.value;
			switch( $1 ) {
			case ABS:
				if ( xval < 0 )  $$.value = -xval;
				else $$.value = xval;
				break;
			case ACOS:
				$$.value = acos( xval );
				break;
			case ASIN:
				$$.value = asin( xval );
				break;
			case ATAN:
				$$.value = atan( xval );
				break;
			case COS:
				$$.value = cos( xval );
				break;
			case EXP:
				$$.value = exp( xval );
				break;
			case GAMMA:
				$$.value = gamma( xval );
				break;
			case INT:
				$$.value = (double)((int)xval);
				break;
			case LOG:
				$$.value = log( xval );
				break;
			case SIN:
				$$.value = sin( xval );
				break;
			case SQRT:
				$$.value = sqrt( xval );
				break;
			default:
				lexinit();
				unput( '\n' );
				yyerror( "Incorrect Function Call" );
			}
		}
	    }
	|	FUNC '(' expr ',' expr ')'
	    {
		double	xval, yval;
		$$ = mathop( &($3), &($5) );
		if ( $$.type == NUM ) {
			xval = $3.value;
			yval = $5.value;
			switch( $1 ) {
			case ATAN2:
				$$.value = atan2( xval, yval );
				break;
			case HYPOT:
				$$.value = hypot( xval, yval );
				break;
			case POW:
				$$.value = pow( xval, yval );
				break;
			default:
				lexinit();
				unput( '\n' );
				yyerror( "Incorrect Function Call" );
			}
		}
	    }
	|	POSITION '[' expr ',' expr ']'
	    {
		struct node	*n;
		$$ = mathop( &($3), &($5) );
		if ( $$.type == NUM ) {
			int	i, j;
			i = (int)$3.value-1;
			j = (int)$5.value-1;
			if ( i < 0 || j < 0 ) {
				char	bfr[128];
				lexinit();
				(void)sprintf( bfr,
				    "Illegal Request for Position[ %d, %d ]",
				    i+1, j+1 );
				yyerror( bfr );
				$$.type = UNDEF;
				unput( '\n' );
			}
			else {
				n = getnode( i, j );
				prnode( n );
				qadd( &(n->depend), getnode( Inrow, Incol ) );
				$$ = *n;
				if ( $$.type == STRING )
					$$.svalue = copystr( $$.svalue );
				$$.row = -1;
				$$.col = -1;
			}
		}
		else
			$$.type = UNRES;
	    }
	|	COL
	    {
		$$.svalue = 0;
		$$.def = 0;
		$$.type = NUM;
		$$.value = (float)(Incol + 1);
		$$.row = -1;
		$$.col = -1;
	    }
	|	ROW
	    {
		$$.svalue = 0;
		$$.def = 0;
		$$.type = NUM;
		$$.value = (float)(Inrow+1);
		$$.row = -1;
		$$.col = -1;
	    }
	|	VARIABLE
	    {
		struct node	*x;
		x = getnode( $1.row, $1.col );
		$$ = *x;
		/*  If it is a string, patch it up  */
		if ( x->type == STRING ) {
			$$.svalue = copystr( x->svalue );
			$$.row = $$.col = (-1);
		}
		/*  Mark The Screen That it is Undef  */
		if ( x->type == UNDEF )  prnode( x );
		/*  Set up the Dependency List  */
		(void)qadd( &(x->depend), getnode( Inrow, Incol ) );
	    }
	|	STR
	    {
		$$.row = -1;
		$$.col = -1;
		$$.type = STRING;
		$$.svalue = $1;
	    }
	|	NUMBER
	    {
		$$.row = -1;
		$$.col = -1;
		$$.type = NUM;
		$$.value = $1;
		$$.svalue = 0;
		$$.def = 0;
	    }
	;
range	:	VARIABLE THRU VARIABLE
	    {
		$$.fromrow = $1.row;
		$$.fromcol = $1.col;
		$$.torow = $3.row;
		$$.tocol = $3.col;
	    }
	|	VARIABLE
	    {
		$$.fromrow = $$.torow = $1.row;
		$$.fromcol = $$.tocol = $1.col;
	    }
	;
direc	:	UP
	    {
		$$ = 1;
	    }
	|	DOWN
	    {
		$$ = 2;
	    }
	|	LEFT
	    {
		$$ = 3;
	    }
	|	RIGHT
	    {
		$$ = 4;
	    }
	;
amount	:	TERM
	    {
		$$ = 1;
	    }
	|	NUMBER TERM
	    {
		$$ = (int)$1;
	    }
	;
colstat	:	NUMBER TERM
	    {
		$$.colnum = -1;
		$$.number = (int)$1;
	    }
	|	LETTERS NUMBER TERM
	    {
		$$.colnum = colval( foldup( $1 ) );
		$$.number = (int)$2;
		free( $1 );
	    }
	;
%%

char *
getfn()
{
	static char	chrbuf[64];
	char		*tbuf;
	move( 22, 0 );
	printw("Enter File Name:  " );
	refresh();


	tbuf = chrbuf;
	loop {
		*tbuf = getch();
		addch( *tbuf );
		refresh();
		if ( *(tbuf++) == '\n' ) break;
	}
	 *(--tbuf) = '\0';

	return( chrbuf );
}

colval( string )
char	*string;
{
	int	i;
	i = -1;
	while ( *string != '\0' ) {
		i = (i + 1) * 26 + *string - 'A';
		string++;
	}
	return( i );
}
