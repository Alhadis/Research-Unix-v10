
# line 2 "parsedag.y"
#include "draw_dag.h"
#include "dag.h"
#define MIN_RANK_SEP	4

static pair_t*		nodelist;
static DAG_edge_t*	this_edge;
static boolean		is_ordered = false;

int					Syntax_error;
DAG_node_t			Reset_node,Default_node,Proto_node;
DAG_edge_t			Reset_edge,Default_edge;
static boolean		set_pointsize,set_label,set_shape,set_color,set_xsize,set_ysize;

static void teardown (pair_t *e) {
	pair_t *f;
	while (e) {
		f = e->next;
		delete e;
		e = f;
	}
}

static void init_proto() {
	Proto_node = Default_node;
	set_pointsize = set_label = set_shape = set_color = set_xsize = set_ysize = false;
}

static void apply_proto(DAG_node_t *np) {
	/* label setting has precedence over size! */
	if (set_pointsize) np->setpointsize(Proto_node.pointsize);
	if (set_label) np->setlabel(Proto_node.label.type,Proto_node.label.value);
	if (set_shape) np->setshape(Proto_node.shape.type,Proto_node.shape.value);
	if (set_color) np->setcolor(Proto_node.color);
	if (set_xsize) np->setxsize(Proto_node.xsize);
	if (set_ysize) np->setysize(Proto_node.ysize);
	np->autosize();
}

static void install_proto() {
	Proto_node.autosize();
	for (pair_t* p = nodelist; p; p = p->next)
		apply_proto(Node[p->node]);
}

/* append list1 to list2 */
static DAG_edge_t* append(DAG_edge_t *list1, DAG_edge_t *list2) {
if (!list2) panic("null list2");
	DAG_edge_t *e,*f;
	e = list2;
	while (f = e->nextof()) e = f;
	e->next = (edge_t*) list1;
	return list2;
}

/* create a new set of same rank nodes within the Options struct */
static int* &newset() {
	const int hunksize = 16;
	if (!Options.same_nodes)
		Options.same_nodes = new int*[hunksize];
	else if (!(Options.n_same_nodes % hunksize))
		Options.same_nodes = (int**)realloc((char*)Options.same_nodes,
			(Options.n_same_nodes + hunksize + 1)*sizeof(int*));
	Options.same_nodes[Options.n_same_nodes] = 0;
	return(Options.same_nodes[Options.n_same_nodes++]);
}

/* take the union of same rank nodes */
overload set_union;
static void set_union (int* &ptr, pair_t *nlist) {
	pair_t *e;
	int olen = 0, nlen = 0;
	for (e = nlist; e; e = e->next) nlen++;
	if (!nlen) return;
	if (ptr) {
		while (ptr[olen++] >= 0);
		ptr = (int*)realloc((char*)ptr,(nlen+olen)*sizeof(int));
	}
	else ptr = new int[nlen + 1];

	for (e = nlist; e; e = e->next)
		ptr[olen++] = e->node;
	ptr[olen] = -1;
}

static void set_union(int* &ptr, DAG_edge_t *elist) {
	pair_t *p = 0;
	while (elist) {
		p = new pair_t(elist->node,p);
		elist = elist->nextof();
	}
	set_union(ptr,p);
	teardown(p);
}

static void make_invis_edge(int fromnode,int tonode) {
	DAG_edge_t *e = new DAG_edge_t();
	e->ink = invis_ink;
	e->node = tonode;
	e->next = Edge[fromnode];
	Edge[fromnode] = e;
}

static void enter_edgelist(int tailnode, DAG_edge_t* elist) {
	DAG_edge_t *e;
	if (is_ordered) {
		set_union(newset(),elist);
		for (e = elist; e->nextof(); e = e->nextof())
			make_invis_edge(e->node,e->nextof()->node);
		is_ordered = false;
	}
	Edge[tailnode] = append(Edge[tailnode],elist);
}

static void enter_backedgelist(int headnode, DAG_edge_t* elist) {
	DAG_edge_t *e = elist;
	while (e) {
		DAG_edge_t *f = e->nextof();
		e->next = 0;
		e->flipped = true;
		int tailnode = e->node;
		e->node = headnode;
		Edge[tailnode] = append(Edge[tailnode],e);
		e = f;
	}
}	

static void enter_pathlist(int tailnode, DAG_edge_t* elist) {
	DAG_edge_t *e,*f;
	e = elist;
	while (e) {
		f = e;
		e = e->nextof();
		f->next = Edge[tailnode];
		Edge[tailnode] = f;
		tailnode = f->node;
	}
	is_ordered = false;
}

static void enter_backpathlist(int headnode, DAG_edge_t *elist) {
	DAG_edge_t *e = elist;
	while (e) {
		DAG_edge_t *f = e->nextof();
		e->next = 0;
		e->flipped = true;
		int tailnode = e->node;
		e->node = headnode;
		Edge[tailnode] = append(Edge[tailnode],e);
		e = f;
		headnode = tailnode;
	}
}


# line 156 "parsedag.y"
typedef union  {
	char		*s;
	int			i;
	pair_t		*p;	// for node lists
	DAG_edge_t	*e;	// for edge lists
} YYSTYPE;
# define AS 257
# define BACKEDGE 258
# define BACKPATH 259
# define COLOR 260
# define DASHED 261
# define DOTTED 262
# define DRAW 263
# define EDGE 264
# define EDGES 265
# define EQUALLY 266
# define EXACTLY 267
# define FROM 268
# define HEIGHT 269
# define INVIS 270
# define LABEL 271
# define MAXIMUM 272
# define MINIMUM 273
# define NODES 274
# define ORDERED 275
# define PATH 276
# define POINTSIZE 277
# define RANK 278
# define RANKS 279
# define SAME 280
# define SEPARATE 281
# define SOLID 282
# define TO 283
# define WEIGHT 284
# define WIDTH 285
# define STRING 286
# define DESC 287
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern short yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256
short yyexca[] ={
-1, 0,
	0, 4,
	258, 4,
	259, 4,
	263, 4,
	264, 4,
	272, 4,
	273, 4,
	275, 4,
	276, 4,
	280, 4,
	281, 4,
	286, 4,
	59, 4,
	-2, 0,
-1, 1,
	0, -1,
	-2, 0,
-1, 36,
	283, 46,
	286, 46,
	59, 46,
	44, 46,
	-2, 44,
	};
# define YYNPROD 72
# define YYLAST 282
short yyact[]={

  20,  81,  84,  85, 117,  58,  45, 111, 115, 114,
  72,  86,  79,  73, 108, 107,  58, 104,  80,  45,
  98,  92,  69,  83,  71,  78,  58,  58, 110,  89,
  70,  97,  58,  99, 105, 106,  20,  45,  68,  45,
  31,  96,  95,  35,  24,  39,  21,  56, 109,  88,
 102, 101,  91,  23,  90,  87,  21,  49,  30,  29,
  28, 119, 118,  21,   3,  21,  77, 103,  67,  76,
  36,  66,  33,  15,  50,  34,  10,  22,  27,  25,
  60,  26,  47,  46,  43,   7,   6,  38,  38,  38,
  38,   5,   4,  44,  37,   2,   1,  15,  82,  25,
  25,  25,   0,  48,   0,  57,  53,  54,  55,  32,
  62,  59,  40,  41,  42,   0,  74,   0,  75,   0,
   0,   0,   0,   0,  44,  44,  44,   0,   0,   0,
  94,  93,   0,   0,  57, 100,  57,  57,  57,   0,
   0,   0,   0, 112, 100,   0, 113,  61, 116,   0,
  63,  64,  65,   0,   0,   0,   0,   0,   0,   0,
   0, 112,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,  17,
  19,   0,   0,   0,   8,  16,   0,   0,  81,  84,
  85,   0,   0,  13,  12,   0,   9,  18,  86,  79,
   0,  14,  11,   0,   0,  80,  72,  21,   0,  73,
  83,  72,  78,   0,  73,  17,  19,   0,  69,   0,
  71,  16,   0,  69,  35,  71,  70,  21,  21,   0,
   0,  70,   0,  18,  68,  35,   0,   0,  21,  68,
   0,  21,   0,  21,  35,  35,  35,  21,  21,  21,
   0,  35,  51,   0,  21,   0,   0,  52,   0,  21,
   0,  21 };
short yypact[]={

-192,-1000, -59,-1000,-1000,-1000,-1000,-1000,-221,-1000,
-1000,-1000,-218,-219,-220, -19,-223,-223,-223,-223,
-1000,-1000, -25,-1000,-1000,-1000, -23,  -2,-230,-230,
-230,-1000, -12,-1000,-1000,-230,-1000,-240,-1000,-230,
-240,-240,-240,-247,-1000,-230,-247,-259,-1000,-1000,
-1000,-231,-237,  -5,  -7, -38,-1000,-1000,-240,-1000,
-259, -17,-1000, -18, -28, -39, -26,-1000,-235,-236,
-269,-252,-272,-238,-1000, -31, -52,-1000,-269,-278,
-269,-282,-1000,-1000,-1000,-1000,-1000,-1000,-205,-1000,
-1000,-1000,-1000,-1000,-259,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000 };
short yypgo[]={

   0,  68,  66,  98,  70,  67,  94, 109,  75,  72,
  77,  96,  95,  92,  91,  86,  85,  84,  71,  83,
  82,  69,  81,  76,  80,  78,  74 };
short yyr1[]={

   0,  11,  11,  12,  12,  13,  13,  13,  17,  14,
  19,  14,  20,  14,  10,  10,  10,  18,  18,   1,
   1,   1,   1,   1,   1,   1,   1,  22,  15,  15,
  23,  23,  23,  23,  23,  23,  23,   6,   6,   7,
   7,   7,   9,   9,  24,   8,   8,  21,  21,   2,
   2,   2,   2,   2,   2,  16,  16,  16,  16,  25,
  25,  26,  26,  26,  26,  26,   3,   3,   3,   3,
   4,   5 };
short yyr2[]={

   0,   1,   1,   2,   0,   1,   1,   1,   0,   5,
   0,   5,   0,   5,   2,   3,   1,   1,   2,   2,
   2,   2,   2,   2,   2,   2,   2,   0,   3,   1,
   2,   3,   4,   4,   4,   4,   1,   1,   2,   1,
   2,   3,   1,   2,   0,   3,   1,   1,   2,   2,
   2,   2,   2,   2,   1,   3,   4,   4,   4,   2,
   0,   2,   3,   3,   2,   2,   1,   1,   1,   1,
   1,   1 };
short yychk[]={

-1000, -11, -12, 256, -13, -14, -15, -16, 263, 275,
 -23, 281, 273, 272, 280,  -4, 264, 258, 276, 259,
  59, 286, -10, 274, 265,  -4, -22, -25, 278, 278,
 278,  59,  -7,  -9,  -8, 283,  -4,  -6,  -4, 268,
  -6,  -6,  -6, -17,  -4,  44, -19, -20, -23,  59,
 -26, 274, 279, -10, -10, -10,  59,  -9,  44,  -8,
 -24,  -7,  -4,  -7,  -7,  -7, -18,  -1, 285, 269,
 277, 271, 257, 260,  -4, -18, -21,  -2, 284, 271,
 277, 260,  -3, 282, 261, 262, 270, 286, 286, 266,
  59,  59,  59,  -9, -21,  59,  59,  59,  59,  59,
  -1, 286, 286,  -5, 286, 286, 287, 287, 286, 286,
  59,  59,  -2,  -5, 287, 286,  -5, 286, 267, 266 };
short yydef[]={

  -2,  -2,   1,   2,   3,   5,   6,   7,   0,  27,
  29,  60,   0,   0,   0,   0,   0,   0,   0,   0,
  36,  70,   8,  10,  12,  16,   0,   0,   0,   0,
   0,  30,   0,  39,  42,   0,  -2,   0,  37,   0,
   0,   0,   0,   0,  14,   0,   0,   0,  28,  55,
  59,   0,   0,   0,   0,   0,  31,  40,   0,  43,
   0,   0,  38,   0,   0,   0,   0,  17,   0,   0,
   0,   0,   0,   0,  15,   0,   0,  47,   0,   0,
   0,   0,  54,  66,  67,  68,  69,  61,  65,  64,
  56,  57,  58,  41,  45,  32,  33,  34,  35,   9,
  18,  19,  20,  21,  71,  22,  23,  24,  25,  26,
  11,  13,  48,  49,  50,  51,  52,  53,  62,  63 };
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
# line 171 "parsedag.y"
{make_drawing();} break;
case 8:
# line 184 "parsedag.y"
{init_proto(); nodelist = yypvt[-0].p;} break;
case 9:
# line 185 "parsedag.y"
{install_proto(); teardown(yypvt[-3].p);} break;
case 10:
# line 186 "parsedag.y"
{init_proto(); nodelist = 0;} break;
case 11:
# line 187 "parsedag.y"
{apply_proto(&Default_node);} break;
case 12:
# line 188 "parsedag.y"
{this_edge = &Default_edge;} break;
case 14:
# line 192 "parsedag.y"
{yyval.p = new pair_t(yypvt[-0].i,yypvt[-1].p);} break;
case 15:
# line 194 "parsedag.y"
{yyval.p = new pair_t(yypvt[-0].i,yypvt[-2].p);} break;
case 16:
# line 196 "parsedag.y"
{yyval.p = new pair_t(yypvt[-0].i,(pair_t*)0);} break;
case 19:
# line 205 "parsedag.y"
{
						Proto_node.setxsize((int)(Resolution*atof(yypvt[-0].s)));
						set_xsize = true;
					} break;
case 20:
# line 211 "parsedag.y"
{
						Proto_node.setysize((int)(Resolution*atof(yypvt[-0].s)));
						set_ysize = true;
					} break;
case 21:
# line 216 "parsedag.y"
{
						Proto_node.setpointsize(yypvt[-0].i);
						set_pointsize = true;
					} break;
case 22:
# line 221 "parsedag.y"
{
						Proto_node.setlabel(STRING,yypvt[-0].s);
						set_label = true;
					} break;
case 23:
# line 226 "parsedag.y"
{
						Proto_node.setlabel(DESC,yypvt[-0].s);
						set_label = true;
					} break;
case 24:
# line 231 "parsedag.y"
{
						Proto_node.setshape(DESC,yypvt[-0].s);
						set_shape = true;
					} break;
case 25:
# line 236 "parsedag.y"
{
						Proto_node.setshape(STRING,yypvt[-0].s);
						set_shape = true;
					} break;
case 26:
# line 241 "parsedag.y"
{
						Proto_node.setcolor(yypvt[-0].s);
						set_color = true;
					} break;
case 27:
# line 247 "parsedag.y"
{is_ordered = true;} break;
case 31:
# line 253 "parsedag.y"
{enter_edgelist(yypvt[-2].i,yypvt[-1].e);} break;
case 32:
# line 255 "parsedag.y"
{enter_edgelist(yypvt[-2].i,yypvt[-1].e);} break;
case 33:
# line 257 "parsedag.y"
{enter_backedgelist(yypvt[-2].i,yypvt[-1].e);} break;
case 34:
# line 259 "parsedag.y"
{enter_pathlist(yypvt[-2].i,yypvt[-1].e);} break;
case 35:
# line 261 "parsedag.y"
{enter_backpathlist(yypvt[-2].i,yypvt[-1].e);} break;
case 38:
# line 267 "parsedag.y"
{yyval.i = yypvt[-0].i;} break;
case 40:
# line 271 "parsedag.y"
{yyval.e = append(yypvt[-0].e,yypvt[-1].e);} break;
case 41:
# line 273 "parsedag.y"
{yyval.e = append(yypvt[-0].e,yypvt[-2].e);} break;
case 43:
# line 278 "parsedag.y"
{yyval.e = yypvt[-0].e;} break;
case 44:
# line 282 "parsedag.y"
{
						this_edge = new DAG_edge_t();
						*this_edge = Default_edge;
						this_edge->node = yypvt[-0].i;
					} break;
case 45:
# line 288 "parsedag.y"
{yyval.e = this_edge;} break;
case 46:
# line 290 "parsedag.y"
{
						this_edge = new DAG_edge_t();
						*this_edge = Default_edge;
						this_edge->node = yypvt[-0].i;
						yyval.e = this_edge;
					} break;
case 49:
# line 303 "parsedag.y"
{this_edge->setweight(yypvt[-0].i);} break;
case 50:
# line 305 "parsedag.y"
{this_edge->setlabel(DESC,newstring(yypvt[-0].s));} break;
case 51:
# line 307 "parsedag.y"
{this_edge->setlabel(STRING,newstring(yypvt[-0].s));} break;
case 52:
# line 309 "parsedag.y"
{this_edge->setpointsize(yypvt[-0].i);} break;
case 53:
# line 311 "parsedag.y"
{this_edge->setcolor(newstring(yypvt[-0].s));} break;
case 54:
# line 313 "parsedag.y"
{this_edge->setink(yypvt[-0].i);} break;
case 56:
# line 318 "parsedag.y"
{set_union(Options.source_nodes,yypvt[-1].p);teardown(yypvt[-1].p);} break;
case 57:
# line 320 "parsedag.y"
{set_union(Options.sink_nodes,yypvt[-1].p);teardown(yypvt[-1].p);} break;
case 58:
# line 322 "parsedag.y"
{set_union(newset(),yypvt[-1].p);teardown(yypvt[-1].p);} break;
case 61:
# line 330 "parsedag.y"
{
						Options.nodesep = max(1,round(Resolution*atof(yypvt[-0].s)));
					} break;
case 62:
# line 334 "parsedag.y"
{
						Options.ranksep = max(MIN_RANK_SEP,round(Resolution*atof(yypvt[-1].s)));
						Options.rankadjust = 2;
					} break;
case 63:
# line 340 "parsedag.y"
{
						Options.ranksep = max(MIN_RANK_SEP,round(Resolution*atof(yypvt[-1].s)));
						Options.rankadjust = 1;
					} break;
case 64:
# line 345 "parsedag.y"
{
						Options.rankadjust = 1;
					} break;
case 65:
# line 349 "parsedag.y"
{
						Options.ranksep = max(MIN_RANK_SEP,round(Resolution*atof(yypvt[-0].s)));
					} break;
case 66:
# line 355 "parsedag.y"
{yyval.i = solid_ink;} break;
case 67:
# line 357 "parsedag.y"
{yyval.i = dashed_ink;} break;
case 68:
# line 359 "parsedag.y"
{yyval.i = dotted_ink;} break;
case 69:
# line 361 "parsedag.y"
{yyval.i = invis_ink;} break;
case 70:
# line 365 "parsedag.y"
{yyval.i = node_lookup(yypvt[-0].s);} break;
case 71:
# line 369 "parsedag.y"
{yyval.i = atoi(yypvt[-0].s);} break;
	}
	goto yystack;  /* stack new state and value */
}
