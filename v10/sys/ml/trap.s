#
# trap and fault interfaces
# these are common to all (big) vaxes
# the microvaxes are elsewhere
#

	.comm	_nofault,4	# where to go instead of death on some traps

	.text

#
# machine check: the VAX catchall fault
#
	.globl	Xmachcheck
	.align	2
Xmachcheck:
	pushr	$SAVREG
	tstl	_nofault	# machine check expected?
	jeql	1f
	 calls	$0,_machreset	# yes.  clear fault indicators
	 popr	$SAVREG
	 addl2	(sp)+,sp
	 jbr	donofault
1:
	addl3	sp,NRS*4(sp),r0
	addl2	$8+(NRS*4),r0
	pushab	NRS*4(sp)	# machine check vector
	pushl	(r0)		# saved ps
	calls	$2,_machinecheck
	popr	$SAVREG		# returned -> recovered
	addl2	(sp)+,sp
	rei

#
# kernel stack not valid.  give up.
#
	.globl	Xkspnotval
	.align	2
Xkspnotval:
	pushr	$SAVREG
	mfpr	$IPL,-(sp)
	mtpr	$HIGH,$IPL
	pushab	1f
	calls	$1,_panic

1:	.asciz	"ksp invalid"

#
# power failure
# eventually, do something smarter
#
	.globl	Xpowfail
	.align	2
Xpowfail:
	pushr	$SAVREG		# why not?
	pushab	1f
	calls	$1,_printf	# doubtless won't finish
0:	brb	0b		# but if it does

1:	.asciz	"power\n"

#
# mysterious change mode instructions
#
	.globl	Xchmx
	.align	2
Xchmx:			# chm[use]
	pushr	$SAVREG
	pushab	1f
	calls	$1,_panic
1:
	.asciz	"chm?"

#
# `soft' clock interrupt;
# software level 8
#
	.globl	Xsoftclock
	.align	2
Xsoftclock:
	pushr	$SAVREG
	pushl	4+NRS*4(sp)
	pushl	4+NRS*4(sp)
	calls	$2,_softclock		# softclock(pc,psl)
	popr	$SAVREG 
	rei

#
# console device interrupts
# should perhaps be better hidden
#

	.globl	Xcnrint
	.align	2
Xcnrint:
	pushr	$SAVREG
	clrl	-(sp)
	calls	$1,_cnrint
	popr	$SAVREG
	rei

	.globl	Xcnxint
	.align	2
Xcnxint:
	pushr	$SAVREG
	clrl	-(sp)
	calls	$1,_cnxint
	popr	$SAVREG
	rei

#
# various traps that go through trap.c
# identified by a trap type, stored here,
# and sometimes by a code
#

#
# software level 2:
# reschedule trap, profiling
#
	.globl	Xastflt
	.align	2
Xastflt:
	pushl	$0
	pushl	$ASTFLT
	jbr	alltraps

#
# software level 3:
# stream queue scheduling
#
	.globl	Xqsched
	.align	2
Xqsched:
	pushr	$SAVREG
	calls	$0,_queuerun
	popr	$SAVREG
	rei

#
# privileged instruction fault
#
	.globl	Xprivinflt
	.align	2
Xprivinflt:
	pushl	$0
	pushl	$PRIVINFLT
	jbr	alltraps

#
# xfc instruction fault
#
	.globl	Xxfcflt
	.align	2
Xxfcflt:
	pushl	$0
	pushl	$XFCFLT
	jbr	alltraps

#
# reserved operand fault
# references to I/O space sometimes cause this,
# hence the nofault
#
	.globl	Xresopflt
	.align	2
Xresopflt:
	tstl	_nofault
	jneq	donofault
	pushl	$0
	pushl	$RESOPFLT
	jbr	alltraps

#
# reserved addressing mode fault
#
	.globl	Xresadflt
	.align	2
Xresadflt:
	pushl	$0
	pushl	$RESADFLT
	jbr	alltraps

#
# breakpoint instruction
#
	.globl	Xbptflt
	.align	2
Xbptflt:
	pushl	$0
	pushl	$BPTFLT
	jbr	alltraps

#
# compatibility mode fault
# code is some fault type
#
	.globl	Xcompatflt
	.align	2
Xcompatflt:
	pushl	$COMPATFLT
	jbr	alltraps

#
# trace bit in psl trap
#
	.globl	Xtracep
	.align	2
Xtracep:
	pushl	$0
	pushl	$TRCTRAP
	jbr	alltraps

#
# arithmetic exception
# code is some description of the fault
#
	.globl	Xarithtrap
	.align	2
Xarithtrap:
	pushl	$ARITHTRAP
	jbr	alltraps

#
# access violation trap
# code is the faulty address
# silly pdp-11 compatibility:
# distinguish between protection faults
# and segmentation violations
#
	.globl	Xprotflt
	.align	2
Xprotflt:
	tstl	_nofault
	beql	1f
	addl2	$8,sp
	jbr	donofault
1:
	blbs	(sp)+,segflt
	pushl	$PROTFLT
	jbr	alltraps

segflt:
	pushl	$SEGFLT
	jbr	alltraps

#
# translation not valid == page fault
# code is the invalid address
# distinguish between ordinary faults
# and page table not valid faults
#
	.globl	Xtransflt
	.align	2
Xtransflt:
	bitl	$1,(sp)+
	bnequ	tableflt
	pushl	$PAGEFLT
	jbr	alltraps

tableflt:
	pushl	$TABLEFLT
#	jbr	alltraps

#
# here one way or another for any call to trap
# code, type already on the stack
#

alltraps:
	mfpr	$USP,-(sp)
	calls	$0,_trap
	mtpr	(sp)+,$USP
	addl2	$8,sp			# pop type, code
	mtpr	$HIGH,$IPL		## dont go to a higher IPL (GROT)
	rei

#
# here on chmk == syscall
# separate from ordinary trap code for speed
#
	.globl	Xsyscall
	.align	2
Xsyscall:
	tstl	-(sp)			# loathsome nonsense
	mfpr	$USP,-(sp)
	calls	$0,_syscall
	mtpr	(sp)+,$USP
	addl2	$8,sp			# pop code and loathsome nonsense
##	mtpr	$HIGH,$IPL		## dont go to a higher IPL (GROT)
	rei

#
# here to return from a trap recovered by nofault
#
donofault:
	movl	_nofault,(sp)
	bicl2	$PSL_FPD,4(sp)
	rei

#
# do the nofault trick from within a device interrupt
# this is a bit unhealthy
# from within xxNint (not from deeper in the call stack),
#	cknofault()
# will arrange for the interrupt to return to the nofault location,
# and return nonzero if there was such a place
#
# assume caller's stack frame looks like
#	fp/	0
#		4 words of calls-saved junk
#		unknown number of saved registers
#	ap/	count
#		count words of argument, probably just one
#		NRS words of saved registers
#		saved pc
#		saved ps
#

	.globl	_cknofault
_cknofault:
	.word	0
	movl	_nofault,r0
	beql	9f		# no nofault in progress
	movl	8(fp),r1	# arg pointer of caller
	movzbl	(r1),r2		# length of arg list
	mull2	$4,r2		# in words
	addl2	$(NRS+1)*4,r2	# + our saved regs + arg count word
	addl2	r2,r1
	movl	r0,(r1)		# overlay return PC
	bicl2	$PSL_FPD,4(r1)	# in case of restartable instr
9:	ret
