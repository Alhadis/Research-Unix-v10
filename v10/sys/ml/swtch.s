/*
 * The following primitives use the fancy VAX instructions
 * much like VMS does.  _whichqs tells which of the 32 queues _qs
 * have processes in them.  Setrq puts processes into queues, Remrq
 * removes them from queues.  The running process is on no queue,
 * other processes are on a queue related to p->p_pri, divided by 4
 * actually to shrink the 0-127 range of priorities into the 32 available
 * queues.
 */
	.comm	_noproc,4
	.comm	_runrun,4
	.text

/*
 * Setrq(p), using fancy VAX instructions.
 *
 * Call should be made at spl6(), and p->p_stat should be SRUN
 */
	.globl	_Setrq		# <<<massaged to jsb by "asm.sed">>>
_Setrq:
	tstl	P_RLINK(r0)		## firewall: p->p_rlink must be 0
	beql	set1			##
	pushab	set3			##
	calls	$1,_panic		##
set1:
	bitl	$SPROCIO,P_FLAG(r0)	# if he's getting PROCIO'd,
	bneq	set2			# we leave him alone
	movzbl	P_PRI(r0),r1		# put on queue which is p->p_pri / 4
	ashl	$-2,r1,r1
	movaq	_qs[r1],r2
	insque	(r0),*4(r2)		# at end of queue
	bbss	r1,_whichqs,set2	# mark queue non-empty
set2:
	rsb

set3:	.asciz	"setrq"

/*
 * Remrq(p), using fancy VAX instructions
 *
 * Call should be made at spl6().
 */
	.globl	_Remrq		# <<<massaged to jsb by "asm.sed">>>
_Remrq:
	bitl	$SPROCIO,P_FLAG(r0)	# if he's getting PROCIO'd,
	bneq	rem2a			# we leave him alone
	movzbl	P_PRI(r0),r1
	ashl	$-2,r1,r1
	bbsc	r1,_whichqs,rem1
	pushab	rem3			# it wasn't recorded to be on its q
	calls	$1,_panic
rem1:
	remque	(r0),r2
	beql	rem2
	bbss	r1,_whichqs,rem2
rem2:
	clrl	P_RLINK(r0)		## for firewall checking
rem2a:
	rsb

rem3:	.asciz	"remrq"

/*
 * Masterpaddr is the p->p_addr of the running process on the master
 * processor.  When a multiprocessor system, the slave processors will have
 * an array of slavepaddr's.
 */
	.globl	_masterpaddr
	.data
_masterpaddr:
	.long	0

	.text
sw0:	.asciz	"swtch"
/*
 * Swtch(), using fancy VAX instructions
 */
	.globl	_Swtch
_Swtch:				# <<<massaged to jsb by "asm.sed">>>
	movl	$1,_noproc
	clrl	_runrun
sw1:	ffs	$0,$32,_whichqs,r0	# look for non-empty queue
	bneq	sw1a
	mtpr	$0,$IPL			# must allow interrupts here
	brw	sw1			# this is an idle loop!
sw1a:	mtpr	$0x18,$IPL		# lock out all so _whichqs==_qs
	bbcc	r0,_whichqs,sw1		# proc moved via lbolt interrupt
	movaq	_qs[r0],r1
	remque	*(r1),r2		# r2 = p = highest pri process
	bvc	sw2			# make sure something was there
sw1b:	pushab	sw0
	calls	$1,_panic
sw2:	beql	sw3
	insv	$1,r0,$1,_whichqs	# still more procs in this queue
sw3:
	clrl	_noproc
	movzbl	P_STAT(r2),r3		## firewalls
	cmpl	$SRUN,r3		##
	bneq	sw1b			##
	clrl	P_RLINK(r2)		##
	movl	*P_ADDR(r2),r0
	movl	r0,_masterpaddr
	ashl	$PGSHIFT,r0,r0		# r0 = pcbb(p)
#	mfpr	$PCBB,r1		# resume of current proc is easy
#	cmpl	r0,r1
#	beql	res0
#	incl	_cnt+V_SWTCH
/* fall into... */

/*
 * Resume(pf)
 */
	.globl	_Resume		# <<<massaged to jsb by "asm.sed">>>
_Resume:
	mtpr	$0x18,$IPL			# no interrupts, please
	pushl	_nofault
	pushl	_CMAP2		# yech
	svpctx
	mtpr	r0,$PCBB
	ldpctx
	movl	(sp)+,_CMAP2	# yech
	mtpr	$_CADDR2,$TBIS
	movl	(sp)+,_nofault
res0:
	tstl	_u+PCB_SSWAP
	bneq	res1
	rei

res1:
	movl	_u+PCB_SSWAP,r0			# longjmp to saved context
	clrl	_u+PCB_SSWAP
	movq	(r0)+,r6
	movq	(r0)+,r8
	movq	(r0)+,r10
	movq	(r0)+,r12
	movl	(r0)+,r1
	cmpl	r1,sp				# must be a pop
	bgequ	1f
	pushab	2f
	calls	$1,_panic
	/* NOTREACHED */
1:
	movl	r1,sp
	movl	(r0),(sp)			# address to return to
	movl	$PSL_PRVMOD,4(sp)		# ``cheating'' (jfr)
	rei

2:	.asciz	"ldctx"
