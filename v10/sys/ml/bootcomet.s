#
# primitives to stop the machine
# with and without rebooting
# comet (750) version
#

#
# death stops the machine cold, without rebooting
# we're not allowed to halt because it's hard to guarantee
# that we'll really halt, and not reboot or restart
#

	.globl	_death
_death:	.word	0x0
	mtpr	$HIGH,$IPL
	pushab	deathmsg
	calls	$1,_printf
dl:	brb	dl

deathmsg:
	.asciz	"death\n\n"

#
# reboot restarts the machine, if possible
# we ask for an auto-restart
# which will take a crash dump and then boot
# registers saved here because comet may spoil them otherwise
#

	.globl	_reboot
_reboot: .word	0x0
	clrl	r0
rb0:	aobleq	$1000000,r0,rb0	# about 1 sec delay, to let disk finish
	mtpr	$HIGH,$IPL	# from here on, no interruptions!
	movl	sp,rebsp	# save current stack
	moval	rebsp,sp	# get ptr to register save place
	pushr	$0x3fff		# save the registers there
	movl	rebsp,sp	# and get the real stack back
	mfpr	$PCBB,rebpcb
	jbr	doadump

	.data
	.globl	rebregs
	.globl	rebsp
	.globl	rebpcb
rebregs:
	.space	14*4		# place to store registers from crash
rebsp:	.space	4
rebpcb:	.space	4		# place for pcbb at crash

	.text

#
# place to call on auto-restart,
# from the restart parameter block
#

	.globl	restart
	.align	2
restart:
	bbss	$0,_rpb+12,0f		# console should do this, but doesn't
0:
#	cmpl	$PFMAGIC,pfmagic	# power fail?
#	bneq	doadump		# no
#	 clrl	_rpb+12		# yes: flag further restarts ok
#	 jmp	Xpowrec		# and do power recovery

#
# not a power fail
# take a crash dump and reboot
# registers have already been saved (above)
#
# crash dump routine is called as
#	dump(dev, low, size)
#

	.globl	doadump
doadump:
	mtpr	$HIGH,$IPL	# no interrupts
	mtpr	$0,$MAPEN	# turn off memory management
	movab	dumpstack,sp	# set up a clean stack
	pushab	dumping
	calls	$1,_printf
#	pushl	_dumpsize
#	pushl	_dumplow
#	pushl	_dumpdev
#	calls	$3,*_dumprout
	clrl	r5		# boot flags; magic: r5 undisturbed in setboot
	calls	$0,_setboot
	halt

dumping: .asciz	"dumping core\n"

	.data
	.align	2
	.globl	dumpstack
	.space	58*4			# separate stack for dumps
dumpstack: 
	.space	4
