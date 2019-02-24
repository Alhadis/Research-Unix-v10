#
# trap routines valid only on MicroVAX
#
# real clock interrupt, MicroVAX version
#
# clock ticks at 100Hz.  skip 4 of each 10 to pretend it's 60Hz

#
# the normal clock-trimming idea is to correct by CLKBIAS microseconds
# every 60Hz tick for clockbias ticks.  on MicroVAX, the interval
# has to be a multiple of the 100Hz tick; so skip or add a tick
# every SKIPBIAS or TICKBIAS ticks instead.
#
# if we skip a tick, the clock changes by 10ms (one hardware tick).
# if we insert an extra tick, the clock changes by 1s/60 - 10ms
# (because there's an extra hardware tick as well as a software tick)
# NB CLK60HZ is a number ready for NICR, hence it's negative
#

	.set	HWCLOCK,10000		# usec per hardware tick

	.set	SKIPBIAS,HWCLOCK/CLKBIAS	# value of a skipped tick
	.set	TICKBIAS,(-CLK60HZ-HWCLOCK)/CLKBIAS # value of an extra tick

	.text

	.lcomm	tick,1			# 100Hz-60Hz counter
	.globl	Xhardclock
	.align	2
Xhardclock:
	incb	tick
	cmpb	$1,tick
	beql	9f
	cmpb	$4,tick
	beql	9f
	cmpb	$6,tick
	beql	9f
	cmpb	$9,tick
	beql	9f
	cmpb	$10,tick
	bneq	1f
	clrb	tick
1:	pushr	$SAVREG
	movl	_clockbias,r0		# need clock adjustment?
	beql	3f
	decl	_clockbias
	movl	$SKIPBIAS,r2
	cvtbl	_clocksign,r3
	blss	2f
	 movl	$TICKBIAS,r2
2:	divl3	r2,r0,r1		# fudge to our units
	mull2	r2,r1
	cmpl	r0,r1			# time to adj by a whole uvax tick?
	bneq	3f			# not yet
	 decb	tick			# yes, invent an extra tick
	 tstl	r3
	 blss	8f			# need to slow down, extra tick is a skip
3:	pushl	4+NRS*4(sp)
	pushl	4+NRS*4(sp)
	calls	$2,_hardclock		# hardclock(pc,psl)
8:	popr	$SAVREG
9:	rei
