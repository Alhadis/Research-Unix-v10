
# line 3 "sty.y"
static	char	SCCSID[] = "@(#) sty.y:	1.1 83/08/02";
/*
	This program turns machine descriptions into a table.c file
*/

# line 53 "sty.y"
# include "ctype.h"
# include "mfile2.h"
# include "dope.h"

typedef struct {
	int	sha;	/* shape */
	int	ty;	/* type */
} SHTY;
SHTY	lshty,	/* left and right shape type */
	rshty;
static int	dcost = 0;	/* default cost */
static int	op,
		tyop,
		needs,
		rewrite,
		cost;
static int	lcount, rcount;
static int	opno;
static int	ophd[DSIZE];
static int	optl[DSIZE];
static int	opsh[DSIZE];

struct optb {		/* operation table */
	int	op;	/* what operation is to be matched */
	int	tyop;	/* the type associated with the op node */
	int	nxop;	/* index of the next op */
	int	line;	/* stin file line number */
	SHTY	l,	/* shapes of the left side */
		r;	/*		right side */
	int	oneeds,	/* needs */
		orew,	/* rewrite info */
		ocost;	/* op cost */
	char	*string;/* the output */
	int	strdef,	
		struse;
	int	olcount,/* usage count for left side */
		orcount;/* and right side */
};
typedef struct optb OPTB;

# ifndef NOPTB
# define NOPTB 400
# endif

OPTB optb[NOPTB];

# ifndef NSTRING
# define NSTRING 10000
# endif

# ifndef NSTYSHPS
# define NSTYSHPS 4000
# endif

typedef struct {
	int	sop,
		sleft,
		sright,
		ssh,
		scset,	/* ==1 after cost for shape has been set, else 0 */
		scost,
		scnt;
	char	shname[8];
} STYSHP;

static int	was_cost;	/* set if a cost was actually specified */
static int	nshp = 0;
static int	nmshp = NSTYSHPS - 1;
/*static*/ STYSHP	shp[NSTYSHPS];
static char	strng[NSTRING];
static char	*string;
static char	*pstring = strng;
static char	*asstring;


# line 129 "sty.y"
typedef union  {
	int	ival;
	char	*str;
	SHTY	shh;
} YYSTYPE;
# define STR 257
# define DEF 258
# define LPRF 259
# define RPRF 260
# define LSHR 261
# define RSHR 262
# define GOODCC 263
# define NOVAL 264
# define PNEED 265
# define LRES 266
# define RRES 267
# define LCOUNT 268
# define RCOUNT 269
# define USERCOST 270
# define CONVAL 271
# define NCONVAL 272
# define POSRANGE 273
# define SGRANGE 274
# define NONAME 275
# define DCOST 276
# define SHAPES 277
# define OPCODES 278
# define OP 279
# define NM 280
# define DIG 281
# define STYPE 282
# define RREG 283
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern short yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256

# line 387 "sty.y"

static int	lineno = 1;	/* line number of stin file */
static char	filename[100] = "<stdin>";

main(argc, argv)
int argc;
char **argv;
{
	register int i;
	for( i=0; i<DSIZE; ++i ) {
		opsh[i] = 0;
		optl[i] = ophd[i] = -1;
	}
	mkdope();
	yyparse();
	exit( 0 );
}


checkit( n )
{
	/* check a shape */
	STYSHP *p;
	if( n<0 ) return;
	if( n>=nshp && n<=nmshp || n>=NSTYSHPS ) {
		yyerror( "out of range shape: %d", n );
	}
	p = &shp[n];
	if( p->sop < 0 || p->sop > MANY ) {
		yyerror( "out of range op: %d", p->sop );
	}

	switch( optype(p->sop) ) {

	case BITYPE:
		if( p->sright < 0 && p->sop != MANY ) {
			yyerror( "right side empty" );
		}
		checkit( p->sright );
	case UTYPE:
		if( p->sleft < 0 ) {
			yyerror( "left side empty" );
		}
		checkit( p->sleft );
	}
}

	/* VARARGS */
yyerror( s, a )
char *s;
{
	fprintf( stderr, s, a );
	fprintf( stderr, ", file \"%s\", line %d\n", filename, lineno );
	abort();
	exit( 1 );
}


static int	otb[20];	/* ops that are part of the shape */
static int	notb;		/* number of otb entries */

loutput()
{
	/*
	 * rhs has a leaf: output templates for all interesting
	 * operators in this leaf. 
	 * An interesting operator is one that is NOT MANY.
	 * This allows us to access the table for each operator
	 * that this leaf may implement.
	 */
	register int	i;
	register int	s;
	register int	ocost = cost;

	notb = 0;
	lout1( s = rshty.sha );
	if( !notb ) yyerror( "lout1() error" );
	for( i = notb-1;  i >= 0;  --i )
	{
		op = otb[i];
		tyop = TANY;
		if( optype(op) == LTYPE ) {
			lcount = 0;
			cost = ocost + (rcount * refine(s));
		}
		else if( optype(op) == BITYPE ) {
			yyerror( "binary op in shape not done right" );
		}
		else {  /* op is unary */
			lcount = rcount;
			rcount = 0;
			cost = ocost + (lcount * urefine(s));
		}
		output();
	}
}

urefine( s ) {  /* construct an entry for a unary op matching shape s */
	/* for now, punt on the costs */
	rcount = lcount;
	lcount = 0;
	return( refine( s ) );
}

smat(s)
register s;
{
	/*
	 *	return 1 if we can find the op anyplace in the
	 *	shape tree `s'
	 */
	if( s < 0 )
		return( 0 );
	if( shp[s].sop == MANY )
	{
		if( shp[s].sleft>=0 && smat( shp[s].sleft ) ) return( 1 );
		if( shp[s].sright>=0 && smat( shp[s].sright ) ) return( 1 );
		return( 0 );
	}
	return( shp[s].sop == op );
}

refine(s)
register s;
{
	/*
	 * find the largest subshape of `s' that contains
	 * the operator `op' (op is global).
	 * We descend MANY nodes until we run out,
	 * or until the operator `op' is found in both descendents.
	 * Since we are throwing away costs buried in the MANY nodes
	 * on the way down, we must keep track of those, to
	 * adjust the costs appropriately in the operator table.
	 */
	register int bc = 0;

	while( shp[s].sop == MANY )
	{
		bc += shp[s].scost;
		if( smat( shp[s].sleft ) )
		{
			if( smat( shp[s].sright ) ) break;
			else s = shp[s].sleft;
		}
		else s = shp[s].sright;
	}
	lshty.sha = -1;
	rshty.sha = mkshp(s);
	return( bc + shp[s].scost );
}

lout1( n )
register n;
{
	register int i;
	while( n >= 0  &&  shp[n].sop == MANY )
	{
		lout1( shp[n].sleft );
		n = shp[n].sright;
	}
	if( n < 0 )
		return;
	for( i=0; i<notb; ++i )
	{
		if( otb[i] == shp[n].sop ) return;
	}
	otb[notb++] = shp[n].sop;
}

mkshp(s)
{
	/* make a shape that yields s */
	/* first, make s a MANY node */
	register i;

	if( s < 0 ) return( s );
	if( shp[s].sop == MANY ) i = s;
	else {
		/* look for a MANY node pointing to s */
		for( i= NSTYSHPS; i>nmshp; --i ) {
			if( shp[i].sright >= 0 ) continue;
			if( shp[i].sleft == s ) goto foundit;
		}
		/* must make a MANY node */
		i = bop( MANY, s, -1, 0 );
	}

	/* now, make sure that i has a name, so it will be output */

	foundit: ;

	if( !shp[i].shname[0] ) {
		strcpy( shp[i].shname, "mkshp" );
	}
	return(i);
}

onebit(x)
register x;
{
	/* return 1 if x has at most 1 bit on, 0 otherwise */
	return( !(x&(x-1)) );
}

output()
{
	register OPTB *q;
	register int j;

	if( lshty.ty == 0 ) lshty.ty = TANY;
	if( rshty.ty == 0 ) rshty.ty = TANY;

	switch( op )
	{

	case 0:
		yyerror( "0 op" );

	case STAR:
	case REG:
	case UNARY MINUS:
	case UNARY AND:
	case FLD:
		if( needs&(LSHARE|RSHARE) ) needs |= (LSHARE|RSHARE);
		break;

	case ASSIGN:
	case ASG PLUS:
	case ASG MINUS:
	case ASG MUL:
	case ASG DIV:
	case ASG MOD:
	case ASG AND:
	case ASG OR:
	case ASG ER:
	case ASG LS:
	case ASG RS:
		if( !(rewrite & (RNULL|RESC1|RESC2|RESC3|RRIGHT)) )
		{
			rewrite |= RLEFT;
		}
	}
	if( !rewrite ) rewrite = RNULL;

	if( !onebit( rewrite & (RNULL|RLEFT|RRIGHT|RESC1|RESC2|RESC3) ) )
	{
		yyerror( "multiple results -- illegal (%o)", rewrite );
	}
	if( ((rewrite&RLEFT)&&(needs&LSHARE)) ||
			((rewrite&RRIGHT)&&(needs&RSHARE)) )
	{
		if( asstring[0] != 'Y' )
			yyerror( "don't share on result side of tree" );
	}
	if( needs && (needs&(LSHARE|RSHARE)) == needs )
	{
		yyerror( "don't share without allocating something" );
	}
	checkout(asstring);

	q = &optb[opno];
	if( opno >= NOPTB ) yyerror( "too many templates" );
	q->line = lineno;
	q->op = op;
	q->tyop = tyop;
	if( ophd[op]>=0 )
	{
		optb[optl[op]].nxop = opno;
		optl[op] = opno;
	}
	else
	{
		optl[op] = ophd[op] = opno;
	}
	q->nxop = -1;
	q->l = lshty;
	q->r = rshty;
	q->oneeds = needs;
	q->orew = rewrite;
	q->ocost = cost;
	q->string = asstring;
	q->olcount = lcount;
	q->orcount = rcount;
	/* now, take care of special cases */
	if( optype(op) == LTYPE ) {  /* leaf */
		int s;
		if( (s=q->r.sha) >= 0 && trivial(s) ) {
			q->r.sha = -1;  /* clobber any right shape */
		}
	}
	else if( optype(op) == UTYPE ) {
		if( q->r.sha >=0 ) {
			/* someday, look for things below op */
			/* for now, we get the cost wrong so back up */
		}
	}
	++opno;
}

trivial( s ) {
	/* is shape s a trivial match for op */
	if( shp[s].sop == MANY ) {
		if( shp[s].sright >= 0 ) {
			return( 0 );  /* nontrivial */
		}
		s = shp[s].sleft;
	}
	if( shp[s].sop != op ) {
		return( 0 );
	}
	if( shp[s].ssh ) {
		return( 0 );
	}
	return( 1 );  /* ok to clobber */
}

checkout(string)
char *string;
{
	/* check out the string, looking at rewrite and needs */
	/* look for {U,I,C,A}{L,R,1,2,3} and \n */
	/* complain about:
	***	1, 2, 3 used, not allocated
	***	shared side after \n after temp used
	***	AL or AR used, w. side effect possible, more than once 
	*/

	/* flagl and flagr are 1 if L and R legal, 0 if not, -1 if
	/* they will be illegal after the next \n */

	register int flagl, flagr, prn, min, cond;
	register char *s;

	flagl = flagr = 1;
	cond = 0;

	for( s=string; *s; ++s )
	{
		switch( *s )
		{

		case '\\':
			++s;
			if( *s == '\\' ) ++s;
			else if( *s == 'n' )
			{
				if( flagl<0 ) flagl=0;
				if( flagr<0 ) flagr=0;
			}
			break;

		case 'Z':
			++s;
			if( *s=='(' )
			{
				while( *++s != ')' ) {;}
			}
			break;

		case 'Y':
			/* this string is asserted to be good; don't check */
			return;

		case 'R':
		case 'D':
			/* conditional; a lot of stuff no longer is true */
			cond = 1;
			break;

		case 'A':
		case 'C':
		case 'U':
		case 'I':
			++s;
			if( *s == '-' )
			{
				++s;
				min = 1;
			}
			else min = 0;
			if( *s == '(' )
			{
				++s;
				prn = 1;
			}
			else prn = 0;
			switch( *s )
			{

			case 'L':
				if( !flagl && !cond )
				{
					yyerror( "illegal L just at \"%s\"",
							s );
				}
				/* look for side-effects here */
				if( !min && seff(lshty.sha)) flagl = 0;
				break;

			case 'R':
				if( !flagr && !cond )
				{
					yyerror( "illegal R just at \"%s\"",
							s );
				}
				/* look for side-effects here */
				if( !min && seff(rshty.sha)) flagr = 0;
				break;

			case '1':
			case '2':
			case '3':
				if( (*s - '0') > (needs&NCOUNT) )
				{
					yyerror( "reg %c used, not allocated",
						*s );
				}
				if( (needs&LSHARE) && flagl ) flagl = -1;
				if( (needs&RSHARE) && flagr ) flagr = -1;

			case '.':
				break;

			default:
				yyerror( "illegal qualifier just at \"%s\"",
					s );
			}
			if( prn ) while( *s != ')' ) ++s;
		}
	}
}

seff( s )
register s;
{
	if( shp[s].sop == INCR || shp[s].sop == DECR ) return( 1 );
	if( shp[s].sleft >= 0 && seff( shp[s].sleft ) ) return( 1 );
	if( shp[s].sright >= 0 && seff( shp[s].sright ) ) return( 1 );
	return( 0 );
}

uop( o, a )
register o,a;
{
	if( o == MUL ) o = STAR;
	else if( o == MINUS ) o = UNARY MINUS;
	else if( o == AND ) o = UNARY AND;
	return( bop( o, a, -1, 0 ) );
}

top( a, ty )
register a,ty;
{
	/* build a type node over a */
	/* must be done differently than uop, since types must be copied */
	register STYSHP *p;

	if( a < 0 )
		return( a );
	checkit( a );
	if( shp[a].sop == MANY )
	{
		register l, r;
		l = shp[a].sleft;
		r = shp[a].sright;
		if( l >= 0 )
			l = top( l, ty );
		if( r >= 0 )
			r = top( r, ty );
		return( bop( MANY, l, r, 0 ) );
	}
	if( shp[a].ssh )
	{
		yyerror( "can't type a special node" );
	}
	shp[nshp] = shp[a];
	shp[nshp].shname[0] = '\0';
	shp[nshp].ssh = ty|SPTYPE;
	shp[nshp].scset = 0;
	nshp++;
	checkit( nshp-1 );
	return( nshp-1 );
}

bop( o, a, b, cst )
register o, a, b, cst;
{
	register STYSHP	*p;
	register int	l, r, ret;

	checkit( a );
	checkit( b );

	if( o != MANY )
	{
		while( shp[a].sop == MANY && shp[a].sright < 0 ) {
			a = shp[a].sleft;
		}
		while( b >= 0 && shp[b].sop == MANY && shp[b].sright < 0 ) {
			b = shp[b].sleft;
		}
		if( a>=0  &&  shp[a].sop == MANY )
		{
			/* distribute MANY nodes to top */
			l = bop( o, shp[a].sleft, b, cst + shp[a].scost );
			r = bop( o, shp[a].sright, b, cst + shp[a].scost );
			return( bop( MANY, l, r, 0 ) );
		}
		if( b>=0 && shp[b].sop == MANY )
		{
			/* distribute MANY nodes to top */
			l = bop( o, a, shp[b].sleft, cst + shp[b].scost );
			r = bop( o, a, shp[b].sright, cst + shp[b].scost );
			return( bop( MANY, l, r, 0 ) );
		}
	}

	if( o == MANY )
		/* MANY nodes go at the end */
		p = &shp[ ret = nmshp-- ];
	else
		p = &shp[ ret = nshp++ ];
	if( nshp >= nmshp ) {
		yyerror( "out of node space" );
		exit( 1 );
	}

	p->sop = o;
	p->sleft = a;
	p->sright =b;
	p->scost = cst;
	p->scset = 0;
	p->ssh = 0;
	p->shname[0] = '\0';
	checkit( ret );
	return( ret );
}

int olist[] = { PLUS, MINUS, MUL, DIV, MOD, LS, RS, OR, ER, AND, -1 };

struct nam
{
	char	*tntn;
	int	tnty;
};
#define NameV(x)	"x",	x	/* name-value pairs */
struct nam Tnam[] = {
	NameV(TANY),
	NameV(TCHAR),
	NameV(TSHORT),
	NameV(TINT),
	NameV(TLONG),
	NameV(TFLOAT),
	NameV(TDOUBLE),
	NameV(TUCHAR),
	NameV(TUSHORT),
	NameV(TUNSIGNED),
	NameV(TULONG),
	NameV(TSTRUCT),
	NameV(TPOINT),
	NameV(TPOINT2),
	NameV(TVOID),
	NameV(urTINT),
	NameV(urTUNSIGNED),
	0,		0,
};
struct nam Rwnam[] = {
	NameV(RLEFT),
	NameV(RRIGHT),
	NameV(RESC1),
	NameV(RESC2),
	NameV(RESC3),
	NameV(RESCC),
	NameV(RNOP),
	NameV(RNULL),
	0,	0
};
struct nam Ndnam[] = {
	NameV(LSHARE),
	NameV(RSHARE),
	NameV(NPAIR),
	NameV(LPREF),
	NameV(RPREF),
	0,	0,
};
finished()
{
	register STYSHP	*p;
	register OPTB	*q, *q1;
	register int	i, j;

	/* terminate the templates */
	printf( "# include \"mfile2.h\"\n\n" );

	/* chain binary ops together with the op= form */
	for( i=0; (op=olist[i])>=0; ++i )
	{
		if( ophd[op]<0 )
			ophd[op] = ophd[ASG op];
		else
			optb[optl[op]].nxop = ophd[ASG op];
	}
	if( ophd[STCALL]<0 )
		ophd[STCALL] = ophd[CALL];
	if( ophd[UNARY STCALL]<0 )
		ophd[UNARY STCALL] = ophd[UNARY CALL];

	optbck();

	/* everything that gets used should be a MANY shape with name */
	/* mkshp is called to cause this to be true */

	for( i=0; i<opno; ++i )
	{
		q = &optb[i];
		q->l.sha = mkshp(q->l.sha);
		q->r.sha = mkshp(q->r.sha);
	}

	/* avoid identical strings in optb[] by finding identical strings,
	giving them names and putting them at the front of table.c */
	for( i=0; i<opno; ++i )
	{
		q = &optb[i];
		q->struse = 1;
		q->strdef = -1;
		for(j = 0; j < i; j++)
		{
			q1 = &optb[j];
			if(!q1->struse)
				continue;
			if(strcmp(q->string,q1->string)==0)
			{
				q->strdef = j;
				q1->struse++;
				q->struse = 0;
				break;
			}
		}
	}
	for(i=0; i<opno; i++)
	{
		q = &optb[i];
		if(q->struse>1)
		{
			printf("static char Str%d[] = \"%s\";\n", i, q->string);
			q->struse = 0;
			q->strdef = i;
		}
	}

	/* set the ssh flags in shp, but don't print yet */

	for( j=0,i=NSTYSHPS-1; i>nmshp; --i ) {
		if( !shp[i].shname[0] ) continue;
		shp[i].ssh = j;
		j += manycount( i );
		++j;  /* count the null */
	}

	printf( "\n# define SHNL ((SHAPE *)0)\n" );
	printf( "# define S(x) (&shapes[x])\n" );

	printf( "\nSHAPE shapes[] = {\n" );
	for( i=0, p=shp; i<nshp; ++i,++p )
	{
		if( p->sop<0 )
			yyerror( "undefined shape: %.8s", p->shname );
		printf( "/*%4d */ ", i);
		printf( "%4d,\t",p->sop );
		saaddr(p->sleft);
		saaddr(p->sright);
		printf( "%#o,\t%d,", p->ssh, p->scost );
		if( p->shname[0] )
			printf( "\t/* %.8s */\n", p->shname );
		else
			putchar( '\n' );
		if(p->shname[0])
			chkdups(i);
	}
	printf( "\n};\n" );

	printf( "\n# define PSHNL ((SHAPE **)0)\n" );
	printf( "# define P(x) (&pshape[x])\n" );

	printf( "\nSHAPE *pshape[] = {\n" );
	for( j = 0, i = NSTYSHPS-1;  i > nmshp; --i ) {
		if( !shp[i].shname[0] )
			continue;
		if( shp[i].ssh != j ) {
			fprintf( stderr, "shape %d, ssh=%d, j=%d\n",
				i, shp[i].ssh, j );
		}
		printf( "/*%4d %.8s */\t", j, shp[i].shname  );
		j = manylist( i, j );
		printf( "SHNL,\n");
		++j;
	}
	printf( "};\n" );

	printf( "struct optab table[] = {\n\n" );
	for( i=0; i<opno; ++i )
	{
		q = &optb[i];
		printf( "/* # %d, line %d */\n", i, q->line );
		prop( q->op );
		prnam( q->tyop, Tnam );
		if( q->nxop >= 0 )
			printf( "\t&table[%d],\n", q->nxop );
		else
			printf( "\t0,\n" );

		shpprint(q->l.sha, q->l.ty);
		shpprint(q->r.sha, q->r.ty);
		printf("\t\t");
		prnam(q->oneeds, Ndnam);
		printf("\t\t");
		prnam(q->orew, Rwnam);
		if(q->strdef>=0)
			printf("\t\tStr%d,\n", q->strdef);
		else
			printf("\t\t\"%s\",\n", q->string);
		printf("\t\t%d,%d,%d,%d,\n", q->ocost, q->olcount, q->orcount,
			q->line );
	}
	printf( "\n};\n" );

	printf( "OPTAB *ophead[] = {\n" );

	for( i=0; i<DSIZE; ++i )
	{
		if( ophd[i] < 0 ) printf( "	0,\n" );
		else printf( "	&table[%d],\n", ophd[i] );
	}
	printf( "};\n" );
}

manycount( i )
{	/* count the descendents of shp[i] */
	int c;

	if( shp[i].sop == MANY ) {
		c = 0;
		if( shp[i].sleft >= 0 ) c += manycount( shp[i].sleft );
		if( shp[i].sright >= 0 ) c += manycount( shp[i].sright );
		return( c );
	}
	return( 1 );
}

manylist( i, j ) {
	/* put out a list of S(x) for shp[i], starting at j */
	/* first, put out CCODES and FREE */
	/* next, put out REG and TEMP */
	/* next, everything else */
	int k;
	int outcount = 0;

	if( i<0 ) return(j);

	setopsh( i );  /* set opsh[k] if op k is legal at head of i */
	if( opsh[FREE] ) {
		j = manyop( i, j, FREE, &outcount );
		opsh[FREE] = 0;
		}
	if( opsh[CCODES] ) {
		j = manyop( i, j, CCODES, &outcount );
		opsh[CCODES] = 0;
		}
	if( opsh[REG] ) {
		j = manyop( i, j, REG, &outcount );
		opsh[REG] = 0;
		}
	if( opsh[TEMP] ) {
		j = manyop( i, j, TEMP, &outcount );
		opsh[TEMP] = 0;
		}
	for( k=0; k<DSIZE; ++k ) {
		if( opsh[k] ) {
			j = manyop( i, j, k, &outcount );
			opsh[k] = 0;
			}
		}
	return( j );
	}

manyop( i, j, o, cp )
int	*cp;
{
	/* put out a list of S(x) for shapes matching op */

	if( i<0 ) return( j );
	if( shp[i].sop == MANY ) {
		j = manyop( shp[i].sleft, j, o, cp );
		return( manyop( shp[i].sright, j, o, cp ) );
		}
	if( shp[i].sop == o ) {
		printf( "S(%d), ", i );
		if( !((++*cp)&07) ) printf( "\n\t\t" );
		return( j+1 );
		}
	return( j );
	}

setopsh( i ) {
	/* set opsh[k] to 1 for every op appearing in shp[i] */
	int s;

	if( i<0 ) return;
	s = shp[i].sop;

	if( s == MANY ) {
		setopsh( shp[i].sleft );
		setopsh( shp[i].sright );
		}
	else opsh[s] = 1;
	}

saaddr(sp)
register sp;
{
	if( sp < 0 ) printf( "SHNL,\t" );
	else printf( "S(%d),\t", sp );
}


int nodcnt;
int treecnt;
chkdups(ii)
register ii;
{
	treecnt = nodcnt = 0;
	chkdup1(ii);
	if(treecnt!=nodcnt)
	{
		fprintf(stderr,"DUPLICATE SHAPES IN TREE %s\n",shp[ii].shname);
		prtree("T",ii);
	}
	clrcnt(ii);
}
chkdup1(nn)
register nn;
{
	register STYSHP *p;

	p = &shp[nn];
	if((p->sleft >= 0) && (p->sop==MANY)) chkdup1(p->sleft);
	if((p->sright >= 0) && (p->sop==MANY)) chkdup1(p->sright);
	p->scnt++;
	nodcnt++;
	treecnt += p->scnt;
}
clrcnt(nn)
register nn;
{
	register STYSHP *p;

	p = &shp[nn];
	if(p->sleft >= 0) clrcnt(p->sleft);
	if(p->sright >= 0) clrcnt(p->sright);
	p->scnt = 0;
}

prtree(str,ii)
register ii;
char *str;
{
	register STYSHP *p;
	p = &shp[ii];
	fprintf(stderr,"%s.%d	=	%d",str,ii,p->scnt);
	if(p->shname[0]) fprintf(stderr,"	/* %s */",p->shname);
	fprintf(stderr,"\n");
	if(p->sright>=0) prtree("R",p->sright);
	if(p->sleft>=0) prtree("L",p->sleft);
}
shpprint(sha, ty)
register sha,ty;
{
	if( sha < 0 ) printf( "\tPSHNL,\t");
	else printf( "\tP(%d),\t", shp[sha].ssh );
	prnam( ty, Tnam );
}

prnam(ty,src)
register struct nam *src;
register ty;
{
	register int ii,jj;
	register flag = 0;
	register struct nam *tn;

	if(!ty)
	{
		printf("0x0,\n");
		return;
	}
	for(tn = src; tn->tntn; tn++)
		if( tn->tnty == ty )
		{
			printf( "%s,\n", tn->tntn );
			return;
		}
	for(ii = 0; ; ii++)
	{
		if((jj = (01<<ii))>ty)
		{
			printf(",\n");
			return;
		}
		if(!(jj & ty))
			continue;
		for(tn = src; tn->tntn; tn++)
			if(tn->tnty==jj)
				break;
		if(flag++)
			printf("|");
		if(tn->tntn)
			printf("%s",tn->tntn);
		else
			printf("%#o",jj);
	}
}

static char	name[100];  /* had better be long enough */

yylex()
{
	register int c, i;

	for(;;)
	{

		c = getchar();
		if( c<0 ) return( -1 );
		switch( c )
		{

		case '\n':
			++lineno;
		case ' ':
		case '\b':
		case '\f':
		case '\v':
		case '\t':
			continue;

		case '<':
		case '>':
		case '+':
		case '-':
		case '=':
		case '*':
		case '%':
		case '/':
		case '&':
		case '|':
		case '^':
		case '!':
			name[0] = c;
			name[1] = getchar();
			name[2] = '\0';
			if( oplook() )
			{
				if( yylval.ival == LS || yylval.ival == RS )
				{
					if( (c=getchar()) == '=' )
						yylval.ival = ASG yylval.ival;
					else ungetc( c, stdin );
				}
				return( OP );
			}
			ungetc( name[1], stdin );
			name[1] = '\0';
			if( oplook() ) return( OP );
			yyerror( "cannot deal with %c", name[0] );
			return( OP );

		case '~':
			yylval.ival = COMPL;
			return( OP );

		case '"':
			string = pstring;
			while( pstring < &strng[NSTRING-2] )
			{
				switch( c = getchar() ) {
				case '\t':
					*pstring++ = '\\';
					*pstring++ = 't';
					break;
				case '\n':
					*pstring++ = '\\';
					*pstring++ = 'n';
					++lineno;
					break;
				case '\\':	/* escape */
					*pstring++ = '\\';
					c = getchar();
					if( c == '\n' )
						++lineno;
					else if( c<0 )
						yyerror( "missing \"" );
					else if( isalpha(c) && isupper(c) )
						*pstring++ = '\\';
					*pstring++ = c;
					break;
				case '"':
					*pstring++ = '\0';
					goto eos;
				default:
					if( c<0 )
						yyerror( "missing \"" );
					*pstring++ = c;
				}
			}
		   eos:
			if( pstring >= &strng[NSTRING] )
				yyerror( "s_table overflow" );
			yylval.str = string;
			return( STR );

		case '\'':
			for( i = 0;  i < sizeof name - 1;  ++i )
			{
				c = getchar();
				if( c == '\'' ) break;
				if( c == '\n' ) yyerror( "missing '" );
				name[i] = c;
			}
			name[i] = '\0';
			if( oplook() ) return( OP );
			yyerror( "bad op name: '%s'", name );

		case '[':
			for( i = 0;  i < sizeof name - 1;  ++i )
			{
				c = getchar();
				if( c == ']' ) break;
				if( c == '\n' ) yyerror( "missing ]" );
				name[i] = c;
			}
			name[i] = '\0';
			yylval.ival = tystr(name);
			return( STYPE );

		case '#':  /* comment */
			while( (c = getchar()) != '\n' )
			{
				if( c < 0 ) yyerror( "unexpected EOF" );
			}
			++lineno;
			continue;

		case '$':
			c = getchar();
			if( isdigit(c) )
			{
				yylval.ival = c-'0';
				return( RREG );
			}
			switch( c )
			{
			case '[':	return( LPRF );
			case ']':	return( RPRF );
			case '<':	return( LSHR );
			case '>':	return( RSHR );
			case 'L':	return( LRES );
			case 'R':	return( RRES );
			case 'P':	return( PNEED );
			case 'C':	return( GOODCC );
			case 'N':	return( NOVAL );
			case 'r':	return( RCOUNT );
			case 'l':	return( LCOUNT );
			case 'A':	yylval.ival = NRGS;
					return( DIG );
			}
			yyerror( "$%c illegal", c );

		default:
			if( isdigit(c) )
			{
				yylval.ival = c-'0';
				return( DIG );
			}
			if( isalpha(c) )
			{
				/* collect the name */
				int i = 1;
				name[0] = c;
				while( isalpha( (c=getchar()) ) || isdigit(c) )
				{
					name[i++] = c;
				}
				name[i] = '\0';
				ungetc( c, stdin );
				return( lookup() );
			}
			return( c );
		}
	}
}

struct nlist {
	char *shop;
	int vop;
} ot[] = {

	"++",	INCR,
	"+",	PLUS,
	"--",	DECR,
	"-",	MINUS,
	"*",	MUL,
	"%",	MOD,
	"/",	DIV,
	"&",	AND,
	"^",	ER,
	"!=",	NE,
	"==",	EQ,
	"UMINUS",	UNARY MINUS,
	"UAND",	UNARY AND,
	"STAR",	STAR,
	"+=",	ASG PLUS,
	"-=",	ASG MINUS,
	"*=",	ASG MUL,
	"/=",	ASG DIV,
	"%=",	ASG MOD,
	"&=",	ASG AND,
	"|=",	ASG OR,
	"|",	OR,
	"^=",	ASG ER,
	"=",	ASSIGN,
	"<",	LT,
	"<=",	LE,
	"<<",	LS,
	">",	GT,
	">=",	GE,
	">>",	RS,
	"FLD",	FLD,
	"CMP",	CMP,
	"COMOP",	COMOP,
	"CM",		CM,
	"GENLAB",	GENLAB,
	"GENUBR",	GENUBR,
	"GENBR",	GENBR,
	"ARG",		FUNARG,
	"STARG",	STARG,
	"STASG",	STASG,
	"INIT",		INIT,
	"GOTO",		GOTO,
	"CALL",		CALL,
	"UCALL",	UNARY CALL,
	"STCALL",	STCALL,
	"USTCALL",	UNARY STCALL,
	"CONV",		CONV,
	"PDIV",		PDIV,
	"PMUL",		PMUL,
	"REG",	REG,
	"CON",	ICON,
	"TEMP",	TEMP,
	"AUTO",	VAUTO,
	"PARAM",	VPARAM,
	"NAME",	NAME,
	"RNODE",	RNODE,
	"SNODE",	SNODE,
	"QNODE",	QNODE,
	"CC",	CCODES,
	"FREE",	FREE,
	"UOP0",	UOP0,
	"UOP1",	UOP1,
	"UOP2",	UOP2,
	"UOP3",	UOP3,
	"UOP4",	UOP4,
	"UOP5",	UOP5,
	"UOP6",	UOP6,
	"UOP7",	UOP7,
	"UOP8",	UOP8,
	"UOP9",	UOP9,
	"",	-1 };

oplook()
{
	/* look up the first n chars of name in the above table */
	/* return 1 if it is an op, after setting yylval */
	register int i;
	for( i=0; ot[i].vop >= 0; ++i )
	{
		if( !strcmp( name, ot[i].shop ) )
		{
			yylval.ival = ot[i].vop;
			return( 1 );
		}
	}
	return( 0 );
}

tystr( p )
register char *p;	/* pointer to name */
{
	register i;

	/* lookup the types in name */
	if( !*p )
		return( TANY );
	else
		i = 0;
	for(;;)
	{
		switch( *p++ )
		{
		case '\0':
			return( i );

		case 'c':
			i |= TCHAR;
		case ' ':
		case ',':
			continue;

		case 's':
			i |= TSHORT;
			continue;

		case 'i':
			i |= TINT;
			continue;

		case 'l':
			i |= TLONG;
			continue;

		case 'f':
			i |= TFLOAT;
			continue;

		case 'd':
			i |= TDOUBLE;
			continue;

		case 'P':
			i |= TPOINT2;
			continue;

		case 'p':
			i |= TPOINT;
			continue;

		case 't':
			i |= TSTRUCT;
			continue;

		case 'v':
			/* later..
			i |= TVOID;
			*/
			continue;

		case 'u':
			switch( *p )
			{
			case 'i':	i |= TUNSIGNED;  break;
			case 's':	i |= TUSHORT;  break;
			case 'c':	i |= TUCHAR;  break;
			case 'l':	i |= TULONG;  break;
			default:	yyerror( "bad u%c type", *p );
			}
			++p;
			continue;

		default:
			yyerror( "illegal type: %c", p[-1] );
		}
	}
}

struct nlist resw[] = {
	NameV(DCOST),
	NameV(SHAPES),
	NameV(OPCODES),
	NameV(USERCOST),
	NameV(CONVAL),
	NameV(NCONVAL),
	NameV(POSRANGE),
	NameV(SGRANGE),
	NameV(NONAME),
	"",	-1,
};

lookup()
{
	/* look up the shape name in name, and return the index */
	register STYSHP *p;
	register int i;
	for( i=0; resw[i].vop >= 0; ++i )
	{
		if( !strcmp( name, resw[i].shop ) ) return( resw[i].vop );
	}
	for( i=NSTYSHPS-1, p= &shp[NSTYSHPS-1]; i>nmshp; --i,--p )
	{
		if( !strncmp( name, p->shname, 8 ) )
		{
			 /* match */
			yylval.ival = i;
			return( NM );
		}
	}
	/* new entry */
	strncpy( p->shname, name, 8 );
	p->scset = p->ssh = p->scost = 0;
	p->sleft = p->sright = -1;
	p->sop = -1;
	yylval.ival = nmshp--;
	if( nmshp <= nshp ) {
		yyerror( "out of node space" );
		exit( 1 );
	}
	return( NM );
}

setcost ( sn, cst )
register int sn,cst;
{
	/* set costs for shape at all reference points.
	/* reference points all are just below MANY ops
	*/
	if ( sn < 0 )
		return;
	if ( shp[sn].sop == MANY )
	{
		setcost( shp[sn].sleft, cst );
		setcost( shp[sn].sright, cst );
	}
	else if ( was_cost )
	{
		if ( shp[sn].scset )
		{
			if ( shp[sn].scost == cst )
				return;
			yyerror( "Inconsistent cost values for shared shape" );
		}
		else
		{
			shp[sn].scost = cst;
			shp[sn].scset = 1;
		}
	}
	else
	{
		shp[sn].scset = 1;
	}
}

sharecost ( sn, cst )
register int sn,cst;
{
	if ( sn == nshp - 1 )	/* quick opt. there is only one choice */
	{
		shp[sn].scost = cst;
		shp[sn].scset = 1;
		return(sn);
	}
/*	if ( cst == 0 )
/*		return( sn );
*/

	setcost(sn, cst);
	return (sn);
/*	if (nshp >= NSTYSHPS)
/*		yyerror("too many shapes");
/*	shp[nshp] = shp[sn];
/*	shp[nshp].scost += cst;
/*	nshp++;
/*	checkit( nshp-1 );
/*	return( nshp-1 );
*/
}
	
uopcost (o, a)
register int o,a;
{
	switch(o)
	{
	default:
	case MUL:
	case AND:	yyerror("Illegal unary cost operator");
			return(0);
	case MINUS:
		return(-a);
	}
}

bopcost (o, a, b)
register int o,a,b;
{
	switch(o)
	{
	case INCR:
	case PLUS:	return(a+b);
	case DECR:
	case MINUS:	return(a-b);
	case MUL:	return(a*b);
	case DIV:	return(a/b);
	case MOD:	return(a%b);
	case LS:	return(a>>b);
	case RS:	return(a<<b);
	case OR:	return(a|b);
	case ER:	return(a^b);
	case AND:	return(a&b);
	case LT:	return(a<b);
	case GT:	return(a>b);
	case LE:	return(a<=b);
	case GE:	return(a>=b);
	case NE:	return(a!=b);
	case EQ:	return(a==b);
	default:	yyerror("Illegal binary operator");
			return(0);
	}
}

/*	print operator name
*/
static
prop( op )
register int	op;
{
	register struct nam	*np;
	static struct nam	Opnam[] = {
		NameV(INCR),
		NameV(PLUS),
		NameV(DECR),
		NameV(MINUS),
		NameV(MUL),
		NameV(MOD),
		NameV(DIV),
		NameV(AND),
		NameV(ER),
		NameV(NE),
		NameV(EQ),
		NameV(UNARY MINUS),
		NameV(UNARY AND),
		NameV(STAR),
		NameV(ASG PLUS),
		NameV(ASG MINUS),
		NameV(ASG MUL),
		NameV(ASG DIV),
		NameV(ASG MOD),
		NameV(ASG AND),
		NameV(ASG OR),
		NameV(OR),
		NameV(ASG ER),
		NameV(ASSIGN),
		NameV(LT),
		NameV(LE),
		NameV(LS),
		NameV(GT),
		NameV(GE),
		NameV(RS),
		NameV(FLD),
		NameV(CMP),
		NameV(COMOP),
		NameV(CM),
		NameV(GENLAB),
		NameV(GENUBR),
		NameV(GENBR),
		NameV(FUNARG),
		NameV(STARG),
		NameV(STASG),
		NameV(INIT),
		NameV(GOTO),
		NameV(CALL),
		NameV(UNARY CALL),
		NameV(STCALL),
		NameV(UNARY STCALL),
		NameV(CONV),
		NameV(PDIV),
		NameV(PMUL),
		NameV(REG),
		NameV(ICON),
		NameV(TEMP),
		NameV(VAUTO),
		NameV(VPARAM),
		NameV(NAME),
		NameV(RNODE),
		NameV(SNODE),
		NameV(QNODE),
		NameV(CCODES),
		NameV(FREE),
		0,	0,
	};
	for( np = Opnam;  np->tnty;  np++ )
	{
		if( op == np->tnty )
		{
			printf( "%s,", np->tntn );
			return;
		}
	}
	printf( "%d,", op );
}


/*
	check the op table for duplicate or partially duplicate templates
*/
static
optbck()
{
#define	EqOptb(a)	(q->a == q1->a)
	register int	i;
	register int	j;
	register OPTB	*q;
	register OPTB	*q1;

	for( i = 0;  i < opno;  i++ )
	{
		q = &optb[i];
		for( j = 0;  j < i;  j++ )
		{
			q1 = &optb[j];
			if(
				EqOptb(op)  &&
				EqOptb(l.ty)  &&
				EqOptb(r.ty)  &&
				EqOptb(orew)  &&
				EqOptb(oneeds)  &&
				shapeshr(q->l.sha, q1->l.sha)  &&
				shapeshr(q->r.sha, q1->r.sha)
			  )
			{
				fprintf( stderr,
					"line %d may be covered by line %d\n",
					q->line, q1->line );
				break;
			}
		}
	}
}


/*	do the shape trees starting at s1 and s2 share any elements?
*/
static
shapeshr( s1, s2 )
register int	s1;	/* indexes into shape table */
register int	s2;
{
	register int	i1;

	for( i1 = s1;  shp[i1].sleft >= 0;  i1 = shp[i1].sleft )
	{
		if( i1 == s2  ||  shp[i1].sright == s2 )
			return  1;
	}
	return  0;
}
short yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
-1, 27,
	282, 61,
	-2, 52,
	};
# define YYNPROD 62
# define YYLAST 255
short yyact[]={

  70,  13,  43,  63,  44,  44,  63,  27,  87,  13,
  55,  13,  88,  13,  13,   4,   2,   5,  34,  35,
  36,  37,  38,  33,  48,  67,  45,  25,  23,  61,
  60,  10,  39,  30,  93,  86,  85,  84,  31,  95,
  51,  21,  41,   7,  12,  49,  18,  64,  16,  24,
  46,  26,  20,   8,  47,   6,   3,   1,  20,  50,
  28,  40,  32,  17,  42,  53,  15,   0,   0,   0,
   0,   0,   0,   0,  42,  20,  68,  66,   0,  52,
   0,  69,  54,  56,  57,  58,  59,   0,  65,   0,
   0,   0,  89,   0,  90,  91,   0,   0,   0,  92,
   0,   0,   0,   0,  25,  25,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,  94,   0,
   0,  96,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,  73,  72,  75,  74,  77,  78,
  76,  79,  80,  82,  83,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,  71,   0,  81,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,  14,  62,  22,  55,  11,   0,  55,   9,  14,
  29,  14,  19,  14,  14 };
short yypact[]={

-261,-1000,-1000,-263,-1000, -15, -31, -27,-1000, -39,
 -96,-1000,-275, -29,-1000, -11, -21,-252, -16, -26,
-277, -18, -26,-1000,-233,-1000, -16,-1000,  -1, -26,
 -29,-1000, -16,-1000,-271,-271,-271,-271,-271,-1000,
-1000, -37,-278, -26,-1000, -26,-1000, -19, -16,-125,
 -22,-1000, -23, -24,-273,-1000,-273,-273,-273,-273,
-1000,-267, -34, -37,-273,-278, -96, -26,-1000, -25,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000, -34,-1000,
  -2,-1000, -96,-1000,-1000,-1000,-1000 };
short yypgo[]={

   0,  32,  47,  66,  44,  63,  62,  48,  46,  61,
  30,  29,  31,  57,  56,  55,  53,  28,  51,  49,
  45 };
short yyr1[]={

   0,  13,  14,  14,  14,  14,   5,   5,  15,  15,
  16,  16,  16,  16,  16,  18,  16,  17,  19,  19,
  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,
  20,  20,  20,  20,   2,   2,   1,   1,   9,  10,
  10,  10,  11,  11,   6,   6,   6,   6,   6,   6,
   6,  12,  12,   3,   3,   7,   8,   8,   8,   4,
   4,   4 };
short yyr2[]={

   0,   4,   0,   7,   5,   7,   0,   1,   0,   2,
   5,   6,   3,   4,   2,   0,   4,   4,   0,   3,
   0,   2,   2,   2,   2,   2,   2,   2,   2,   2,
   2,   2,   2,   2,   1,   2,   1,   0,   2,   1,
   2,   3,   3,   1,   0,   1,   2,   2,   2,   2,
   2,   1,   2,   1,   3,   2,   1,   2,   3,   3,
   1,   2 };
short yychk[]={

-1000, -13, 277, -14, 278, 280, -15,  58, -16, 279,
 -12, 276,  -4,  40, 280,  -3,  -7,  -5,  -8, 279,
  -4, -12, 282, -17, -19, 123, -18, 282,  -8, 279,
  44,  59,  -6, 275, 270, 271, 272, 273, 274,  -1,
  -9,  58,  -4, 279, 282,  44, -17, -12, 257, -20,
  -1,  41,  -7,  -1,  -2, 281,  -2,  -2,  -2,  -2,
 -10, -11, 279,  40,  -2,  -4, -12,  44, -17,  -1,
 125, 281, 260, 259, 262, 261, 265, 263, 264, 266,
 267, 283, 268, 269,  59,  59,  59, 281, 279, -11,
 -10, -17, -12,  59, -11,  41, -17 };
short yydef[]={

   0,  -2,   2,   0,   8,   0,   1,   6,   9,   0,
  18,  15,  51,   0,  60,   0,  53,  44,  37,   7,
  56,  18,   0,  14,   0,  20,  37,  -2,   0,   0,
   0,   4,  37,  45,   0,   0,   0,   0,   0,  55,
  36,   0,  57,   0,  61,   0,  12,  18,  37,   0,
   0,  59,  54,   0,  46,  34,  47,  48,  49,  50,
  38,  39,   0,   0,  43,  58,  18,   0,  13,   0,
  19,  21,  22,  23,  24,  25,  26,  27,  28,  29,
  30,  31,  32,  33,  16,   3,   5,  35,   0,  40,
   0,  10,  18,  17,  41,  42,  11 };
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
# line 158 "sty.y"
{	finished(); } break;
case 3:
# line 163 "sty.y"
{	shp[yypvt[-5].ival].sop = MANY;
			shp[yypvt[-5].ival].sleft = yypvt[-3].ival;
			shp[yypvt[-5].ival].sright = yypvt[-1].ival;
			shp[yypvt[-5].ival].scost = 0;
			shp[yypvt[-5].ival].scset = 1;
			shp[yypvt[-5].ival].ssh = 0;
		} break;
case 4:
# line 171 "sty.y"
{	
			shp[yypvt[-3].ival].sop = MANY;
			shp[yypvt[-3].ival].sleft = yypvt[-1].ival;
			shp[yypvt[-3].ival].sright = -1;
			shp[yypvt[-3].ival].scost = 0;
			shp[yypvt[-3].ival].scset = 1;
			shp[yypvt[-3].ival].ssh = 0;
		} break;
case 5:
# line 180 "sty.y"
{	shp[nshp].scost = yypvt[-1].ival;
			shp[nshp].scset = was_cost;
			shp[nshp].ssh = yypvt[-2].ival;
			shp[nshp].sop = yypvt[-3].ival;
			shp[nshp].sleft = -1;
			shp[nshp].sright = -1;
			++nshp;
			checkit( nshp-1 );
			shp[yypvt[-5].ival].sop = MANY;
			shp[yypvt[-5].ival].sleft = nshp-1;
			shp[yypvt[-5].ival].sright = -1;
			shp[yypvt[-5].ival].scost = 0;
			shp[yypvt[-5].ival].scset = 1;
			shp[yypvt[-5].ival].ssh = 0;
			if( nshp >= nmshp ) {
				yyerror( "out of node space" );
				exit( 1 );
			}
		} break;
case 6:
# line 202 "sty.y"
{	yyval.ival = ICON; } break;
case 8:
# line 207 "sty.y"
{	needs = op = rewrite = cost = 0;
			lcount = rcount = 1;
		} break;
case 10:
# line 215 "sty.y"
{	lshty = yypvt[-3].shh;
			rshty = yypvt[-1].shh;
			op = yypvt[-4].ival;
			tyop = TANY;
			output();
			tyop = needs = op = rewrite = cost = 0;
			lcount = rcount = 1;
		} break;
case 11:
# line 224 "sty.y"
{	lshty = yypvt[-3].shh;
			rshty = yypvt[-1].shh;
			op = yypvt[-5].ival;
			tyop = yypvt[-4].ival;
			output();
			tyop = needs = op = rewrite = cost = 0;
			lcount = rcount = 1;
		} break;
case 12:
# line 233 "sty.y"
{	lshty = yypvt[-1].shh;
			rshty.sha = -1;
			rshty.ty = TANY;
			op = yypvt[-2].ival;
			tyop = TANY;
			output();
			tyop = needs = op = rewrite = cost = 0;
			lcount = rcount = 1;
		} break;
case 13:
# line 243 "sty.y"
{	lshty = yypvt[-1].shh;
			rshty.sha = -1;
			rshty.ty = yypvt[-2].ival;
			op = yypvt[-3].ival;
			tyop = TANY;
			output();
			tyop = needs = op = rewrite = cost = 0;
			lcount = rcount = 1;
		} break;
case 14:
# line 253 "sty.y"
{	rshty = yypvt[-1].shh;
			lshty.sha = -1;
			lshty.ty = TANY;
			loutput();
			tyop = needs = op = rewrite = cost = 0;
			lcount = rcount = 1;
			} break;
case 15:
# line 260 "sty.y"
{dcost=0;} break;
case 16:
# line 261 "sty.y"
{	dcost = yypvt[-1].ival; } break;
case 17:
# line 265 "sty.y"
{	
			cost = yypvt[-1].ival;
			asstring = yypvt[-2].str;
			} break;
case 18:
# line 272 "sty.y"
{	needs = 0; } break;
case 20:
# line 277 "sty.y"
{	needs = 0; } break;
case 21:
# line 279 "sty.y"
{	needs = (needs&~NCOUNT) | yypvt[-0].ival; } break;
case 22:
# line 281 "sty.y"
{	needs |= RSHARE|RPREF; } break;
case 23:
# line 283 "sty.y"
{	needs |= LSHARE|LPREF; } break;
case 24:
# line 285 "sty.y"
{	needs |= RSHARE; } break;
case 25:
# line 287 "sty.y"
{	needs |= LSHARE; } break;
case 26:
# line 289 "sty.y"
{	needs |= NPAIR; } break;
case 27:
# line 291 "sty.y"
{	rewrite |= RESCC; } break;
case 28:
# line 293 "sty.y"
{
			if (rewrite)
				yyerror("$N incompatible with other results");
			rewrite = RNULL;
		} break;
case 29:
# line 299 "sty.y"
{	rewrite |= RLEFT; } break;
case 30:
# line 301 "sty.y"
{	rewrite |= RRIGHT; } break;
case 31:
# line 303 "sty.y"
{	if( !(needs&NCOUNT) ) needs |= yypvt[-0].ival;
			rewrite |= ((yypvt[-0].ival==1)?RESC1:((yypvt[-0].ival==2)?RESC2:RESC3));
			} break;
case 32:
# line 307 "sty.y"
{	lcount += 1; } break;
case 33:
# line 309 "sty.y"
{	rcount += 1; } break;
case 35:
# line 314 "sty.y"
{	yyval.ival = 10*yypvt[-1].ival + yypvt[-0].ival; } break;
case 36:
# line 318 "sty.y"
{	was_cost = 1; } break;
case 37:
# line 320 "sty.y"
{	was_cost = 0; yyval.ival = dcost; } break;
case 38:
# line 324 "sty.y"
{	yyval.ival = yypvt[-0].ival; } break;
case 40:
# line 329 "sty.y"
{	yyval.ival = uopcost( yypvt[-1].ival, yypvt[-0].ival ); } break;
case 41:
# line 331 "sty.y"
{	yyval.ival = bopcost( yypvt[-1].ival, yypvt[-2].ival, yypvt[-0].ival ); } break;
case 42:
# line 335 "sty.y"
{	yyval.ival = yypvt[-1].ival; } break;
case 44:
# line 340 "sty.y"
{	yyval.ival = 0; } break;
case 45:
# line 342 "sty.y"
{	yyval.ival = NACON;	/* constant with no name */ } break;
case 46:
# line 344 "sty.y"
{	yyval.ival = yypvt[-0].ival|SUSER;  /* user's cost ftn */ } break;
case 47:
# line 346 "sty.y"
{	yyval.ival = yypvt[-0].ival|SVAL;  /* positive constant value */ } break;
case 48:
# line 348 "sty.y"
{	yyval.ival = yypvt[-0].ival|SNVAL;  /* negative constant value */ } break;
case 49:
# line 350 "sty.y"
{	yyval.ival = yypvt[-0].ival|SRANGE0;  /* positive range */ } break;
case 50:
# line 352 "sty.y"
{	yyval.ival = yypvt[-0].ival|SSRANGE;  /* signed range */ } break;
case 51:
# line 356 "sty.y"
{	yyval.shh.ty = 0;
			yyval.shh.sha = yypvt[-0].ival;
			} break;
case 52:
# line 360 "sty.y"
{	yyval.shh.sha = yypvt[-1].ival;
			yyval.shh.ty = yypvt[-0].ival; } break;
case 54:
# line 366 "sty.y"
{	yyval.ival = bop( MANY, yypvt[-2].ival, yypvt[-0].ival, 0 ); } break;
case 55:
# line 370 "sty.y"
{	yyval.ival = sharecost(yypvt[-1].ival, yypvt[-0].ival); } break;
case 57:
# line 375 "sty.y"
{	yyval.ival = uop( yypvt[-1].ival, yypvt[-0].ival ); } break;
case 58:
# line 377 "sty.y"
{	yyval.ival = bop( yypvt[-1].ival, yypvt[-2].ival, yypvt[-0].ival, 0 ); } break;
case 59:
# line 381 "sty.y"
{	yyval.ival = yypvt[-1].ival; } break;
case 61:
# line 384 "sty.y"
{	yyval.ival = top( yypvt[-1].ival, yypvt[-0].ival );  } break;
	}
	goto yystack;  /* stack new state and value */
}
