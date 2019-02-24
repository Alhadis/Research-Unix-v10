static char ID[] = "@(#) expr0.c: 1.3 3/13/83";
#include "system.h"

#include <stdio.h>
#include "structs.h"
#include "extrns.h"
#include "sgsmacros.h"
#include "y.tab.h"
#include "ldmacros.h"
/*eject*/
ENODE *
buildtree(op,l,r)
int op;
ENODE *l,*r; 
{
 
/*
 * Build a node in the expression tree and
 * return a pointer to it
 */

	register ENODE *p;

	p = (ENODE *) myalloc(sizeof(ENODE));

	p->gnode.exop = op;
	p->gnode.exleft = l;
	p->gnode.exright = r;
	return(p);
}
/*eject*/
ENODE *
symnode(symp)
char *symp;
{

/*
 * Build an expression node for NAME or DOT
 */

	register ENODE *p;

#if ONEPROC
	SYMENT sym;
#endif

	p = (ENODE *) myalloc(sizeof(ENODE));

	if( symp == NULL ) {		/* special DOT symbol */
		p->pnnode.exop = DOT;
		}
	else {
#if ONEPROC
		p->nnode.exop = NAME;
		zero( (char *) &sym, SYMESZ );
#if FLEXNAMES
		if (strlen(symp) > 8) {
			sym.n_zeroes = 0L;
			sym.n_nptr = symp;
			}
		else
#endif
			copy(sym.n_name,symp,8);
		sym.n_type = T_INT;
		sym.n_sclass = C_EXT;
		p->nnode.exsymptr = putsym(makesym(&sym, NULL),1);
#else
		p->pnnode.exop = NAME;
		copy(p->pnnode.symbol,symp,8);
#endif
		}

	return(p);
}
/*eject*/
ENODE *
cnstnode(val)
long val;
{

/*
 * Build an expression node for a constant
 */

	register ENODE *p;

	p = (ENODE *) myalloc(sizeof(ENODE));

	p->vnode.exop = INT;
	p->vnode.exvalue = val;

	return(p);
}
