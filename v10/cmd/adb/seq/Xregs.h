/* $Copyright:	$
 * Copyright (c) 1984 Sequent Computer Systems, Inc.
 * All rights reserved
 * 
 * This software is furnished under a license and may be used
 * only in accordance with the terms of that license and with the
 * inclusion of the above copyright notice.   This software may not
 * be provided or otherwise made available to, or used by, any
 * other person.  No title to or ownership of the software is
 * hereby transferred.
 */

/*
 * $Header: reg.h 1.3 84/12/11 $
 *
 * reg.h
 *	Defines location of the users' stored registers relative to R0.
 *
 * Usage is u.u_ar0[XX].
 */

/* $Log:	reg.h,v $
 */

/*
 * Assumes use of cxp call into syscall/trap.
 * Once in syscall/trap handler, stack looks like:
 *
 *	modpsr			user program
 *	pc			return context
 *	fp			user FP
 *	old spl value		to restore on exit (nested interrupts)
 *	r0			assumes "enter [r0,r1,r2],ENTLOCL*4"
 *	r1
 *	r2
 *	user SP
 *	trap-type		identify trap
 *	modpsr			of locore
 *	pc			into locore
 *	fp			of locore
 *	<locals>		# stack variables
 *	r3
 *	r4
 *	r5
 *	r6
 *	r7
 */

#define	TRAPOFF	8		/* delta between sp&r3, includes trap locals */
#define	ENTLOCL	1		/* # local words on trap entry */

#define	MODPSR	(R0+(3+ENTLOCL)*4)
#define	PS	MODPSR
#define	PC_OFF	(R0+(2+ENTLOCL)*4)
#define	FP	(R0+(1+ENTLOCL)*4)

#define	R0	(ctob(UPAGES)-0x14)
#define	R1	(R0-1*4)
#define	R2	(R0-2*4)
#define	SP	(R0-3*4)
#define	R3	(R0-(4+TRAPOFF)*4)
#define	R4	(R0-(5+TRAPOFF)*4)
#define	R5	(R0-(6+TRAPOFF)*4)
#define	R6	(R0-(7+TRAPOFF)*4)
#define	R7	(R0-(8+TRAPOFF)*4)

#define	PSRADJ	(16)		/* # bits offset of PSR in MODPSR */

#define	BADREG	(-1)		/* guaranteed not a valid reg offset */

#define	MAXREG	7		/* biggest register number in stabs */
#define	MINREG	0		/* smallest reg number */

#define	PC	(-2)		/* some number */
