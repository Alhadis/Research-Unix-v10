# line 1 "picy.y"
# define BOX 1
# define ARROW 2
# define CIRCLE 3
# define ARC 4
# define ELLIPSE 5
# define LINE 6
# define MOVE 7
# define TEXT 8
# define TROFF 9
# define SPLINE 10
# define BLOCK 11
# define BLOCKEND 12
# define PRINT 269
# define PLACE 270
# define ATTR 271
# define SPREAD 272
# define FILL 273
# define LJUST 274
# define RJUST 275
# define ABOVE 276
# define BELOW 277
# define LEFT 278
# define RIGHT 279
# define UP 280
# define DOWN 281
# define FROM 282
# define TO 283
# define AT 284
# define BY 285
# define WITH 286
# define HEAD 287
# define CW 288
# define CCW 289
# define THEN 290
# define HEIGHT 291
# define WIDTH 292
# define RADIUS 293
# define DIAMETER 294
# define LENGTH 295
# define SIZE 296
# define PLACENAME 297
# define VARNAME 298
# define DEFNAME 299
# define CORNER 300
# define HERE 301
# define LAST 302
# define NTH 303
# define SAME 304
# define BETWEEN 305
# define AND 306
# define EAST 307
# define WEST 308
# define NORTH 309
# define SOUTH 310
# define NE 311
# define NW 312
# define SE 313
# define SW 314
# define CENTER 315
# define START 316
# define END 317
# define DOTX 318
# define DOTY 319
# define DOTHT 320
# define DOTWID 321
# define DOTRAD 322
# define NUMBER 323
# define DIR 324
# define DOT 325
# define DASH 326
# define CHOP 327
# define ST 328
# define UMINUS 329

# line 32 "picy.y"
#include	<stdio.h>
extern	int	hvmode, codegen;
extern	float	between, lastfloat;
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern int yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
#ifndef YYSTYPE
#define YYSTYPE int
#endif
YYSTYPE yylval, yyval;
typedef int yytabelem;
# define YYERRCODE 256
yytabelem yyexca[] ={
-1, 0,
	0, 2,
	-2, 0,
-1, 1,
	0, -1,
	-2, 0,
-1, 30,
	60, 70,
	-2, 72,
-1, 60,
	60, 70,
	-2, 72,
	};
# define YYNPROD 115
# define YYLAST 607
yytabelem yyact[]={

    12,    17,    13,    15,    14,    16,    19,    20,    21,    18,
    26,    74,   126,    75,    12,    17,    13,    15,    14,    16,
    19,    20,    21,    18,    12,    17,    13,    15,    14,    16,
    19,    20,    21,    18,   134,    72,   152,   150,   103,    33,
    70,    68,    65,    69,    32,    71,   180,   179,    12,    17,
    13,    15,    14,    16,    19,    20,    21,    18,    74,   178,
    75,    74,   147,    75,    12,    17,    13,    15,    14,    16,
    19,    20,    21,    18,    72,   113,   133,    42,    29,    70,
    68,    85,    69,    28,    71,    79,    80,    81,    82,    83,
    24,   153,   146,    37,    40,    33,    38,    39,    43,    41,
    32,   162,   154,    90,    24,    85,   127,   121,   122,   119,
   120,   124,   125,    72,    24,    86,    89,    28,    70,    62,
    43,    88,    23,    71,    32,    93,    98,    94,    96,    95,
    97,   151,    33,    87,    99,   101,    23,    32,    24,    74,
    85,    75,    74,   183,    75,   100,    23,   143,    54,    31,
   123,    74,    32,    75,    24,   190,   141,   167,   195,    67,
    66,    32,    93,    98,    94,    96,    95,    97,   118,   104,
    23,    99,    91,   169,    22,     6,     5,     4,    57,    44,
    25,     2,     1,     0,     0,     0,    23,     0,    27,     0,
     0,   148,   149,    45,    46,    47,    48,    49,    50,    51,
    52,     0,   168,     0,    53,    25,    55,    72,     0,   156,
     0,   145,    70,    68,   188,    69,     0,    71,   164,   165,
   166,     0,    72,     0,     0,     0,   145,    70,    68,   186,
    69,    25,    71,     0,   128,     0,     0,     0,     0,    89,
    72,     0,     0,   181,   145,    70,    68,   144,    69,     0,
    71,     0,     0,     0,     0,     0,    87,   157,   158,   159,
   160,   163,     0,     0,     0,     0,     0,     0,    10,     0,
     0,     0,     0,     0,   184,   105,   121,   122,   119,   120,
   124,   125,    10,   170,   171,     0,   107,   108,   109,   110,
   111,   114,    10,     0,     0,     0,    58,    59,     0,    38,
    39,    43,    41,     3,     0,     0,     0,    72,   112,     0,
     7,     8,    70,    68,   174,    69,    10,    71,     0,   123,
     7,     8,    35,     9,     0,     0,    64,    56,   106,   115,
   116,   117,    10,   191,   192,    84,     0,     9,     0,     0,
     0,    11,     0,   129,     7,     8,    73,     9,     0,   182,
     0,    11,    37,    34,     0,   161,    39,    43,    41,    84,
     7,     8,     0,     0,     0,   130,    79,    80,    81,    82,
    83,     9,     0,     0,     0,    11,    37,    34,    35,    38,
    39,    43,    41,     0,     0,     0,     0,     9,     0,    37,
    34,    11,    38,    39,    43,    41,     0,     0,     0,     0,
     0,    60,    35,     0,    37,    34,     0,    38,    39,    43,
    41,     0,    30,    37,    34,    35,    38,    39,    43,    41,
     0,    63,     0,     0,     0,     0,   102,     0,     0,     0,
    35,    61,    36,     0,    76,    77,    72,     0,     0,    35,
   197,    70,    68,    72,    69,     0,    71,   196,    70,    68,
    36,    69,    72,    71,     0,    78,   189,    70,    68,     0,
    69,     0,    71,    92,   131,     0,     0,   132,     0,     0,
   135,   136,   137,   138,   139,    72,   140,   142,     0,     0,
    70,    68,   172,    69,    72,    71,     0,     0,   145,    70,
    68,    72,    69,     0,    71,     0,    70,    68,    65,    69,
     0,    71,     0,     0,    72,     0,     0,   155,   155,    70,
    68,     0,    69,     0,    71,     0,     0,   155,   155,   155,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    36,
    36,    36,    36,    36,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   173,     0,   175,   176,     0,     0,     0,
     0,     0,     0,     0,     0,    36,    36,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   177,     0,
     0,     0,     0,     0,   185,     0,   187,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   193,     0,
   194,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    36,    36 };
yytabelem yypact[]={

    47, -1000,    63, -1000, -1000,  -318,    63,    25,    17, -1000,
    92, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
 -1000, -1000,    63, -1000, -1000, -1000, -1000,    23,    -1,    79,
    -2,    18,    79,    79, -1000, -1000,  -233,    35,  -182, -1000,
   161,   124,  -262, -1000,     4,     4,     4,     4,     4,     4,
     4,     4,     4,    13, -1000, -1000,    63,    15,    59,    17,
   454,    37,    79,  -233, -1000,    79,    16,  -271,    79,    79,
    79,    79,    79, -1000,   116,   107, -1000,   203,    48, -1000,
 -1000, -1000, -1000, -1000, -1000,  -235,    94,   161,   124, -1000,
  -263,    85, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
  -264,    45, -1000, -1000, -1000,    79,    79,    92,    92,    92,
    92,    55, -1000, -1000, -1000,    79,    79,    79,   149, -1000,
 -1000, -1000, -1000, -1000, -1000, -1000,  -165, -1000, -1000, -1000,
 -1000,   447,   467,    92,    92,    76,    76, -1000, -1000, -1000,
   438,    79,   270,    79,    79, -1000,  -204, -1000, -1000, -1000,
 -1000,  -238, -1000,  -250, -1000,   467, -1000,   108,   108,   108,
   108,  -182,  -251,   108, -1000, -1000, -1000,  -165, -1000, -1000,
    99,   -32,    79,   185,    79,   170,   415,   114, -1000, -1000,
 -1000, -1000,     4,    92,    92,   467,    79,   467,    79, -1000,
 -1000,    96,   108,   406,   399, -1000, -1000, -1000 };
yytabelem yypgo[]={

     0,   182,   181,   177,   176,   175,   149,   401,   179,   174,
   173,   169,   102,    75,   168,   421,   160,   159,    94,   103,
    77 };
yytabelem yyr1[]={

     0,     1,     1,     1,     2,     2,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     5,     4,     4,     4,
     4,     4,     4,     4,     4,     4,     4,    10,     4,     9,
     8,     8,    11,    11,    11,    11,    11,    11,    11,    11,
    11,    11,    11,    11,    11,    11,    11,    11,    12,    12,
    14,    14,    14,    14,    13,    13,    13,    13,    13,    13,
    13,     6,     6,     6,     6,     6,     6,     6,     6,     6,
    16,     6,    17,     6,    15,    15,    15,    15,    15,    15,
    15,    15,    15,    15,    15,    15,    15,    20,    20,    20,
    18,    18,    18,    19,    19,    19,    19,    19,    19,    19,
    19,     7,     7,     7,     7,     7,     7,     7,     7,     7,
     7,     7,     7,     7,     7 };
yytabelem yyr2[]={

     0,     2,     0,     3,     2,     4,     5,     7,     7,     9,
     9,     9,     3,     7,     7,     2,     3,     5,     5,     5,
     5,     5,     5,     5,     5,     5,     3,     1,    11,     3,
     4,     1,     5,     5,     5,     5,     5,     5,     5,     7,
     5,     3,     3,     3,     5,     5,     5,     2,     2,     1,
     3,     5,     5,     7,     2,     2,     2,     2,     2,     2,
     2,     0,     2,     7,    11,    11,    11,    15,    15,    11,
     1,    15,     1,    13,     3,     5,     5,     3,     5,     7,
     7,     5,     7,     7,     2,     5,     5,     9,     9,     7,
     5,     5,     3,     2,     2,     2,     2,     2,     2,     2,
     2,     7,     7,     7,     7,     7,     5,     7,     3,     2,
     5,     5,     5,     5,     5 };
yytabelem yychk[]={

 -1000,    -1,    -2,   256,    -3,    -4,    -5,   297,   298,   324,
   269,   328,     1,     3,     5,     4,     6,     2,    10,     7,
     8,     9,    -9,   123,    91,    -3,   328,    -2,    58,    61,
    -7,    -6,    45,    40,   298,   323,   -15,   297,   300,   301,
   -18,   303,   -20,   302,    -8,    -8,    -8,    -8,    -8,    -8,
    -8,    -8,    -8,    -2,   125,    -3,   328,    -6,   297,   298,
    -7,    -7,    40,   -15,   328,    44,   -16,   -17,    43,    45,
    42,    47,    37,   328,    43,    45,    -7,    -7,   -15,   318,
   319,   320,   321,   322,   300,    46,   297,   -18,   303,   -20,
   -19,    11,   302,     1,     3,     5,     4,     6,     2,    10,
   -19,    11,   302,   300,   -11,   271,   324,   282,   283,   284,
   285,   286,   304,   -13,   287,   325,   326,   327,   -14,   274,
   275,   272,   273,   315,   276,   277,     8,    93,    -3,   328,
   328,    -7,    -7,    60,   305,    -7,    -7,    -7,    -7,    -7,
    -7,    40,    -7,    40,    44,    41,    44,   297,   -19,   -19,
   300,    46,   300,    46,   -12,    -7,   -12,    -6,    -6,    -6,
    -6,   300,    46,    -6,   -12,   -12,   -12,     8,   -13,   -10,
    -6,    -6,    44,    -7,    44,    -7,    -7,   -15,   297,   297,
   297,   -13,    -8,    44,   306,    -7,    44,    -7,    44,    41,
    41,    -6,    -6,    -7,    -7,    62,    41,    41 };
yytabelem yydef[]={

    -2,    -2,     1,     3,     4,     0,     0,     0,     0,    12,
    61,    15,    31,    31,    31,    31,    31,    31,    31,    31,
    31,    26,     0,    16,    29,     5,     6,     0,    61,     0,
    -2,     0,     0,     0,   108,   109,    62,    74,     0,    77,
     0,     0,    84,    92,    17,    18,    19,    20,    21,    22,
    23,    24,    25,     0,     7,     8,    15,     0,    74,   108,
    -2,     0,     0,     0,    13,     0,     0,     0,     0,     0,
     0,     0,     0,    14,     0,     0,   106,     0,     0,   110,
   111,   112,   113,   114,    75,     0,    76,     0,     0,    86,
    78,   100,    90,    93,    94,    95,    96,    97,    98,    99,
    81,   100,    91,    85,    30,    49,    49,    61,    61,    61,
    61,    61,    41,    42,    43,    49,    49,    49,    47,    54,
    55,    56,    57,    58,    59,    60,    50,    27,     9,    10,
    11,     0,    63,    61,    61,   101,   102,   103,   104,   105,
     0,     0,     0,     0,     0,   107,     0,    89,    80,    83,
    79,     0,    82,     0,    32,    48,    33,    34,    35,    36,
    37,    38,     0,    40,    44,    45,    46,    52,    51,    31,
     0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
    39,    53,    28,    61,    61,    64,     0,    65,     0,    66,
    69,     0,    73,     0,     0,    71,    67,    68 };
typedef struct { char *t_name; int t_val; } yytoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0	/* don't allow debugging */
#endif

#if YYDEBUG

yytoktype yytoks[] =
{
	"BOX",	1,
	"ARROW",	2,
	"CIRCLE",	3,
	"ARC",	4,
	"ELLIPSE",	5,
	"LINE",	6,
	"MOVE",	7,
	"TEXT",	8,
	"TROFF",	9,
	"SPLINE",	10,
	"BLOCK",	11,
	"BLOCKEND",	12,
	"PRINT",	269,
	"PLACE",	270,
	"ATTR",	271,
	"SPREAD",	272,
	"FILL",	273,
	"LJUST",	274,
	"RJUST",	275,
	"ABOVE",	276,
	"BELOW",	277,
	"LEFT",	278,
	"RIGHT",	279,
	"UP",	280,
	"DOWN",	281,
	"FROM",	282,
	"TO",	283,
	"AT",	284,
	"BY",	285,
	"WITH",	286,
	"HEAD",	287,
	"CW",	288,
	"CCW",	289,
	"THEN",	290,
	"HEIGHT",	291,
	"WIDTH",	292,
	"RADIUS",	293,
	"DIAMETER",	294,
	"LENGTH",	295,
	"SIZE",	296,
	"PLACENAME",	297,
	"VARNAME",	298,
	"DEFNAME",	299,
	"CORNER",	300,
	"HERE",	301,
	"LAST",	302,
	"NTH",	303,
	"SAME",	304,
	"BETWEEN",	305,
	"AND",	306,
	"EAST",	307,
	"WEST",	308,
	"NORTH",	309,
	"SOUTH",	310,
	"NE",	311,
	"NW",	312,
	"SE",	313,
	"SW",	314,
	"CENTER",	315,
	"START",	316,
	"END",	317,
	"DOTX",	318,
	"DOTY",	319,
	"DOTHT",	320,
	"DOTWID",	321,
	"DOTRAD",	322,
	"NUMBER",	323,
	"DIR",	324,
	"DOT",	325,
	"DASH",	326,
	"CHOP",	327,
	"ST",	328,
	"+",	43,
	"-",	45,
	"*",	42,
	"/",	47,
	"%",	37,
	"UMINUS",	329,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
{
	"-no such reduction-",
	"top : piclist",
	"top : /* empty */",
	"top : error",
	"piclist : picture",
	"piclist : piclist picture",
	"picture : prim ST",
	"picture : leftbrace piclist '}'",
	"picture : PLACENAME ':' picture",
	"picture : PLACENAME ':' ST picture",
	"picture : PLACENAME ':' position ST",
	"picture : VARNAME '=' expr ST",
	"picture : DIR",
	"picture : PRINT expr ST",
	"picture : PRINT position ST",
	"picture : ST",
	"leftbrace : '{'",
	"prim : BOX attrlist",
	"prim : CIRCLE attrlist",
	"prim : ELLIPSE attrlist",
	"prim : ARC attrlist",
	"prim : LINE attrlist",
	"prim : ARROW attrlist",
	"prim : SPLINE attrlist",
	"prim : MOVE attrlist",
	"prim : TEXT attrlist",
	"prim : TROFF",
	"prim : lbracket piclist ']'",
	"prim : lbracket piclist ']' attrlist",
	"lbracket : '['",
	"attrlist : attrlist attr",
	"attrlist : /* empty */",
	"attr : ATTR opt_expr",
	"attr : DIR opt_expr",
	"attr : FROM position",
	"attr : TO position",
	"attr : AT position",
	"attr : BY position",
	"attr : WITH CORNER",
	"attr : WITH '.' PLACENAME",
	"attr : WITH position",
	"attr : SAME",
	"attr : textattr",
	"attr : HEAD",
	"attr : DOT opt_expr",
	"attr : DASH opt_expr",
	"attr : CHOP opt_expr",
	"attr : textlist",
	"opt_expr : expr",
	"opt_expr : /* empty */",
	"textlist : TEXT",
	"textlist : TEXT textattr",
	"textlist : textlist TEXT",
	"textlist : textlist TEXT textattr",
	"textattr : LJUST",
	"textattr : RJUST",
	"textattr : SPREAD",
	"textattr : FILL",
	"textattr : CENTER",
	"textattr : ABOVE",
	"textattr : BELOW",
	"position : /* empty */",
	"position : place",
	"position : expr ',' expr",
	"position : position '+' expr ',' expr",
	"position : position '-' expr ',' expr",
	"position : '(' expr ',' expr ')'",
	"position : position '+' '(' expr ',' expr ')'",
	"position : position '-' '(' expr ',' expr ')'",
	"position : '(' place ',' place ')'",
	"position : expr",
	"position : expr '<' position ',' position '>'",
	"position : expr",
	"position : expr BETWEEN position AND position",
	"place : PLACENAME",
	"place : PLACENAME CORNER",
	"place : CORNER PLACENAME",
	"place : HERE",
	"place : last type",
	"place : last type CORNER",
	"place : CORNER last type",
	"place : NTH type",
	"place : NTH type CORNER",
	"place : CORNER NTH type",
	"place : blockname",
	"place : blockname CORNER",
	"place : CORNER blockname",
	"blockname : last BLOCK '.' PLACENAME",
	"blockname : NTH BLOCK '.' PLACENAME",
	"blockname : PLACENAME '.' PLACENAME",
	"last : last LAST",
	"last : NTH LAST",
	"last : LAST",
	"type : BOX",
	"type : CIRCLE",
	"type : ELLIPSE",
	"type : ARC",
	"type : LINE",
	"type : ARROW",
	"type : SPLINE",
	"type : BLOCK",
	"expr : expr '+' expr",
	"expr : expr '-' expr",
	"expr : expr '*' expr",
	"expr : expr '/' expr",
	"expr : expr '%' expr",
	"expr : '-' expr",
	"expr : '(' expr ')'",
	"expr : VARNAME",
	"expr : NUMBER",
	"expr : place DOTX",
	"expr : place DOTY",
	"expr : place DOTHT",
	"expr : place DOTWID",
	"expr : place DOTRAD",
};
#endif /* YYDEBUG */
/*      @(#)yaccpar	11.1 btl-uts  source        */
#ident "@(#)/usr/src/cmd/yacc/yaccpar	1.3 6/11/85 14:16:28 - Amdahl/UTS"
/*	@(#)yaccpar	1.9	*/

/*
** Skeleton parser driver for yacc output
*/

/*
** yacc user known macros and defines
*/
#define YYERROR		goto yyerrlab
#define YYACCEPT	return(0)
#define YYABORT		return(1)
#define YYBACKUP( newtoken, newvalue )\
{\
	if ( yychar >= 0 || ( yyr2[ yytmp ] >> 1 ) != 1 )\
	{\
		yyerror( "syntax error - cannot backup" );\
		goto yyerrlab;\
	}\
	yychar = newtoken;\
	yystate = *yyps;\
	yylval = newvalue;\
	goto yynewstate;\
}
#define YYRECOVERING()	(!!yyerrflag)
#ifndef YYDEBUG
#	define YYDEBUG	1	/* make debugging available */
#endif

/*
** user known globals
*/
int yydebug;			/* set to 1 to get debugging */

/*
** driver internal defines
*/
#define YYFLAG		(-1000)

/*
** global variables used by the parser
*/
YYSTYPE yyv[ YYMAXDEPTH ];	/* value stack */
int yys[ YYMAXDEPTH ];		/* state stack */

YYSTYPE *yypv;			/* top of value stack */
int *yyps;			/* top of state stack */

int yystate;			/* current state */
int yytmp;			/* extra var (lasts between blocks) */

int yynerrs;			/* number of errors */
int yyerrflag;			/* error recovery flag */
int yychar;			/* current input token number */



/*
** yyparse - return 0 if worked, 1 if syntax error not recovered from
*/
int
yyparse()
# line 1 "picy.y"
{
# line 1 "y.tab.c"
	register YYSTYPE *yypvt;	/* top of value stack for $vars */

	/*
	** Initialize externals - yyparse may be called more than once
	*/
	yypv = &yyv[-1];
	yyps = &yys[-1];
	yystate = 0;
	yytmp = 0;
	yynerrs = 0;
	yyerrflag = 0;
	yychar = -1;

	goto yystack;
	{
		register YYSTYPE *yy_pv;	/* top of value stack */
		register int *yy_ps;		/* top of state stack */
		register int yy_state;		/* current state */
		register int  yy_n;		/* internal state number info */

		/*
		** get globals into registers.
		** branch to here only if YYBACKUP was called.
		*/
	yynewstate:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;
		goto yy_newstate;

		/*
		** get globals into registers.
		** either we just started, or we just finished a reduction
		*/
	yystack:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;

		/*
		** top of for (;;) loop while no reductions done
		*/
	yy_stack:
		/*
		** put a state and value onto the stacks
		*/
#if YYDEBUG
		/*
		** if debugging, look up token value in list of value vs.
		** name pairs.  0 and negative (-1) are special values.
		** Note: linear search is used since time is not a real
		** consideration while debugging.
		*/
		if ( yydebug )
		{
			register int yy_i;

			printf( "State %d, token ", yy_state );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ++yy_ps >= &yys[ YYMAXDEPTH ] )	/* room on stack? */
		{
			yyerror( "yacc stack overflow" );
			YYABORT;
		}
		*yy_ps = yy_state;
		*++yy_pv = yyval;

		/*
		** we have a new state - find out what to do
		*/
	yy_newstate:
		if ( ( yy_n = yypact[ yy_state ] ) <= YYFLAG )
			goto yydefault;		/* simple state */
#if YYDEBUG
		/*
		** if debugging, need to mark whether new token grabbed
		*/
		yytmp = yychar < 0;
#endif
		if ( ( yychar < 0 ) && ( ( yychar = yylex() ) < 0 ) )
			yychar = 0;		/* reached EOF */
#if YYDEBUG
		if ( yydebug && yytmp )
		{
			register int yy_i;

			printf( "Received token " );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ( ( yy_n += yychar ) < 0 ) || ( yy_n >= YYLAST ) )
			goto yydefault;
		if ( yychk[ yy_n = yyact[ yy_n ] ] == yychar )	/*valid shift*/
		{
			yychar = -1;
			yyval = yylval;
			yy_state = yy_n;
			if ( yyerrflag > 0 )
				yyerrflag--;
			goto yy_stack;
		}

	yydefault:
		if ( ( yy_n = yydef[ yy_state ] ) == -2 )
		{
#if YYDEBUG
			yytmp = yychar < 0;
#endif
			if ( ( yychar < 0 ) && ( ( yychar = yylex() ) < 0 ) )
				yychar = 0;		/* reached EOF */
#if YYDEBUG
			if ( yydebug && yytmp )
			{
				register int yy_i;

				printf( "Received token " );
				if ( yychar == 0 )
					printf( "end-of-file\n" );
				else if ( yychar < 0 )
					printf( "-none-\n" );
				else
				{
					for ( yy_i = 0;
						yytoks[yy_i].t_val >= 0;
						yy_i++ )
					{
						if ( yytoks[yy_i].t_val
							== yychar )
						{
							break;
						}
					}
					printf( "%s\n", yytoks[yy_i].t_name );
				}
			}
#endif /* YYDEBUG */
			/*
			** look through exception table
			*/
			{
				register int *yyxi = yyexca;

				while ( ( *yyxi != -1 ) ||
					( yyxi[1] != yy_state ) )
				{
					yyxi += 2;
				}
				while ( ( *(yyxi += 2) >= 0 ) &&
					( *yyxi != yychar ) )
					;
				if ( ( yy_n = yyxi[1] ) < 0 )
					YYACCEPT;
			}
		}

		/*
		** check for syntax error
		*/
		if ( yy_n == 0 )	/* have an error */
		{
			/* no worry about speed here! */
			switch ( yyerrflag )
			{
			case 0:		/* new error */
				yyerror( "syntax error" );
				goto skip_init;
			yyerrlab:
				/*
				** get globals into registers.
				** we have a user generated syntax type error
				*/
				yy_pv = yypv;
				yy_ps = yyps;
				yy_state = yystate;
				yynerrs++;
			skip_init:
			case 1:
			case 2:		/* incompletely recovered error */
					/* try again... */
				yyerrflag = 3;
				/*
				** find state where "error" is a legal
				** shift action
				*/
				while ( yy_ps >= yys )
				{
					yy_n = yypact[ *yy_ps ] + YYERRCODE;
					if ( yy_n >= 0 && yy_n < YYLAST &&
						yychk[yyact[yy_n]] == YYERRCODE)					{
						/*
						** simulate shift of "error"
						*/
						yy_state = yyact[ yy_n ];
						goto yy_stack;
					}
					/*
					** current state has no shift on
					** "error", pop stack
					*/
#if YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if ( yydebug )
						printf( _POP_, *yy_ps,
							yy_ps[-1] );
#	undef _POP_
#endif
					yy_ps--;
					yy_pv--;
				}
				/*
				** there is no state on stack with "error" as
				** a valid shift.  give up.
				*/
				YYABORT;
			case 3:		/* no shift yet; eat a token */
#if YYDEBUG
				/*
				** if debugging, look up token in list of
				** pairs.  0 and negative shouldn't occur,
				** but since timing doesn't matter when
				** debugging, it doesn't hurt to leave the
				** tests here.
				*/
				if ( yydebug )
				{
					register int yy_i;

					printf( "Error recovery discards " );
					if ( yychar == 0 )
						printf( "token end-of-file\n" );
					else if ( yychar < 0 )
						printf( "token -none-\n" );
					else
					{
						for ( yy_i = 0;
							yytoks[yy_i].t_val >= 0;
							yy_i++ )
						{
							if ( yytoks[yy_i].t_val
								== yychar )
							{
								break;
							}
						}
						printf( "token %s\n",
							yytoks[yy_i].t_name );
					}
				}
#endif /* YYDEBUG */
				if ( yychar == 0 )	/* reached EOF. quit */
					YYABORT;
				yychar = -1;
				goto yy_newstate;
			}
		}/* end if ( yy_n == 0 ) */
		/*
		** reduction by production yy_n
		** put stack tops, etc. so things right after switch
		*/
#if YYDEBUG
		/*
		** if debugging, print the string that is the user's
		** specification of the reduction which is just about
		** to be done.
		*/
		if ( yydebug )
			printf( "Reduce by (%d) \"%s\"\n",
				yy_n, yyreds[ yy_n ] );
#endif
		yytmp = yy_n;			/* value to switch over */
		yypvt = yy_pv;			/* $vars top of value stack */
		/*
		** Look in goto table for next state
		** Sorry about using yy_state here as temporary
		** register variable, but why not, if it works...
		** If yyr2[ yy_n ] doesn't have the low order bit
		** set, then there is no action to be done for
		** this reduction.  So, no saving & unsaving of
		** registers done.  The only difference between the
		** code just after the if and the body of the if is
		** the goto yy_stack in the body.  This way the test
		** can be made before the choice of what to do is needed.
		*/
		{
			/* length of production doubled with extra bit */
			register int yy_len = yyr2[ yy_n ];

			if ( !( yy_len & 01 ) )
			{
				yy_len >>= 1;
				yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
				yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
					*( yy_ps -= yy_len ) + 1;
				if ( yy_state >= YYLAST ||
					yychk[ yy_state =
					yyact[ yy_state ] ] != -yy_n )
				{
					yy_state = yyact[ yypgo[ yy_n ] ];
				}
				goto yy_stack;
			}
			yy_len >>= 1;
			yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
			yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
				*( yy_ps -= yy_len ) + 1;
			if ( yy_state >= YYLAST ||
				yychk[ yy_state = yyact[ yy_state ] ] != -yy_n )
			{
				yy_state = yyact[ yypgo[ yy_n ] ];
			}
		}
					/* save until reenter driver code */
		yystate = yy_state;
		yyps = yy_ps;
		yypv = yy_pv;
	}
	/*
	** code supplied by user is placed in this switch
	*/
	switch( yytmp )
	{
		
case 3:
# line 42 "picy.y"
{ yyerror("syntax error"); } break;
case 6:
# line 51 "picy.y"
{ codegen = 1; } break;
case 7:
# line 52 "picy.y"
{ rightthing(yypvt[-2], '}'); yyval = yypvt[-1]; } break;
case 8:
# line 53 "picy.y"
{ makevar(yypvt[-2], PLACENAME, yypvt[-0]); yyval = yypvt[-0]; } break;
case 9:
# line 54 "picy.y"
{ makevar(yypvt[-3], PLACENAME, yypvt[-0]); yyval = yypvt[-0]; } break;
case 10:
# line 55 "picy.y"
{ makevar(yypvt[-3], PLACENAME, yypvt[-1]); yyval = yypvt[-1]; } break;
case 11:
# line 56 "picy.y"
{ makevar(yypvt[-3], VARNAME, yypvt[-1]); yyval = yypvt[-3]; } break;
case 12:
# line 57 "picy.y"
{ setdir(yypvt[-0]); } break;
case 13:
# line 58 "picy.y"
{ printexpr(yypvt[-1]); } break;
case 14:
# line 59 "picy.y"
{ printpos(yypvt[-1]); } break;
case 16:
# line 64 "picy.y"
{ leftthing('{'); } break;
case 17:
# line 68 "picy.y"
{ yyval = boxgen(yypvt[-1]); } break;
case 18:
# line 69 "picy.y"
{ yyval = circgen(yypvt[-1]); } break;
case 19:
# line 70 "picy.y"
{ yyval = circgen(yypvt[-1]); } break;
case 20:
# line 71 "picy.y"
{ yyval = arcgen(yypvt[-1]); } break;
case 21:
# line 72 "picy.y"
{ yyval = linegen(yypvt[-1]); } break;
case 22:
# line 73 "picy.y"
{ yyval = linegen(yypvt[-1]); } break;
case 23:
# line 74 "picy.y"
{ yyval = splinegen(yypvt[-1]); } break;
case 24:
# line 75 "picy.y"
{ yyval = movegen(yypvt[-1]); } break;
case 25:
# line 76 "picy.y"
{ yyval = textgen(yypvt[-1]); } break;
case 26:
# line 77 "picy.y"
{ yyval = troffgen(yypvt[-0]); } break;
case 27:
# line 78 "picy.y"
{ yyval=rightthing(yypvt[-2],']'); } break;
case 28:
# line 79 "picy.y"
{ yyval = blockgen(yypvt[-4], yypvt[-3], yypvt[-1]); } break;
case 29:
# line 83 "picy.y"
{ yyval = leftthing('['); } break;
case 31:
# line 88 "picy.y"
{ makeattr(0, 0); } break;
case 32:
# line 92 "picy.y"
{ makeattr(yypvt[-1], yypvt[-0]); } break;
case 33:
# line 93 "picy.y"
{ makeattr(yypvt[-1], yypvt[-0]); } break;
case 34:
# line 94 "picy.y"
{ makeattr(yypvt[-1], yypvt[-0]); } break;
case 35:
# line 95 "picy.y"
{ makeattr(yypvt[-1], yypvt[-0]); } break;
case 36:
# line 96 "picy.y"
{ makeattr(yypvt[-1], yypvt[-0]); } break;
case 37:
# line 97 "picy.y"
{ makeattr(yypvt[-1], yypvt[-0]); } break;
case 38:
# line 98 "picy.y"
{ makeattr(WITH, yypvt[-0]); } break;
case 39:
# line 99 "picy.y"
{ makeattr(PLACE, getblock(getlast(1,BLOCK), yypvt[-0])); } break;
case 40:
# line 100 "picy.y"
{ makeattr(PLACE, yypvt[-0]); } break;
case 41:
# line 101 "picy.y"
{ makeattr(SAME, yypvt[-0]); } break;
case 42:
# line 102 "picy.y"
{ makeattr(yypvt[-0], 0); } break;
case 43:
# line 103 "picy.y"
{ makeattr(HEAD, yypvt[-0]); } break;
case 44:
# line 104 "picy.y"
{ makeattr(DOT, yypvt[-0]); } break;
case 45:
# line 105 "picy.y"
{ makeattr(DASH, yypvt[-0]); } break;
case 46:
# line 106 "picy.y"
{ makeattr(CHOP, yypvt[-0]); } break;
case 49:
# line 112 "picy.y"
{ yyval = 0; } break;
case 50:
# line 116 "picy.y"
{ makeattr(CENTER, yypvt[-0]); } break;
case 51:
# line 117 "picy.y"
{ makeattr(yypvt[-0], yypvt[-1]); } break;
case 52:
# line 118 "picy.y"
{ makeattr(CENTER, yypvt[-0]); } break;
case 53:
# line 119 "picy.y"
{ makeattr(yypvt[-0], yypvt[-1]); } break;
case 63:
# line 134 "picy.y"
{ yyval = makepos(yypvt[-2], yypvt[-0]); } break;
case 64:
# line 135 "picy.y"
{ yyval = fixpos(yypvt[-4], yypvt[-2], yypvt[-0]); } break;
case 65:
# line 136 "picy.y"
{ yyval = fixpos(yypvt[-4], -yypvt[-2], -yypvt[-0]); } break;
case 66:
# line 137 "picy.y"
{ yyval = makepos(yypvt[-3], yypvt[-1]); } break;
case 67:
# line 138 "picy.y"
{ yyval = fixpos(yypvt[-6], yypvt[-3], yypvt[-1]); } break;
case 68:
# line 139 "picy.y"
{ yyval = fixpos(yypvt[-6], -yypvt[-3], -yypvt[-1]); } break;
case 69:
# line 140 "picy.y"
{ yyval = makepos(getcomp(yypvt[-3],DOTX), getcomp(yypvt[-1],DOTY)); } break;
case 70:
# line 141 "picy.y"
{ between=lastfloat; } break;
case 71:
# line 142 "picy.y"
{ yyval = makebetween(yypvt[-6], yypvt[-3], yypvt[-1]); } break;
case 72:
# line 143 "picy.y"
{ between=lastfloat; } break;
case 73:
# line 144 "picy.y"
{ yyval = makebetween(yypvt[-5], yypvt[-2], yypvt[-0]); } break;
case 74:
# line 148 "picy.y"
{ yyval = getpos(getvar(yypvt[-0]), 0); } break;
case 75:
# line 149 "picy.y"
{ yyval = getpos(getvar(yypvt[-1]), yypvt[-0]); } break;
case 76:
# line 150 "picy.y"
{ yyval = getpos(getvar(yypvt[-0]), yypvt[-1]); } break;
case 77:
# line 151 "picy.y"
{ yyval = gethere(yypvt[-0]); } break;
case 78:
# line 152 "picy.y"
{ yyval = getlast(yypvt[-1], yypvt[-0]); } break;
case 79:
# line 153 "picy.y"
{ yyval = getpos(getlast(yypvt[-2], yypvt[-1]), yypvt[-0]); } break;
case 80:
# line 154 "picy.y"
{ yyval = getpos(getlast(yypvt[-1], yypvt[-0]), yypvt[-2]); } break;
case 81:
# line 155 "picy.y"
{ yyval = getfirst(yypvt[-1], yypvt[-0]); } break;
case 82:
# line 156 "picy.y"
{ yyval = getpos(getfirst(yypvt[-2], yypvt[-1]), yypvt[-0]); } break;
case 83:
# line 157 "picy.y"
{ yyval = getpos(getfirst(yypvt[-1], yypvt[-0]), yypvt[-2]); } break;
case 85:
# line 159 "picy.y"
{ yyval = getpos(yypvt[-1], yypvt[-0]); } break;
case 86:
# line 160 "picy.y"
{ yyval = getpos(yypvt[-0], yypvt[-1]); } break;
case 87:
# line 164 "picy.y"
{ yyval = getblock(getlast(yypvt[-3],yypvt[-2]), yypvt[-0]); } break;
case 88:
# line 165 "picy.y"
{ yyval = getblock(getfirst(yypvt[-3],yypvt[-2]), yypvt[-0]); } break;
case 89:
# line 166 "picy.y"
{ yyval = getblock(getvar(yypvt[-2]), yypvt[-0]); } break;
case 90:
# line 170 "picy.y"
{ yyval = yypvt[-1] + 1; } break;
case 91:
# line 171 "picy.y"
{ yyval = yypvt[-1]; } break;
case 92:
# line 172 "picy.y"
{ yyval = 1; } break;
case 101:
# line 187 "picy.y"
{ yyval = yypvt[-2] + yypvt[-0]; } break;
case 102:
# line 188 "picy.y"
{ yyval = yypvt[-2] - yypvt[-0]; } break;
case 103:
# line 189 "picy.y"
{ yyval = yypvt[-2] * yypvt[-0]; } break;
case 104:
# line 190 "picy.y"
{ yyval = yypvt[-2] / yypvt[-0]; } break;
case 105:
# line 191 "picy.y"
{ yyval = yypvt[-2] % yypvt[-0]; } break;
case 106:
# line 192 "picy.y"
{ yyval = -yypvt[-0]; } break;
case 107:
# line 193 "picy.y"
{ yyval = yypvt[-1]; } break;
case 108:
# line 194 "picy.y"
{ yyval = getvar(yypvt[-0]); } break;
case 110:
# line 196 "picy.y"
{ yyval = getcomp(yypvt[-1], yypvt[-0]); } break;
case 111:
# line 197 "picy.y"
{ yyval = getcomp(yypvt[-1], yypvt[-0]); } break;
case 112:
# line 198 "picy.y"
{ yyval = getcomp(yypvt[-1], yypvt[-0]); } break;
case 113:
# line 199 "picy.y"
{ yyval = getcomp(yypvt[-1], yypvt[-0]); } break;
case 114:
# line 200 "picy.y"
{ yyval = getcomp(yypvt[-1], yypvt[-0]); } break;
	}
	goto yystack;		/* reset registers in driver code */
}
