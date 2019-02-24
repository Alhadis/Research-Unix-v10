static char ID[] = "@(#) expr1.c: 1.3 3/13/83";
#include "system.h"

#include <stdio.h>
#include "structs.h"
#include "extrns.h"
#include "sgsmacros.h"
#include "y.tab.h"
#include "ldmacros.h"

static int expsect;
/*eject*/
long
eval(p)
ENODE *p;
{

/*
 * Evaluate an expression node, returning the (long) value of the
 * expression.
 *
 * The extern variable "cur_dot" is used when processing exprs
 * inside of an output section specification, and is set to
 * -1L after allocation, when all sections have been relocated.
 */

	long val1, val2;
	SYMTAB *symp;
	OUTSECT *s;

	switch (p->gnode.exop) {
	case INT:
		return( p->vnode.exvalue );
	case DOT:
		/*
		 * Assign to DOT, if not a misuse of the assignment
		 */
		if( p->nnode.exsymptr == 0L ) {
			if( cur_dot == -1L ) {
				lderror(1, curexp->evexpr.aiinlnno, curexp->evexpr.aiinflnm,
					"misuse of DOT symbol in assignment instruction");
				return (0L);
				}
			return(cur_dot);
			}
		else {
			/*
			 * Use of dot in assign to symbol
			 */
			symp = getsym(p->nnode.exsymptr);
			s = (OUTSECT *) symp->smscnptr;
			val1 = symp->sment.n_value + s->oshdr.s_paddr;
			if ( expsect == 0 )
				expsect = s->ossecnum;
			PUTSYM(symp, 0);
			return (val1);
			}
	case NAME:
		symp = getsym(p->nnode.exsymptr);
		val1 = symp->smnewval;
		if (symp->sment.n_scnum > 0) {
			if(expsect == 0)
				expsect = symp->sment.n_scnum;
			else
				expsect = -1;
			}
		else if( symp->sment.n_scnum == 0 )
			lderror(1, curexp->evexpr.aiinlnno, curexp->evexpr.aiinflnm,
				"undefined symbol in expression");
		PUTSYM(symp, 0);
		return (val1);
	case ALIGN:
		if( cur_dot == -1L ) {
			lderror(1, curexp->evexpr.aiinlnno, curexp->evexpr.aiinflnm,
				"ALIGN illegal in this context");
		return(0L);
		}
		val2 = eval(p->gnode.exleft);
		val1 = (cur_dot + val2 - 1) & (~(val2-1));
		return(val1);

	case PHY:
		/*
		 * If (cur_dot != -1L) here, then eval() was
		 * called from pboslist(), which is processing
		 * the output section list.  Moreover, we have
		 * an assignment to DOT, because eval() is not
		 * called otherwise.  The statement is
		 *	. = ... PHY(...) ...
		 * which is invalid.
		 */
		if( cur_dot != -1L ) {
			lderror(1, curexp->evexpr.aiinlnno,
				curexp->evexpr.aiinflnm,
				"illegal assignment of physical address to DOT");
			return (0L);
			}
		expsect = -1;
		switch( p->gnode.exleft->gnode.exop ) {
		case NAME:
			symp = getsym(p->gnode.exleft->nnode.exsymptr);
			val1 = symp->smnewval;
			if ( symp->smscnptr == NULL ) {
				switch ( symp->sment.n_scnum ) {
				case -1 :	/* absolute */
					lderror(1, curexp->evexpr.aiinlnno, curexp->evexpr.aiinflnm,
						"phy of absolute symbol %s is illegal",
						SYMNAME(symp->sment));
					break;
				case 0 :	/* undefined */
					lderror(1, curexp->evexpr.aiinlnno, curexp->evexpr.aiinflnm,
						"symbol %s is undefined",
						SYMNAME(symp->sment));
					break;
				}
				}
			PUTSYM(symp, 0);
			return ( val1 );
		case DOT:
			if( p->gnode.exleft->nnode.exsymptr == 0L ) {
				lderror(1,curexp->evexpr.aiinlnno,
					curexp->evexpr.aiinflnm,
					"internal error: no symtab entry for DOT");
				return (0L);
				}
			symp = getsym(p->gnode.exleft->nnode.exsymptr);
			val1 = symp->sment.n_value;
			val1 += ((OUTSECT *) symp->smscnptr)->oshdr.s_paddr;

			PUTSYM(symp, 0);
			return ( val1 );
		default:
			lderror(1, curexp->evexpr.aiinlnno, curexp->evexpr.aiinflnm,
				"operand of PHY must be a name");
			return(0L);
		}


	case UMINUS:	val1 = eval(p->gnode.exleft);
			return(-val1);
	case NOT:	val1 = eval(p->gnode.exleft);
			return(!val1);
	case BNOT:	val1 = eval(p->gnode.exleft);
			return(~val1);
	case AND:	val1 = eval(p->gnode.exleft);
			val2 = eval(p->gnode.exright);
			return(val1 & val2);
	case ANDAND:	val1 = eval(p->gnode.exleft);
			val2 = eval(p->gnode.exright);
			return(val1 && val2);
	case DIV:	val1 = eval(p->gnode.exleft);
			val2 = eval(p->gnode.exright);
			return(val1 / val2);
	case EQEQ:	val1 = eval(p->gnode.exleft);
			val2 = eval(p->gnode.exright);
			return(val1 == val2);
	case GE:	val1 = eval(p->gnode.exleft);
			val2 = eval(p->gnode.exright);
			return(val1 >= val2);
	case GT:	val1 = eval(p->gnode.exleft);
			val2 = eval(p->gnode.exright);
			return(val1 > val2);
	case LE:	val1 = eval(p->gnode.exleft);
			val2 = eval(p->gnode.exright);
			return(val1 <= val2);
	case LSHIFT:	val1 = eval(p->gnode.exleft);
			val2 = eval(p->gnode.exright);
			return(val1 << val2);
	case LT:	val1 = eval(p->gnode.exleft);
			val2 = eval(p->gnode.exright);
			return(val1 < val2);
	case MINUS:	val1 = eval(p->gnode.exleft);
			val2 = eval(p->gnode.exright);
			return(val1 - val2);
	case MULT:	val1 = eval(p->gnode.exleft);
			val2 = eval(p->gnode.exright);
			return(val1 * val2);
	case NE:	val1 = eval(p->gnode.exleft);
			val2 = eval(p->gnode.exright);
			return(val1 != val2);
	case OR:	val1 = eval(p->gnode.exleft);
			val2 = eval(p->gnode.exright);
			return(val1 | val2);
	case OROR:	val1 = eval(p->gnode.exleft);
			val2 = eval(p->gnode.exright);
			return(val1 || val2);
	case PC:	val1 = eval(p->gnode.exleft);
			val2 = eval(p->gnode.exright);
			return(val1 % val2);
	case PLUS:	val1 = eval(p->gnode.exleft);
			val2 = eval(p->gnode.exright);
			return(val1 + val2);
	case RSHIFT:	val1 = eval(p->gnode.exleft);
			val2 = eval(p->gnode.exright);
			return(val1 >> val2);

	default:
		lderror(1, curexp->evexpr.aiinlnno, curexp->evexpr.aiinflnm,
			"illegal operator in expression");
		return(0L);
	}
}
/*eject*/
pexlist()
{

/*
 * After allocation, process the "expression list"
 * to define any user-specified symbols.
 */

#if !ONEPROC
	ACTITEM a;
#endif

	cur_dot = -1L;
	curexp = (ACTITEM *) explist.head;

	while( curexp ) {

		pexitem();

		clrexp( curexp );

		curexp = curexp->evexpr.ainext;

		}

#if DEBUG
	if( dflag > 2 )
		fprintf( stderr, "\nEXPLIST data structures:" );
#endif
#if !ONEPROC
	while( expcnt-- ) {

		fread( &a, 1, sizeof(ACTITEM), trnfdes );
		a.evexpr.aiinflnm = (char *) ((int) a.evexpr.aiinflnm + (int) strbase);

#if DEBUG
		if( dflag > 2 )
			fprintf( stderr, "\n\t%04x AIEVEXPR (%s) %d %04x\n\t\tROOT:",
				a.evexpr.ainext, a.evexpr.aiinflnm,
				a.evexpr.aiinlnno, a.evexpr.aiexptr );
#endif

		curexp = ldexp( &a );

		pexitem();

		clrexp( curexp );

		}
#endif
}
/*eject*/
pexitem()
{

/*
 * Process one assignment instruction
 */

	long val;
	register SYMTAB *sp;
	register OUTSECT *oscn;

	expsect = 0;
	val = eval(curexp->evexpr.aiexptr->gnode.exright);
	sp = getsym(curexp->evexpr.aiexptr->gnode.exleft->nnode.exsymptr);

/*
 * If the section number of the symbol we
 * are assigning a value is not zero ( = undefined),
 * the user is redefining the value of the symbol.
 */

	if( sp->sment.n_scnum )
		if( sp->smscnptr )
			lderror(0, curexp->evexpr.aiinlnno, curexp->evexpr.aiinflnm,
				"symbol %s from file %s being redefined",
				SYMNAME(sp->sment), sp->smscnptr->isfilptr->flname);
		else
			lderror(0, curexp->evexpr.aiinlnno, curexp->evexpr.aiinflnm,
				"absolute symbol %s being redefined",
				SYMNAME(sp->sment));

	if( expsect == 0 )
		sp->sment.n_scnum = -1;		/* absolute symbol */
	else
		sp->sment.n_scnum = expsect;

	if( (oscn = findoscn(sp->sment.n_scnum)) != NULL )
		sp->smnewvirt = val - oscn->oshdr.s_paddr + oscn->oshdr.s_vaddr;
	else
		sp->smnewvirt = val;
	sp->smnewval = val;
	sp->sment.n_sclass = C_EXT;
	PUTSYM(sp, 1);

}
/*eject*/
#if !ONEPROC
ACTITEM *
ldexp(a)
ACTITEM *a;
{

/*
 * Load an assignment instruction from the "transfer file" generated
 * by PASS 1, into memory
 *
 * "a" is the action item which heads the expression tree
 *
 * Return a pointer to a separate copy of this action item
 */

	register ACTITEM *aptr;
	register ENODE   *eptr;
	ENODE *ldlimb();

	aptr = (ACTITEM *) myalloc(sizeof(ACTITEM));
	*aptr = *a;

	eptr = (ENODE *) myalloc(sizeof(ENODE));
	fread( eptr, 1, sizeof(ENODE), trnfdes );

#if DEBUG
	if( dflag > 2 )
		fprintf( stderr, "\t%d %04x %04x", (unsigned) eptr->gnode.exop,
			eptr->gnode.exleft, eptr->gnode.exright );
#endif

	if( eptr->gnode.exleft )
		eptr->gnode.exleft = ldlimb();
	if( eptr->gnode.exright )
		eptr->gnode.exright = ldlimb();

	aptr->evexpr.ainext = NULL;
	aptr->evexpr.aiexptr = eptr;

	return( aptr );
}
/*eject*/
ENODE *
ldlimb()
{

/*
 * Load a non-root node of an expression tree into memory
 */

	register ENODE *eptr;
	SYMENT sym;

	eptr = (ENODE *) myalloc(sizeof(ENODE));
	fread( eptr, 1, sizeof(ENODE), trnfdes );

	switch( (unsigned) eptr->gnode.exop ) {
	case INT:
#if DEBUG
		if( dflag > 2 )
			fprintf( stderr, "\n\t\t\tINT %08lx", eptr->vnode.exvalue );
#endif
		break;
	case DOT:
#if DEBUG
		if( dflag > 2 )
			fprintf( stderr, "\n\t\t\tDOT ()" );
#endif
		break;
	case NAME:
#if DEBUG
		if( dflag > 2 )
			fprintf( stderr, "\n\t\t\tNAME (%s)",
				eptr->pnnode.symbol );
#endif
		zero( (char *) &sym, SYMESZ );
#if FLEXNAMES
		if (strlen(eptr->pnnode.symbol) > 8) {
			sym.n_zeroes = 0L;
			sym.n_nptr = eptr->pnnode.symbol;
			}
		else
#endif
			copy( sym.n_name, eptr->pnnode.symbol, 8);
		sym.n_type = T_INT;
		sym.n_sclass = C_EXT;
		eptr->nnode.exsymptr = putsym( makesym(&sym), 1 );
		break;
	default:
#if DEBUG
		if( dflag > 2 )
			fprintf( stderr, "\n\t\t\t%d %04x %04x", (unsigned) eptr->gnode.exop,
				eptr->gnode.exleft, eptr->gnode.exright );
#endif
		if( eptr->gnode.exleft )
			eptr->gnode.exleft = ldlimb();
		if( eptr->gnode.exright )
			eptr->gnode.exright = ldlimb();
		break;
	}

	return( eptr );
}
/*eject*/
#endif
clrexp(a)
ACTITEM *a;
{

/*
 * Free up all memory assigned to the assignment instruction pointed
 * to by "a"
 */
	ENODE *eptr;

	eptr = a->evexpr.aiexptr;

	switch( (unsigned) eptr->gnode.exop ) {
	case INT:
	case DOT:
	case NAME:
		break;
	default:
		if( eptr->gnode.exleft )
			clrlimb(eptr->gnode.exleft);
		if( eptr->gnode.exright )
			clrlimb(eptr->gnode.exright);
		break;
	}

	free( eptr );
	free( a );
}




clrlimb(eptr)
ENODE *eptr;
{

/*
 * Free up a non-root node of an expression tree
 */

	switch( (unsigned) eptr->gnode.exop ) {
	case INT:
	case DOT:
	case NAME:
		break;
	default:
		if( eptr->gnode.exleft )
			clrlimb(eptr->gnode.exleft);
		if( eptr->gnode.exright )
			clrlimb(eptr->gnode.exright);
		break;
	}

	free( eptr );
}
