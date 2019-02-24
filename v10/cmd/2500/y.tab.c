
# line 4 "gram.y"

#define		LTIME(t, hr, mi, se, fr)	(t.h=hr, t.m=mi, t.s=se, t.f=fr)

#define		YYDEBUG		1

#include	"hdr.h"


# line 24 "gram.y"
typedef union {
	int ival;
	Time time;
	char *str;
} YYSTYPE;
# define AUTO 257
# define CUE 258
# define EDIT 259
# define HELP 260
# define INSERT 261
# define MODE 262
# define PLAY 263
# define REC 264
# define SET 265
# define SNAP 266
# define STATUS 267
# define STEP 268
# define STOP 269
# define LOOP 270
# define VIEW 271
# define WAIT 272
# define FRAME 273
# define STANDBY 274
# define REMOTE 275
# define SHOOT 276
# define SLEEP 277
# define FWD 278
# define REV 279
# define OFF 280
# define ON 281
# define PREROLL 282
# define STILL 283
# define GO 284
# define VIDSYNC 285
# define AUDIO1 286
# define AUDIO2 287
# define AUDIO3 288
# define VIDEO 289
# define TIME 290
# define LTC 291
# define VITC 292
# define T1 293
# define T2 294
# define TCR 295
# define UB 296
# define NUMBER 297
# define STRING 298
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern short yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256

# line 116 "gram.y"


yywrap()
{
	return(1);
}

yyerror(s1, s2)
	char *s1;
{
	fprint(2, s1, s2);
}
short yyexca[] ={
-1, 1,
	0, -1,
	-2, 6,
	};
# define YYNPROD 66
# define YYLAST 317
short yyact[]={

   3,   4,   5,   7,   8,  83,  10,  12,  14,  26,
  19,  20,  22,   9,  24,  25,   6,  18,  13,  15,
  16,  44,  45,  70,  57,  11,  21,  55,  96,  93,
  33,  82,  80,  23,  77,  78,  76,  73,  46,  67,
  42,  49,  48,  47,  71,  35,  72,  89,  90,  91,
  92,  75,  74,  58,  59,  60,  61,  38,  39,  50,
  34,  37,  33,  43,  36,  31,  30,  52,  53,  51,
  27,  88,  32,  63,  95,  86,  65,  17,   2,  87,
  56,  40,  41,   1,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,  54,   0,   0,  62,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,  66,   0,
   0,   0,   0,   0,  68,  69,   0,   0,   0,  28,
   0,   0,  64,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,  79,   0,   0,   0,  81,
   0,   0,   0,   0,  84,  85,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,  94,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,  29 };
short yypact[]={

-1000,-257,  60,-215,-267,-220,-223,-1000,-267,-267,
-1000,-250,-205,-276,-252,-254,-255,-256,-221,-198,
-211,-235,-1000,-238,-267,-1000,-1000,-1000,-1000,  63,
-1000,-1000,-1000,  30,-1000,-267,-1000,-1000,-1000,-1000,
-258,-267,-267,-1000,-1000,-275,-249,-260,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-229,-1000,-259,-1000,-1000,
-1000,-1000,-267,-1000,-1000,-265,-267,-1000,-266,-1000,
-293,-267,-267,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
  29,-239,-1000,-1000,-1000,-1000,-268,-239,-1000,-1000,
-1000,-1000,-1000,  28,-1000,-269,-1000 };
short yypgo[]={

   0,  83,  72,  80,  71,  79,  78,  77 };
short yyr1[]={

   0,   1,   1,   1,   1,   1,   6,   6,   6,   6,
   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
   6,   6,   6,   6,   6,   6,   6,   6,   6,   7,
   7,   2,   2,   2,   2,   3,   3,   3,   3,   3,
   5,   5,   4,   4,   4,   4 };
short yyr2[]={

   0,   0,   3,   3,   4,   4,   0,   2,   2,   2,
   2,   2,   2,   1,   5,   2,   2,   3,   4,   1,
   3,   1,   2,   2,   2,   1,   3,   4,   4,   4,
   3,   2,   1,   2,   2,   1,   2,   2,   2,   2,
   3,   3,   1,   2,   3,   3,   3,   3,   1,   1,
   2,   1,   3,   5,   7,   1,   0,   1,   1,   1,
   1,   2,   1,   1,   1,   1 };
short yychk[]={

-1000,  -1,  -6, 257, 258, 259, 273, 260, 261, 270,
 263, 282, 264, 275, 265, 276, 277,  -7, 274, 267,
 268, 283, 269, 290, 271, 272, 266,  10,  59, 256,
 281, 280,  -2, 297, 280, 265, 284, 281, 280, 281,
  -2,  -2, 290, 268, 297, 298, 290, 297, 297, 297,
 280, 267, 278, 279,  -2, 262,  -3, 262, 291, 292,
 293, 294,  -2,  10,  59,  46,  -2, 297,  -2,  -2,
 298, 293, 295, 297, 281, 280, 295, 293, 294,  -2,
 297,  -2, 297, 298,  -2,  -2,  46,  -5,  -4, 286,
 287, 288, 289, 297,  -4,  46, 297 };
short yydef[]={

   1,  -2,   0,   0,   0,   0,   0,  13,   0,   0,
  19,   0,  21,  25,   0,   0,   0,  32,   0,  35,
   0,   0,  42,  56,   0,  48,  49,   2,   3,   0,
   7,   8,   9,  51,  10,   0,  15,  50,  11,  12,
  16,   0,   0,  22,  23,  24,   0,   0,  31,  33,
  34,  36,  37,  38,  39,   0,  43,   0,  55,  57,
  58,  59,   0,   4,   5,   0,   0,  17,   0,  20,
  26,   0,   0,  30,  40,  41,  44,  45,  46,  47,
  52,   0,  18,  27,  28,  29,   0,  14,  60,  62,
  63,  64,  65,  53,  61,   0,  54 };
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
			print("char %d in %s", yychar, yystates[yystate]);
		else
			print("%s in %s", yytoknames[yychar], yystates[yystate]);
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
				print("lex EOF\n");
			else if(yytoknames[yychar])
				print("lex %s\n", yytoknames[yychar]);
			else
				print("lex (%c)\n", yychar);
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
					print("lex EOF\n");
				else
					print("lex %s\n", yytoknames[yychar]);
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
				if( yydebug ) print( "error recovery pops state %d, uncovers %d\n", *yyps, yyps[-1] );
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
				print("error recovery discards ");
				if(yytoknames[yychar])
					print("%s\n", yytoknames[yychar]);
				else if(yychar >= ' ' && yychar < '\177')
					print("`%c'\n", yychar);
				else if(yychar == 0)
					print("EOF\n");
				else
					print("char 0%o\n", yychar);
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
		print("reduce %d in:\n\t", yyn);
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
		
case 7:
# line 43 "gram.y"
{ two("auto on", 0x40, 0x41); } break;
case 8:
# line 44 "gram.y"
{ two("auto off", 0x40, 0x40); } break;
case 9:
# line 45 "gram.y"
{ cue(yypvt[-0].time); } break;
case 10:
# line 46 "gram.y"
{ two("edit off", 0x20, 0x64); } break;
case 11:
# line 47 "gram.y"
{ frame(0); } break;
case 12:
# line 48 "gram.y"
{ frame(1); } break;
case 13:
# line 49 "gram.y"
{ help(); } break;
case 14:
# line 50 "gram.y"
{ editset(yypvt[-2].time, yypvt[-1].time, yypvt[-0].ival); } break;
case 15:
# line 51 "gram.y"
{ editgo(); } break;
case 16:
# line 52 "gram.y"
{ insert(yypvt[-0].time, 1); } break;
case 17:
# line 53 "gram.y"
{ insert(yypvt[-1].time, yypvt[-0].ival); } break;
case 18:
# line 54 "gram.y"
{ loop(yypvt[-2].time, yypvt[-1].time, yypvt[-0].ival); } break;
case 19:
# line 55 "gram.y"
{ two("play", 0x20, 0x01); } break;
case 20:
# line 56 "gram.y"
{ prerollt(yypvt[-0].time); } break;
case 21:
# line 57 "gram.y"
{ two("rec", 0x20, 0x02); } break;
case 22:
# line 58 "gram.y"
{ three("rec step", 0x41, 0x3F, 2); } break;
case 23:
# line 59 "gram.y"
{ remiframe(yypvt[-0].ival); } break;
case 24:
# line 60 "gram.y"
{ remsframe(yypvt[-0].str); } break;
case 25:
# line 61 "gram.y"
{ remiframe(-1); } break;
case 26:
# line 62 "gram.y"
{ remsetup(yypvt[-1].str, yypvt[-0].str, (char *)0); } break;
case 27:
# line 63 "gram.y"
{ remsetup(yypvt[-2].str, yypvt[-1].str, yypvt[-0].str); } break;
case 28:
# line 64 "gram.y"
{ settimer(yypvt[-0].time, 0); } break;
case 29:
# line 65 "gram.y"
{ settimer(yypvt[-0].time, 4); } break;
case 30:
# line 66 "gram.y"
{ shoot(yypvt[-1].ival, yypvt[-0].ival, 1); } break;
case 31:
# line 67 "gram.y"
{ sleep(yypvt[-0].ival); } break;
case 32:
# line 68 "gram.y"
{ snap(1); } break;
case 33:
# line 69 "gram.y"
{ snap(yypvt[-0].ival); } break;
case 34:
# line 70 "gram.y"
{ two("stop", 0x20, 0x00); two("standby off", 0x20, 0x04); } break;
case 35:
# line 71 "gram.y"
{ status(0); } break;
case 36:
# line 72 "gram.y"
{ status(1); } break;
case 37:
# line 73 "gram.y"
{ two("step fwd", 0x20, 0x14); } break;
case 38:
# line 74 "gram.y"
{ two("step rev", 0x20, 0x24); } break;
case 39:
# line 75 "gram.y"
{ still(yypvt[-0].time); } break;
case 40:
# line 76 "gram.y"
{ three("", 0x41, 0x3F, 0x01); } break;
case 41:
# line 77 "gram.y"
{ three("", 0x41, 0x3F, 0x00); } break;
case 42:
# line 78 "gram.y"
{ two("stop", 0x20, 0x00); printclock(); } break;
case 43:
# line 79 "gram.y"
{ timer(yypvt[-0].ival); } break;
case 44:
# line 80 "gram.y"
{ three("time mode tcr", 0x41, 0x36, 0); } break;
case 45:
# line 81 "gram.y"
{ three("time mode t1", 0x41, 0x36, 1); } break;
case 46:
# line 82 "gram.y"
{ three("time mode t2", 0x41, 0x36, 2); } break;
case 47:
# line 83 "gram.y"
{ view(yypvt[-1].time,yypvt[-0].time); } break;
case 48:
# line 84 "gram.y"
{  swait(); } break;
case 51:
# line 93 "gram.y"
{ LTIME(yyval.time, 0, 0, yypvt[-0].ival, 0); } break;
case 52:
# line 94 "gram.y"
{ LTIME(yyval.time, 0, 0, yypvt[-2].ival, yypvt[-0].ival); } break;
case 53:
# line 95 "gram.y"
{ LTIME(yyval.time, 0, yypvt[-4].ival, yypvt[-2].ival, yypvt[-0].ival); } break;
case 54:
# line 96 "gram.y"
{ LTIME(yyval.time, yypvt[-6].ival, yypvt[-4].ival, yypvt[-2].ival, yypvt[-0].ival); } break;
case 55:
# line 99 "gram.y"
{ yyval.ival = 0x01; } break;
case 56:
# line 100 "gram.y"
{ yyval.ival = 0x01; } break;
case 57:
# line 101 "gram.y"
{ yyval.ival = 0x02; } break;
case 58:
# line 102 "gram.y"
{ yyval.ival = 0x04; } break;
case 59:
# line 103 "gram.y"
{ yyval.ival = 0x08; } break;
case 60:
# line 106 "gram.y"
{ yyval.ival = yypvt[-0].ival; } break;
case 61:
# line 107 "gram.y"
{ yyval.ival = yypvt[-1].ival|yypvt[-0].ival; } break;
case 62:
# line 110 "gram.y"
{ yyval.ival = 0x01; } break;
case 63:
# line 111 "gram.y"
{ yyval.ival = 0x02; } break;
case 64:
# line 112 "gram.y"
{ yyval.ival = 0x04; } break;
case 65:
# line 113 "gram.y"
{ yyval.ival = 0x18; } break;
	}
	goto yystack;  /* stack new state and value */
}
