#
# fake boot ROM for MicroVAX, UQSSP/MSCP disk
# read boot block into memory,
# and call it with parameters pointing to and used by
# a device driver
#
# the MicroVAX ROM actually has horrible VMB in it.
# this code is stuck in block 0,
# with a header VMB will recognize.
# the boot block is put in block 1.
# VMB reads block 0;
# the header tells it to read block 0 (sic);
# code packaged in uvhdr.s relocates us to the
# standard ROM address, moves VMB's parameters into
# our standard places, and calls us.
#

#
# KDA50 defs
#
	.set	IP,0x0		# ip register
	.set	SA,0x2		# sa register

	.set	STEP0,10		# STEP1 == bit 11

	.set	BPOWN,31		# bit 31 == port owns descriptor
	.set	RTOC,4

#
# mscp defs
# packet offsets have 4 added to account for UQSSP header (mostly length)
#

	.set	m_crf,4			# command ref number
	.set	m_unit,8		# disk unit number
	.set	m_opcd,12		# opcode
	.set	m_sts,14		# status (word)
	.set	RPKSIZE,16		# as much as we want on rcv
	.set	m_bcnt,16		# byte count
	.set	SOSIZE,20		# as much as we send for ONLINE
	.set	m_buff,20		# buffer address
	.set	m_lbn,32		# block number
	.set	SRSIZE,36		# as much as we send to read
	.set	SOSIZE,36		# as much as we send for ONLINE

	.set	OPONL,011		# unit online
	.set	OPRD,041		# read block

#
# MicroVAX Q-bus defs
#
	.set	LMEAE,040		# bit to allow DMA from Q-bus
	.set	ICR,0x20001f40		# where to set it

#
# boot ROM conventions
#
	.set	WMEM,0x200		# sp == some working memory + WMEM
					# usually working memory == 0
	.set	BSIZE,512		# size of a block
	.set	BBLOCK,1		# boot LBN

ourstack:
#
# here to start off, at 0xfa02
# registers:
#	r2	device register address
#	r3	unit number
#	r5	boot flags; ignored but preserved
#	sp	working memory + WMEM
#
# registers to save: r0 r1 r3 r5 r10 r11 ap sp
#
rombegin:
	movl	sp,savesp
	movab	ourstack,sp

#
# init the KDA50
#

	bisw2	$LMEAE,*$ICR	# enable dma
	clrw	IP(r2)		# reset: kick into startup
0:	movw	SA(r2),r7
	blss	oops
	bbc	$STEP0+1,r7,0b	# wait for step1
	movw	$0x8000,SA(r2)	# no vector, ring size (2^0,2^0)
0:	movw	SA(r2),r7
	blss	oops
	bbc	$STEP0+2,r7,0b	# wait for step2
	movab	rsp,r6		# ring address
	movw	r6,SA(r2)
0:	movw	SA(r2),r7
	blss	oops
	bbc	$STEP0+3,r7,0b	# wait for step3
	ashl	$-16,r6,r6
	movw	r6,SA(r2)
0:	movw	SA(r2),r7
	blss	oops
	bbc	$STEP0+4,r7,0b	# wait for step4
	movw	$1,SA(r2)	# GO

#
# read boot block into bottom of memory
#

	pushl	r0		# rblk eats it
	movab	rblk,r6		# set up device driver address
	subl3	$WMEM,savesp,-(sp)	# physical addr
# trick: we know driver doesn't use r5, so we needn't save boot flags
	movl	$BBLOCK,r8
	jsb	(r6)
	blbc	r0,oops
	tstl	(sp)+
	movl	(sp)+,r0
#
# start boot block
# registers now:
#	r0	device type (unchanged)
#	r1	BI node number (unchanged)
#	r2	device register addr
#	r3	device unit number (unchanged)
#	r5	boot flags (unchanged)
#	r6	address of our device driver
#	sp	`working memory + 0x200' (unchanged)
#
	movl	savesp,sp
	jmp	0xc-WMEM(sp)

#
# here when something is broken
#
oops:	halt

#
# device driver; called above and by boot block and friends
#
#	r2	device registers
#	r3	device unit number
#	r5	buffer address, relative to `working memory' (ignored)
#	4(sp)	physical buffer address; we use that
#	r8	block number
#
# do not destroy r1-r6 r10 r11 ap
# on return, low bit of r0 set if we read it, clear if we failed
#

rblk:
	movq	r10,-(sp)
	clrl	r0			# assume failure
	movab	cp,r10
	movab	rp,r11
	movab	rsp,r9
	movzbl	$OPRD,m_opcd(r10)	# make READ packet
	movl	4+8(sp),m_buff(r10)	# unmapped address
	movzwl	$BSIZE,m_bcnt(r10)
	movl	r8,m_lbn(r10)
	movzbl	$SRSIZE,(r10)
	bsbb	sendrcv
	beql	rdone
	movl	r3,m_unit(r10)		# didn't work; make ONLINE packet
	movzbl	$OPONL,m_opcd(r10)
	movzbl	$SOSIZE,(r10)
	clrl	m_bcnt(r10)		# ugh
	clrl	m_buff(r10)		# ugh
	clrl	m_lbn(r10)		# ugh
	bsbb	sendrcv
	bneq	rfail
	movzbl	$OPRD,m_opcd(r10)	# and try read again
	movl	4+8(sp),m_buff(r10)
	movzwl	$BSIZE,m_bcnt(r10)
	movl	r8,m_lbn(r10)
	movzbl	$SRSIZE,(r10)
	bsbb	sendrcv
	bneq	rfail
rdone:	incl	r0			# success
rfail:	movq	(sp)+,r10
	rsb

sendrcv:
	movzbl	$RPKSIZE,(r11)
	movab	m_crf(r10),RTOC(r9)
	movab	m_crf(r11),(r9)
	bbss	$BPOWN,(r9),0f		# make response buffer available
0:	bbss	$BPOWN,RTOC(r9),0f	# send the ONLINE packet
0:	movw	IP(r2),r7		# kick to make it go
0:	bbs	$BPOWN,(r9),0b		# wait for answer
	tstb	m_sts(r11)		# tstb: ignore `already online' flag
	rsb

#
# KDB50 communication area
#
	.align	2
savesp:	.space	4		# working mem + WMEM
	.space	8		# crap at the top
rsp:	.space	4		# response descriptor
	.space	4		# command descriptor

#
# a pair of packets
# +4 for length
#

cp:	.space	SRSIZE+4	# command packet
rp:	.space	RPKSIZE+4	# response packet
