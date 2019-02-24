#include <stdio.h>
#include <ctype.h>
#include "pico.h"
#include "y.tab.h"

#define Prompt2		/* if (INPUT == stdin) fprintf(stderr, ":%d ", pline) */

extern FILE	*INPUT;

Symbol	*whichcmd(), *lookup(), *getname();
char	*getstring();

int	linenumber = 1;
int	pline	   =  0;
int	lexlast    = -1;
int	LO	   = -1;
char	lastyy     = '\n';
char	nolf	   = 1;
char	nesting    = 0;
char	operator   = 0;
char	lastdepth  = 0;

/*
 * if the last token seen was not an operator the next
 * newline will be interpreted as a statement separator
 */

#define ADDEQ	210
#define SUBEQ	211
#define MULEQ	212
#define DIVEQ	213
#define MODEQ	214
#define INC	215
#define DEC	216

yylex()
{ int c;
	while ((c = ylexlex()) == SCOM)
	{	do c = ylexlex();
		while (c != ECOM);
	}
	/* fprintf(stderr, "lex: %d [[%c]]\n", c, (isalnum(c))?c:'-');/*  */
	switch (c) {
	case '+':	case '-':	case '*':
	case '/':	case '%':	case '^':
	case POW:	case GT:	case GE:
	case LT:	case LE:	case EQ:
	case NE:	case ANDAND:	case OROR:
	case OR:	case AND:	case LSH:
	case RSH:	case ',':	case NOT:
	case '~':	case '?':	case ':':
	case ASSIGN:	case '(':	case '[':
		operator = 1;
		break;
	case ADDEQ:	operator = 1; LO = OADD; return OPER;
	case SUBEQ:	operator = 1; LO = OSUB; return OPER;
	case MULEQ:	operator = 1; LO = OMUL; return OPER;
	case DIVEQ:	operator = 1; LO = DIVV; return OPER;
	case MODEQ:	operator = 1; LO = MODU; return OPER;
	case INC:	operator = 0; LO = OADD; return POST;
	case DEC:	operator = 0; LO = OSUB; return POST;
	default:
		operator = 0;
		break;
	}
	return c;
}

ylexlex()
{ int c;

again:	switch (c = getc(INPUT)) {
	case  EOF: undirect(); checkit();
	case  ' ':
	case   27:
	case '\t': goto again;

	case '\\': if ((c = getc(INPUT)) == '\n')
			goto again;
		   else
			break;
	case '\n': if (nesting > 0 && (operator || lastyy == '\n'))
		   {	Prompt2;
			pline++;
			goto again;
		   }
	default  : break;
	}

	nolf = (c != '\n');

	if (c == '\"')
	{	yylval.resu = (int) getstring();
		lastyy = '\0';
		return STRING;
	}
	if (isdigit(c))
	{	lexlast = 0;
		do
		{	lexlast = lexlast*10 + c - '0';
			c = getc(INPUT);
		} while (isdigit(c));
		ungetc(c, INPUT);
		lastyy = '\0';

		yylval.resu = lexlast;
		return VAL;
	}

	if (isalpha(c) || c == '_')
	{	yylval.sym = getname(c);
		lastyy = '\0';
		return yylval.sym->type;
	}

	lastyy = '\0';
	switch (c) {
	case '{' : if (++nesting == 1) pline = 1; lastyy = '\n'; return OPEN;
	case '}' : sympurge();
		   if (nesting-- == 1) pline = 0;
		   else ungetc(';', INPUT);
		   return CLOSE;
	case '/' : return follow('=', DIVEQ, follow('*', SCOM, '/'));
	case '%' : return follow('=', MODEQ, '%');
	case '+' : return follow('+', INC, follow('=', ADDEQ, '+'));
	case '-' : return follow('-', DEC, follow('=', SUBEQ, '-'));
	case '*' : return follow('*', POW, follow('=',MULEQ, follow('/',ECOM,'*')));
	case '>' : return follow('=', GE,  follow('>', RSH, GT));
	case '<' : return follow('=', LE,  follow('<', LSH, LT));
	case '=' : return follow('=', EQ, ASSIGN);
	case '!' : return follow('=', NE, NOT);
	case '|' : return follow('|', OROR, OR);
	case '&' : return follow('&', ANDAND, AND);
	case '\n': if (nesting == 0)
			linenumber++;	/* fall through */
		   else
		   {	Prompt2;
			pline++;
		   }
	case ';' : lastyy = '\n';
		   return ';';
	default  : return c;
	}
}

follow(expect, ifyes, ifno)
{
	int c = getc(INPUT);

	if (c == expect)
		return ifyes;

	ungetc(c, INPUT);
	return ifno;
}

Symbol *
getname(cc)
	char cc;
{	char c=cc, sbuf[128], *p = sbuf;

	do
	{	if (p >= sbuf + sizeof(sbuf) - 1)
		{	*p = '\0';
			yyerror("name too long: %s", sbuf);
		}
		*p++ = c;
	} while ((c=getc(INPUT)) != EOF && (isalnum(c) || c == '_'));
	ungetc(c, INPUT);
	*p = '\0';

	if (nesting == 0 && lastyy == '\n')
		return whichcmd(sbuf);
	else
		return lookup(sbuf);
}

char *
getstring()
{	char c, *sbuf, *p;
	sbuf = (char *) Emalloc(512);
	p = sbuf;

	while ((c=getc(INPUT)) != EOF && c != '\"')
	{	if (c == '\\')
			c = follow('n', '\n', follow('t', '\t', '\\'));
		*p++ = c;
		if (p >= sbuf + 512 - 1)
		{	*p = '\0';
			yyerror("string too long: %s", sbuf);
		}
	} ;
	*p = '\0';

	return sbuf;
}
