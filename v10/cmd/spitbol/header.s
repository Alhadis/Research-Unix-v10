
#
#	globls for internal compiler blocks.
#
	.globl	b$icl
	.globl	b$rcl
	.globl	b$scl
	.globl	b$xnt
	.globl	b$xrt
#
#	globl for &case keyword.
#
	.globl	kvcas
#
#	globl for compiler fcb chain.
#
	.globl	r$fcb
#
#	globl for next available spot in dynamic area
#
	.globl	dnamp
#
#	globls for pointers into stack that must be relocated
#	during a restart.
#
	.globl	flptr
	.globl	stbas
	.globl	gtcef
#
#	invalid$ error entry
#
	.text
invalid$:
	movl	$999,r6
	jmp	error

