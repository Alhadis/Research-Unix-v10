
# C library -- chmod

# error = fchmod(fd, mode);

	.set	fchmod,31
.globl	_fchmod

_fchmod:
	.word	0x0000
	chmk	$fchmod
	bcc 	noerror
	jmp 	cerror
noerror:
	clrl	r0
	ret
