# C library -- funmount

# error = funmount(file)

	.set	funmount,50
.globl	_funmount
.globl  cerror

_funmount:
	.word	0x0000
	chmk	$funmount
	bcc 	noerror
	jmp 	cerror
noerror:
	clrl	r0
	ret
