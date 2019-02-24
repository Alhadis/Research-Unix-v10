/*	@(#) allo.c: 1.7 6/26/84	*/

# include "mfile2.h"

NODE resc[NRGS];

int busy[NRGS];

# define TBUSY 0100

allo0()
{
	/* free everything */
	register i;

	for( i=0; i<NRGS; ++i )
	{
		busy[i] = 0;
	}
}

allo( p, q )
register NODE *p; 
register struct optab *q; 
{
	register n, i, j;
	register NODE *presc;

	n = q->needs;
	i = 0;
	/* This code assumes a double reg counts 1 */
	while( n & NCOUNT )
	{
		if( n&NPAIR ) 
		{
			j = freepair( p, n&NMASK );
			busy[j] |= TBUSY;
			busy[j+1] |= TBUSY;
#if defined(M32B) && defined(IMPREGAL)
			radbl();	/* inform optimizer double reg used */
#endif
		}
		else 
		{
			j = freereg( p, n&NMASK );
			busy[j] |= TBUSY;
		}
		n -= NREG;
		presc = &resc[i];
		presc->in.op = REG;
		presc->tn.rval = j;
		presc->tn.type = p->tn.type;
		presc->tn.lval = 0;
		presc->in.name = (char *) 0;
		++i;
	}

	/* turn off "temporarily busy" bit */
	for( j=0; j<NRGS; ++j )
	{
		busy[j] &= ~TBUSY;
	}
#ifndef NODBG
	if( rdebug > 1 )
	{
		printf( "allo( %d, %d ), %o", p-node, q->stinline, q->needs );
		for( j=0; j<i; ++j )
		{
			if( resc[j].tn.op == REG )
				printf( ", REG(%d)", resc[j].tn.rval );
			else
				printf( ", TEMP(%ld)", resc[j].tn.lval );
		}
		putchar( '\n' );
		prbusy( "busy:" );
	}
#endif
}

int tmpoff;  /* offset of next temp to be allocated */

int rdebug = 0;

freetemp( k )
register k;
{
	/* allocate k integers worth of temp space
	** we also make the convention that, if the number of words is more than 1,
	** it must be aligned for storing doubles... 
	*/

# ifndef BACKTEMP
	int t;

	if( k>1 )
	{
		SETOFF( tmpoff, ALDOUBLE );
	}

	t = tmpoff;
	tmpoff += k*SZINT;
	if( tmpoff > maxtemp ) maxtemp = tmpoff;
	return(t);

# else
	tmpoff += k*SZINT;
	if( k>1 ) 
	{
		SETOFF( tmpoff, ALDOUBLE );
	}
	if( tmpoff > maxtemp ) maxtemp = tmpoff;
	return( -tmpoff );
# endif
}

# ifdef STACK
/* for stack machines, totally disable the register allocation */
freereg( p, n )
NODE *p; 
{
	return( 0 );
}

freepair( p, n )
NODE *p; 
{
	cerror( "pairs on a stack machine?" );
}

rbusy( r, t )
TWORD t; 
{
}
rfree( r, t )
TWORD t; 
{
}

regrcl( p )
NODE *p; 
{
}
# else
freepair( p, n )
register NODE *p; 
register n;
{
	/* allocate a register pair */
	/* p gives the type */

#ifdef MYPAIR
	return (mypair(p, n));
#else
	register j;

	if( callop(p->in.op) )
	{
		j = callreg(p);
		if( j&1 ) cerror( "callreg returns bad pair" );
		if( usable( p, n, j ) ) return( j );
		/* have allocated callreg first */
	}
	if( n&NMASK )
	{
#ifdef ODDPAIR			/* if pair may start on odd reg. */
		for( j=0; j<NRGS; j++ )
#else
		for( j=0; j<NRGS; j+=2 )
#endif
			if( usable(p,n,j) && usable(p,n,j+1))
				return( j );
	}
	cerror( "allocation fails, op %s", opst[p->tn.op] );
	/* NOTREACHED */
#endif
}

freereg( p, n )
register NODE *p; 
register n;
{
	/* allocate a register */
	/* p gives the type */

	register j;
	register int t = optype( p->tn.op );

	if( callop(p->in.op) )
	{
		j = callreg(p);
		if( usable( p, n, j ) ) return( j );
		/* have allocated callreg first */
	}
	if( n&NMASK )
	{
		if( (n&LPREF) && (j = shared( getlt( p, t ) ) ) >= 0 &&
		   usable( p, n, j ) ) return( j );
		if( (n&RPREF) && (j = shared( getrt( p, t ) ) ) >= 0 &&
		   usable( p, n, j ) ) return( j );
		for( j=0; j<NRGS; ++j ) if( usable(p,n,j) ) return( j );
	}
	cerror( "allocation fails, op %s", opst[p->tn.op] );
	/* NOTREACHED */
}

shared( p )
register NODE *p; 
{
	/* simple, at present */
	/* try to find a single register to share */
	register r, o;
#ifndef NODBG
	if( rdebug ) 
	{
		printf( "shared called on:\n" );
		e2print( p );
	}
#endif
	if( (o=p->tn.op) == REG ) 
	{
		r = p->tn.rval;
		if (r >= NRGS) return ( -1 );
#ifndef NODBG
		if( rdebug ) 
		{
			printf( "preference for %s\n", rnames[r] );
		}
#endif
		return( r );
	}
	/* we look for shared regs under unary-like ops */
	switch( optype( o ) ) 
	{

	case BITYPE:
		/* look for simple cases */
		/* look only on the left */
	case UTYPE:
		return( shared( p->in.left ) );
	}
	return( -1 );
}

usable( p, n, r )
register NODE *p; 
register n,r;
{
	/* decide if register r is usable in tree p to satisfy need n */
	/* this does not concern itself with pairs */

	if( r>= NRGS || (busy[r] & TBUSY) ) return( 0 );
	if( busy[r] > 1 ) 
	{
		/*
		** uerror( "register %d too busy", r );
		*/
		return( 0 );
	}
	if( busy[r] == 0 ) {
		return(1);
	}

	/* busy[r] is 1: is there chance for sharing */
	return( shareit( p, r, n ) );

}

shareit( p, r, n )
register NODE *p; 
register r;
int n;
{
	/* can we make register r available by sharing from p
	** given that the need is n 
	*/
	register NODE *sub;
	register int t = optype(p->tn.op);
	
	sub = getlt( p, t );
	if( (n&LSHARE) && ushare( sub, r ) )  {
		return 1;
	}
	sub = getrt( p, t );
	if( (n&RSHARE) && ushare( sub, r ) ) {
		return(1);
	}
	return(0);
}

ushare( p, r )
register NODE *p; 
register r;
{
	/* can we find register r to share in p */
	if( p->in.op == REG ) 
	{
		if( szty( p->tn.type ) == 2 && r==(p->tn.rval+1) ) return( 1 );
		return( r == p->tn.rval );
	}
	switch( optype( p->tn.op ) )
	{
	case BITYPE:
		if( ushare( p->in.right, r ) ) return( 1 );
	case UTYPE:
		if( ushare( p->in.left, r ) ) return( 1 );
	}
	return(0);
}

regrcl( p )
register NODE *p; 
{
	/* free registers in the tree (or fragment) p */
	register r;
	if( !p ) return;
	r = p->tn.rval;
	if( p->in.op == REG ) rfree( r, p->in.type );
	switch( optype( p->tn.op ) )
	{
	case BITYPE:
		regrcl( p->in.right );
		/* explict assignment to regs not accounted for */
		if( asgop(p->tn.op) && p->in.left->tn.op == REG ) break;
	case UTYPE:
		regrcl( p->in.left );
	}
}

rfree( r, t )
register TWORD t; 
register r;
{
	/* mark register r free, if it is legal to do so */
	/* t is the type */

#ifndef NODBG
	if( rdebug )
	{
		printf( "rfree( %s, ", rnames[r] );
		t2print( t );
		printf( " )\n" );
	}
#endif
	if( istreg(r) )
	{
		if( --busy[r] < 0 ) cerror( "register overfreed");
		if( szty( t ) > 1 )
		{
			if( !istreg(r+1) ) cerror( "big register" );
			if( --busy[r+1] < 0 ) cerror( "register overfreed" );
		}
	}
}

rbusy(r, t )
register r; 
register TWORD t; 
{
	/* mark register r busy */

#ifndef NODBG
	if( rdebug )
	{
		printf( "rbusy( %s, ", rnames[r] );
		t2print( t );
		printf( " )\n" );
	}
#endif
	if( istreg(r) ) 
	{
		++busy[r];
		if( szty( t ) > 1 )
		{
			if( !istreg(r+1) ) cerror( "big register" );
			++busy[r+1];
		}
	}
}

prbusy( s )
char *s;
{
	/* print out the busy[] array */
	int i;
	printf( "%s [", s );
	for( i=0; i<NRGS-1; ++i ) printf( "%d,", busy[i] );
	printf( "%d]\n", busy[NRGS-1] );
}

# endif

rwprint( rw )
register rw;
{
	/* print rewriting rule */
	register i, flag;
	static char * rwnames[] = 
	{
		"RLEFT",
		"RRIGHT",
		"RESC1",
		"RESC2",
		"RESC3",
		"RESCC",
		"RNOP",
		0,
	};
	if( rw == RNULL )
	{
		printf( "RNULL" );
		return;
	}
	flag = 0;
	for( i=0; rwnames[i]; ++i )
	{
		if( rw & (1<<i) )
		{
			if( flag ) printf( "|" );
			++flag;
			printf( rwnames[i] );
		}
	}
	if( !flag ) printf( "?%o", rw );
}

reclaim( p, rw, goal )
register NODE *p; 
register rw, goal;
{
	register NODE *q;
	register o;

	/* get back stuff */
#ifndef NODBG
	if( rdebug )
	{
		printf( "reclaim( %d, ", p-node );
		rwprint( rw );
		printf( ", " );
		prgoal( goal );
		printf( " )\n" );
	}
#endif
	if( !p ) return;
	/* special cases... */
	if( (o=p->tn.op) == COMOP )
	{
		/* LHS has already been freed; don't free again */
		regrcl( p->in.right );
	}
	else regrcl( p );
	if( (o==FREE && rw==RNULL) || rw==RNOP ) return;
	if( callop(o) )
	{
		/* check that all scratch regs are free */
		callchk(p);  /* ordinarily, this is the same as allchk() */
	}
	if( rw == RNULL || (goal&FOREFF) )
	{
		/* totally clobber, leave nothing */
		tfree(p);
		return;
	}
	/* handle condition codes specially */
	if( (goal & FORCC) && (rw&RESCC)) 
	{
		/* result is CC register */
		tfree(p);
		p->in.op = CCODES;
		p->tn.lval = 0;
		p->tn.rval = 0;
		return;
	}
	q = 0;
	if( rw&RLEFT) q = getl( p );
	else if( rw&RRIGHT ) q = getr( p );
	else if( rw&RESC1 ) q = &resc[0];
	else if( rw&RESC2 ) q = &resc[1];
	else if( rw&RESC3 ) q = &resc[2];
	else 
	{
		cerror( "illegal reclaim, op %s", opst[p->tn.op]);
	}
	if( o == STARG ) p = p->in.left;  /* STARGs are still STARGS */
	q = tcopy(q);
	tfree(p);
	*p = *q;  /* make the result replace the original */
	q->in.op = FREE;
}

NODE *
tcopy( p )
register NODE *p; 
{
	/* make a fresh copy of p */
	register NODE *q;
	register r;

	q=talloc();
	*q = *p;
	r = p->tn.rval;
	if( p->in.op == REG ) rbusy( r, p->in.type );
	switch( optype(q->in.op) )
	{
	case BITYPE:
		q->in.right = tcopy(p->in.right);
	case UTYPE:
		q->in.left = tcopy(p->in.left);
	}
	return(q);
}

allchk()
{
	/* check to ensure that all register are free */
	register i;

	for( i=0; i<NRGS; ++i )
	{
		if( busy[i] )
		{
			cerror( "register allocation error");
		}
	}
}

/* this may not be the best place for this routine... */
argsize( p )
register NODE *p; 
{
	/* size of the arguments */
	register t;
	t = 0;
	if( p->tn.op == CM )
	{
		t = argsize( p->in.left );
		p = p->in.right;
	}
	if( p->tn.type & (TDOUBLE|TFLOAT) )
	{
		SETOFF( t, ALDOUBLE );
		t += SZDOUBLE;
	}
	else if( p->tn.type & (TLONG|TULONG) )
	{
		SETOFF( t, ALLONG );
		t += SZLONG;
	}
	else if( p->tn.type & TPOINT )
	{
		SETOFF( t, ALPOINT );
		t += SZPOINT;
	}
	else if( p->tn.type & TSTRUCT )
	{
		SETOFF( t, p->stn.stalign );  /* alignment */
		t += p->stn.stsize;  /* size */
	}
	else 
	{
		SETOFF( t, ALINT );
		t += SZINT;
	}
	return( t );
}
