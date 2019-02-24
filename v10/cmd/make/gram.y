%{#include "defs"
static	char *sccsid = "@(#)gram.y	8th Edition (Bell Labs) 85/05/14";
%}

%term NAME SHELLINE START MACRODEF COLON DOUBLECOLON GREATER AMPER AMPERAMPER
%union
	{
	struct shblock *yshblock;
	depblkp ydepblock;
	nameblkp ynameblock;
	}

%type <yshblock> SHELLINE, shlist, shellist
%type <ynameblock> NAME, namelist
%type <ydepblock> deplist, dlist


%%

%{
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
%}


file:
	| file comline
	;

comline:  START
	| MACRODEF
	| START namelist deplist shellist = {
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
		else if(sepc==ALLDEPS && leftp->namep[0]!='.' && $4!=0)
			{
			for(lp=leftp->linep; lp->nxtlineblock; lp=lp->nxtlineblock)
			if(lp->shp)
				fprintf(stderr,
					"Multiple rules lines for `%s'\n",
					leftp->namep);
			}

		lp = ALLOC(lineblock);
		lp->nxtlineblock = NULL;
		lp->depp = $3;
		lp->shp = $4;
		if(wp)
			wp->linep = lp;

		if(equal(leftp->namep, ".SUFFIXES") && $3==0)
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
	}
	| error
	;

namelist: NAME	= { lefts[0] = $1; nlefts = 1; }
	| namelist NAME	= { lefts[nlefts++] = $2;
	    	if(nlefts>=NLEFTS) fatal("Too many lefts"); }
	;

deplist:
		{
		char junk[10];
		sprintf(junk, "%d", yylineno);
		fatal1("Must be a separator on rules line %s", junk);
		}
	| dlist
	;

dlist:  sepchar	= { prevdep = 0;  $$ = 0; allnowait = NO; }
	| sepchar AMPER	= { prevdep = 0; $$ = 0; allnowait = YES; }
	| dlist NAME	= {
			  pp = ALLOC(depblock);
			  pp->nxtdepblock = NULL;
			  pp->depname = $2;
			  pp->nowait = allnowait;
			  if(prevdep == 0) $$ = pp;
			  else  prevdep->nxtdepblock = pp;
			  prevdep = pp;
			  }
	| dlist AMPER	= { if(prevdep) prevdep->nowait = YES; }
	| dlist AMPERAMPER
	;

sepchar:  COLON 	= { sepc = ALLDEPS; }
	| DOUBLECOLON	= { sepc = SOMEDEPS; }
	;

shellist:	= {$$ = 0; }
	| shlist = { $$ = $1; }
	;

shlist:	SHELLINE   = { $$ = $1;  prevshp = $1; }
	| shlist SHELLINE = { $$ = $1;
			prevshp->nxtshblock = $2;
			prevshp = $2;
			}
	;

%%

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
