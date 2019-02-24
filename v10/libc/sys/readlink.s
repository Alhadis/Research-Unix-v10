# C library -- readlink

# nread = readlink(name, buf, count)
#

	.set	readlink,58
.globl	_readlink
.globl  cerror

_readlink:
	.word	0x0000
	chmk	$readlink
	bcc 	noerror
	jmp 	cerror
noerror:
	ret
