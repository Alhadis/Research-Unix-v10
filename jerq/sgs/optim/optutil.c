/* @(#) optutil.c: 1.3 3/27/84				*/
/* optutil.c
**
**	Optimizer utilities for 3B code improver (PCI).
**
**
** This module contains utility routines for the various peephole
** window optimizations.
*/

/* #include <ctype.h> -- optim.h takes care of this */
/* #include "defs" -- optim.h takes care of this; machine-dependent defs */
#include "optim.h"			/* machine independent optimizer defs */

extern long atol();
/* isreg -- is operand string a register reference?
**
** This routine tells whether an operand string is a register reference.
*/

boolean
isreg(s)
char * s;				/* operand string */
{
/* assume operand is a register reference if it begins with a % */

	return(*s == '%');
}



/* isnib -- is operand a nibble?
**
** This routine tests an operand string and tells whether it is an
** immediate operand that can fit in a nibble (4 bits).
*/

boolean
isnib(s)
char * s;				/* operand string to check */
{
    long x;				/* temporary to hold value */

/* Return true if the operand begins with a '&', it is numeric,
** and the numeric value is between 0 and 15.
*/

    return(	*s == '&'		/* begins with & */
	   &&	isdigit(*++s)		/* operand is numeric */
	   &&	(x = atol(s)) >= 0	/* is positive */
	   &&	x <= 15			/* and <= 15 */
	  );
}
/* isnegnib -- is operand a negative nibble?
**
** This routine tests whether an operand string is an immediate operand
** with a value between -1 and -15.  Another condition tested is that
** the minus sign immediately follows an '&'.  (This condition is
** important if the operand is overwritten.)
*/

boolean
isnegnib(s)
char * s;				/* operand string */
{
    long n;				/* temporary operand value */

    return(
		s != NULL
	    &&  *s++ == '&'		/* operand starts with & */
	    &&  *s++ == '-'		/* followed by - */
	    &&  isdigit(*s)		/* operand is numeric */
	    &&  1 <= (n = atol(s))	/* (positive) value between 1... */
	    &&	n <= 15			/* and 15 */
	    );
}
/* getbit -- get bit number for power of 2
**
** This routine tests whether an operand is a power of 2.  If not, it
** returns -1.  Otherwise it returns a bit number, which is also the
** exponent of 2.
*/

int
getbit(s)
char * s;				/* pointer to operand string */
{
    int bit = 0;			/* initialize bit number for later */
    long n;				/* numeric value of operand */
    boolean isnumlit();

    if ( (! isnumlit(s)) || (n = atol(s+1)) < 0 )
	return(-1);			/* check for numeric literal with
					** good value
					*/

/* This test really works, although it looks suspicious.  A power of two
** has only one bit set, so that value minus one has all of the bits to
** the right of the original bit set.  Anding them together gives zero.
** No other value yields zero.
*/

    if ( ( n & (n-1) ) != 0)
	return(-1);			/* not a power of 2 */

/* Shift right until the word is zero */

    while ((n = n>>1) != 0)
	bit++;				/* bump bit number each time */

    return(bit);			/* return bit number */
}
/* insert -- insert new instruction node
**
** This routine inserts a new instruction node after the one pointed
** to.  The fields in the node are initialized to null values.
*/

NODE *					/* return pointer to the new node */
insert(pn)
NODE * pn;				/* node to add pointer after */
{
    NODE * Saveop();			/* in machine-independent part */
    NODE * new = Saveop(0,"",0,GHOST);	/* create isolated node, init. */

    APPNODE(new,pn);			/* append new node after current */

    return(new);			/* return pointer to the new one */
}
/* chgop -- change op code number and op code string in node
**
** This routine changes the op code information in a node.  The
** operand information is unaffected.
*/

void
chgop(n,op,op_code)
NODE * n;				/* pointer to node to change */
int op;					/* op code number */
char * op_code;				/* pointer to op code string */
{
    n->op = op;				/* set op code number */
    n->opcode = op_code;		/* and string */
    return;
}
/* copyopn -- copy operand string
**
** This routine duplicates an operand string in a new piece of memory.
** The caller can specify a +/- number of additional bytes required,
** as well as an offset for copying.
*/

char *					/* return duplicate string */
copyopn(s,extra,offset)
char * s;				/* original operand string */
int extra;				/* number of extra (or fewer) chars */
int offset;				/* offset in original operand from
					** which to start copying.
					*/
{
    int len = strlen(s) + extra;	/* length of duplicate */
    char * stemp;			/* future duplicate string */

    if (len <= 0)
	return(NULL);			/* deleted everything */
    
    stemp = getspace(len+1);		/* get new string (+1 for null byte) */
    return(strcpy(stemp,s+offset));	/* return copy */
}
/* isdyadic -- is instruction dyadic
**
** This routine tells whether an instruction is dyadic according
** to the following conditions:
**
**	1.	It takes two operands.
**	2.	It reads the first operand, writes the second.
**	3.	It sets the condition codes according to the value stored in
**		the second operand.
**	4.	The operands have normal address mode meanings:  disallow,
**		for example, MOVAW, which does address arithmetic of op1.
*/

boolean
isdyadic(n)
NODE * n;				/* instruction node to test */
{
    switch(n->op)			/* dispatch on op code number */
    {
    case MCOMW:
    case MOVZBH:
    case MOVZBW:
    case MOVZHW:
    case ANDW2:
    case ORW2:
    case XORW2:
    case LLSW2:
    case LRSW2:
    case MOVW:
    case MOVBBH:
    case MOVBBW:
    case MOVBHW:
    case MOVTHB:
    case MOVTWB:
    case MOVTWH:
    case MNEGW:
    case ADDW2:
    case SUBW2:
    case MULW2:
    case UMULW2:
    case DIVW2:
    case UDIVW2:
    case UMODW2:
    case ALSW2:
    case ARSW2:
	return(true);			/* all of these are dyadic */
    default:
	return(false);			/* others are not */
    }
}
/* istriadic -- is instruction triadic?
**
** This routine serves a similar function to isdyadic, except that
** it also returns the related dyadic op code number and string.
** The only instructions included here are the obvious ones.  They
** are assumed to have integer operands (by callers).
*/

/* define macro to use below */

#define CHANGE(op,new,newst)	case op: *pnewop=new;*pnewopcode=newst;break;


boolean
istriadic(n,pnewop,pnewopcode)
NODE * n;				/* node to test */
int * pnewop;				/* place to put equiv. dyadic # */
char ** pnewopcode;			/* place to put equiv. dyadic string */
{
    switch (n->op)			/* dispatch on op code number */
    {
    CHANGE(ADDW3,ADDW2,"addw2");
    CHANGE(SUBW3,SUBW2,"subw2");
    CHANGE(MULW3,MULW2,"mulw2");
    CHANGE(UMULW3,UMULW2,"umulw2");
    CHANGE(DIVW3,DIVW2,"divw2");
    CHANGE(UDIVW3,UDIVW2,"udivw2");
    CHANGE(MODW3,MODW2,"modw2");
    CHANGE(UMODW3,UMODW2,"umodw2");
    CHANGE(ALSW3,ALSW2,"alsw2");
    CHANGE(ARSW3,ARSW2,"arsw2");
    CHANGE(ANDW3,ANDW2,"andw2");
    CHANGE(ORW3,ORW2,"orw2");
    CHANGE(XORW3,XORW2,"xorw2");
    CHANGE(LLSW3,LLSW2,"llsw2");
    CHANGE(LRSW3,LRSW2,"lrsw2");
    default:				/* for op codes not found */
	return(false);
    }
    return(true);			/* found op code */
}
/* totriadic -- can dyadic be made triadic?
**
** This routine checks a dyadic instruction to see if it can be
** made triadic.  If so, it returns the opcode number and string
** for the corresponding triadic.  Totriadic is really the inverse
** of istriadic.
*/

boolean
totriadic(n,pnewop,pnewopcode)
NODE * n;				/* node to test */
int * pnewop;				/* place to put equiv. triadic # */
char ** pnewopcode;			/* place to put equiv. triadic string */
{
    switch (n->op)			/* dispatch on op code number */
    {
    CHANGE(ADDW2,ADDW3,"addw3");
    CHANGE(SUBW2,SUBW3,"subw3");
    CHANGE(MULW2,MULW3,"mulw3");
    CHANGE(UMULW2,UMULW3,"umulw3");
    CHANGE(DIVW2,DIVW3,"divw3");
    CHANGE(UDIVW2,UDIVW3,"udivw3");
    CHANGE(MODW2,MODW3,"modw3");
    CHANGE(UMODW2,UMODW3,"umodw3");
    CHANGE(ALSW2,ALSW3,"alsw3");
    CHANGE(ARSW2,ARSW3,"arsw3");
    CHANGE(ANDW2,ANDW3,"andw3");
    CHANGE(ORW2,ORW3,"orw3");
    CHANGE(XORW2,XORW3,"xorw3");
    CHANGE(LLSW2,LLSW3,"llsw3");
    CHANGE(LRSW2,LRSW3,"lrsw3");
    default:				/* for op codes not found */
	return(false);
    }
    return(true);			/* found op code */
}
/* makelive -- make register live
**
** This routine marks a register operand as live.
*/

void
makelive(s,n)
char * s;				/* operand string (to %r0) */
NODE * n;				/* pointer to node to enliven */
{
    n->nlive |= setreg(s);
    return;
}



/* makedead -- make register dead
**
** This routine marks a register operand as dead.
*/

void
makedead(s,n)
char * s;				/* operand string (to %rn) */
NODE * n;				/* pointer to node to kill */
{
    n->nlive &= ~ setreg(s);
    return;
}
/* isindex -- is operand an index off a register?
**
** This routine checks whether an operand string is of the
** form:
**
**	n(%rn)
**
** and whether it uses a designated register.
**
** Actually, it cheats:  it just checks for an initial digit or sign followed
** by a (.  In fact the ( test is a cheat, because we just check to see
** if there's a ( in the string.
*/

boolean
isindex(s,r)
char * s;				/* operand string to test */
char * r;				/* register operand string to check
					** for
					*/

{
    char * strchr();

    if(*s == '*')
		s++;
    return(
		usesreg(s,r)
	    &&	(isdigit(*s) || *s == '-') /* could be leading sign, too */
	    &&  strchr(s,'(') != (char *) 0
	    );
}
/* ismove -- is instruction a move?
**
** This routine determines whether an instruction is a move-type.
** There is a multiplicity of move instructions which copy, zero-
** or sign-extend, and truncate.  This routine returns the natural
** size of the source and destination operands.
*/

boolean
ismove(n,srcsize,dstsize)
NODE * n;				/* pointer to instruction node */
int * srcsize;				/* place to put size of source
					** operand (in bytes)
					*/
int * dstsize;				/* place to put size of destination */
{
    register int srctemp = 1;		/* temporary source size */
    register int dsttemp = 4;		/* temporary destination size */
/* Initial values chosen on basis of most common value of each. */

    switch (n->op)			/* dispatch on op-code number */
    {
    case MOVZBH:
			dsttemp = 2;	break;
    case MOVZBW:
					break;
    case MOVZHW:
	srctemp = 2;			break;

    case MOVB:
			dsttemp = 1;	break;
    case MOVH:
	srctemp = 2;	dsttemp = 2;	break;
    case MOVW:
	srctemp = 4;			break;

    case MOVBBH:
			dsttemp = 2;	break;
    case MOVBBW:
					break;
    case MOVBHW:
	srctemp = 2;			break;

    case MOVTHB:
	srctemp = 2;	dsttemp = 1;	break;
    case MOVTWB:
	srctemp = 4;	dsttemp = 1;	break;
    case MOVTWH:
	srctemp = 4;	dsttemp = 2;	break;
    default:
	return(false);			/* not a move instruction */
    }
    *srcsize = srctemp;			/* copy out correct values */
    *dstsize = dsttemp;
    return(true);			/* found a move */
}
/* iszoffset -- is operand zero offset from register
**
** This routine determines whether a given operand is an indexed
** operation from a designated register, where the index is zero.
*/

boolean
iszoffset(operand,reg)
register char * operand;		/* operand string */
char * reg;				/* register string */
{

/* We use a quick and dirty strategy here:  we test the operand for
** zero, followed by (, with matching register numbers in the right
** place.  We assume register designations are "%rn".
*/

    return(
		*operand == '0'
	    &&  *(operand+1) == '('
	    &&  *(operand+4) == *(reg+2)
	  );
}
/* exchange -- exchange node and successor
**
** This routine reverses the linkages of two nodes so the first is
** the second and the second is the first.
*/

void
exchange(first)
NODE * first;				/* pointer to first of two nodes */
{

/* This can probably be done with fewer temporaries, but it's a lot
** easier to understand what's going on this way....
*/

    NODE * prev = first->back;		/* predecessor of first node */
    NODE * second = first->forw;	/* second node of pair to exchange */
    NODE * next = second->forw;		/* successor of second node */

    prev->forw = second;		/* second will now be first */
    second->back = prev;

    first->forw = next;			/* successor of old first is now
					** 'next'
					*/
    next->back=first;			/* 'next's' predecessor now 'first' */

    second->forw = first;
    first->back = second;		/* re-link connection of node pair */

    return;
}
/* doindirect -- resolve indirect references
**
** This routine changes variable references of the form
**	0(%rn)
** to
**	*var
** where possible.  Its purpose is to make it possible to discard a
** register load.
**
** Here's the scenario:  we're handed an instruction, the name of the
** register whose zero-index use we want to change, and the real location
** of the variable.  For example, in the sequence
**
**	movw xyz,%r0
**	addw2 0(%r0),%r1
**
** the instruction we would be handed is the addw2, the register would be
** %r0, and the real location of the variable is 'xyz'.
**
** Complications:
**	1.  The register must be dead after the instruction.
**	2.  We want to change all such references in the instruction.
**	3.  If there are any other uses of the register than a zero
**		index, like actually using the register contents or
**		using an index other than zero (e.g., 4(%r0)), we
**		can't make the transformation, since we're going to
**		eliminate the preceding register load.
**	4.  ... except that if the destination of a triadic instruction or
**		move is the register it's safe to proceed.
**
** One, possibly unsafe assumption:
**	The algorithm assumes that unused operands in an instruction (like
**	operands 3 and 4 in a dyadic) are always NULL.  We also assume that
**	an instruction has a "destination" (returned by 'dst') if and only
**	if it can alter something.  We further assume that the destination
**	is always the last operand.
*/

boolean					/* true if the transformation
					** was made
					*/
doindirect(n,reg,repl)
NODE * n;				/* instruction node to examine */
char * reg;				/* register (string) to look for */
char * repl;				/* operand string to use */
{
    boolean first = true;		/* if examining first real operand */
    int indmask = 0;			/* will have bits set telling which
					** operands to change
					*/
    int i;				/* loop index */
    char * operand;			/* current operand */
    int src,dest;			/* for ismove (but unused) */
/* Begin:  check whether transformation can be made, note which operands
** to change if so.
*/

    if (
	    isret(n)			/* will show register dead, which
					** will confuse us
					*/
	||  ! (
		  isdead(reg,n)		/* reg must be dead after n */
	       || ( ismove(n,&src,&dest) && samereg(n->op2,reg) )
	       )			/* unless set by this inst. as move */
	||  *repl == '*'		/* can't do it if replacement is
					** already an indirect...
					*/
	||  *repl == '&'		/* or an immediate */
	)
	return(false);			/* can't do it */

/* Now scan operands for things that can't be transformed.  We do this
** backward from higher numbered operands on the assumption that a
** destination, which we treat differently, is the highest numbered
** non-null operand.
*/

    for ( i = MAXOPS; i > 0; i-- )
    {
	int opn;			/* op code number in istriadic */
	char * opst;			/* op code string in istriadic */

	if ( (operand = n->ops[i]) != NULL )
	{				/* found an operand */
	    if (iszoffset(operand,reg))	/* this is what we want! */
		indmask |= 1<<i;	/* remember where it is */
	    
	    /* otherwise can only use register if this is the last
	    ** operand (but the first we'll find) and the instruction
	    ** is triadic
	    */

	    else if (
			 ! (    samereg(operand,reg)
			    &&  first
			    &&  (
				    istriadic(n,&opn,&opst)
				 || ismove(n,&src,&dest)
				)
			    )
		     &&  usesreg(operand,reg)
		     )
		return(false);		/* bad use of register */
	    
	    first = false;		/* no longer looking at first (last)
					** operand
					*/
	}
    }
    /* Reaching here, we have successfully scanned all operands without
    ** finding any register references that violate the required conditions.
    */

    if (indmask == 0)			/* if no register references... */
	return(false);			/* say we didn't change anything */
    if ( !isiros(repl) ) return( false ); /* check for mmio */
/* Now we're ready to do the actual transformations.  If the replacement
** operand is a register reference, we create a new operand reference
** of the form:
**	0(reg) --> 0(repl)
**
** Otherwise we have a vanilla operand.  Rewrite it as
**
**	0(reg) --> *repl
*/

    {					/* for some more variables */
	char * indstring;		/* new operand string */
	int extra;			/* number of extra chars in xform */
	char * format;			/* sprintf format string for xform */

	if (isreg(repl))		/* replacement is also reference */
	{
	    format = "0(%s)";		/* to form 0(repl) */
	    extra = 3;			/* for 0 ( ) */
	}
	else
	{
	    format = "*%s";		/* rewrite as *repl */
	    extra = 1;			/* prepend * only */
	}

	indstring = getspace(strlen(repl) + extra + 1);
					/* allocate space + 1 for null */
	(void) sprintf(indstring,format,repl);
					/* build replacement */
	
	wchange();			/* Finally!  About to make changes */

	/* loop through operands, replacing ones whose bit is set in mask */

	for ( i = MAXOPS; i > 0; i-- )
	{
	    if ( (indmask & (1<<i)) != 0)
		n->ops[i] = indstring;	/* stick in replacement string */
	}

	return(true);			/* say we changed stuff */
    } /* end extra block */
}
/* isnumlit -- is operand a numeric literal
**
** This routine tests operand strings for the form
**
**	&[-]n
**
** where n is a positive integer.
*/

boolean
isnumlit(s)
char * s;				/* pointer to operand string */
{
    if (*s++ != '&')
	return(false);
    
    if (*s == '-')			/* sign okay */
	s++;
    
    while (*s != '\0')			/* check for all digits */
	if ( ! isdigit(*s++) )
	    return(false);
    
    return(true);			/* passed all tests */
}
/* usesvar -- does first operand use second?
**
** This routine is a generalization of "usesreg":  it returns true
** if the first operand uses the second.  An example is
** v1 = "*p", v2 = "p".  v1 uses the contents of v2.
*/

boolean
usesvar(v1,v2)
char * v1;				/* using operand */
char * v2;				/* used operand */
{
    if (isreg(v2) && usesreg(v1,v2))
	return(true);			/* handle register case specially */

    if (*v1 == '*')			/* check for initial indirect */
	v1++;

    return(strcmp(v1,v2) == 0);		/* result is result of compare */
}


/* These routines try to preserve the line number information.
   The criterion for saving or discarding line number information is
   that the state of the machine from a C point of view should be the
   same at any given line both before and after the optimization.
   If it would not be then the line number is deleted.
   The exceptions to the above are that the state of the condition codes 
   and of dead registers is ignored. 
   
	May 1983 */

/* NOTE: All instruction nodes that these routines assume will be tossed
	  have their uniqids set to IDVAL.  For example, in the merges,
	  the uniqids of the original instructions are set to IDVAL
	  and then the uniqid of the resultant instruction is computed.
	  This has the effect of setting the uniqid of the instruction 
	  not corresponding to the resultant to IDVAL. This fact is used 
	  in some instances where lnmrginst2 is called -- see w2opt.c */

/* delete instruction -- save line number
	if the instruction below has no line number or has a line number
	greater than the present instruction, the present number 
	is transfered down */

void
ldelin(nodep)
NODE * nodep;
{
	if(nodep->uniqid == IDVAL)
		return;

	if(nodep->forw != &ntail && 
	   (nodep->forw->uniqid > nodep->uniqid 
		 || nodep->forw->uniqid == IDVAL))
		nodep->forw->uniqid = nodep->uniqid;

	nodep->uniqid = IDVAL;

}

/* more conservative version of lndelinst that does not overwrite
	line numbers below */
void
ldelin2(nodep)
NODE *nodep;
{
	if(nodep == IDVAL)
		return;

	if(nodep->forw != &ntail && nodep->forw->uniqid == IDVAL)
		nodep->forw->uniqid = nodep->uniqid;

	nodep->uniqid = IDVAL;

}


/* exchange instructions
	The line number of the first instruction, if any, is given to the
	second instruction.  The line number of the second instruction is 
	deleted. */
void
lexchin(nodep1,nodep2)
NODE *nodep1, *nodep2;
{
	if(nodep1->uniqid != IDVAL) {
		nodep2->uniqid = nodep1->uniqid;
		nodep1->uniqid = IDVAL;
	}
	else {
		nodep2->uniqid = IDVAL;
	}
}


/* merge instructions - case 1
	The result instruction is given the line number of the
	top instruction, if it exists, or else it is given the line
	number of the bottom instruction. */
void
lmrgin1(nodep1,nodep2,resnode)
NODE *nodep1, *nodep2, *resnode;
{
	IDTYPE val1, val2;

	val1 = nodep1->uniqid;
	val2 = nodep2->uniqid;

	nodep1->uniqid = nodep2->uniqid = IDVAL;

	if(val1 != IDVAL)
		resnode->uniqid = val1;
	else 
		resnode->uniqid = val2;

}


/* instruction merge - case 2
	The resultant instruction is given the line number of the
	top instruction, if it exists, else it is not given a line number */
void
lmrgin2(nodep1,nodep2,resnode)
NODE *nodep1, *nodep2, *resnode;
{
	IDTYPE val1;

	val1 = nodep1->uniqid;

	nodep1->uniqid = nodep2->uniqid = IDVAL;

	resnode->uniqid = val1;

}

/* instruction merge - case 3
	The resultant instruction is given the line number of the first
	instruction, if it exists. The instruction below the pair to be 
	merged is given the line number of the second instruction, 
	if it exists */
void
lmrgin3(nodep1,nodep2,resnode)
NODE *nodep1, *nodep2, *resnode;
{
	IDTYPE val1;

	ldelin2(nodep2);

	val1 = nodep1->uniqid;

	nodep1->uniqid = nodep2->uniqid = IDVAL;

	resnode->uniqid = val1;
}

/* isiros  --  is operand either immediate, register or on stack?
**
** This routine returns true if the the operand 
** pointed to by its argument has address mode
**
**		immediate,
**		register, or
**		offset from a stack pointer (fp, ap, or sp).
**
** This test is provided for checking operands to see whether they
** can be memory mapped i/o references.
*/

boolean
isiros( op )
char *op;
{
	return( *op == '&' ||
		*op == '%' ||
		( *op != '*' && 
			( usesreg( op, "%fp" ) ||
			usesreg( op, "%ap" ) ||
			usesreg( op, "%sp" ) 
			) 
		)
	      );
}
