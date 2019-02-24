# C library -- lstat
#   (stat without link following)

# lstat(name, &statb);
#

	.set	lstat,40
.globl	_lstat
.globl  cerror

_lstat:
	.word	0x0000
	chmk	$lstat
	bcc 	noerror
	jmp 	cerror
noerror:
	ret
