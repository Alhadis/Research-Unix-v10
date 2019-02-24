# C library -- setruid

# error = setruid(uid);

	.set	setruid,56
.globl	_setruid
.globl  cerror

_setruid:
	.word	0x0000
	chmk	$setruid
	bcc 	noerror
	jmp 	cerror
noerror:
	clrl	r0
	ret
