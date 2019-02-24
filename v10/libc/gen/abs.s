# abs - int absolute value.
# labs - long abs

.globl	_abs
.globl	_labs
	.align	1
_abs:
_labs:
	.word	0x0000
	movl	4(ap),r0
	bgeq	absl
	mnegl	r0,r0
absl:
	ret
