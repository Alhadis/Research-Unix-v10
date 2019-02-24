%{
#include <stdio.h>
#include "pico.h"
#include "pico2.h"

#define usedboth	usedold=usednew=1

#define YYDEBUG	1

extern short CURSCRATCH, CUROLD;
extern char *progr;
extern struct SRC src[MANY];
extern int nsrc, nrparams, LO, linenumber, whichdevice;
extern char usednew, usedold, faster, frameb, metheus;
extern char isglobal, seetree, optim, touchedcmap;
extern FILE *INPUT;
%}

%union{
	Node	*node;
	Symbol	*sym;
	int	resu;
	struct SNode *snode;
}

%type	<node>	stmnt sstmnt pprog pbody rval assign ifpre whpre
%type	<node>	aparams fcall elist expr chann
%type	<snode>	iarray lval
%type	<resu>	array SPEC eval vval arname
%type	<sym>	ANAME XYI pathn defpr
%token	<sym>	VARIABLE AUTO ARRAY FCT BLTN NAME
%token	<resu>	VAL STRING

%start		comm
%token		A WINDOW C DEF DEL SCOM ECOM
%token		UP DOWN RUN F H Q R SET SETF
%token		GET W PCONST RETURN
%token		OPEN CLOSE DECL INT GLOBAL
%token		X Y I OLD TMP CMAP
%token		FOR DO WHILE JUMP IF ELSE
%token		COLOR NOCOLOR ZAP FASTER SLOWER HELP
%token		FRAMEBUFFER METHEUS BLIND SEETREE OPTIM
%right		','
%right		ASSIGN
%right		'?' ':' ';'
%right		OPER
%left		OROR
%left		ANDAND
%left		OR
%left		'^'
%left		AND
%left		EQ NE
%left		GT GE LT LE
%left		LSH RSH
%left		'+' '-'
%left		'*' '/' '%'
%right		POST
%right		UNARYMINUS NOT '$' '~'
%right		RCHAN GCHAN BCHAN BW RGB
%right		POW
%right		'.'

%%
comm:	pcom ';'		{ return 1; }
|	RUN SPEC		{ munge($2); return 1; }
|	defpr YoN SPEC		{ postfunct($1); return 1; }
|	R pathn ';'		{ redirect($2->name);
				  checkit();
				  return 1;
				}
|	Q ';'			{ return 0; }

defpr:	DEF NAME '(' rglst ')' 	{ prefunct($2, nrparams); $$ = $2; }
|	DEF FCT	 '(' rglst ')' 	{ prefunct($2, nrparams); $$ = $2; }

pcom:	/* empty */		{ /* if (INPUT == stdin) linenumber--; */ }
|	A pathn			{ append( 0,  0,  0,  0, $2->name); }
|	A vval vval vval vval pathn { append($2, $3, $4, $5, $6->name); }
|	DEL pathn		{ discard($2->name); }
|	DEL array	 	{ if ($2 > 1 && $2 <= nsrc-1)
				  	discard(src[$2].fname);
				  else
					yyerror("unknown file number %d", $2);
				}
|	DEL VAL		 	{ if ($2 > 1 && $2 <= nsrc-1)
				  	discard(src[$2].fname);
				  else
					yyerror("unknown file number %d", $2);
				}
|	F			{ files(); }
|	GET			{ getscreen(Old); }
|	GET arname		{ getter($2); }
|	GET pathn		{ getter(append( 0,  0,  0,  0, $2->name)); }
|	GET vval vval vval vval pathn { getter(append($2, $3, $4, $5, $6->name)); }
|	H hnmlst
|	FASTER			{ faster = 1; }
|	SLOWER			{ faster = 0; }
|	SEETREE			{ seetree = 1 - seetree; }
|	OPTIM			{ optim = 1 - optim; }
|	NOCOLOR			{ Old->nchan = Scratch->nchan = 1; }
|	COLOR			{ Old->nchan = Scratch->nchan = 3;
				  checkpix(Old); checkpix(Scratch);
				}
|	HELP			{ dohelp(); }
|	SET			{ showvars(); }
|	SET ANAME		{ showsym($2); }
|	SETF			{ showfcts(); }
|	WINDOW vval vval vval vval  { setwindow($2, $3, $4, $5); }
|	W pathn			{ putdpix($2->name, PICO); }
|	W '-' pathn		{ putdpix($3->name, 0); }

arname:	array			{ $$ = $1; }
|	vval vval array		{ $$ = append($1, $2,  0,  0, src[$3].fname); }
|	vval vval vval vval array { $$ = append($1, $2, $3, $4, src[$5].fname); }

hnmlst:	pathn			{ interpret($1->name); }
|	hnmlst pathn		{ interpret($2->name); }

rglst:	/* empty */
|	arglst

arglst:	NAME			{ automa($1, nrparams++); }
|	arglst ',' NAME		{ automa($3, nrparams++); }

SPEC:	sstmnt			{ compile($1, progr); $$ = BLOOP; }
|	chann ';'		{ compile($1, progr); $$ = SLOOP; }
|	pprog ';'		{ compile($1, progr); $$ = NLOOP; }

chann:	CMAP ASSIGN expr		{ $$ = mapall(cast($3)); }
|	CMAP '.' RGB   ASSIGN expr	{ $$ = mapall($5); }
|	CMAP '.' RCHAN ASSIGN expr	{ $$ = mapred(cast($5)); }
|	CMAP '.' GCHAN ASSIGN expr	{ $$ = mapgrn(cast($5)); }
|	CMAP '.' BCHAN ASSIGN expr	{ $$ = mapblu(cast($5)); }

YoN:	/* empty */
|	';'

decls:	glob INT declst ';'
|	glob DECL decars ';'

glob:	GLOBAL			{ isglobal = 1; }
|	/* none */		{ isglobal = 0; }

declst:	declo
|	declst ',' declo

decars:	decar
|	decars ',' decar

declo:	XYI
|	NAME			{ decvar($1, 0); }
|	VARIABLE		{ decvar($1, 0); }
|	NAME ASSIGN eval	{ decvar($1, $3); }
|	VARIABLE ASSIGN eval	{ decvar($1, $3); }

decar:	NAME  '[' eval ']'	{ decarray($1, $3); }
|	ARRAY '[' eval ']'	{ decarray($1, $3); }

XYI:	X			{ $$ = newsym("x"); }
|	Y			{ $$ = newsym("y"); }
|	I			{ $$ = newsym("i"); }

pprog:	OPEN pbody YoN CLOSE	{ $$ = $2; }

pbody:	stmnt			{ $$ = $1; }
|	pbody stmnt		{ $$ = new(OCOMMA, $1, $2, Z); }

aparams: '(' ')'		{ $$ =  Z; }
|	 '(' elist ')'		{ $$ = $2; }

elist:	expr			{ $$ = cast($1); }
|	expr ',' elist		{ $$ = new(ACOMMA, cast($1), $3, Z); }

ifpre:	IF '(' expr ')' YoN	{ $$ = $3; }
whpre:	WHILE '(' expr ')' YoN	{ $$ = $3; }

ANAME:	NAME			{ $$ = $1; }
|	VARIABLE		{ $$ = $1; }
|	FCT			{ $$ = $1; }
|	BLTN			{ $$ = $1; }
|	AUTO			{ $$ = $1; }
|	ARRAY			{ $$ = $1; }
|	XYI			{ $$ = $1; }
|	STRING			{ $$ = lookup($1); }

pathn:	ANAME			{ $$ = $1; }

stmnt:	pprog ';'		{ $$ = $1; }
|	ANAME ':' stmnt		{ $$ = new(LABL,$3, Z, $1); }
|	JUMP ANAME ';'		{ $$ = new(GOTO, Z, Z, $2); }
|	sstmnt			{ $$ = $1; }
|	chann ';'		{ $$ = $1; }

sstmnt:	assign ';'		{ $$ = $1; }
|	decls			{ $$ = Z; }
|	fcall ';'		{ $$ = $1; }
|	ifpre stmnt		{ $$ = new(CONDI, $2, Z, $1); }
|	ifpre stmnt ELSE YoN stmnt		{ $$ = new(CONDI, $2, $5, $1); }
|	FOR '(' expr ';' expr ';' expr ')' YoN stmnt {
				  $$ = newloop($3, $5, $7, $10); }
|	whpre stmnt		{ $$ = newhile($1, $2); }
|	DO YoN stmnt WHILE '(' expr ')' ';'	{ $$ = newuntil($3, $6); }
|	RETURN	expr ';'	{ $$ = new(ORETURN, $2, Z, Z); }

iarray:	BW			{ usedboth; $$ = super(CURSCRATCH, Z, BW); }
|	RGB			{ usedboth; $$ = super(CURSCRATCH, Z, RGB); }
|	RCHAN			{ usedboth; $$ = super(CURSCRATCH, Z, RCHAN); }
|	GCHAN			{ usedboth; $$ = super(CURSCRATCH, Z, GCHAN); }
|	BCHAN			{ usedboth; $$ = super(CURSCRATCH, Z, BCHAN); }
|	array			{ $$ = super($1, Z, (src[$1].nchan == 1)?BW:RGB); }
|	array '.' RGB		{ $$ = super($1, Z, RGB); }
|	array '.' BW		{ $$ = super($1, Z, BW);  }
|	array '.' RCHAN		{ $$ = super($1, Z, RCHAN); }
|	array '.' GCHAN		{ $$ = super($1, Z, GCHAN); }
|	array '.' BCHAN		{ $$ = super($1, Z, BCHAN); }
|	array '[' elist ']'	{ $$ = super($1, $3,(src[$1].nchan == 1)?BW:RGB); }
|	array '[' elist ']' '.' RGB	{ $$ = super($1, $3, RGB); }
|	array '[' elist ']' '.' BW	{ $$ = super($1, $3, BW);  }
|	array '[' elist ']' '.' RCHAN	{ $$ = super($1, $3, RCHAN); }
|	array '[' elist ']' '.' GCHAN	{ $$ = super($1, $3, GCHAN); }
|	array '[' elist ']' '.' BCHAN	{ $$ = super($1, $3, BCHAN); }
|	array '.'   RGB '[' elist ']'	{ $$ = super($1, $5, RGB); }
|	array '.'    BW '[' elist ']'	{ $$ = super($1, $5, BW);  }
|	array '.' RCHAN '[' elist ']'	{ $$ = super($1, $5, RCHAN); }
|	array '.' GCHAN '[' elist ']'	{ $$ = super($1, $5, GCHAN); }
|	array '.' BCHAN '[' elist ']'	{ $$ = super($1, $5, BCHAN); }
|	ARRAY '[' elist ']'	{ $$=nsup(RL(($1->u.ar->base),Index($3)),0,0,128);}
|	ARRAY			{ $$=nsup(RL(($1->u.ar->base),DII), 0,0,128); }

assign:	lval POST		{ $$ = weird($1, notnew(LO, $1->n, NR(1))); }
|	lval OPER expr	{ $$ = weird($1, notnew(LO, $1->n, $3)); }
|	lval ASSIGN expr	{ $$ = weird($1, $3); }

rval:	lval			{ $$ = $1->n; }
|	fcall			{ $$ = $1;    }

lval:	X			{ $$ = nsup(new(REG, Z, Z, XREG), 0, 0, 128); }
|	Y			{ $$ = nsup(new(REG, Z, Z, YREG), 0, 0, 128); }
|	I			{ $$ = nsup(new(REG, Z, Z, IREG), 0, 0, 128); }
|	iarray			{ $$ = $1; }
|	NAME			{ yyerror("undeclared variable %s", $1->name); }
|	VARIABLE		{ $$ = nsup(new(VAR,  Z, Z, &$1->u.val), 0,0,128);}
|	AUTO			{ $$ = nsup(new(OARG, Z, Z,  $1->u.val), 0,0,128);}

fcall:	FCT aparams		{ if ($1->u.fu->usednew == -1 && notrecurcall($1))
					yyerror("undefined function %s", $1->name);
				  argcount($1->u.fu->nrparams, $2, $1->name);
				  usednew += $1->u.fu->usednew;
				  usedold += $1->u.fu->usedold;
				  $$ = new(OCALL, $2, Z, $1->u.fu->fct);
				}
|	BLTN aparams		{ argcount($1->u.bl->nrparams, $2, $1->name);
				  usednew += $1->u.bl->usednew;
				  $$ = new(CCALL, $2, Z, $1->u.bl->faddr);
				}

vval:	VAL			{ $$ = $1; }
|	'-' VAL			{ $$ = -$2; }

eval:	VAL			{ $$ = $1; }
|	VARIABLE		{ $$ = $1->u.val; }
|	fcall			{ compile($1, progr); $$ = callit(); }

expr:	rval			{ $$ = $1; }
|	assign			{ $$ = $1; }
|	'(' expr ')'		{ $$ = $2; }
|	'[' elist ']'		{ $$ = new(COMP, $2, Z, Z); }
|	VAL			{ $$ = NR($1); }
|	STRING			{ $$ = NR($1); }
|	expr '?' expr ':' expr { $$ = new(CONDI, $3, $5, $1); }
|	'-' expr %prec UNARYMINUS  { $$ = new(OMINUS, $2, Z, Z); }
|	'~' expr		{ $$ = new(ONEG, $2, Z, Z); }
|	NOT expr		{ $$ = new(ONOT, $2, Z, Z); }
|	expr '+' expr		{ $$ = notnew(OADD, $1, $3); }
|	expr '-' expr		{ $$ = notnew(OSUB, $1, $3); }
|	expr '*' expr		{ $$ = notnew(OMUL, $1, $3); }
|	expr '/' expr		{ $$ = notnew(DIVV, $1, $3); }
|	expr '%' expr		{ $$ = notnew(MODU, $1, $3); }
|	expr '^' expr		{ $$ = notnew(OXOR, $1, $3); }
|	expr POW expr		{ $$ = notnew(OPOW, $1, $3); }
|	expr GT expr		{ $$ = notnew(OGT,  $1, $3); }
|	expr GE expr		{ $$ = notnew(OGE,  $1, $3); }
|	expr LT expr		{ $$ = notnew(OLT,  $1, $3); }
|	expr LE expr		{ $$ = notnew(OLE,  $1, $3); }
|	expr EQ expr		{ $$ = notnew(OEQ,  $1, $3); }
|	expr NE expr		{ $$ = notnew(ONE,  $1, $3); }
|	expr ANDAND expr	{ $$ = notnew(OANDAND, $1, $3); }
|	expr OROR expr		{ $$ = notnew(OOROR, $1, $3); }
|	expr OR expr		{ $$ = notnew(OOR,  $1, $3); }
|	expr AND expr		{ $$ = notnew(OAND, $1, $3); }
|	expr LSH expr		{ $$ = notnew(OLSH, $1, $3); }
|	expr RSH expr		{ $$ = notnew(OLSH, $1, notnew(OMINUS, $3, Z)); }

array:	TMP			{ usednew=usedold=1; $$ = CURSCRATCH; }
|	OLD			{ usedold=1; $$ = CUROLD; }
|	'$' TMP			{ usednew=usedold=1; $$ = CURSCRATCH; }
|	'$' OLD			{ usedold=1; $$ = CUROLD; }
|	'$' VAL 		{ int x = $2;
				  if (x < 0 || x > nsrc-2)
					yyerror("unknown file number %d", x);
				  x = (x == 0) ? CUROLD : x+1;
				  if (x == CUROLD)
					usedold=1;
				  else
					src[x].used = 1;
				  $$ = x;
				}
|	'$' ANAME		{ int x = whatarray($2->name);
				  if (x <= 0)
					x = prepare($2->name);
				  if (x <= 0)
					yyerror("unknown file %s", $2->name);
				  src[x].used = 1;
				  $$ = x;
				}
%%
