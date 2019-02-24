#ifndef GRAM_H
#define GRAM_H
extern int LexIndex;
extern int LexGoal;
extern struct Expr *CurrentExpr;
extern char *LexString;
extern char *yyerr;		/* yacc doesn't use this */
extern long yyres;
extern char Token[];
#endif
