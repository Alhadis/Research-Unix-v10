%{
#include "hoc.h"
#define	code2(c1,c2)	code(c1); code(c2)
#define	code3(c1,c2,c3)	code(c1); code(c2); code(c3)
%}
%union {
	Symbol	*sym;	/* symbol table pointer */
	Inst	*inst;	/* machine instruction */
	int	narg;	/* number of arguments */
}
%token	<sym>	NUMBER STRING PRINT VAR BLTIN UNDEF WHILE FOR IF ELSE
%token	<sym>	FUNCTION PROCEDURE RETURN FUNC PROC READ
%token	<narg>	ARG
%type	<inst>	expr stmt asgn prlist stmtlist
%type	<inst>	cond while for if begin end 
%type	<sym>	procname
%type	<narg>	arglist
%right	'=' ADDEQ SUBEQ MULEQ DIVEQ MODEQ
%left	OR
%left	AND
%left	GT GE LT LE EQ NE
%left	'+' '-'
%left	'*' '/' '%'
%left	UNARYMINUS NOT INC DEC
%right	'^'
%%
list:	  /* nothing */
	| list '\n'
	| list defn '\n'
	| list asgn '\n'  { code2(xpop, STOP); return 1; }
	| list stmt '\n'  { code(STOP); return 1; } 
	| list expr '\n'  { code2(print, STOP); return 1; }
	| list error '\n' { yyerrok; }
	;
asgn:	  VAR '=' expr { code3(varpush,(Inst)$1,assign); $$=$3; }
	| VAR ADDEQ expr	{ code3(varpush,(Inst)$1,addeq); $$=$3; }
	| VAR SUBEQ expr	{ code3(varpush,(Inst)$1,subeq); $$=$3; }
	| VAR MULEQ expr	{ code3(varpush,(Inst)$1,muleq); $$=$3; }
	| VAR DIVEQ expr	{ code3(varpush,(Inst)$1,diveq); $$=$3; }
	| VAR MODEQ expr	{ code3(varpush,(Inst)$1,modeq); $$=$3; }
	| ARG '=' expr   { defnonly("$"); code2(argassign,(Inst)$1); $$=$3;}
	| ARG ADDEQ expr { defnonly("$"); code2(argaddeq,(Inst)$1); $$=$3;}
	| ARG SUBEQ expr { defnonly("$"); code2(argsubeq,(Inst)$1); $$=$3;}
	| ARG MULEQ expr { defnonly("$"); code2(argmuleq,(Inst)$1); $$=$3;}
	| ARG DIVEQ expr { defnonly("$"); code2(argdiveq,(Inst)$1); $$=$3;}
	| ARG MODEQ expr { defnonly("$"); code2(argmodeq,(Inst)$1); $$=$3;}
	;
stmt:	  expr	{ code(xpop); }
	| RETURN { defnonly("return"); code(procret); }
	| RETURN expr
	        { defnonly("return"); $$=$2; code(funcret); }
	| PROCEDURE begin '(' arglist ')'
		{ $$ = $2; code3(call, (Inst)$1, (Inst)$4); }
	| PRINT prlist	{ $$ = $2; }
	| while '(' cond ')' stmt end {
		($1)[1] = (Inst)$5;	/* body of loop */
		($1)[2] = (Inst)$6; }	/* end, if cond fails */
	| for '(' cond ';' cond ';' cond ')' stmt end {
		($1)[1] = (Inst)$5;	/* condition */
		($1)[2] = (Inst)$7;	/* post loop */
		($1)[3] = (Inst)$9;	/* body of loop */
		($1)[4] = (Inst)$10; }	/* end, if cond fails */
	| if '(' cond ')' stmt end {	/* else-less if */
		($1)[1] = (Inst)$5;	/* thenpart */
		($1)[3] = (Inst)$6; }	/* end, if cond fails */
	| if '(' cond ')' stmt end ELSE stmt end {	/* if with else */
		($1)[1] = (Inst)$5;	/* thenpart */
		($1)[2] = (Inst)$8;	/* elsepart */
		($1)[3] = (Inst)$9; }	/* end, if cond fails */
	| '{' stmtlist '}'	{ $$ = $2; }
	;
cond:	   expr 	{ code(STOP); }
	;
while:	  WHILE	{ $$ = code3(whilecode,STOP,STOP); }
	;
for:	  FOR	{ $$ = code(forcode); code3(STOP,STOP,STOP); code(STOP); }
	;
if:	  IF	{ $$ = code(ifcode); code3(STOP,STOP,STOP); }
	;
begin:	  /* nothing */		{ $$ = progp; }
	;
end:	  /* nothing */		{ code(STOP); $$ = progp; }
	;
stmtlist: /* nothing */		{ $$ = progp; }
	| stmtlist '\n'
	| stmtlist stmt
	;
expr:	  NUMBER { $$ = code2(constpush, (Inst)$1); }
	| VAR	 { $$ = code3(varpush, (Inst)$1, eval); }
	| ARG	 { defnonly("$"); $$ = code2(arg, (Inst)$1); }
	| asgn
	| FUNCTION begin '(' arglist ')'
		{ $$ = $2; code3(call,(Inst)$1,(Inst)$4); }
	| READ '(' VAR ')' { $$ = code2(varread, (Inst)$3); }
	| BLTIN '(' expr ')' { $$=$3; code2(bltin, (Inst)$1->u.ptr); }
	| '(' expr ')'	{ $$ = $2; }
	| expr '+' expr	{ code(add); }
	| expr '-' expr	{ code(sub); }
	| expr '*' expr	{ code(mul); }
	| expr '/' expr	{ code(div); }
	| expr '%' expr	{ code(mod); }
	| expr '^' expr	{ code (power); }
	| '-' expr   %prec UNARYMINUS   { $$=$2; code(negate); }
	| expr GT expr	{ code(gt); }
	| expr GE expr	{ code(ge); }
	| expr LT expr	{ code(lt); }
	| expr LE expr	{ code(le); }
	| expr EQ expr	{ code(eq); }
	| expr NE expr	{ code(ne); }
	| expr AND expr	{ code(and); }
	| expr OR expr	{ code(or); }
	| NOT expr	{ $$ = $2; code(not); }
	| INC VAR	{ $$ = code2(preinc,(Inst)$2); }
	| DEC VAR	{ $$ = code2(predec,(Inst)$2); }
	| VAR INC	{ $$ = code2(postinc,(Inst)$1); }
	| VAR DEC	{ $$ = code2(postdec,(Inst)$1); }
	;
prlist:	  expr			{ code(prexpr); }
	| STRING		{ $$ = code2(prstr, (Inst)$1); }
	| prlist ',' expr	{ code(prexpr); }
	| prlist ',' STRING	{ code2(prstr, (Inst)$3); }
	;
defn:	  FUNC procname { $2->type=FUNCTION; indef=1; }
	    '(' ')' stmt { code(procret); define($2); indef=0; }
	| PROC procname { $2->type=PROCEDURE; indef=1; }
	    '(' ')' stmt { code(procret); define($2); indef=0; }
	;
procname: VAR
	| FUNCTION
	| PROCEDURE
	;
arglist:  /* nothing */ 	{ $$ = 0; }
	| expr			{ $$ = 1; }
	| arglist ',' expr	{ $$ = $1 + 1; }
	;
%%
	/* end of grammar */
#include <stdio.h>
#include <ctype.h>
char	*progname;
int	lineno = 1;
#include <signal.h>
#include <setjmp.h>
#include <errno.h>
jmp_buf	begin;
int	indef;
char	*infile;	/* input file name */
FILE	*fin;		/* input file pointer */
char	**gargv;	/* global argument list */
extern	errno;
int	gargc;

int c = '\n';	/* global for use by warning() */

yylex()		/* hoc6 */
{
	while ((c=getc(fin)) == ' ' || c == '\t')
		;
	if (c == EOF)
		return 0;
	if (c == '\\') {
		c = getc(fin);
		if (c == '\n') {
			lineno++;
			return yylex();
		}
	}
	if (c == '#') {		/* comment */
		while ((c=getc(fin)) != '\n' && c != EOF)
			;
		if (c == '\n')
			lineno++;
		return c;
	}
	if (c == '.' || isdigit(c)) {	/* number */
		double d;
		ungetc(c, fin);
		fscanf(fin, "%lf", &d);
		yylval.sym = install("", NUMBER, d);
		return NUMBER;
	}
	if (isalpha(c) || c == '_') {
		Symbol *s;
		char sbuf[100], *p = sbuf;
		do {
			if (p >= sbuf + sizeof(sbuf) - 1) {
				*p = '\0';
				execerror("name too long", sbuf);
			}
			*p++ = c;
		} while ((c=getc(fin)) != EOF && (isalnum(c) || c == '_'));
		ungetc(c, fin);
		*p = '\0';
		if ((s=lookup(sbuf)) == 0)
			s = install(sbuf, UNDEF, 0.0);
		yylval.sym = s;
		return s->type == UNDEF ? VAR : s->type;
	}
	if (c == '$') {	/* argument? */
		int n = 0;
		while (isdigit(c=getc(fin)))
			n = 10 * n + c - '0';
		ungetc(c, fin);
		if (n == 0)
			execerror("strange $...", (char *)0);
		yylval.narg = n;
		return ARG;
	}
	if (c == '"') {	/* quoted string */
		char sbuf[100], *p, *emalloc();
		for (p = sbuf; (c=getc(fin)) != '"'; p++) {
			if (c == '\n' || c == EOF)
				execerror("missing quote", "");
			if (p >= sbuf + sizeof(sbuf) - 1) {
				*p = '\0';
				execerror("string too long", sbuf);
			}
			*p = backslash(c);
		}
		*p = 0;
		yylval.sym = (Symbol *)emalloc(strlen(sbuf)+1);
		strcpy(yylval.sym, sbuf);
		return STRING;
	}
	switch (c) {
	case '+':	return follow('+', INC, follow('=', ADDEQ, '+'));
	case '-':	return follow('-', DEC, follow('=', SUBEQ, '-'));
	case '*':	return follow('=', MULEQ, '*');
	case '/':	return follow('=', DIVEQ, '/');
	case '%':	return follow('=', MODEQ, '%');
	case '>':	return follow('=', GE, GT);
	case '<':	return follow('=', LE, LT);
	case '=':	return follow('=', EQ, '=');
	case '!':	return follow('=', NE, NOT);
	case '|':	return follow('|', OR, '|');
	case '&':	return follow('&', AND, '&');
	case '\n':	lineno++; return '\n';
	default:	return c;
	}
}

backslash(c)	/* get next char with \'s interpreted */
	int c;
{
	char *strchr();	/* `index()' in some systems */
	static char transtab[] = "b\bf\fn\nr\rt\t";
	if (c != '\\')
		return c;
	c = getc(fin);
	if (islower(c) && strchr(transtab, c))
		return strchr(transtab, c)[1];
	return c;
}

follow(expect, ifyes, ifno)	/* look ahead for >=, etc. */
{
	int c = getc(fin);

	if (c == expect)
		return ifyes;
	ungetc(c, fin);
	return ifno;
}

defnonly(s)	/* warn if illegal definition */
	char *s;
{
	if (!indef)
		execerror(s, "used outside definition");
}

yyerror(s)	/* report compile-time error */
	char *s;
{
/*rob
	warning(s, (char *)0);
	longjmp(begin, 0);
rob*/
	execerror(s, (char *)0);
}

execerror(s, t)	/* recover from run-time error */
	char *s, *t;
{
	warning(s, t);
	fseek(fin, 0L, 2);		/* flush rest of file */
	longjmp(begin, 0);
}

fpecatch()	/* catch floating point exceptions */
{
	execerror("floating point exception", (char *) 0);
}

intcatch()	/* catch interrupts */
{
	execerror("interrupt", (char *) 0);
}

main(argc, argv)	/* hoc6 */
	char *argv[];
{
	int i, fpecatch();
	static int first = 1;

	progname = argv[0];
	init();
	if (argc == 1) {	/* fake an argument list */
		static char *stdinonly[] = { "-" };

		gargv = stdinonly;
		gargc = 1;
	} else if (first) {	/* for interrupts */
		first = 0;
		gargv = argv+1;
		gargc = argc-1;
	}
	while (moreinput())
		run();
	signal(SIGINT, SIG_IGN);
	return 0;
}

moreinput()
{
	if (gargc-- <= 0)
		return 0;
	if (fin && fin != stdin)
		fclose(fin);
	infile = *gargv++;
	lineno = 1;
	if (strcmp(infile, "-") == 0) {
		fin = stdin;
		infile = 0;
	} else if ((fin=fopen(infile, "r")) == NULL) {
		fprintf(stderr, "%s: can't open %s\n", progname, infile);
		return moreinput();
	}
	return 1;
}

run()	/* execute until EOF */
{
	setjmp(begin);
	signal(SIGINT, intcatch);
	signal(SIGFPE, fpecatch);
	for (initcode(); yyparse(); initcode())
		execute(progbase);
}

warning(s, t)	/* print warning message */
	char *s, *t;
{
	fprintf(stderr, "%s: %s", progname, s);
	if (t)
		fprintf(stderr, " %s", t);
	if (infile)
		fprintf(stderr, " in %s", infile);
	fprintf(stderr, " near line %d\n", lineno);
	while (c != '\n' && c != EOF)
		if((c = getc(fin)) == '\n')	/* flush rest of input line */
			lineno++;
		else if (c == EOF && errno == EINTR) {
			clearerr(stdin);	/* ick! */
			errno = 0;
		}
}
