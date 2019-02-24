/* Copyright 1989 by AT&T Bell Laboratories */
#include "tags.h"
#include "prof.h"
#include "prim.h"
#define String(handle,len,str) .align 2;\
			       .double len*power_tags+tag_string;\
			       handle: .ascii str
/* args come in as
   r2 = closure; can be ignored because contains no free vars
   r0 = arg
   r1 = continuation

   registers 0-4 can contain only pointers or tagged integers
   register 5 is ptrtemp, and it could be anything?!?
   registers 6-7 can contain anything except pointers
   4(sb) is the store pointer
   fp is the data ptr
   0(sb) is the exception handler
   sp is the stack pointer (mostly unused)
   pc is the program counter
   sb is the base address for the exception handler and store pointer
   mod is the base address for sb
*/
#define Closure(name) .align	2;\
		      .double	mak_desc(1,tag_record);\
		      name:	.double .L/**/name/**/code;\
		      .double	1;\
		      .double	tag_backptr;\
		      .L/**/name/**/code:

	.text
	.globl	_mlmodreg
	.align	2
_mlmodreg:
	.double	_mlsbreg
	.data
_saved_cmodreg:
	.double	0		/* keep C happy, just in case */
_saved_csbreg:
	.double	0
	.globl	_mlsbreg
_mlsbreg:
	.double	0		/* exnptr */
	.double	0		/* storeptr */
	.text
	/* Aligned strings for floating point exceptions */
String(.Loflw,8,"overflow")
String(.Luflw,9,"underflow\0\0\0")
	.globl	_runvec
	.align	2
        .double	mak_desc(8,tag_record)
_runvec:
	.double	_array_v
        .double	_callc_v
	.double	_create_b_v
	.double	_create_s_v
	.double	_floor_v
	.double	_logb_v
	.double	_scalb_v
	.double	_syscall_v

Closure(_array_v)
	movd	0(r0),r6	/* r6 = ML_int(length) */
	ashd	$-1,r6		/* r6 = length */
	movd	4(r0),r2	/* r2 = initial value */
	movd	r6,r7
	ashd	$width_tags,r7
	ord	$tag_array,r7	/* r7 = proper tag for array */
	movzbd	$0,-4(fp)[r6:d]	/* allocate (maybe fault) */
	movd	r7,-4(fp)	/* set array tag */
	sprd	fp,r0		/* return value is array start */
	addr	4(fp)[r6:d],r5	/* r5 = new freespace pointer */
	br	.Ltest
.Ltop:	movd	r2,0(fp)[r6:d]	/* store default at array sub r6 */
.Ltest: acbd	$-1,r6,.Ltop
	movd	r2,0(fp)	/* store default at array sub 0 */
	lprd	fp,r5		/* update freespace pointer */
	movd	0(r1),r5	/* r5 = code pointer for cont */
	jump	0(r5)

Closure(_create_b_v)
	movd	$tag_bytearray,r4
	br	.Lcre
Closure(_create_s_v)
	movd	$tag_string,r4
.Lcre:	addr	13(r0),r7	/* r0 = ML_int(length) */
	ashd	$-3,r7		/* r7 = (length + 7) div 4 */
	movd	r7,r6
	ashd	$2,r6		/* r6 = bytes in string including tag */
	addqd	$-1,r7		/* r7 = words in string, not including tag */
	movzbd	$0,-4(fp)[r7:d]	/* allocate (maybe fault) */
	movd	r0,r7
	ashd	$-1,r7		/* r7 = length */
	ashd	$width_tags,r7
	ord	r4,r7
	movd	r7,-4(fp)	/* install new tag */
	sprd	fp,r0		/* return value is start of object */
	addr	0(fp)[r6:b],r5
	lprd	fp,r5		/* uptdate freespace pointer */
	movd	0(r1),r5
	jump	0(r5)

	.globl _saveregs
	.globl _handle_c
	.globl _return_c
	.globl _restoreregs
Closure(_handle_c)
	movd	$CAUSE_EXN,_cause;
	br	_saveregs
Closure(_return_c)
	movd	$CAUSE_RET,_cause;
_saveregs:
	addr	-4(fp),_saved_dataptr
	movd	0(sb),_saved_exnptr
	movd	4(sb),_saved_storeptr
	movd	r0,_saved_ptrs
	movd	r1,_saved_ptrs+4
	movd	r2,_saved_ptrs+8
	movd	r3,_saved_ptrs+12
	movd	r4,_saved_ptrs+16
	movd	r5,_saved_nonptrs
	movd	r6,_saved_nonptrs+4
	movd	r7,_saved_nonptrs+8
	lprd	sp,_bottom
	lprd	fp,_fpsave
	lprw	mod,_saved_cmodreg
	lprd	sb,_saved_csbreg
	exit	[r0,r1,r2,r3,r4,r5,r6,r7]
	ret	$0

_restoreregs:
/*	.word	0x4000		/* What is this for on the VAX??? */
				/* Is it the register mask, for C calls? */
	enter	[r0,r1,r2,r3,r4,r5,r6,r7],$0
	sprd	fp,_fpsave
	sprd	sp,_bottom
	sprd	sb,_saved_csbreg
	sprw	mod,_saved_cmodreg
	movd	_saved_nonptrs,r5
	movd	_saved_nonptrs+4,r6
	movd	_saved_nonptrs+8,r7
	movd	_saved_dataptr,r0
	addqd	$4,r0
	lprd	fp,r0
	addr	_mlmodreg,r0
	lprw	mod,r0		/* _mlmodreg had better be below 64K */
	lprd	sb,0(r0)	/* this will always be _mlsbreg */
	movd	_saved_exnptr,0(sb)
	movd	_saved_storeptr,4(sb)
#ifdef BSD
        movd	$0,TOS
	bsr	?_sigsetmask
	adjspb	$-4
#endif
#ifdef V9
        bsr	?_setupsignals
#endif
	movd	_saved_ptrs,r0
	movd	_saved_ptrs+4,r1
	movd	_saved_ptrs+8,r2
	movd	_saved_ptrs+12,r3
	movd	_saved_ptrs+16,r4
go:	movd	_saved_pc,tos
	ret	$0

/* Floating exceptions raised (assuming ROP's are never passed to functions):
 *	DIVIDE BY ZERO - (div)
 *	OVERFLOW/UNDERFLOW - (add,div,sub,mul) as appropriate
 *
 * floor raises integer overflow if the float is out of 32-bit range,
 * so the float is tested before conversion, to make sure it is in (31-bit)
 * range */

Closure(_floor_v)
	floorld	0(r0),r6
	ashd	$1,r6
	ord	$1,r6
	movd	r6,r0
	movd	0(r1),r5
	jump	0(r5)

/*
 * I don't know whether this routine is supposed to get
 * the log base 2, or one more than that.
 * On the Vax, it seems to do the latter, although in
 * my port to the Sony, it does the former.
 */
Closure(_logb_v)
	movd	4(r0),r6	/* Second half, containing exponent */
	bicd	$0x800fffff,r6	/* grab only the exponent */
	ashd	$-19,r6		/* move into position */
	subd	$2046,r6	/* unbias */
	ord	$1,r6		/* convert to ML int */
	movd	r6,r0		/* give as arg to cont */
	movd	0(r1),r5	/* follow cont */
	jump	0(r5)

Closure(_scalb_v)
				/* args: float and incr */
	movd	4(r0),r6
	bicd	$1,r6			/* grab add value */
	cmpqd	$0,r6
	beq	.Lnoth			/* 0? */
        ashd	$3,r6			/* shift to exponent field */
 	movd	0(r0),r0		/* grab old float */
	movd	4(r0),r7
	bicd	$0x800fffff,r7		/* grab exponent */
	ashd	$-16,r7			/* move to vax exp field */
	addd	r6,r7			/* check out the new exponent */
	cmpd	r7,$0
	ble	under			/* too small? */
	cmpd	r7,$0x8000
	bge	over			/* too large? */
	ashd	$16,r6			/* move to ns32 exp field */
	addd	4(r0),r6
	movd	r6,4(fp)		/* place second half of float */
	movd	0(r0),0(fp)		/* place first half of float */
	movd 	$mak_desc(8,tag_string),-4(fp)	/* place tag */
	sprd	fp,r0			/* return value is new float */
	addr	12(fp),r5		/* update freespace pointer */
	lprd	fp,r5
	movd	0(r1),r5		/* follow cont */
	jump	0(r5)
.Lnoth: movd    0(r0),r0		/* return original float */
	movd	0(r1),r5		/* follow cont */
	jump	0(r5)
over:	movzbd	$0,4(fp)		/* make space */
	addr	.Loflw,r0
	br	_raise_real
under:	movzbd	$0,4(fp)		/* make space */
	addr	.Luflw,r0
	br	_raise_real

_raise_real:
	/* Danger: syntax of the C macro _real_e may matter here! */
	addr	_real_e,4(fp)	/* allocate arg record for exception */
	movd	r0,0(fp)
	movd	$mak_desc(2,tag_record),-4(fp)
	sprd	fp,r0		/* arg to handler is exception value */
	addr	12(fp),r5
	lprd	fp,r5		/* update freespace pointer */
	movd	0(sb),r1	/* get handler's cont */
	movd	0(r1),r5	/* jump to handler */
	jump	0(r5)

Closure(_syscall_v)	/* ARGS: call#, arglist, argcount */
	movzbd	$0,20(fp)	/* ensure enough space */
	movd	8(r0),r6
	ashd	$-1,r6		/* r6 = argcount */
	movzbd	$0,r7		/* r7 = 0 */
	movd	4(r0),r4	/* r4 = arglist */
.Largloop:
	cmpqd	$0,r6
	bge	.Largdone
	addqd	$-1,r6
	addqd	$1,r7
	movd	0(r4),r2	/* r2 = hd(r4) */
	movd	4(r4),r4	/* r4 = tl(r4) */
	tbitb	$0,r2		/* test tag bit of r2 */
	bfs	.Largint
	movd	r2,0(fp)[r7:d]	/* place arg */
	br	.Largloop
.Largint:
	movd	r2,r5		/* convert from ML to C int */
	ashd	$-1,r5
	movd	r5,0(fp)[r7:d]	/* place arg */
	br	.Largloop
.Largdone:
	movd	8(r0),r5
	ashd	$-1,r5
	movd	r5,0(fp)	/* place argcount */
	movd	0(r0),r0	/* ML int call# */
	ashd	$-1,r0		/* call# in r0 */
	movd	r1,r5		/* save our continuation */
	addr	4(fp),r1	/* addr of first arg in r1 */
	movzbd	$0,r2		/* clear r2, why? */
	svc
	bcs	.Lsyslose
	addd	r0,r0		/* convert result to ML int */
	addqd	$1,r0
	movd	r5,r1		/* restore cont */
	movd	0(r1),r5	/* follow cont */
	jump	0(r5)
.Lsyslose:
	movd	r0,_errno
	movd	$-1,r0		/* system call lost */
	movd	r5,r1		/* restore cont */
	movd	0(r1),r5	/* follow cont */
	jump	0(r5)

Closure(_callc_v)
	movd	r1,TOS
	movd	4(r0),TOS
	movd	0(r0),r5
	lprw	mod,_saved_cmodreg	/* keep C happy, just in case */
	lprd	sb,_saved_csbreg
	jsr	0(r5)		/* call C routine */
	adjspb	$-4		/* pop single C arg */
	addr	_mlmodreg,r5		/* put ml mod and sb back */
	lprw	mod,r5
	lprd	sb,0(r5)
	movd	TOS,r1
	movzbd	$0,r2
	movzbd	$0,r3
	movzbd	$0,r4
	movzbd	$0,r5
        cmpqd	$0,_cause
        bne	_saveregs
	movd	0(r1),r5
	jump	0(r5)

/* this bogosity is for export.c */
	.globl	_startptr
_startptr: .double    start
