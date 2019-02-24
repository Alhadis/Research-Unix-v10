/* Copyright 1989 by AT&T Bell Laboratories */
#include "tags.h"
#include "prof.h"
#include "prim.h"
#define String(handle,len,str) .align 2;\
			       .long len*power_tags+tag_string;\
			       handle: .ascii str
/* args come in as
   a0 = arg
   a1 = continuation
   a2 = closure; can be ignored because contains no free vars
   a3,a4 = general purpose pointers
   a5 = pointer temp
   a6 = data ptr
   a7 = stack ptr
   d0 =  unused
   d1,d2,d4 = arithtemps
   d3 = pointer temp
   d5 = data limit
   d6 = store ptr
   d7 = exception handler
*/
#define Closure(name) .align	2;\
		      .long	mak_desc(1,tag_record);\
		      name:	.long 7f;\
		      .long	1;\
		      .long	tag_backptr;\
		      7:

	.text
	.globl	_runvec
	.align	2
	.long	mak_desc(8,tag_record)
_runvec:
	.long	_array_v
        .long	_callc_v
	.long	_create_b_v
	.long	_create_s_v
	.long	_floor_v
	.long	_logb_v
	.long	_scalb_v
	.long	_syscall_v

Closure(_array_v)
	movl a0@,d1		/* a0 = (int * int) */
	movl a0@(4),d3		/* d3 = initial value */
	asrl #1,d1		/* d1 = length */
	movl d1,d2
	asll #width_tags,d2
	orl #tag_array,d2	/* d2 = new tag */
	asll #2,d1
3:	movl a6,d4
	addl d1,d4
        cmpl d4,d5
        jmi  6f
	movl d2,a6@+
	movl a6,a0
	jra  2f
1:	movl d3,a6@+		/* store default */
2:	subql #4,d1
	jge  1b
	movl a1@,a2
	jmp a2@

	clrl d0
6:	trapmi
	jra 3b

Closure(_create_b_v)
	movl	#tag_bytearray,d3
	jra	2f
Closure(_create_s_v)
	movl	#tag_string,d3
2:	movl	a0,d1
	asrl	#1,d1		/* d1 = length */
	movl	d1,d2
	asll	#width_tags,d2
	addl	d3,d2		/* d2 = new tag */
	andl	#~3,d1		/* d1+4 = bytes in string, not including tag */
3:	movl	a6,d4
	addl	d1,d4
	cmpl	d4,d5
	jmi	1f
	movl	d2,a6@+
	movl	a6,a0
	addl	d1,a6
	clrl	a6@+		/* must clear the last 4 bytes */
	movl a1@,a2
	jmp a2@

	clrl	d0
1:	trapmi
	jra 3b

#define RAISE \
	movl	d7,a1 ; \
	movl	a1@,a2 ; \
	jra	a2@

.align 2
.globl _handleinterrupt
_handleinterrupt:
	movl	#0,sp@-
	jsr	_sigsetmask
	movl	_bottom, sp
	clrl	d3
	movl	d3,a0
	movl	d3,a1
	movl	d3,a2
	movl	d3,a3
	movl	d3,a4
	movl	d3,a5
	lea	_interrupt_e,a0
	RAISE

	.globl _saveregs
	.globl _handle_c
	.globl _return_c
	.globl _restoreregs
Closure(_handle_c)
	movl	#CAUSE_EXN,_cause;
	jra	_saveregs

Closure(_return_c)
	movl	#CAUSE_RET,_cause;
_saveregs:
	movl	a6,_saved_dataptr
	movl	d7,_saved_exnptr
	movl	d6,_saved_storeptr
        movl	d5,_saved_limit
	movl	a0,_saved_ptrs
	movl	a1,_saved_ptrs+4
	movl	a2,_saved_ptrs+8
	movl	a3,_saved_ptrs+12
	movl	a4,_saved_ptrs+16
	movl	a5,_saved_ptrs+20
	movl	d3,_saved_ptrs+24
	movl	d1,_saved_nonptrs
	movl	d2,_saved_nonptrs+4
	movl	d4,_saved_nonptrs+8
	movl	_bottom,sp
	movl	_fpsave,a6
#ifdef MACH
	moveml	sp@+, a0-a5/d0-d7
#endif MACH
	rts

_restoreregs:
#ifdef MACH
	/* C requires callee to save, and NeXT/MACH compiler uses registers */ 
	moveml	a0-a5/d0-d7,sp@-
#endif MACH
	movl	a6,_fpsave
	movl	sp,_bottom
	movl	_saved_dataptr,a6
	movl	_saved_exnptr,d7
	movl	_saved_storeptr,d6
        movl	_saved_limit,d5
	movl	_saved_ptrs,a0
	movl	_saved_ptrs+4,a1
	movl	_saved_ptrs+8,a2
	movl	_saved_ptrs+12,a3
	movl	_saved_ptrs+16,a4
	movl	_saved_ptrs+20,a5
	movl	_saved_ptrs+24,d3
	movl	_saved_nonptrs,d1
	movl	_saved_nonptrs+4,d2
	movl	_saved_nonptrs+8,d4
go:	movl	_saved_pc,sp@-	
	rts

/* Floating point primitives
 *
 * All the m68code for ML reals assumes that NaN's and INF's are never
 * generated and therefore do not need to be handled.
 * This code does not produce NaN's or INF's as long as none are passed to it
 * and overflow, underflow, and operr are handled.
 *
 * Floating exceptions raised (assuming NaN's and INF's are never passed to
 * functions):
 *	OPERR - (div) for 0.0/0.0 (does NOT also cause DZ)
 *	DZ - (div) for (in range) / 0.0
 *	OVERFLOW/UNDERFLOW - (add,div,sub,mul) as appropriate */

Closure(_floor_v)
	fmoved a0@,fp0
	ftstx fp0		| handle positive and negative cases separately
	fblt 1f
/* positive numbers */
	fintrzx fp0,fp0		| round towards zero (down)
	fmovel fp0,d0
	asll #1,d0
	trapv
	addql #1,d0
	movl d0,a0
	movl a1@,a2
	jmp a2@
/* negative numbers */
1:	fintrzx fp0,fp1		| round towards zero (up)
	fmovel fp1,d0
        asll #1,d0
	trapv
	fcmpx fp0,fp1
	fjeq 1f
	subql #1,d0
	trapv
	movl d0,a0
	movl a1@,a2
	jmp a2@
1:	addql #1,d0
	movl d0,a0
	movl a1@,a2
	jmp a2@

/* returns 0 on 0. */
Closure(_logb_v)
	fgetexpd a0@,fp0
	fmovel fp0,d0
	asll	#1,d0
	addql #1,d0
	movl d0,a0
	movl a1@,a2
	jmp a2@

Closure(_scalb_v)
	lea 2f,a0
	RAISE
	.align 2
	.long	mak_desc(2,tag_record);
2:	.long	1f
	.long	_real_e
String(1,19,"scalb unimplemented\0")

Closure(_callc_v)
	movl a1,sp@-
	movl a0@,a1
	movl a0@(4),sp@-
	jsr a1@
        movl d0,a0
        addql #4,sp
	movl sp@+,a1
	tstl _cause
	jne  _saveregs
	movl a1@,a2
	jmp a2@

#ifdef sony_news
	.data
	.align 2
_freestart:
	.long	0
	.text
#endif sony_news

Closure(_syscall_v)        /* ARGS: call#, arglist, argcount */
	movl	a0@(8),d0  /* arg count */
	asrl	#1,d0
	movl	d0,d1
	asll	#2,d1
	subl	d1,sp
	movl	sp,a2
	movl	a0@(4),a3
1:	subl	#1,d0
	blt	2f
	movl	a3@,d1
	movl	a3@(4),a3
	btst	#0,d1
	jeq	3f
	asrl	#1,d1
3:	movl	d1,a2@+
	jra	1b
2:	pea	0
	movl	a0@,d0
	asrl	#1,d0
	movl	d0,sp@-
#ifdef sony_news
	movl	a6,_freestart		/* save help pointer */
	movl	sp,a6			/* tell kernel args ptr */
#endif sony_news
	trap	#0
	jcs	1f
#ifdef sony_news
	movl	_freestart,a6		/* restore heap pointer */ 
#endif sony_news
	movl	a2,sp
	addl	d0,d0
	addql	#1,d0
	movl	d0,a0
	movl a1@,a2
	jmp a2@

#ifdef sony_news
1:	movl	_freestart,a6		/* restore heap pointer */
	movl	a2,sp
#else
1:	movl	a2,sp
#endif sony_news
        movl	d0,_errno
	movl	#-1,d0
	movl	d0,a0
	movl a1@,a2
	jmp a2@


#ifndef sony_news
#ifndef MACH
.globl _minitfp_		/* checks for 68881 and sets flags */
.globl _fp_state_mc68881	/* a flag that gets set */
#define fp_enabled 2 /* from /usr/src/lib/libc/sun/crt/fpcrttypes.h */
#endif MACH
#endif sony_news
.align 2
/* Enable/disable float operand error, overflow, and div.  If no 68881
   is present, nothing happens. */
.globl _fpenable
.globl _fpdisable
_fpenable:
#ifndef sony_news
#ifndef MACH
	jsr _minitfp_			/* checks for 68881 and sets flags.
					   normally executed on startup,
					   but won't be if compiled without
					   -f68881 (for possible sun/50
					   compatibility).  This is just
					   to make sure. */
	cmpl #fp_enabled,_fp_state_mc68881
	jne 1f
#endif MACH
#endif sony_news
	fmovel #0x3400,fpcr
1:	rts
_fpdisable:
#ifndef sony_news
#ifndef MACH
	cmpl #fp_enabled,_fp_state_mc68881
	jne 1f
#endif MACH
#endif sony_news
	fmovel #0,fpcr
1:	rts

/* From here on used to be M68.math.s */
	.text
	.align	2
	.globl	_mathvec
	.long	mak_desc(9,tag_record)
_mathvec:
	.long	_exp_e
	.long	_ln_e
	.long	_sqrt_e
	.long	_atan_v
	.long	_cos_v
	.long	_exp_v
	.long	_ln_v
	.long	_sin_v
	.long   _sqrt_v
end_mathvec:

	.align 2
	.globl _math_functor
Closure(_math_functor)
	lea _mathvec,a0
	movl a1@,a2
	jmp a2@

	.align 2
	.long mak_desc(2,tag_record)
_exp_e:	.long 1
	.long 1f
	.long mak_desc(1,tag_array)
1:	.long 1f
String(1,3,"Exp\0")

	.align 2
	.long mak_desc(2,tag_record)
_ln_e:	.long 1
	.long 1f
	.long mak_desc(1,tag_array)
1:	.long 1f
String(1,2,"Ln\0\0")

	.align 2
	.long mak_desc(2,tag_record)
_sqrt_e:
	.long 1
	.long 1f
	.long mak_desc(1,tag_array)
1:	.long 1f
String(1,4,"Sqrt")

Closure(_sin_v)
	fsind a0@,fp0
finishfloat:
        cmpl a6,d5
        trapmi
	movl #mak_desc(8,tag_string),a6@+
	movl a6,a0
	fmoved fp0,a6@+
	movl a1@,a2
	jmp a2@

Closure(_cos_v)
	fcosd a0@,fp0
        jra finishfloat
Closure(_ln_v)
	flognd a0@,fp0
        jra finishfloat
Closure(_exp_v)
	fetoxd a0@,fp0
        jra finishfloat
Closure(_sqrt_v)
	fsqrtd a0@,fp0
        jra finishfloat
Closure(_atan_v)
	fatand a0@,fp0
        jra finishfloat

/* this bogosity is for export.c */
	.globl	_startptr
_startptr: .long    start
