	.globl _htonl
	.align 2
_htonl: .word 0
	rotl	$-8,4(ap),r0
	insv	r0,$16,$8,r0
	movb	7(ap),r0
	ret

	.globl _htons
	.align 2
_htons: .word 0
	rotl	$8,4(ap),r0
	movb	5(ap),r0
	movzwl	r0,r0
	ret

	.globl _ntohl
	.align 2
_ntohl: .word 0
	rotl	$-8,4(ap),r0
	insv	r0,$16,$8,r0
	movb	7(ap),r0
	ret

	.globl _ntohs
	.align 2
_ntohs: .word 0
	rotl	$8,4(ap),r0
	movb	5(ap),r0
	movzwl	r0,r0
	ret

