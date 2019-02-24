#
# traps specific to the comet
#
	.text

#
# corrected memory error
# uncorrected ones show up as machine checks
#
	.globl	Xcmrd
	.align	2
Xcmrd:
	pushr	$SAVREG
	calls	$0,_memerr
	popr	$SAVREG
	rei

#
# asynchronous bus write timeout
#
	.globl	Xwtime
	.align	2
Xwtime:
	tstl	_nofault
	jneq	donofault
	pushr	$SAVREG
	pushl	NRS*4(sp)
	pushl	(NRS+1+1)*4(sp)
	calls	$2,_wtimeout
	popr	$SAVREG
	rei
