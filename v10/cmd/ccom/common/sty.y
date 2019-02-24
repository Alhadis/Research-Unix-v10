/* @(#) sty.y: 1.1 12/22/83				*/
%{
static	char	SCCSID[] = "@(#) sty.y:	1.1 83/08/02";
/*
	This program turns machine descriptions into a table.c file
*/
%}
/*	The input language uses many C conventions and notations */
/*	Types are represented as a shorthand:
	c	char
	i	int
	s	short
	l	long
	p	pointer
	P	alternate form of pointer
	t	structure
	v	void
	ux	unsigned x
	f	float
	d	double
/*	There are a number of builtin cost names:
	NONAME		a constant with no name field (not an address)
	CONVAL n	the constant n
	NCONVAL n	the constant -n
	POSRANGE n	constants in the range [0,2**n -1]
	SGRANGE n	constants in the range [-2**n, 2**n - 1]
	*/
/*	There are also several incedental needs, etc, specified
	1,2,3	Number of needed registers
	$P	need pairs
	$<	share left
	$>	share right
	$L	result left
	$R	result right
	$1,2,3	result in reg 1, 2, 3
	$C	operation produces correct condition codes
	$N	no value produced: side effects only
	$A	need them all
	$[	share left, RHS is preferred (if a temp register)
	$]	share right, RHS is preferred (if a temp register)
	$l	left side is referenced once more
	$r	right side is referenced once more
	*/

/* NOTE:
	When several templates match with equal cost, the first is chosen.
	Thus, when side-effects only are desired, the first template matching
	is taken, if it is of lowest cost.
	Moral:  $N templates should be cheaper or should preceed $L, $1, etc.
	*/

%{
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

%}

%union {
	int	ival;
	char	*str;
	SHTY	shh;
};

%token STR DEF LPRF RPRF LSHR RSHR GOODCC NOVAL PNEED LRES RRES LCOUNT RCOUNT
%token USERCOST CONVAL NCONVAL POSRANGE SGRANGE NONAME DCOST SHAPES OPCODES
%token <ival> OP NM DIG STYPE RREG
%left OP
%left STYPE

%type <str> STR
%type <ival> opcost num slist nterm opop shape
%type <ival> costnexp nexp
%type <ival> cost cexpr cterm
%type <shh> sref

	/*
	OP	the ops <, <<, >, >>, <=, >=, +, -, *, /, %, &, ^, ~, !
	NM	names (letter, followed by 0 or more letters or digits
	STR	a string in ""
	DIG	a digit, 0-9
	Other letters are returned: (, ), {, }, etc.
	*/

%%

file	:	SHAPES lshapes OPCODES lops
		{	finished(); }
	;

lshapes	:	/* EMPTY */
	|	lshapes NM ':' slist ',' costnexp ';'
		{	shp[$2].sop = MANY;
			shp[$2].sleft = $4;
			shp[$2].sright = $6;
			shp[$2].scost = 0;
			shp[$2].scset = 1;
			shp[$2].ssh = 0;
		}
	|	lshapes NM ':' costnexp ';'
		{	
			shp[$2].sop = MANY;
			shp[$2].sleft = $4;
			shp[$2].sright = -1;
			shp[$2].scost = 0;
			shp[$2].scset = 1;
			shp[$2].ssh = 0;
		}
	|	lshapes NM ':' opop shape opcost ';'
		{	shp[nshp].scost = $6;
			shp[nshp].scset = was_cost;
			shp[nshp].ssh = $5;
			shp[nshp].sop = $4;
			shp[nshp].sleft = -1;
			shp[nshp].sright = -1;
			++nshp;
			checkit( nshp-1 );
			shp[$2].sop = MANY;
			shp[$2].sleft = nshp-1;
			shp[$2].sright = -1;
			shp[$2].scost = 0;
			shp[$2].scset = 1;
			shp[$2].ssh = 0;
			if( nshp >= nmshp ) {
				yyerror( "out of node space" );
				exit( 1 );
			}
		}
	;

opop	:	/* EMPTY */
		{	$$ = ICON; }
	|	OP
	;

lops	:	/* EMPTY */
		{	needs = op = rewrite = cost = 0;
			lcount = rcount = 1;
		}

	|	lops lop
	;

lop	:	OP sref ',' sref ltail
		{	lshty = $2;
			rshty = $4;
			op = $1;
			tyop = TANY;
			output();
			tyop = needs = op = rewrite = cost = 0;
			lcount = rcount = 1;
		}
	|	OP STYPE sref ',' sref ltail
		{	lshty = $3;
			rshty = $5;
			op = $1;
			tyop = $2;
			output();
			tyop = needs = op = rewrite = cost = 0;
			lcount = rcount = 1;
		}
	|	OP sref ltail
		{	lshty = $2;
			rshty.sha = -1;
			rshty.ty = TANY;
			op = $1;
			tyop = TANY;
			output();
			tyop = needs = op = rewrite = cost = 0;
			lcount = rcount = 1;
		}
	|	OP STYPE sref ltail
		{	lshty = $3;
			rshty.sha = -1;
			rshty.ty = $2;
			op = $1;
			tyop = TANY;
			output();
			tyop = needs = op = rewrite = cost = 0;
			lcount = rcount = 1;
		}
	|	sref ltail
		{	rshty = $1;
			lshty.sha = -1;
			lshty.ty = TANY;
			loutput();
			tyop = needs = op = rewrite = cost = 0;
			lcount = rcount = 1;
			}
	|	DCOST {dcost=0;} opcost ';'
			{	dcost = $3; }
	;

ltail	:	needs STR opcost ';'
		{	
			cost = $3;
			asstring = $2;
			}
	;

needs	:	/* EMPTY */
		{	needs = 0; }
	|	'{' nlist '}'
	;

nlist	:	/* EMPTY */
		{	needs = 0; }
	|	nlist DIG
		{	needs = (needs&~NCOUNT) | $2; }
	|	nlist RPRF
		{	needs |= RSHARE|RPREF; }
	|	nlist LPRF
		{	needs |= LSHARE|LPREF; }
	|	nlist RSHR
		{	needs |= RSHARE; }
	|	nlist LSHR
		{	needs |= LSHARE; }
	|	nlist PNEED
		{	needs |= NPAIR; }
	|	nlist GOODCC
		{	rewrite |= RESCC; }
	|	nlist NOVAL
		{
			if (rewrite)
				yyerror("$N incompatible with other results");
			rewrite = RNULL;
		}
	|	nlist LRES
		{	rewrite |= RLEFT; }
	|	nlist RRES
		{	rewrite |= RRIGHT; }
	|	nlist RREG
		{	if( !(needs&NCOUNT) ) needs |= $2;
			rewrite |= (($2==1)?RESC1:(($2==2)?RESC2:RESC3));
			}
	|	nlist LCOUNT
		{	lcount += 1; }
	|	nlist RCOUNT
		{	rcount += 1; }
	;

num	:	DIG
	|	num DIG
		{	$$ = 10*$1 + $2; }
	;

opcost	:	cost
		{	was_cost = 1; }
	|	/* EMPTY */
		{	was_cost = 0; $$ = dcost; }
	;

cost	:	':' cexpr
		{	$$ = $2; }
	;

cexpr	:	cterm
	|	OP cterm
		{	$$ = uopcost( $1, $2 ); }
	|	cterm OP cterm
		{	$$ = bopcost( $2, $1, $3 ); }
	;

cterm	:	'(' cexpr ')'
		{	$$ = $2; }
	|	num
	;

shape	:	/* EMPTY */
		{	$$ = 0; }
	|	NONAME
		{	$$ = NACON;	/* constant with no name */ }
	|	USERCOST num
		{	$$ = $2|SUSER;  /* user's cost ftn */ }
	|	CONVAL num
		{	$$ = $2|SVAL;  /* positive constant value */ }
	|	NCONVAL num
		{	$$ = $2|SNVAL;  /* negative constant value */ }
	|	POSRANGE num
		{	$$ = $2|SRANGE0;  /* positive range */ }
	|	SGRANGE num
		{	$$ = $2|SSRANGE;  /* signed range */ }
	;

sref	:	nterm
		{	$$.ty = 0;
			$$.sha = $1;
			}
	|	nterm STYPE     /* do this before doing in nterm */
		{	$$.sha = $1;
			$$.ty = $2; }
	;

slist	:	costnexp
	|	slist ',' costnexp
		{	$$ = bop( MANY, $1, $3, 0 ); }
	;

costnexp:	nexp opcost
		{	$$ = sharecost($1, $2); }
	;
	
nexp	:	nterm
	|	OP nterm
		{	$$ = uop( $1, $2 ); }
	|	nterm OP nterm
		{	$$ = bop( $2, $1, $3, 0 ); }
	;

nterm	:	'(' nexp ')'
		{	$$ = $2; }
	|	NM
	|	nterm STYPE
		{	$$ = top( $1, $2 );  }
	;

%%
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
