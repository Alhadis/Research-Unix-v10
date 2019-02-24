%{
#include "picasso.h"
YYSTYPE	y;
%}

%token	<i>	BOX	  1	/* DON'T CHANGE THESE!		*/
%token	<i>	LINE	  2	/* (or at least use care; some	*/
%token	<i>	ARROW	  3	/* code requires real primitves	*/
%token	<i>	CIRCLE	  4	/* to have token value <= TEXT	*/
%token	<i>	ELLIPSE	  5	/* (these have bounding boxes &	*/
%token	<i>	ARC	  6	/* line & color attributes.)	*/
%token	<i>	SECTOR	  7
%token	<i>	SPLINE	  8
%token	<i>	BLOCK	  9
%token	<i>	PSFILE	 10
%token	<p>	TEXT	 11
%token	<i>	TROFF	 12
%token	<i>	MOVE	 13
%token	<i>	BLOCKEND 14
%token	<i>	PLACE	 15
%token	<i>	OBJECT	 16
%token	<i>	TEXTOBJ	 17
%token	<i>	PRINT RESET SHOW THRU UNTIL
%token	<o>	FOR IF COPY
%token	<o>	DUP DELETE
%token	<p>	THENSTR ELSESTR DOSTR DEFNAME PLACENAME VARNAME SPRINTF
%token	<i>	ATTR TEXTATTR LAYER COLOR LCOLOR PCOLOR TCOLOR LWEIGHT NOEDGE
%token	<i>	LEFT RIGHT UP DOWN FONT SIZE SPACE
%token	<p>	TEXTVAR
%token	<i>	FROM TO AT BY WITH HEAD CW CCW THEN
%token	<i>	HEIGHT WIDTH RADIUS DIAMETER LENGTH LOCUS XLIST YLIST
%token	<i>	CORNER HERE LAST NTH DOTNTH SAME BETWEEN AND
%token	<i>	EAST WEST NORTH SOUTH NE NW SE SW START END
%token	<i>	TRANSFORM TRANSLATE ROTATE SCALE REFLECT
%token	<i>	DOTX DOTY DOTHT DOTWID DOTRAD
%token	<f>	NUMBER
%token	<f>	ABS LOG LOG10 EXP POW SIN COS ATAN2 SQRT
%token	<f>	RAND MIN MAX INT RGB SETFONT
%token	<i>	DIR
%token	<i>	DOT DASH DASHPAT CHOP
%token	<o>	ST	/* statement terminator */

%right	<f>	'='
%left	<f>	OROR
%left	<f>	ANDAND
%nonassoc <f>	GT LT LE GE EQ NEQ
%left	<f>	'+' '-'
%left	<f>	'*' '/' '%'
%right	<f>	UMINUS NOT

%type	<f>	expr if_expr asgn
%type	<p>	text
%type	<i>	atto optby optop exprlist
%type	<o>	if for copy

%type	<o>	leftbrace picture piclist position lbracket
%type	<o>	prim place objname blockname show transform dup
%type	<i>	textlist textattr	/* not a sensible value */
%type	<i>	last type

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
	  prim ST			{ codegen = 1; makeiattr(0, 0); }
	| leftbrace piclist '}'		{ rightthing($1, '}'); $$ = $2; }
	| PLACENAME ':' picture		{ $$=y.o=$3; makevar($1,PLACENAME,y); }
	| PLACENAME ':' ST picture	{ $$=y.o=$4; makevar($1,PLACENAME,y); }
	| PLACENAME ':' position ST	{ $$=y.o=$3; makevar($1,PLACENAME,y); }
	| asgn ST			{ y.f = $1; /* ??? */ $$ = y.o; }
	| dup ST
	| transform ST
	| DIR				{ setdir($1); }
	| PRINT expr ST			{ printexpr($2); }
	| PRINT position ST		{ printpos($2); }
	| PRINT text ST			{ fprintf(stderr,"%s\n",$2);free($2); }
	| RESET varlist ST		{ resetvar(); makeiattr(0, 0); }
	| DELETE objname ST		{ freenode($2); }
	| show ST
	| copy ST
	| for
	| if
	| ST
	;

varlist:
	  /* empty */
	| VARNAME			{ makevattr($1); }
	| varlist VARNAME		{ makevattr($2); }
	| varlist ',' VARNAME		{ makevattr($3); }
	;

dup:
	  DUP objname atto '(' VARNAME ',' VARNAME ')' { $$=copyobj($2,$5,$7); }
	| DUP objname atto position			{ $$ = copypos($2,$4); }
	| DUP objname					{ $$ = copypos($2,$2); }

copy:
	  COPY copylist					{/* lexical */ copy(); }
	;

atto:
	  AT
	| TO
	;

copylist:
	  copyattr
	| copylist copyattr
	;
copyattr:
	  text			{ copyfile($1); }
	| THRU DEFNAME		{ copydef($2); }
	| UNTIL text		{ copyuntil($2); }
	;

show:
	  SHOW expr FROM position TO position
				{ if (batch) print_layer_bnd($2, $4, $6); }
	| SHOW FROM position TO position
				{ if (batch) print_bnd($3, $5); }
	| SHOW objname		{ if (batch) print_obj($2); }
	| SHOW expr		{ if (batch) print_layer($2); }
	| SHOW			{ if (batch) print(0); }

for:
	  FOR VARNAME FROM expr TO expr BY optop expr DOSTR
		{ forloop($2, $4, $6, $8, $9, $10); }
	| FOR VARNAME FROM expr TO expr DOSTR
		{ forloop($2, $4, $6, '+', 1.0, $7); }
	| FOR VARNAME '=' expr TO expr BY optop expr DOSTR
		{ forloop($2, $4, $6, $8, $9, $10); }
	| FOR VARNAME '=' expr TO expr DOSTR
		{ forloop($2, $4, $6, '+', 1.0, $7); }
	;

if:
	  IF if_expr THENSTR ELSESTR	{ ifstat($2, $3, $4); }
	| IF if_expr THENSTR		{ ifstat($2, $3, (char *) 0); }
	;
if_expr:
	  expr
	| text EQ text		{ $$ = strcmp($1,$3) == 0; free($1); free($3); }
	| text NEQ text		{ $$ = strcmp($1,$3) != 0; free($1); free($3); }
	;

optop:
	  '+'		{ $$ = '+'; }
	| '-'		{ $$ = '-'; }
	| '*'		{ $$ = '*'; }
	| '/'		{ $$ = '/'; }
	| /* empty */	{ $$ = ' '; }
	;


leftbrace:
	  '{'			{ $$ = leftthing('{'); }
	;

prim:
	  BOX attrlist		{ $$ = boxgen(); }
	| CIRCLE attrlist	{ $$ = circgen($1); }
	| ELLIPSE attrlist	{ $$ = circgen($1); }
	| ARC attrlist		{ $$ = arcgen($1); }
	| SECTOR attrlist	{ $$ = arcgen($1); }
	| LINE attrlist		{ $$ = linegen($1); }
	| ARROW attrlist	{ $$ = linegen($1); }
	| SPLINE attrlist	{ $$ = linegen($1); }
	| MOVE attrlist		{ $$ = movegen(); }
	| PSFILE attrlist	{ $$ = picgen(); }
	| textlist attrlist	{ $$ = textgen(); }
	| TROFF			{ $$ = troffgen($1); }
	| lbracket piclist ']' { $<o>$=rightthing($1,']'); } attrlist
				{ $$ = blockgen($1); }
	;

lbracket:
	  '['			{ $$ = leftthing('['); }
	;

attrlist:
	  attrlist attr
	| /* empty */
	;

attr:
	  ATTR expr		{ makefattr($1, !DEFAULT, $2); }
	| ATTR			{ makefattr($1, DEFAULT, 0.0); }
	| COLOR expr		{ makefattr($1, !DEFAULT, checkcolor($2)); }
	| COLOR			{ makefattr($1, DEFAULT, 0.0); }
	| expr			{ makefattr(curdir(), !DEFAULT, $1); }
	| DIR expr		{ makefattr($1, !DEFAULT, $2); }
	| DIR			{ makefattr($1, DEFAULT, 0.0); }
	| FROM position				{ makeoattr($1, $2); }
	| LOCUS '(' VARNAME ',' VARNAME ')'	{ makelattr($3, $5); }
	| TO position				{ makeoattr($1, $2); }
	| AT position				{ makeoattr($1, $2); }
	| BY position				{ makeoattr($1, $2); }
	| WITH CORNER				{ makeiattr(WITH, $2); }
	| WITH DOTNTH				{ makeiattr(WITH, $2); }
	| WITH '.' PLACENAME
		{ makeoattr(PLACE,getblock(getlast(1,BLOCK),$3)); }
	| WITH '.' PLACENAME CORNER
		{ makeoattr(PLACE,getpos(getblock(getlast(1,BLOCK),$3),$4)); }
	| WITH position		{ makeoattr(PLACE, $2); /* ??? */ }
	| SAME			{ makeiattr(SAME, $1); }
	| FONT expr		{ makefattr($1, !DEFAULT, checkfont($2)); } 
	| FONT TEXT		{ makefattr($1, !DEFAULT, setfont($2)); } 
	| SIZE optop expr	{ makefattr($1, $2, $3); }
	| SPACE optop expr	{ makefattr($1, $2, $3); }
	| TEXTATTR		{ maketattr($1, (char *) 0); }
	| HEAD			{ makeiattr(HEAD, $1); }
	| CHOP			{ makefattr(CHOP, DEFAULT, -1.0); }
	| CHOP expr		{ makefattr(CHOP, !DEFAULT, $2); }
	| DOT			{ makefattr(DOT, DEFAULT, 0.0); }
	| DOT expr		{ makefattr(DOT, !DEFAULT, $2); }
	| DASH			{ makefattr(DASH, DEFAULT, 0.0); }
	| DASH expr		{ makefattr(DASH, !DEFAULT, $2); }
	| DASHPAT VARNAME		{ makedattr($2); }
	| DASHPAT '(' exprlist ')'	{ makedattr((char *)0); }
	| textlist
	;

textlist:
	  textattr
	| textlist textattr
	;
textattr:
	  text			{ maketattr(DEFAULT, $1); }
	| text TEXTATTR		{ maketattr($2, $1); }
	| textattr TEXTATTR	{ addtattr($2); }
	;
text:
	  TEXT
	| SPRINTF '(' text ')'			{ $$ = sprintgen($3); }
	| SPRINTF '(' text ',' exprlist ')'	{ $$ = sprintgen($3); }
	;

exprlist:
	  '(' exprlist ')'	{ $$ = $2; }
	| expr			{ $$ = exprsave($1); }
	| exprlist  expr	{ $$ = exprsave($2); }
	| exprlist ',' expr	{ $$ = exprsave($3); }
	;

position:		/* absolute, not relative */
	  place
	| '(' position ')'			{ $$ = $2; }
	| expr ',' expr				{ $$ = makepos($1, $3,
								0, (obj *)0); }
	| position '+' expr ',' expr		{ $$ = fixpos($1,  $3,  $5);  }
	| position '-' expr ',' expr		{ $$ = fixpos($1, -$3, -$5);  }
	| position '+' '(' expr ',' expr ')'	{ $$ = fixpos($1,  $4,  $6);  }
	| position '-' '(' expr ',' expr ')'	{ $$ = fixpos($1, -$4, -$6);  }
	| position '+' place			{ $$ = addpos($1, $3); }
	| position '-' place			{ $$ = subpos($1, $3); }
	| '(' place ',' place ')'
			{ $$ = makepos(getcomp($2,DOTX), getcomp($4,DOTY),
								0, (obj *)0); }
	| expr LT position ',' position GT	{ $$ = makebetween($1,$3,$5); }
	| expr BETWEEN position AND position	{ $$ = makebetween($1,$3,$5); }
	;

place:
	  objname		{ $$ = getpos($1, -1); }
	| objname CORNER	{ $$ = getpos($1, $2); }
	| CORNER objname	{ $$ = getpos($2, $1); }
	| objname DOTNTH	{ $$ = getnth($1, $2); }
	| HERE			{ $$ = gethere(); }
	;

objname:
	  PLACENAME		{ y = getvar($1); $$ = y.o; }
	| last type		{ $$ = getlast($1, $2); }
	| NTH type		{ $$ = getfirst($1, $2); }
	| blockname
	;

blockname:
	  last BLOCK '.' PLACENAME	{ $$ = getblock(getlast($1,$2), $4); }
	| NTH BLOCK '.' PLACENAME	{ $$ = getblock(getfirst($1,$2), $4); }
	| PLACENAME '.' PLACENAME	{ y=getvar($1); $$=getblock(y.o, $3); }
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
	| SECTOR
	| LINE
	| ARROW
	| SPLINE
	| BLOCK
	| OBJECT
	| PSFILE
	| TEXTOBJ
	;

transform:
	  TRANSLATE place optby position
				{ $$=$2; xlate($2, $4); }
	| MOVE place TO position
				{ $$=$2; xlate_to($2, $4); }
	| TRANSLATE optby position
				{ y=getvar("O"); $$=y.o; xlate(y.o,$3); }
	| SCALE '=' expr	{ rescale($3);   }
	| SCALE place optby exprlist
				{ $$=$2; scale($2, $4, $2); }
	| SCALE optby exprlist
				{ y=getvar("O"); $$=y.o; scale(y.o,$3,NULL); }
	| SCALE place optby exprlist AT position
				{ $$=$2; scale($2, $4, $6); }
	| SCALE optby exprlist AT position
				{ y=getvar("O"); $$=y.o; scale(y.o,$3,$5); }
	| ROTATE place optby expr
				{ $$=$2; rotate($2, $4, $2); }
	| ROTATE optby expr
				{ y=getvar("O"); $$=y.o; rotate(y.o,$3,NULL); }
	| ROTATE place optby expr AT position
				{ $$=$2; rotate($2, $4, $6); }
	| ROTATE optby expr AT position
				{ y=getvar("O"); $$=y.o; rotate(y.o,$3,$5); }
	| REFLECT place
				{ $$=$2; reflect($2,$2); }
	| REFLECT place AT position
				{ $$=$2; reflect($2,$4); }
	| REFLECT AT position
				{ y=getvar("O"); $$=y.o; reflect(y.o,$3); }
	| TRANSFORM place optby exprlist
				{ $$=$2; xform($2,$4); }
	| TRANSFORM optby exprlist
				{ y=getvar("O"); $$=y.o; xform(y.o,$3); }
	;

optby:
	  BY			{ $$ = BY; }
	| 			{ $$ = ' '; }
	;

asgn:
	  VARNAME '=' exprlist			{ $$ = setarray($1); }
	| VARNAME DOTNTH '=' expr		{ $$ = setvar($1,$2,$4); }
	| TEXTVAR '=' expr			{ $$ = setsize($1,$3); }
/*	| ATTR '(' objname ')' '=' expr		{ $$ = setattr($3,$1,$6);} */
/*	| COLOR '(' objname ')' '=' expr	{ $$ = setattr($3,$1,$6);} */
	;

expr:
	  NUMBER
	| VARNAME DOTNTH	{ $$ = getsub($1,$2); free($1); }
	| VARNAME		{ $$ = getfval($1); free($1); }
	| TEXTVAR		{ $$ = getfval($1); free($1); }
	| asgn
	| expr '+' expr		{ $$ = $1 + $3; }
	| expr '-' expr		{ $$ = $1 - $3; }
	| expr '*' expr		{ $$ = $1 * $3; }
	| expr '/' expr		{ if ($3 == 0.0) {
					yyerror("division by 0"); $3 = 1; }
				  $$ = $1 / $3; }
	| expr '%' expr		{ if ((long)$3 == 0) {
					yyerror("mod division by 0"); $3 = 1; }
				  $$ = (long)$1 % (long)$3; }
	| '-' expr %prec UMINUS	{ $$ = -$2; }
	| '(' expr ')'		{ $$ = $2; }
	| place DOTX		{ $$ = getcomp($1, $2); }
	| place DOTY		{ $$ = getcomp($1, $2); }
	| place DOTHT		{ $$ = getcomp($1, $2); }
	| place DOTWID		{ $$ = getcomp($1, $2); }
	| place DOTRAD		{ $$ = getcomp($1, $2) / 2.0; }
	| PLACENAME '.' VARNAME	{ y = getvar($1); $$ = getblkvar(y.o, $3); }
	| last BLOCK '.' VARNAME { $$ = getblkvar(getlast($1,$2), $4); }
	| NTH BLOCK '.' VARNAME	{ $$ = getblkvar(getfirst($1,$2), $4); }
	| expr GT expr		{ $$ = $1 > $3; }
	| expr LT expr		{ $$ = $1 < $3; }
	| expr LE expr		{ $$ = $1 <= $3; }
	| expr GE expr		{ $$ = $1 >= $3; }
	| expr EQ expr		{ $$ = $1 == $3; }
	| expr NEQ expr		{ $$ = $1 != $3; }
	| expr ANDAND expr	{ $$ = $1 && $3; }
	| expr OROR expr	{ $$ = $1 || $3; }
	| NOT expr		{ $$ = !($2); }
	| ABS '(' expr ')'		{ $$ = fabs($3); }
	| LOG '(' expr ')'		{ $$ = Log($3); }
	| LOG10 '(' expr ')'		{ $$ = Log10($3); }
	| EXP '(' expr ')'		{ $$ = Exp($3); }
	| SIN '(' expr ')'		{ $$ = Sin($3); }
	| COS '(' expr ')'		{ $$ = Cos($3); }
	| POW '(' expr ',' expr ')'	{ $$ = Pow($3, $5); }
	| ATAN2 '(' expr ',' expr ')'	{ $$ = Atan2($3, $5); }
	| SQRT '(' expr ')'		{ $$ = Sqrt($3); }
	| RAND '(' ')'			{ $$ = (float)rand() / 32767.0; /* might be 2^31-1 */ }
	| MAX '(' VARNAME ')'		{ $$ = Maxvar($3); }
	| MAX '(' exprlist ')'		{ $$ = Maxlist(); }
	| MIN '(' VARNAME ')'		{ $$ = Minvar($3); }
	| MIN '(' exprlist ')'		{ $$ = Minlist(); }
	| INT '(' expr ')'		{ $$ = (long) $3; }
	| RGB '(' exprlist ')'		{ $$ = setrgbindex(); }
	| SETFONT '(' TEXT ')'		{ $$ = setfont($3); }
	;
