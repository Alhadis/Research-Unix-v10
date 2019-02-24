#
# trap code specific to DW780 --
# SBI UNIBUS adapter
#
# pull the appropriate vector register out of the air
# if the high bit is set or the number is 0, call the driver
# else call the proper interrupt routine
#
# must supply an entry point for each unibus adapter,
# by hand; never mind why, for now.
#

	.set	PSIZE,2		# sizeof(pushl $SAVREG)
	.set	LOCREG,0x7	# registers needed locally -- r0 r1 r2

#
# entry points; up to 3 DWs for now
#
	.globl	Xuba30
	.globl	Xuba20
	.globl	Xuba10
	.globl	Xuba00
	.align	2
Xuba30:	pushr	$SAVREG
	movl	$3,r0
	brb	ubint
	.align	2
Xuba20:	pushr	$SAVREG
	movl	$2,r0
	brb	ubint
	.align	2
Xuba10:	pushr	$SAVREG
	movl	$1,r0
	brb	ubint
	.align	2
Xuba00:	pushr	$SAVREG
	clrl	r0
# fall in ...
ubint:
	movl	_ubavreg[r0],r1	# pick up address of BRRVRs
	beql	2f
	mfpr	$IPL,r2
	extzv	$0,$2,r2,r2
	movl	(r1)[r2],r1	# grab the appropriate BRRVR
	bleq	1f		# br if adapter intr
0:	addl3	$scb,_ubavoff[r0],r0	# addr of first vector for this adapter
	addl2	r1,r0		# addr of addr of interrupt routine
	bicl3	$3,(r0),r0	# pick up addr, clear stack bits
	jmp	PSIZE(r0)	# skip initial pushr and go there

#
# here for an adapter interrupt
#
1:	pushr	$LOCREG
	movq	r0,-(sp)	# pushl r1; pushl r0
	calls	$2,_uba0int	# service interrupt
	popr	$LOCREG
	bbcc	$31,r1,2f	# clear `need service' bit
2:	tstl	r1		# anything left?
	bgtr	0b		# yes, go do intr
8:	popr	$SAVREG
	rei

#
# here if the adapter hasn't been initialized,
# but interrupted anyway
# perhaps a disaster -- will it just keep interrupting?
#
ubstray:
	mfpr	$IPL,-(sp)
	pushl	r0
	pushab	9f
	calls	$3,_printf
	brb	8b

9:	.asciz	"ub%d: intr without init, ipl%x\n"
