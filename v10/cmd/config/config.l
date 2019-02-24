%{
/*	config.l	1.8	81/05/18	*/

#include <ctype.h>
#include "y.tab.h"
#include "config.h"

#define tprintf if (do_trace) printf

YYSTYPE yylval;

/*
 * Key word table
 */

struct kt {
	char *kt_name;
	int kt_val;
} key_words[] = {
	"cpu", CPU, "ident", IDENT, "config", CONFIG, "options", OPTIONS,
	"device", DEVICE, "controller", CONTROLLER, "uba", UBA, "mba", MBA,
	"csr", CSR, "nexus", NEXUS, "drive", DRIVE, "vector", VECTOR,
	"pseudo-device", PSEUDO_DEVICE, "flags", FLAGS, "trace", TRACE,
	"disk", DISK, "tape", DEVICE, "slave", SLAVE, "at", AT,
	"hz", HZ, "timezone", TIMEZONE, "dst", DST, "maxusers", MAXUSERS,
	"master", MASTER, "makefile", MAKEFILE,
	0,0,
};
%}
WORD	[A-Za-z_][-A-Za-z_]*
%%
{WORD}		{
			int i;

			if ((i = kw_lookup(yytext)) == -1)
			{
				yylval.cp = yytext;
				tprintf("id(%s) ", yytext);
				return ID;
			}
			tprintf("(%s) ", yytext);
			return i;
		}
\"[^"]+\"	{
			yytext[strlen(yytext)-1] = '\0';
			yylval.cp = yytext + 1;
			return ID;
		}
0[0-7]*		{
			yylval.i = octal(yytext);
			tprintf("#O:%o ", yylval.i);
			return NUMBER;
		}
0x[0-9a-f]+	{
			yylval.i = hex(yytext);
			tprintf("#X:%x ", yylval.i);
			return NUMBER;
		}
[1-9][0-9]*	{
			yylval.i = atoi(yytext);
			tprintf("#D:%d ", yylval.i);
			return NUMBER;
		}
[0-9]"."[0-9]*	{
			double atof();
			yylval.i = (int) (60 * atof(yytext) + 0.5);
			return FPNUMBER;
		}
"-"		{
			return MINUS;
		}
"?"		{
			yylval.i = QUES;
			tprintf("? ");
			return NUMBER;
		}
\n/[ \t]	{
			yyline++;
			tprintf("\n... ");
		}
\n		{
			yyline++;
			tprintf("\n");
			return SEMICOLON;
		}
^#.*		{	/* Ignored (comment) */;	}
[ \t]*		{	/* Ignored (white space) */;	}
";"		{	return SEMICOLON;		}
","		{	return COMMA;			}
%%
/*
 * kw_lookup
 *	Look up a string in the keyword table.  Returns a -1 if the
 *	string is not a keyword otherwise it returns the keyword number
 */

kw_lookup(word)
register char *word;
{
	register struct kt *kp;

	for (kp = key_words; kp->kt_name != 0; kp++)
		if (eq(word, kp->kt_name))
			return kp->kt_val;
	return -1;
}

/*
 * Number conversion routines
 */

octal(str)
char *str;
{
	int num;

	sscanf(str, "%o", &num);
	return num;
}

hex(str)
char *str;
{
	int num;

	sscanf(str+2, "%x", &num);
	return num;
}
