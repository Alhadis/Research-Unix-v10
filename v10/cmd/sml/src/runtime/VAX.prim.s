/* Copyright 1989 by AT&T Bell Laboratories */
#include "tags.h"
#include "prof.h"
#include "prim.h"
#define String(handle,len,str) .align 2;\
			       .long len*power_tags+tag_string;\
			       handle: .ascii str
/* args come in as
   r2 = closure; can be ignored because contains no free vars
   r0 = arg
   r1 = continuation

   registers 0-7 can contain only pointers or tagged integers
   r8 is the data limit
   registers 9-10 can contain anything except pointers
   r11 is the store pointer
   r12 is the data ptr
   r13 is the exception handler
   r14 is the stack pointer (mostly unused)
   r15 is the program counter
*/
#define Closure(name) .align	2;\
		      .long	mak_desc(1,tag_record);\
		      name:	.long 9f;\
		      .long	1;\
		      .long	tag_backptr;\
		      9:

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
4:	ashl $-1,(r0),r9	/* r9 = length */
	ashl $2,r9,r10
        addl2 r12,r10
        addl2 $0x80000000,r10
        subl2 $4096,r10
        addl2 r8,r10
        jgeq 3f
	ashl $width_tags,r9,r10
	bisl3 $tag_array,r10,-4(r12)
	movl 4(r0),r2		/* r2 = initial value */
	movl r12,r0
	jbr 2f
1:	movl r2,(r12)+		/* store default */
2:	sobgeq	r9,1b
	addl2 $4,r12
	jmp *(r1)
3:      movl	$0x7fffffff,r8
        addl3	r12,r8,r9		/* guaranteed to fault */
        jbr	4b

Closure(_create_b_v)
	movl	$tag_bytearray,r4
	jbr	2f
Closure(_create_s_v)
	movl	$tag_string,r4
2:	addl3	$13,r0,r10
	ashl	$-3,r10,r10
	ashl	$2,r10,r10	/* r9 = bytes in string including tag */
        movl	r10,r9
        addl2	r12,r10
        addl2	$0x80000000,r10
        subl2	$4096,r10
        addl2 	r8,r10
        jgeq 	3f
	ashl	$-1,r0,r10	/* r10 = length */
	ashl	$width_tags,r10,r10
	bisl3	r4,r10,-4(r12)	/* new tag */
	movl	r12,r0
	addl2	r9,r12
	jmp	*(r1)
3:      movl	$0x7fffffff,r8
        addl3	r12,r8,r9		/* guaranteed to fault */
        jbr	2b

	.globl _saveregs
	.globl _handle_c
	.globl _return_c
	.globl _restoreregs
Closure(_handle_c)
	movl	$CAUSE_EXN,_cause;
	jbr	_saveregs
Closure(_return_c)
	movl	$CAUSE_RET,_cause;
_saveregs:
	subl3	$4,r12,_saved_dataptr
	movl	r13,_saved_exnptr
	movl	r11,_saved_storeptr
	movq	r0,_saved_ptrs
	movq	r2,_saved_ptrs+8
	movq	r4,_saved_ptrs+16
	movq	r6,_saved_ptrs+24
/*	movl	r8,_saved_limit */
	movq	r9,_saved_nonptrs
	movl	_bottom,sp
	movl	_fpsave,fp
	ret

_restoreregs:
	.word	0x4000
	movl	fp,_fpsave
	movl	sp,_bottom
	movq	_saved_nonptrs,r9
	addl3	$4,_saved_dataptr,r12
	movl	_saved_exnptr,r13
	movl	_saved_storeptr,r11
#ifdef BSD
        pushl   $0
        calls   $1,_sigsetmask
#endif
#ifdef V9
        calls   $0,_setupsignals
#endif
	subl3	_saved_limit,$0x7fffffff,r8
        incl	r8
	movq	_saved_ptrs,r0
	movq	_saved_ptrs+8,r2
	movq	_saved_ptrs+16,r4
	movq	_saved_ptrs+24,r6
go:	jmp	*_saved_pc

/* Floating exceptions raised (assuming ROP's are never passed to functions):
 *	DIVIDE BY ZERO - (div)
 *	OVERFLOW/UNDERFLOW - (add,div,sub,mul) as appropriate
 *
 * floor raises integer overflow if the float is out of 32-bit range,
 * so the float is tested before conversion, to make sure it is in (31-bit)
 * range */

Closure(_floor_v)
        .byte 0xfd; cvtfl (r0),r9	# cvtgl
        .byte 0xfd; tstf (r0)		# tstg
	bgeq 1f
	.byte 0xfd; cvtlf r9,r4		# cvtlg, to handle negative
	.byte 0xfd; cmpf (r0),r4	# cmpg
	beql 2f
	decl r9
2:	clrq r4
1:	ashl $1,r9,r9
	bisl3 $1,r9,r0
	jmp *(r1)

Closure(_logb_v)
	bicl3	$0xffff800f,(r0),r9	# grab exponent
	ashl	$-3,r9,r9
	subl2	$2048,r9		# unbias
	bisl3	$1,r9,r0
	jmp	*(r1)

Closure(_scalb_v)
        addl3	r12,r8,r9
	bicl3	$1,4(r0),r9		# grab add value
	beql	1f			# 0?
        ashl	$3,r9,r9		# shift to exponent field
 	movl	(r0),r0			# grab old float
	bicl3	$0xffff800f,(r0),r10	# grab exponent
	addl2	r9,r10			# check out the new exponent
	bleq	under			# too small?
	cmpl	r10,$0x8000
	bgeq	over			# too large?
	movl	4(r0),4(r12)
	addl3	(r0),r9,(r12)
	movl 	$mak_desc(8,tag_string),-4(r12)
	movl	r12,r0
	addl2	$12,r12
	jmp	*(r1)
1:      movl    (r0),r0
        jmp     *(r1)
over:	moval	1f,r0
	jbr	_raise_real
String(1,8,"overflow")
under:	moval	1f,r0
	jbr	_raise_real
String(1,9,"underflow\0\0\0")

_raise_real:
        addl3	r12,r8,r9    /* ensure enough space */
	moval _real_e,4(r12)
	movl  r0,(r12)
	movl  $mak_desc(2,tag_record),-4(r12)
	movl  r12,r0
	addl2 $12,r12
	movl	r13,r1
	jmp	*(r13)

Closure(_syscall_v)         /* ARGS: call#, arglist, argcount */
        addl3	r12,r8,r9  /* ensure enough space */
	ashl	$-1,8(r0),r9
	clrl	r10
	movl	4(r0),r5
1:	tstl	r9
	jleq	2f
	decl	r9
	incl	r10
	movl	(r5),r2
	movl	4(r5),r5
	jlbs	r2,3f
	movl	r2,(ap)[r10]
	jbr	1b
3:	ashl	$-1,r2,(ap)[r10]
	jbr	1b
2:	ashl	$-1,8(r0),(ap)
	ashl	$-1,(r0),r0
	clrl	r2
	chmk	r0
	jcs 1f
	addl2	r0,r0
	incl	r0
	jmp	*(r1)
1:	movl	r0,_errno
	movl	$-1,r0
	jmp	*(r1)

Closure(_callc_v)
	pushl r1
	pushl 4(r0)
	calls $1,*(r0)
	movl  (sp)+,r1
	clrq  r2
	clrq  r4
        tstl  _cause
        jneq  _saveregs
	jmp   *(r1)

/* this bogosity is for export.c */
	.globl	_startptr
_startptr: .long    start
