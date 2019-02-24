/* @(#) func.c: 1.4 4/28/84				*/

/* func.c
**
**	Function improvement routines
**
**
**	This file contains routines that require knowledge of a whole
**	function or more to perform their optimizations.
*/

#include "optim.h"
#include "optutil.h"
#include "storclass.h"
#include <string.h>
#include "paths.h"

/* Macro to add new instruction:
**	opn	op code number of new instruction
**	opst	op code string of new instruction
**	opn1	operand 1 for new instruction
**	opn2	operand 2 for new instruction
*/

#define addi(opn,opst,opn1,opn2) \
	{ \
		pn = insert( pn );		/* get new node */ \
		chgop( pn, opn, opst );		/* put in opcode num, str */ \
		pn->op1 = opn1;			/* put in operands */ \
		pn->op2 = opn2;			\
	}
#define opm 	ops[MAXOPS]

#ifdef IMPREGAL
/* register allocation optimization
**
** F. B. Wolverton 6/16/83
**
** This optimization accepts a list of autos and args from the
** compiler and assigns them to registers if there is sufficient estimated
** payoff.  
**
** Scratch registers are used first, if they are available, 
** because they are "free".  A variable can be put into a scratch register if
** 	(a) the register is not used by any instruction in the procedure,
**	(b) the variable is dead, i.e., not used before being set, 
**          following any call in the procedure
** 
** Register 3 is considered a scratch register, if IMPSCRATCH is defined.
**
** Next registers r8 through r4 are assigned (and r3, if it has not been used
** as a scratch register).
**
** Register variables assigned by the compiler are assigned first.  A variable
** the compiler assigned to a user register may get assigned to a scratch 
** register by the optimizer.
** 
** Variables in the list of autos and args are assigned next.
** 
** The cumulative cost for puting all variables up to a given variable
** into registers is computed and used to select a cutoff.
** All register variables assigned by the compiler remain in 
** registers.  Other variables are assigned only if the
** estimated payoff os positive.
** 
** Instruction operands are modified to reflect the additional assignments.
**
*/

#define MAXREGS 10
#define MAXVARS 20

/* variable storage classes */

#define SCLNULL 0
#define AUTO 1
#define REGISTER 4
#define PARAM 9

/* register types */

#define SCRATCH 0
#define USER 1

/* register status */

#define AVAIL 0
#define NOTAVAIL 1

/* cycle estimates for BELLMAC-32B instructions */

#define CYCSAVE 39	/* SAVE and RESTORE with 0 user registers */
#define CYCDELTA 5	/* delta to save and restore an additional register */
#define CYCADD 6	/* ADD small literal to stack pointer */
#define CYCMOVS 16	/* MOVW from register to register offset and back */
#define CYCAWORD 7	/* MOVAW of register offset to frame pointer */
#define CYCPUSH 10	/* PUSH a regsiter */
#define CYCPOP 10	/* POPW a value into a register */

struct hinode { /* ordered table of variables with highest estimators */
	int 	hiestim;		/* estimator of cycle payoff */
	int	hiscl;			/* scl of quant to put in reg */
	char*	hiname;			/* name of quantity:
					 * 	local		offset 
					 *	argument	offset*/
	int	hilen;			/* length in bytes of quantity */
} high[MAXVARS];

int vars = 0;		/* number of variables stored in 'high' array */
int nodblflg = 0;	/* flag indicating absence of doubles */

struct assign {
	char *asrname;	/* register name */
	char *asrind;	/* indirect reference through register */
	int asrtype;	/* register type */
	int asrsave;	/* number of user registers saved */
	int asavail;	/* availabilty of register */
	int asestim;	/* estimator of cycle payoff of assigned variable */
	int asvscl;	/* storage class of variable assigned */
	char *asvname;	/* variable name */
	int asvlen;	/* variable lenggth in bytes */
} asreg[MAXREGS] = {
	{ "%r0", "0(%r0)", SCRATCH, 0 },
	{ "%r1", "0(%r1)", SCRATCH, 0 },
	{ "%r2", "0(%r2)", SCRATCH, 0 },
	{ "%r3", "0(%r3)", SCRATCH, 0 },
	{ "%r8", "0(%r8)", USER, 1 },
	{ "%r7", "0(%r7)", USER, 2 },
	{ "%r6", "0(%r6)", USER, 3 },
	{ "%r5", "0(%r5)", USER, 4 },
	{ "%r4", "0(%r4)", USER, 5 },
	{ "%r3", "0(%r3)", USER, 6 }
};

/* Note that register 3 appears twice.  It is used as a scratch register if 
** possible and as a user register otherwise. 
*/

char* rnames[] = { "%r8", "%r7", "%r6", "%r5", "%r4", "%r3" };


/* routine to read in table of variables stored in high[] */

void
ratable( p )
register char *p;

{
	extern int atoi();
	register char *q, *r;

	/* check for table overflow */
	if( vars >= MAXVARS ) return;

	/* scan to estimator and read it */
	while( *p++ != '\t' );
	high[vars].hiestim = atoi( p );

	/* scan to storage class and read it */
	while( *p++ != '\t' );
	switch (*p) {
	case 'A': high[vars].hiscl = AUTO; break;
	case 'P': high[vars].hiscl = PARAM; break;
	/* set doubles flag */
	case 'N': nodblflg = 1; return;
	}

	/* scan to name and read it */
	while( *p++ != '\t' );
	q = p;
	while( *q++ != '\t' );
	high[vars].hiname = r = getspace( q - p );
	while( p < ( q - 1 ) ) *r++ = *p++;
	*r = '\0';

	/* read length */
	high[vars].hilen = atoi(q);

	/* increment table index */
	vars++;
	return;
}


/* main routine for register allocation optimization */

int
raoptim( numnreg, numauto )
register int numnreg;	/* input number of registers saved/restored */
register int numauto; 	/* number of words of automatic variables */

{
	void raavail(), ravassign(), raremove(), raopn(), raparam(), radef();
	void rainsld(), raremld();
	int rarassign(), racut();
	int iascut;	/* index for assignment cutoff */
	int iasrlast;	/* index of the last register variable assigned */
	int newnreg;	/* new number of registers to be saved */
	register int ias;

	/* check for doubles */
	if( nodblflg == 0 ) {
		vars = 0;
		return( numnreg );
	}

	/* initialize assignment table */
	for( ias = 0; ias < MAXREGS; ias++ ) {
		asreg[ias].asavail = AVAIL;
		asreg[ias].asvscl = SCLNULL;
		asreg[ias].asvlen = 0;
	}

	/* determine availability of scratch registers */
	raavail( numnreg );

	/* insert branch pointers for live/dead analysis */
	rainsld();
	
	/* assign register variables */
	iasrlast = rarassign( numnreg );

	/* assign non-register variables */
	ravassign();

	/* remove remove branch pointers from live/dead analysis */
	raremld();

	/* compute cutoff from cumulative payoff */
	iascut = racut( numnreg, numauto, iasrlast );
	if( iascut < 0 ) { /* if no payoff for this optimization */
		vars = 0;
		return( numnreg );	
	}
	newnreg = asreg[iascut].asrsave;

	/* modify the instruction operands */
	raopn( iascut, newnreg );

	/* insert moves for parameters */
	raparam( iascut );

	/* modify def statements */
	radef( iascut );

	/* re-initailize for next procedure */
	vars = 0;
	return( newnreg );
}

/* determine availability of scratch registers */
void
raavail( numnreg )
int numnreg; 	/* input number of registers to be saved/restored */
{
	register NODE *pn;
	register struct assign *pa;
	register int iop, i;
	register char *p;
	struct assign *map[MAXREGS];

	/* check availability of scratch %r3 */
	if( numnreg == 6 ) asreg[3].asavail = NOTAVAIL;
	else asreg[9].asavail = NOTAVAIL;
#ifndef IMPSCRATCH
	asreg[3].asavail = NOTAVAIL;
	asreg[9].asavail = AVAIL;
#endif /* IMPSCRATCH */

	/* set up mapping for scratch registers */
	for( i = 0; i < MAXREGS; i++ ) map[i] = NULL;
	for( i = 0; i < MAXREGS; i++ ) {
		pa = &asreg[i];
		if( pa->asrtype == SCRATCH && pa->asavail == AVAIL )
			map[*( pa-> asrname + 2 ) - '0'] = pa;
	}
	
	/* scan for uses */
	for( ALLN( pn ) ) { /* scan nodes */
		if( pn->op == FILTER ) continue;
		/* check for use of scratch registers */
		for( iop = 1; iop < MAXOPS + 1; iop++ ) { /* scan operands */
			if( ( p = pn->ops[iop] ) == NULL ) continue;
			while( *p != '\0' ) {
				if( *p++ != '%' ) continue; 
				if( *p++ != 'r' ) break;
				pa = map[*p - '0'];
				if( pa != NULL ) pa->asavail = NOTAVAIL;
				break;
			}
		}
	}
}

/* insert pointers for live/dead analysis */
void
rainsld()
{
	register NODE *pn, *qn;
	register char *ppn;

	for( ALLN( pn ) ) {
		if( isbr( pn ) && !isret( pn ) ) {
			ppn = getp( pn );
			/* use wraparound scan when looking for label */
			for( qn = ( pn->forw != &ntail ) ? pn->forw : n0.forw; 
				qn != pn; qn = ( qn->forw != &ntail ) ?
				qn->forw : n0.forw ) {
				if( islabel( qn ) ) {
					if(*(ppn + 2) != *(qn->ops[0] + 2) &&
						*(ppn + 1) != '\0' ) 
						continue; /* for speed */
					if( strcmp( ppn, qn->ops[0] ) == 0 ){
						pn -> opm = (char *) qn;
						break;
					}
				}
			}
		}
	}
}

/* assign register variables */
int
rarassign( numnreg ) 
register int numnreg;	/* input number of registers to be saved/restored */
{
	register struct assign *pa;
	register int ias, ireg, ok;
	int iasrlast;

	iasrlast = -1;

	/* assign to first available register, and mark unavailable  */
	for( ireg = 0; ireg < numnreg; ireg++ ) {
		/* check whether is dead at CALL's etc. */
		ok = raregok( rnames[ireg], 0 );
		for( ias = 0; ias < MAXREGS; ias++ ) {
			pa = &asreg[ias];
			if( pa->asavail == NOTAVAIL ) continue;
			if( pa->asrtype == SCRATCH && !ok ) continue;

			/* assign it */
			pa->asavail = NOTAVAIL;
			pa->asestim = 0;
			pa->asvscl = REGISTER;
			pa->asvname = rnames[ireg];
			pa->asvlen = 4;
			if( iasrlast < ias ) iasrlast = ias;
			break;
		}
	}
	return( iasrlast );
}

/* assign non-register variables */
void
ravassign()
{
	register int ivar, ias, ok;
	register struct assign *pa;
	register struct hinode *ph;

	/* go through variables */
	for( ivar = 0; ivar < vars; ivar++ ) {
		ph = &high[ivar];
		/* check to see if dead at CALL's etc. */
		ok = raregok(ph->hiname, ph->hilen);
		/* assign to first register that qualifies */
		for( ias = 0; ias < MAXREGS; ias++ ) {
			pa = &asreg[ias];
			if( pa->asavail == NOTAVAIL ) continue;
			if( pa->asrtype == SCRATCH && !ok ) continue;

			/* assign it */
			pa->asavail = NOTAVAIL;
			pa->asestim = ph->hiestim;
			pa->asvscl = ph->hiscl;
			pa->asvname = ph->hiname;
			pa->asvlen = ph->hilen;
			break;
		}
	}
}

/* check if dead at CALL's etc. */
int
raregok( name, len )
register char *name;	/* pointer to register or variable name */
register int len;	/* length of variable (0 for register) */
{
	register NODE *pn;
	NODE *rald();

	for( pn = n0.forw; pn != 0; pn = pn->forw ) { /* scan nodes */
		switch( pn->op ) {
		default: continue;
		case CALL:
		case JSB:
		case MOVBLB:
		case MOVBLH:
		case MOVBLW:
			/* check if 'name' is live */
			if( rald( pn->forw, name, len ) != NULL ) return( 0 );
		}
	}
	/* dead */
	return( 1 );
}

/* check for register or variable dead in a block */
/* Note that this routine may visit a block more than
 * once, but that it cannot loop because any closed path
 * in the program contains at least one label, and the
 * routine will not scan a block beginning with a given
 * label more than once. 
 */
NODE *
rald( pn, name, len )
register NODE *pn;	/* pointer to block to be searched */
register char *name;	/* pointer to register or variable name */
register int len;	/* length of variable (0 for register) */
{
	NODE *qn;
	register int iop;
	register char *p;
	int srcsize, dstsize;

	/* scan block */
	for( ; pn != 0; pn = pn->forw ) {
		if( pn->op == FILTER ) continue;
		/*control recursion */
		if( islabel( pn ) ) {
			/* terminate recursion if this block has been visited */
			if( pn->opm == name ) return( NULL );
			/* mark block as visited */
			pn->opm = name;
			continue;
		}
		/* if no references, its dead */
		if( isret( pn ) ) return( NULL );
		/* end of block */
		if( isbr( pn ) ) break;
		/* check operands of remaining instructions */
		for( iop = 1; iop < MAXOPS + 1; iop++ ) {
			p = pn->ops[iop];
			if( p == NULL ) continue;
			/* look for register or variable */
			if( ( *name == '%' && usesreg( p, name ) ) ||
				rausesoff( p, name, len ) ) {
				/* see if it's a destination */
				if( iop==2 && ( length(name) == length(p) )
					&& ismove( pn, &srcsize, &dstsize )) 
					return( NULL );
				if( iop==3 && ( length(name) == length(p) )) {
					switch ( pn->op ) {
					case ANDB3: case ANDH3: case ANDW3:
					case ORB3: case ORH3: case ORW3:
					case XORB3: case XORH3: case XORW3:
					case LLSW3: case LRSW3:
					case ADDB3: case ADDH3: case ADDW3:
					case SUBB3: case SUBH3: case SUBW3:
					case MULW3: case UMULW3:
					case DIVW3: case UDIVW3:
					case MODW3: case UMODW3:
					case ALSW3: case ARSW3:
						return( NULL );
					}
				}
				/* nope, it's live */
				return( pn );
			}
		}
	}
	/* examine next block(s) recursively */
	if( pn->opm == NULL ) return( pn ); /* dst not a simple label */
	
	if( ( qn = rald( (NODE *) pn->opm, name, len ) ) != NULL ) return( qn );
	if( isuncbr( pn ) ) return( NULL );
	qn = rald( pn->forw, name, len );
	return( qn );
}

/* remove branch information from live/dead analysis */
void
raremld()
{
	register NODE *pn;

	for( pn = n0.forw; pn != 0; pn = pn->forw ) {
		if( isbr( pn ) || islabel( pn ) ) pn->opm = NULL;
	}
}

/* compute cutoff from cumulative payoff */
int
racut( numnreg, numauto, iasrlast ) 
int numnreg;	/* input number of registers saved */
int numauto;	/* number of autos */
int iasrlast;	/* index of last assigned register variable */
{
	int rsave(), rsaveo();
	int vcumul;	/* cumulative payoff for variables */
	int rcumul;	/* cumulative payoff for reg save/restore */
	int cumul;	/* cumulative total payoff */
	int paycut;	/* payoff for cutoff register */
	int oldsave;	/* cycles to save and restore */
	int oldsaveo; 	/* cycles to save and restore with func call opt */
	int ias, iascut;
	struct assign *pa;

	vcumul = 0;
	iascut = iasrlast;
	paycut = 0;
	oldsave = rsave( numnreg, numauto );

	for( ias = 0; ias < MAXREGS; ias ++ ) {

		/* skip if no assignment */
		pa = &asreg[ias];
		if( pa->asvscl == SCLNULL ) continue;

		/* compute cumulative variable payoff */
		vcumul += pa->asestim;

		/* compute cost/payoff from save and restore */
		rcumul = oldsave - rsave( pa->asrsave, numauto );

		/* add cumulative payoffs, find max of last reg and all var */
		cumul = vcumul + rcumul;
		if( ias >= iasrlast && cumul >= paycut ) { 
			paycut = cumul; 
			iascut = ias; 
		}
/*
		printf( "#ias=%d %s asestim=%d %s vcumul=%d rcumul=%d cumul=%d iascut=%d\n",
			ias,pa->asrname,pa->asestim,pa->asvname,vcumul,rcumul,cumul,iascut );
*/
	}
	return( iascut );
}
/* compute estimated save/restore cycles without func call optimization */
int
rsave( n, numauto )
int n;	/* number of registers to save/restore, not including frame pointer */
int numauto; /* number of autos */

{
	int e;

	if( numauto ) e = CYCSAVE + n * CYCDELTA + CYCADD ;
	else e = CYCSAVE + n * CYCDELTA ;
	return e;
}

/* modify the operands */
void
raopn( iascut, newnreg )
int iascut;	/* index for assignment cutoff */
int newnreg;	/* new number of registers to be saved/restored */
{
	register char *p, *q, *q2;
	NODE *pn;
	register struct assign *pa, *pacut;
	struct assign *pamin;
	int iop;
	static char* numbers[] = { "&0", "&1", "&2", "&3", "&4", "&5", "&6" };
	struct assign *map[MAXREGS];
	int i;

	/* suppress changes for registers assigned to themselves */
	pamin = &asreg[0];
	pacut = &asreg[iascut];
	for( pa = pamin; pa <= pacut; pa++ ) {
		if( strcmp( pa->asvname, pa->asrname ) == 0 )
			pa->asvscl = SCLNULL;
	}

	/* set up mapping for registers */
	for( i = 0; i < MAXREGS; i++ ) map[i] = NULL;
	for( pa = pamin; pa <= pacut; pa++ )
		if( pa->asvscl == REGISTER )
			map[ *( pa->asvname + 2 ) - '0'] = pa;

	/* make changes on the rest */
	for( ALLN( pn ) ) {
		if( pn->op == FILTER ) continue;
		for( iop = 1; iop <= MAXOPS; iop++ ) { /* scan operands */
			if( ( q = pn->ops[iop] ) == NULL ) continue;
			/* check for address arithmetic */
			if( ( ( pn->op == MOVAW ) || ( pn->op == PUSHAW ) ) &&
				( iop == 1 ) ) continue;
			/* find operands that use registers */
			for( p = q; *p != '%' && *p != '\0'; p++ );
			if( *p == '\0' ) continue;
			p++;
			switch( *p ) {
			case 'r': 	/* %r0 through %r8 */
				/* change register, if appropriate */
				if( ( pa = map[*++p - '0'] ) != NULL ) {
					if( *q == '%' )
						pn->ops[iop] = pa->asrname;
					else {
						for( q2=q; *q2!='\0'; q2++ );
						q2 = getspace( q2 - q + 1 );
						pn->ops[iop] = q2;
						p = q2 + ( p - q );
						while((*q2++ = *q++) != '\0');
						*p = *(pa->asrname + 2);
					}
				}
				break;
			case 'f':	/* auto */
			case 'a': 	/* parameter */
				/* convert to register, if apropriate */
				for( pa = pamin; pa <= pacut; pa++ ) {
					if( ( *p=='f' && pa->asvscl!=AUTO ) ||
						(*p=='a'&&pa->asvscl!=PARAM )) 
						continue; /* for speed */
					if( !rausesoff( q, pa->asvname,
						pa->asvlen ) ) continue;
					if( *q == '*' )
						pn->ops[iop] = pa->asrind;
					else	pn->ops[iop] = pa->asrname;
					break;
				}
				break;
			}
		}

		/* change number of registers saved and restored */
		if( pn->op==SAVE || pn->op==RET ) pn->op1 = numbers[newnreg];
	}
}

/* uses offset addressed variable */
int
rausesoff( oper, var, len )
char* oper;	/* pointer to operand */
char* var;	/* pointer to offset addressed variable */
int len;	/* length of the variable in bytes */
{
	register int io, iv;

	if( *oper == '*' ) oper++;
	io = strtol( oper, &oper, 10 );
	iv = strtol( var, &var, 10 );
	if( *oper != '(' || *var != '(' || *(oper + 2) != *(var + 2) )
		return( 0 );
	if( io < iv || io > ( iv + len - 1 ) ) return( 0 );
	return( 1 );
}

/* insert moves for parameters */
void
raparam( iascut )
register int iascut;	/* index for assignment cutoff */
{
	register NODE *pn;
	register int ias;
	register struct assign *pa;

	/* find save instruction */
	for( pn = n0.forw; pn != 0; pn = pn->forw ) { /* scan nodes */
		if( pn->op == SAVE ) {

			/* skip over stack pointer increment for locals */
			if( *( pn->forw->ops[1] + 2 ) == 'F' ) pn = pn->forw;

			/* insert move for each parameter assigned to reg */
			for( ias = 0; ias <= iascut;  ias++ ) {
				pa = &asreg[ias];
				if( pa->asvscl != PARAM ) continue;
				switch( atoi( pa->asvname ) % 4 ) {
				case 0:
					addi( MOVW, "movw", pa->asvname, 
						pa->asrname );
					break;
				case 1:
				case 3:
					addi( MOVB, "movb", pa->asvname,
						pa->asrname );
					break;
				case 2:
					addi( MOVH, "movh", pa->asvname,
						pa->asrname );
					break;
				}
			}
			break;
 		}
	}
}

/* modify def statements */
void
radef( iascut )
int iascut;	/* index for assignment cutoff */
{
	register NODE *pn;
	int ias;
	register struct assign *pa;
	register int scl;
	int val, vval, dscl;
	register char *p, *q;
	char *pv, *ps, *s;

	for( pn = n0.forw; pn != 0; pn = pn->forw ) { /* scan nodes */
		if( pn->op != FILTER ) continue;
		p = pn->opcode;
		if( ! ( p = strchr( p, '.' ) ) || *++p != 'd' ) continue;
		if( ! ( p = strchr( p, '.' ) ) || *++p != 'v' ) continue;
		val = atoi( pv = p += 3 );
		if( ! ( p = strchr( p, '.' ) ) || *++p != 's' ) continue;
		scl = atoi( ps = p += 3 );
		switch( scl ) {
		case C_AUTO: dscl = AUTO; break;
		case C_ARG: dscl = PARAM; break;
		case C_REG:
		case C_REGPARM: dscl = REGISTER; break;
		default: continue;
		}
		for( ias = 0; ias <= iascut; ias++ ) {
			pa = &asreg[ias];
			if( dscl != pa->asvscl ) continue;
			vval = ( scl == C_AUTO || scl == C_ARG ) ?
				atoi( pa->asvname ) : 
				*( pa->asvname + 2 ) - '0' ;
			if( val != vval ) continue;
			q = s = getspace( length( p = pn->opcode ) + 5 );
			while( p != pv ) *q++ = *p++; 		/* thru .val */
			*q++ = *p++;				/* tab */
			*q++ = *( pa->asrname + 2 );		/* reg no */
			while( *++p != ';' );
			while( p != ps ) *q++ = *p++;		/* thru .scl */
			*q++ = *p++;				/* tab */
			q += sprintf( q, "%d", 			/* scl value */
				( scl == C_AUTO || scl == C_REG ) ? 
				C_REG : C_REGPARM );
			while( *++p != ';' );
			while( *q++ = *p++ );			/* the rest */
			pn->opcode = s;
		}
	}
}

/* eliminate auto area if no more autos */
int
raautos( numauto )
int numauto;	/* number of bytes of automatic variables */
{
	register NODE *pn, *qn;
	register int iop;
	register char *p;

	for( pn = n0.forw; pn != 0; pn = pn->forw ) { /* scan nodes */
		if( pn->op == FILTER ) continue;
		if( pn->op == SAVE ) qn = pn->forw;
		for( iop = 1; iop < MAXOPS + 1; iop++ ) { /* scan operands */
			p = pn->ops[iop];
			if( p == NULL ) continue;
			/* look for uses of frame pointer */
			if( usesreg( p, "%fp" ) ) return( numauto );
		}
	}
	/* remove allocation of auto area */
	if( *( qn->op1 + 2 ) == 'F' ) {
		DELNODE( qn );
		numauto = 0;
	}
	return( numauto );
}
#endif /* IMPREGAL */


#ifdef IMPIL
/* in-line substitution optimization
**
** F.B. Wolverton 8/12/83
**
*/

#define	MAXINSTR	22	/* = 20 * (cost/benefit) + 2 */

struct procnode {
	struct procnode	*pnforw;	/* forward pointer for list of procs */
	struct procnode *pnback;	/* backward pointer for list of procs */
	char		*pnname;	/* procedure name */
	int		pncalls;	/* num of times procedure is called */
	int 		pnni; 		/* num of instr in procedure */
	struct node	*pnhead;	/* proc head pointer */
	struct node	*pntail;	/* proc tail pointer */
} prochead, proctail;
int	proccnt;	/* number of procs in table */
int	totalni = 0;	/* total number of instructions in file */
#define PNODE		struct procnode
#define	ALLPN( prn )	prn = prochead.pnforw; prn != &proctail; prn=prn->pnforw
#define ALLPNB( prn )	prn = proctail.pnback; prn != &prochead; prn=prn->pnback
#define PNALLN(prn,pn)  pn = prn->pnhead->forw; pn != prn->pntail; pn=pn->forw
#define PNALLNB(prn,pn) pn = prn->pntail->back; pn != prn->pnhead; pn=pn->back
extern char *mktemp();
char	itmpname[50]; /* name of file for output before in-line subst */
int	outfd;		/* descriptor for output file after in-line subst */
FILE	*outfp;		/* pointer for output file after in-line subst */
void	iledit(), ilinsert();
struct procnode *ilalloc();
#define	LINELEN	400
char	linebuf[LINELEN];
int	intpc = -3; /* persent limit on in-line expansion */
char	defltpc[] = MAXPC;
char	*pcdecode();
extern int zflag;	/* debug flag for in-line expansion */
extern boolean swflag;

/* initialization for in-line substitution */
void
ilinit()
{
	/* initialize procedure list */
	prochead.pnforw = &proctail;
	prochead.pnback = NULL;
	proctail.pnforw = NULL;
	proctail.pnback = &prochead;
	proccnt = 0;

	/* default the in-line expansion limit if not set */
	if( intpc == -3 && *( pcdecode( defltpc )+1 ) != '\0' ) pcdecode( "" );

	/* redirect stdout to tempfile to hold output before in-line subst */
	strcpy( itmpname, TMPDIR );
	strcat( itmpname, "/ccilXXXXXX" );
	mktemp( itmpname );
	if( ( outfd = dup( 1 ) ) == -1 ) 
		fatal( "can't get file descriptor\n", 0 );
	outfp = fdopen( outfd, "w" );
	if( freopen( itmpname, "w", stdout ) == NULL )
		fatal( "can't open file %s\n", itmpname );
}
/* routine to decode the limit on percent in-line expansion */
char *
pcdecode( flags )
char *flags; /* pointer to first character of suboption for 'y' */
{
	char *p;
	switch( *flags ) {
	case 'u': intpc = -1; break;
	case 's': intpc = -2; break;
	case '\n':
	case '\0': 
		fprintf( stderr, "Optimizer: in-line option missing, " );
		fprintf( stderr, "expansion suppressed\n");
		flags--;
		intpc = -2;
		break;
	default:
		intpc = ( int ) strtol( p = flags, &flags, 10 );
		flags--;
		if( flags >= p ) break; /* break if found an integer */
		fprintf( stderr, "Optimizer: invalid in-line option " );
		fprintf( stderr, "starting with '%c', ", *p );
		fprintf( stderr, "expansion suppressed\n" );
		intpc = -2;
	}
	return( flags ); /* returns pointer to last character of suboption */
}
/* routine to mark calls with bytes pushed on stack at time of call */

/* The purpose of this routine is to permit nested calls to be expanded
** in-line.  This routine routine does static analysis of the stack
** size and should, therefore, precede the branch optimization. */

void
ilmark()

{
	register int npush = 0;		/* number of bytes of
				   arguments pushed or moved onto the stack */
	register NODE *pn;		/* pointer to instruction node being
				   processed */
	
	/* check whether in-line expansion is being suppressed */
	if( intpc == -2 ) return;

	/* mark calls */
	for( ALLN( pn ) ) {
		if( pn->op == FILTER ) continue;
		switch(pn->op) {
		case PUSHZB:
		case PUSHZH:
		case PUSHAW:
		case PUSHBB:
		case PUSHBH:
		case PUSHW:
		case PUSHD:
			npush += 4;
			break;
		case ADDW2:
			if( strncmp( pn->op2, "%sp", 3 ) != 0 ) break;
			if( strncmp( pn->op1, "&.F", 3 ) == 0 ) break;
			npush += atoi( pn->op1 + 1 );
			break;
		case SUBW2:
			if( strncmp( pn->op2, "%sp", 3 ) != 0 ) break;
			npush -= atoi( pn->op1 + 1 );
			break;
		case CALL:
			npush -= 4 * atoi( pn->op1 + 1 );
			pn->opm = (char * ) npush;
			break;
		}
	}
}

/* gather statistics for in-line substitution */
void
ilstat( numnreg, numauto )
int numnreg;	/* number of registers to save and restore */
int numauto;	/* number of words of autos */
{
	register NODE *pn, *p;
	PNODE *prn, *pp;
	int nn, ni, iop, nc, lastop;
	char *ncp;

	/* check whether in-line expansion is being suppressed */
	if( intpc == -2 ) return;

	/* count CALL's and total number of instructions */
	lastop = 0;
	for( ALLN( pn ) ) {
		if( pn->op == CALL ) {
			if( ( prn = ilalloc( pn->op2 ) ) == NULL ) return;
			prn->pncalls++;
		}
		if( !islabel( pn ) && pn->op != MISC &&
			( pn->op != RET || 
			( lastop != RET && lastop != JMP ) ) ) {
			totalni++;
			lastop = pn->op;
/*
			printf( "%d %s\n", totalni, pn->ops[0] );
*/
		}
	}

	/* allocate a node for this proc if one does not exist */
	for( ALLN( pn ) ) {
		if( islabel( pn ) ) {
			if( ( prn = ilalloc( pn->opcode ) ) == NULL ) return; 
			break;
		}
	}
	if( pn == 0 ) return;

	/* check conditions for being substituted in line */
	/* check for locals or saved registers */
	if( numnreg != 0 || numauto != 0 ) return;
	/* check procedure size and compute total length of strings */
	nn = ni = nc = lastop = 0;
	for( ALLN( pn ) ) {
		register char *cp;
		nn++;
		for( iop = 0; iop <= MAXOPS; iop++ ) {
			if( (cp = pn->ops[iop] ) != NULL ) nc += length( cp );
		}
		if( !islabel( pn ) && pn->op != MISC &&
			( pn->op != RET || 
			( lastop != RET && lastop != JMP ) ) ) {
			if( ++ni > MAXINSTR ) return;
			lastop = pn->op;
/*
			printf( "%d %s\n", ni, pn->ops[0] );
*/
		}
	}
	/* check for label expressions */
	for( ALLN( pn ) ) {
		if( !islabel( pn ) ) continue;
		for( ALLN( p ) ) {
			register int i;
			for( i = 0; i <= MAXOPS; i++ ) {
				register char *cp;
				cp = p->ops[i];
				if( cp == NULL ) continue;
				if( *cp == *( pn->opcode ) &&/*speed*/
					strcmp(cp, pn->opcode ) == 0) continue;
				while( *cp != '\0' ) {
					cp++;
					if( *cp == *( pn->opcode )&&/* speed */
						strcmp( cp, pn->opcode ) == 0 )
						return;
				}
			}
		}
	}

	/* check for switch tables */
	if( swflag ) { swflag = false; return; }

	/* save the code */
	prn->pnni = ni;
	if( ( p = (NODE *)malloc( ( nn + 2 ) * sizeof( NODE ) + nc ) ) == NULL )
		return;
	ncp = (char *) p + ( nn + 2 ) * sizeof( NODE );
	prn->pnhead = p;
	p->forw = p + 1;
	p->back = NULL;
	for( ALLN( pn ) ) {
		register int i;
		p++;
		p->forw = p + 1;
		p->back = p - 1;
		for( i = 0; i <= MAXOPS; i++ ) {
			if( ( p->ops[i] = pn->ops[i] ) == NULL ) continue;
			strcpy( ncp, pn->ops[i] );
			p->ops[i] = ncp;
			ncp += length( ncp );
		}
		p->op = pn->op;
	}
	p++;	
	p->forw = NULL;
	p->back = p - 1;
	prn->pntail = p;

	/* reposition in list by increasing size */
	/* remove from list */
	prn->pnforw->pnback = prn->pnback;
	prn->pnback->pnforw = prn->pnforw;
	/* locate proper position */
	for( ALLPNB( pp ) ) {
		if( prn->pnni >= pp->pnni ) break;
	}
	/* reinsert it */
	prn->pnforw = pp->pnforw;
	prn->pnback = pp;
	prn->pnforw->pnback = prn;
	prn->pnback->pnforw = prn;
}

/* perform the in-line substitution for whole file */
void
ilfile()
{
	register struct procnode *prn;
	int fpc, delta;
	register char *linptr;

	/* eliminate nodes for progs not in the file or not called */
	for( ALLPN( prn  ) ) {
		if( prn->pnni == 0 || prn->pncalls ==0 ) {
			prn->pnforw->pnback = prn->pnback;
			prn->pnback->pnforw = prn->pnforw;
		}
	}

	/* apply percent limit unless no limit */
	if( intpc != -1 ) {
		/* apply percent text increase constraint */
		fpc = 100 * intpc;
		for( ALLPN( prn ) ) {
			delta = 100 * ( prn->pnni - 2 ) * 100 / totalni;
			fpc -= prn->pncalls * delta;
			if( fpc < 0 ) {
				while( fpc < 0 && prn->pncalls > 0 ) { 
					fpc += delta;
					prn->pncalls--;
				}
				break;
			}
		}
		if( prn != &proctail ) {
			if( ( prn->pncalls ) == 0 ) prn = prn -> pnback;
			prn->pnforw = &proctail;
			prn->pnforw->pnback = prn;
		}
	}

	/* analytic printout */
	if( zflag ) {
		int size, sumsize;
		sumsize = 0;
		if( zflag ) fprintf(stderr, "in-line expansion limit = %d\n",
			intpc );
		for( ALLPN( prn ) ) {
			size = prn->pncalls * 
				( prn->pnni -2 ) * 100 / totalni;
			sumsize += size;
			fprintf( stderr,
				"%s calls=%d inst=%d sz=%d%% t_sz=%d%% \n",
				prn->pnname,prn->pncalls,prn->pnni,size,
				sumsize );
		}
	}

	/* edit routines before inserting */
	iledit();

	/* read temp file and write output with inserted procedures */
	fclose( stdout );
	if( freopen( itmpname, "r", stdin ) == NULL )
		fatal( "can't open file %s\n", itmpname );
	while( ( linptr = fgets( linebuf, LINELEN, stdin ) ) != NULL ){
		register int numauto, nargs;
		register char *cp;
		char *linp;

		if( *linptr == '!' ) continue;
		if( *linptr == '*' ) {
			/* insert .il flag to indicate in-line expansion */
			if( ( linptr = fgets( linebuf, LINELEN, stdin ) )
				== NULL ) break;
			for( cp = linebuf; *cp != '\n'; cp++ );
			*cp = '\0';
			for( ALLPN( prn ) ) {
				if( strcmp( linebuf, prn->pnname ) == 0 ) {
					linptr = fgets( linebuf, LINELEN,
						stdin );
					for(cp=linebuf; *cp != '\n'; cp++)
						if(*cp == ';') linptr = cp;
					*linptr = '\0';
					fprintf( outfp, "%s;\t.il;\t.endef\n",
						linebuf );
					break;
				}
			}
			continue;
		}
		/* default */
		if( *linptr != '@' ) {
			fprintf( outfp, "%s", linptr );
			continue;
		}
		/* expand the call */
		linp = linptr;
		numauto = (int) strtol( ++linp, &linp, 10 );
		nargs = (int) strtol( ++linp, &linp, 10 );
		linptr = linp;
		cp = ++linptr;
		while( *cp != '\n' ) cp++;
		*cp = '\0';
		for( ALLPNB( prn ) ) {
			if( strcmp( linptr, prn->pnname ) == 0 ) {
				if( prn->pncalls <= 0 ) break;
				ilinsert( numauto, nargs, prn );
				prn->pncalls--;
				fgets( linebuf, LINELEN, stdin );
				break;
			}
		}
	}
	(void) fclose( stdin );
	unlink( itmpname );
}


/* edit routines before substitution */
void
iledit()
{
	register NODE *pn;
	register PNODE *prn;

	for( ALLPN( prn ) ) {
		/* replace entry label with new label */
		for( PNALLN( prn, pn ) ) {
			if( islabel( pn ) ) {
				prn->pnhead->forw = pn;
				pn->back = prn->pnhead;
				pn->opcode = "EL";
				break;
			}
		}
		/* replace last return with lab or follow last jump with lab */
		for( PNALLNB( prn, pn ) ) {
			if( pn->op == RET ) {
				pn->op = LABEL;
				pn->opcode = "RL";
				prn->pntail->back = pn;
				pn->forw = prn->pntail;
				/* remove duplicate return, if any */
				while( islabel( pn ) ) pn = pn ->back;
				if( pn->op == RET ) DELNODE( pn );
				break;
			}
			if( pn->op == JMP ) {
				addi( LABEL, "RL", 0, 0 );
				prn->pntail->back = pn;
				pn->forw = prn->pntail;
				break;
			}
		}
		/* replace other returns with jump to new label */
		for( PNALLN( prn, pn ) ) {
			if( pn->op == RET ) {
				pn->op = JMP;
				pn->opcode = "jmp";
				pn->op1 = "RL";
			}
		}
	}
}


/* insert a procedure */
void
ilinsert( numauto, nargs, prn )
int numauto;	/* numbers of words of autos in calling procedure */
int nargs;	/* number of arguments procedure is called with */
PNODE *prn;	/* procedure to be inserted */
{
	register NODE *pn, *p;
	static int ilabel = 0;
	register int i;
	register char *cp, *cpn;
	char *lab;

	/* rewrite labels */
	for( PNALLN( prn, pn ) ) {
		if( !islabel( pn ) ) continue;
		cpn = pn->opcode;
		ilabel++;
		lab = getspace( 9 );
		sprintf( lab, ".I%d\0", ilabel );
		for( PNALLN( prn, p ) ) {
			for( i = 0; i <= MAXOPS; i++ ) {
				cp = p->ops[i];	
				if( cp == NULL ) continue;
				if( *cp == *cpn &&
					strcmp( cp, cpn ) == 0 )
					p->ops[i] = lab;
			}
		}
	}

	/* write out instructions, rewriting args and omitting save */
	for( PNALLN( prn, pn ) ) {
		switch( pn->op ) {
		case SAVE: continue;
		case LABEL:
			fprintf( outfp, "%s:\n", pn->opcode );
			break;
		case HLABEL:
			fprintf( outfp, "%s:\n", pn->opcode );
			break;
		case MISC:
			fprintf( outfp, "	%s\n", pn->opcode );
			break;
		case CALL:
			pn->opm = NULL;
			/* fall through */
		default:
			fprintf( outfp, "	%s	", pn->opcode );
			for( i = 1; i < MAXOPS + 1; i++ ) {
				if( (cp=pn->ops[i]) == NULL ) continue;
				if( i > 1 ) fprintf( outfp, "," );
				if( usesreg( cp, "%ap" ) ) {
					if( *cp == '*') {
						fprintf( outfp, "*" );
						cp++;
					}
					fprintf( outfp, "%d(%%fp)",
						atoi( cp ) +  numauto );
				}
				else fprintf( outfp, "%s", cp );
			}
			fprintf( outfp, "\n" );
			break;
		}
	}

	/* reset stack pointer if necessary */
	if( nargs != 0 ) 
		fprintf( outfp, "	subw2	&%d,%%sp\n", 4 * nargs );
}

/* allocate procedure nodes */
struct procnode *
ilalloc( name )
char	*name;	/* procedure name */
{
	register PNODE *prn;

	/* check whether node already exists */
	for( ALLPN( prn ) ) {
		if( strcmp( name, prn->pnname ) == 0 ) 
			return( prn );
	}

	/* allocate node */
	if( ( prn = ( PNODE * ) 
		malloc( sizeof( PNODE ) + length( name ) ) ) == NULL ) 
		return( prn );
	prn->pnforw = prochead.pnforw;	
	prn->pnback = &prochead;
	prn->pnforw->pnback = prn;
	prn->pnback->pnforw = prn;
	prn->pnname = (char *) prn + sizeof( PNODE );
	strcpy( prn->pnname, name );
	prn->pncalls = 0;
	prn->pnni = 0;
	prn->pnhead = NULL;
	return( prn );
}
#endif /* IMPIL */
