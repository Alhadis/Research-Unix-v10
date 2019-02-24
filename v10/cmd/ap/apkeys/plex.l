%{
# define WORD 1
# define DOT 2
extern char *yylval, *sufstr();
char *svc();
# include "assert.h"
# include "ctype.h"
%}
%%
[-'A-Za-z0-9#]+	{lcase(yytext); cfree(yylval);
		yylval=svc(sufstr(yytext)); return(WORD);}
"."	return(DOT);
"!"	return(DOT);
"?"	return(DOT);
.	;
\n	;
%%
yywrap()
{
return(1); /* always stop on eof */
}
char *
svc(s)
	char *s;
{
char *p, *calloc();
p = calloc(strlen(s)+1,1);
assert(p!=0);
strcpy(p,s);
return(p);
}
trimnl(s)
	char *s;
{
while (*s)s++;
if (*--s=='\n') *s=0;
}
lcase(s)
	register char *s;
{
register int c;
register char *p;
if (*s==0) return;
for(p=s+1; c= *p; p++)
	if (isupper(c))
		return;
if (isupper(c = *s))
	*s= tolower(c);
}
