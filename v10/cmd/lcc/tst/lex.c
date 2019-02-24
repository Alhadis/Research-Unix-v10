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
FILE *yyin ={stdin}, *yyout ={stdout};
extern int yylineno;
struct yysvf { 
	struct yywork *yystoff;
	struct yysvf *yyother;
	int *yystops;};
struct yysvf *yyestate;
extern struct yysvf yysvec[], *yybgin;
enum { Begin=257, End, Id, Const, MulOp, AddOp, AsgnOp };
# define YYNEWLINE 10
yylex(){
int nstr; extern int yyprevious;
while((nstr = yylook()) >= 0)
yyfussy: switch(nstr){
case 0:
if(yywrap()) return(0); break;
case 1:
                return Begin;
break;
case 2:
                  return End;
break;
case 3:
        return Id;
break;
case 4:
                 return Const;
break;
case 5:
              ;
break;
case 6:
 ;
break;
case 7:
          return MulOp;
break;
case 8:
                 return AddOp;
break;
case 9:
                   return AsgnOp;
break;
case 10:
                return yytext[0];
break;
case 11:
              ;
break;
case 12:
                    printf("  ? %s\n", yytext);
break;
case -1:
break;
default:
fprintf(yyout,"bad switch yylook %d",nstr);
} return(0); }
/* end of yylex */
main() {
        int t;
        while (t = yylex())
                printf("%3d %s\n", t, yytext);
        return 0;
}
int yyvstop[] ={
0,

12,
0,

11,
12,
0,

11,
0,

10,
12,
0,

7,
12,
0,

8,
12,
0,

7,
12,
0,

4,
12,
0,

12,
0,

3,
12,
0,

3,
12,
0,

3,
12,
0,

7,
0,

4,
0,

9,
0,

3,
0,

3,
0,

3,
0,

3,
0,

2,
3,
0,

6,
0,

3,
0,

5,
0,

1,
3,
0,
0};
# define YYTYPE char
struct yywork { YYTYPE verify, advance; } yycrank[] ={
0,0,	0,0,	1,3,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,4,	1,5,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	1,6,	0,0,	1,7,	
1,8,	9,15,	0,0,	0,0,	
1,9,	1,10,	9,16,	22,26,	
23,27,	26,22,	0,0,	0,0,	
27,26,	23,28,	26,28,	1,11,	
2,11,	27,30,	11,18,	0,0,	
0,0,	0,0,	1,12,	10,17,	
10,17,	10,17,	10,17,	10,17,	
10,17,	10,17,	10,17,	10,17,	
10,17,	0,0,	30,26,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	12,19,	
12,19,	12,19,	12,19,	12,19,	
12,19,	12,19,	12,19,	12,19,	
12,19,	0,0,	0,0,	1,13,	
2,13,	0,0,	1,14,	2,14,	
12,19,	12,19,	12,19,	12,19,	
12,19,	12,19,	12,19,	12,19,	
12,19,	12,19,	12,19,	12,19,	
12,19,	12,19,	12,19,	12,19,	
12,19,	12,19,	12,19,	12,19,	
12,19,	12,19,	12,19,	12,19,	
12,19,	12,19,	13,20,	14,21,	
20,24,	21,25,	24,29,	29,31,	
12,19,	12,19,	12,19,	12,19,	
12,19,	12,19,	12,19,	12,19,	
12,19,	12,19,	12,19,	12,19,	
12,19,	12,19,	12,19,	12,19,	
12,19,	12,19,	12,19,	12,19,	
12,19,	12,19,	12,19,	12,19,	
12,19,	12,19,	15,22,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	15,22,	15,22,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	15,22,	0,0,	15,23,	
15,22,	0,0,	0,0,	0,0,	
15,22,	15,22,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	15,22,	0,0,	
0,0};
struct yysvf yysvec[] ={
0,	0,	0,
yycrank+-1,	0,		0,	
yycrank+-2,	yysvec+1,	0,	
yycrank+0,	0,		yyvstop+1,
yycrank+0,	0,		yyvstop+3,
yycrank+0,	0,		yyvstop+6,
yycrank+0,	0,		yyvstop+8,
yycrank+0,	0,		yyvstop+11,
yycrank+0,	0,		yyvstop+14,
yycrank+3,	0,		yyvstop+17,
yycrank+19,	0,		yyvstop+20,
yycrank+1,	0,		yyvstop+23,
yycrank+39,	0,		yyvstop+25,
yycrank+29,	yysvec+12,	yyvstop+28,
yycrank+21,	yysvec+12,	yyvstop+31,
yycrank+-161,	0,		0,	
yycrank+0,	0,		yyvstop+34,
yycrank+0,	yysvec+10,	yyvstop+36,
yycrank+0,	0,		yyvstop+38,
yycrank+0,	yysvec+12,	yyvstop+40,
yycrank+29,	yysvec+12,	yyvstop+42,
yycrank+33,	yysvec+12,	yyvstop+44,
yycrank+-9,	yysvec+15,	0,	
yycrank+-10,	yysvec+15,	0,	
yycrank+29,	yysvec+12,	yyvstop+46,
yycrank+0,	yysvec+12,	yyvstop+48,
yycrank+-11,	yysvec+15,	0,	
yycrank+-14,	yysvec+15,	0,	
yycrank+0,	0,		yyvstop+51,
yycrank+25,	yysvec+12,	yyvstop+53,
yycrank+-36,	yysvec+15,	yyvstop+55,
yycrank+0,	yysvec+12,	yyvstop+57,
0,	0,	0};
struct yywork *yytop = yycrank+226;
struct yysvf *yybgin = yysvec+1;
char yymatch[] ={
00  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,011 ,012 ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
011 ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
'(' ,'(' ,'*' ,'+' ,01  ,'+' ,01  ,'/' ,
'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,
'0' ,'0' ,01  ,'(' ,01  ,01  ,01  ,01  ,
01  ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,01  ,01  ,01  ,01  ,01  ,
01  ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,01  ,01  ,01  ,01  ,01  ,
0};
char yyextra[] ={
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0};
/*	ncform	4.1	83/08/11	*/

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
