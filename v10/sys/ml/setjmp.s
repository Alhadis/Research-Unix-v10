
/*
 * non-local goto's
 */

/*
 * the microvax constantly updates the ksp in the pcb block so
 * we can't trust a ldpctx to get the right sp for longjmps
 */
	.text

	.set	PCLOC,16	# location of pc in calls frame
	.set	APLOC,8		# location of ap,fp in calls frame

	.globl	_Longjmp
_Longjmp:
	movl	(r0)+,newfp	# must save parameters in memory as all
	movl	(r0),newpc	# registers may be clobbered.
1:
	cmpl	fp,newfp	# are we there yet?
	bgequ	2f		# yes
	moval	1b,PCLOC(fp)	# redirect return pc to us!
	ret			# pop next frame
2:
	beql	3f		# did we miss our frame?
	pushab	4f		# yep ?!?
	calls	$1,_panic
3:
	movl	newpc,r0	# all done, just return to the `setjmp'
	jmp	(r0)		# ``rsb''

	.data
newpc:	.space	4
newfp:	.space	4
4:	.asciz	"longjmp"
	.text
/*
 * setjmp that saves all registers as the call frame may not
 * be available to recover them in the usual mannor by longjmp.
 * Called before swapping out the u. area, restored by resume()
 * below.
 */

	.globl	_savectx
	.align	1
_savectx:
	.word	0
	movl	4(ap),r0
	movq	r6,(r0)+
	movq	r8,(r0)+
	movq	r10,(r0)+
	movq	APLOC(fp),(r0)+	# save ap, fp
	addl3	$8,ap,(r0)+	# save sp
	movl	PCLOC(fp),(r0)	# save pc
	clrl	r0
	ret
