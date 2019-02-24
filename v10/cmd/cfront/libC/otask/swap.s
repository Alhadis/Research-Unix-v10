.globl _swap
	.align	1
_swap:
	.word	0x0000
	movl	4(ap),r1	#	r1 = this
	#  the following constant is the displacement of t_framep in task
	movl	20(r1),fp	#	fp = this->t_framep
	movl	24(r1),r0	#	fudge return -- this->th
	ret
