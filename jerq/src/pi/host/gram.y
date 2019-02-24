%{
#include "gram.h"
#include "expr.pub"
#include "bpts.pub"
#include <ctype.h>
int LexIndex;
int LexGoal;
struct Expr *CurrentExpr;
char *LexString;
char *yyerr;		/* yacc doesn't use this */
void yyerror(char*);
long yyres;
#define TOKEN_SIZE 32
char Token[TOKEN_SIZE];
int DotDot;
Expr *E_IConst(long), *E_DConst(double);
%}

%union {
	char		cc;
	long		ll;
	char		ss[32];
	struct Expr	*ee;
	double		dd;
}

%token G_EXPR G_DOTEQ_CONEX G_DOLEQ_CONEX G_CONEX G_DOTDOT
%token ICONST, ID, PCENT, EQUAL, SLASH, DOLLAR, SIZEOF, TYPEOF, QMARK, SEMI
%token UNOP, STAR, PLUS, MINUS, AMPER, ARROW, DOT, LB, LP, COMMA, ERROR, RB, RP
%token PLUSPLUS, MINUSMINUS, EQUALEQUAL, GREATER, LESS, BAR, BARBAR
%token AMPERAMPER, HAT, TILDE, GREATEREQUAL, LESSEQUAL, FABS
%token GREATERGREATER, LESSLESS, BANG, BANGEQUAL, DCONST, LC, RC, DOTDOT

%type <dd> DCONST
%type <ll> ICONST
%type <ss> ID
%type <ee> expr, list, conex
%type <cc> UNOP, STAR, PLUS, MINUS, AMPER, ARROW, DOT, LB, LP, COMMA, ERROR, SLASH
%type <cc> EQUAL, PCENT, QMARK, SEMI
%type <cc> PLUSPLUS, MINUSMINUS, EQUALEQUAL, GREATER, LESS, BAR, BARBAR
%type <cc> AMPERAMPER, HAT, TILDE, GREATEREQUAL, LESSEQUAL
%type <cc> GREATERGREATER, LESSLESS, BANG, BANGEQUAL, FABS, LC, RC, DOTDOT

%left DOTDOT
%left COMMA
%right EQUAL
%left BARBAR
%left AMPERAMPER
%left BAR
%left HAT
%left AMPER
%left EQUALEQUAL BANGEQUAL
%left LESS GREATER LESSEQUAL GREATEREQUAL
%left LESSLESS GREATERGREATER
%left PLUS MINUS
%left STAR SLASH PCENT
%left SIZEOF TYPEOF BANG TILDE
%right UNOP
%left ARROW DOT LB LP

%%

start:	G_EXPR   { DotDot=0; } expr SEMI	{ yyres = (long) $3;  }
|	G_DOTDOT { DotDot=1; } expr SEMI	{ yyres = (long) $3;  }
|	G_DOTEQ_CONEX DOT EQUAL conex SEMI	{ yyres = (long) $4; }
|	G_DOLEQ_CONEX DOLLAR EQUAL conex SEMI	{ yyres = (long) $4; }
|	G_CONEX conex SEMI			{ yyres = (long) $2; }

expr:	DOLLAR			{ if( !CurrentExpr){
					yyerror("$ cannot be used here");
					YYACCEPT;
				  }
				  $$ = CurrentExpr;
				}
|	LC expr RC ID		{ $$ = E_Binary( $2, O_ENV, E_Id($4) ); }
|	ID			{ $$ = E_Id( $1 ); }
|	DCONST			{ $$ = E_DConst( $1 ); }
|	ICONST			{ $$ = E_IConst( $1 ); }
|	STAR  expr %prec UNOP 	{ $$ = E_Unary( O_DEREF, $2 ); }
|	AMPER expr  		{ $$ = E_Unary( O_REF, $2 ); }
|	SIZEOF expr 		{ $$ = E_Unary( O_SIZEOF, $2 ); }
|	TYPEOF expr 		{ $$ = E_Unary( O_TYPEOF, $2 ); }
|	BANG expr		{ $$ = E_Unary( O_LOGNOT, $2 ); }
|	MINUS expr %prec UNOP	{ $$ = E_Unary( O_MINUS, $2 ); }
|	TILDE expr		{ $$ = E_Unary( O_1SCOMP, $2 ); }
|	expr DOTDOT expr	{ if( !DotDot ){
					yyerror(".. cannot be used here");
					YYACCEPT;
				  }
				  $$ = E_Binary( $1, O_RANGE, $3 );}
|	expr DOT ID		{ $$ = E_Binary( $1, O_DOT, E_Id($3)); }
|	expr COMMA expr		{ $$ = E_Binary( $1, O_COMMA, $3 ); }
|	expr EQUAL expr		{ $$ = E_Binary( $1, O_ASSIGN, $3 ); }
|	expr BARBAR expr	{ $$ = E_Binary( $1, O_LOGOR, $3 ); }
|	expr AMPERAMPER expr	{ $$ = E_Binary( $1, O_LOGAND, $3 ); }
|	expr BAR expr		{ $$ = E_Binary( $1, O_BITOR, $3 ); }
|	expr AMPER expr		{ $$ = E_Binary( $1, O_BITAND, $3 ); }
|	expr HAT expr		{ $$ = E_Binary( $1, O_BITXOR, $3 ); }
|	expr EQUALEQUAL expr	{ $$ = E_Binary( $1, O_EQ, $3 ); }
|	expr BANGEQUAL expr	{ $$ = E_Binary( $1, O_NE, $3 ); }
|	expr LESS expr		{ $$ = E_Binary( $1, O_LT, $3 ); }
|	expr GREATER expr	{ $$ = E_Binary( $1, O_GT, $3 ); }
|	expr LESSEQUAL expr	{ $$ = E_Binary( $1, O_LE, $3 ); }
|	expr GREATEREQUAL expr	{ $$ = E_Binary( $1, O_GE, $3 ); }
|	expr GREATERGREATER expr{ $$ = E_Binary( $1, O_RSHIFT, $3 ); }
|	expr LESSLESS expr	{ $$ = E_Binary( $1, O_LSHIFT, $3 ); }
|	expr MINUS expr		{ $$ = E_Binary( $1, O_MINUS, $3 ); }
|	expr ARROW ID		{ $$ = E_Binary( $1, O_ARROW, E_Id($3)); }
|	expr PCENT expr		{ $$ = E_Binary( $1, O_MOD, $3 ); }
|	expr STAR expr		{ $$ = E_Binary( $1, O_MULT, $3 ); }
|	expr SLASH expr		{ $$ = E_Binary( $1, O_DIV, $3 ); }
|	expr PLUS expr		{ $$ = E_Binary( $1, O_PLUS, $3 ); }
|	expr LB expr RB		{ $$ = E_Binary( $1, O_INDEX, $3 );}
|	ID LP list RP		{ $$ = E_Binary( E_Id($1), O_CALL, $3 ); }
|	ID LP RP 		{ $$ = E_Binary( E_Id($1), O_CALL, 0 ); }
|	LP expr RP		{ $$ = $2; }
|	FABS LP expr RP		{ $$ = E_Unary( O_FABS, $3 ); }

conex:	DOLLAR			{ if( !CurrentExpr){
					yyerror("no current expression for $");
					YYACCEPT;
				  }
				  $$ = CurrentExpr;
				}
|	ID			{ $$ = E_Id( $1 ); }
|	ICONST			{ $$ = E_IConst( $1 ); }
|	AMPER conex  %prec UNOP	{ $$ = E_Unary( O_REF, $2 ); }
|	MINUS conex %prec UNOP	{ $$ = E_Unary( O_MINUS, $2 ); }
|	conex MINUS conex	{ $$ = E_Binary( $1, O_MINUS, $3 ); }
|	conex PCENT conex	{ $$ = E_Binary( $1, O_MOD, $3 ); }
|	conex STAR  conex	{ $$ = E_Binary( $1, O_MULT, $3 ); }
|	conex SLASH conex	{ $$ = E_Binary( $1, O_DIV, $3 ); }
|	conex PLUS  conex	{ $$ = E_Binary( $1, O_PLUS, $3 ); }
|	conex LB conex RB	{ $$ = E_Binary( $1, O_INDEX, $3 );}
|	LP conex RP		{ $$ = $2; }

list:	expr	%prec COMMA	{ $$ = $1; }
|	list COMMA expr		{ $$ = E_Binary( $1, O_COMMA, $3 ); } 

%%

#define LOOK (LexString[LexIndex ])
#define TAKE (AddToken(), LexString[LexIndex++])
#define MORE (LexString[LexIndex+1])
#define yc (yylval.cc)
#define yd (yylval.dd)
#define yl (yylval.ll)
#define ys (yylval.ss)
#define ishex(x) (isdigit(x) || (x>='a'&&x<='f') || (x>='A'&&x<='F'))
#define isoct(x) ( x>='0' && x<='7' )

yylex()
{
	int doyylex(), token = doyylex();

	return token;
}

void AddToken()
{
	int l = strlen(Token);

	if( l < TOKEN_SIZE-1 ){
		Token[l] = LOOK;
		Token[l+1] = '\0';
	}
}

doyylex()
{
	double atof(char*);

	if( LexIndex < 0 ){
		LexIndex = 0;
		return LexGoal;
	}
	while( isspace(LOOK) ) TAKE;
	Token[0] = '\0';
	if( isalpha(LOOK) || LOOK=='_' || LOOK=='$' ){
		TAKE;
		while( isalnum(LOOK) || LOOK=='_' ) TAKE;
		strcpy(ys, Token);
		if( !strcmp(ys,"sizeof") ) return SIZEOF;
		if( !strcmp(ys,"typeof") ) return TYPEOF;
		if( !strcmp(ys,"fabs") ) return FABS;
		if( !strcmp(ys,"$") ) return DOLLAR;
		return ID;
	}
	if( LOOK == '\'' ){
		TAKE;
		if( LOOK == '\\' ){
			TAKE;
			if( MORE != '\'' ) return 0;
			char *trans = "bnftv", *late = "\b\n\f\t\v";
			yl = LOOK;
			for( int i = 0; trans[i]; ++i )
				if( LOOK == trans[i] ) yl = late[i];
			TAKE; TAKE; return ICONST;
		}
		if( MORE != '\'' ) return 0;
		yl = TAKE;
		TAKE;
		return ICONST;
	}
	if( LOOK=='0' && (MORE=='x' || MORE=='X') ){
		TAKE; TAKE;
		if( !ishex(LOOK) ) return 0;
		for( yl = 0; ishex(LOOK); TAKE )
		    yl = (yl<<4) + (isalpha(LOOK) ? (LOOK|' ')+10-'a' : LOOK-'0');
		return ICONST;
	}
	if( LOOK=='0' ){
		for( TAKE, yl = 0; isoct(LOOK); TAKE ) yl = (yl<<3) + LOOK - '0';
		goto IorD;
	}
	if( isdigit(LOOK) ){
		for( yl = 0; isdigit(LOOK); TAKE ) yl = yl*10 + LOOK - '0';
		goto IorD;
	}
	if( LOOK == '.' && isdigit(MORE) ) goto Point;
	if( LOOK=='.' && MORE=='.' ) return (TAKE, TAKE, DOTDOT);
	if( LOOK=='-' && MORE=='>' ) return (TAKE, TAKE, ARROW);
	if( LOOK=='-' && MORE=='-' ) return (TAKE, TAKE, MINUSMINUS);
	if( LOOK=='+' && MORE=='+' ) return (TAKE, TAKE, PLUSPLUS);
	if( LOOK=='=' && MORE=='=' ) return (TAKE, TAKE, EQUALEQUAL);
	if( LOOK=='!' && MORE=='=' ) return (TAKE, TAKE, BANGEQUAL);
	if( LOOK==':' && MORE=='=' ) return (TAKE, TAKE, EQUAL);
	if( LOOK=='>' && MORE=='=' ) return (TAKE, TAKE, GREATEREQUAL);
	if( LOOK=='<' && MORE=='=' ) return (TAKE, TAKE, LESSEQUAL);
	if( LOOK=='&' && MORE=='&' ) return (TAKE, TAKE, AMPERAMPER);
	if( LOOK=='|' && MORE=='|' ) return (TAKE, TAKE, BARBAR);
	if( LOOK=='>' && MORE=='>' ) return (TAKE, TAKE, GREATERGREATER);
	if( LOOK=='<' && MORE=='<' ) return (TAKE, TAKE, LESSLESS);
	switch( TAKE ){
		case '>' : return GREATER;
		case '<' : return LESS;
		case '/' : return SLASH;
		case '*' : return STAR;
		case '+' : return PLUS;
		case '-' : return MINUS;
		case '.' : return DOT;
		case '(' : return LP;
		case ')' : return RP;
		case '[' : return LB;
		case ']' : return RB;
		case '&' : return AMPER;
		case ',' : return COMMA;
		case '%' : return PCENT;
		case '=' : return EQUAL;
		case ';' : return SEMI;
		case '|' : return BAR;
		case '^' : return HAT;
		case '~' : return TILDE;
		case '!' : return BANG;
		case '{' : return LC;
		case '}' : return RC;
		default  : return 0;
	}
IorD:
	if( LOOK=='l' || LOOK=='L' ) return TAKE, ICONST;
	if( LOOK=='.' && MORE=='.' ) return ICONST;
	if( LOOK=='.' ) goto Point;
	if( LOOK=='e' || LOOK=='E' ) goto Exp;
	return ICONST;
Point:
	for( TAKE; isdigit(LOOK); TAKE) {}
	if( LOOK!='e' && LOOK!='E' ) goto Double;
Exp:
	TAKE;
	if( LOOK=='+' || LOOK=='-' ) TAKE;
	if( !isdigit(LOOK) ) return 0;
	while( isdigit(LOOK) ) TAKE;
Double:
	yd = atof(Token);
	return DCONST;	

}
