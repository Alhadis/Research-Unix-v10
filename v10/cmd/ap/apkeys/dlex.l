%%
[-'A-Za-z0-9#]+	{lcase(yytext);yylval=svc(sufstr(yytext)); return(WORD);}
"!"	return(NEG);
","	return(COMMA);
"."	return(DOT);
"*H"	return(HEAD);
"*P"	return(PARA);
"*S"	return(STORY);
">".*	{yylval=svc(yytext); return(WRITE);}
">>".*	{yylval=svc(yytext); return(APPEND);}
"|".*	{yylval=svc(yytext); return(PIPE);}
\n	return(NL);
.	;
%%
yyerror(s)
{
extern char *infile;
fprintf(stderr, "%s\n", s);
fprintf(stderr, "Bad input spec file %s line %d\n", infile, yylineno);
}
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
lcase(s)
	register char *s;
{
register int c;
register char *p;
if (*s==0) return;
for(p=s+1; c= *p; p++)
	if (isupper(c))
		return;
if (isupper( c = *s))
	*s = tolower(c);
}
