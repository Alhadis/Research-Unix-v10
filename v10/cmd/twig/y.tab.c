# define ERROR 257

# line 3 "twig.y"
#include "common.h"
#include "code.h"
#include "sym.h"
#define UNDEFINED -1
#define GIVENUP	-2

int debug_flag = 0;
int Dflag = 0;
int tflag = 0;
int line_xref_flag = 0;
int ntrees = 0;
int nerrors = 0;
int fatalerrors = 0;
int tree_lineno;
FILE *outfile;
FILE *symfile;
Code *epilogue;

SymbolEntry ErrorSymbol;

# line 23 "twig.y"
typedef union  {
	Node *y_nodep;
	SymbolEntry *y_symp;
	Code *y_code;
	int y_int;
} YYSTYPE;
# define K_NODE 258
# define K_LABEL 259
# define K_PROLOGUE 260
# define K_CONST 261
# define K_INSERT 262
# define K_COST 263
# define K_ACTION 264
# define ID 265
# define NUMBER 266
# define CBLOCK 267
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern short yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256

# line 255 "twig.y"


extern char *process_suffix();

set_arity(symp, arityp, count)
	SymbolEntry *symp;
	int *arityp;
	int count;
{
	if(*arityp!=GIVENUP) {
		if (*arityp==UNDEFINED)
			*arityp = count;
		else if (*arityp!=count) {
			sem_error("inconsistent arity for %s", symp->name);
			*arityp = GIVENUP;
		}
	}
}

is_node(symp)
	SymbolEntry *symp;
{ return(symp->attr==A_NODE); }

is_label(symp)
	SymbolEntry *symp;
{ return(symp->attr==A_LABEL); }

CheckIsNodeOrPred (symp)
	SymbolEntry *symp;
{
	if (symp->attr==A_ERROR)
		return;
	if (symp->attr!=A_NODE)
		sem_error ("non-node identifier %s", symp->name);
}

CheckIsUndefined(symp)
	SymbolEntry *symp;
{
	if (symp->attr==A_ERROR)
		return(0);
	if (symp->attr!=A_UNDEFINED) {
		sem_error ("multiple declaration: %s", symp->name);
		return(0);
	} else return(1);
}

CheckIsDefined(symp)
	SymbolEntry *symp;
{
	if (symp->attr==A_ERROR)
		return(0);
	if (symp->attr==A_UNDEFINED) {
		sem_error ("undefined identifier: %s", symp->name);
		symp->attr=A_ERROR;
		return(0);
	} else return(1);
}



/*VARARGS*/
yyerror(fmt, s1, s2, s3)
	char *fmt, *s1, *s2, *s3;
{
	fprintf(stderr, "line %d: ", yyline);
	fprintf(stderr, fmt, s1, s2, s3);
	if (token_buffer[0]!=0)
		fprintf(stderr, " at \"%s\"\n", token_buffer);
}

/*VARARGS*/
yyerror2 (fmt, s1, s2, s3)
	char *fmt, *s1, *s2, *s3;
{
	fprintf(stderr, "line %d: ", yyline);
	fprintf(stderr, fmt, s1, s2, s3);
	putchar('\n');
}

char *cmdnam;
char *inFileName;
char *outFileName;
char prefixFile[100];
static char usage[] = "usage: mt [-d] file";
#define USAGE	error(1, usage)
char *prefix_base = PREFIX_BASE;
char *suffix = "c1";

main(argc, argv)
	int argc;
	char **argv;
{
	int i;
	char *cp;

#ifndef PREFIX_BASE
	error(1,"PREFIX_BASE has not been defined; recompile twig");
#endif

	cmdnam = argv[0];
	argv++;
	inFileName = NULL;

	while(--argc > 0) {
		if (*argv[0]=='-')
			switch(argv[0][1]) {
			/* to see what each of these flags mean...
			 * see common.h */
			case 'd': {
				char *cp;
				for(cp = &argv[0][2]; *cp!='\0'; cp++) 
					switch(*cp) {
					case 'l': debug_flag |= DB_LEX; break;
					case 'm': debug_flag |= DB_MACH; break;
					case 's': debug_flag |= DB_SYM; break;
					case 't': debug_flag |= DB_TREE; break;
					case 'M': debug_flag |= DB_MEM;	break;
					}
				}
				break;
			case 'D': Dflag++; break;
			case 't': tflag++; break;
			case 'w': suffix = process_suffix(&argv[0][2]); break;
			case 'l': prefix_base = &argv[0][2]; break;
			case 'x': line_xref_flag++; break;
			default:
				USAGE;
			}
		else inFileName = argv[0];
		argv++;
	}
	if(inFileName==NULL)
		USAGE;
			
	if(freopen(inFileName, "r", stdin)==NULL)
		error(1, "can't open %s", inFileName);
	if((cp=strrchr(inFileName, '.'))!=NULL && strcmp(cp,".mt")==0) {
		if ((outfile = fopen("walker.c" , "w"))==NULL)
			error(1, "can't write %s", outFileName);
		if ((symfile = fopen("symbols.h", "w"))==NULL)
			error(1, "can't write %s", outFileName);
	} else error(1, "input file must have suffix .mt");

	ErrorSymbol.attr = A_LABEL;
	TreeInit();
	SymbolInit();
	LexInit();
	yyparse();

	SymbolDump();
	if(nerrors == 0) {
		if(!tflag) {
			FILE *prefix;
			char c;
 			sprintf(prefixFile, "%s.%s", prefix_base, suffix);
			prefix = fopen(prefixFile, "r");
			assert(prefix!=NULL);
			if(Dflag)
				fputs("#define DEBUG 1\n", outfile);
			if(line_xref_flag)
				fputs("#define LINE_XREF 1\n", outfile);
			fprintf(outfile,"# line 1 \"%s\"\n", prefixFile);
			while((c=getc(prefix))!=EOF) putc(c, outfile);
		}
		MachineGen();
		SymbolGenerateWalkerCode();
		CodeWrite(outfile, epilogue);
		CodeFreeBlock(epilogue);
	}

	/* cleanup */
	cleanup(0);
}

cleanup(retcode)
	int retcode;
{
	lexCleanup();
	if(retcode==0) {
		SymbolFinish();
	}
	exit(retcode);
}

/*VARARGS*/
error(rc, fmt, a1, a2, a3)
	int rc;
	char *fmt, *a1, *a2, *a3;
{
	fprintf(stderr, "%s: ", cmdnam);
	fprintf(stderr, fmt, a1, a2, a3);
	putc ('\n', stderr);
	if(rc)
		exit (rc);
}

/*VARARGS*/
sem_error (fmt, a1, a2, a3)
	char *fmt, *a1, *a2, *a3;
{
	fprintf (stderr, "line %d: ", yyline);
	sem_error2(fmt, a1, a2, a3);
	nerrors++;
	fatalerrors++;
}

/*VARARGS*/
sem_error2(fmt, a1, a2, a3)
	char *fmt, *a1, *a2, *a3;
{
	fprintf (stderr, fmt, a1, a2, a3);
	putc('\n', stderr);
	nerrors++;
}

char *
process_suffix(suf)
	char *suf;
{
	extern int gen_table2;
	if(strcmp(suf,"exper")==0) {
		/* experimental walker */
		/* expect this to change alot */
		gen_table2++;
	}
	return(suf);
}
short yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
-1, 10,
	0, 1,
	-2, 0,
-1, 66,
	40, 49,
	-2, 51,
	};
# define YYNPROD 56
# define YYLAST 261
short yyact[]={

  16,  71,   4,   6,   7,   5,  14,   8,   9,  17,
   4,   6,   7,   5,  37,   8,   9,  29,  89,  79,
  88,  78,  73,  72,  54,  36,  61,  59,  28,  50,
  44,  40,  16,  92,  33,  31,  66,  27,  14,  25,
  22,  17,  66,  32,  30,  93,  65,  70,  94,  85,
  69,  53,  87,  76,  75,  64,  63,  62,  60,  58,
  57,  39,  48,  38,  83,  82,  86,  49,  24,  21,
  12,  20,  13,   3,  80,  10,  11,   2,  77,  84,
  23,  35,  19,  34,  18,  15,  26,  90,   1,  45,
  41,   0,  51,   0,   0,   0,   0,   0,   0,   0,
   0,  74,   0,   0,   0,   0,  67,   0,   0,   0,
  68,   0,   0,   0,   0,   0,   0,  81,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,  91,   0,   0,   0,   0,   0,   0,
   0,  96,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,  56,   0,   0,  73,  72,  52,  46,  42,   0,
  55,   0,   0,   0,   0,  25,  47,  43,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  95 };
short yypact[]={

-248,-1000,-256,-1000,-225,-226,-228,-239,-221,-222,
-224,-1000,-1000,-1000,-242,   5,   2,-1000, -28, -29,
-1000,-1000,  27, -30,-1000, -10, -35,-1000,   1,   0,
-240,  -1,-241,  -2,-1000,-1000,  -3,  -4,-229,-1000,
-1000,-1000,-1000,  27,-1000,-1000,-1000,  27, -11, -41,
-1000,-1000,-1000,-243,-1000,-1000,-1000,-1000,-1000,  -5,
-1000,  -6,-1000,-1000,-1000,-246,-1000,-1000, -11,-243,
  24,  23,-1000,-1000,-1000,-1000,-1000, -12,-1000,-1000,
  26,-1000,-1000,-1000,  -7,-247,-223,-1000,-1000,-1000,
   4,-1000,-1000,-1000,-229,-1000,-1000 };
short yypgo[]={

   0,  88,  46,  87,  86,  85,  84,  71,  82,  69,
  80,  68,  62,  47,  79,  78,  77,  75,  73,  72,
  70,  74 };
short yyr1[]={

   0,   1,  16,  16,  18,  18,  18,  18,  18,  18,
  18,  18,  18,  18,   4,   4,   4,   8,   8,   8,
   6,   6,   6,   7,   9,  12,  12,  12,  10,  10,
  10,  11,  13,  13,  17,  17,  17,  17,  20,  20,
  19,  19,  14,  14,  14,  15,  15,  15,   5,  21,
   2,   2,   3,   3,   3,   3 };
short yyr2[]={

   0,   2,   2,   1,   3,   3,   3,   3,   3,   4,
   4,   3,   3,   3,   2,   1,   2,   2,   1,   2,
   2,   1,   2,   2,   4,   3,   3,   0,   2,   1,
   2,   3,   1,   1,   2,   2,   1,   1,   3,   3,
   6,   2,   2,   2,   0,   1,   1,   0,   1,   0,
   5,   1,   3,   1,   1,   2 };
short yychk[]={

-1000,  -1, -16, -18, 258, 261, 259, 260, 263, 264,
 -17, -18, -20, -19, 262,  -5, 256, 265,  -6,  -8,
  -7,  -9, 265, -10, -11, 265,  -4, 265, 267, 256,
 265, 256, 265, 256, -19, -20, 267, 256,  58,  59,
  59,  -7, 256, 265,  59,  -9, 256, 265, -12,  40,
  59, -11, 256,  61,  59, 265, 256,  59,  59, 267,
  59, 267,  59,  59,  59,  -2, 265, -12, -12,  61,
 -13,  42, 266, 265, -13,  59,  59, -15, 267, 265,
 -21, -13,  41,  41, -14,  61,  40,  59, 267, 265,
  -3,  -2, 256,  41,  44, 256,  -2 };
short yydef[]={

   0,  -2,   0,   3,   0,   0,   0,   0,   0,   0,
  -2,   2,  36,  37,   0,   0,   0,  48,   0,   0,
  21,  18,  27,   0,  29,   0,   0,  15,   0,   0,
   0,   0,   0,   0,  34,  35,   0,   0,   0,  41,
   4,  20,  22,  27,   5,  17,  19,  27,  23,   0,
   6,  28,  30,   0,   7,  14,  16,   8,  11,   0,
  13,   0,  12,  38,  39,  47,  -2,  23,   0,   0,
   0,   0,  32,  33,  31,   9,  10,  44,  45,  46,
   0,  24,  25,  26,   0,   0,   0,  40,  42,  43,
   0,  53,  54,  50,   0,  55,  52 };
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
		
case 1:
# line 42 "twig.y"
 
	{ if (nerrors==0) machine_build(); } break;
case 4:
# line 48 "twig.y"
 { SymbolEnterList (yypvt[-1].y_symp, A_NODE); } break;
case 5:
# line 51 "twig.y"
 {
			SymbolEnterList(yypvt[-1].y_symp, A_NODE);
			SymbolCheckNodeValues();
		} break;
case 6:
# line 56 "twig.y"
 { SymbolEnterList (yypvt[-1].y_symp, A_CONST); } break;
case 7:
# line 58 "twig.y"
 { SymbolEnterList (yypvt[-1].y_symp, A_LABEL); } break;
case 8:
# line 60 "twig.y"
 { CodeWrite(outfile, yypvt[-1].y_code); CodeFreeBlock(yypvt[-1].y_code); } break;
case 9:
# line 63 "twig.y"
 { yypvt[-2].y_symp->sd.ca.code = yypvt[-1].y_code; yypvt[-2].y_symp->sd.ca.assoc = NULL;
		 SymbolEnter (yypvt[-2].y_symp, A_COST); } break;
case 10:
# line 67 "twig.y"
 { yypvt[-2].y_symp->sd.ca.code = yypvt[-1].y_code; yypvt[-2].y_symp->sd.ca.assoc = NULL;
		 SymbolEnter (yypvt[-2].y_symp, A_ACTION); } break;
case 14:
# line 83 "twig.y"
 {
				if(CheckIsUndefined(yypvt[-0].y_symp)) {
					yypvt[-0].y_symp->next = yypvt[-1].y_symp;
					yyval.y_symp = yypvt[-0].y_symp;
				} else yyval.y_symp = yypvt[-1].y_symp;
			} break;
case 15:
# line 89 "twig.y"
{ if(CheckIsUndefined(yypvt[-0].y_symp)) yyval.y_symp = yypvt[-0].y_symp; else yyval.y_symp = NULL; } break;
case 17:
# line 97 "twig.y"
 {
			if(yypvt[-0].y_symp->attr==A_ERROR)
				yyval.y_symp = yypvt[-1].y_symp;
			else { yypvt[-0].y_symp->next = yypvt[-1].y_symp; yyval.y_symp = yypvt[-0].y_symp; }
		} break;
case 18:
# line 102 "twig.y"
{ yyval.y_symp = yypvt[-0].y_symp->attr==A_ERROR ? NULL : yypvt[-0].y_symp; } break;
case 20:
# line 106 "twig.y"
 { 
					if(yypvt[-0].y_symp->attr==A_ERROR) yyval.y_symp = yypvt[-1].y_symp;
			        	else { yypvt[-0].y_symp->next = yypvt[-1].y_symp; yyval.y_symp = yypvt[-0].y_symp; }
				} break;
case 21:
# line 110 "twig.y"
{ yyval.y_symp = yypvt[-0].y_symp->attr==A_ERROR ? NULL : yypvt[-0].y_symp; } break;
case 23:
# line 114 "twig.y"
 {
			if (CheckIsUndefined(yypvt[-1].y_symp)) {
				yypvt[-1].y_symp->sd.arity = yypvt[-0].y_int; yyval.y_symp = yypvt[-1].y_symp;
			} else yyval.y_symp = &ErrorSymbol;
		} break;
case 24:
# line 121 "twig.y"
{
				if(CheckIsUndefined(yypvt[-3].y_symp)) {
					yypvt[-3].y_symp->unique = yypvt[-0].y_int; yypvt[-3].y_symp->sd.arity = yypvt[-2].y_int;
					yyval.y_symp = yypvt[-3].y_symp;
				} else yyval.y_symp = &ErrorSymbol;
			} break;
case 25:
# line 129 "twig.y"
 { yyval.y_int = yypvt[-1].y_int; } break;
case 26:
# line 130 "twig.y"
 { yyval.y_int=GIVENUP; } break;
case 27:
# line 131 "twig.y"
 { yyval.y_int = UNDEFINED; } break;
case 28:
# line 134 "twig.y"
 {
				if (yypvt[-0].y_symp->attr==A_ERROR) yyval.y_symp = yypvt[-1].y_symp;
				else { yypvt[-0].y_symp->next = yypvt[-1].y_symp; yyval.y_symp = yypvt[-0].y_symp; }
			} break;
case 29:
# line 138 "twig.y"
{ yyval.y_symp = yypvt[-0].y_symp->attr==A_ERROR ? NULL : yypvt[-0].y_symp; } break;
case 31:
# line 142 "twig.y"
 {
				if(CheckIsUndefined(yypvt[-2].y_symp)) {
					yypvt[-2].y_symp->sd.cvalue = yypvt[-0].y_int; yyval.y_symp = yypvt[-2].y_symp;
				} else yyval.y_symp = &ErrorSymbol;
		} break;
case 33:
# line 150 "twig.y"
 {
		if(!CheckIsDefined(yypvt[-0].y_symp)) yyval.y_int = UNDEFINED;
		else if(yypvt[-0].y_symp->attr!=A_CONST) {
			sem_error("non-constant id used");
			yyval.y_int = -1;
		} else yyval.y_int = yypvt[-0].y_symp->sd.cvalue;
	} break;
case 38:
# line 164 "twig.y"
 { epilogue = CodeAppend(epilogue, yypvt[-1].y_code); } break;
case 40:
# line 169 "twig.y"
 {
		    if (yypvt[-5].y_symp->attr==A_ERROR) {
			error(0, "\"label: tree\" pair ignored");
			TreeFree(yypvt[-3].y_nodep);
		    } else {
			if(nerrors==0)
				cgotofn(SymbolEnterTreeIntoLabel(yypvt[-5].y_symp,
					yypvt[-3].y_nodep, yypvt[-2].y_symp, yypvt[-1].y_symp, tree_lineno));
			if(debug_flag&DB_TREE)
				TreePrint(yypvt[-3].y_nodep, 1);
		    }
		} break;
case 42:
# line 183 "twig.y"
{ SymbolEntry *sp = SymbolAllocate (SymbolGenUnique());
		sp->sd.ca.code = yypvt[-0].y_code; sp->sd.ca.assoc = NULL;
		SymbolEnter(sp, A_ACTION); yyval.y_symp = sp; } break;
case 43:
# line 186 "twig.y"
{ if(CheckIsDefined(yypvt[-0].y_symp)) {
			if (yypvt[-0].y_symp->attr!=A_ACTION) {
				sem_error ("non action id: %s", yypvt[-0].y_symp->name);
				yyval.y_symp = &ErrorSymbol;
			} else yyval.y_symp = yypvt[-0].y_symp;
		} else yyval.y_symp = &ErrorSymbol; } break;
case 44:
# line 192 "twig.y"
{ yyval.y_symp = NULL;} break;
case 45:
# line 194 "twig.y"
{ SymbolEntry *sp = SymbolAllocate (SymbolGenUnique());
		sp->sd.ca.code = yypvt[-0].y_code; sp->sd.ca.assoc = NULL;
		SymbolEnter (sp, A_COST); yyval.y_symp = sp;
		} break;
case 46:
# line 198 "twig.y"
{ if (CheckIsDefined(yypvt[-0].y_symp)) {
			if (yypvt[-0].y_symp->attr!=A_COST) {
				sem_error ("non cost id: %s", yypvt[-0].y_symp->name);
				yyval.y_symp = &ErrorSymbol;
			} else yyval.y_symp = yypvt[-0].y_symp;
		} else yyval.y_symp = &ErrorSymbol; } break;
case 47:
# line 204 "twig.y"
{ yyval.y_symp = NULL; } break;
case 48:
# line 207 "twig.y"
 {
		tree_lineno = yyline;	/* record line no */
	        if(!CheckIsDefined(yypvt[-0].y_symp))
			yypvt[-0].y_symp->attr = A_ERROR;
		else if(!is_label(yypvt[-0].y_symp)) {
			sem_error("non label id: %s", yypvt[-0].y_symp->name);
			yypvt[-0].y_symp->attr = A_ERROR;
		}
		yyval.y_symp = yypvt[-0].y_symp;
	} break;
case 49:
# line 218 "twig.y"
{CheckIsNodeOrPred(yypvt[-0].y_symp);} break;
case 50:
# line 219 "twig.y"
 {
		int count;
		Node *ap; 
		/* check the arity of the node */
		for(count=0, ap = yypvt[-1].y_nodep; ap!=NULL; ap=ap->siblings) count++;
		switch(yypvt[-4].y_symp->attr) {
			case A_NODE:
				set_arity(yypvt[-4].y_symp, &yypvt[-4].y_symp->sd.arity, count);
				break;
		}

		yyval.y_nodep = TreeBuild (yypvt[-4].y_symp, yypvt[-1].y_nodep);
	} break;
case 51:
# line 233 "twig.y"
 {
		CheckIsDefined(yypvt[-0].y_symp);
		switch (yypvt[-0].y_symp->attr) {
			case A_NODE:
				set_arity(yypvt[-0].y_symp, &yypvt[-0].y_symp->sd.arity, 0);
				break;
		}
		yyval.y_nodep = TreeBuild (yypvt[-0].y_symp, NULL);
	} break;
case 52:
# line 243 "twig.y"
 {
			/*
			 * build sibling list in reverse order TreeBuild will
			 * put it right later.
			 */
			yypvt[-0].y_nodep->siblings = yypvt[-2].y_nodep;
			yyval.y_nodep = yypvt[-0].y_nodep;
		} break;
case 53:
# line 251 "twig.y"
 { yypvt[-0].y_nodep->siblings = NULL; yyval.y_nodep = yypvt[-0].y_nodep; } break;
case 54:
# line 252 "twig.y"
{ yyval.y_nodep = NULL; } break;
	}
	goto yystack;  /* stack new state and value */
}
