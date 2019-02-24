/* @(#) w1opt.c: 1.4 3/27/84				*/
/* w1opt.c
**
**	3B20S optimizer:  for one-instruction window
**
**
*/

/* #include "defs" -- optim.h takes care of this */
#include "optim.h"
#include "optutil.h"


/*	D A N G E R
**
** This definition selects the highest numbered register that we
** can arbitrarily choose as a temporary in the multiply strength
** reduction that is performed below.  It should equal the highest
** numbered temporary register used by the compiler (1 less than
** lowest numbered register used for register variables.
*/

#define	TEMPREG		3		/* highest temp. reg. to use */
/* w1opt -- one-instruction optimizer
**
** This routine handles the single-instruction optimization window.
** See individual comments below about what's going on.
** In some cases (which are noted), the optimizations are ordered.
*/

boolean					/* true if we make any changes */
w1opt(pf,pl)
register NODE * pf;			/* pointer to first instruction in
					** window (and last)
					*/
NODE * pl;				/* pointer to last instruction in
					** window (= pf)
					*/
{

    register int cop = pf->op;		/* single instruction's op code # */
    int opn;				/* temporary op code number */
    char * opst;			/* temporary op code string */
    boolean retval = false;		/* return value:  in some cases
					** we fall through after completing
					** an optimization because it could
					** lead into others.  This variable
					** contains the return state for the
					** end.
					*/
    char * dest;			/* destination string, used below */
    long mult;				/* multiplier, used below */
    boolean f;				/* random boolean flag, used below */
    long templ;				/* general long temporary */
/* eliminate dead code:
**
**	op2 O1,R	or	op3 O1,O2,R
**		where R is dead
*/

    if (
	    isdead(dst(pf),pf) 		/* we don't really care what the op
					** is
					*/
	&&  ! isbr(pf)			/* but some branches set variables
					** and jump:  keep them
					*/
	&&  isdeadcc( pf )		/* this inst. may have side effects
					** relied upon by cond. branch
					*/
	&&  isiros( pf->op1 )
	&&  isiros( pf->op2 )		/* are operands safe for mmio */
	)
    {
	wchange();			/* Note we're changing the window */
	ldelin2(pf);			/* preserve line number info */
	mvlivecc(pf);			/* preserve condition codes line info */
	DELNODE(pf);			/* discard instruction */
	return(true);			/* announce success */
    }

/*
**	cmpw R,&0	->	movw R,R
**
** This wins on the 3B20S, but might not on other 3B processors.
*/

/* Although this improvement is a small win, it messes up other
** transformations and generally confuses things.  It's probably
** better to leave it turned off.
*/

#ifdef CMPWTOMOVW

    if (
	    cop == CMPW
	&&  isreg(pf->op1)
	&&  strcmp(pf->op2,"&0") == 0
	)

    {
	wchange();			/* note change */
	chgop(pf,MOVW,"movw");		/* change the op code */
	pf->op2 = pf->op1;		/* both operands point at R */
	makelive(pf->op2,pf);		/* make register appear to be live
					** hereafter so "compare" isn't thrown
					** away.  (Otherwise R might be dead
					** and we would eliminate the inst.)
					*/
	return(true);			/* made a change */
    }

#endif /* def CMPWTOMOV */
/* get rid of useless arithmetic
**
**	addw2	&0,O		->	deleted  or  cmpw O,&0
**	subw2	&0,O		->	deleted  or  cmpw O,&0
**	orw2	&0,O		->	deleted  or  cmpw O,&0
**	xorw2	&0,O		->	deleted  or  cmpw O,&0
**	alsw2	&0,O		->	deleted  or  cmpw O,&0
**	arsw2	&0,O		->	deleted  or  cmpw O,&0
**	llsw2	&0,O		->	deleted  or  cmpw O,&0
**	lrsw2	&0,O		->	deleted  or  cmpw O,&0
**	mulw2	&1,O		->	deleted  or  cmpw O,&0
**	umulw2	&1,O		->	deleted  or  cmpw O,&0
**	divw2	&1,O		->	deleted  or  cmpw O,&0
**	udivw2	&1,O		->	deleted  or  cmpw O,&0
**	andw2	&-1,O		->	deleted  or  cmpw O,&0

**	addw3	&0,O1,O2	->	movw O1,O2
**	subw3	&0,O1,O2	->	movw O1,O2
**	orw3	&0,O1,O2	->	movw O1,O2
**	xorw3	&0,O1,O2	->	movw O1,O2
**	alsw3	&0,O1,O2	->	movw O1,O2
**	arsw3	&0,O1,O2	->	movw O1,O2
**	llsw3	&0,O1,O2	->	movw O1,O2
**	lrsw3	&0,O1,O2	->	movw O1,O2
**	mulw3	&1,O1,O2	->	movw O1,O2
**	umulw3	&1,O1,O2	->	movw O1,O2
**	divw3	&1,O1,O2	->	movw O1,O2
**	udivw3	&1,O1,O2	->	movw O1,O2
**	andw3	&-1,O1,O2	->	movw O1,O2
**
**	mulw2	&0,O		->	movw &0,O
**	umulw2	&0,O		->	movw &0,O
**	andw2	&0,O		->	movw &0,O
**	mulw3	&0,O1,O2	->	movw &0,O2
**	umulw3	&0,O1,O2	->	movw &0,O2
**	andw3	&0,O1,O2	->	movw &0,O2
**
**	xorw2	&-1,O		->	mcomw O,O
**	xorw3	&-1,O1,O2	->	mcomw O1,O2
**
**	mulw2	&-1,O		->	mnegw O,O
**	divw2	&-1,O		->	mnegw O,O
**	mulw3	&-1,O1,O2	->	mnegw O1,O2
**	divw3	&-1,O1,O2	->	mnegw O1,O2
**
** Note that since we've already gotten rid of dead code, we won't
** check whether O (O2) is live.  However, we must be careful to
** preserve the sense of result indicators if a conditional branch
** follows some of these changes.
*/

/* Define types of changes we will make.... */

#define	UA_NOP	1			/* no change */
#define UA_DEL	2			/* delete instruction */
#define UA_MOV	3			/* change to move */
#define UA_MOVZ	4			/* change to move zero to ... */
#define UA_MCOM	5			/* change to move complemented */
#define UA_MNEG	6			/* change to move negated */
/* We must have a literal as the first operand, and its value must fit
** in an integer.  We check the latter condition by comparing the converted
** value from atol to the same value cast as an int:  they must agree.
** (This is a particular problem for DMERT when they run the optimizer
** on a PDP-11/70 to optimize 3B20 code.)
*/
    if (    isnumlit(pf->op1)
	&&  (templ = atol(pf->op1+1)) == (long)((int) templ)
	)
    {
	int ultype = UA_NOP;		/* initial type of change = none */

	switch((int) templ)		/* branch on literal */
	{
	case 0:				/* handle all instructions with &0
					** as first operand
					*/
	    switch (cop)
	    {
	    case ADDW2:
	    case SUBW2:
	    case ORW2:
	    case XORW2:
	    case ALSW2:
	    case ARSW2:
	    case LLSW2:
	    case LRSW2:
		if( !isiros( pf->op2 ) ) break;
		ultype = UA_DEL;
		break;
					/* if safe from mmio,
					** delete all of these */
	    
	    case ADDW3:
	    case SUBW3:
	    case ORW3:
	    case XORW3:
	    case ALSW3:
	    case ARSW3:
	    case LLSW3:
	    case LRSW3:
		ultype = UA_MOV;	/* convert to simple moves */
		break;

	    case MULW2:
	    case UMULW2:
	    case ANDW2:
		ultype = UA_MOVZ;	/* convert to move zero */
		break;

	    case MULW3:
	    case UMULW3:
	    case ANDW3:
		if( !isiros( pf->op2 ) ) break;
		ultype = UA_MOVZ;	
		break;
					/* if safe from mmio,
					** convert to move zero */
	    }
	    break;			/* done &0 case */

	case 1:				/* &1 case */
	    switch( cop )		/* branch on op code */
	    {
	    case DIVW2:
	    case UDIVW2:
	    case MULW2:
	    case UMULW2:
		if( !isiros( pf->op2 ) ) break;
		ultype = UA_DEL;	
		break;
					/* if safe from mmio, 
					** delete these */
	    
	    case DIVW3:
	    case UDIVW3:
	    case MULW3:
	    case UMULW3:
		ultype = UA_MOV;	/* convert these to moves */
		break;
	    }
	    break;			/* done &1 case */
	
	case -1:			/* &-1 case */
	    switch ( cop )		/* branch on op code */
	    {
	    case ANDW2:
		if( !isiros( pf->op2 ) ) break;
		ultype = UA_DEL;	
		break;
					/* if safe from mmio,
					** delete this */
	    
	    case ANDW3:
		ultype = UA_MOV;	/* change to move */
		break;
	    
	    case XORW2:
	        if( !isiros( pf->op2 ) ) break;
		ultype = UA_MCOM;
		break;
					/* if safe from mmio,
					** change to move complemented */

	    case XORW3:
		ultype = UA_MCOM;	/* change to move complemented */
		break;
	    
	    case MULW2:
	    case DIVW2:
	        if( !isiros( pf->op2 ) ) break;
	        ultype = UA_MNEG;
	        break;
					/* if safe from mmio,
					** change to move complemented */
	    case MULW3:
	    case DIVW3:
		ultype = UA_MNEG;	/* change to move negated */
		break;
	    }
	    break;			/* end &-1 case */
	} /* end switch on immediate value */
/* Now do something, based on selections made above */

	switch ( ultype )
	{
	case UA_MOV:			/* change instruction to move */
	    wchange();			/* changing window */
	    pf->op1 = pf->op2;		/* shift operands */
	    pf->op2 = dst(pf);
	    pf->op3 = NULL;		/* in case we removed it */
	    chgop(pf,MOVW,"movw");	/* change op code */
	    retval = true;		/* made a change */
	    break;
	
	case UA_MOVZ:			/* change to move zero to operand */
	    wchange();
	    pf->op1 = "&0";		/* first operand is zero */
	    pf->op2 = dst(pf);		/* second is ultimate destination */
	    pf->op3 = NULL;		/* clean out if there was one */
	    chgop(pf,MOVW,"movw");	/* change op code */
	    retval = true;		/* made a change */
	    break;
	
	case UA_MCOM:			/* change to move complemented */
	    wchange();
	    pf->op1 = pf->op2;		/* shift operands */
	    pf->op2 = dst(pf);
	    pf->op3 = NULL;
	    chgop(pf,MCOMW,"mcomw");	/* change op code */
	    retval = true;		/* made a change */
	    break;
	
	case UA_MNEG:			/* change to move negated */
	    wchange();
	    pf->op1 = pf->op2;		/* shift operands */
	    pf->op2 = dst(pf);
	    pf->op3 = NULL;
	    chgop(pf,MNEGW,"mnegw");	/* change op code */
	    retval = true;		/* made a change */
	    break;
/* For this case we must be careful:  if a following instruction is a
** conditional branch, it is clearly depending on the result of the
** arithmetic, so we must put in a compare against zero instead of deleting
** the instruction.
*/

	case UA_DEL:			/* delete instruction */

	    wchange();			/* we will make a change */

	    if ( ! isdeadcc(pf) )
	    {
		chgop(pf,CMPW,"cmpw");
		pf->op1 = pf->op2;	/* always test second operand */
		pf->op2 = "&0";		/* compare to zero */
		pf->op3 = NULL;		/* for completeness */
		retval = true;		/* made a change */
	    }
	    else
	    {
		ldelin2(pf);		/* preserve line number info */
		mvlivecc(pf);		/* preserve condition codes line info */
		DELNODE(pf);		/* not conditional; delete node */
		return(true);		/* say we changed something */
	    }
	    break;
	} /* end case that decides what to do */
	
	cop = pf->op;			/* reset current op for changed inst. */

    } /* end useless arithmetic removal */
/* discard useless movw's
**
**	movw	O,O		->	deleted
**
** The movw must not be followed by a conditional jump, since we
** must leave the condition codes set.  Note that this improvement
** picks up some strange code generated above, like
**
**	mulw3	&1,%r0,%r0	->	movw	%r0,%r0
**
*/

    if (    pf->op == MOVW
	&&  strcmp(pf->op1,pf->op2) == 0
	&&  isdeadcc(pf)
	&&  isiros(pf->op1)		/* safe from mmio */
	)
    {
	wchange();			/* changing the window */
	ldelin2(pf);			/* preserve line number info */
	mvlivecc(pf);			/* preserve condition codes line info */
	DELNODE(pf);			/* delete the movw */
	return(true);
    }
/* change triadics to dyadics if possible
**
**	op3 O1,O2,O2		->	op2 O1,O2
**
*/

    if (istriadic(pf,&opn,&opst) 
	&& strcmp(pf->op2,pf->op3) == 0
	&& isiros(pf->op2) 		/* safe from mmio */
	)

    /* triadic and last two operands match */

    {
	wchange();			/* we're making a change */
	chgop(pf,opn,opst);		/* change the op code */
	pf->op3 = NULL;			/* so we don't keep looking at 3rd
					** operand
					*/
	retval = true;			/* remember that we made a change,
					** but don't exit, as the next
					** optimization may also apply.
					*/
	cop = opn;			/* changed op code */
    }
    /* falling through either way !! */
/* change multiplies and divides to shifts if power of 2 */

/*
**	udivw2 &2^n,O		->	lrsw2 &n,O
**	udivw3 &2^n,O1,O2	->	lrsw3 &n,O1,O2
**	mulw2 &2^n,O		->	arsw2 &n,O
**	mulw3 &2^n,O1,O2	->	arsw3 &n,O1,O2
**	umulw2 &2^n,O		->	llsw2 &n,O
**	umulw3 &2^n,O1,O2	->	llsw3 &n,O1,O2
**
** Note that signed divide cannot safely be altered with these
** transformations!
*/

    switch (cop)			/* dispatch on type */
    {
	int bit;			/* temporary bit number if 2^n */

    case UDIVW2:
    case UDIVW3:
    case MULW2:
    case MULW3:
    case UMULW2:
    case UMULW3:

	if ( (bit = getbit(pf->op1)) < 0) /* if not power of 2, done this */
	    break;

	wchange();			/* about to change window */
	pf->op1 = getspace(1+2+1);	/* room for &dd\0 */
	(void) sprintf(pf->op1,"&%d",bit); /* write shift amount */
	switch (cop)			/* now change op code as needed */
	{
	case UDIVW2:
	    chgop(pf,LRSW2,"lrsw2"); break;
	case UDIVW3:
	    chgop(pf,LRSW3,"lrsw3"); break;
	case MULW2:
	    chgop(pf,ALSW2,"alsw2"); break;
	case MULW3:
	    chgop(pf,ALSW3,"alsw3"); break;
	case UMULW2:
	    chgop(pf,LLSW2,"llsw2"); break;
	case UMULW3:
	    chgop(pf,LLSW3,"llsw3"); break;
	}
	retval = true;			/* we changed something */
	cop = pf->op;			/* changed op code, too */
    }

#ifndef M32			/* applies only to 3B20S */

/* The 3B20 can do shifts and adds faster than multiplies for small
** integer multipliers where the result ends up in a register.  Here
** we pick such multiplies apart, arbitrarily stopping at 10 as an
** upper bound.
*/

    if (
	    (
		cop == MULW2
	    ||  cop == MULW3
	    )
	&&  *pf->op1 == '&'		/* multiplier is small literal */
	&&  isreg(dest = (cop == MULW2 ? pf->op2 : pf->op3))
					/* remember dest.; must be reg. */
	&&  (mult = atol(pf->op1+1)) > 2/* positive multiplier (exclude
					** 0, 1, 2
					*/
	&&  mult <= 10			/* arbitrary upper limit */
	)
    {
	static char regstring[] = "%rx";/* boiler-plate register name */
	char * temp = NULL;		/* string representing temp. used
					** during "multiply"
					*/
	NODE * new;			/* pointer to new instruction node */

/* The approach works like this:
**
**	1.  Identify destination register and temporary register that
**		we will need to use.
**	2.  Determine live/dead data for new instructions.
**	3.  Add new instructions after the MULW_, but holding on to the
**		MULW_ as an anchor (since pf points at it).
**	4.  Delete MULW_ when done.
**
**	Note that the instruction sequences we create always set the
**	result indicators the same as if a multiply had been done.
*/

/* Macro to add new instruction:
**	ptr	points to instruction to add after
**	opn	op code number of new instruction
**	opst	op code string of instruction
**	opn1	operand 1 for new instruction
**	opn2	operand 2 for new instruction
**	ld	live/dead data for new instruction
*/

#define	addinst(ptr,opn,opst,opn1,opn2,ld) \
    { \
	last = insert(ptr);		/* get new node */ \
	chgop(last,opn,opst);		/* put in op code number, string */ \
	last->op1 = opn1;		/* put in operands */ \
	last->op2 = opn2;		\
	last->nlive = ld;		/* put in live/dead info. */ \
    }
/* Identify temporary and destination registers.
**
** Destination is already in 'dest':  the destination of the MULW_.
** 'temp' must be chosen for MULW2 or for MULW3 where the second
** operand is not already in a register.
*/

	/* f will remember whether we needed to "create" a temporary.
	** We must do so for MULW2 or for MULW3 when 2nd operand not a reg.
	*/

	if ( f = (
		    cop == MULW2	/* inst. is MULW2 */
		 ||  ! isreg(pf->op2)	/* MULW3 2nd operand not reg. */
		 )
	    )

	{
	    int j;			/* register number */

	    /* We're going to loop, trying to find a register to steal.
	    ** Then we save a copy of the appropriate register string for
	    ** the instructions we'll build.
	    */

	    for ( j = '0' + TEMPREG; j >= '0'; j--) /* go down from highest */
	    {
		regstring[2] = (char) j; /* stick char in string */
		if (isdead(regstring,pf)) /* if dead, we can use it */
		{
		    temp = strcpy(getspace(sizeof regstring),regstring);
		    break;
		}
	    }
	} /* end if that builds a temp. string */
	else				/* MULW3 had reg. as 2nd operand */
	    temp = pf->op2;		/* point at it as suitable temporary */

/* 'dest' and 'temp' now point at suitable strings representing registers.
** Although we're not sure about the multiplier value, we know powers of
** 2 have already been handled.  Therefore there is no danger of allocating
** a string above without actually using it.
*/
/* Build correct instruction sequence. */

	if (temp != NULL)		/* make sure we got a temporary */
	{
	    NODE * last = pf;		/* remember last node in sequence */
	    int ld;			/* current live/dead bits */

	    wchange();			/* tell the world we're changing */

	/* Set up live/dead data.  We want to replicate the information in
	** the current multiply node, plus we want to set 'temp' and 'dest'
	** registers live.
	*/

	    makelive(dest,pf);		/* set destination live */
	    ld = pf->nlive;		/* remember data */

	    if (cop == MULW3)		/* On MULW3 we must load the dest. */
		addinst(last,MOVW,"movw",pf->op2,dest,ld);
	    
	/* Now we change the live/dead information so 'temp' will appear live */

	    makelive(temp,pf);
	    ld = pf->nlive;

	/* If we created a temp, move the current destination (multiplicand)
	** there now.
	*/

	    if (f)
		addinst(last,MOVW,"movw",dest,temp,ld);

	/* Now generate instruction sequences based on multiplier. */

	    switch ((int) mult)		/* value known to be between 2, 10 */
	    {
	    case 3:
		addinst(last,LLSW2,"llsw2","&1",dest,ld);	/* *2 */
		addinst(last,ADDW2,"addw2",temp,dest,ld);	/* *3 */
		break;
	    case 5:
		addinst(last,LLSW2,"llsw2","&2",dest,ld);	/* *4 */
		addinst(last,ADDW2,"addw2",temp,dest,ld);	/* *5 */
		break;
	    case 6:
		addinst(last,LLSW2,"llsw2","&2",dest,ld);	/* *4 */
		addinst(last,ADDW2,"addw2",temp,dest,ld);	/* *5 */
		addinst(last,ADDW2,"addw2",temp,dest,ld);	/* *6 */
		break;
	    case 7:
		addinst(last,LLSW2,"llsw2","&3",dest,ld);	/* *8 */
		addinst(last,SUBW2,"subw2",temp,dest,ld);	/* *7 */
		break;
	    case 9:
		addinst(last,LLSW2,"llsw2","&3",dest,ld);	/* *8 */
		addinst(last,ADDW2,"addw2",temp,dest,ld);	/* *9 */
		break;
	    case 10:
		addinst(last,LLSW2,"llsw2","&3",dest,ld);	/* *8 */
		addinst(last,ADDW2,"addw2",temp,dest,ld);	/* *9 */
		addinst(last,ADDW2,"addw2",temp,dest,ld);	/* *10 */
		break;
	    }
	/* Done generating instructions.  Now clean up.
	** First, fix up live/dead data so temp is dead after last
	** instruction.  Then kill old node.
	*/

	    makedead(temp,last);
	    ldelin(pf);		/* preserve line number info */
	    DELNODE(pf);		/* delete the multiply */
	    return(true);		/* we-ve changed something */
	} /* end if, testing for NULL temp */
    } /* end if, testing for multiply */

#endif	/* ndef M32 */

#ifndef M32			/* 3B20 only */

/* The 3B20 has special instructions to move a positive nibble to a
** register.  It does less well with negative ones, since it puts
** them in a large immediate value.  The following transformation
** runs faster for small negative constants.
**
**	movw	&-n,R		->	movw	&n,R
**				->	mnegw	R,R
**
**	when 1 <= n <= 15
*/

    if (
	    cop == MOVW
	&&  isreg(pf->op2)
	&&  isnegnib(pf->op1)
	)
    {
	NODE * new = insert(pf);	/* make new node after pf */

	wchange();			/* we're making changes */
	*(pf->op1 = copyopn(pf->op1,-1,1)) = '&';
					/* copy operand, offset by 1,
					** overwrite - with &
					*/
	chgop(new,MNEGW,"mnegw");	/* fill in new node */
	new->op1 = new->op2 = pf->op2;
	new->nlive = pf->nlive;		/* replicate live/dead data for new
					** node so it won't go away
					*/
	return(true);
    }

#endif	/* ndef M32 */

#ifndef M32				/* 3B20 only */

/* From empirical studies it appears that triadic instructions should
** not be used on the 3B20S when the destination is a register, because
** an equivalent two-instruction sequence is faster.  The same applies
** in another specialized triadic case below.
**
**	op3 O1,O2,R		->	movw O2,R
**				->	op2 O1,R
**
**	if R not used in O1
**
**
**	op3 &n,R,O		->	op2 &n,R
**				->	movw R,O
**
**	if R is dead
*/

    if (istriadic(pf,&opn,&opst))	/* check triadic, save equiv. op code
					** number and string
					*/
    {

    /* first case:  op3 O1,O2,R */

	if ( isreg(pf->op3) && ! usesreg(pf->op1,pf->op3) )
					/* check R not used in O1 */
	{
	    NODE * new = insert(pf);	/* add new node after pf */

	    wchange();			/* changing something */

	    chgop(new,opn,opst);	/* set up new node with dyadic */
	    new->op1 = pf->op1;		/* set O1 */
	    new->op2 = pf->op3;		/* set R */
	    new->nlive = pf->nlive;	/* propagate live/dead stuff */

	    chgop(pf,MOVW,"movw");	/* modify original node */
	    pf->op1 = pf->op2;		/* make first operand O2 */
	    pf->op2 = pf->op3;		/* second is R */
	    pf->op3 = NULL;		/* clean up third one */
	    makelive(pf->op2,pf);	/* force R live; R may be dead
					** after pl if pl is followed by a
					** conditional branch.  We must make
					** it live here.
					*/
	    return(true);
	}
	else if (isnib(pf->op1) && isdead(pf->op2,pf))
	/* (test for register implicit in "isdead") */

	/* second case:  op3 &n,R,O */

	{
	    NODE * new = insert(pf);	/* add new following node */

	    wchange();			/* changing something */

	    chgop(pf,opn,opst);		/* put dyadic in first node */

	    chgop(new,MOVW,"movw");	/* second node is MOVW */
	    new->op1 = pf->op2;		/* set R */
	    new->op2 = pf->op3;		/* set O */
	    new->nlive = pf->nlive;	/* propagate live/dead in movw */
	    makelive(pf->op2,pf);	/* make R live after op2 */

	    pf->op3 = NULL;		/* clean out 3rd operand of original */
	    return(true);
	}
    }

#endif	/* ndef M32 */

#ifdef IMPLLSW

/* For BELLMAC-32, a shift by one bit is more efficiently
** done as an add.
**
**	llsw2 &1,O1		->	addw2 O1,O1
**	
**	llsw3 &1,O1,O2		->	addw3 O1,O1,O2
**
*/

{
	if( strcmp( pf->op1, "&1" ) == 0 
	    && isiros(pf->op2)		/* safe from mmio */
	    ) {
		if( pf->op == LLSW2 ) {
			chgop( pf, ADDW2, "addw2" );
			pf->op1 = pf->op2;
			return( true );
		}
		if( pf->op == LLSW3 ) {
			chgop( pf, ADDW3, "addw3" );
			pf->op1 = pf->op2;
			return( true );
		}
	}
}
#endif /* IMPLLSW */

    return(retval);			/* indicate whether anything changed */
}
