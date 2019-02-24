#
# VAX-11/780 (star) -specific traps
#

#
# memory error
#

	.globl	Xmemerr
	.align	2
Xmemerr:
	pushr	$SAVREG
	calls	$0,_memerr
	popr	$SAVREG
	rei

#
# SBI fault
#

	.globl	Xsbiflt
	.align	2
Xsbiflt:
	pushr	$SAVREG
	calls	$0,_sbifault
	popr	$SAVREG
	rei

#
# memory write timeout
#

	.globl	Xwtime
	.align	2
Xwtime:
	pushr	$SAVREG
	calls	$0,_machwtmo
	popr	$SAVREG
	rei
