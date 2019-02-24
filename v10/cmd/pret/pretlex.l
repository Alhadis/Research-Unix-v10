%{
#include "stdio.h"
#include "pret.h"
#include "y.tab.h"

extern linenumber, nest;
extern char filename[256];

putback(c)
{	unput(c);
}

%}
%%
\n	{ linenumber++; }
\/\*	{ char c1, c2;		/* comment string */

	  for(nest++, c2 = ' ';;){
		c1 = c2;
		c2 = input();
		if (c1 == '/' && c2 == '*')
			nest++;
		else if (c1 == '*' && c2 == '/')
			nest--;
		if (nest <= 0)
			break;
		if (c2 == '\n')
			linenumber++;
		else if (c2 == 0)
			whoops("unexpected eof (in comment)");
	  }
	}
[ \t]	{ ; }
^#[ ]+[0-9]+[ ]+\"[^\"]+\"\n	{
		sscanf(&yytext[1], "%d \"%s\"\n", &linenumber, filename);
		filename[strlen(filename)-1] = 0;	/* strip the last " */
	}
queue	{ return(QUEUES); }
qset	{ return(QSET); }
pvar	{ return(PVAR); }
proc	{ return(PROCESS); }
if	{ return(IF); }
fi	{ return(FI); }
do	{ return(DO); }
od	{ return(OD); }
skip	{ return(skip); }
goto	{ return(GOTO); }
break	{ return(BREAK); }
any 	{ return(DEFAULT); }
default	{ return(DEFAULT); }
assert	{ return(ASSERT); }
error	{ return(ERROR); }
timeout	{ return(timeout); }
\{	{ return(PBEGIN); }
\}	{ return(END); }
[0-9]+	{ yylval.resu = atoi(yytext); return(VALUE); }
[a-zA-Z][_a-zA-Z0-9]*	{
		yylval.resu = newstring(yytext);
		if (isarrayvar(yytext)) return(ARNAME);
		if (isqset(yytext)) return(QSNAME);
		return(NAME);
	}
_PROCID	{ yylval.resu = newstring("_PROCID"); return(NAME); }
"::"	{ return(FLAG); }
"->"	{ return(ARROW); }
";"	{ return(SEMICOLON); }
":"	{ return(COLON); }
\+\+	{ return(INC); }
\-\-	{ return(DEC); }
\+\=	{ return(ADDEQ); }
\-\=	{ return(SUBEQ); }
\*\=	{ return(MULEQ); }
\/\=	{ return(DIVEQ); }
\%\=	{ return(MODEQ); }
\|\|	{ return(OR); }
\&\&	{ return(AND); }
\>\=	{ return(GE); }
\<\=	{ return(LE); }
\!\=	{ return(NE); }
\>	{ return(GT); }
\<	{ return(LT); }
\!	{ return(NOT); }
==	{ return(EQ); }
.	{ return(yytext[0]); }
