# C library-- nap

# error = nap(nticks)

	.set	nap,64+41
.globl	_nap

_nap:
	.word	0x0000
	chmk	$nap
	ret
