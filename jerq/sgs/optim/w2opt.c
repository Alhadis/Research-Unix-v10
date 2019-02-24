/* @(#) w2opt.c: 1.6 4/28/84				*/
/* w2opt.c
**
**	3B20S optimizer -- two instruction peephole window
**
**
** This module contains the code that improves two-instruction
** sequences.  The general scheme is to put those improvements
** first which result in removing code, followed by the others.
**
** In some cases we play around with the live/dead information
** to convince the one-instruction window not to throw away
** code we need.  This is particularly evident when we remove
** redundant compares:  we want to make sure that the instruction
** that calculates the result indicators does not get deleted.
*/

/* Some general caveats (learned the hard way):
**
**	1.  When instructions get interchanged, we must take care that
**	    we don't alter the condition codes that would have resulted
**	    when executing the instructions in their original order.
**	2.  We can't move adds to %sp, since we may move them after the
**	    place which refers to the newly allocated space on the stack.
*/

/* #include "defs" -- optim.h takes care of this */
#include "optim.h"
#include "optutil.h"

extern int sprintf();
/* w2opt -- 2-instruction peephole window */

boolean					/* true if changes made */
w2opt(pf,pl)
register NODE * pf;			/* first instruction node of window */
register NODE * pl;			/* second instruction node */
{
    int cop1 = pf->op;			/* op code number of first inst. */
    int cop2 = pl->op;			/* op code number of second inst. */

    int opn;				/* op code number (for istriadic) */
    char * opst;			/* op code string (for istriadic) */

    boolean f;				/* temporary boolean used to
					** distinguish cases below
					*/
    int temp;				/* general integer temporary */
    long templ;				/* general long temporary */
    int src1, dst1;			/* sizes of movX source, destination */
    int src2, dst2;			/* sizes of second such movX */
/* merge moves back into triadic
**
**	opw3 O1,O2,R
**	movw R,O3		->	op3 O1,O2,O3
**
**	if R dead
**
**	We depend on the 1-instruction peephole to break such instructions
**	apart again if appropriate.
*/


    if (
	    istriadic(pf,&opn,&opst)
	&&  cop2 == MOVW
	&&  isdead(pl->op1,pl)		/* also tests whether reg. */
	&&  strcmp(pf->op3,pl->op1) == 0
	&&  ! usesreg(pl->op2,pl->op1)	/* register not used in destination */
	)
    {
	wchange();			/* making a change */
	pf->op3 = pl->op2;		/* copy operand pointer */
	makedead(pl->op1,pf);		/* turn off liveness of R in op3 */
	if (! isdead(pl->op2,pl))
	    makelive(pl->op2,pf);	/* turn on liveness of O3, in case
					** it's a register
					*/
	lmrgin1(pf,pl,pf);		/* preserve line number info */ 
	mvlivecc(pl);			/* preserve condition code info */
	DELNODE(pl);			/* delete second node */
	return(true);
    }
/* Combine two dyadics into a triadic, if possible.
**
** This improvement is useful on processors for which triadics are
** better than dyadics.
**
**	movw O2,O3
**	opw2 O1,O3		->	op3 O1,O2,O3
**
**	if O1 does not use O3 in any way
*/

#ifdef IMPTRIAD
#ifdef	M32				/* applies only to BELLMAC-32 */

	if (   cop1 == MOVW
	    && totriadic(pl, &opn, &opst) /* check whether dyadic can go
					  ** to triadic
					  */
	    && strcmp(pf->op2, pl->op2) == 0
	    && ! usesvar(pl->op1, pf->op2)
	    && ( isiros(pf->op1) || isiros(pl->op1) )
	    && isiros(pf->op2)		/* safe from mmio */
	    )
	{
	    wchange();			/* we're changing something */

	    /* make changes to second instruction */

	    pl->op3 = pl->op2;		/* make room for new operand */
	    pl->op2 = pf->op1;		/* copy from first inst. */
	    chgop(pl, opn, opst);	/* change opcode to triadic */
	    lmrgin2(pf,pl,pl);	/* preserve line number info */ 
	    mvlivecc(pf);		/* preserve condition code info */
	    DELNODE(pf);		/* delete first inst. */
	    return(true);
	}

#endif	/* def M32 */
#endif /* IMPTRIAD */
/* Remove unnecessary movw's before compares.
**
**	movw O1,R
**	cmpw O2,R		->	cmpw O2,O1
** or
**
**	movw O1,R
**	cmpw R,O2		->	cmpw O1,O2
**
**	if R dead after cmpw
**/

    if (
	    cop1 == MOVW
	&&  cop2 == CMPW
	&&  isdead(pf->op2,pl)		 /* (implicit isreg test, too) */
	&&  (
		( (f = samereg(pf->op2,pl->op1)) 
		       && ! usesreg(pl->op2,pl->op1)
		       && ( isiros(pf->op1) || isiros(pl->op2) )
					/* safe from mmio */
		       )
	     ||	( samereg(pf->op2,pl->op2) && ! usesreg(pl->op1,pl->op2) )
	    )
	)
    {
	wchange();			/* flag changes */
	if (f)				/* true if second case above */
	    pl->op1 = pf->op1;
	else
	    pl->op2 = pf->op1;
	lmrgin1(pf,pl,pl);		/* preserve line number info */ 
	mvlivecc(pf);		/* preserve condition code info */
	DELNODE(pf);			/* discard movw */
	return(true);
    }
/* Eliminate compares against zero if preceded by instruction that
** sets result indicators.  Because some 3B20S instructions do not
** set all result indicators, we can only discard compares after
** those instructions that set all of the relevant ones.  w3opt
** takes care of those which set a limited range of result indicators,
** but which are followed by a suitable conditional jump.
**
** None of the arithmetic operations set the "unsigned" result indicators,
** so we retain compares before any unsigned-conditional jump.
**
**	op O1,O2[,O3]		->	op O1,O2[,O3]
**	cmpX O[2|3],&0
**
**/

    if (
	    (cop2 == CMPW || cop2 == CMPH || cop2 == CMPB)
	&&  strcmp(pl->op2,"&0") == 0
	&&  strcmp(dst(pf),pl->op1) == 0
	&&  (MOVB <= cop1 && cop1 <= ARSW3)
	&&  stype(cop1) == stype(cop2)
	&&  (pl->forw->op < JLU || pl->forw->op > JGEU)
	&&  isiros(pl->op1)		/* safe from mmio */
	)
    {
	wchange();			/* changing window */
	lmrgin3(pf,pl,pf);		/* preserve line number info */ 
	mvlivecc(pl);		/* preserve condition code info */
	DELNODE(pl);			/* delete the compare */
	return(true);			/* announce success */
    }
/* Eliminate compares against zero if preceding instruction sets condition
** codes (special cases).
**
**	movw O1,O2
**	cmpw O1,&0		->	movw O1,O2
**
** or
**	call &n,O
**	cmpw %r0,&0		->	call &n,O
**
** This last improvement is moderately dangerous.  It depends on the
** fact that, in C, a function returning a value puts the value in r0
** just before exiting.  The return indicators would then be set correctly.
** An assembly coder could blow this assumption to pieces.
** Once again we have to watch out for unsigned tests following the cmpw,
** since a "movw" in the called function would not set the unsigned
** result indicators.
*/


    if (
	    cop2 == CMPW
	&&  strcmp(pl->op2,"&0") == 0
	&& (
		(
		    cop1 == MOVW
		&&  strcmp(pf->op1,pl->op1) == 0
		&&  ! usesvar(pf->op1,pf->op2)
					/* O1 cannot use O2 in any way */
		&&  isiros(pf->op1)	/* safe for mmio */
		)
#ifdef CALLCMPW				/* make this a selectable option */
	    ||
		(
		    cop1 == CALL
		&&  strcmp(pl->op1,"%r0") == 0
		)
#endif	/* def CALLCMPW */
	    )
	&&  ! (JLU <= pl->forw->op && pl->forw->op <= JGEU)
					/* disallow unsigned jump after cmpw */
	)
    {
	wchange();
	lmrgin3(pf,pl,pf);		/* preserve line number info */ 
	mvlivecc(pl);		/* preserve condition code info */
	DELNODE(pl);
	return(true);
    }
/* One of the assumptions about eliminating "cmpw" after a "call"
** is that all functions set result indicators upon returning a value.
** If the value is set by a bit-field extract, this is not so.
** Therefore, we must insert an extra "cmpw" to compensate:
**
**	extzv	O1,O2,O3,%r0	-->	extzv	O1,O2,O3,%r0
**				-->	cmpw	%r0,&0
**	ret			-->	ret
*/

#ifdef	CALLCMPW			/* only if this is enabled */

    if (
	    cop1 == EXTZV
	&&  cop2 == RET
	&&  strcmp(pf->op4,"%r0") == 0
	)
    {
	NODE * newnode = insert(pf);	/* create new node after extzv */

	wchange();			/* making a change */
	chgop(newnode,CMPW,"cmpw");	/* set op code number and string */
	newnode->op1 = "%r0";		/* set up instruction */
	newnode->op2 = "&0";
	newnode->nlive = pf->nlive;	/* save live/dead as predecessor */
	pf->nlive &= ~CONCODES;         /* killing condition codes */
	return(true);			/* made a change */
    }
#endif	/* def CALLCMPW */
/* This next set of improvements deals with pairs of move's. */

/* case 1:  redundant movw
**
**	movw O1,O2
**	movw O2,O1		->	movw O1,O2
** or
**
**	movw O1,O2
**	movw O1,O2		->	movw O1,O2 (second one)
**
** Note that, for the second improvement, O2 cannot be used by O1.
*/
    if (cop1 == MOVW && cop2 == MOVW)
    {
	if (    strcmp(pf->op1,pl->op2) == 0	/* first case */ 
 	    &&  strcmp(pf->op2,pl->op1) == 0
	    &&  isiros(pf->op1)
	    &&  isiros(pf->op2)		/* safe from mmio */
	   )
	{
	    wchange();				/* change window */
	    lmrgin3(pf,pl,pf);		/* preserve line number info */ 
	    mvlivecc(pl);	/* perserve conditions codes live info */
	    DELNODE(pl);			/* delete second inst. */
	    return(true);
	}
	
	if (    strcmp(pf->op1,pl->op1) == 0	/* second case */
	    &&  strcmp(pf->op2,pl->op2) == 0
	    &&  ! usesvar(pl->op1,pf->op2)
	    &&  isiros(pf->op1)
	    &&  isiros(pf->op2)		/* safe from mmio */
	   )
	{
	    wchange();
	    lmrgin3(pf,pl,pl);		/* preserve line number info */ 
	    mvlivecc(pf);	/* perserve conditions codes live info */
	    DELNODE(pf);			/* delete first inst. */
	    return(true);
	}
    }
/* Case 2:  arbitrary size moves with intermediate register
**
** The idea here is to collapse pairs of moves, possibly with different
** sized operands, into one correct move.  We assume that a register
** is used unnecessarily in the operation.  That is, we are looking at
**
**	movX	O1,R
**	movY	R,O2		->	movZ O1,O2
**
**	if R dead after movY and O2 doesn't use R
**	or O2 is same register as R
**
** where X, Y, and Z are things like "twb" (as in, "movtwb"), "h",
** and "bbw".  The trick is to choose the correct move based on the
** sizes of the operands.  There are 81 (gasp!) combinations of four
** operands, with three choices per operand.  The code below is
** correct, but misses one obscure case:
**
**	movtbh	O1,R
**	movtwh	R,O2		->	movh O1,O2
*/

    if (
	    ismove(pf,&src1,&dst1)	/* test, get source, dest. sizes */
	&&  ismove(pl,&src2,&dst2)
	&&  isreg(pf->op2)
	&&  samereg(pf->op2, pl->op1)
	&&  (
		( isdead(pl->op1,pl) &&  ! usesreg(pl->op2,pl->op1) )
	     || samereg(pl->op1,pl->op2)
	     )
	)
    /* fall through to sub-cases */
    {
    /* sub-case 1:
    **  Sizes of movX source and movY destination are equal.  Source of
    **  movX no larger than destination, source of movY no smaller than its
    **  destination.  ("Size" here refers to number of bytes in effective
    **  operand.)  Example:
    **
    **		movbbw O1,R				s = 1; d = 4
    **		movthb R,O2	->	movb O1,O2	s = 2; d = 1
    **
    ** (More common case...)
    **
    **		movw O1,R				s = 4; d = 4
    **		movw R,O2	->	movw O1,O2	s = 4; d = 4
    */

	if (
		src1 == dst2
	    &&  src1 <= dst1
	    &&  src2 >= dst2
	    )
	{
	    wchange();
	    pl->op1 = pf->op1;		/* copy down first operand */
	    lmrgin1(pf,pl,pl);	/* preserve line number info */ 
	    mvlivecc(pf);	/* perserve conditions codes live info */
	    DELNODE(pf);		/* delete first node */
	    switch (src1)		/* choose correct new instruction */
	    {
	    case 1:			/* byte to byte */
		chgop(pl,MOVB,"movb"); break;
	    
	    case 2:			/* halfword to halfword */
		chgop(pl,MOVH,"movh"); break;

	    case 4:			/* word to word */
		chgop(pl,MOVW,"movw"); break;
	    }
	    return(true);
	}
    /* sub-case 2:
    **	Both sources are the same size, and either the destinations are
    **  the same size or destination of movX larger than source.
    **  Examples:
    **
    **		movzbw	O1,R				s = 1; d = 4
    **		movb	R,O2	->	movb O1,O2	s = 1; d = 1
    **
    **		movtwh	O1,R				s = 4; d = 2
    **		movtwh	R,O2	->	movtwh O1,O2	s = 4; d = 2
    */

	if (
		src1 == src2		/* source sizes equal */
	    &&  (
		    src1 <= dst1
		||  dst1 == dst2
		)
	    )
	{
	    wchange();			/* making a change */
	    pl->op1 = pf->op1;		/* copy operand */
	    lmrgin1(pf,pl,pl);	/* preserve line number info */ 
	    mvlivecc(pf);	/* perserve conditions codes live info */
	    DELNODE(pf);		/* delete first node */
	    return(true);
	}
    /* sub-case 3:
    **  Destination 1, source 2, and destination 2 are all the same size.
    **  Example:
    **
    **		movzhw O1,R				s = 2; d = 4
    **		movw R,O2	->	movzhw O1,O2	s = 4; d = 4
    */

	if (
		dst1 == src2
	    &&  src2 == dst2
	    )
	{
	    wchange();
	    pf->op2 = pl->op2;		/* copy up second operand */
	    makelive(pf->op2,pf);	/* make sure first inst. won't now
					** go away
					*/
	    lmrgin1(pf,pl,pf);	/* preserve line number info */ 
	    mvlivecc(pl);	/* perserve conditions codes live info */
	    DELNODE(pl);		/* delete second node */
	    return(true);
	}
    } /* end move-move merging */
/* Merge register move into indirect reference.
**
** The purpose of this improvement is to eliminate a movw if it is
** used to load a register which is then used exclusively for an
** indirect reference.  Examples:
**
**	movw O1,R
**	op2 0(R),O2		->	op2 *O1,O2
**
**	if R dead after op2
**	if R not used in O2, except as 0(R)
**
** We use utility routine 'doindirect' to do this in a general way
** for, in fact, any instruction and not just op2.
*/

    if ( cop1 == MOVW && isreg(pf->op2))
    {
	/* use utility routine to check for feasibility.  Replace
	** 0(R) with suitable indirection from O1
	*/

	if (doindirect(pl,pf->op2,pf->op1))
	{
	    lmrgin1(pf,pl,pl);	/* preserve line number info */ 
	    mvlivecc(pf);	/* perserve conditions codes live info */
	    DELNODE(pf);		/* success.  delete first node */
	    return(true);
	}
    }
/* Address arithmetic:
**
** This transformation merges instructions of the form
**
**	addw2 &m,R
**
** with operands of the form
**
**	n(R)
**
** yielding a replacement operand of the form
**
**	m+n(R)
**
** We do this in a general way, transforming all of the operands in the
** instruction that follows the addw2 (subw2) that can be transformed.
** The transformation cannot be performed if the second instruction uses
** R in any other way.  Note that dyadic instructions like
**
**	addw2 a,R
**
** use R implicitly:  a + R -> R.
** However, for moves and triadics, the last operand is only a destination,
** not an operand, so having R as the last operand does not impede our
** transformation.
**
** In general we don't delete the addw2 (subw2) node but simply move it
** past its successor after transforming the operands.  At first this
** would seem to be a waste of time.  However, if R is dead after the
** second instruction, we can discard the addw2 (subw2).  Thus we
** can propagate the addw2 (subw2) through a series of offsets:
**
**	addw2 &4,%r0
**	movw 0(%r0),0(%r1)
**	movw 4(%r0),4(%r1)
**
**		|
**		V
**	movw 4(%r0),0(%r1)
**	addw2 &4,%r0
**	movw 4(%r0),4(%r1)
**
**		|
**		V
**	movw 4(%r0),0(%r1)
**	movw 8(%r0),4(%r1)
**	addw2 &4,%r0
**
** and the addw2 could be discarded if %r0 is dead now.  Notice that no
** particular performance penalty is incurred by doing this transformation
** if we fail to discard the addw2 (subw2), but we will win if we can
** throw it out.  NOTE, however, that we cannot move the add/sub if
** there is a conditional branch or compare following the second instruction,
** since we will provide different condition codes from the original sequence.
*/
/* Begin address arithmetic transformation */

    if (
	    (cop1 == ADDW2 || cop1 == SUBW2)
	&&  isnumlit(pf->op1)
	&&  isreg(pf->op2)
	&&  strcmp(pf->op2,"%sp") != 0	/* not stack pointer */
	&&  ! isbr(pl)			/* don't move add/sub past branch! */
	)
    {
	char * reg = pf->op2;		/* point at register name (R) */
	int destreg = 0;		/* number of destination operand,
					** zero if none
					*/
	int idxmask = 0;		/* bit mask of operands to change */
	long idxval[MAXOPS+1];		/* current indices for each (indexed
					** 1 to MAXOPS, not 0 to MAXOPS-1)
					*/
	int i;				/* loop index */
	long m = atol(pf->op1+1);	/* literal in addw2 &m,R */
	NODE * nextinst = pl->forw;	/* pointer to inst. after window */


/* Check for conditional branch or compare following second instruction in
** situation where we can't delete the add/sub.  Don't do improvement
** if we find one.
*/

	f = ( isdead(reg,pl) || strcmp(dst(pl),reg) == 0 );
	if ( (! f) && ( ! isdeadcc(pl) || iscompare(nextinst)) )
	    goto noindex;		/* won't delete and have cond. br. */
	
	if (cop1 == SUBW2)
	    m = -m;			/* negate literal if subtract */
	
/* Determine in which instructions we allow ourselves to see R as an
** operand (destination) and remember which operand number it is.
*/

	if (istriadic(pl,&opn,&opst))
	    destreg = 3;		/* in triadics, is third operand */
	else if (ismove(pl,&src1,&dst1))
	    destreg = 2;		/* in moves, is second */
	/* in all others, not allowed */

/* Now we loop through all possible operands in an instruction, looking
** for indexed uses of R (i.e., n(R) ).  If we find, instead, a use of
** R which is not in the "destreg" position, we cannot do the transform.
*/

	for (i = MAXOPS; i > 0; i--)
	{
	    char * t = pl->ops[i];	/* point at new operand */

	    if (t == NULL)
		continue;		/* disregard null operands */

	    if (isindex(t,reg))		/* this is what we seek */
	    {
		idxmask |= (1<<i);	/* remember where we saw it */
		if(*t == '*')
			t++;
		idxval[i] = atol(t);	/* remember current index value
					** (i.e., n from n(R)
					*/
	    }
	    else if (samereg(t,reg) && i != destreg)
		goto noindex;		/* instruction uses register in
					** non-transformable way
					*/
	}
/* We now know there have been only valid uses of R in the second
** instruction.  If there are any uses of R at all, transform the
** second instruction.
*/

/* Define maximum size of new operand. */

#define NEWSIZE	(11+1+3+1+1)		/* for "ddddddddddd(%rn)\0" */

	if (idxmask == 0)
	    goto noindex;		/* nothing to do if no uses of R */

	wchange();			/* we may just move/delete the add */
	for (i = MAXOPS; i>0; i--)	/* find operands to alter */
	{
	    char * t;
	    if ((idxmask & (1<<i)) == 0)
		continue;		/* ignore this operand:  not n(R) */

	    t = pl->ops[i];
	    pl->ops[i] = getspace(NEWSIZE);
					/* get space for new operand */
	    if(*t == '*')
		(void) sprintf(pl->ops[i],"*%d(%s)",(int)(m+idxval[i]),reg);
	    else
	    	(void) sprintf(pl->ops[i],"%d(%s)",(int)(m+idxval[i]),reg);
					/* build new operand string */
	}

/* Discard add/sub if R is dead after second instruction, or if that
** instruction set a new value.
*/

	if (f)				/* conditions permit deleting */
	    { 
	     ldelin(pf);		/* preserve line number info */ 
	     mvlivecc(pf);	/* perserve conditions codes live info */
	     DELNODE(pf); }		/* delete add/sub */
	else {	
	    lexchin(pf,pl);		/* preserve line number info */ 
	    exchange(pf);		/* otherwise exchange add/sub and
					** second inst.
					*/
	    swplivecc(pf,pl);	/* swaping the condition codes live/dead info */
	}
	return(true);
    }
noindex:
/* Address arithmetic, case 2:
**
**	addw3 &n,R1,R2
**	pushw R2		->	pushaw n(R1)
** or
**
**	subw3 &n,R1,R2
**	pushw R2		->	pushaw -n(R1)
**
**	if R2 dead after pushw
**
**	(Sigh!  These aren't safe in general, but for the specific cases
**	where R2 is sp, fp, or ap, we can be sure we're pushing an
**	address and not the result of an integer computation.  pushaw
**	doesn't do 32 bit arithmetic.)
*/

    if (
	    (cop1 == ADDW3 || cop1 == SUBW3)
	&&  cop2 == PUSHW
	&&  *pf->op1 == '&'
	&&  isreg(pf->op2)
	&&  (
		strcmp(pf->op2,"%fp") == 0
	    ||  strcmp(pf->op2,"%ap") == 0
	    ||  strcmp(pf->op2,"%sp") == 0
	    )
	&&  isreg(pf->op3)
	&&  samereg(pf->op3,pl->op1)
	&&  isdead(pf->op3,pl)
	)
    {
	/* allocate space for new operand:  room for constant, less room
	** for &, plus room for possible sign, plus room for parens and
	** register declaration, plus room for null.
	*/
	char * ts = getspace(	
				strlen(pf->op1)-1+1	/* less &, plus sign */
				+ 1 + strlen(pf->op2) + 1 /* ( reg ) */
				+ 1			/* null */
			    );
	
	char * operand = pf->op1 + 1;	/* point past & at operand */

	int msign = 0;			/* non-zero if minus sign required */

	wchange();

	/* create new operand */

	if (*operand == '-')
	{
	    msign = 1;			/* - sign required */
	    operand++;			/* skip past - in operand */
	}
	if (cop1 == SUBW3)		/* invert sign if op was subtract */
	    msign = 1-msign;

	(void) sprintf(ts,"%s%s(%s)",
		(msign != 0 ? "-" : ""),
		operand,		/* string for constant, no & */
		pf->op2);
	lmrgin1(pf,pl,pl);		/* preserve line number info */ 
	mvlivecc(pf);	/* perserve conditions codes live info */
	DELNODE(pf);			/* discard add/sub */
	chgop(pl,PUSHAW,"pushaw");	/* change op code */
	pl->op1 = ts;			/* change operand */
	return(true);
    }
/* Collapse bit test/branch combination
**
**	bitw	&2^n,O
**	je	to		->	jbc &n,O,to
**
**	bitw	&2^n,O
**	jne	to		->	jbs &n,O,to
**
**	jz is a synonym for je.
**	jnz is a synonym for jne.
*/

/* ----------- FLASH -------------
** It turns out that these "improvements" aren't:  they are slower than
** the sequences they would replace.  If someday that condition changes,
** remove the conditionals.
*/

#ifdef FASTJBCJBS

    if (
	    cop1 == BITW
	&&  (
		(f = (cop2 == JE))
	     ||	(f = (cop2 == JZ))
	     ||	cop2 == JNE
	     ||	cop2 == JNZ
	    )
	&&  (temp = getbit(pf->op1)) >= 0
	)
    {
	wchange();
	pl->op3 = pl->op1;		/* shift operands */
	pl->op2 = pf->op2;
	/* build new operand: &n */
	pl->op1 = getspace(1+2+1);	/* space for "&dd\0" */
	(void) sprintf(pl->op1,"&%d",temp);
	if (f)				/* f true for je/jz */
	    chgop(pl,JBC,"jbc");
	else
	    chgop(pl,JBS,"jbs");
	
	lmrgin1(pf,pl,pl);		/* preserve line number info */ 
	mvlivecc(pf);	/* perserve conditions codes live info */
	DELNODE(pf);			/* discard bitw */
	return(true);
    }

#endif	/* def FASTJBCJBS */

#ifdef UCODE50				/* with 5.0 microcode (3B20) only */

/* Increment and test improvement
**
**	addw2	&n,R
**	cmpw	R,O		->	inctst R,&n-1,O
**
**	if O doesn't use R (this may be unnecessarily restrictive,
**	depending on when the effective address of O is calculated,
**	but it would be a strange use, anyway)
**
*/

    if (
	    cop1 == ADDW2
	&&  cop2 == CMPW
	&&  isreg(pf->op2)
	&&  samereg(pl->op1,pf->op2)
	&&  isnumlit(pf->op1)
	&&  1 <= (templ = atol(pf->op1+1)) && templ <= 16
					/* can't use isnib here:  allow for
					** 16, disallow 0
					*/
	&&  ! usesreg(pl->op2,pf->op2)
	)
    {
	wchange();			/* making a change */
	chgop(pl,INCTST,"inctst");	/* new op code number, string */
	pl->op3 = pl->op2;		/* shift operand */
	pl->op2 = getspace(1+2+1);	/* for "&dd\0" */
	(void) sprintf(pl->op2,"&%d",(int)(templ-1));
					/* literal in inctmp is 1 less */
	lmrgin3(pf,pl,pl);		/* preserve line number info */ 
	DELNODE( pf );			/* delete ADDW2 */
	return(true);
    }
#endif	/* def UCODE50 */

#ifdef UCODE50				/* if 5.0 microcode (3B20) only */

/* Use increment/decrement pointer
**
**	llsw2	&1,R1
**	addw2	R1,R2		->	incpth R2,R1
**
**	llsw2	&2,R1
**	addw2	R1,R2		->	incptw R2,R1
**
**	llsw2	&1,R1
**	subw2	R1,R2		->	decpth R2,R1
**
**	llsw2	&2,R1
**	subw2	R1,R2		->	decptw R2,R1
**
**	if R1 dead after addw2/subw2
**	if R1 and R2 different
*/

    if (
	    cop1 == LLSW2
	&&  (cop2 == ADDW2 || cop2 == SUBW2)
	&&  isreg(pl->op1)
	&&  isreg(pl->op2)
	&&  isdead(pf->op2,pl)
	&&  samereg(pl->op1,pf->op2)
	&&  ! samereg(pl->op1,pl->op2)
	&&  ( 
		(f = (strcmp(pf->op1,"&2") == 0))
	    ||  strcmp(pf->op1,"&1") == 0
	    )
	)
    {
	wchange();
	pl->op1 = pl->op2;		/* switch operands */
	pl->op2 = pf->op2;

/* select new op code for merged instruction */

	if (cop2 == ADDW2)		/* do incpt cases */
	{
	    if (f)			/* true for word incr. */
		chgop(pl,INCPTW,"incptw");
	    else
		chgop(pl,INCPTH,"incpth");
	}
	else				/* decpt case */
	{
	    if (f)			/* true for word decr. */
		chgop(pl,DECPTW,"decptw");
	    else
		chgop(pl,DECPTH,"decpth");
	}

	lmrgin1(pf,pl,pl);		/* preserve line number info */ 
	DELNODE( pf );			/* delete shift */
	return(true);
    }
#endif	/* def UCODE50 */
/********************************************************************
**
**	Begin improvements that alter code, rather than deleting it.
**
***********************************************************************
*/

/* Use register in pushw if possible
**
**	movw R,O		->	movw R,O
**	pushw O			->	pushw R
**
** if O is not a register.
*/

    if (
	     cop1 == MOVW
	&&   cop2 == PUSHW
	&&   strcmp(pf->op2,pl->op1) == 0
	&&   isreg(pf->op1)
	&&   !isreg(pl->op1)
	&&   isiros(pf->op2)		/* safe from mmio */
	)
    {
	wchange();
	makelive((pl->op1 = pf->op1),pl); /* propagate liveness of R */
	return(true);
    }
/* Use register if possible in op2 or op3
**
**	movw R,O1		->	movw R,O1
**	op2 O1,O2		->	op2 R,O2
**
**	if O1 not a register
** or
**
**	movw R,O1		->	movw R,O1
**	op3 O1,O2,O3		->	op3 R,O2,O3
**
**	if O1 is not a register
** or
**
**	movw R,O2		->	movw R,O2
**	op3 O1,O2,O3		->	op3 O1,R,O3
**
**	if O2 is not a register
*/

    if (
	    cop1 == MOVW
	&&  isreg(pf->op1)
	&&  ! isreg(pf->op2)
	)
    {
	if (
	       (
		    isdyadic(pl)
		&&  (f = (strcmp(pf->op2,pl->op1) == 0))
		&&  (CMPW <= cop2 && cop2 <= UMODW3)
		&&  ( ismove( pl, &src2, &dst2 ) && src2 == stype(cop1) ||
		     !ismove( pl, &src2, &dst2 ) && stype(cop2) == stype(cop1)
		    )
	        &&  isiros(pf->op2)		/* safe from mmio */
		)
	    ||
		(
		    istriadic(pl,&opn,&opst)
		 && (
			(f = (strcmp(pf->op2,pl->op1) == 0))
		     ||	strcmp(pf->op2,pl->op2) == 0
		    )
	         &&  isiros(pf->op2)		/* safe from mmio */
		)
	    )
	{
	    wchange();
	    if (f)			/* f true if we modify first operand */
		pl->op1 = pf->op1;
	    else
		pl->op2 = pf->op1;
	    makelive(pf->op2,pl);	/* propagate liveness into op2/3 */
	    return(true);
	}
    }
/* This transformation propagates a register use back into a triadic.
**
**	op3 O1,O2,O3		->	op3 O1,O2,R
**	movw O3,R		->	movw R,O3  (possibly deleted)
**
**	if R not used by O3
**
**	If O3 is a register that is dead after movw, we can change
**	this to just the op3.  If O3 is a register that is not dead,
**	we can't do the transformation, because it will oscillate.
*/

    if (
	    istriadic(pf,&opn,&opst)
	&&  cop2 == MOVW
	&&  isreg(pl->op2)
	&&  strcmp(pf->op3,pl->op1) == 0
	&&  ! usesreg(pf->op3,pl->op2)
	&&  (				/* avoid oscillation case */
		! ( f = isreg(pl->op1) )
	     || isdead(pl->op1,pl)
	     )
	&&  isiros(pf->op3)		/* safe from mmio */
	)
    {
	wchange();

	pf->op3 = pl->op2;
	makelive(pf->op3,pf);		/* make R live after op3 */

	lmrgin2(pf,pl,pf);		/* preserve line number info */ 
					/* lmrgin2 works here
					   because it sets the uniqid of the
					   non-result (i.e. pl) to IDVAL */
	if (f)				/* true if O3 was a register */
      	      { mvlivecc(pl);	/* preserve conditions codes live info */
	      DELNODE(pl); }
	else				/* O3 not a register */
	{
	    pl->op2 = pl->op1;
	    pl->op1 = pf->op3;		/* pf->op3 contains R from first move */
	}
	return(true);
    }
/* Use register in second of two moves, if possible.
**
** This improvement attempts to propagate a register use, if possible.
** We do this in a general way for pairs of moves.  Here's the case
** we're considering:
**
**		movX R,O1	->	movX R,O1
**		movY O1,O2	->	movY R,O2
**
** We don't bother with this improvement if O1 is already a register.
**
** We can make the identical improvement if "R" is really a nibble.
**
** We can do this when (using earlier terminology) the destination
** of movX and the source of movY are the same size.  There are three
** sub-cases:
**
**	1.  destination1 == source2 == destination2.
**	2.  source1 >= destination1 == source2 .
**		(remember, source1 is register or nibble)
**	3.  source1 >= destination2.
**
** Examples:
**
**		movbbh R,O1				s = 1; d = 2
**		movthb O1,O2	->	movb R,O2	s = 2; d = 1
**
**		movbbh R,O1				s = 1; d = 2
**		movh O1,O2	->	movbbh R,O2	s = 2; d = 2
**
*/

    if (
	    ismove(pf, &src1, &dst1)
	&&  ismove(pl, &src2, &dst2)
	&&  src2 == dst1
	&&  ( isreg(pf->op1) || isnib(pf->op1) ) /* R can be reg. or nibble */
	&&  ! isreg(pf->op2)		/* this is "don't bother" test */
	&&  strcmp(pf->op2, pl->op1) == 0
	&&  isiros(pf->op2)		/* safe from mmio */
	)
    /* fall through to sub-cases */
    {

    /* sub-case 1:  source2 and destination2 same */

	if (src2 == dst2)
	{
	    wchange();
	    chgop(pl,pf->op,pf->opcode); /* copy instruction of first node */
	    pl->op1 = pf->op1;		/* propagate register/nibble use */
	    makelive(pl->op1,pl);	/* mark it as live */
	    return(true);
	}

    /* sub-case 2:  source1 >= destination1 */

	if (src1 >= dst1)
	{
	    wchange();
	    pl->op1 = pf->op1;		/* propagate register/nibble */
	    makelive(pl->op1,pl);	/* make register live after pl */
	    return(true);
	}

    /* sub-case 3:  source1 >= destination2 */

	if (src1 >= dst2)
	{
	    wchange();
	    switch (dst2)		/* choose correct new instruction */
	    {
	    case 1:
		chgop(pl,MOVB,"movb"); break;
	    case 2:
		chgop(pl,MOVH,"movh"); break;
	    case 4:
		chgop(pl,MOVW,"movw"); break;
	    }
	    pl->op1 = pf->op1;		/* propagate register/nibble use */
	    makelive(pl->op1,pl);	/* make this register live here */
	    return(true);
	}
    }
/* More paired move improvements
**
** This improvement propagates the use of a register, if possible from
** one move to a following one.  It also has the potential of killing
** off the liveness of a register.  Example:
**
**	movX O,R1		->	movX O,R1
**	movY O,R2		->	movY R1,R2
**
**	Since we are trying to kill registers off early, these special
**	conditions apply if O is a register:
**		1)  O must be dead after movY or be the same as R2.
**		2)  R1 must be live after movY.
**	If O is a register, it must be live after movY or be the same
**	register as R2.
**	O may not use R1.
**
**	The size of source1 must be the same as source2,
**	and destination1 must be at least as large as source2.
*/

    if (
	    ismove(pf,&src1,&src2)
	&&  ismove(pl,&src2,&src2)
	&&  strcmp(pf->op1,pl->op1) == 0
	&&  isreg(pf->op2)
	&&  isreg(pl->op2)
	&&  ! usesreg(pl->op1,pf->op2)
	&&  src1 == src2
	&&  src2 <= dst1
	&&  (
		! isreg(pl->op1)		/* O not a register */
	    ||  (
		    (
			samereg(pl->op1,pl->op2)/* O same as R2 */
		    ||  isdead(pl->op1,pl)	/* O dead after movY */
		    )
		
		&&  ! isdead(pf->op2,pl)	/* R1 live after movY */
		)
	    )
	&&  isiros(pf->op1)		/* safe from mmio */
	)
    {
	wchange();
	makelive(pl->op1 = pf->op2,pf); /* copy operand, make register live
					** after movX
					*/
	return(true);
    }
/* Re-order pairs of instructions to make better use of registers.
**
** This improvement reverses a dyadic and a movw to make better use of
** registers.  The canonical sequence is:
**
**	op2 O1,O2		->	movw O1,R
**	movw O1,R		->	op2 R,O2
**
**	if O1 and O2 don't use R
**	if R live after movw
**	if instruction following movw is not a conditional branch
**	(since we're changing the instruction that sets codes)
**
**	op2 can also be CMPW
*/

    if (
	    (     cop1 == CMPW
	     ||  (   isdyadic(pf) 
		  && ! ismove(pf,&src1,&dst1)
		 )
	    )
				/* since moves are handled elsewhere
				** and this would tend to undo them
				*/
	&&  cop2 == MOVW
	&&  strcmp(pf->op1,pl->op1) == 0
	&&  isreg(pl->op2)
	&&  ! isdead(pl->op2,pl)
	&&  ! usesreg(pf->op1,pl->op2)
	&&  ! usesreg(pf->op2,pl->op2)
	&&  ! usesvar(pf->op1,pf->op2)	/* O1 cannot use O2 in any way */
	&&  isdeadcc( pl )
	&&  isiros(pf->op1)		/* safe from mmio */
	)
    {
	wchange();
	pf->op1 = pl->op2;		/* copy operand first */
	lexchin(pf,pl);		/* preserve line number info */ 
	exchange(pf);			/* switch the two instructions */
	swplivecc(pf,pl);    /* switch the live/dead info on condition codes */
	makelive(pl->op2,pf);		/* show register as live after op2 */
	return(true);
    }
/* Similar to the above, this transformation reverses a triadic and a
** movw:
**
**	op3 O1,O2,O3		->	movw [O1 | O2],R
**	movw [O1 | O2],R	->	op3 [O1 | R], [O2 | R], O3
**
** That is, it inserts a register reference into op3.
** R must not be used in O1, O2, or O3.
** O3 cannot be the same as R, nor can O1/O2 use O3 in any way.
** Instruction after movw must not be conditional branch, since we're
** setting different condition codes.
** Don't bother if R dead after movw, since the inst. will go away eventually.
*/

    if (
	    istriadic(pf,&opn,&opst)
	&&  cop2 == MOVW
	&&  isreg(pl->op2)
	&&  ! isdead(pl->op2,pl)
	&&  (
		(f = (strcmp(pl->op1,pf->op1) == 0))
	    ||  strcmp(pl->op1,pf->op2) == 0
	    )
	&&  ! usesreg(pf->op1,pl->op2)
	&&  ! usesreg(pf->op2,pl->op2)
	&&  ! usesreg(pf->op3,pl->op2)
	&&  ! usesvar(pl->op1,pf->op3)
	&&  ! usesvar(pl->op2,pf->op3)
	&&  isdeadcc( pl )
	&&  isiros(pl->op1)		/* safe from mmio */
	)
    {
	wchange();
	if (f)				/* true if first operand is R ref. */
	    pf->op1 = pl->op2;
	else
	    pf->op2 = pl->op2;		/* second operand refers to R */
	makelive(pl->op2,pf);		/* R now (will be) live after op3
					** since we knew it was live after movw
					*/
	lexchin(pf,pl);		/* preserve line number info */ 
	exchange(pf);			/* exchange the instructions */
	swplivecc(pf,pl);    /* switch the live/dead info on condition codes */
	return(true);
    }
/* Merge successive adds.
**
** This improvement occurs in address arithmetic and in some funny
** expressions.
**
**	addw3 X,Y,R1		->	addw2 X,R2
**	addw2 R1,R2		->	addw2 Y,R2
**
**	if R1 dead after addw2, R1 and R2 are different.
**	neither X nor Y can use R1
**
** Unfortunately, we learn the hard way of special cases we hadn't
** thought of earlier, to wit:
**
**	addw3 X,R2,R1		->	addw2 R2,R2
**	addw2 R1,R2		->	addw2 X,R2
**
** If X is R2, we can't do this at all!
*/

    if (
	    cop1 == ADDW3
	&&  cop2 == ADDW2
	&&  isdead(pf->op3,pl)		/* also tests register-ness */
	&&  samereg(pf->op3,pl->op1)
	&&  isreg(pl->op2)
	&&  ! samereg(pl->op1,pl->op2)
	&&  strcmp(pf->op1,pf->op2) != 0 /* different operands */
	&&  ! usesreg(pf->op1,pf->op3)
	&&  ! usesreg(pf->op2,pf->op3)
	)
    {
	wchange();
	if (! samereg(pl->op2,pf->op2))	/* check special case */
	    pl->op1 = pf->op2;		/* not special:  move Y to 2nd inst. */
	else
	{
	    pl->op1 = pf->op1;		/* special case:  move X to 2nd inst. */
	    pf->op1 = pf->op2;		/* move R2 over in first */
	}
	pf->op2 = pl->op2;		/* move ultimate dest. into 1st inst. */
	pf->op3 = NULL;			/* clean up 1st inst. */
	chgop(pf,ADDW2,"addw2");	/* change op code number and string */
	lmrgin2(pf,pl,pf);		/* preserve line number info */ 
					/* again this works because pl->uniqid
					  is set to IDVAL */
	return(true);
    }
/* Reverse adds/subtracts of literals and other adds/subtracts.
**
** This transformation facilitates other improvements by bubbling
** adds/subtracts of literals downward and other adds/subtracts upwards.
**
**	addw2	&n,R		->	addw2	O,R
**	addw2	O,R		->	addw2	&n,R
**
**	if R not used in O, O not a literal.
*/

    if (
	    (cop1 == ADDW2 || cop1 == SUBW2)
	&&  (cop2 == ADDW2 || cop2 == SUBW2)
	&&  isreg(pf->op2)
	&&  samereg(pf->op2,pl->op2)
	&&  *pf->op1 == '&'
	&&  ! usesreg(pl->op1,pl->op2)
	&&  *pl->op1 != '&'
	)
    {
	wchange();
	lexchin(pf,pl);		/* preserve line number info */ 
	exchange(pf);			/* just exchange instructions */
	swplivecc(pf,pl);    /* switch the live/dead info on condition codes */
	return(true);
    }
/* Reverse adds/subtracts of literals and moves.
**
** This transformation facilitates address arithmetic transformations.
**
**	addw2 &n,R		->	movX O1,O2
**	movX O1,O2		->	addw2 &n,R
**
**	if neither O1 nor O2 uses R
**	if instruction following movX is not conditional branch, since we
**	  will set different condition codes
*/

    if (
	    (cop1 == ADDW2 || cop2 == SUBW2)
	&&  isreg(pf->op2)
	&&  strcmp(pf->op2,"%sp") != 0	/* not stack pointer */
	&&  isnumlit(pf->op1)		/* only useful with numeric lits. */
	&&  ismove(pl,&src1,&dst1)
	&&  ! (usesreg(pl->op1,pf->op2) || usesreg(pl->op2,pf->op2))
	&&  isdeadcc( pl )
	)
    {
	wchange();
	lexchin(pf,pl);		/* preserve line number info */ 
	exchange(pf);			/* interchange instructions */
	swplivecc(pf,pl);    /* switch the live/dead info on condition codes */
	/* leave live/dead information alone */
	return(true);
    }
    return(false);
}
