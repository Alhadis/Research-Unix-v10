%{
/*
 *      lex.l 1.3
 *
 *	Lexical Analyzer Description for Spreadsheet Program `vis'
 *
 *      A. F. Gettier
 *      Bell Laboratories
 *      Update made 11/1/82 11:12:08
 *      Retrieved 11/15/82 13:22:28
 */
#include	<stdio.h>
#include	<ctype.h>
#include	"vis.h"
#include	"y.tab.h"
#undef	input()
#undef	unput(c)
char	input();
extern int	Inrow, Incol;
extern char	Inline[];
%}

D	[0-9]
L	[A-Za-z]
A	[A-Za-z0-9]

%%
[ \t]	;
\"[^\"]*\"	{
	yytext[yyleng-1] = '\0';
	yylval.sval = copystr( yytext + 1 );
	return( STR );
}
\'[^\']*\'	{
	yytext[yyleng-1] = '\0';
	yylval.sval = copystr( yytext + 1 );
	return( STR );
}
^[ \t]*{L}+{D}+	{
	int	i;
	(void)foldup( yytext );
	yylval.vval.col = -1;
	i = 0;
	while ( yytext[i] == ' ' || yytext[i] == '\t' ) i++;
	while ( ! isdigit( yytext[i] ) ) {
		yylval.vval.col = ( yylval.vval.col + 1 ) * 26
		    + yytext[i] - 'A';
		i++;
	}
	yylval.vval.row = atoi( &(yytext[i]) ) - 1;
	Inrow = yylval.vval.row;
	Incol = yylval.vval.col;
	yylval.vval.tval = copystr( Inline );
	return( AVARIABLE );
}
{L}+{D}+	{
	int	i;
	(void)foldup( yytext );
	yylval.nval.col = -1;
	i = 0;
	while ( ! isdigit( yytext[i] ) ) {
		yylval.nval.col = ( yylval.nval.col + 1 ) * 26
		    + yytext[i] - 'A';
		i++;
	}
	yylval.nval.row = atoi( &(yytext[i]) ) - 1;
	return( VARIABLE );
}
{L}+	{
	char	tbuf[128];
	int	tval;
	(void)strcpy( tbuf, yytext );
	(void)foldup( tbuf );
	tval = hashsearch( tbuf );
	switch( tval ) {
	/*
	 *	Numbers
	 */
	case PI:
		yylval.dval = 3.14159265358979;
		return( NUMBER );
	/*
	 *	Functions
	 */
	case ABS:
	case ACOS:
	case ASIN:
	case ATAN:
	case ATAN2:
	case COS:
	case EXP:
	case GAMMA:
	case HYPOT:
	case INT:
	case LOG:
	case POW:
	case SIN:
	case SQRT:
		yylval.ival = tval;
		return( FUNC );
	/*
	 *	Actual Tokens
	 */
	case AT:
	case COL:
	case DEBUG:
	case DOWN:
	case DUP:
	case DUPLICATE:
	case EDIT:
	case HELP:
	case LEFT:
	case LIST:
	case POSITION:
	case REDRAW:
	case REFRESH:
	case REP:
	case REPLICATE:
	case RIGHT:
	case ROW:
	case SCALE:
	case SHIFT:
	case SLIDE:
	case SHELL:
	case SH:
	case THRU:
	case UP:
	case VER:
	case WIDTH:
	case ZERO:
	case QUIT:
		return( tval );
	case COPY:
	case READ:
	case WRITE:
		yylval.sval = collect();
		return( tval );
	}
	yylval.sval = copystr( yytext );
	return( LETTERS );
}
{D}*\.?{D}*[eE][\+\-]?{D}+	{ yylval.dval = atof(yytext);return(NUMBER); }
{D}*\.?{D}*	{ yylval.dval = atof(yytext);return(NUMBER); }
"="	return( '=' );
","	return( ',' );
"["	return( '[' );
"]"	return( ']' );
"("	return( '(' );
")"	return( ')' );
"+"	return( '+' );
"%"	return( '%' );
"-"	return( '-' );
"**"	return( EXPON );
"^"	return( EXPON );
"*"	return( '*' );
"/"	return( '/' );
"\n"	return( TERM );
.	return( ERROR );
%%
