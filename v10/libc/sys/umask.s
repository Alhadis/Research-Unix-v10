#  C library -- umask
 
#  omask = umask(mode);
 
	.set	umask,60
.globl	_umask

_umask:
	.word	0x0000
	chmk	$umask
	ret
