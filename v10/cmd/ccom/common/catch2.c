# include "mfile2.h"

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

int tmpoff;  /* offset of next temp to be allocated */

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
		SETOFF( p->stn.stsize, ALSTACK);  /* alignment */
		t += p->stn.stsize;  /* size */
	}
	else 
	{
		SETOFF( t, ALINT );
		t += SZINT;
	}
	return( t );
}
