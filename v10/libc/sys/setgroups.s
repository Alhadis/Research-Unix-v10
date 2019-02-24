# C library -- setgroups

.globl	_setgroups
.globl	cerror

	.set	setgroups,74

_setgroups:
	.word	0x0000
				# Entry
	chmk	$setgroups
	bcc	noerror
	jmp	cerror
noerror:
	ret
