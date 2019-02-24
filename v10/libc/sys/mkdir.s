# C library -- mkdir

# error = mkdir(string, mode);

	.set	mkdir,65
.globl	_mkdir

_mkdir:
	.word	0x0000
	chmk	$mkdir
	bcc 	noerror
	jmp 	cerror
noerror:
	clrl	r0
	ret
