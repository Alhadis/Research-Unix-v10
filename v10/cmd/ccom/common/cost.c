/*	@(#) cost.c: 1.4 3/4/84	*/

# include "mfile2.h"

# ifndef CSTORE
# define CSTORE(q) 2
# endif
# ifndef CLOAD
# define CLOAD(q) 2
# endif
# ifndef CCTEST
# define CCTEST(q) 1
# endif
# ifndef DFLT_STRATEGY
# define DFLT_STRATEGY	LTOR|RTOL
# endif

	/* enough regs so there is always a free pair */
# define HREG ((1+NRGS)/2+1)

#define LSAVED 30	/* since we're storing goals, also */
			/* actually, since only 0.6% of time is here, */
			/* who cares what the size is */
/* stores costs of leaves costed so far */
static struct leaf {
/*		int op;
/*		int type;
/*		int goal;
*/
		unsigned long key;	/* (goal<<26) | (type<<9) | op */
		int cst[NCOSTS];
}	leafcosts[LSAVED], *leaf_ptr=leafcosts, *recost_leaf=0;
	/* used to identify subtrees */
# define NSUBTREES 10
int nsubtree;
NODE *subtree[NSUBTREES];
int subgoal[NSUBTREES];

int strbc[NCOSTS];  /* the strategy done by bcost */
SHAPE * lshbc[NCOSTS];  /* left-hand shape */
SHAPE * rshbc[NCOSTS];  /* right-hand shape */

commute( p ) NODE *p; {
	/* commute p in place */
	register NODE *q;
#ifndef NODBG
	if(odebug) printf("commute: .=%u l=%u r=%u\n",p,p->in.left,p->in.right);
#endif
	q = p->in.left;
	p->in.left = p->in.right;
	p->in.right = q;
	}

# ifndef NODBG
# define GETS(x,y) if( e2debug>1) printf( "	x gets %d\n", y );
# define GETSN(x,y) if( e2debug>1) printf( "	cst[%d] gets %d\n", x, y );
# else
# define GETS(x,y)
# define GETSN(x,y)
# endif

bcost( p, q )
register NODE *p;
register OPTAB *q;
{
	/* return the basic costs of matching q against tree p */
	/* sha is set previously by match with a list of legal left
	/* and right shapes */
	/* bcost updates strbc, lshbc, and rshbc to reflect the
	/* strategy, left shape, and right shape, that minimizes the cost
	/* for q on p */

	/* j has its address taken, so it can't be in a reg */
	int j;
	int o, cc, c, s, tc, ttc, n, nn, lnn, lregs, rregs, cs, res;
	int il, ir;  /* index into left and right address shapes */
	int lsubtree;
	NODE *l, *r;
	register NODE *pp;
	register ix;
	SHAPE *sl, *sr;

	o = p->tn.op;

	/* look for the simple cases with register counts */
	n = (q->needs&NCOUNT);
	if( (q->needs&NPAIR) && n<HREG ) n = HREG;

	/* set up the left and right descendents */
	l = getlo( p, o );
	r = getro( p, o );
	/*
	 *	If the operator table entry does not have a left shape,
	 *	but it does have a right shape, then this
	 *	table entry is for leaves ONLY, referenced to p, not to r
	 */
	if( q->rshape && !q->lshape)
		r = p;

	res = q->rewrite;

	/* determine the code generation strategy */

	if( o == COMOP ) cerror( "COMOP in bcost" );

	s = LTOR;  /* default strategy */

	if( optype(o) == BITYPE ) {

		switch( o ) {

		case CALL:
		case STCALL:
		case FORTCALL:
# ifndef LTORARGS
		case CM:  /* function arguments */
# endif
			s = RTOL;
			break;

		default:
# ifdef STACK
			if( asgop(o) ) s = RTOL;
			else s = LTOR;
# else
			s = DFLT_STRATEGY;
# endif
			break;
		}
	}

# ifndef NODBG
	if(e2debug) {
		printf("bcost(%d(%s),%d(%s),%x), s = ",
			p-node, opst[o], q->stinline, opst[q->op], sha[0] );
		pstrat(s);
		printf( "\n" );
		printf( "\tneeds=%d%s%s\n", n,
			(q->needs&LSHARE)?", LSHARE":"",
			(q->needs&RSHARE)?", RSHARE":"" );
		printf( "\tshape table: (%d %d %d ... )(%d %d %d ... )\n",
			sha[0][0]-shapes, sha[0][1]-shapes, sha[0][2]-shapes,
			sha[1][0]-shapes, sha[1][1]-shapes, sha[1][2]-shapes
			);
		}
# endif

	/* triple loop:
	/* double loop over the left and right sides */
	/* single loop over the number of regs available */

	for( il=0; (sl = sha[0][il]) || il==0; ++il ) {
		lregs = lsubtree = nsubtree = 0;
		c = q->cost;
		lnn = n;
		if( sl ) {
			/* list the left subtrees */
			findsub( l, sl );
			if( l->tn.op==REG && sl->op==REG && asgop(q->op)
				&& !asgop(o) ) {

				/* in an expression such as a+b, where a is */
				/* a register var, copy a to a scratch reg */
				/* before using += to do the add */
				/* this test causes that copy, by suggesting */
				/* that the lhs is a subtree, even though it
				/* matches the template exactly */

				subtree[nsubtree] = l;
				subgoal[nsubtree] = NRGS;
				++nsubtree;
				}
			lsubtree = nsubtree;

			/* account for the cost of the shape */
			c += q->lcount * sl->sc;

			/* count lhs register usage */
			for( lregs=ix=0; ix<lsubtree; ++ix ) {
				if( subgoal[ix] == NRGS ) {
					lregs += szty(subtree[ix]->tn.type);
					}
				}
			if( !(q->needs&LSHARE) ) lnn += lregs;
			}

# ifndef NODBG
		if( e2debug ) {
			printf( "\tbcost left shape: sl=%d(%s), cost=%d\n",
				sl-shapes, sl?opst[sl->op]:"?", c );
			printf( "\t%d left subtrees\n", nsubtree );
			for( j=0; j<nsubtree; ++j ) {
				printf( "\t\tsubtree %d, goal %d\n",
					subtree[j]-node, subgoal[j] );
				}
			}
# endif

		for( ir=0; (sr = sha[1][ir]) || ir==0; ++ir ) {
			ttc = c;
			nsubtree = lsubtree;
			if( sr ) {
				ttc += q->rcount * sr->sc;
				findsub( r, sr );
				}
# ifndef NODBG
			if( e2debug ) {
				printf( "\tbcost rt. shp: sr=%d(%s), cost=%d\n",
					sr-shapes, sr?opst[sr->op]:"?", ttc );
				printf( "\t%d right subtrees\n",
					nsubtree-lsubtree );
				for( j=lsubtree; j<nsubtree; ++j ) {
					printf( "\t\tsubtree %d, goal %d\n",
						subtree[j]-node, subgoal[j] );
					}
				}
# endif

			/* figure out the minimum number of regs. possible */
			for( rregs=0,ix=lsubtree; ix<nsubtree; ++ix ) {
				if( subgoal[ix] == NRGS ) {
					rregs += szty(subtree[ix]->tn.type);
					}
				}

			nn = lnn;
			if( q->needs & RSHARE ) nn -= rregs;
			if( nn < lregs ) nn = lregs;
			nn += rregs;

# ifndef NODBG
			if( e2debug ) {
				printf( "%d left, %d right regs, need >= %d\n",
					lregs, rregs, nn );				
				}
# endif

			for( j=NRGS; j>=nn; --j ) {
# ifndef NODBG
				if( e2debug ) {
					printf( "\t***  j = %d  ***\n", j );
					}
# endif
				/* exact match: don't fool around */
				if( nsubtree==0 ) {
					cc = ttc;
					cs = LTOR;
					goto distribute;
					}
				/* general case: grub around */
				/* LTOR means ascending, RTOL means descending*/

				cc = INFINITY;
				if( s&LTOR ){ /* do it left to right */
					int j1 = j;
					tc = ttc;
					for( ix=0; ix<nsubtree; ++ix ) {
						pp = subtree[ix];
						if( subgoal[ix] == NRGS ){
							/* shouldn't happen */
							if( j1<0 ) tc=INFINITY;
							else tc+=pp->tn.cst[j1];
							j1 -= szty(pp->tn.type);
							}
						else tc +=
							pp->tn.cst[subgoal[ix]];
						}
					cc = tc;
					cs = LTOR;
					}
				if( s&RTOL ){ /* do it right to left */
					int j1 = j;
					tc = ttc;
					for( ix=nsubtree-1; ix>=0; --ix ) {
						pp = subtree[ix];
						if( subgoal[ix] == NRGS ){
							/* shouldn't happen */
							if( j1<0 ) tc=INFINITY;
							else tc+=pp->tn.cst[j1];
							j1 -= szty(pp->tn.type);
							}
						else tc +=
							pp->tn.cst[subgoal[ix]];
						}
					if( tc < cc ){
						cc = tc;
						cs = RTOL;
						}
					}
				if( cc >= INFINITY ) break; /* done */

				/* now, cc is the minmal cost with j regs */
				/* update the various cost measures */
				/* everything affects CEFF */
			distribute:
# ifndef NODBG
				if( e2debug ) {
					printf( "\tdistribute %d\n", cc );
					}
# endif
				if( cc < p->tn.cst[CEFF] ) {
					GETS(EFF,cc);
					p->tn.cst[CEFF] = cc;
					strbc[CEFF] = cs;
					lshbc[CEFF] = sl;
					rshbc[CEFF] = sr;
					}
				/* for EFF, only do with NRGS */
				if( p->tn.goal == CEFF ) break;
				if( res == RNULL || res == RNOP ){
					/* affects only CEFF */
					cerror( "RNULL/RNOP error" );
					}

				if( (p->tn.goal==CCC) && (res&RESCC) ) {
					/* CC's set */
					if( cc < p->tn.cst[CCC] ) {
						GETS(CC,cc);
						p->tn.cst[CCC] = cc;
						strbc[CCC] = cs;
						lshbc[CCC] = sl;
						rshbc[CCC] = sr;
						}
					}


				/* now, the register cost */
				tc = cc;
				if( (res&RLEFT) && sl->op != REG ){
					cc += CLOAD(q);
					}
				else if( (res&RRIGHT) && sr->op != REG ){
					cc += CLOAD(q);
					}
				if( cc < p->tn.cst[j] ) {
					GETSN(j,cc);
					p->tn.cst[j] = cc;
					strbc[j] = cs;
					lshbc[j] = sl;
					rshbc[j] = sr;
					}

				/* for CC's, only do w. NRGS */
				if( p->tn.goal == CCC ) break;

				if( j != NRGS ) continue;

				/* record if lhs is actually a temp */
				/* need only do for NRGS */

				if( tempok(p) && tc < p->tn.cst[CTEMP] ) {
					GETS(TEMP,tc);
					p->tn.cst[CTEMP] = tc;
					strbc[CTEMP] = cs;
					lshbc[CTEMP] = sl;
					rshbc[CTEMP] = sr;
					}
				}
			}
		}

	/* now, some global cleanup */
	/* some things are worth updating only once per template */

	if( p->tn.goal == CEFF ) return;  /* done */

	/* set Condition Codes by testing a register */
	if( p->tn.goal == CCC ) {
		tc = p->tn.cst[NRGS]+CCTEST(q);
		if( tc < p->tn.cst[CCC] ) {
			GETS(CC,tc);
			p->tn.cst[CCC] = tc;
			strbc[CCC] = strbc[NRGS];
			lshbc[CCC] = lshbc[NRGS];
			rshbc[CCC] = rshbc[NRGS];
			}
		return;
		}

	/* put into TEMP by putting into REG, then storing */
	/* if the lhs type is OK and we have an assignment op, don't
	/* need to store: just use the result from EFF */

	if( asgop(o) && o!=INCR && o!= DECR && lhsok( l ) &&
			p->tn.type == l->tn.type ) {
		cc = p->tn.cst[CEFF];
		if( cc < p->tn.cst[CTEMP] ) {
			GETS(CTEMP,cc);
			p->tn.cst[CTEMP] = cc;
			strbc[CTEMP] = strbc[CEFF];
			lshbc[CTEMP] = lshbc[CEFF];
			rshbc[CTEMP] = rshbc[CEFF];
		}
	}
	tc = p->tn.cst[NRGS] + CSTORE(q);

	if( tc < p->tn.cst[CTEMP] ) {
		GETS(TEMP,tc);
		p->tn.cst[CTEMP] = tc;
		strbc[CTEMP] = strbc[NRGS];
		lshbc[CTEMP] = lshbc[NRGS];
		rshbc[CTEMP] = rshbc[NRGS];
		}

	/* compute with few regs by storing, then loading */

	tc = p->tn.cst[CTEMP] + CLOAD(q);

	for( j=1; j<NRGS; ++j ) {
		if( tc < p->tn.cst[j] ) {
			p->tn.cst[j] = tc;
			GETSN(j,tc);
			strbc[j] = strbc[CTEMP] | STORE;
			lshbc[j] = lshbc[CTEMP];
			rshbc[j] = rshbc[CTEMP];
			}
		}
	}

lhsok( p )
NODE *p;
{
	/* p appears on the lhs of an assignment op */
	/* is it an OK substitute for a TEMP? */

	switch( p->tn.op ) {

	case NAME:
	case VAUTO:
	case VPARAM:
	case TEMP:
	case REG:
		return( 1 );

	}
	return( 0 );
}

shpr(sp) register SHAPE *sp; {
	if (!sp) return;
	if( sp->op < 0 || sp->op > DSIZE ) cerror( "shape op %d\n", sp->op );
	printf(" %s", opst[sp->op]);
	shpr(sp->sl);
	shpr(sp->sr);
	}

pstrat( s ) {
	/* print a nice version of the strategy s */
	register i, flag;
	static char *stratnames[] = {
		"STORE",
		"LTOR",
		"RTOL",
		0 };
	flag = 0;
	for( i=0; stratnames[i]; ++i ){
		if( s & (1<<i) ) {
			if( flag ) putchar( '|' );
			printf( "%s", stratnames[i] );
			flag = 1;
			}
		}
	if( !flag ) printf( "0" );
	}

insout( p, i )
NODE *p;
{
	OPTAB *q;
	int c, o, j;

	/* generate the actual instructions */
	/* if the cost is infinite, try rewriting */

	c = p->in.cst[i];
	o = p->tn.op;

#ifndef NODBG
	if( odebug>1 ) printf( "insout(%d,%d), cost %d\n", p-node,i,c );
#endif
	if( c >= INFINITY ){
		cerror( "missing table entry, op %s", opst[p->tn.op] );
	}

	/* handle COMOP specially */
	if( o == COMOP ) {
		q = match( p, (OPTAB *)0 );  /* had better match */
		if( !q ) cerror( "COMOP match fails" );
		bprt( p, q, i );
		return;
	}

	/* want to force bcost to do some work */
	/* this is because the strbc, etc., arrays, set by bcost, are used
	/* by bprt */

	for( j=0; j<NCOSTS; ++j ) ++p->in.cst[j];
	for( q=0; q = match( p, q ); ){
		if( i != CEFF ) {
			if( q->rewrite & RLEFT ) restrip( sha[0] );
			if( q->rewrite & RRIGHT ) restrip( sha[1] );
		}
		bcost( p, q );
		if( p->tn.cst[i] == c ) {  /* we have found it */
			if( strbc[i]&STORE ) bprt( p, q, CTEMP );
			else bprt( p, q, i );
			return;
		}
	}

	/* commuting must be in order here */
	/* if fast flag is on, we can only fail, but it's ok to try */

	if( o != PLUS  &&  o != MUL  &&  o != AND  &&  o != OR  &&  o != ER ) {
		e2print( p );
		cerror( "commute??, op[%d] == %s", o, opst[o] );
	}
	commute( p );  /* this is the payoff; don't need to commute back */
	for( q=0; q = match( p, q ); ){
		if( i != CEFF ) {
			if( q->rewrite & RLEFT ) restrip( sha[0] );
			if( q->rewrite & RRIGHT ) restrip( sha[1] );
		}
		bcost( p, q );
		if( p->tn.cst[i] == c ) { /* we found it */
			bprt( p, q, i );
			return;
		}
	}

	cerror( "insout returns without a match" );
	/* NOTREACHED */

}

bprt( p, q, i )
NODE *p;
OPTAB *q;
{
	/* this routine is called to print out the actual instructions */
	/* it is called with a tree node p, a template q, and a goal i */
	/* bprt calls bcost, and then captures the left and right shapes */
	/* it then uses findsub to determine the preconditions and goals */
	/* a local copy of this information must be made, since bprt can be
	/* called recursively */
	/* then, bprt calls insout to output the instructions that establish
	/* the preconditions.  Finally, it can output its own instruction */

	int j, j1, s, o, k;
	NODE *l, *r;
	SHAPE *ls, *rs;
	int nn;
	int mygoal[NSUBTREES];
	NODE *mysubs[NSUBTREES];

	/* sets j as well */
	if( i < NRGS ) j = i;
	else j = NRGS;
	l = getl( p );
	r = getr( p );
	if (q->rshape && !q->lshape)
		r = p;
	s = strbc[i];
	ls = lshbc[i];
	rs = rshbc[i];
	o = p->tn.op;
# ifndef NODBG
	if( odebug>1 ) {
		printf( "	matches %d, ls = %d(%s), rs = %d(%s),  s= ",
			q->stinline, ls-shapes, ls?opst[ls->op]:"SHNL",
			rs-shapes, rs?opst[rs->op]:"SHNL" );
		pstrat( s );
		printf( "\n" );
		}
# endif

	/* handle COMOP differently; this has more to do with the register
	/* allocation than the ordering */

	if( o == COMOP ) {
		insout( l, CEFF );
		insout( r, i );
		goto generate;
	}

	nsubtree = 0;
	if(rs && (s&RTOL) ) findsub( r, rs );
	if( ls ) {
		findsub( l, ls );
		if( l->tn.op==REG && ls->op==REG && asgop(q->op) &&
				!asgop(o) ) {
			/* we must arrange to copy a reg variable on the lhs
			/* of a binary op, in some cases (cf. bcost) */
			subtree[nsubtree] = l;
			subgoal[nsubtree] = NRGS;
			++nsubtree;
			}
		}
	if(rs && (s&LTOR) ) findsub( r, rs );
	nn = nsubtree;

	/* make a local copy */
	for( k=0; k<nn; ++k ) {
		mygoal[k] = subgoal[k];
		mysubs[k] = subtree[k];
		}

# ifndef NODBG
	if( odebug>1 ) {  /* subtree matches are: */
		printf( "\t\t%d matches\n", nn );
		for( k=0; k<nn; ++k ) {
			printf( "\t\tnode %d, goal %d\n",mysubs[k]-node,
				mygoal[k] );
			}
		}
# endif

	/* do the subtrees */
	/* someday, rewrite the temps right here and now */

	j1 = j;
	for( k=0; k<nn; ++k ) {
# ifndef NODBG
		if( odebug>2 )
			printf( "\t\tcalling insout(%d,%d)\n", mysubs[k]-node,
					j1 );
# endif
		if( mygoal[k] == NRGS ) {
			insout( mysubs[k], j1 );
			j1 -= szty( mysubs[k]->tn.type );
			}
		else {
			insout( mysubs[k], mygoal[k] );
			}		
		}
	/* put onto the instruction string the info about the instruction */
    generate:
	if( nins >= NINS ) cerror( "too many instructions generated" );
	inst[nins].p = p;
	inst[nins].q = q;
	inst[nins].goal = i;
	/* a special case: REG op= xxx, should be done as early as possible */
	if( asgop(o) && p->in.left->tn.op == REG && o != INCR && o != DECR
			&& i!=CEFF && i!=CCC && !istreg(p->in.left->tn.rval)){
		/* "istreg" guards against rewriting returns, switches, etc. */
		inst[nins].goal = CTEMP;
	}
	++nins;
}

findsub( p, s )
NODE *p;
SHAPE *s;
{
	/* account for the costs of matching the shape s with the tree j */

	if( !s )
		return;

# ifndef NODBG
	if( e2debug>1 ) {
		printf( "\t\tfindsub( %d, %d )\n", p-node, s-shapes );
		}
# endif

	switch( s->op ) {

	case TEMP:
		/* leave j unchanged */
		if( p->tn.op == TEMP ) return;
		subtree[nsubtree] = p;
		subgoal[nsubtree] = CTEMP;
		++nsubtree;
		return;

	case FREE:
		subtree[nsubtree] = p;
		subgoal[nsubtree] = CEFF;
		++nsubtree;
		return;

	case CCODES:
		subtree[nsubtree] = p;
		subgoal[nsubtree] = CCC;
		++nsubtree;
		return;

	case REG:
		if( p->tn.op == REG ) return;  /* exact match */

		/* in general, look beneath */
		/* also, look here if a REG and rcst is 1 */

		subtree[nsubtree] = p;
		subgoal[nsubtree] = NRGS;
		++nsubtree;
		return;
		}

	if( s->op == p->tn.op ) {

		/* look at subtrees */
		if( s->sl ) findsub( getl(p), s->sl );
		if( s->sr ) findsub( getr(p), s->sr );
		return;
		}
	}

costs( p ) register NODE *p; {
	register OPTAB *q;
	int i, o, ty;
	register *pc;

	/* compute the costs for p */
	/* the goal is either NRGS (into a reg. or temp), CCC, or CEFF */

	/* in a stack machine, this will probably look very different.
	/* it is possible that seting szty() to be 0 will deal with the
	/* stack machine problems; if not, we will need to put some special
	/* code in here under control of ifdef STACK
	/* the stack machine issue is that the "register" use on the left does
	/* not limit the computations on the right, and conversely */
    again:
	ty = optype( o = p->tn.op );
	if (ty == LTYPE && get_leaf(p) ) return(0);
	pc = p->in.cst;
	for( i=0; i<NCOSTS; ++i ) {
		pc[i] = INFINITY;
		strbc[i] = 0;
		lshbc[i] = rshbc[i] = (SHAPE *)0;
	}

# ifndef NODBG
	if( udebug ) {
		printf( "costs( %d, %d ), op = %s\n", p-node,
			p->tn.goal, opst[o] );
		}
# endif

	if( ty != LTYPE ) if( costs( p->in.left ) ) return(1);
	if( ty == BITYPE ) if( costs( p->in.right ) ) return(1);

	pc = p->in.cst;

	/* now, compute the costs based on matches */
	/* handle COMOP specially */
	if( o == COMOP ) {
		int cc = p->in.left->in.cst[CEFF];
		for( i=NRGS; i<NCOSTS; ++i ) {
			pc[i] = cc + p->in.right->in.cst[i];
			if( pc[i] > INFINITY ) pc[i] = INFINITY;
		}
		return(0);
	}

	for( q=0; q = match(p,q); ){
		if( p->tn.goal != CEFF ) {
			if( q->rewrite & RLEFT ) restrip( sha[0] );
			if( q->rewrite & RRIGHT ) restrip( sha[1] );
		}
		bcost( p, q );
# ifndef NODBG
		if( udebug ) {
			printf( "bcost( %d, %d )\n", p-node, q->stinline);
			e222print( 1, p, "T" );
		}
# endif
	}

#ifndef NOCOMMUTE
	/* don't commute if we are trying to be fast */
	if( !fast && (o==PLUS||o==MUL||o==AND||o==OR||o==ER) ){
# ifndef NODBG
		if( udebug ) {
			printf( "COMMUTE %d *******\n", p-node );
		}
# endif
		commute( p );
		for( q=0; q = match(p,q); ){
		if( p->tn.goal != CEFF ) {
			if( q->rewrite & RLEFT ) restrip( sha[0] );
			if( q->rewrite & RRIGHT ) restrip( sha[1] );
		}
			bcost( p, q );
# ifndef NODBG
			if( udebug ) {
				printf( "bcost( %d, %d )\n", p-node, q->stinline);
				e222print( 1, p, "T" );
			}
# endif
		}
		commute( p );
# ifndef NODBG
		if( udebug ) {
			printf( "END OF COMMUTE %d *******\n", p-node );
		}
# endif
		}

/*	END OF COMMUTE CODE *****  */
# endif

	/* here is a big worry; when do we do this rewriting?
	/* if we do it too early, we may miss some neat possibilities */
	/* if we do it too late, we may have miscomputed some earlier things */

	if( pc[p->tn.goal]>=INFINITY ){
		if( p->fn.type == TSTRUCT ) return(0);
		if( optype( o ) == LTYPE ) return( 0 );
		if( rewass( p ) ) return( 1 );  /* major rewrite */
		goto again;  /* minor rewrite: restart here */
		}
	if (ty == LTYPE)
		save_leaf(p);
	return( 0 );
	}

/* static		let me profile */
get_leaf(p)
register NODE *p;
{
	register struct leaf *lf;
	register unsigned long key =
	    (p->tn.goal<<26) | (p->tn.type<<9) | p->tn.op;
			/*see if this leaf/type/goal triple is in the tables*/
	if (p->tn.op == ICON) return (0);	/* no ICONs here */
	recost_leaf=0;
	for (lf=leafcosts;lf < leaf_ptr; lf++)
	{
/*		if (lf->op == p->tn.op && lf->type == p->tn.type &&
/*		    lf->goal == p->tn.goal)
*/
		if (lf->key == key)
		{
			/*if the saved cost was infinite, we will have
			  to recost the leaf anyway*/
			if (lf->cst[p->tn.goal] >= INFINITY)
			{
				recost_leaf = lf;
				return(0);
			}
			/*else, load the costs and leave*/
			memcpy(p->tn.cst,lf->cst,sizeof(int)*NCOSTS);
			return(1);
		}
	} /*end for*/
	return(0);
} /*end get_leaf*/

/* static	let me profile */
save_leaf(p)
register NODE *p;
{
		/*save the costs of this leaf for future reference.
		  if recost_leaf is non-zero, it is already in the table*/
		/* have to be careful with constants: different constant
		   shapes may have different costs and we'll be unable
		   to find a template that gives us the purported cost */
		/* for now, just chuck ICONs */
		/* I also seem to be getting hung up on changing costs
		   based on evaluation context. Let's see if adding the
		   goal to the shape helps */
	register struct leaf *lf;

	if ( !recost_leaf && (leaf_ptr >= leafcosts + LSAVED) ) return;
	if (p->tn.op == ICON) return;		/* sorry */
	lf = recost_leaf ? recost_leaf : leaf_ptr++;
/*	lf->op = p->tn.op;
/*	lf->type = p->tn.type;
/*	lf->goal = p->tn.goal;
*/
	lf->key = (p->tn.goal<<26) | (p->tn.type<<9) | p->tn.op;
	memcpy(lf->cst,p->tn.cst,sizeof(int)*NCOSTS);
	recost_leaf=0;
}

