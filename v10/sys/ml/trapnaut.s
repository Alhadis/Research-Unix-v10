#
# traps specific to the nautilus
#
	.text

#
# nmi fault
#
	.globl	Xnmiflt
	.align	2
Xnmiflt:
	tstl	_nofault
	jneq	donofault
	pushr	$SAVREG
	calls	$0,_nmiflt
xpop:	popr	$SAVREG
	rei

#
# memory interrupt
#
	.globl	Xmemerr
	.align	2
Xmemerr:
	pushr	$SAVREG
	calls	$0,_mcrint
	jbr	xpop

#
# nmi device interrupt
#
	.globl	Xnbia0
	.align	2
Xnbia0:
	pushr	$SAVREG
	pushl	$0
xnbia:	calls	$1,_nbiaintr
	jbr	xpop

	.globl	Xnbia1
	.align	2
Xnbia1:
	pushr	$SAVREG
	pushl	$1
	jbr	xnbia
