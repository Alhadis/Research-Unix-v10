# include "stdio.h"
# define U(x) x
# define NLSTATE yyprevious=YYNEWLINE
# define BEGIN yybgin = yysvec + 1 +
# define INITIAL 0
# define YYLERR yysvec
# define YYSTATE (yyestate-yysvec-1)
# define YYOPTIM 1
# define YYLMAX 200
# define output(c) putc(c,yyout)
# define input() (((yytchar=yysptr>yysbuf?U(*--yysptr):getc(yyin))==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar)
# define unput(c) {yytchar= (c);if(yytchar=='\n')yylineno--;*yysptr++=yytchar;}
# define yymore() (yymorfg=1)
# define ECHO fprintf(yyout, "%s",yytext)
# define REJECT { nstr = yyreject(); goto yyfussy;}
int yyleng; extern char yytext[];
int yymorfg;
extern char *yysptr, yysbuf[];
int yytchar;
FILE *yyin = {stdin}, *yyout = {stdout};
extern int yylineno;
struct yysvf { 
	struct yywork *yystoff;
	struct yysvf *yyother;
	int *yystops;};
struct yysvf *yyestate;
extern struct yysvf yysvec[], *yybgin;

/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef lint
static char sccsid[] = "@(#)scan.l	5.6 (Berkeley) 6/18/88";
#endif /* not lint */

#include "htable.h"
# define YYNEWLINE 10
yylex(){
int nstr; extern int yyprevious;
while((nstr = yylook()) >= 0)
yyfussy: switch(nstr){
case 0:
if(yywrap()) return(0); break;
case 1:
	{
			yylval.number = KW_NET;
			return (KEYWORD);
		}
break;
case 2:
{
			yylval.number = KW_GATEWAY;
			return (KEYWORD);
		}
break;
case 3:
	{
			yylval.number = KW_HOST;
			return (KEYWORD);
		}
break;
case 4:
{
			yylval.namelist = newname(yytext);
			return (NAME);
		}
break;
case 5:
	{
			yylval.namelist = newname(yytext);
			return (NAME);
		}
break;
case 6:
{
			fprintf(stderr, "Warning: nonstandard name \"%s\"\n",
				yytext);
			yylval.namelist = newname(yytext);
			return (NAME);
		}
break;
case 7:
{
			yylval.number = atoi(yytext);
			return (NUMBER);
		}
break;
case 8:
	return ('.');
break;
case 9:
	return (':');
break;
case 10:
	return (',');
break;
case 11:
	return ('/');
break;
case 12:
	;
break;
case 13:
;
break;
case 14:
;
break;
case 15:
	return (END);
break;
case 16:
	fprintf(stderr, "Illegal char: '%s'\n", yytext);
break;
case -1:
break;
default:
fprintf(yyout,"bad switch yylook %d",nstr);
} return(0); }
/* end of yylex */

yywrap()
{
	return (1);
}
int yyvstop[] = {
0,

16,
0,

14,
16,
0,

15,
0,

10,
16,
0,

8,
16,
0,

11,
16,
0,

7,
16,
0,

9,
16,
0,

12,
16,
0,

5,
16,
0,

5,
16,
0,

5,
16,
0,

5,
16,
0,

14,
0,

13,
0,

7,
0,

6,
0,

12,
0,

4,
0,

4,
0,

4,
0,

4,
0,

4,
0,

4,
0,

1,
4,
0,

4,
0,

3,
4,
0,

4,
0,

4,
0,

2,
4,
0,
0};
# define YYTYPE char
struct yywork { YYTYPE verify, advance; } yycrank[] = {
0,0,	0,0,	1,3,	0,0,	
0,0,	0,0,	0,0,	11,20,	
0,0,	0,0,	1,4,	1,5,	
4,16,	5,17,	20,0,	11,20,	
11,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	4,16,	
5,17,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	1,6,	1,3,	1,7,	
1,8,	1,9,	2,6,	11,20,	
2,7,	2,8,	11,20,	0,0,	
0,0,	0,0,	0,0,	1,10,	
1,11,	0,0,	0,0,	0,0,	
2,10,	2,11,	1,12,	13,23,	
31,32,	0,0,	15,25,	11,20,	
1,13,	1,14,	26,29,	0,0,	
0,0,	2,13,	2,14,	1,15,	
14,24,	0,0,	0,0,	0,0,	
2,15,	9,18,	9,18,	9,18,	
9,18,	9,18,	9,18,	9,18,	
9,18,	9,18,	9,18,	23,26,	
24,27,	25,28,	27,30,	29,31,	
32,33,	0,0,	9,19,	9,19,	
9,19,	9,19,	9,19,	9,19,	
9,19,	9,19,	9,19,	9,19,	
9,19,	9,19,	9,19,	9,19,	
9,19,	9,19,	9,19,	9,19,	
9,19,	9,19,	9,19,	9,19,	
9,19,	9,19,	9,19,	9,19,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	9,19,	9,19,	
9,19,	9,19,	9,19,	9,19,	
9,19,	9,19,	9,19,	9,19,	
9,19,	9,19,	9,19,	9,19,	
9,19,	9,19,	9,19,	9,19,	
9,19,	9,19,	9,19,	9,19,	
9,19,	9,19,	9,19,	9,19,	
12,21,	12,21,	12,21,	12,22,	
12,22,	12,22,	12,22,	12,22,	
12,22,	12,22,	12,22,	12,22,	
12,22,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
12,22,	12,22,	12,22,	12,22,	
12,22,	12,22,	12,22,	12,22,	
12,22,	12,22,	12,22,	12,22,	
12,22,	12,22,	12,22,	12,22,	
12,22,	12,22,	12,22,	12,22,	
12,22,	12,22,	12,22,	12,22,	
12,22,	12,22,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
12,22,	12,22,	12,22,	12,22,	
12,22,	12,22,	12,22,	12,22,	
12,22,	12,22,	12,22,	12,22,	
12,22,	12,22,	12,22,	12,22,	
12,22,	12,22,	12,22,	12,22,	
12,22,	12,22,	12,22,	12,22,	
12,22,	12,22,	19,19,	19,19,	
19,19,	19,19,	19,19,	19,19,	
19,19,	19,19,	19,19,	19,19,	
19,19,	19,19,	19,19,	0,0,	
0,0};
struct yysvf yysvec[] = {
0,	0,	0,
yycrank+-1,	0,		0,	
yycrank+-6,	yysvec+1,	0,	
yycrank+0,	0,		yyvstop+1,
yycrank+3,	0,		yyvstop+3,
yycrank+4,	0,		yyvstop+6,
yycrank+0,	0,		yyvstop+8,
yycrank+0,	0,		yyvstop+11,
yycrank+0,	0,		yyvstop+14,
yycrank+37,	0,		yyvstop+17,
yycrank+0,	0,		yyvstop+20,
yycrank+-6,	0,		yyvstop+23,
yycrank+115,	0,		yyvstop+26,
yycrank+2,	yysvec+12,	yyvstop+29,
yycrank+1,	yysvec+12,	yyvstop+32,
yycrank+1,	yysvec+12,	yyvstop+35,
yycrank+0,	yysvec+4,	yyvstop+38,
yycrank+0,	yysvec+5,	yyvstop+40,
yycrank+0,	yysvec+9,	yyvstop+42,
yycrank+193,	yysvec+9,	yyvstop+44,
yycrank+-4,	yysvec+11,	yyvstop+46,
yycrank+0,	yysvec+12,	0,	
yycrank+0,	yysvec+12,	yyvstop+48,
yycrank+11,	yysvec+12,	yyvstop+50,
yycrank+13,	yysvec+12,	yyvstop+52,
yycrank+13,	yysvec+12,	yyvstop+54,
yycrank+5,	yysvec+12,	yyvstop+56,
yycrank+14,	yysvec+12,	yyvstop+58,
yycrank+0,	yysvec+12,	yyvstop+60,
yycrank+12,	yysvec+12,	yyvstop+63,
yycrank+0,	yysvec+12,	yyvstop+65,
yycrank+3,	yysvec+12,	yyvstop+68,
yycrank+11,	yysvec+12,	yyvstop+70,
yycrank+0,	yysvec+12,	yyvstop+72,
0,	0,	0};
struct yywork *yytop = yycrank+250;
struct yysvf *yybgin = yysvec+1;
char yymatch[] = {
00  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,011 ,012 ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
011 ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,'-' ,'-' ,'-' ,
'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,
'0' ,'0' ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,01  ,01  ,01  ,01  ,01  ,
01  ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,01  ,01  ,01  ,01  ,01  ,
0};
char yyextra[] = {
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0};
int yylineno =1;
# define YYU(x) x
# define NLSTATE yyprevious=YYNEWLINE
char yytext[YYLMAX];
struct yysvf *yylstate [YYLMAX], **yylsp, **yyolsp;
char yysbuf[YYLMAX];
char *yysptr = yysbuf;
int *yyfnd;
extern struct yysvf *yyestate;
int yyprevious = YYNEWLINE;
yylook(){
	register struct yysvf *yystate, **lsp;
	register struct yywork *yyt;
	struct yysvf *yyz;
	int yych;
	struct yywork *yyr;
# ifdef LEXDEBUG
	int debug;
# endif
	char *yylastch;
	/* start off machines */
# ifdef LEXDEBUG
	debug = 0;
# endif
	if (!yymorfg)
		yylastch = yytext;
	else {
		yymorfg=0;
		yylastch = yytext+yyleng;
		}
	for(;;){
		lsp = yylstate;
		yyestate = yystate = yybgin;
		if (yyprevious==YYNEWLINE) yystate++;
		for (;;){
# ifdef LEXDEBUG
			if(debug)fprintf(yyout,"state %d\n",yystate-yysvec-1);
# endif
			yyt = yystate->yystoff;
			if(yyt == yycrank){		/* may not be any transitions */
				yyz = yystate->yyother;
				if(yyz == 0)break;
				if(yyz->yystoff == yycrank)break;
				}
			*yylastch++ = yych = input();
		tryagain:
# ifdef LEXDEBUG
			if(debug){
				fprintf(yyout,"char ");
				allprint(yych);
				putchar('\n');
				}
# endif
			yyr = yyt;
			if ( (int)yyt > (int)yycrank){
				yyt = yyr + yych;
				if (yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transitions */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					goto contin;
					}
				}
# ifdef YYOPTIM
			else if((int)yyt < (int)yycrank) {		/* r < yycrank */
				yyt = yyr = yycrank+(yycrank-yyt);
# ifdef LEXDEBUG
				if(debug)fprintf(yyout,"compressed state\n");
# endif
				yyt = yyt + yych;
				if(yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transitions */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					goto contin;
					}
				yyt = yyr + YYU(yymatch[yych]);
# ifdef LEXDEBUG
				if(debug){
					fprintf(yyout,"try fall back character ");
					allprint(YYU(yymatch[yych]));
					putchar('\n');
					}
# endif
				if(yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transition */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					goto contin;
					}
				}
			if ((yystate = yystate->yyother) && (yyt= yystate->yystoff) != yycrank){
# ifdef LEXDEBUG
				if(debug)fprintf(yyout,"fall back to state %d\n",yystate-yysvec-1);
# endif
				goto tryagain;
				}
# endif
			else
				{unput(*--yylastch);break;}
		contin:
# ifdef LEXDEBUG
			if(debug){
				fprintf(yyout,"state %d char ",yystate-yysvec-1);
				allprint(yych);
				putchar('\n');
				}
# endif
			;
			}
# ifdef LEXDEBUG
		if(debug){
			fprintf(yyout,"stopped at %d with ",*(lsp-1)-yysvec-1);
			allprint(yych);
			putchar('\n');
			}
# endif
		while (lsp-- > yylstate){
			*yylastch-- = 0;
			if (*lsp != 0 && (yyfnd= (*lsp)->yystops) && *yyfnd > 0){
				yyolsp = lsp;
				if(yyextra[*yyfnd]){		/* must backup */
					while(yyback((*lsp)->yystops,-*yyfnd) != 1 && lsp > yylstate){
						lsp--;
						unput(*yylastch--);
						}
					}
				yyprevious = YYU(*yylastch);
				yylsp = lsp;
				yyleng = yylastch-yytext+1;
				yytext[yyleng] = 0;
# ifdef LEXDEBUG
				if(debug){
					fprintf(yyout,"\nmatch ");
					sprint(yytext);
					fprintf(yyout," action %d\n",*yyfnd);
					}
# endif
				return(*yyfnd++);
				}
			unput(*yylastch);
			}
		if (yytext[0] == 0  /* && feof(yyin) */)
			{
			yysptr=yysbuf;
			return(0);
			}
		yyprevious = yytext[0] = input();
		if (yyprevious>0)
			output(yyprevious);
		yylastch=yytext;
# ifdef LEXDEBUG
		if(debug)putchar('\n');
# endif
		}
	}
yyback(p, m)
	int *p;
{
if (p==0) return(0);
while (*p)
	{
	if (*p++ == m)
		return(1);
	}
return(0);
}
	/* the following are only used in the lex library */
yyinput(){
	return(input());
	}
yyoutput(c)
  int c; {
	output(c);
	}
yyunput(c)
   int c; {
	unput(c);
	}
