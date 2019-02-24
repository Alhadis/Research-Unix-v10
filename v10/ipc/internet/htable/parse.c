
# line 2 "parse.y"

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
static char sccsid[] = "@(#)parse.y	5.4 (Berkeley) 6/18/88";
#endif /* not lint */
#include "htable.h"

#include "y.tab.h"

# line 28 "parse.y"
typedef union  {
	int	number;
	struct	addr *addrlist;
	struct	name *namelist;
} YYSTYPE;
# define END 257
# define NUMBER 258
# define KEYWORD 259
# define NAME 260
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern short yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256

# line 139 "parse.y"


#include <stdio.h>

extern int yylineno;

yyerror(msg)
	char *msg;
{
	fprintf(stderr, "\"%s\", line %d: %s\n", infile, yylineno, msg);
}
short yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 22
# define YYLAST 203
short yyact[]={

  35,  29,  22,  38,  31,  24,   4,   5,  16,   3,
  32,  26,  18,  11,  42,  39,   8,  40,  33,  27,
  19,  12,   7,  28,  21,  14,  41,  20,  13,  36,
  15,   9,   2,  10,   6,  37,  30,  23,   1,   0,
   0,   0,   0,   0,   0,  17,   0,   0,   0,   0,
   0,  25,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,  43,   0,   0,   0,   0,   0,   0,   0,   0,
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
   0,   0,   0,   0,   0,   0,   0,   0,   0,  34,
   0,   0,  38 };
short yypact[]={

-250,-250,-1000, -36,-241,-1000,-1000,-245,-1000, -37,
 -16, -21,-252,-245,-246, -38, -17,-1000, -22,-255,
-252,-247,-1000, -39,-1000,-1000, -23,-256,-248,-1000,
 -40,-1000,-1000, -58,-1000,-242, -41, -18,-1000,-1000,
-243,-257,-1000,-1000 };
short yypgo[]={

   0,  38,  30,  37,  36,  29,  35,  31,  33,  32 };
short yyr1[]={

   0,   1,   1,   9,   9,   9,   9,   9,   9,   9,
   7,   7,   8,   2,   2,   3,   3,   4,   4,   5,
   5,   6 };
short yyr2[]={

   0,   1,   2,   7,   9,  11,  12,  13,   2,   1,
   1,   3,   7,   1,   3,   0,   1,   0,   1,   1,
   3,   1 };
short yychk[]={

-1000,  -1,  -9, 259, 256, 257,  -9,  58, 257,  -7,
  -8, 258,  58,  44,  46,  -2, 260,  -7, 258,  58,
  44,  46, 257,  -3, 260,  -2, 258,  58,  46, 257,
  -4, 260, 258,  58, 257,  58,  -5,  -6, 260, 257,
  58,  44, 257,  -5 };
short yydef[]={

   0,  -2,   1,   0,   0,   9,   2,   0,   8,   0,
  10,   0,   0,   0,   0,   0,  13,  11,   0,  15,
   0,   0,   3,   0,  16,  14,   0,  17,   0,   4,
   0,  18,  12,   0,   5,   0,   0,  19,  21,   6,
   0,   0,   7,  20 };
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
# line 47 "parse.y"
 {
		do_entry(yypvt[-6].number, yypvt[-4].addrlist, yypvt[-2].namelist, NONAME, NONAME, NONAME);
	} break;
case 4:
# line 51 "parse.y"
 {
		do_entry(yypvt[-8].number, yypvt[-6].addrlist, yypvt[-4].namelist, yypvt[-2].namelist, NONAME, NONAME);
	} break;
case 5:
# line 55 "parse.y"
 {
		do_entry(yypvt[-10].number, yypvt[-8].addrlist, yypvt[-6].namelist, yypvt[-4].namelist, yypvt[-2].namelist, NONAME);
	} break;
case 6:
# line 59 "parse.y"
 {
		do_entry(yypvt[-11].number, yypvt[-9].addrlist, yypvt[-7].namelist, yypvt[-5].namelist, yypvt[-3].namelist, NONAME);
	} break;
case 7:
# line 63 "parse.y"
 {
		do_entry(yypvt[-12].number, yypvt[-10].addrlist, yypvt[-8].namelist, yypvt[-6].namelist, yypvt[-4].namelist, yypvt[-2].namelist);
	} break;
case 10:
# line 71 "parse.y"
 {
		yyval.addrlist = yypvt[-0].addrlist;
	} break;
case 11:
# line 75 "parse.y"
 {
		yypvt[-2].addrlist->addr_link = yypvt[-0].addrlist;
		yyval.addrlist = yypvt[-2].addrlist;
	} break;
case 12:
# line 82 "parse.y"
 {
		char *a;

		yyval.addrlist = (struct addr *)malloc(sizeof (struct addr));
		a = (char *)&(yyval.addrlist->addr_val);
		a[0] = yypvt[-6].number; a[1] = yypvt[-4].number; a[2] = yypvt[-2].number; a[3] = yypvt[-0].number;
		yyval.addrlist->addr_link = NOADDR;
	} break;
case 13:
# line 93 "parse.y"
 {
		yyval.namelist = yypvt[-0].namelist;
	} break;
case 14:
# line 97 "parse.y"
 {
		yypvt[-2].namelist->name_link = yypvt[-0].namelist;
		yyval.namelist = yypvt[-2].namelist;
	} break;
case 15:
# line 104 "parse.y"
 {
		yyval.namelist = NONAME;
	} break;
case 16:
# line 108 "parse.y"
 {
		yyval.namelist = yypvt[-0].namelist;
	} break;
case 17:
# line 114 "parse.y"
 {
		yyval.namelist = NONAME;
	} break;
case 18:
# line 118 "parse.y"
 {
		yyval.namelist = yypvt[-0].namelist;
	} break;
case 19:
# line 124 "parse.y"
 {
		yyval.namelist = yypvt[-0].namelist;
	} break;
case 20:
# line 128 "parse.y"
 {
		yypvt[-2].namelist->name_link = yypvt[-0].namelist;
		yyval.namelist = yypvt[-2].namelist;
	} break;
case 21:
# line 135 "parse.y"
 {
		yyval.namelist = yypvt[-0].namelist;
	} break;
	}
	goto yystack;  /* stack new state and value */
}
