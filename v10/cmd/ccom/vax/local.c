# include <stdio.h>
#include "signal.h"
extern int Pflag, bbcnt;
# include "mfile1.h"

int minrvar = 11;
int wloop_level = LL_BOT;
int floop_level = LL_BOT;
int maxboff;
int maxtemp;

dontdump()
{
	write(2, "giving up\n", 10);
	cerror("internal error");
	fflush(stderr);
	_exit(1);
}

fperr()
{
	cerror("floating point error");
	(void) signal(SIGFPE, fperr);
} 

main( argc, argv ) char *argv[];
{
	int r; char errbuf[BUFSIZ];
	(void) signal(SIGBUS, dontdump);
	(void) signal(SIGSEGV, dontdump);
	(void) signal(SIGFPE, fperr);
	setbuf(stderr, errbuf);
	r = mainp1( argc, argv );
	flushx();
	return( r );
}

beg_file()	/* only used in cgram.y */
{
	/* called as the very first thing by the parser to do machine
	 * dependent stuff
	 */
	regvar = minrvar;
	dbfile(NULL);
}

NODE *
treecpy(p)		/* first pass version of tcopy() */
	register NODE *p; 
{
	/* make a fresh copy of p */
	register NODE *q;

	q = talloc();
	*q = *p;
	switch (optype(q->in.op))
	{
	case BITYPE:
		q->in.right = treecpy(p->in.right);
	case UTYPE:
		q->in.left = treecpy(p->in.left);
	}
	return (q);
}

NODE *
clocal(p) NODE *p;	/* optim.c and trees.c */
{
	register NODE *l,*ll,*r;
	if( p->in.op == STAR )
	{	/* if it looks like index mode put the	*/
		/* offset on the right */
		l = p->in.left;
		if( l->in.op == PLUS )
		{
			ll = l->in.left;
			if( ll->in.op != MUL && ll->in.op != UNARY AND )
			{
				if( (l->in.right)->in.op == MUL )
				{
					r = l->in.right;
					l->in.right = l->in.left;
					l->in.left = r;
				}
			}
		}
	}
#ifdef DASSOVCOL
	if (!asgbinop(p->in.op) && p->in.op != ASSIGN)
		return (p);
	r = p->in.right;
	if (optype(r->in.op) == LTYPE)
		return (p);
	l = r->in.left;
	if (r->in.op == QUEST || (r->in.op == CONV && l->in.op == QUEST) ||
		(r->in.op == CONV && l->in.op == CONV &&
		l->in.left->in.op == QUEST))
				/* distribute assigns over colons */
	{
		register NODE *pwork;
		NODE *pcpy = treecpy(p), *pnew;
#ifndef NODBG
		extern int xdebug, eprint();

		if (xdebug)
		{
			puts("Entering [op]=?: distribution");
			eprint(p);
		}
#endif
		pnew = pcpy->in.right;
		while (pnew->in.op != QUEST)
			pnew = pnew->in.left;
		/*
		* pnew is top of new tree
		*/
		if ((pwork = p)->in.right->in.op == QUEST)
		{
			tfree(pwork->in.right);
			pwork->in.right = pnew->in.right->in.left;
			pnew->in.right->in.left = pwork;
			/* at this point, 1/2 distributed. Tree looks like:
			*		ASSIGN|ASGOP
			*	LVAL			QUEST
			*		EXPR1		COLON
			*			ASSIGN|ASGOP	EXPR3
			*		LVAL		EXPR2
			* pnew "holds" new tree from QUEST node
			*/
		}
		else
		{
			NODE *pholdtop = pwork;

			pwork = pwork->in.right;
			while (pwork->in.left->in.op != QUEST)
				pwork = pwork->in.left;
			tfree(pwork->in.left);
			pwork->in.left = pnew->in.right->in.left;
			pnew->in.right->in.left = pholdtop;
			/* at this point, 1/2 distributed. Tree looks like:
			*		ASSIGN|ASGOP
			*	LVAL			ANY # OF CONVs
			*			QUEST
			*		EXPR1		COLON
			*			ASSIGN|ASGOP	EXPR3
			*		LVAL		ANY # OF CONVs
			*			EXPR2
			* pnew "holds" new tree from QUEST node
			*/
		}
		if ((pwork = pcpy)->in.right->in.op == QUEST)
		{
			pwork->in.right = pnew->in.right->in.right;
			pnew->in.right->in.right = pwork;
			/*
			* done with the easy case
			*/
		}
		else
		{
			NODE *pholdtop = pwork;

			pwork = pwork->in.right;
			while (pwork->in.left->in.op != QUEST)
				pwork = pwork->in.left;
			pwork->in.left = pnew->in.right->in.right;
			pnew->in.right->in.right = pholdtop;
			/*
			* done with the CONVs case
			*/
		}
		p = pnew;
#ifndef NODBG
		if (xdebug)
		{
			puts("Leaving [op]=?: distribution");
			eprint(p);
		}
#endif
	}
#endif
	return(p);
}

cisreg( t ) TWORD t;	/* pftn.c */
{ /* is an automatic variable of type t OK for a register variable */

	if( t==INT || t==UNSIGNED || ISPTR(t) || t==CHAR || t==UCHAR
		   || t==SHORT || t==USHORT || t==FLOAT
		   /* (sigh) || t==STRTY || t == UNIONTY*/ )
	{
		if( regvar >= 6 )
		{
			nextrvar = regvar--;
			if( regvar < minrvar ) minrvar = regvar;
			return(1);
		}
	}
	if(t == DOUBLE && regvar >= 7) {
		nextrvar = --regvar;
		regvar--;
		if(regvar < minrvar)
			minrvar = regvar;
		return(1);
	}
	return(0);
}

opbigsz( op )
{
	/* the size below which we do not shrink ops */
	switch( op )
	{

	default:
		return( SZINT );

	case PLUS:
	case MINUS:
	case OR:
	case AND:
	case ER:
	case COMPL:
	case UNARY MINUS:
		return( SZCHAR );

	}
}

branch(n)	/* cgram.y */	/* branch to label n or return */
int n;
{
	if (!reached)			/* return <expr>; } comes here 2x */
		return;
	genubr(n);
}

	/* direct switch beginning */
static int tablelabel;

struct sw heapsw[SWITSZ];	/* heap for switches */

	/* test for whether to do a direct switch */
# ifndef DSWTEST
# define DSWTEST(r,n) (r>0 && r<9*n &&  n>=2)	/* pjw, faster */
# endif
	/* test for whether to do a heap switch */
# ifndef HEAPTEST
# define HEAPTEST( n ) (n>8)
# endif

genswitch(p,n) register struct sw *p;
{
	/*	p points to an array of structures, each consisting
		of a constant value and a label.
		The first is >=0 if there is a default label;
		its value is the label number
		The entries p[1] to p[n] are the nontrivial cases
		*/
	register i;
	register CONSZ j, range;
	register dlab, swlab;

	range = p[n].sval-p[1].sval;

	if( DSWTEST( range, n ) )
	{	/* implement a direct switch */

		swlab = getlab();
		dlab = ((p->slab >= 0) ? p->slab : getlab());

		dswbegin( n, p[1].sval, range, swlab, dlab );

		for( i=1,j=p[1].sval; i<=n; j++)
		{
			if( j == p[i].sval )
			{
				dswcase( p[i].slab );
				j = p[i++].sval;
			}
			else
			{
				dswcase( dlab );
			}
		}

		/* in case dswbegin changed location counters... */
		locctr( PROG );

		if( p->slab >= 0 ) genubr( dlab );
		else deflab( dlab );
		return;
	}

	if( HEAPTEST(n) )
	{	/* heap switch */

		heapsw[0].slab = dlab = (p->slab >= 0 ? p->slab : getlab());
		makeheap(p, n, 1);	/* build heap */
		walkheap(1, n);		/* produce code */

		if( p->slab >= 0 )
			genubr( dlab );
		else
			deflab( dlab );
		return;
	}

	/* simple switch code */

	for( i=1; i<=n; ++i ) sswtest( p[i].sval, p[i].slab );
	if( p->slab>=0 ) genubr( p->slab );

	}

makeheap(p, m, n)
register struct sw *p;
{
	register int q;

	q = select(m);
	heapsw[n] = p[q];
	if( q>1 ) makeheap(p, q-1, 2*n);
	if( q<m ) makeheap(p+q, m-q, 2*n+1);
}

select(m) {
	register int l,i,k;

	for(i=1; ; i*=2)
		if( (i-1) > m ) break;
	l = ((k = i/2 - 1) + 1)/2;
	return( l + (m-k < l ? m-k : l));
}

walkheap(start, limit)
{
	int label;


	if( start > limit ) return;
	sswtest( heapsw[start].sval, heapsw[start].slab );
	if( (2*start) > limit ) {
		genubr( heapsw[0].slab );
		return;
	}
	if( (2*start+1) <= limit ) {
		label = getlab();
		hswelse( label );
	} else
		hswelse( heapsw[0].slab );
	walkheap( 2*start, limit);
	if( (2*start+1) <= limit ) {
		deflab( label );
		walkheap( 2*start+1, limit);
	}
}


dswbegin( numb, first, range, labl, dlab )
CONSZ first, range;
int numb, labl, dlab;
{
	printx("	casel	r0,$%ld,$%ld\n", first, range );
	printx("L%d:\n", labl );
	tablelabel = labl;
}

dswcase( l )
int l;
{
	printx("	.word	L%d-L%d\n", l, tablelabel );
}

sswtest( val, lab )
CONSZ val;
int lab;
{
	printx( "	cmpl	r0,$%ld\n	jeql	L%d\n", val, lab );
}

hswelse( lab )
int lab;
{
	printx("	jgtr	L%d\n", lab );
}

OFFSZ inoff;		/* size of offset in structure */

static inwd;		/* current bit offset in word */
static long word;	/* word being built from fields */

zecode( n )
int n;
{
	/* n integer words of zeros */
	if (n <= 0) return;
	printx( "	.space	%d\n", 4*n );
	inoff += n*SZINT;
}

vfdzero( n ){ /* define n bits of zeros in a vfd */

	/* this could be done more cleverly: the following is safe */

	sz_incode( (CONSZ)0, n );
	}

incode (p, sz)
NODE *p;
{
	sz_incode(p->tn.lval, sz);
}

sz_incode( val, sz )
CONSZ val;
{

	/* generate initialization code for assigning a constant c
		to a field of width sz */
	/* we assume that the proper alignment has been obtained */
	/* inoff is updated to have the proper final value */

	if((sz+inwd) > SZLONG) cerror("incode: field > long");

	/* this code will have to be replaced if the size of a long on
	/* the target machine differs from that on the host machine */

# ifdef RTOLBYTES
	word |= ((unsigned)(val<<(SZLONG-sz))) >> (SZLONG-sz-inwd);
# else
	word |= ((unsigned)(val<<(SZLONG-sz))) >> inwd;
# endif
	inwd += sz;
	inoff += sz;

	/* if initialization can be carried out using shorts, do it */
# if (SZSHORT >= ALINIT)
	if(inwd == SZSHORT )
	{
# ifdef RTOLBYTES
	genshort( (short) word );
# else
	genshort( (short) (word>>(SZLONG-SZSHORT)) );
# endif
	word = inwd = 0;
	} else 
# endif
	if( inwd == SZLONG )
	{
		genlong( word );
		word = inwd = 0;
	}
}

fincode( d, sz )
double d;
int sz;
{
	/* output code to initialize space of size sz to the value d */
	/* the proper alignment has been obtained */
	/* on the target machine, write it out in hex! */

#if defined(vax)
	union { float f; double d; int i[2] } cheat;

	if (sz == SZDOUBLE)
	{
		cheat.d = d;
		printx("\t.long\t0x%x,0x%x\t# %.20e\n", cheat.i[0], cheat.i[1],
		    cheat.d);
	}
	else
	{
		cheat.f = d;
		printx("\t.long\t0x%x\t# %.20e\n", cheat.i[0], cheat.f);
	}
#else
	printx("	%s	0%c%.20e\n",
		sz == SZDOUBLE ? ".double" : ".float",
		sz == SZDOUBLE ? 'd' : 'f', d);
#endif
	inoff += sz;

}

int ftlab1, ftlab2;
int proflag;

int ent_mask[] = {
	0,0,0,0,0, 0xfc0, 0xf80, 0xf00, 0xe00, 0xc00, 0x800, 0};

efcode()
{
	/* code for the end of a function */
	long spoff;	/* offset from stack pointer */

	genret( strftn, strftn, retlab );
	printx( "	.set	L.R%d,0x%x\n", ftnno, ent_mask[minrvar] );

#ifdef FORT
	spoff = maxboff;
	if( spoff >= BITOOR(AUTOINIT) ) spoff -= BITOOR(AUTOINIT);
	spoff += maxtemp;
	spoff /= SZCHAR;
	printx( "	.set	L.F%d,%ld\n", ftnno, spoff / SZCHAR );
#else
	spoff = maxboff;
	if( spoff >= BITOOR(AUTOINIT) ) spoff -= BITOOR(AUTOINIT);
	spoff += maxtemp;
	spoff /= SZCHAR;
	printx("\t.set\tL.SO%d,0x%x\n", ftnno, spoff);
#endif
	regvar = minrvar = 11;
#ifdef GDEBUG
	dbfunend(getlab());
#endif
}

bfcode( a, n )	/* used only in pftn.c */
int a[], n;
{
	/* code for the beginning of a function; a is an array of
		indices in stab for the arguments; n is the number */
	register i;

	/* routine prolog */

	printx( "	.word	L.R%d\n", ftnno);
	printx("\tsubl2\t$L.SO%d,sp\n", ftnno);

	retlab = getlab();

	if( proflag )
	{	/* profile code */
		i = getlab();
		printx("	movab	L%d,r0\n", i);
		printx("	jsb 	mcount\n");
		printx("	.data\n");
		printx("	.align	2\n");
		printx("L%d:	.long	0\n", i);
		printx("	.text\n");
	}
	if(Pflag) {
		printx("\t.data\n\t.comm _proFptr,4\n\t.text\n");
		printx("\ttstl locprof+4\n\tbneq L%da\n", ++bbcnt);
		printx("\tmovl _proFptr,locprof+4\n\tmoval locprof,_proFptr\n");
		printx("#entry %d\n", bbcnt);
		printf("L%da:\tincl locprof+%d\n", bbcnt, 4*(bbcnt+3));
	}
#ifdef GDEBUG
	dbfunbeg(&stab[curftn]);
	for (i = 0; i < n; ++i) {
		extern TWORD argty[];
		extern int argsoff[];
		struct symtab q;
		q = stab[a[i]];
		q.sclass = PARAM;
		q.stype = argty[i];
		q.offset = argsoff[i];
		dbfunarg(&q);
	}
#endif
}

defnam( psym )
register struct symtab *psym;
{
	/* define the current location as the name psym->sname
	 * first give the debugging info for external definitions
	 */
	/*if( psym->slevel == 0 )	/* make sure it's external */
	/*	ISFTN(psym->stype) ? prdef(psym,0) : prdef(psym,dsflag);
	*/

	if (psym->sclass == EXTDEF)
		printx( "	.globl	%s\n", exname(psym->sname) );
	printx("%s:\n", exname(psym->sname));
}

sretname(n)	/* pftn.c (SRETNAME), generate .lcomm for struct return */
{	int i;
	i = (n + SZINT-1)/SZINT;	/* words */
	printx("\t.lcomm\tL%d,%d\n", strftn = getlab(), i * SZINT/SZCHAR);
}

commdec(id)	/* pftn.c, generate a .comm from stab index id */
int id;
{
	register struct symtab *psym;
	OFFSZ n;

	psym = &stab[id];
	psym->sflags |= SBSS;
	n = tsize(psym->stype, psym->dimoff, psym->sizoff) / SZCHAR;
	if (psym->sclass == STATIC)
		if (psym->slevel)
			printx("	.lcomm	L%d,%ld\n", psym->offset, n);
		else	
			printx("	.lcomm	%s,%ld\n", exname(psym->sname), n);
	else if (psym->sclass == EXTERN)
		printx("	.comm	%s,%ld\n", exname(psym->sname), n);

	else
		cerror("Non-static/external in common");
}

myfcon(p)
NODE *p;
{
	union { double d; int i[2]; } u;

	u.d = p->fpn.dval;
	if (u.i[1] == 0)		/* no significant lo bits, shorten */
	{
		p->fn.type = FLOAT;
		p->fn.csiz = FLOAT;
	}
}

e2print() {cerror("e2print called");}
t2print() {cerror("t2print called");}
