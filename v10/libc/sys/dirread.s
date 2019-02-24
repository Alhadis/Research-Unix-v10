	.set	dirread,22
.globl	_dirread
.globl  cerror

_dirread:
	.word	0x0000
	chmk	$dirread
	bcc 	noerror
	jmp 	cerror
noerror:
	ret
