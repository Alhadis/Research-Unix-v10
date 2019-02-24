/* Copyright 1989 by AT&T Bell Laboratories */
/* SPARC.prim.s
 *
 * J.H. Reppy
 * Cornell University
 * Ithaca, NY  14853
 * jhr@cs.cornell.edu
 *
 * HISTORY:
 *   03/15/89  created
 *   11/20/89  revised for to use heap limit check.
 *
 * SPARC runtime code for ML.  Registers are used as follows:
 *
 * %g7 : exception handler continuation
 * %g6 : freespace pointer
 * %g5 : store pointer
 * %g4 : heap limit pointer
 *
 * %i2 = closure; can be ignored because contains no free vars
 * %i0 = arg
 * %i1 = continuation
 * %i3 = base code pointer
 *
 * %o0,%o1 = arith. temps
 * %g1-%g3,%l0-%l7,%i4-%i5 = misc. registers (contain only pointers or tagged ints)
 * %o5 = pointer temp.
 *
 * %o2,%o3 = used for args to ml_mul & ml_div, also used as temp registers
 * %o4,%o5 = temp registers
 *
 * %o6 = %sp (not used by ML)
 * %i6 = %fp (not used by ML)
 * %i7 = return address to C code (not used by ML)
 * %o7 = not used
 *
 * There are four places in this file where garbage collection can be triggered:
 * in array_v, create_s_v, scalb_v and in raise_real.  It it important that only
 * registers saved by _saveregs be live at these points. Also, only pointer
 * registers can contain heap pointers at these points.
 */

#include <machine/asm_linkage.h>
#include <machine/trap.h>
#include "tags.h"
#include "prof.h"
#include "prim.h"

/* Macros to fetch and store values in memory; use %o5 as an addressing register. */
#define FETCH(addr, reg)			\
	    sethi   %hi(addr),%o3;		\
	    ld	    [%o3+%lo(addr)],reg
#define STORE(reg, addr)			\
	    sethi   %hi(addr),%o3;		\
	    st	    reg,[%o3+%lo(addr)]


/* Macros to save and restore the global registers.  We save %g1-%g7 in the
 * corresponding locals of the new window.  The C-routine/system call is
 * guaranteed not to touch the local or in registers (which are the out registers
 * of the ML window).
 */
#define SAVE_ML_REGS				\
	    mov	    %g1,%l1;			\
	    mov	    %g2,%l2;			\
	    mov	    %g3,%l3;			\
	    mov	    %g4,%l4;			\
	    mov	    %g5,%l5;			\
	    mov	    %g6,%l6;			\
	    mov	    %g7,%l7

#define RESTORE_ML_REGS				\
	    mov	    %l1,%g1;			\
	    mov	    %l2,%g2;			\
	    mov	    %l3,%g3;			\
	    mov	    %l4,%g4;			\
	    mov	    %l5,%g5;			\
	    mov	    %l6,%g6;			\
	    mov	    %l7,%g7

/* The ML stack frame has the following layout (set up by restoreregs):
 *
 *			+-----------------+
 *	%fp = %sp+80:	| temp for floor  |
 *			+-----------------+
 *	%sp+76:		| addr of _ml_div |
 *			+-----------------+
 *	%sp+72:		| addr of _ml_mul |
 *			+-----------------+
 *	%sp+68:		|    saved %g6    |
 *			+-----------------+
 *	%sp+64:		|    saved %g7    |
 *			+-----------------+
 *			|  space to save  |
 *			|  in and local   |
 *	%sp:		|    registers    |
 *			+-----------------+
 *
 * the size of the frame is
 */
#define ML_FRAMESIZE (WINDOWSIZE+20)


#define String(handle,len,str)			\
	    .align  4;				\
	    .word   len*power_tags+tag_string;	\
    handle: .ascii  str;			\
	    .align  4

#define Closure(name)				\
	    .align  4;				\
	    .word   mak_desc(1,tag_record);	\
    name:   .word   7f;				\
	    .word   1;				\
	    .word   tag_backptr;		\
    7:

#define RAISE					\
	    ld	    [%g7],%i2;			\
	    jmp	    %i2;			\
	    mov	    %g7,%i1	/* (delay slot) */

#define CONTINUE				\
	    ld	    [%i1],%i2;			\
	    jmp	    %i2;			\
	    nop


	.seg	"text"
	.global _runvec
	.align	4
	.word	mak_desc(8, tag_record)
_runvec:
	.word	_array_v
	.word	_callc_v
	.word	_create_b_v
	.word	_create_s_v
	.word	_floor_v
	.word	_logb_v
	.word	_scalb_v
	.word	_syscall_v


	.global	_startprim
_startprim:


/* array : (int * 'a) -> 'a array
 * Allocate and initialize a new array.	 This can cause GC.
 */
Closure(_array_v)
1:
	ld	[%i0],%o0		/* get length into %o0 */
	ld	[%i0+4],%l0		/* get default into %l0 */
	sra	%o0,1,%o0		/* convert to sparc int */
	sll	%o0,width_tags,%o1	/* build the tag in %o1 */
	or	%o1,tag_array,%o1
	sll	%o0,2,%o0		/* scale length to bytes */
	add	%g4,%o0,%o4		/* check the heap limit */
	addcc	%g6,%o4,%g0
	bvs	3f			    /* we hit the limit, so trap safely */
	dec	4,%o0			/* length-- */
	st	%o1,[%g6-4]		/* store the tag */
	mov	%g6,%i0			/* result := object addr. */
2:					/* initialization loop */
	st	%l0,[%g6]		    /* store default. */
	deccc	4,%o0			    /* length-- */
	bge	2b
	inc	4,%g6			    /* freeptr++ (delay slot) */
	/* end loop */
	inc	4,%g6			/* freeptr++ */
	CONTINUE
3:					/* we come here to do a safe GC trap. */
	add	%g0,0,%g0		    /* a nop to get PC adjust right */
	taddcctv %g6,%o4,%g0		    /* cause the GC trap. */
	ba	1b			    /* retry the allocation. */
	nop


/* create_b : int -> string
 * create_s : int -> string
 * Create bytearray or string of given length.	This can cause GC.
 */
Closure(_create_b_v)
	ba	2f
	mov	tag_bytearray,%l1   /* (delay slot) */

Closure(_create_s_v)
	mov	tag_string,%l1

2:	sra	%i0,1,%o0	    /* %o0 = length (sparc int) */
	sll	%o0,width_tags,%o2
	or	%o2,%l1,%o1	    /* build the tag in %o1 */
	add	%o0,3,%o0	    /* %o0 = length in words (no tag) */
	sra	%o0,2,%o0
	sll	%o0,2,%o0	    /* %o0 = length in bytes (no tag) */
	add	%g4,%o0,%o4	    /* Check the heap limit. */
	addcc	%g6,%o4,%g0
	bvs	3f			/* we hit the limit, so trap safely. */
	st	%o1,[%g6-4]	    /* store the tag */
	mov	%g6,%i0		    /* result := object addr */
	add	%o0,4,%o0	    /* %o0 = length in bytes (including tag) */
	add	%o0,%g6,%g6	    /* freeptr += length */
	CONTINUE
3:				    /* we come here to do a safe GC trap. */
	add	%g0,0,%g0		/* a nop to get PC adjust right */
	taddcctv %g6,%o4,%g0		/* cause the GC trap. */
	ba	2b			/* retry the allocation. */
	nop


/* floor : real -> int
 * Return the floor of the argument or else raise Float("floor") if out of range.
 * We implement the range check by using an integer comparison with the high 32
 * bits of the real value (which contains the biased exponent).
 * (double)(2^30)   == [0x41d00000, 0x0]
 * (double)(-2^30)  == [0xc1d00000, 0x0]
 */
Closure(_floor_v)
	ld	[%i0],%f0	    /* fetch arg into %f0, %f1. */
	ld	[%i0+4],%f1
	ld	[%i0],%o0	    /* %o0 gets high word. */
	tst	%o0		    /* negative ? */
	blt	1f
	nop
				/* handle positive case */
	set	0x41d00000,%o1	    /* %o1 = 2^30 */
	cmp	%o0,%o1		    /* if %o0 >= 2^30 then range error */
	bge	out_of_range
	nop
	fdtoi	%f0,%f2		    /* cvt to int (round towards 0) */
	st	%f2,[%sp+80]
	ld	[%sp+80],%o0	    /* %o0 gets int result (via stack temp). */
	ba	2f
	nop
1:				/* handle negative case. */
	set	0xc1d00000,%o1	    /* %o1 = -2^30 */
	cmp	%o0,%o1		    /* if %o0 < -2^30 then range error */
	bge	out_of_range	    /* not bl because of sign. */
	nop
	fdtoi	%f0,%f2		    /* cvt to int (round towards 0) */
	st	%f2,[%sp+80]
	fitod	%f2,%f4		    /* cvt back to real to check for fraction */
	fcmpd	%f0,%f4		    /* same value? */
	ld	[%sp+80],%o0	    /* %o0 gets int result (via stack temp). */
	fbe	2f		    /* check result of fcmpd */
	nop
	dec	%o0		    /* push one lower */
2:				/* cvt result to ML int, and continue */
	add	%o0,%o0,%o0
	add	%o0,1,%i0
	CONTINUE

out_of_range:			/* out of range, so raise Float("floor"). */
	set	1f,%i0
	ba	_raise_real
String(1, 5, "floor\0\0\0")


/* logb : real -> int
 * Extract and unbias the exponent, return 0 for a zero exponent.
 * The IEEE bias is 1023.
 */
Closure(_logb_v)
	ld	[%i0],%o0		/* extract exponent. */
	srl	%o0,20,%o0
	andcc	%o0,0x7ff,%o0		/* if (exp == 0) */
	beq	1f
	nop
	sll	%o0,1,%o0		/* else unbias and cvt to ML int. */
	sub	%o0,2045,%i0		/* 2(n-1023)+1 == 2n-2045. */
1:	CONTINUE
2:	ba	1b
	set	1,%i0			/* return ML zero (delay slot) */


/* scalb : (real * int) -> real
 * Scale the first argument by 2 raised to the second argument.	 Raise
 * Float("underflow") or Float("overflow") as appropriate.
 */
Closure(_scalb_v)
	taddcctv %g6,%g4,%g0        /* check the heap limit. */
	ld	[%i0+4],%o0	    /* %o0 gets scale (second arg) */
	sra	%o0,1,%o0	    /* cvt scale to sparc int */
	ld	[%i0],%i0	    /* %i0 gets real (first arg) */
	ld	[%i0],%o1	    /* %o1 gets high word of real value. */
	set	0x7ff00000,%o3	    /* %o3 gets exponent mask. */
	andcc	%o1,%o3,%o4	    /* extract exponent into %o4. */
	beq	1f		    /* if 0 then return same */
	nop
	srl	%o4,20,%o4	    /* cvt exp to int (delay slot). */
	addcc	%o4,%o0,%o0	    /* %o0 = exp + scale */
	ble	under		    /* if new exp <= 0 then underflow */
	nop
	cmp	%o0,2047	    /* if new exp >= 2047 then overflow */
	bge	over
	nop
	andn	%o1,%o3,%o1	    /* mask out old exponent. */
	sll	%o0,20,%o0	    /* shift new exp to exponent position. */
	or	%o1,%o0,%o1	    /* set new exponent. */
	ld	[%i0+4],%o0	    /* %o0 gets low word of real value. */
	st	%o1,[%g6]	    /* allocate the new real value */
	st	%o0,[%g6+4]
	set	mak_desc(8,tag_string),%o0
	st	%o0,[%g6-4]
	mov	%g6,%i0		    /* set result. */
	inc	12,%g6		    /* storeptr += 3 */
1:	CONTINUE

over:				/* handle overflow */
	set	1f,%i0
	ba	_raise_real
	nop
String(1, 8, "overflow")
under:				/* handle underflow */
	set	1f,%i0
	ba	_raise_real
	nop
String(1, 9, "underflow\0\0\0")


/* raise_real:
 * Raise the exception Float with the string in %i0.
 */
_raise_real:
	taddcctv %g6,%g4,%g0		/* check the heap limit. */
	set	_real_e,%o0		/* allocate the exception. */
	st	%i0,[%g6]
	st	%o0,[%g6+4]
	set	mak_desc(2,tag_record),%o0
	st	%o0,[%g6-4]
	mov	%g6,%i0
	inc	12,%g6			/* freeptr += 3 */
	RAISE


	.global	_endprim
_endprim:


/* syscall : (int * string list * int) -> int
 * Note: it is assumed that there are no more than five arguments, plus the code.
 */
Closure(_syscall_v)
#define TMPSIZE 20		/* space to save upto five args */
#define TMPBASE WINDOWSIZE	/* offset of start of temp save area. */
	mov	%i0,%o0
	save	%sp,-SA(WINDOWSIZE+TMPSIZE),%sp
	SAVE_ML_REGS
	ld	[%i0+8],%o0	/* %o0 = argcount */
	sra	%o0,1,%o0	/* cvt to sparc int */
	add	%sp,TMPBASE,%o1 /* %o1 = base address of temp area */
	ld	[%i0+4],%o2	/* %o2 = arglist */
1:				/* loop: process arg list, saving in temp area */
	deccc	1,%o0		    /* if --argcount < 0 then done */
	bl	3f
	nop
	ld	[%o2],%o3	    /* fetch next arg */
	btst	1,%o3		    /* is the arg an unboxed value? */
	be	2f
	ld	[%o2+4],%o2	    /* advance arg list pointer (delay slot) */
	sra	%o3,1,%o3	    /* cvt unboxed arg to sparc int */
2:	st	%o3,[%o1]	    /* store arg in temp area */
	ba	1b
	inc	4,%o1		    /* tempptr++ (delay slot) */
	/* end of loop */
3:				/* load the args into the output registers, we */
				/* use all six registers even though there may */
				/* be fewer arguments. */
	ld	[%i0],%o0	/* %o0 = system call number */
	sra	%o0,1,%o0	/* cvt to sparc int */
	ld	[%sp+TMPBASE],%o1
	ld	[%sp+TMPBASE+4],%o2
	ld	[%sp+TMPBASE+8],%o3
	ld	[%sp+TMPBASE+12],%o4
	ld	[%sp+TMPBASE+16],%o5
	clr	%g1		/* 0 is code for syscall. */
	ta	%g0
	blu	5f
	nop
	sll	%o0,1,%o0	/* cvt result to ML int */
	inc	1,%o0
4:	RESTORE_ML_REGS
	restore %o0,0,%i0	/* restore register window, returning result */
	CONTINUE
5:				/* an error, so return -1 */
	STORE	(%o0, _errno)
	ba	4b
	set	-1,%o0		/* (delay slot) */

/* callc :  ('b * 'a) -> int
 * Call a C function with one argument.
 */
Closure(_callc_v)
	mov	%i0,%o0
	save	%sp,-SA(WINDOWSIZE),%sp
	SAVE_ML_REGS
	ld	[%i0],%o1	/* %o1 = address of the C function */
	jmpl	%o1,%o7		/* call the C function */
	ld	[%i0+4],%o0	/* %o0 = arg (delay slot) */
	RESTORE_ML_REGS
	restore %o0,0,%i0	/* restore ML register window, returning result */
	FETCH	(_cause, %o4)
	tst	%o4		/* if (cause != 0) */
	bne	_saveregs	/* then return up to the C environment. */
	nop			/* (delay slot) */
	CONTINUE


/* Space to save pointer and non-pointer registers; this is allocated here to
 * insure quad-word alignment.
 */
	.seg	"data"
	.global _saved_ptrs,_saved_nonptrs
	.align	8
_saved_ptrs:
	.skip	(32*4)
	.align	8
_saved_nonptrs:
	.skip	(32*4)
	.seg	"text"


/* handle_c, return_c, saveregs and restoreregs:
 *
 * handle_c is the root exception handler continuation.
 * return_c is the root return continuation.
 * saveregs saves the current ML registers and returns to C code.
 * restoreregs restores the ML registers and jumps to the ML code address
 * in save_pc.
 */
	.global _handle_c, _return_c, _saveregs, _restoreregs
	.global	_inML, _fault_pending, _fault_code

Closure(_handle_c)
	ba	set_cause
	set	CAUSE_EXN,%o4

Closure(_return_c)
	set	CAUSE_RET,%o4
set_cause:
	STORE	(%o4, _cause)
_saveregs:
	add	%i3,-4096,%i3		/* adjust the base code ptr (sub 4096) */
	set	_saved_ptrs,%o3
	std	%i0,[%o3]		/* save %i0, %i1 */
	std	%i2,[%o3+8]		/* save %i2, %i3 */
	std	%i4,[%o3+16]		/* save %i4, %i5 */
	std	%l0,[%o3+24]		/* save %l0, %l1 */
	std	%l2,[%o3+32]		/* save %l2, %l3 */
	std	%l4,[%o3+40]		/* save %l4, %l5 */
	std	%l6,[%o3+48]		/* save %l6, %l7 */
	std	%g2,[%o3+56]		/* save %g2, %g3 */
	st	%g1,[%o3+64]		/* save %g1 */
	set	_saved_nonptrs,%o3
	std	%o0,[%o3]		/* save %o0, %o1 as non-pointers */
	STORE	(%g0, _inML)		/* note that we are leaving ML code */
	STORE	(%g7, _saved_exnptr)
	dec	4,%g6			/* adjust store pointer */
	STORE	(%g6, _saved_dataptr)
	STORE	(%g5, _saved_storeptr)
#ifdef OLD
	set	0x7ffffffc,%o0		/* adjust limit ptr */
	sub	%o0,%g4,%g4
	STORE	(%g4, _saved_limit)
#endif
	ldd	[%sp+64],%g6		/* restore C registers %g6 & %g7. */
	ret
	restore				/* restore C register window (delay slot) */

_restoreregs:
	save	%sp,-SA(ML_FRAMESIZE),%sp
	STORE	(%sp, _bottom)		/* record the base of the ML frame */
	std	%g6,[%sp+64]		/* save C registers %g6 & %g7 */
	set	_ml_mul,%o0		/* set pointer to ml_mul */
	st	%o0,[%sp+72]
	set	_ml_div,%o0		/* set pointer to ml_div */
	st	%o0,[%sp+76]
	FETCH	(_saved_exnptr, %g7)
	FETCH	(_saved_dataptr, %g6)
	inc	4,%g6			/* adjust store pointer */
	FETCH	(_saved_storeptr, %g5)
	FETCH	(_saved_limit, %g4)
	set	0x7ffffffc,%o0		/* adjust limit ptr */
	sub	%o0,%g4,%g4
	set	1,%o0			/* note that we are entering ML code */
	STORE	(%o0, _inML)
	set	_saved_ptrs,%o3
	ldd	[%o3],%i0		/* restore %i0, %i1 */
	ldd	[%o3+8],%i2		/* restore %i2, %i3 */
	ldd	[%o3+16],%i4		/* restore %i4, %i5 */
	ldd	[%o3+24],%l0		/* restore %l0, %l1 */
	ldd	[%o3+32],%l2		/* restore %l2, %l3 */
	ldd	[%o3+40],%l4		/* restore %l4, %l5 */
	ldd	[%o3+48],%l6		/* restore %l6, %l7 */
	ldd	[%o3+56],%g2		/* restore %g2, %g3 */
	ld	[%o3+64],%g1		/* restore %g1 */
	set	_saved_nonptrs,%o3
	ldd	[%o3],%o0		/* restore %o0, %o1 as non-pointers */
	FETCH	(_fault_pending, %o4)	/* check for a pending fault */
	tst	%o4
	bne	_fault
	nop
	sub	%i3,-4096,%i3		/* adjust the base code ptr (add 4096) */
	FETCH	(_saved_pc, %o4)	/* fetch the ML code address. */
	jmp	%o4			/* invoke the ML code */
	nop

_fault:				    /* there is a pending fault */
        clr     %o4                     /* clear the pending fault flag */
        STORE   (%o4, _fault_pending)
	FETCH	(_fault_code, %i0)	/* get the fault exception packet. */
	RAISE				/* raise the exception */

/** Integer multiplication and division routines **
 *
 * The arguments are %o2, %o3 and the result is in %o2.
 * Note: this code assumes that .mul and .div don't trash any global or input
 * registers.
 */
	.global .mul, .div

/* ml_mul:
 * multiply %o2 by %o3, returning the result in %o2
 * Note: this code assumes that .mul doesn't trash any global or input
 * registers.
 */
_ml_mul:
	save	%sp,-SA(WINDOWSIZE),%sp
	mov	%i2,%o0
	call	.mul
	mov	%i3,%o1			/* (delay slot) */
	bnz	1f			/* if z is clear, then overflow */
	restore %o0,0,%o2		/* result in %o2 (delay slot) */
	retl
	nop
1:					/* handle overflow. */
	set	_overflow_e,%i0
	RAISE
	
/* ml_div:
 * divide %o2 by %o3, returning the result in %o2.
 * Note: .div uses %g1, %g2 and %g3, so we must save them.  We do this using the
 * locals of the new window, since .div is a leaf routine.
 */
_ml_div:
	save	%sp,-SA(WINDOWSIZE),%sp
	addcc	%i3,%g0,%o1		/* %o1 is divisor (and check for zero) */
	bz	1f
				    /* save %g1, %g2 and %g3 (using new window) */
	mov	%g1,%l1			/* (delay slot) */
	mov	%g2,%l2
	mov	%g3,%l3
	call	.div
	mov	%i2,%o0			/* (delay slot) */
				    /* restore %g1, %g2 and %g3 */
	mov	%l3,%g3
	mov	%l2,%g2
	mov	%l1,%g1
	ret
	restore %o0,0,%o2		/* result in %o2 (delay slot) */
1:				    /* handle zero divide */
	restore				/* restore ML window */
	set	_div_e,%i0
	RAISE


/* this bogosity is for export.c */
	.global _startptr
_startptr:
	.long	 start


#include <sun4/trap.h>

/* ml_longjmp:
 * Restore the ML register window.
 */
	.global	_ml_longjmp
_ml_longjmp:
	t	ST_FLUSH_WINDOWS	/* flush all reg windows to the stack */
	FETCH	(_bottom, %fp)		/* set %fp to the %sp of the ML frame */
	sub	%fp,SA(WINDOWSIZE),%sp
	restore				/* restore the ML frame, also restoring */
					/* the local and in registers. */
	ba	_saveregs
	nop


/* _set_fpsr:
 *
 *    Load the floating-point status register with the given word.
 */
	.global	_set_fsr
_set_fsr:
	set	fsrtmp,%o1
	st	%o0,[%o1]
	retl
	ld	[%o1],%fsr		/* (delay slot) */
	.seg	"data"
fsrtmp:	.word	0
