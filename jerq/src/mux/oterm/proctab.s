# This MUST be the very last thing in bss; NPROC is a variable
# set according to VALMAXADDR
	.data
	.align	4
	.globl	NPROC
NPROC:
	.word	0
	.globl	proctab
proctab:
