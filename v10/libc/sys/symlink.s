# C library -- symlink

# symlink(target, linkname);
#

	.set	symlink,57
.globl	_symlink
.globl  cerror

_symlink:
	.word	0x0000
	chmk	$symlink
	bcc 	noerror
	jmp 	cerror
noerror:
	ret
