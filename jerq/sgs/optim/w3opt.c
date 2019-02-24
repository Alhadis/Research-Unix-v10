/* @(#) w3opt.c: 1.4 3/27/84				*/
/* w3opt.c
**
**	3B20S three-instruction window improver
**
**
**
** This module contains improvements for three instruction windows,
** of which there aren't many.
*/

/* #include "defs" -- optim.h takes care of this */
#include "optim.h"
#include "optutil.h"
/* w3opt -- 3-instruction peephole window */

boolean					/* true if changes made */
w3opt(pf,pl)
register NODE * pf;			/* pointer to first inst. in window */
register NODE * pl;			/* pointer to last inst. in window */
{
    register NODE * pm = pf->forw;	/* point at middle node */

    int cop1 = pf->op;			/* op code number of first inst. */
    int cop2 = pm->op;			/* op code number of second */
    int cop3 = pl->op;			/* ... of third */

    int src1 ;				/* size (bytes) of source of move */
    int dst1;				/* size of destination of move */
/* *p++ improvement
**
** This improvement rearranges things to facilitate a later 2-instruction
** improvement.  We're looking for the kind of code the compiler generates
** (naively) for *p++.  We want to make an indirect reference possible:
**
**	movw	O1,R		->	movw O1,R
**	addw2	&n,O1		->	movX 0(R),O2
**	movX	0(R),O2		->	addw2 &n,O1
**
** or
**	movw	O1,R		->	movw O1,R
**	addw2	&n,O1		->	movX O2,0(R)
**	movX	O2,0(R)		->	addw2 &n,O1
**
**	if O1 is not a register used by O2
**	if instruction following movX is not conditional branch, since
**	  we're setting different condition codes than before
**	in the first case, O2 cannot be a register used by O1.
**
** Note that this transformation is always correct because:
**
**	1.  O1 could not use R, or the first 2 instructions wouldn't work.
**	2.  O2 can use or set R without problems.
*/

    if (
	    cop1 == MOVW
	&&  cop2 == ADDW2
	&&  ismove(pl,&src1,&dst1)
	&&  isreg(pf->op2)
	&&  *pm->op1 == '&'		/* immediate operand */
	&&  strcmp(pf->op1,pm->op2) == 0
	&&  isdeadcc( pl )
	&&  ! usesvar(pm->op2,pl->op2)	/* O1 can't use O2 */
	)
    {
	char * R = pf->op2;		/* point at register string */
	char * O1 = pf->op1;		/* point at first operand */
	char * O2;			/* second operand */

	if (
		(   ( O2 = pl->op2, iszoffset(pl->op1,R) ) /* test 0(R) */
		||  ( O2 = pl->op1, iszoffset(pl->op2,R) )
		)
	    &&  ! usesvar(O2,O1)	/* O2 can't use O1 */
	    &&  ( isiros( O1 ) || isiros( O2 ) )
					/* safe for mmio */
	    )
	{
	    wchange();			/* change the window */
	    lexchin(pm,pl);		/* preserve line number info */
	    exchange(pm);		/* exchange the last two nodes */
	    swplivecc(pm,pl);       /* swap live/dead info on condtion codes */
	    return(true);
	}
    }
/* Remove redundant compares (see, also w2opt)
**
** On the 3B20S, some instructions do not set all of the result
** indicators correctly.  Consequently it is only safe to remove
** a compare against zero if the following conditional jump only
** tests those bits which are set.  The canonical 3 instruction
** sequence is:
**
**	op O1,O2[,O3]
**	cmpX O[2|3],&0
**	jcond foo
**
** For the instructions we test here, the only valid conditions to
** jump on are equal (zero) and not equal (non-zero).
** Note that signed tests are picked up correctly in w2opt for those
** instructions that generate correct result indicators.
*/

    if (
	    (cop2 == CMPW || cop2 == CMPH || cop2 == CMPB)
	&&  strcmp(pm->op2,"&0") == 0
	&&  strcmp(dst(pf),pm->op1) == 0
	&&  (cop3 == JE || cop3 == JZ || cop3 == JNE || cop3 == JNZ)
	&&  (MCOMB <= cop1 && cop1 <= UMODW3)
	&&  stype(cop1) == stype(cop2)
	&&  isiros(pm->op1)		/* safe from mmio */
	)
    {
	wchange();
	ldelin2(pm);			/* preserve line number info */
	mvlivecc(pm);       /* preserve live/dead info on condtion codes */
	DELNODE(pm);			/* delete the compare */
	return(true);
    }
    return(false);
}
