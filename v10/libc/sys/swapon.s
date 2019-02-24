# C library -- swapon

# error = swapon(string);

	.set	swapon,64+21
.globl	_swapon

_swapon:
	.word	0x0000
	chmk	$swapon
	bcc 	noerror
	jmp 	cerror
noerror:
	clrl	r0
	ret
