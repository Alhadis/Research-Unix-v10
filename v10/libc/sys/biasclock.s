#
# sys biasclock
#
	.set	biasclock,52
.globl	_biasclock
_biasclock:
	.word	0
	chmk	$biasclock
	bcc	1f
	jmp	cerror
1:	ret
