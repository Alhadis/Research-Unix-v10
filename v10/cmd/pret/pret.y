%{
#include <stdio.h>
#include "pret.h"
#include "pret.d"

#define YYDEBUG 1

struct Node {
	int ntyp;	/* OP ~ nval&left&right, NM ~ val */
	int nval;
	struct Node *left, *right;
};

extern struct Node *newnode();

struct PARS {
	int home;
	int dest;
	int bpnt;
} ties[MAXDEPTH];

FILE *tb;				/* temp file to store tables */

char procname[MAXNAME], refname[MAXNAME], qsetname[MAXNAME];
char str[256], filename[256];
char strings[MANY][MAXNAME];
extern char yytext[];
extern struct QTABLE qtable[NQUEUES];
extern int varwidths;
int nnames = 0;
int linenumber = 1;
int linecode = 0;	/* include source code references  */
int nest = 0;		/* nesting level of comments (lex) */
int anyerror = 0;

int pid = NONE;	/* process number  */
int qid = NONE;	/* queue number    */
int rid = NONE;	/* template number when defined */
int sid = NONE;	/* qset number     */
int nid = NONE;	/* template number when refered */
int cid = NONE;	/* index in call table          */
int qind, qisz;		/* queue's initial string size  */
int parnum;		/* counts actual parameters     */
int assertion = -1;	/* id of assertion table, if any */
int inertion = -1;	/* id of error table, if any */
int vartype, inside;
int n, m, from, xx, zz;
int soo = 0;		/* start of optionlist */
int curstate = 0;
int curdepth = 0;
int lastloop = -1;

int verbose = 0;
int nopurge = 0;

checknames()
{
	checkrefs();
	checkglobvars();
	checkqs();
}

putglobals(fd)
	FILE *fd;
{
	numrefs(fd);
	numprocs(fd);
	numsorts(fd);
	numinits(fd);
	numglobvars(fd);
}

puttables(fd1)
	FILE *fd1;
{	FILE *fd2;
	char buffer[MANY];
	int howmuch;

	if ((fd2 = fopen("pret.tmp", "r")) == NULL)
		whoops("cannot find pret.tmp");

	while ((howmuch = fread(buffer, sizeof(*buffer), MANY, fd2)) > 0)
		fwrite(buffer, sizeof(*buffer), howmuch, fd1);

 	fclose(fd2);
	unlink("pret.tmp");
}

makebin()
{	FILE *fd;

	if ((fd = fopen((anyerror)?"pret.err":"pret.out", "w")) == NULL)
		whoops("cannot create output file");

	putglobals(fd);
	puttables(fd);
	numexps(fd);
	fclose(fd);
	if (anyerror)
		fprintf(stderr, "output written to `pret.err'\n");
}

transfer(cl, tg)
{	int i = curstate;
	curstate = enterowname(NEW, "", DAR);
	setrans(i, cl, curstate, tg);
}

#include "pret.expr.c"

%}
%union{
	int resu;
	struct Node *node;
}

%type	<resu>	PREIO INDEX QINDEX IMPORT ASGN ASGN
%type	<node>	expr VARNAME
%token	<resu>	NAME VALUE ARNAME QSNAME

%start		PROT_SPEC
%token		ASSERT ERROR
%token		PROCESS PBEGIN END IF FI DO OD
%token		timeout skip BREAK DEFAULT GOTO
%token		FLAG ARROW SEMICOLON COLON
%token		QUEUES QSET PVAR MESG
%right		'=' ADDEQ SUBEQ MULEQ DIVEQ MODEQ
%left		OR
%left		AND
%left		GT GE LT LE EQ NE
%left		'+' '-'
%left		'*' '/' '%'
%left		UNARYMINUS NOT INC DEC
%right		'^'

%%

PROT_SPEC	:	ONEMODULE
	 	|	PROT_SPEC ONEMODULE
	 	;

ONEMODULE	:	PROC_SPEC
		|	TASK_SPEC
		|	ONEDECL
		|	REQUIREMENT
		;

PROC_SPEC	:	PROCESS NAME QINDEX
			{	strcpy(procname, strings[$2]);
				pid = newprocname(strings[$2], $3);
			}
			PBEGIN ANYDECLS
			{ extern int extras;
			  int ival;
				curstate = enterowname(NEW, "", DAR);
				newcalltable();
				ival = 3*MANY+pid+extras;
				addvarname("_PROCID", (DCL|RFR), ival, NONE, 0);
			}
			SEQUENCE END
			{	wrapup(NONE, pid, tb, nopurge, verbose);
				pid = NONE;
				strcpy(procname, "_");
			}
	       	;

ANYDECLS	:	/* empty */
		|	DECLS
		;
DECLS		:	ONEDECL
		|	DECLS ONEDECL
		;

ONEDECL		:	QDECLS
		|	VARDECLS
		;
/*
 *	QUEUES
 *      ======
 */

QDECLS		:	QUEUES QDECL SEPARATOR
		;
QDECL		:	ONEQ
		|	QNAMELIST
		;

ONEQ		:	NAME QINDEX '='
			{	if ($2 == NONE || (qisz=$2) <= 0)
				yyerror("illegal queue size, %s", strings[$1]);
				qid = newqname(strings[$1], DCL, qisz, NONE);
			}
			PBEGIN MNAMELIST END
		;

/*
 *	VARIABLES and QUEUESETS
 *      =======================
 */

VARDECLS	:	PVAR VNAMELIST  SEPARATOR
		|	QSET QSETDECL SEPARATOR
		;

QINDEX		:	/* empty */	{ $$ = NONE; }
		|	'[' VALUE ']'	{ $$ =   $2; }
		;

QSETDECL	:	NAME PBEGIN NAME QINDEX COLON
			{	xx = newqname(strings[$3], RFR, NONE, $4);
				sid = newqset(strings[$1], strings[$3], DCL, $4);
			}
			SNAMELIST END
			{	closeqset(sid);
			}
		;
/*
 *	PROCEDURES
 *      ==========
 */

TASK_SPEC	:	NAME
			{	rid = newreftask(strings[$1], DCL);
				curstate = enterowname(NEW, "", DAR);
				strcpy(refname, strings[$1]);
				newcalltable();
				vartype = ISV;
				inside = 1;
			}
			'(' ANYPARAMS ')'
			{	inside = 0;
			}
			ANYPARTYPES
			{	reorder();	/* renumber formal parameters */
			}
			PBEGIN
			ANYDECLS
			SEQUENCE
			END
			{	wrapup(rid, NONE, tb, nopurge, verbose);
				strcpy(refname, "_");
				rid = NONE;
			}
		;

ANYPARAMS	:	/* empty */
		|	PNAMELIST
		|	error
			{	yyerror("bad namelist", "");
			}

		;

ANYPARTYPES	:	/* empty */
		|	PARTYPES
		|	error
			{	yyerror("bad parameterlist", "");
			}
		;

PARTYPES	:	ONEPARTYPE SEMICOLON
		|	PARTYPES ONEPARTYPE SEMICOLON
		;
ONEPARTYPE	:	PVAR
			{	vartype = ISV;
			}
			PNAMELIST
		|	QSET NAME PBEGIN NAME COLON
			{	strcpy(qsetname, strings[$2]);
				sid = newqset(strings[$2], strings[$4], DCL, NONE);
				qid = addFpar(rid, strings[$2], sid, ISQ, inside);
				vartype = ISM;
			}
			PNAMELIST END
			{	qid = NONE;
				closeqset(sid);
			}
		;
/*
 *	REQUIREMENTS
 *	============
 */
REQUIREMENT	:	ASSERT
			PBEGIN
			{	rid = newreftask(" assert", DCL);
				assertion = rid;
				curstate = enterowname(NEW, "", DAR);
				strcpy(refname, " assert");
				newcalltable();
			}
			_SEQUENCE
			END
			{	wrapup(rid, NONE, tb, nopurge, verbose);
				strcpy(refname, "_");
				rid = NONE;
				lastloop = -1;
			}
		|	ERROR
			PBEGIN
			{	rid = newreftask(" error", DCL);
				inertion = rid;
				curstate = enterowname(NEW, "", DAR);
				strcpy(refname, " error");
				newcalltable();
			}
			_SEQUENCE
			END
			{	wrapup(rid, NONE, tb, nopurge, verbose);
				strcpy(refname, "_");
				rid = NONE;
				lastloop = -1;
			}
		;
_SEQUENCE	:	_STMNT
		|	_SEQUENCE SEPARATOR _STMNT
	 	;

_STMNT		:	skip		{ lastloop = -1; }
		|	_SELECT		{ lastloop = -1; }
	 	|	_CYCLE
	 	|	SEND		{ lastloop = -1; }
	 	|	GUARD		{ lastloop = -1; }
		|	STRUCTGOTO	{ lastloop = -1; }
		|	error
			{	yyerror("illegal assertion statement, %s", yytext);
			}
	 	;
_SELECT		:	IF
			{	if (++curdepth == MAXDEPTH)
					whoops("nesting too deep");

				ties[curdepth].home = curstate;
				ties[curdepth].dest = enterowname(NEW, "", DAR);
				ties[curdepth].bpnt =
					(curdepth > 1) ? ties[curdepth-1].bpnt : -1;
			}
			_OPTIONLIST FI
			{	setrowname(ties[curdepth].dest);
				curstate = ties[curdepth--].dest;
			}
		;

_CYCLE		:	DO
			{	if (curdepth++ == MAXDEPTH)
					whoops("nesting too deep");

				labelrow(curstate);
				ties[curdepth].home = curstate;
				ties[curdepth].dest = curstate;
				ties[curdepth].bpnt = enterowname(NEW, "", DAR);
			}
			_OPTIONLIST OD
			{	lastloop = ties[curdepth].home;
				setrowname(ties[curdepth].bpnt);
				curstate = ties[curdepth--].bpnt;
			}
		;
_OPTIONLIST	:	_ONEOPTION
	 	|	_ONEOPTION _OPTIONLIST
	 	;

_ONEOPTION	:	FLAG _SEQUENCE
			{	getrowname(str, ties[curdepth].dest);
				from = curstate;
				curstate = enterowname(OLD, str, DAR);
				setrans(from, 0, curstate, NONE);
				curstate = ties[curdepth].home;
			}
	 	;
/*
 *	NAMELISTS
 *	=========
 */

ANAMELIST	:	ANAME
		|	ANAMELIST ',' ANAME
		;
MNAMELIST	:	MNAME
		|	MNAMELIST ',' MNAME
		;
PNAMELIST	:	PNAME
		|	PNAMELIST ',' PNAME
		;
QNAMELIST	:	QNAME
		|	QNAMELIST ',' QNAME
		;
SNAMELIST	:	SNAME
		|	SNAMELIST ',' SNAME
		;
VNAMELIST	:	VNAME
		|	VNAMELIST ',' VNAME
		;
/*
 *	NAMES
 *	=====
 */

ANAME		:	QSNAME { addApars(strings[$1], nid, parnum++, NONE); }
		|	expr   { addAspecial(makeexpr($1), nid, parnum++);   }
		;
MNAME		:	NAME
			{	qtable[qid].status |= ADR;
				addmsg(strings[$1], qid, SND, INITM, NONE);
				if (--qisz < 0)
				yyerror("queue overfilled, %s", strings[$1]);
			}
		;
PNAME		:	NAME
			{	if (vartype == ISM)
				addsetname(strings[$1], sid, 1);
				else
				addFpar(rid, strings[$1], qid, vartype, inside);
			}
		;
QNAME		:	NAME '[' VALUE ']'
			{	if ((qisz = $3) <= 0)
				yyerror("illegal queue size, %s", strings[$1]);
				newqname(strings[$1], DCL, $3, NONE);
			}
		|	NAME '[' VALUE ']' '[' VALUE ']'
			{	if ((qisz = $6) <= 0)
				yyerror("illegal queue size, %s", strings[$1]);
				newqname(strings[$1], DCL, qisz, $3);
			}
		;
SNAME		:	NAME		{ addsetname(strings[$1], sid, 0); }
		;
VNAME		:	NAME QINDEX
			{	addvarname(strings[$1], DCL, NONE, $2, 0);
			}
		|	NAME QINDEX '=' expr
			{	addvarname(strings[$1], DCL, makeexpr($4), $2, 0);
			}
		|	NAME QINDEX COLON VALUE
			{	addvarname(strings[$1], DCL, NONE, $2, $4);
			}
		;
/*
 *	CODE
 *	====
 */

SEQUENCE	:	STMNT
			{	soo = 0;
			}
		|	SEQUENCE SEPARATOR STMNT
			{	soo = 0;
			}
	 	;

STMNT		:	skip
			{	if (soo)	/* only if used as a guard */
					transfer(0, NONE);
			}
		|	SELECT
	 	|	CYCLE
	 	|	SEND
	 	|	GUARD
	 	|	JUMP
	 	|	LABEL STMNT
	 	|	TEMPLATE
	 	|	BUILTIN
	 	|	CONDITIONAL
		|	END
			{	yyerror("expecting a statement", "");
				whoops("exit");
			}
	 	;

SELECT		:	IF
			{	if (++curdepth == MAXDEPTH)
					whoops("nesting too deep");

				ties[curdepth].home = curstate;
				ties[curdepth].dest = enterowname(NEW, "", DAR);
				ties[curdepth].bpnt =
					(curdepth>1) ? ties[curdepth-1].bpnt : -1;
			}
			OPTIONLIST FI
			{	setrowname(ties[curdepth].dest);
				curstate = ties[curdepth--].dest;
			}
		;

CYCLE		:	DO
			{	if (curdepth++ == MAXDEPTH)
					whoops("nesting too deep");

				labelrow(curstate);
				ties[curdepth].home = curstate;
				ties[curdepth].dest = curstate;
				ties[curdepth].bpnt = enterowname(NEW, "", DAR);
			}
			OPTIONLIST OD
			{	setrowname(ties[curdepth].bpnt);
				curstate = ties[curdepth--].bpnt;
			}
		;

OPTIONLIST	:	ONEOPTION
	 	|	ONEOPTION OPTIONLIST
	 	;

ONEOPTION	:	FLAG
			{	soo = 1;	/* start of an option string */
			}
			SEQUENCE
			{	getrowname(str, ties[curdepth].dest);
				from = curstate;
				curstate = enterowname(OLD, str, DAR);
				setrans(from, 0, curstate, NONE);
				curstate = ties[curdepth].home;
			}
	 	;

INDEX		:	/* empty */	{ $$ = NONE; }
		|	'[' expr ']'	{ $$ = makeexpr($2); }
		;

PREIO		:	NAME INDEX
			{	strcpy(str, strings[$1]); /* swivel away name */
				$$ = $2;
			}
		;

SEND		:	PREIO NOT NAME EXPORT
			{	xx = newqname(str, ADR, NONE, $1);
				zz = addmsg(strings[$3], xx, SND, NORM, $1);
				n = entercolname(zz, OUTP);
				transfer(n, m);
			}
		;

EXPORT	:		/* empty */	{ m = NONE; }
		|	'(' expr ')'	{ m = makeexpr($2); }
		;

GUARD		:	PREIO '?' timeout
			{ int x;
				xx = newqname(str, RFR, NONE, $1);
				x = addmsg(" tau", xx, SAR, NORM, $1);
				/*
				** the leading space is to avoid
				** clashes with a user name `tau',
				** e.g. in a formal parameter list
				*/
				n = entercolname(x, TMO);
				transfer(n, NONE);
			}
	  	|	PREIO '?' NAME IMPORT
			{	xx = newqname(str, RFR, NONE, $1);
				zz = addmsg(strings[$3], xx, RCV, NORM, $1);
				n = entercolname(zz, INP);
				transfer(n, $4);
			}
	  	|	PREIO '?' DEFAULT IMPORT
			{	xx = newqname(str, RFR, NONE, $1);
				zz = addmsg(" any", xx, SAR, NORM, $1);
				n = entercolname(zz, DFL);
				transfer(n, $4);
			}
	 	;

IMPORT	:		/* empty */
			{	$$ = NONE;
			}
	 	|	'(' ARNAME INDEX ')'
			{	$$ = addvarname(strings[$2], RFR, NONE, $3, 0);
			}
	 	|	'(' NAME ')'
			{	$$ = addvarname(strings[$2], RFR, NONE, NONE, 0);
			}
	 	|	'(' VALUE ')'
			{	yyerror("importing into constant", "");
			}
		;
 
SEPARATOR	:	ARROW
	  	|	SEMICOLON
	  	|	error
			{	yyerror("expecting a stmnt separator", "");
			}
	 	;

JUMP		:	GOTO NAME
			{	from = curstate;
				curstate = enterowname(LAB, strings[$2], ADR);
				setrans(from, 0, curstate, NONE);
				curstate = enterowname(NEW, "", DAR);
			}
		|	STRUCTGOTO
		;

STRUCTGOTO	:	BREAK
			{ int i;
				from = curstate;
				i = ties[curdepth].bpnt;

				if (curdepth == 0 || i == -1)
					whoops("illegal break statement");

				getrowname(str, i);
				curstate = enterowname(OLD, str, DAR);
				setrans(from, 0, curstate, NONE);
				curstate = enterowname(NEW, "", DAR);
			}
	 	;

TEMPLATE	:	NAME
			{	nid = newreftask(strings[$1], RFR);
				parnum = 0; qid = sid = NONE;
				cid = newcall(nid);
			}
			'(' ANYACTUALS ')'
			{ int x;
				parrefs(parnum, nid);
				x = entercolname(cid, FCT);
				transfer(x, NONE);
			}
	 	;

ANYACTUALS	:	/* empty */
		|	ANAMELIST
		;

CONDITIONAL	:	'(' expr ')'
			{	transfer(entercolname(makeexpr($2), CND), NONE);
			}
		;

VARNAME		:	ARNAME INDEX
			{	m = addvarname(strings[$1], RFR, NONE, $2, 0);
				$$ = newnode(NM, m, NULL, NULL);
			}
		|	NAME
			{	m = addvarname(strings[$1], RFR, NONE, NONE, 0);
				$$ = newnode(NM, m, NULL, NULL);
			}
		;

BUILTIN		:	VARNAME ASGN expr
			{	m = makeexpr(newnode(OP, $2, $1, $3));
				transfer(0, m);
			}
		|	VARNAME INC 
			{	m = makeexpr(newnode(OP, poinc, $1, NULL));
				transfer(0, m);
			}
		|	VARNAME DEC
			{	m = makeexpr(newnode(OP, podec, $1, NULL));
				transfer(0, m);
			}
		;

ASGN		:	'='	{ $$ =  setv; }
		|	ADDEQ	{ $$ = addeq; }
		|	SUBEQ	{ $$ = subeq; }
		|	MULEQ	{ $$ = muleq; }
		|	DIVEQ	{ $$ = diveq; }
		|	MODEQ	{ $$ = modeq; }
		;

expr		:	expr '+' expr { $$ = newnode(OP, plus, $1, $3); }
		|	expr '-' expr { $$ = newnode(OP, minus, $1, $3); }
		|	expr '*' expr { $$ = newnode(OP, times, $1, $3); }
		|	expr '/' expr { $$ = newnode(OP, div, $1, $3); }
		|	expr '%' expr { $$ = newnode(OP, mod, $1, $3); }
		|	expr '^' expr { $$ = newnode(OP, power, $1, $3); }
		|	expr GT expr { $$ = newnode(OP, gt, $1, $3); }
		|	expr GE expr { $$ = newnode(OP, ge, $1, $3); }
		|	expr LT expr { $$ = newnode(OP, lt, $1, $3); }
		|	expr LE expr { $$ = newnode(OP, le, $1, $3); }
		|	expr EQ expr { $$ = newnode(OP, eq, $1, $3); }
		|	expr NE expr { $$ = newnode(OP, ne, $1, $3); }
		|	expr AND expr { $$ = newnode(OP, land, $1, $3); }
		|	expr OR expr { $$ = newnode(OP, lor, $1, $3); }
		|	'-' expr %prec UNARYMINUS
			{	$$ = newnode(OP, uminus, $2, NULL); }
		|	NOT expr
			{	$$ = newnode(OP, lnot, $2, NULL); }
		|	VALUE
			{	$$ = newnode(NM, $1+3*MANY, NULL, NULL); }
		|	ARNAME INDEX
			{	m = addvarname(strings[$1], RFR, NONE, $2, 0);
				$$ = newnode(NM, m, NULL, NULL);
			}
		|	NAME
			{	m = addvarname(strings[$1], RFR, NONE, NONE, 0);
				$$ = newnode(NM, m, NULL, NULL);
			}
		|	'(' expr ')' { $$ = $2; }
		;

LABEL		:	NAME COLON
			{	from = curstate;
				curstate = enterowname(LAB, strings[$1], DCL);
				labelrow(curstate);
				setrans(from, 0, curstate, NONE);
			}
		;
%%

extern FILE *yyin;

main(argc, argv)
	char **argv;
{
	int base = 1, i = 1;
	char c, buff[256];
	char outfile[32];

	if (argc > base && argv[1][0] == '-')
	{	while ((c = argv[1][i++]) != '\0')
			switch (c) {
			case 's': varwidths = 1; break;
			case 'v': verbose = 1; break;
			case 'n': nopurge = 1; break;
			case 'l': linecode = 1; break;
			default : fprintf(stderr, "usage: pret [-vsnl] file\n");
				  fprintf(stderr, "\tv - verbose\n");
				  fprintf(stderr, "\ts - supertrace format\n");
				  fprintf(stderr, "\tn - no minimization\n");
				  fprintf(stderr, "\tl - enables linecode\n");
				  exit(1);
			}
		base++;
	}
	if (argc <= base)
	{	fprintf(stderr, "usage: pret [-vnl] file\n");
		exit(1);

	}
	if ((tb = fopen("pret.tmp", "w")) == NULL)
		whoops("cannot create pret.tmp\n");

	strcpy(procname, "_");
	strcpy(refname, "_");
	unlink("pret.out");
	unlink("pret.err");

	if (argc > base)
	{	strcpy(filename, argv[base]);

		mktemp(strcpy(outfile, "/tmp/trans.XXXXXX"));
		sprintf(buff, "/lib/cpp %s > %s", filename, outfile);

		if (system(buff))
		{	unlink(outfile);
			exit(1);
		} else
			if ((yyin = fopen(outfile, "r")) == NULL)
			{	printf("cannot open %s\n", outfile);
				whoops("aborting");
			}
		unlink(outfile);
	}
	parsed = (struct REVPOL *)
		Emalloc(EXPRMAX * sizeof(struct REVPOL));
	prs = 0;

	yyparse();
	prepsorts();
	checknames();
	fclose(tb);
	chatter();
	makebin();
	exit(0);
}

newstring(str)
	char *str;
{	register int i;

	for (i = 0; i < nnames; i++)
		if (strcmp(str, strings[i]) == 0)
			return i;
	if (++nnames >= MANY)
		yyerror("symbol table overflow, %s", str);
	strncpy(strings[i], str, MAXNAME-1);
	return i;
}
