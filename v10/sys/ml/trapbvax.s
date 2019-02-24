#
# traps routines valid only on `bigger' VAXes
# (not MicroVAXes)
#
# real clock interrupt
#
	.text
	.globl	Xhardclock
	.align	2
Xhardclock:
	pushr	$SAVREG
	tstl	_clockbias
	beql	1f
	decl	_clockbias
	bneq	1f
	mtpr	$CLK60HZ,$NICR
1:	pushl	4+NRS*4(sp)
	pushl	4+NRS*4(sp)
	calls	$2,_hardclock		# hardclock(pc,psl)
2:	popr	$SAVREG
	rei
