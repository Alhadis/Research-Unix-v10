# include "mfile2.h"
/* a lot of the machine dependent parts of the second pass */

char *
ccbranches[] = {
	"	jeql	L%d\n",
	"	jneq	L%d\n",
	"	jleq	L%d\n",
	"	jlss	L%d\n",
	"	jgeq	L%d\n",
	"	jgtr	L%d\n",
	"	jlequ	L%d\n",
	"	jlssu	L%d\n",
	"	jgequ	L%d\n",
	"	jgtru	L%d\n",
	};

cbgen( o, lab, mode )
int o, lab, mode;
{ /*   printf conditional and unconditional branches */

	if( !o ) printf( "	jbr	L%d\n", lab );
	else	if( o > UGT ) cerror( "bad conditional branch: %s", opst[o] );
	else printf( ccbranches[o-EQ], lab );
}

zzzcode( p, ppc, q )
NODE *p; char **ppc; OPTAB *q;
{
	register c;

	switch( c= *++(*ppc) )
	{
	case 'I':
		cbgen( p->bn.lop, p->bn.label, c );
		return;


	case 'k':	/* complement mask for bit instr */
		printf("$%ld", ~p->in.right->tn.lval);
		return;

	case 'c':
		/* argument size */
		printf( "$%d", p->stn.argsize/SZINT);
		return;

	case 'U':	/* 32 - n, for unsigned right shifts */
		printf("$%d", 32 - p->in.right->tn.lval );
		return;

	case 'T':	/* structure length for arguments */
		printf("$%d", p->stn.stsize / SZCHAR);
		break;

	case 'M':	/* move address	*/
		staradr( p->in.right );
		break;

	case 'S':  /* structure assignment */
		{
			register NODE *l, *r;
			register size;

			if( p->in.op == STASG )
			{
				l = p->in.left;
				r = p->in.right;
			}
			else if( p->in.op == STARG )
			{  /* store an arg */
				r = p->in.left;
			}
			else cerror( "STASG bad" );

			size = p->stn.stsize/SZCHAR;

			if( size <= 0 || size > 65535 )
				cerror("structure size <0=0 or >65535");

			switch(size)
			{
			case 1:
				printf("	movb	");
				break;
			case 2:
				printf("	movw	");
				break;
			case 4:
				printf("	movl	");
				break;
			case 8:
				printf("	movq	");
				break;
			default:
				printf("	movc3	$%d,", size);
				break;
			}
			staradr( r );
			printf(",");
			if( p->in.op == STARG ) printf( "(sp)" );
			else staradr( l );
			printf("\n");

		}
		break;

	default:
		cerror( "illegal zzzcode" );
	}
}

staradr( p )
NODE *p;
{
	NODE *pp;
	pp = talloc();
	pp->in.op = STAR;
	pp->in.left = p;
	upput( pp );
	pp->in.op = FREE;
}

conput( p )
register NODE *p;
{
	switch( p->in.op )
	{
	case ICON:
		acon( p );
		return;

	case REG:
		printf( "%s", rnames[p->tn.rval] );
		return;

	default:
		cerror( "illegal conput" );
	}
}

insput( p )
NODE *p;
{
	cerror( "insput" );
}

/* new use for an old routine
 * upput now puts out an indirect address ( STAR node )
 * equivalent to the old case STAR in adrput
 * as upput, we can now get to this routine directly from the
 * templates, using the U symbol
 */

int sideff;

upput( p )
NODE *p;
{
	register NODE *r, *l, *pp;
	register o;
	pp = ( STAR == p->in.op ) ? p->in.left : p ;
	o = pp->in.op;
	if( o==NAME || o==STAR || o==TEMP || o==VAUTO || o==VPARAM )
	{
		printf( "*" );
		adrput(pp);
		return;
	}
	if( o==ICON )
	{
		acon( pp );
		sideff = 0;
		return;
	}
	if( o == PLUS )
	{
		r = pp->in.right;
		l = pp->in.left;
		if( l->in.op == REG && r->in.op == ICON )
		{
			acon( r );
			pp = pp->in.left;
		}
		else
		{
			/* index mode (shudder) */
			/* save the index, rewrite p to point to
			/* the rest, call adrput recursively, and
			/* then tack on the index */
			if( l->in.op == UNARY AND )
			{ /* double index */
				adrput( l->in.left );
				l = r;
			}
			else
			{
				p->in.left = r;
				adrput( p );
				p->in.left = pp;  /* back to normal */
			}
			if( l->in.op == LS ) l = l->in.left;
			if( l->in.op != REG ) cerror("illegal address");
			printf( "[%s]", rnames[l->tn.rval] );
			return;
		}
	}
	else if( o == MINUS )
	{
		r = pp->in.right;
		if( r->tn.op != ICON || r->tn.name ) cerror("illegal address");
		r->tn.lval = -r->tn.lval;
		acon( r );
		r->tn.lval = -r->tn.lval;
		pp = pp->in.left;
	}
	else if( o == ASG MINUS )
	{
		r = pp->in.right;
		if( r->tn.op != ICON ) cerror("illegal address");
		r = pp->in.left;
		if( r->tn.op != REG ) cerror("illegal address");
		/* always do the side effect */
		printf( "-(%s)", rnames[r->tn.rval] );
		sideff = 1;  /* cream it */
		return;
	}
	else if( o == INCR )
	{
		r = pp->in.right;
		if( r->tn.op != ICON ) cerror("illegal address");
		r = pp->in.left;
		if( r->tn.op != REG ) cerror("illegal address");
		if( sideff ) printf( "(%s)+", rnames[r->tn.rval] );
		else printf( "(%s)", rnames[r->tn.rval] );
		return;
	}
	if( pp->tn.op != REG ) cerror("illegal address");
	printf( "(%s)", rnames[pp->tn.rval] );
	return;
}

adrput( p )
register NODE *p;
{
	/* output an address, with offsets, from p */
	register o;

	while( (o=p->in.op) == FLD || o==CONV )
	{
		p = p->in.left;
		o = p->in.op;
	}
	switch( o )
	{
	case NAME:
		acon( p );
		sideff = 0;
		return;

	case ICON:
		/* addressable value of the constant */
		printf( "$" );
		acon( p );
		sideff = 0;
		return;

	case REG:
		printf( "%s", rnames[p->tn.rval] );
		sideff = 0;
		return;

	case STAR:
		upput( p );
		return;

	case TEMP:
		sideff = 0;
		printf( "%ld(fp)", p->tn.lval - maxboff / SZCHAR );
		return;

	case VAUTO:
		sideff = 0;
		printf( "%ld(fp)", p->tn.lval );
		return;

	case VPARAM:
		sideff = 0;
		printf( "%ld(ap)", p->tn.lval );
		return;

	default:
		cerror( "illegal address" );
		return;
	}
}

acon(p)
NODE *p;
{ /* print out a constant */

	if( p->tn.name == 0 )
	{	/* constant only */
		printf( "%ld", p->tn.lval);
	}
	else if( p->tn.lval == 0 )
	{	/* name only */
		printf( "%s", p->tn.name );
	}
	else
	{				/* name + offset */
		printf( "%s+%ld", p->tn.name, p->tn.lval );
	}
}

special()
{
	cerror("reached special");
}
