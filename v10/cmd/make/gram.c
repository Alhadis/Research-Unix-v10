
# line 1 "gram.y"
#include "defs"
static	char *sccsid = "@(#)gram.y	8th Edition (Bell Labs) 85/05/14";

# line 6 "gram.y"
typedef union 
	{
	struct shblock *yshblock;
	depblkp ydepblock;
	nameblkp ynameblock;
	} YYSTYPE;
# define NAME 257
# define SHELLINE 258
# define START 259
# define MACRODEF 260
# define COLON 261
# define DOUBLECOLON 262
# define GREATER 263
# define AMPER 264
# define AMPERAMPER 265
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern short yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;

# line 21 "gram.y"
struct depblock *pp;
FSTATIC struct shblock *prevshp;

FSTATIC struct nameblock *lefts[NLEFTS];
struct nameblock *leftp;
FSTATIC int nlefts;

struct lineblock *lp, *lpp;
FSTATIC struct depblock *prevdep;
FSTATIC int sepc;
FSTATIC int allnowait;

FSTATIC struct fstack
	{
	FILE *fin;
	char *fname;
	int lineno;
	} filestack[MAXINCLUDE];
FSTATIC int ninclude = 0;
# define YYERRCODE 256

# line 150 "gram.y"


FSTATIC char *zznextc;	/* null if need another line;
			   otherwise points to next char */
FSTATIC int yylineno;
FSTATIC FILE * fin;

parse(name)
char *name;
{
extern int yylineno;
FILE *stream;
extern char *zznextc;

if(name == CHNULL)
	{
	stream = NULL;
	name = "(builtin-rules)";
	}
else if(equal(name, "-"))
	{
	stream = stdin;
	name = "(stdin)";
	}
else if( (stream = fopen(name, "r")) == NULL)
	return NO;
filestack[0].fname = copys(name);
ninclude = 1;
fin = stream;
yylineno = 0;
zznextc = 0;

if( yyparse() )
	fatal("Description file error");

if(fin)
	fclose(fin);
return YES;
}


yylex()
{
register char *p;
register char *q;
char word[INMAX];

if(! zznextc )
	return nextlin() ;

while( isspace(*zznextc) )
	++zznextc;
switch(*zznextc)
	{
	case '\0':
		return nextlin() ;

	case '|':
		if(zznextc[1]==':')
			{
			zznextc += 2;
			return DOUBLECOLON;
			}
		break;
	case ':':
		if(*++zznextc == ':')
			{
			++zznextc;
			return DOUBLECOLON;
			}
		return COLON;
	case '>':
		++zznextc;
		return GREATER;
	case '&':
		if(*++zznextc == '&')
			{
			++zznextc;
			return AMPERAMPER;
			}
		return AMPER;
	case ';':
		return retsh(zznextc) ;
	}

p = zznextc;
q = word;

while( ! ( funny[*p] & TERMINAL) )
	*q++ = *p++;

if(p != zznextc)
	{
	*q = '\0';
	if((yylval.ynameblock=srchname(word))==0)
		yylval.ynameblock = makename(word);
	zznextc = p;
	return NAME;
	}

else	{
	char junk[100];
	sprintf(junk, "Bad character %c (octal %o), line %d of file %s",
		*zznextc, *zznextc, yylineno, filestack[ninclude-1].fname);
	fatal(junk);
	}
return 0;	/* never executed */
}





retsh(q)
char *q;
{
register char *p;
struct shblock *sp;
char *copys();

for(p=q+1 ; *p==' '||*p=='\t' ; ++p)  ;

sp = ALLOC(shblock);
sp->nxtshblock = NULL;
sp->shbp = (fin ? copys(p) : p );
yylval.yshblock = sp;
zznextc = 0;
return SHELLINE;
}

nextlin()
{
static char yytext[INMAX];
static char *yytextl	= yytext+INMAX;
char *text, templin[INMAX];
register char c;
register char *p, *t;
char lastch, *lastchp;
extern char **linesptr;
int incom;
int kc;

again:

	incom = NO;
	zznextc = 0;

if(fin == NULL)
	{
	if( (text = *linesptr++) == 0)
		return 0;
	++yylineno;
	}

else	{
	for(p = text = yytext ; p<yytextl ; *p++ = kc)
		switch(kc = getc(fin))
			{
			case '\t':
				if(p == yytext)
					incom = YES;
				break;

			case ';':
				incom = YES;
				break;

			case '#':
				if(! incom)
					kc = '\0';
				break;

			case '\n':
				++yylineno;
				if(p==yytext || p[-1]!='\\')
					{
					*p = '\0';
					goto endloop;
					}
				p[-1] = ' ';
				while( (kc=getc(fin))=='\t' || kc==' ' || kc=='\n')
					if(kc == '\n')
						++yylineno;
	
				if(kc != EOF)
					break;
			case EOF:
				*p = '\0';
				if(ninclude > 1)
					{
					register struct fstack *stp;
					fclose(fin);
					--ninclude;
					stp = filestack + ninclude;
					fin = stp->fin;
					yylineno = stp->lineno;
					free(stp->fname);
					goto again;
					}
				return 0;
			}

	fatal("line too long");
	}

endloop:

	if((c = text[0]) == '\t')
		return retsh(text) ;
	
	if(isalpha(c) || isdigit(c) || c==' ' || c=='.')
		for(p=text+1; *p!='\0'; )
			if(*p == ':')
				break;
			else if(*p++ == '=')
				{
				eqsign(text);
				return MACRODEF;
				}


if(!strncmp(text, "include", 7) && isinclude(text+7))
	goto again;

/* substitute for macros on dependency line up to the semicolon if any */

for(t = yytext ; *t!='\0' && *t!=';' ; ++t)
	;

lastchp = t;
lastch = *t;
*t = '\0';	/* replace the semi with a null so subst will stop */

subst(yytext, templin);		/* Substitute for macros on dependency lines */

if(lastch)	/* copy the stuff after the semicolon */
	{
	*lastchp = lastch;
	strcat(templin, lastchp);
	}

strcpy(yytext, templin);

for(p = zznextc = text ; *p ; ++p )
	if(*p!=' ' && *p!='\t')
		return START;
goto again;
}



isinclude(s)
register char *s;
{
register char *t;
register struct fstack *p;

for(t=s; *t==' ' || *t=='\t' ; ++t)
	;
if(t == s)
	return NO;

for(s = t; *s!='\n' && *s!='#' && *s!='\0' ; ++s)
	if(*s == ':')
		return NO;
*s = '\0';

if(ninclude >= MAXINCLUDE)
	fatal("include depth exceeded");
p = filestack + ninclude;
p->fin = fin;
p->lineno = yylineno;
p->fname = copys(t);
if( (fin = fopen(t, "r")) == NULL)
	fatal1("Cannot open include file %s", t);
yylineno = 0;
++ninclude;
return YES;
}



yyerror(s)
char *s;
{
char buf[100];

sprintf(buf, "line %d of file %s: %s",
		yylineno, filestack[ninclude-1].fname, s);
fatal(buf);
}
short yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 22
# define YYLAST 21
short yyact[]={

  17,  20,  21,  16,   7,   9,  11,  18,  19,  12,
  13,   5,   2,   1,   3,   4,  10,   8,   6,  14,
  15 };
short yypact[]={

-1000,-245,-1000,-253,-1000,-1000,-252,-1000,-255,-1000,
-257,-263,-1000,-1000,-1000,-256,-1000,-1000,-1000,-1000,
-1000,-1000 };
short yypgo[]={

   0,  20,  19,  18,  17,  16,  13,  12,   6 };
short yyr1[]={

   0,   6,   6,   7,   7,   7,   7,   3,   3,   4,
   4,   5,   5,   5,   5,   5,   8,   8,   2,   2,
   1,   1 };
short yyr2[]={

   0,   0,   2,   1,   1,   4,   1,   1,   2,   0,
   1,   1,   2,   2,   2,   2,   1,   1,   0,   1,
   1,   2 };
short yychk[]={

-1000,  -6,  -7, 259, 260, 256,  -3, 257,  -4, 257,
  -5,  -8, 261, 262,  -2,  -1, 258, 257, 264, 265,
 264, 258 };
short yydef[]={

   1,  -2,   2,   3,   4,   6,   9,   7,  18,   8,
  10,  11,  16,  17,   5,  19,  20,  13,  14,  15,
  12,  21 };
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
		
case 5:
# line 49 "gram.y"
 {
	    while( --nlefts >= 0)
		{
		wildp iswild(), wp;

		leftp = lefts[nlefts];
		if(wp = iswild(leftp->namep))
			{
			leftp->septype = SOMEDEPS;
			if(lastwild)
				lastwild->next = wp;
			else
				firstwild = wp;
			lastwild = wp;
			}

		if(leftp->septype == 0)
			leftp->septype = sepc;
		else if(leftp->septype != sepc)
			{
			if(! wp)
				fprintf(stderr,
					"Inconsistent rules lines for `%s'\n",
					leftp->namep);
			}
		else if(sepc==ALLDEPS && leftp->namep[0]!='.' && yypvt[-0].yshblock!=0)
			{
			for(lp=leftp->linep; lp->nxtlineblock; lp=lp->nxtlineblock)
			if(lp->shp)
				fprintf(stderr,
					"Multiple rules lines for `%s'\n",
					leftp->namep);
			}

		lp = ALLOC(lineblock);
		lp->nxtlineblock = NULL;
		lp->depp = yypvt[-1].ydepblock;
		lp->shp = yypvt[-0].yshblock;
		if(wp)
			wp->linep = lp;

		if(equal(leftp->namep, ".SUFFIXES") && yypvt[-1].ydepblock==0)
			leftp->linep = 0;
		else if(leftp->linep == 0)
			leftp->linep = lp;
		else	{
			for(lpp = leftp->linep; lpp->nxtlineblock;
				lpp = lpp->nxtlineblock) ;
				if(sepc==ALLDEPS && leftp->namep[0]=='.')
					lpp->shp = 0;
			lpp->nxtlineblock = lp;
			}
		}
	} break;
case 7:
# line 106 "gram.y"
 { lefts[0] = yypvt[-0].ynameblock; nlefts = 1; } break;
case 8:
# line 107 "gram.y"
 { lefts[nlefts++] = yypvt[-0].ynameblock;
	    	if(nlefts>=NLEFTS) fatal("Too many lefts"); } break;
case 9:
# line 112 "gram.y"
{
		char junk[10];
		sprintf(junk, "%d", yylineno);
		fatal1("Must be a separator on rules line %s", junk);
		} break;
case 11:
# line 120 "gram.y"
 { prevdep = 0;  yyval.ydepblock = 0; allnowait = NO; } break;
case 12:
# line 121 "gram.y"
 { prevdep = 0; yyval.ydepblock = 0; allnowait = YES; } break;
case 13:
# line 122 "gram.y"
 {
			  pp = ALLOC(depblock);
			  pp->nxtdepblock = NULL;
			  pp->depname = yypvt[-0].ynameblock;
			  pp->nowait = allnowait;
			  if(prevdep == 0) yyval.ydepblock = pp;
			  else  prevdep->nxtdepblock = pp;
			  prevdep = pp;
			  } break;
case 14:
# line 131 "gram.y"
 { if(prevdep) prevdep->nowait = YES; } break;
case 16:
# line 135 "gram.y"
 { sepc = ALLDEPS; } break;
case 17:
# line 136 "gram.y"
 { sepc = SOMEDEPS; } break;
case 18:
# line 139 "gram.y"
 {yyval.yshblock = 0; } break;
case 19:
# line 140 "gram.y"
 { yyval.yshblock = yypvt[-0].yshblock; } break;
case 20:
# line 143 "gram.y"
 { yyval.yshblock = yypvt[-0].yshblock;  prevshp = yypvt[-0].yshblock; } break;
case 21:
# line 144 "gram.y"
 { yyval.yshblock = yypvt[-1].yshblock;
			prevshp->nxtshblock = yypvt[-0].yshblock;
			prevshp = yypvt[-0].yshblock;
			} break;
	}
	goto yystack;  /* stack new state and value */
}
