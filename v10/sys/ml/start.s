#
# initialization
#
# ipl 0x1f; mapen 0
# nothing much set up
#

	.data
	.globl	eintstack
intstack:
	.space	NISP*NBPG
eintstack:

	.text

	.globl	start
start:
	.word	0
	movl	r11,bootflags	# save boot flags; wrong, should be r5
	mtpr	$0,$ICCS	# stop the clock
	subl3	$KA,$scb,r0	# init system control block
	mtpr	r0,$SCBB
	moval	*$eintstack,sp	# absolute addr so kernel bit is set
#
# miscellaneous hacks:
# fix up rpb, esp checksum
# silly speed hacks
#
	movab	_rpb,r0
	bbcc	$31,4(r0),0f	# make restart addr physical (sigh)
	movl	4(r0),r1
0:	movl	$0x1f,r2
	clrl	r3
1:	addl2	(r1)+,r3
	sobgtr	r2,1b
	movl	r3,8(r0)
	clrl	12(r0)		# so a restart can happen
	bisw2	$0x0fff,_trap		# speed hack, known in trap
	bisw2	$0x0fff,_syscall	# speed hack, known in syscall
#
# clear bss
#
	movab	_edata,r1
	movab	_end,r2
1:	clrq	(r1)+
	cmpl	r1,r2
	blss	1b
#
# count up memory into r0
#
	movab	9f,_nofault
	clrl	r0
1:	tstl	(r0)
	acbl	$MAXPHMEM-1,$64*1024,r0,1b
9:	clrl	_nofault
#
# write the system page table, set SBR and SLR
#
	pushl	r0
	calls	$1,_machmem	# some vaxes have ideas about top of mem
	pushl	r0
	calls	$1,_mmapinit
#
# hand-craft the bare bones of proc[0]
# set up page table, initial pcb, allocate user block
#
	pushab	*$1f		# first kernel pc; abs addr so kernel bit set
	calls	$1,_uctinit
#
# turn on memory mapping
# P0BR dance gives us time to change the pc
#
	mtpr	_Sysmap,$P0BR
	mfpr	$SLR,r0
	mtpr	r0,$P0LR
	mtpr	$0,$TBIA
	mtpr	$1,$MAPEN
	jmp	*$0f; 0:	# load kernel address into pc
#
# use the pcb: go to kernel mode, kernel stack
#
	ldpctx
	rei
1:
#
# any other hardware-dependent initialization
#
	calls	$0,_machinit
#
# start unix
#
	calls	$0,_main
#
# return here as process 1: run icode
#
	pushl	$0x03c00000	# mode (user,user)
	pushl	$0		# pc 0
	rei
