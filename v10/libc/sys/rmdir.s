# C library -- rmdir

# error = rmdir(string);

	.set	rmdir,64
.globl	_rmdir

_rmdir:
	.word	0x0000
	chmk	$rmdir
	bcc 	noerror
	jmp 	cerror
noerror:
	clrl	r0
	ret
