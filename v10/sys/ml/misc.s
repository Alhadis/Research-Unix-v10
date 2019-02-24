#
# miscellaneous assembly stuff
#

	.globl	_u
	.set	_u,KA - UPAGES*NBPG

	.text

_addupc:	.globl	_addupc
	.word	0x0
	movl	8(ap),r2		# &u.u_prof
	subl3	8(r2),4(ap),r0		# corrected pc
	blss	9f
	extzv	$1,$31,r0,r0		# logical right shift
	extzv	$1,$31,12(r2),r1	# ditto for scale
	emul	r1,r0,$0,r0
	ashq	$-14,r0,r0
	tstl	r1
	bneq	9f
	incl	r0
	bicl2	$1,r0
	cmpl	r0,4(r2)		# length
	bgequ	9f
	addl2	(r2),r0			# base
	probew	$3,$2,(r0)
	beql	8f
	addw2	12(ap),(r0)
9:
	ret
8:
	clrl	12(r2)
	ret

#
# 2-byte access to memory;
# useful for UNIBUS address space
# someone has already probed access
#
# UNIcpy(uniaddr, useraddr, count, direct)
# direct is nonzero (B_READ) if data is going to useraddr
#
# returns nonzero if access failed, like copyin
#

	.globl	_UNIcpy
_UNIcpy:
	.word	0
	movl	4(ap),r0	# from
	movl	8(ap),r1	# to
	tstl	16(ap)
	jneq	1f
	 movl	8(ap),r0	# from
	 movl	4(ap),r1	# to
1:
	divl3	$2,12(ap),r2	# byte count
	jeql	3f
	pushl	_nofault
	movab	9f,_nofault
2:
	movw	(r0)+,(r1)+
	sobgtr	r2,2b
3:
	movl	(sp)+,_nofault
	clrl	r0		# ok
	ret

9:
	movl	(sp)+,_nofault
	mnegl	$1,r0		# error
	ret

#
# `safe' (nofault-protected) mfpr/mtpr
# umptr returns 1 if ok, 0 if not
# umfpr returns 0 on error, which is ambiguous.  too bad.
# they should really set u.u_error.
#

	.globl	_umfpr
_umfpr:	.word	0
	pushl	_nofault
	movab	9f,_nofault
	mfpr	4(ap),r0
	movl	(sp)+,_nofault
	ret

	.globl	_umtpr
_umtpr:	.word	0
	pushl	_nofault
	movab	9f,_nofault
	mtpr	8(ap),4(ap)
	movl	(sp)+,_nofault
	movl	$1,r0
	ret

9:
	movl	(sp)+,_nofault
	clrl	r0
	ret

#
# here on stray interrupt
#

	.globl	Xstray
Xstray:
	pushr	$SAVREG
	subl3	$straycatch+3,NRS*4(sp),-(sp)
	calls	$1,_strayintr
	popr	$SAVREG
	tstl	(sp)+
	rei
