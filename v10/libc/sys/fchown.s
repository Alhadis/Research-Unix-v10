# C library -- fchown

# error = fchown(fd, owner);

	.set	fchown,32
.globl	_fchown

_fchown:
	.word	0x0000
	chmk	$fchown
	bcc 	noerror
	jmp 	cerror
noerror:
	clrl	r0
	ret
