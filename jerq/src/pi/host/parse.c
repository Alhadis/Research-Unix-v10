#include "parse.h"
SRCFILE("parse.c")

void *Parse.parse(char *s)
{
	trace( "%d.parse(%s) %d", this, s, goal );
	
	LexIndex = -1;
	LexGoal = goal;
	LexString = sf("%s;", s);
	yyerr = 0;
	yyres = 0;
	CurrentExpr = expr;
	yyparse();
	if( !yyerr && !yyres ) yyerr = "parse error";
	error = yyerr;
	return (void*) yyres;
}

void yyerror(char *msg)
{
	yyerr = msg ? msg : "parse error";
}
