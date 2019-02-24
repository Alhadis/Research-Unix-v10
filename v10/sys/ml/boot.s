#
# primitives to stop the machine
# with and without rebooting
# big vax version
#
# subtleties:
# the crash dump is written after a `warm restart',
# so it will happen if the system mysteriously halts.
# but registers must be saved before the restart,
# as restarts often destroy the registers.
# but they must be saved after the restart too if
# the system just halted.
# hence some of the tangle below.
#
# the flow:
# if we are supposed to reboot:
#	save the registers
#	cause a warm restart
# when we restart:
#	if this wasn't a power failure,
#	save registers if not already saved,
#	write a crash dump,
#	reboot.
#
	.text

#
# reboot restarts the machine, if possible
# we ask for an auto-restart
# which will take a crash dump and then boot
#
# if the argument is nonzero, and it seems safe, call update first
#

	.globl	_reboot
_reboot: .word	0x0
	mfpr	$IPL,rebipl	# merely for debugging
	tstl	4(ap)
	beql	1f		# don't want to sync
	movpsl	r0
	bbs	$PSLB_IS,r0,1f	# or on interrupt stack, so shouldn't
	mtpr	$0,$IPL
	calls	$0,_update
	mull3	$1000000,_delayfact,r0	# about 1 sec delay to let update finish
0:	sobgtr	r0,0b
1:	mtpr	$HIGH,$IPL	# from here on, no interruptions!
	bbss	$0,gotregs,1f
	movl	sp,rebsp	# save current stack
	moval	rebsp,sp	# get ptr to register save place
	pushr	$0x3fff		# save the registers there
	movl	rebsp,sp	# and get the real stack back
	mfpr	$PCBB,rebpcb
1:
	mtpr	$0,$MAPEN	# else comet can't find rpb!
	halt			# whereupon we shall restart

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
# here if started at 0x10 -> dump core
#
	.globl	doadump
doadump:
	mtpr	$HIGH,$IPL	# from here on, no interruptions!
	bbss	$0,gotregs,1f
	movl	sp,rebsp	# save current stack
	moval	rebsp,sp	# get ptr to register save place
	pushr	$0x3fff		# save the registers there
	movl	rebsp,sp	# and get the real stack back
	mfpr	$PCBB,rebpcb
	mfpr	$MAPEN,rebmapen
1:
	mtpr	$0,$MAPEN	# turn off memory management
	movab	dumpstack,sp	# set up a clean stack
	pushab	dumping
	calls	$1,_printf
	movl	_time,dumptime
	movl	_dumpsize,dumplen
	cmpl	dumplen,_physmem
	bleq	1f
	 movl	_physmem,dumplen
1:	movl	$DUMPMAGIC,dumpmagic
	pushl	_dumpsize
	pushl	_dumplow
	pushl	_dumpunit
	calls	$3,*_dumprout
	tstl	r0
	beql	2f
	 pushab	dumpfailed
	 calls	$1,_printf
2:
	clrl	r5		# boot flags; magic: r5 undisturbed in setboot
	calls	$0,_setboot
	halt

dumping: .asciz	"dumping core\n"
dumpfailed: .asciz "failed\n";

	.data
	.align	2
	.globl	dumpstack
	.globl	rebregs
	.globl	rebsp
	.globl	rebpcb
	.space	128*4			# separate stack for dumps
dumpstack: 
	.space	4
rebregs:
	.space	14*4		# place to store registers from crash
rebsp:	.space	4
rebpcb:	.space	4		# place for pcbb at crash
rebipl:	.space	4		# ipl at reboot
rebmapen: .space 4		# mapen at reboot
gotregs: .space	4
