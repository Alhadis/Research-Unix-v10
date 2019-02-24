# C library -- getgroups

.globl	_getgroups
.globl	cerror

	.set	getgroups,75

_getgroups:
	.word	0x0000
				# Entry
	chmk	$getgroups
	bcc	noerror
	jmp	cerror
noerror:
	ret
