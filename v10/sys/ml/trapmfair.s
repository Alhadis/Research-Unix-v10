#
# traps specific to the Mayfair
#
	.text

#
# memory error, soft or hard
#
	.globl	Xmemerr
	.align	2
Xmemerr:
	pushr	$SAVREG
	calls	$0,_memerr
	popr	$SAVREG
	rei
