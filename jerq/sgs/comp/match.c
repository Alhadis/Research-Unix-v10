/* @(#) match.c: 1.1 12/22/83				*/

# include "mfile2.h"
# ifdef SDB
# include "sdb.h"
# endif

int fldsz, fldshf;

int sdebug = 0;
int zflag = 0;			/* non-zero to output stin line and cost for
				** each assembly instruction
				*/

ttype( t )
register TWORD t; 
{
	/* return the coded type of t */
	/* this is called only from the first pass */

# ifdef TWOPTRS
	if( ISPTR(t) ) 
	{
		do 
		{
			t = DECREF(t);
		} while ( ISARY(t) );
		/* arrays that are left are usually only
		** in structure references... 
		*/
		if( TWOPTRS(t) ) return( TPOINT2 );
		return( TPOINT );
	}
# endif

	if( t != BTYPE(t) ) return( TPOINT ); /* TPOINT means not simple! */

	switch( t )
	{

	case CHAR:
		return( TCHAR );
	case SHORT:
		return( TSHORT );
	case STRTY:
	case UNIONTY:
		return( TSTRUCT );
	case INT:
		return( TINT );
	case UNSIGNED:
		return( TUNSIGNED );
	case USHORT:
		return( TUSHORT );
	case UCHAR:
		return( TUCHAR );
	case ULONG:
		return( TULONG );
	case LONG:
		return( TLONG );
	case FLOAT:
		return( TFLOAT );
	case DOUBLE:
		return( TDOUBLE );
	case VOID:
		return( TVOID );
	}
	cerror( "ttype(0%o)", t );
	/* NOTREACHED */
}

# define VALUE (RESC1|RESC2|RESC3|RLEFT|RRIGHT)

OPTAB *
match( p, q )
register NODE *p; 
register OPTAB *q; 
{
	/* look for match in table */
	/* given a tree, p, and a template q, returns the next template that
	/* matches p */
	/* q == NULL starts the process off */
	/* match is not called recursively, so much of the setup can be done
	/* into static variables */

	static NODE *r, *l;
	static tl, tr, goal, tyop;
	static pop, lflg;
	int rr;

	if( !q ) {
		/* startup code */

		q = ophead[pop=p->tn.op];
		l = getlo( p, pop );		/* left child */
		r = getro( p, pop );		/* right child */
		tl = l->in.type;
		tr = r->in.type;
		lflg = asgop(pop);  /* forces a match on lvalues */
		if( p->tn.goal == CCC ) goal = (VALUE|RESCC);
		else if( p->tn.goal == NRGS ) goal = VALUE;
		else goal = ~0;
		tyop = p->in.type;
	}

	else {
		if( fast ) return( (OPTAB *)0 );  /* only first match */
		else q = q->nextop;
	}
	if( !q ) return( (OPTAB *)0 );

# ifndef NODBG
	if( sdebug ) {
		printf( "match tree %d, op %s\n", p-node, opst[pop] );
	}
# endif
	for( ; q ; q = q->nextop ){

		/* some templates are no good for computing values */

		if( !( tl & q->ltype ) ) continue;
		if( !( tr & q->rtype ) ) continue;

		if( !( goal & q->rewrite ) ) continue;

# ifndef NODBG
		if( sdebug ) {
			printf( "	try table entry, line %d, op %s\n",
				q->stinline, opst[q->op] );
		}
# endif
		if( !( tyop & q->tyop ) ) {
# ifndef NODBG
			if( sdebug ) {
				printf(
				"\tentry line %d fails tyop, %o vs %o\n",
				q->stinline, tyop, q->tyop );
			}
# endif
			continue;
		}

		if( q->rshape ){
			if( rr=mspine( q->rshape, (q->lshape)?r:p, 0, sha[1] )){
				sha[1][rr] = 0;
				if( !q->lshape ) ustrip(pop);
			}
			else continue;
		}
		else {
			sha[1][1] = sha[1][0] = (SHAPE *)0;
		}

		if( q->lshape ){
			if( !lflg && asgop(q->op) ){
				/* q->op is an assignment op, pop isn't */
				/* thus, the lhs => a register or temp */
				rr = rtspine( q->lshape, p );
			}
			else rr = mspine( q->lshape, l, lflg, sha[0] );
			if( rr ) sha[0][rr] = 0;
			else continue;
		}
		else {
			sha[0][1] = sha[0][0] = (SHAPE *)0;
		}

		/* at this point, we have a match */

# ifndef NODBG
		if( sdebug ) {
			printf( "table line %d matches tree %d\n",
				q->stinline, p-node );
		}
# endif

		return( q );  /* found match */
	}
	return( (OPTAB *)0 );
}

ustrip( o )
{
	/* strip out of sha[1] all shapes with op != o */
	/* sty should do this someday, but for now, it's done here */
	register i, j;

	for( j=i=0; sha[1][j]; ++j ) {
		if( sha[1][j]->op == o ) sha[1][i++] = sha[1][j];
	}
	sha[1][i] = (SHAPE *)0;
}

rtspine( ps, p )
SHAPE **ps;
NODE *p; {
	/* special routine to put the lhs into a reg or a temp */
	/* looks for reg and/or temp in s */
	/* makes a list in sha[0] */
	register rr;
	register SHAPE *s;

	for( rr = 0; s = *ps; ++ps ) {
		/* works because FREE, CCODES, REG, and TEMP are first */
		switch( s->op ) {

		case FREE:
		case CCODES:
			continue;

		case REG:
		case TEMP:
			if( s->sh ) if( !spshape( s->sh, p ) ) continue;
			if( rr >= NSHP-1 ) cerror( "too many shapes" );
			sha[0][rr] = s;
			++rr;
			continue;

		default:
			break;
		}

		break;
	}

	return( rr );
}

restrip( pls )
register SHAPE **pls; 
{
	/* throw away those shapes that can't be results */
	register SHAPE **p;

	for( p=pls; *p = *pls; ++p, ++pls ) 
	{
		if( noresult( *p ) ) --p;  /* overwrite if result is illegal */
	}
}

noresult( s )
register SHAPE *s; 
{
	if( !s ) return( 0 );
	if( s->op == INCR || s->op == DECR ) return( 1 );
	return( (s->sr && noresult(s->sr)) ||
	    (s->sl && noresult( s->sl )) );
}

smspine( s, p, flag )
register NODE	*p; 
register SHAPE	*s; 
{
	/* match the spine of s, including special shapes */
	/* this is a submatch, called only by mspine */
	/* flag is nonzero if STAR's must be skipped */

	register sop, pop;

again:

	sop = s->op;
	pop = p->tn.op;

# ifndef NODBG
	if( sdebug>1 )
	{
		printf( "			smspine(%d[%s], %d[%s])\n",
		s-shapes, opst[sop], p-node, opst[pop] );
	}
# endif

	if( ( s->sh ) && !spshape( s->sh,p) ) return( 0 );
	if( sop != pop ) 
	{
		if( sop == CCODES || sop == FREE ) return(1);
		return( !flag && (sop==REG || sop==TEMP) );
	}
	if( sop == STAR ) 
	{
		flag = 0;
		goto recurse;
	}
	switch( optype( sop ) ) 
	{

	case BITYPE:
		if( !smspine( s->sr, p->in.right, 0 ) ) return(0);
		flag = asgop(sop);
		/* FALLTHRU */

	case UTYPE:
recurse:
		s = s->sl;
		p = p->in.left;
		goto again;

	default:
		return( 1 );
	}
}

mspine( ps, p, flag, ppls )
register NODE	*p;
register SHAPE	**ps;
register SHAPE	**ppls; 
{
	/* match the spine of s, including special shapes */
	/* s points to a list of shapes */
	/* those shapes that match are copied into **ppls, with a null at end */
	/* flag = 1 forces regs and temps to match exactly */
	/* rr, the number of matches, is returned */

	register SHAPE *s;
	int pop, rr;

# ifndef NODBG
	if( sdebug ) 
	{
		printf( "\tmspine( %d, %d, %d )\n", ps-pshape, p-node, flag );
	}
# endif
	pop = p->tn.op;

	/* copy the wild-cards, and those where the op matches */
	for( rr=0; s = *ps; ++ps ) 
	{
		switch( s->op ) 
		{

		case REG:
		case TEMP:
			if( flag ) break;
			if( s->sh ) if( !spshape(s->sh,p) ) continue;
			/* FALLTHRU */

		case FREE:
		case CCODES:
			if( rr >= NSHP-1 ) cerror( "too many shapes" );

# ifndef NODBG
			if( sdebug )
			{
				printf(
				"\t\tmspine( %d[%s], %d[%s] ), becomes %d\n",
				s-shapes, opst[s->op], p-node,
				opst[pop], rr );
			}
# endif

			ppls[rr++] = s;
			continue;

		default:
			break;
		}
		break;
	}

	/* now, either s is 0 or s has a "real" op */
	/* the first condition survives the next two loops */

	/* we have matched the wild cards: find others where the op matches */

	for( ; (s= *ps) && s->op != pop; ++ps ) 
	{
		 ; 
	}

	/* now, s is null or matches the op of p */
	for( ; (s = *ps) && s->op == pop; ++ps ) 
	{

		if( pop == STAR ) 
		{
			if( !smspine( s->sl, p->in.left, 0 ) ) continue;
		}

		/* general case: check the subtrees also */
		else if( s->sl ) 
		{
			if( s->sr ) 
			{
				if( !smspine( s->sr, p->in.right, 0 ) )
					continue;
				if( !smspine( s->sl, p->in.left, asgop(s->op)) )
					continue;
			}
			else 
			{
				if( !smspine( s->sl, p->in.left, flag ))
					continue;
			}
		}
		else if( s->sr && !smspine( s->sr, p->in.right, 0 ) )
			continue;

		/* if we get this far, we are in good shape */

		if( s->sh ) if( !spshape( s->sh, p ) ) continue;
		if( rr >= NSHP-1 ) cerror( "too many shapes" );

# ifndef NODBG
		if( sdebug )
		{
			printf( "\t\tmspine( %d[%s], %d[%s] ), becomes %d\n",
			s-shapes, opst[s->op], p-node,
			opst[pop], rr );
		}
# endif

		ppls[rr++] = s;
	}

	/* fall out and we are done */
	return( rr );
}


tempok( p )
NODE *p; 

{
	/* decide if the result of p is OK as a temp */
	int o;
	if( (o = p->tn.op) == TEMP ) return( 1 );
	if( asgop(o) && o!=INCR && o!=DECR ) 
	{
		return( p->in.left->tn.op == TEMP );
	}
	return( 0 );
}

int sideff;

rmside( p )
NODE *p; 

{
	/* p is a tree with side effects: remove them */
	/* this is done in place */
	int o;
	NODE *q;

	o = p->tn.op;
	switch( o ) 
	{

	case INCR:
	case DECR:
	case ASG PLUS:
	case ASG MINUS:
		if( p->in.right->tn.op == ICON ) p->in.right->tn.op = FREE;
		else 
		{
			regrcl( p->in.right );
			tfree( p->in.right );
		}
		q = p->in.left;
		*p = *q;
		q->tn.op = FREE;
		rmside( p );
		return;
	}

	switch( optype( o ) )
	{

	case BITYPE:
		rmside( p->in.right );
	case UTYPE:
		rmside( p->in.left );
	}
}

expand( p, goal, cp, q )
NODE *p;  
char *cp; 
OPTAB *q;
{
	/* generate code by interpreting table entry */

	CONSZ val;
	NODE *q1, *q2;
	int c;
	TWORD t;

/*# ifdef SDB
/*	if( (c = p->tn.op) == CALL || c == UNARY CALL )
/*	{
/*		/* number of args (for sdb) */
/*		pstab( S_NARGS, p->stn.argsize/SZINT);
/*	}
/*# endif
/**/

	for( ; *cp; ++cp )
	{
		switch( *cp )
		{

		case '\\':
			++cp;
			/* FALLTHRU */
		default:
			PUTCHAR( *cp );
			continue;  /* this is the usual case... */

		case '\n':
			if( zflag )
			{  /* add stin line number of production as a comment */
				printf( "\t\t%s %d", COMMENTSTR, q->stinline );
				if( p->in.cst[CEFF] < INFINITY )
					printf( " = %d", p->in.cst[CEFF] );
			}
			PUTCHAR( *cp );
			continue;

		case 'Y':
			/* don't check this string: deleted by match */
			continue;

		case 'Z':  /* special machine dependent operations */
			zzzcode( p, &cp, q );
			continue;

		case 'E':	/* exit */
			if( zflag )
			{  /* add stin line number of production as a comment */
				printf( "\t%s\t%d = %d",
				COMMENTSTR, q->stinline, p->in.cst[CEFF] );
			}
			PUTCHAR( '\n' );
			return;

# ifndef STACK
		case 'D':	/* test for dependency */
			q1 = getadr( p, &cp );
			if( cp[1] == '!' )
			{
				c=0;
				++cp;
			}
			else c=1;
			q2 = getadr( p, &cp );
			if( q1->tn.op != REG ) cerror( "bad D" );
			if( ushare( q2, q1->tn.rval ) == c ) continue;
			goto cream;
# endif

		case 'R':  /* put an operand into a particular register */
			/* if the operand is in the register, delete the line */
			q1 = getadr( p, &cp );
			c = (*++cp == '=');
			q2 = getadr( p, &cp );
			if( q1->tn.op != REG )
			{
				if( c ) goto cream;
				continue;
			}
			if( q2->tn.op != REG )
			{
				if( c ) goto cream;
				continue;
			}
			if( q1->tn.rval != q2->tn.rval )
			{
				if( c ) goto cream;
				continue;
			}
			if( c ) continue;
cream:
			/* clobber a line to newline, or up to an E */
			while( *++cp != '\n' && *cp != 'E' ) 
			{
				if( *cp == '\\' ) ++cp; /* hide next char */
				if( !*cp ) return;
			}
			continue;

		case 'F':  /* this line deleted if FOREFF is active */
			if( goal & FOREFF ) goto cream;
			continue;

		case 'S':  /* field size */
		case 'H':  /* field shift */
		case 'M':  /* field mask, in place */
		case 'N':  /* field mask, right shifted */
			c = *cp;
			if( cp[1] == '~' ) 
			{
				if( c == 'M' ) c = 'm';
				else if( c == 'N' ) c = 'n';
				else cerror( "bad ~ after S or H" );
				++cp;
			}
			else if( cp[1] == '?' ) 
			{
				if( c == 'H' ) c = 'h';
				else cerror( "bad ? after S, M, or N" );
				++cp;
			}
			q1 = getadr(p,&cp);
			if( q1->tn.op != FLD ) cerror( "bad FLD for %c", c );
			fldsz = UPKFSZ(q1->tn.rval);
# ifdef RTOLBYTES
			fldshf = UPKFOFF(q1->tn.rval);
# else
			t = q1->tn.type;
			if( t & (TLONG|TULONG) )
			{
				fldshf = SZLONG - fldsz - UPKFOFF(q1->tn.rval);
			}
			else if( t & (TSHORT|TUSHORT) )
			{
				fldshf = SZSHORT - fldsz - UPKFOFF(q1->tn.rval);
			}
			else if( t & (TCHAR|TUCHAR) )
			{
				fldshf = SZCHAR - fldsz - UPKFOFF(q1->tn.rval);
			}
			else fldshf = SZINT - fldsz - UPKFOFF(q1->tn.rval);
# endif
			if( c == 'h' ) 
			{
				if( fldshf == 0 ) goto cream;
				continue;
			}
			if( c=='S' || c=='H' )
			{
				printf( "%d", c=='S' ? fldsz : fldshf );
				continue;
			}
			val = 1;
			val <<= fldsz;
			--val;
			if( c=='M' || c == 'm' ) val <<= fldshf;
			if( c=='m' || c == 'n' ) val = ~val;
# ifdef MYMASK
			MYMASK(val);
# else
			printf( "%ld" , val );
# endif
			continue;

		case 'L':  /* output special label field */
			printf( "%d", p->bn.label );
			continue;

		case 'C': /* for constant value only */
			conput( q1 = getadr( p, &cp ) );
			if( !sideff ) cerror( "constant with side effects?" );
			continue;

		case 'I': /* in instruction */
			insput( q1 = getadr( p, &cp ) );
			if( sideff ) rmside( q1 );
			continue;

		case 'A': /* address of */
			adrput( q1 = getadr( p, &cp ) );
			if( sideff ) rmside( q1 );
			continue;

		case 'U': /* for upper half of address, only */
			upput( q1 = getadr( p, &cp ) );
			if( sideff ) rmside( q1 );
			continue;
#ifdef TMPSRET
		case 'T': /* grab a temp for structure return and give it */
			  /* to adrput */
			q1 = talloc();
			q1->tn.op = TEMP;
			q1->tn.type = TSTRUCT;
			q1->tn.lval = freetemp(p->stn.stsize / SZINT);
			q1->tn.lval = BITOOR(q1->tn.lval);
			adrput(q1);
			tfree(q1);
			continue;
#endif

		}
	}
}

NODE *
getlr( p, c )
NODE *p; 

{

	/* return the pointer to the left or right side of p, or p itself,
	** depending on the optype of p 
	*/

	switch( c ) 
	{

	case '1':
	case '2':
	case '3':
		return( &resc[c-'1'] );

	case 'L':
		return( optype( p->in.op ) == LTYPE ? p : p->in.left );

	case 'R':
		return( optype( p->in.op ) != BITYPE ? p : p->in.right );

	}
	cerror( "bad getlr: %c", c );
	/* NOTREACHED */
}

NODE *
getadr( p, pc )
NODE *p; 
char **pc; 

{
	/* like getlr, but allows (LL), (LR), etc. */
	int c;
	sideff = 1;
	c = *++(*pc);
	if( c == '-' )
	{
		c = *++(*pc);
		sideff = 0;
	}
	if( c == '.' ) return( p );
	else if( c != '(' ) return( getlr( p, c ) );

	for(;;) 
	{
		switch( c = *++(*pc) ) 
		{

		case ')':
			return( p );

		case 'L':
			p = getl( p );
			continue;

		case 'R':
			p = getr( p );
			continue;

		default:
			cerror( "bad table char in (): %c", c );
		}
	}
	/* NOTREACHED */
}

long pow2[] = 
{
	01L,	02L,	04L,
	010L,	020L,	040L,
	0100L,	0200L,	0400L,
	01000L,	02000L,	04000L,
	010000L,	020000L,	040000L,
	0100000L,	0200000L,	0400000L,
	01000000L,	02000000L,	04000000L,
	010000000L,	020000000L,	040000000L,
	0100000000L,	0200000000L,	0400000000L,
	01000000000L,	02000000000L,	04000000000L,
	010000000000L,	020000000000L,	040000000000L,
};

spshape( sh, p )
NODE *p; 

{
	long val;
	int t, i;

	if( !(sh&SPECIAL) ) cerror( "spshape" );
	if( sh&SPTYPE )
	{
# ifndef NODBG
		if( sdebug )
			printf( "SPTYPE(%d, %o), ttype=%o\n", p-node,
				sh, p->tn.type );
# endif

		if( sh & p->tn.type ) return( 1 );
		else return( 0 );
	}
	t = (sh&STMASK);
	i = (sh&SVMASK);

	switch( t ) 
	{

	case SRANGE0:
	case SSRANGE:
		if( i<0 || i>31 ) cerror( "bad shape range" );
	case SVAL:
	case SNVAL:
	case NACON:

		if( p->tn.op != ICON || p->tn.name != (char *) 0 ) return( 0 );
		if( t == NACON ) return(1);
		val = p->tn.lval;
		if( t==SVAL && val == i ) return( 1 );
		else if( t==SNVAL && val == -i ) return( 1 );
		else if( t==SRANGE0 && val>=0 && val<pow2[i] ) return( 1 );
		else if( t==SSRANGE && val>= -pow2[i] && val<pow2[i] )return(1);
		return( 0 );

	case SUSER:
		return( special( sh, p ) );

	default:
		cerror( "bad special call" );
		/* NOTREACHED */
	}
}
