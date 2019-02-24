	.set	CONTROL_STATE, 4516
	.text
	.align	8
	.globl	P
	.globl	Sys
P:
Sys:
	# start of Sys
	.word	0	# this is really P == Sys[0]
	.word	defont
	.word	add
	.word	Uaddr
	.word	alarm
	.word	Ualloc
	.word	Uballoc
	.word	bfree
	.word	Ubitblt
	.word	Ucursallow
	.word	Ucursinhibit
	.word	Ucursset
	.word	Ucursswitch
	.word	debug
	.word	dellayer
	.word	div
	.word	eqrect
	.word	Uexit
	.word	free
	.word	realgcalloc
	.word	gcfree
	.word	Uogetrect
	.word	inset
	.word	cursallow
	.word	cursinhibit
	.word	Ukbdchar
	.word	lpoint
	.word	lrectf
	.word	lsegment
	.word	ltexture
	.word	menuhit
	.word	mul
	.word	muxnewwind
	.word	nap
	.word	newlayer
	.word	Unewproc
	.word	Unewwindow
	.word	Uown
	.word	Upoint
	.word	ptinrect
	.word	raddp
	.word	Urcvchar
	.word	realtime
	.word	rectXrect
	.word	rectclip
	.word	Urectf
	.word	Urequest
	.word	rsubp
	.word	Uscreenswap
	.word	Usegment
	.word	Usendchar
	.word	sendwithdelim
	.word	sleep
	.word	0
	.word	string
	.word	strwidth
	.word	sub
	.word	Utexture
	.word	tolayer
	.word	upfront
	.word	Uwait
	.word	muxkill
	.word	setmuxbuf
	.word	getmuxbuf
	.word	insure
	.word	GCalloc
	.word	movstring
	.word	insstring
	.word	delstring
	.word	frselect
	.word	fralloc
	.word	frsetrects
	.word	frinit
	.word	frfree
	.word	ptofchar
	.word	charofpt
	.word	border
	.word	getproctab
	.word	lbitblt
	.word	getrect
	.word	getrectb
	.word	qputc
	.word	qgetc
	.word	qputstr
	.word	qclear
	.word	jstrwidth
	.word	getproc
	.word	layerop
	.word	putname
	.word	getname
	.word	qpeekc
	# end of Sys
	.globl	_start
	.align	8
_start:
	call	&0,main
	RETPS

	.text
	.align	8
	.globl	msvid_int
msvid_int:
	SAVE	%r8
	MOVB	0x200013,%r5
	BITB	&0x40,%r5
	BEB	mous_chk
	BITB	&0x04,%r5	# update only at 60 hz. rate
	BNEB	mous_chk	# if fall through, have	a video	interrupt
	call	&0,auto1		# calls	cursor update in cursor.c
	call	&0,clockroutine
mous_chk:
	BITB	&0xb0,%r5
	BEB	msvid_exit
				# if fall through, have	a mouse	interrupt
	call	&0,auto4		# call buttons handler in buttons.c
	orw2	&1,proctab+CONTROL_STATE
msvid_exit:
	RESTORE	%r8
	RETPS

	.text
	.align	8
	.globl	key_int
key_int:
	call	&0,auto2
	RETPS

	.text
	.align	8
	.globl	host_int
host_int:
	pushw	&queues
	andw3	0x20000c,&255,%r0
	pushw	%r0
	call	&2,qputc
	orw2	&1,proctab+80
	RETPS

	.text
	.align	8
	.globl	out_int
out_int:
	call	&0,aciatrint
	RETPS

	.text
	.align	8
	.globl	pioh_int
pioh_int:
	TSTB	0x300003
	RETPS

	.text
	.align	8
	.globl	piol_int
piol_int:
	TSTB	0x300003
	RETPS

	.data
	.globl	ticks0
	.bss	ticks0,4,4

	.text
	.align	8
	.globl	realtime
realtime: movw	ticks0,%r0
	RET

#	.globl	reboot
#reboot:
#exit:
#	movaw	reset_pcb,%r0			# when return then reboot
#	CALLPS				# reboot the entire system
#	jmp	reboot			#  boo-boo if you get here

	.globl	callps
	.align	8
callps:
	movw	0(%ap), %r0
	CALLPS
	RET

	.globl	resume
	.align	8
resume:
	movw	0(%ap), -4(%isp)
	RETPS

	.globl	retps
	.align	8
retps:
	RETPS

	.globl	spl0
	.align	8
spl0:	movw	%psw,%r0
	ANDW2	&0xfffe1fff,%psw	# turn off all priority bits
	TSTW	%r0
	RET

	.globl	spl1
	.globl	spl4
	.align	8
spl1:
spl4:	movw	%psw,%r0
	INSFW	&4,&13,&14,%psw
	TSTW	%r0
	RET

	.globl	spl5
	.globl	spl6
	.globl	spl7
	.align	8
spl5:
spl6:
spl7:	movw	%psw,%r0
	ORW2	&0x1e000,%psw
	TSTW	%r0
	RET
	.globl	splx
	.align	8
splx:
	movw	%psw,%r0
	LRSW3	&13,0(%ap),%r1
	INSFW	&4,&13,%r1,%psw
	TSTW	%r0
	RET

	.globl	excep_norm
	.align	8
excep_norm:
	ORW2	&0x281e000,%psw	# IPL up to 15
	TSTW	%r0		# let psw bits settle 
	PUSHW	%r0		# save temporary registers 0-2
	PUSHW	%r1
	PUSHW	%r2
	PUSHW	%r3		# save register variables 3-8
	PUSHW	%r4
	PUSHW	%r5
	PUSHW	%r6
	PUSHW	%r7
	PUSHW	%r8
	PUSHW	%r9		# save fp
	PUSHW	%r10		# save ap
	PUSHW	-48(%sp)	# save psw
	PUSHAW	-56(%sp)	# save sp's value at interrupt
	PUSHW	&3		# type = NORMAL
	PUSHAW	-64(%sp)	# address of pc from normal gate
	PUSHAW	-64(%sp)	# address of psw from normal gate
	PUSHW	-68(%sp)	# psw from normal gate
	PUSHW	%pcbp		# (not used!)
	call	&5,trap		# trap will decide to return or not
	POPW	%r0		# ignore saved sp/psw
	POPW	%r0
	POPW	%r10		# restore ap/fp
	POPW	%r9
	POPW	%r8		# restore register variables 3-8
	POPW	%r7
	POPW	%r6
	POPW	%r5
	POPW	%r4
	POPW	%r3
	POPW	%r2		# restore temporary registers 0-2
	POPW	%r1
	POPW	%r0
	RETG			# resume if trap allows (brkpt or trace)

	.globl	excep_stack
	.align	8
excep_stack:
	MOVW	-0x4(%isp),%r0	# get saved %pcbp 
	PUSHW	&2		# type = STACK
	PUSHAW	0x4(%r0)	# push address of saved pc 
	PUSHAW	0x0(%r0)	# push address of saved psw 
	PUSHW	0x0(%r0)	# push saved psw 
	PUSHW	%r0		# push saved pcbp 
	call	&5,trap		# let trap handle the situation
	RETPS			# if trap returns
	.globl	excep_int
	.globl	excep_proc
	.align	8
excep_int:
excep_proc:
	MOVW	-0x4(%isp),%r0	# get saved %pcbp 
	PUSHW	&1		# type = PROCESS
	PUSHAW	0x4(%r0)	# push address of saved pc 
	PUSHAW	0x0(%r0)	# push address of saved psw 
	PUSHW	0x0(%r0)	# push saved psw 
	PUSHW	%r0		# push saved pcbp 
	call	&5,trap		# let trap handle the situation
	RETPS			# if trap returns

#	hacks
	.globl	ringbell
	.globl	test32
	.globl	pfkey
	.globl	curse
	.globl	pt
	.align	8
ringbell:
test32:
pfkey:
curse:
pt:
	RET

	.data
	.comm	blocked,4
	.comm	ublocked,4
	.comm	cur,4
