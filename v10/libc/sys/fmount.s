# C library -- fmount

# error = fmount(type, fd, file, flag)

	.set	fmount,26
.globl	_fmount
.globl  cerror

_fmount:
	.word	0x0000
	chmk	$fmount
	bcc 	noerror
	jmp 	cerror
noerror:
	clrl	r0
	ret
