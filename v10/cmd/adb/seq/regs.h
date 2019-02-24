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

#define	MODPSR	(3+ENTLOCL)
#define	PC	(2+ENTLOCL)
#define	FP	(1+ENTLOCL)

#define	R0	(0)
#define	R1	(-1)
#define	R2	(-2)
#define	SP	(-3)
#define	R3	(-4-TRAPOFF)
#define	R4	(-5-TRAPOFF)
#define	R5	(-6-TRAPOFF)
#define	R6	(-7-TRAPOFF)
#define	R7	(-8-TRAPOFF)

#define	PSRADJ	(16)		/* # bits offset of PSR in MODPSR */

#define	BADREG	(-100)		/* guaranteed not a valid reg offset */

#define	MAXREG	7		/* biggest register number in stabs */
#define	MINREG	0		/* smallest reg number */
