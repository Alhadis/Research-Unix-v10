%token	BOX	1
%token	ARROW	2
%token	CIRCLE	3
%token	ARC	4
%token	ELLIPSE	5
%token	LINE	6
%token	MOVE	7
%token	TEXT	8
%token	TROFF	9
%token	SPLINE	10
%token	BLOCK	11
%token	BLOCKEND 12
%token	PRINT
%token	PLACE
%token	ATTR
%token	SPREAD FILL LJUST RJUST ABOVE BELOW
%token	LEFT RIGHT UP DOWN FROM TO AT BY WITH HEAD CW CCW THEN
%token	HEIGHT WIDTH RADIUS DIAMETER LENGTH SIZE
%token	PLACENAME VARNAME DEFNAME CORNER HERE LAST NTH SAME BETWEEN AND
%token	EAST WEST NORTH SOUTH NE NW SE SW CENTER START END
%token	DOTX DOTY DOTHT DOTWID DOTRAD
%token	NUMBER
%token	DIR
%token	DOT DASH CHOP
%token	ST	/* statement terminator */

%left	'+' '-'
%left	'*' '/' '%'
%right	UMINUS

%{
#include	<stdio.h>
extern	int	hvmode, codegen;
extern	float	between, lastfloat;
%}

%%

top:
	  piclist
	| /* empty */
	| error		{ yyerror("syntax error"); }
	;

piclist:
	  picture
	| piclist picture
	;

picture:
	  prim ST			{ codegen = 1; }
	| leftbrace piclist '}'		{ rightthing($1, '}'); $$ = $2; }
	| PLACENAME ':' picture		{ makevar($1, PLACENAME, $3); $$ = $3; }
	| PLACENAME ':' ST picture	{ makevar($1, PLACENAME, $4); $$ = $4; }
	| PLACENAME ':' position ST	{ makevar($1, PLACENAME, $3); $$ = $3; }
        | VARNAME '=' expr ST		{ makevar($1, VARNAME, $3); $$ = $1; }
	| DIR				{ setdir($1); }
	| PRINT expr ST			{ printexpr($2); }
	| PRINT position ST		{ printpos($2); }
	| ST
	;

leftbrace:
	  '{'			{ leftthing('{'); }
	;

prim:
	  BOX attrlist		{ $$ = boxgen($1); }
	| CIRCLE attrlist	{ $$ = circgen($1); }
	| ELLIPSE attrlist	{ $$ = circgen($1); }
	| ARC attrlist		{ $$ = arcgen($1); }
	| LINE attrlist		{ $$ = linegen($1); }
	| ARROW attrlist	{ $$ = linegen($1); }
	| SPLINE attrlist	{ $$ = splinegen($1); }
	| MOVE attrlist		{ $$ = movegen($1); }
	| TEXT attrlist		{ $$ = textgen($1); }
	| TROFF			{ $$ = troffgen($1); }
	| lbracket piclist ']' { $$=rightthing($1,']'); } attrlist
				{ $$ = blockgen($1, $2, $4); }
	;

lbracket:
	  '['			{ $$ = leftthing('['); }
	;

attrlist:
	  attrlist attr
	| /* empty */		{ makeattr(0, 0); }
	;

attr:
	  ATTR opt_expr		{ makeattr($1, $2); }
	| DIR opt_expr		{ makeattr($1, $2); }
	| FROM position		{ makeattr($1, $2); }
	| TO position		{ makeattr($1, $2); }
	| AT position		{ makeattr($1, $2); }
	| BY position		{ makeattr($1, $2); }
	| WITH CORNER		{ makeattr(WITH, $2); }
	| WITH '.' PLACENAME	{ makeattr(PLACE, getblock(getlast(1,BLOCK), $3)); }
	| WITH position		{ makeattr(PLACE, $2); }
	| SAME			{ makeattr(SAME, $1); }
	| textattr		{ makeattr($1, 0); }
	| HEAD			{ makeattr(HEAD, $1); }
	| DOT opt_expr		{ makeattr(DOT, $2); }
	| DASH opt_expr		{ makeattr(DASH, $2); }
	| CHOP opt_expr		{ makeattr(CHOP, $2); }
	| textlist
	;

opt_expr:
	  expr
	| /* empty */		{ $$ = 0; }
	;

textlist:
	  TEXT			{ makeattr(CENTER, $1); }
	| TEXT textattr		{ makeattr($2, $1); }
	| textlist TEXT		{ makeattr(CENTER, $2); }
	| textlist TEXT textattr { makeattr($3, $2); }
	;

textattr:
	  LJUST
	| RJUST
	| SPREAD
	| FILL
	| CENTER
	| ABOVE
	| BELOW
	;

position:		/* absolute, not relative */
	| place
	| expr ',' expr			{ $$ = makepos($1, $3); }
	| position '+' expr ',' expr	{ $$ = fixpos($1, $3, $5); }
	| position '-' expr ',' expr	{ $$ = fixpos($1, -$3, -$5); }
	| '(' expr ',' expr ')'			{ $$ = makepos($2, $4); }
	| position '+' '(' expr ',' expr ')'	{ $$ = fixpos($1, $4, $6); }
	| position '-' '(' expr ',' expr ')'	{ $$ = fixpos($1, -$4, -$6); }
	| '(' place ',' place ')'	{ $$ = makepos(getcomp($2,DOTX), getcomp($4,DOTY)); }
	| expr { between=lastfloat; } '<' position ',' position '>'
					{ $$ = makebetween($1, $4, $6); }
	| expr { between=lastfloat; } BETWEEN position AND position
					{ $$ = makebetween($1, $4, $6); }
	;

place:
	  PLACENAME		{ $$ = getpos(getvar($1), 0); }
	| PLACENAME CORNER	{ $$ = getpos(getvar($1), $2); }
	| CORNER PLACENAME	{ $$ = getpos(getvar($2), $1); }
	| HERE			{ $$ = gethere($1); }
	| last type		{ $$ = getlast($1, $2); }
	| last type CORNER	{ $$ = getpos(getlast($1, $2), $3); }
	| CORNER last type	{ $$ = getpos(getlast($2, $3), $1); }
	| NTH type		{ $$ = getfirst($1, $2); }
	| NTH type CORNER	{ $$ = getpos(getfirst($1, $2), $3); }
	| CORNER NTH type	{ $$ = getpos(getfirst($2, $3), $1); }
	| blockname
	| blockname CORNER	{ $$ = getpos($1, $2); }
	| CORNER blockname	{ $$ = getpos($2, $1); }
	;

blockname:
	  last BLOCK '.' PLACENAME	{ $$ = getblock(getlast($1,$2), $4); }
	| NTH BLOCK '.' PLACENAME	{ $$ = getblock(getfirst($1,$2), $4); }
	| PLACENAME '.' PLACENAME	{ $$ = getblock(getvar($1), $3); }
	;

last:
	  last LAST		{ $$ = $1 + 1; }
	| NTH LAST		{ $$ = $1; }
	| LAST			{ $$ = 1; }
	;

type:
	  BOX
	| CIRCLE
	| ELLIPSE
	| ARC
	| LINE
	| ARROW
	| SPLINE
	| BLOCK
	;

expr:
	  expr '+' expr		{ $$ = $1 + $3; }
	| expr '-' expr		{ $$ = $1 - $3; }
	| expr '*' expr		{ $$ = $1 * $3; }
	| expr '/' expr		{ $$ = $1 / $3; }
	| expr '%' expr		{ $$ = $1 % $3; }
	| '-' expr %prec UMINUS	{ $$ = -$2; }
	| '(' expr ')'		{ $$ = $2; }
	| VARNAME		{ $$ = getvar($1); }
	| NUMBER
	| place DOTX		{ $$ = getcomp($1, $2); }
	| place DOTY		{ $$ = getcomp($1, $2); }
	| place DOTHT		{ $$ = getcomp($1, $2); }
	| place DOTWID		{ $$ = getcomp($1, $2); }
	| place DOTRAD		{ $$ = getcomp($1, $2); }
	;
