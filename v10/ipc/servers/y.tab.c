
# line 7 "ftpcmd.y"

#ifndef lint
static char sccsid[] = "@(#)ftpcmd.y	4.11 83/06/22";
#endif

#include <sys/types.h>

#include <sys/inet/in.h>

#include "ftp.h"

#include <stdio.h>
#include <signal.h>
#include <ctype.h>
#include <pwd.h>
#include <setjmp.h>

struct socket {
	unsigned short sport;
	long saddr;
};
char dest[128];
extern	int logged_in;
extern	struct passwd *pw;
extern	int logging;
extern	int type;
extern	int form;
extern	int debug;
extern	int timeout;
extern	char hostname[];
extern	char *globerr;
extern	int usedefault;
extern	int pasv;
char	**glob();

static	int cmd_type;
static	int cmd_form;
static	int cmd_bytesz;

char	*strchr();
# define A 257
# define B 258
# define C 259
# define E 260
# define F 261
# define I 262
# define L 263
# define N 264
# define P 265
# define R 266
# define S 267
# define T 268
# define SP 269
# define CRLF 270
# define COMMA 271
# define STRING 272
# define NUMBER 273
# define USER 274
# define PASS 275
# define ACCT 276
# define REIN 277
# define QUIT 278
# define PORT 279
# define PASV 280
# define TYPE 281
# define STRU 282
# define MODE 283
# define RETR 284
# define STOR 285
# define APPE 286
# define MLFL 287
# define MAIL 288
# define MSND 289
# define MSOM 290
# define MSAM 291
# define MRSQ 292
# define MRCP 293
# define ALLO 294
# define REST 295
# define RNFR 296
# define RNTO 297
# define ABOR 298
# define DELE 299
# define CWD 300
# define LIST 301
# define NLST 302
# define SITE 303
# define STAT 304
# define HELP 305
# define NOOP 306
# define XMKD 307
# define XRMD 308
# define XPWD 309
# define XCUP 310
# define OPEN 311
# define CLOS 312
# define READ 313
# define WRIT 314
# define SEEK 315
# define LEXERR 316
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern short yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
#ifndef YYSTYPE
#define YYSTYPE int
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256

# line 430 "ftpcmd.y"


extern jmp_buf errcatch;

#define	CMD	0	/* beginniAg of command */
#define	ARGS	1	/* expect miscellaneous arguments */
#define	STR1	2	/* expect SP followed by STRING */
#define	STR2	3	/* expect STRING */
#define	OSTR	4	/* optional STRING */

struct tab {
	char	*name;
	short	token;
	short	state;
	short	implemented;	/* 1 if command is implemented */
	char	*help;
};

struct tab cmdtab[] = {		/* In order defined in RFC 765 */
	{ "USER", USER, STR1, 1,	"<sp> username" },
	{ "PASS", PASS, STR1, 1,	"<sp> password" },
	{ "ACCT", ACCT, STR1, 0,	"(specify account)" },
	{ "REIN", REIN, ARGS, 0,	"(reinitialize server state)" },
	{ "QUIT", QUIT, ARGS, 1,	"(terminate service)", },
	{ "PORT", PORT, ARGS, 1,	"<sp> b0, b1, b2, b3, b4" },
	{ "PASV", PASV, ARGS, 1,	"(set server in passive mode)" },
	{ "TYPE", TYPE, ARGS, 1,	"<sp> [ A | E | I | L ]" },
	{ "STRU", STRU, ARGS, 1,	"(specify file structure)" },
	{ "MODE", MODE, ARGS, 1,	"(specify transfer mode)" },
	{ "RETR", RETR, STR1, 1,	"<sp> file-name" },
	{ "STOR", STOR, STR1, 1,	"<sp> file-name" },
	{ "APPE", APPE, STR1, 1,	"<sp> file-name" },
	{ "MLFL", MLFL, OSTR, 0,	"(mail file)" },
	{ "MAIL", MAIL, OSTR, 0,	"(mail to user)" },
	{ "MSND", MSND, OSTR, 0,	"(mail send to terminal)" },
	{ "MSOM", MSOM, OSTR, 0,	"(mail send to terminal or mailbox)" },
	{ "MSAM", MSAM, OSTR, 0,	"(mail send to terminal and mailbox)" },
	{ "MRSQ", MRSQ, OSTR, 0,	"(mail recipient scheme question)" },
	{ "MRCP", MRCP, STR1, 0,	"(mail recipient)" },
	{ "ALLO", ALLO, ARGS, 1,	"allocate storage (vacuously)" },
	{ "REST", REST, STR1, 0,	"(restart command)" },
	{ "RNFR", RNFR, STR1, 1,	"<sp> file-name" },
	{ "RNTO", RNTO, STR1, 1,	"<sp> file-name" },
	{ "ABOR", ABOR, ARGS, 0,	"(abort operation)" },
	{ "DELE", DELE, STR1, 1,	"<sp> file-name" },
	{ "CWD",  CWD,  OSTR, 1,	"[ <sp> directory-name]" },
	{ "XCWD", CWD,	OSTR, 1,	"[ <sp> directory-name ]" },
	{ "LIST", LIST, OSTR, 1,	"[ <sp> path-name ]" },
	{ "NLST", NLST, OSTR, 1,	"[ <sp> path-name ]" },
	{ "SITE", SITE, STR1, 0,	"(get site parameters)" },
	{ "STAT", STAT, OSTR, 0,	"(get server status)" },
	{ "HELP", HELP, OSTR, 1,	"[ <sp> <string> ]" },
	{ "NOOP", NOOP, ARGS, 1,	"" },
	{ "XMKD", XMKD, STR1, 1,	"<sp> path-name" },
	{ "XRMD", XRMD, STR1, 1,	"<sp> path-name" },
	{ "XPWD", XPWD, ARGS, 1,	"(return current directory)" },
	{ "XCUP", XCUP, ARGS, 1,	"(change to parent directory)" },
	{ "OPEN", OPEN, STR1, 1,	"[ <sp> path-name ]" },
	{ "CLOS", CLOS, ARGS, 1,	"(close file)" },
	{ "READ", READ, ARGS, 1,	"(read from file)" },
	{ "WRIT", WRIT, ARGS, 1,	"(writ from file)" },
	{ "SEEK", SEEK, ARGS, 1,	"(seek in file)" },
	{ NULL,   0,    0,    0,	0 }
};

struct tab *
lookup(cmd)
	char *cmd;
{
	register struct tab *p;

	for (p = cmdtab; p->name != NULL; p++)
		if (strcmp(cmd, p->name) == 0)
			return (p);
	return (0);
}

#include "telnet.h"

/*
 * getline - a hacked up version of fgets to ignore TELNET escape codes.
 */
char *
getline(s, n, iop)
	char *s;
	register FILE *iop;
{
	register c;
	register char *cs;

	cs = s;
	while (--n > 0 && (c = getc(iop)) >= 0) {
		while (c == IAC) {
			c = getc(iop);	/* skip command */
			c = getc(iop);	/* try next char */
		}
		*cs++ = c;
		if (c=='\n')
			break;
	}
	if (c < 0 && cs == s)
		return (NULL);
	*cs++ = '\0';
	if (debug) {
		fprintf(stderr, "FTPD: command: %s", s);
		if (c != '\n')
			putc('\n', stderr);
		fflush(stderr);
	}
	return (s);
}

static int
toolong()
{
	long now;
	extern char *ctime();

	reply(421,
	  "Timeout (%d seconds): closing control connection.", timeout);
	time(&now);
	if (logging) {
		fprintf(stderr,
			"FTPD: User %s timed out after %d seconds at %s",
			(pw ? pw -> pw_name : "unknown"), timeout, ctime(&now));
		fflush(stderr);
	}
	dologout(1);
}

yylex()
{
	static char cbuf[512];
	static int cpos, state;
	register char *cp;
	register struct tab *p;
	int n;
	char c;

	for (;;) {
		switch (state) {

		case CMD:
			signal(SIGALRM, toolong);
			alarm(timeout);
			if (getline(cbuf, sizeof(cbuf)-1, stdin) == NULL) {
				reply(221, "You could at least say goodbye.");
				dologout(0);
			}
			alarm(0);
			if (strchr(cbuf, '\r')) {
				cp = strchr(cbuf, '\r');
				cp[0] = '\n'; cp[1] = 0;
			}
			if (strchr(cbuf, ' '))
				cpos = strchr(cbuf, ' ') - cbuf;
			else
				cpos = 4;
			c = cbuf[cpos];
			cbuf[cpos] = '\0';
			upper(cbuf);
			p = lookup(cbuf);
			cbuf[cpos] = c;
			if (p != 0) {
				if (p->implemented == 0) {
					nack(p->name);
					longjmp(errcatch);
					/* NOTREACHED */
				}
				state = p->state;
				yylval = (int) p->name;
				return (p->token);
			}
			break;

		case OSTR:
			if (cbuf[cpos] == '\n') {
				state = CMD;
				return (CRLF);
			}
			/* FALL THRU */

		case STR1:
			if (cbuf[cpos] == ' ') {
				cpos++;
				state = STR2;
				return (SP);
			}
			break;

		case STR2:
			cp = &cbuf[cpos];
			n = strlen(cp);
			cpos += n - 1;
			/*
			 * Make sure the string is nonempty and \n terminated.
			 */
			if (n > 1 && cbuf[cpos] == '\n') {
				cbuf[cpos] = '\0';
				yylval = copy(cp);
				cbuf[cpos] = '\n';
				state = ARGS;
				return (STRING);
			}
			break;

		case ARGS:
			if (isdigit(cbuf[cpos])) {
				cp = &cbuf[cpos];
				while (isdigit(cbuf[++cpos]))
					;
				c = cbuf[cpos];
				cbuf[cpos] = '\0';
				yylval = atoi(cp);
				cbuf[cpos] = c;
				return (NUMBER);
			}
			switch (cbuf[cpos++]) {

			case '\n':
				state = CMD;
				return (CRLF);

			case ' ':
				return (SP);

			case ',':
				return (COMMA);

			case 'A':
			case 'a':
				return (A);

			case 'B':
			case 'b':
				return (B);

			case 'C':
			case 'c':
				return (C);

			case 'E':
			case 'e':
				return (E);

			case 'F':
			case 'f':
				return (F);

			case 'I':
			case 'i':
				return (I);

			case 'L':
			case 'l':
				return (L);

			case 'N':
			case 'n':
				return (N);

			case 'P':
			case 'p':
				return (P);

			case 'R':
			case 'r':
				return (R);

			case 'S':
			case 's':
				return (S);

			case 'T':
			case 't':
				return (T);

			}
			break;

		default:
			fatal("Unknown state in scanner.");
		}
		yyerror();
		state = CMD;
		longjmp(errcatch);
	}
}

upper(s)
	char *s;
{
	while (*s != '\0') {
		if (islower(*s))
			*s = toupper(*s);
		s++;
	}
}

copy(s)
	char *s;
{
	char *p;
	extern char *malloc();

	p = malloc(strlen(s) + 1);
	if (p == NULL)
		fatal("Ran out of memory.");
	strcpy(p, s);
	return ((int)p);
}

help(s)
	char *s;
{
	register struct tab *c;
	register int width, NCMDS;

	width = 0, NCMDS = 0;
	for (c = cmdtab; c->name != NULL; c++) {
		int len = strlen(c->name);

		if (c->implemented == 0)
			len++;
		if (len > width)
			width = len;
		NCMDS++;
	}
	width = (width + 8) &~ 7;
	if (s == 0) {
		register int i, j, w;
		int columns, lines;

		lreply(214,
	  "The following commands are recognized (* =>'s unimplemented).");
		columns = 76 / width;
		if (columns == 0)
			columns = 1;
		lines = (NCMDS + columns - 1) / columns;
		for (i = 0; i < lines; i++) {
			printf("    ");
			for (j = 0; j < columns; j++) {
				c = cmdtab + j * lines + i;
				printf("%s%c", c->name,
					c->implemented ? ' ' : '*');
				if (c + lines >= &cmdtab[NCMDS])
					break;
				w = strlen(c->name);
				while (w < width) {
					putchar(' ');
					w++;
				}
			}
			printf("\r\n");
		}
		fflush(stdout);
		reply(214, "Direct comments to ftp-bugs@%s.", hostname);
		return;
	}
	upper(s);
	c = lookup(s);
	if (c == (struct tab *)0) {
		reply(504, "Unknown command %s.", s);
		return;
	}
	if (c->implemented)
		reply(214, "Syntax: %s %s", c->name, c->help);
	else
		reply(214, "%-*s\t%s; unimplemented.", width, c->name, c->help);
}
short yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 59
# define YYLAST 215
short yyact[]={

  27,  56, 103, 150, 148, 146, 105, 144, 105, 124,
  77,  63, 111,  89,  61,  59, 149, 147,   3,   4,
 145, 143,  26,   5,  21,   6,   7,   8,  10,  12,
  13,  99,  88,  87,  85,  84,  83,  82,   9, 142,
  29,  46,  45,  16,  17,  15,  14, 141, 140,  19,
  20,  22,  23,  24,  25,  11, 139, 138, 137, 136,
 135, 134, 133, 132, 131, 128, 119, 108, 107, 106,
 126, 100,  98,  97, 127,  96, 109,  93,  92,  54,
  53,  48,  47, 102, 101,  95,  94,  91,  90,  86,
  81,  80,  79,  78,  75,  76,  36,  35,  34,  33,
  32,  31,  30,  74,  70, 104,  37,  65,  72,  71,
  66, 125,  67,  68,  55,  28, 110,  18,  38,  39,
  40,  41,  42,  43,  44,  73,  69,  64,  62,  49,
  50,  51,  52,  60,  58,   2,  57,   1,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0, 112, 113, 114,   0,
 115,   0, 116, 117,   0, 118,   0, 120, 121,   0,
   0, 122, 123,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0, 130,
   0,   0,   0,   0, 129 };
short yypact[]={

-1000,-256,-1000,-167,-168,-169,-170,-171,-172,-173,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-228,
-188,-189,-1000,-1000,-1000,-1000,-190,-191,-296,-1000,
-257,-258,-262,-150,-157,-164,-263,-176,-177,-178,
-179,-233,-235,-180,-237,-1000,-259,-1000,-1000,-181,
-182,-192,-193,-1000,-1000,-1000,-183,-184,-195,-1000,
-197,-1000,-198,-240,-199,-185,-186,-1000,-267,-201,
-1000,-1000,-1000,-202,-1000,-1000,-1000,-203,-260,-260,
-260,-260,-1000,-260,-1000,-260,-260,-1000,-260,-204,
-260,-260,-1000,-1000,-260,-260,-1000,-1000,-1000,-264,
-1000,-194,-194,-265,-1000,-1000,-1000,-1000,-1000,-206,
-1000,-1000,-207,-208,-209,-210,-211,-212,-213,-1000,
-214,-222,-223,-231,-250,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-266,-251,-268,-254,-269,-255,-270,
-1000 };
short yypgo[]={

   0, 137, 135, 134, 133, 128, 127, 126, 125, 106,
  76, 117, 105, 111, 116, 115, 114 };
short yyr1[]={

   0,   1,   1,   2,   2,   2,   2,   2,   2,   2,
   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
   2,   2,   3,   4,  12,   5,  13,  13,  13,   6,
   6,   6,   6,   6,   6,   6,   6,   7,   7,   7,
   8,   8,   8,  10,  14,  11,  15,  16,   9 };
short yyr2[]={

   0,   0,   2,   4,   4,   4,   4,   4,   4,   4,
   5,   5,   5,   5,   3,   5,   3,   5,   5,   3,
   5,   1,   2,   4,   2,   2,   5,   5,   3,   3,
   2,   2,   1,   1,   1,  11,   1,   1,   1,   1,
   3,   1,   3,   1,   1,   3,   2,   1,   1,   1,
   1,   1,   1,   1,   1,   2,   5,   4,   0 };
short yychk[]={

-1000,  -1,  -2, 274, 275, 279, 281, 282, 283, 294,
 284, 311, 285, 286, 302, 301, 299, 300, -11, 305,
 306, 280, 307, 308, 309, 310, 278, 256, -15, 296,
 269, 269, 269, 269, 269, 269, 269,  -9,  -9,  -9,
  -9,  -9,  -9,  -9,  -9, 270, 269, 270, 270,  -9,
  -9,  -9,  -9, 270, 270, -16, 297,  -9,  -3, 272,
  -4, 272,  -5, 273,  -6, 257, 260, 262, 263,  -7,
 261, 266, 265,  -8, 267, 258, 259, 273, 269, 269,
 269, 269, 270, 269, 270, 269, 269, 270, 269, 272,
 269, 269, 270, 270, 269, 269, 270, 270, 270, 271,
 270, 269, 269, 269, -12, 273, 270, 270, 270, -10,
 -14, 272, -10, -10, -10, -10, -10, -10, -10, 270,
 -10, -10, -10, -10, 273, -13, 264, 268, 259, -13,
 -12, 270, 270, 270, 270, 270, 270, 270, 270, 270,
 270, 270, 270, 271, 273, 271, 273, 271, 273, 271,
 273 };
short yydef[]={

   1,  -2,   2,   0,   0,   0,   0,   0,   0,   0,
  58,  58,  58,  58,  58,  58,  58,  58,  21,   0,
   0,   0,  58,  58,  58,  58,   0,   0,   0,  58,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,  22,   0,  24,  25,   0,
   0,   0,   0,  30,  31,  55,   0,   0,   0,  32,
   0,  33,   0,   0,   0,  39,  41,  43,  44,   0,
  47,  48,  49,   0,  50,  51,  52,   0,   0,   0,
   0,   0,  14,   0,  16,   0,   0,  19,   0,   0,
   0,   0,  28,  29,   0,   0,   3,   4,   5,   0,
   6,   0,   0,   0,  46,  34,   7,   8,   9,   0,
  53,  54,   0,   0,   0,   0,   0,   0,   0,  23,
   0,   0,   0,   0,   0,  40,  36,  37,  38,  42,
  45,  10,  11,  12,  13,  15,  17,  18,  20,  26,
  27,  57,  56,   0,   0,   0,   0,   0,   0,   0,
  35 };
# ifdef YYDEBUG
# include "y.debug"
# endif

# define YYFLAG -1000
# define YYERROR goto yyerrlab
# define YYACCEPT return(0)
# define YYABORT return(1)

/*	parser for yacc output	*/

#ifdef YYDEBUG
int yydebug = 0; /* 1 for debugging */
#endif
YYSTYPE yyv[YYMAXDEPTH]; /* where the values are stored */
int yychar = -1; /* current input token number */
int yynerrs = 0;  /* number of errors */
short yyerrflag = 0;  /* error recovery flag */

yyparse()
{	short yys[YYMAXDEPTH];
	int yyj, yym;
	register YYSTYPE *yypvt;
	register int yystate, yyn;
	register short *yyps;
	register YYSTYPE *yypv;
	register short *yyxi;

	yystate = 0;
	yychar = -1;
	yynerrs = 0;
	yyerrflag = 0;
	yyps= &yys[-1];
	yypv= &yyv[-1];

yystack:    /* put a state and value onto the stack */
#ifdef YYDEBUG
	if(yydebug >= 3)
		if(yychar < 0 || yytoknames[yychar] == 0)
			printf("char %d in %s", yychar, yystates[yystate]);
		else
			printf("%s in %s", yytoknames[yychar], yystates[yystate]);
#endif
	if( ++yyps >= &yys[YYMAXDEPTH] ) { 
		yyerror( "yacc stack overflow" ); 
		return(1); 
	}
	*yyps = yystate;
	++yypv;
	*yypv = yyval;
yynewstate:
	yyn = yypact[yystate];
	if(yyn <= YYFLAG) goto yydefault; /* simple state */
	if(yychar<0) {
		yychar = yylex();
#ifdef YYDEBUG
		if(yydebug >= 2) {
			if(yychar <= 0)
				printf("lex EOF\n");
			else if(yytoknames[yychar])
				printf("lex %s\n", yytoknames[yychar]);
			else
				printf("lex (%c)\n", yychar);
		}
#endif
		if(yychar < 0)
			yychar = 0;
	}
	if((yyn += yychar) < 0 || yyn >= YYLAST)
		goto yydefault;
	if( yychk[ yyn=yyact[ yyn ] ] == yychar ){ /* valid shift */
		yychar = -1;
		yyval = yylval;
		yystate = yyn;
		if( yyerrflag > 0 ) --yyerrflag;
		goto yystack;
	}
yydefault:
	/* default state action */
	if( (yyn=yydef[yystate]) == -2 ) {
		if(yychar < 0) {
			yychar = yylex();
#ifdef YYDEBUG
			if(yydebug >= 2)
				if(yychar < 0)
					printf("lex EOF\n");
				else
					printf("lex %s\n", yytoknames[yychar]);
#endif
			if(yychar < 0)
				yychar = 0;
		}
		/* look through exception table */
		for(yyxi=yyexca; (*yyxi!= (-1)) || (yyxi[1]!=yystate);
			yyxi += 2 ) ; /* VOID */
		while( *(yyxi+=2) >= 0 ){
			if( *yyxi == yychar ) break;
		}
		if( (yyn = yyxi[1]) < 0 ) return(0);   /* accept */
	}
	if( yyn == 0 ){ /* error */
		/* error ... attempt to resume parsing */
		switch( yyerrflag ){
		case 0:   /* brand new error */
#ifdef YYDEBUG
			yyerror("syntax error\n%s", yystates[yystate]);
			if(yytoknames[yychar])
				yyerror("saw %s\n", yytoknames[yychar]);
			else if(yychar >= ' ' && yychar < '\177')
				yyerror("saw `%c'\n", yychar);
			else if(yychar == 0)
				yyerror("saw EOF\n");
			else
				yyerror("saw char 0%o\n", yychar);
#else
			yyerror( "syntax error" );
#endif
yyerrlab:
			++yynerrs;
		case 1:
		case 2: /* incompletely recovered error ... try again */
			yyerrflag = 3;
			/* find a state where "error" is a legal shift action */
			while ( yyps >= yys ) {
				yyn = yypact[*yyps] + YYERRCODE;
				if( yyn>= 0 && yyn < YYLAST && yychk[yyact[yyn]] == YYERRCODE ){
					yystate = yyact[yyn];  /* simulate a shift of "error" */
					goto yystack;
				}
				yyn = yypact[*yyps];
				/* the current yyps has no shift onn "error", pop stack */
#ifdef YYDEBUG
				if( yydebug ) printf( "error recovery pops state %d, uncovers %d\n", *yyps, yyps[-1] );
#endif
				--yyps;
				--yypv;
			}
			/* there is no state on the stack with an error shift ... abort */
yyabort:
			return(1);
		case 3:  /* no shift yet; clobber input char */
#ifdef YYDEBUG
			if( yydebug ) {
				printf("error recovery discards ");
				if(yytoknames[yychar])
					printf("%s\n", yytoknames[yychar]);
				else if(yychar >= ' ' && yychar < '\177')
					printf("`%c'\n", yychar);
				else if(yychar == 0)
					printf("EOF\n");
				else
					printf("char 0%o\n", yychar);
			}
#endif
			if( yychar == 0 ) goto yyabort; /* don't discard EOF, quit */
			yychar = -1;
			goto yynewstate;   /* try again in the same state */
		}
	}
	/* reduction by production yyn */
#ifdef YYDEBUG
	if(yydebug) {	char *s;
		printf("reduce %d in:\n\t", yyn);
		for(s = yystates[yystate]; *s; s++) {
			putchar(*s);
			if(*s == '\n' && *(s+1))
				putchar('\t');
		}
	}
#endif
	yyps -= yyr2[yyn];
	yypvt = yypv;
	yypv -= yyr2[yyn];
	yyval = yypv[1];
	yym=yyn;
	/* consult goto table to find next state */
	yyn = yyr1[yyn];
	yyj = yypgo[yyn] + *yyps + 1;
	if( yyj>=YYLAST || yychk[ yystate = yyact[yyj] ] != -yyn ) yystate = yyact[yypgo[yyn]];
	switch(yym){
		
case 3:
# line 74 "ftpcmd.y"
 {
			extern struct passwd *getpwnam();

			pw = getpwnam(yypvt[-1]);
			reply(331, "Password required for %s.", yypvt[-1]);
			if (pw->pw_uid==0)
				pw = NULL;
			if (pw == NULL)
				reply(530, "User %s unknown.", yypvt[-1]);
			free(yypvt[-1]);
		} break;
case 4:
# line 86 "ftpcmd.y"
 {
			pass(yypvt[-1]);
			free(yypvt[-1]);
		} break;
case 5:
# line 91 "ftpcmd.y"
 {
			usedefault = 0;
			ack(yypvt[-3]);
		} break;
case 6:
# line 96 "ftpcmd.y"
 {
			switch (cmd_type) {

			case TYPE_A:
				if (cmd_form == FORM_N) {
					reply(200, "Type set to A.");
					type = cmd_type;
					form = cmd_form;
				} else
					reply(504, "Form must be N.");
				break;

			case TYPE_E:
				reply(504, "Type E not implemented.");
				break;

			case TYPE_I:
				reply(200, "Type set to I.");
				type = cmd_type;
				break;

			case TYPE_L:
				if (cmd_bytesz == 8) {
					reply(200,
					    "Type set to L (byte size 8).");
					type = cmd_type;
				} else
					reply(504, "Byte size must be 8.");
			}
		} break;
case 7:
# line 127 "ftpcmd.y"
 {
			switch (yypvt[-1]) {

			case STRU_F:
				reply(200, "STRU F ok.");
				break;

			default:
				reply(502, "Unimplemented STRU type.");
			}
		} break;
case 8:
# line 139 "ftpcmd.y"
 {
			switch (yypvt[-1]) {

			case MODE_S:
				reply(200, "MODE S ok.");
				break;

			default:
				reply(502, "Unimplemented MODE type.");
			}
		} break;
case 9:
# line 151 "ftpcmd.y"
 {
			ack(yypvt[-3]);
		} break;
case 10:
# line 155 "ftpcmd.y"
 {
			if (yypvt[-3] && yypvt[-1] != NULL)
				retrieve(0, yypvt[-1]);
			if (yypvt[-1] != NULL)
				free(yypvt[-1]);
		} break;
case 11:
# line 162 "ftpcmd.y"
 {
			if (yypvt[-3] && yypvt[-1] != NULL)
				openfile(yypvt[-1]);
			if (yypvt[-1] != NULL)
				free(yypvt[-1]);
		} break;
case 12:
# line 169 "ftpcmd.y"
 {
			if (yypvt[-3] && yypvt[-1] != NULL)
				store(yypvt[-1], "w");
			if (yypvt[-1] != NULL)
				free(yypvt[-1]);
		} break;
case 13:
# line 176 "ftpcmd.y"
 {
			if (yypvt[-3] && yypvt[-1] != NULL)
				store(yypvt[-1], "a");
			if (yypvt[-1] != NULL)
				free(yypvt[-1]);
		} break;
case 14:
# line 183 "ftpcmd.y"
 {
			if (yypvt[-1])
				retrieve("/bin/ls", "");
		} break;
case 15:
# line 188 "ftpcmd.y"
 {
			if (yypvt[-3] && yypvt[-1] != NULL)
				retrieve("/bin/ls %s", yypvt[-1]);
			if (yypvt[-1] != NULL)
				free(yypvt[-1]);
		} break;
case 16:
# line 195 "ftpcmd.y"
 {
			if (yypvt[-1])
				retrieve("/bin/ls -lg", "");
		} break;
case 17:
# line 200 "ftpcmd.y"
 {
			if (yypvt[-3] && yypvt[-1] != NULL)
				retrieve("/bin/ls -lg %s", yypvt[-1]);
			if (yypvt[-1] != NULL)
				free(yypvt[-1]);
		} break;
case 18:
# line 207 "ftpcmd.y"
 {
			if (yypvt[-3] && yypvt[-1] != NULL)
				delete(yypvt[-1]);
			if (yypvt[-1] != NULL)
				free(yypvt[-1]);
		} break;
case 19:
# line 214 "ftpcmd.y"
 {
			if (yypvt[-1])
				cwd(pw->pw_dir);
		} break;
case 20:
# line 219 "ftpcmd.y"
 {
			if (yypvt[-3] && yypvt[-1] != NULL)
				cwd(yypvt[-1]);
			if (yypvt[-1] != NULL)
				free(yypvt[-1]);
		} break;
case 22:
# line 227 "ftpcmd.y"
 {
			help(0);
		} break;
case 23:
# line 231 "ftpcmd.y"
 {
			help(yypvt[-1]);
		} break;
case 24:
# line 235 "ftpcmd.y"
 {
			ack(yypvt[-1]);
		} break;
case 25:
# line 239 "ftpcmd.y"
 {
			pasv++;
			ack(yypvt[-1]);
		} break;
case 26:
# line 244 "ftpcmd.y"
 {
			if (yypvt[-3] && yypvt[-1] != NULL)
				makedir(yypvt[-1]);
			if (yypvt[-1] != NULL)
				free(yypvt[-1]);
		} break;
case 27:
# line 251 "ftpcmd.y"
 {
			if (yypvt[-3] && yypvt[-1] != NULL)
				removedir(yypvt[-1]);
			if (yypvt[-1] != NULL)
				free(yypvt[-1]);
		} break;
case 28:
# line 258 "ftpcmd.y"
 {
			if (yypvt[-1])
				pwd();
		} break;
case 29:
# line 263 "ftpcmd.y"
 {
			if (yypvt[-1])
				cwd("..");
		} break;
case 30:
# line 268 "ftpcmd.y"
 {
			reply(221, "Goodbye.");
			dologout(0);
		} break;
case 31:
# line 273 "ftpcmd.y"
 {
			yyerrok;
		} break;
case 35:
# line 289 "ftpcmd.y"
 {
			sprintf(dest, "tcp!%d.%d.%d.%d!%d",
				yypvt[-10], yypvt[-8], yypvt[-6], yypvt[-4], yypvt[-2] * 256 + yypvt[-0]);
		} break;
case 36:
# line 296 "ftpcmd.y"
 {
		yyval = FORM_N;
	} break;
case 37:
# line 300 "ftpcmd.y"
 {
		yyval = FORM_T;
	} break;
case 38:
# line 304 "ftpcmd.y"
 {
		yyval = FORM_C;
	} break;
case 39:
# line 310 "ftpcmd.y"
 {
		cmd_type = TYPE_A;
		cmd_form = FORM_N;
	} break;
case 40:
# line 315 "ftpcmd.y"
 {
		cmd_type = TYPE_A;
		cmd_form = yypvt[-0];
	} break;
case 41:
# line 320 "ftpcmd.y"
 {
		cmd_type = TYPE_E;
		cmd_form = FORM_N;
	} break;
case 42:
# line 325 "ftpcmd.y"
 {
		cmd_type = TYPE_E;
		cmd_form = yypvt[-0];
	} break;
case 43:
# line 330 "ftpcmd.y"
 {
		cmd_type = TYPE_I;
	} break;
case 44:
# line 334 "ftpcmd.y"
 {
		cmd_type = TYPE_L;
		cmd_bytesz = 8;
	} break;
case 45:
# line 339 "ftpcmd.y"
 {
		cmd_type = TYPE_L;
		cmd_bytesz = yypvt[-0];
	} break;
case 46:
# line 345 "ftpcmd.y"
 {
		cmd_type = TYPE_L;
		cmd_bytesz = yypvt[-0];
	} break;
case 47:
# line 352 "ftpcmd.y"
 {
		yyval = STRU_F;
	} break;
case 48:
# line 356 "ftpcmd.y"
 {
		yyval = STRU_R;
	} break;
case 49:
# line 360 "ftpcmd.y"
 {
		yyval = STRU_P;
	} break;
case 50:
# line 366 "ftpcmd.y"
 {
		yyval = MODE_S;
	} break;
case 51:
# line 370 "ftpcmd.y"
 {
		yyval = MODE_B;
	} break;
case 52:
# line 374 "ftpcmd.y"
 {
		yyval = MODE_C;
	} break;
case 53:
# line 380 "ftpcmd.y"
 {
		yyval = yypvt[-0];
	} break;
case 55:
# line 389 "ftpcmd.y"
 {
		if (yypvt[-1] && yypvt[-0])
			renamecmd(yypvt[-1], yypvt[-0]);
		else
			reply(503, "Bad sequence of commands.");
		if (yypvt[-1])
			free(yypvt[-1]);
		if (yypvt[-0])
			free(yypvt[-0]);
	} break;
case 56:
# line 402 "ftpcmd.y"
 {
		char *from = 0, *renamefrom();

		if (yypvt[-3] && yypvt[-1])
			from = renamefrom(yypvt[-1]);
		if (from == 0 && yypvt[-1])
			free(yypvt[-1]);
		yyval = (int)from;
	} break;
case 57:
# line 414 "ftpcmd.y"
 {
		yyval = yypvt[-1];
	} break;
case 58:
# line 420 "ftpcmd.y"
 {
		if (logged_in)
			yyval = 1;
		else {
			reply(530, "Please login with USER and PASS.");
			yyval = 0;
		}
	} break;
	}
	goto yystack;  /* stack new state and value */
}
