	.set	getlog,64+3

	.globl	_getlogname
_getlogname:
	.word	0
	clrl	-(sp)
1:
	pushl	4(ap)
	pushl	$2
	movl	sp,ap
	chmk	$getlog
	bcc	noerror
	jmp	cerror
noerror:
	clrl	r0
	ret

	.globl	_setlogname
_setlogname:
	.word	0
	pushl	$1
	brb	1b
